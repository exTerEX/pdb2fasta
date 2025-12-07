#include "pdb2fasta/parser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace pdb2fasta {

namespace {

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::string to_upper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

} // anonymous namespace

// PDB Parser Implementation
Structure PDBParser::parse(std::istream& input) {
    Structure structure;
    std::string line;
    
    while (std::getline(input, line)) {
        if (line.size() >= 6) {
            std::string record_type = line.substr(0, 6);
            if (record_type == "ATOM  " || record_type == "HETATM") {
                parse_atom_line(line, structure);
            }
        }
    }
    
    return structure;
}

Structure PDBParser::parse(const std::string& content) {
    std::istringstream stream(content);
    return parse(stream);
}

void PDBParser::parse_atom_line(const std::string& line, Structure& structure) {
    if (line.size() < 27) return;
    
    // Only process CA atoms for proteins, or specific atoms for nucleic acids
    std::string atom_name = trim(line.substr(12, 4));
    if (atom_name != "CA" && atom_name != "P") return;
    
    char chain_id = line[21];
    std::string res_name = trim(line.substr(17, 3));
    
    int res_seq = 0;
    try {
        res_seq = std::stoi(trim(line.substr(22, 4)));
    } catch (...) {
        return;
    }
    
    char ins_code = (line.size() > 26) ? line[26] : ' ';
    
    // Find or create chain
    Chain* chain = nullptr;
    for (auto& c : structure.chains) {
        if (c.id == chain_id) {
            chain = &c;
            break;
        }
    }
    
    if (!chain) {
        structure.chains.push_back({chain_id, {}});
        chain = &structure.chains.back();
    }
    
    // Check for duplicate residues
    bool duplicate = false;
    for (const auto& r : chain->residues) {
        if (r.sequence_number == res_seq && r.insertion_code == ins_code) {
            duplicate = true;
            break;
        }
    }
    
    if (!duplicate) {
        chain->residues.push_back({res_name, res_seq, ins_code});
    }
}

// mmCIF Parser Implementation
Structure MMCIFParser::parse(std::istream& input) {
    Structure structure;
    std::vector<std::string> atom_site_lines;
    std::string line;
    bool in_atom_site = false;
    std::vector<std::string> column_names;
    
    while (std::getline(input, line)) {
        line = trim(line);
        
        if (line.empty()) continue;
        
        // Check for atom_site loop
        if (line == "loop_") {
            in_atom_site = false;
            column_names.clear();
            continue;
        }
        
        if (line.rfind("_atom_site.", 0) == 0) {
            in_atom_site = true;
            column_names.push_back(line);
            continue;
        }
        
        if (in_atom_site && !line.empty() && line[0] != '_' && line[0] != '#') {
            if (line == "loop_" || line.rfind("_", 0) == 0) {
                in_atom_site = false;
                continue;
            }
            atom_site_lines.push_back(line);
        }
    }
    
    if (!atom_site_lines.empty()) {
        // Build column index map
        std::unordered_map<std::string, int> col_index;
        for (size_t i = 0; i < column_names.size(); ++i) {
            col_index[column_names[i]] = static_cast<int>(i);
        }
        
        int atom_id_col = col_index.count("_atom_site.label_atom_id") ? col_index["_atom_site.label_atom_id"] : -1;
        int res_name_col = col_index.count("_atom_site.label_comp_id") ? col_index["_atom_site.label_comp_id"] : -1;
        int chain_col = col_index.count("_atom_site.label_asym_id") ? col_index["_atom_site.label_asym_id"] : -1;
        int res_seq_col = col_index.count("_atom_site.label_seq_id") ? col_index["_atom_site.label_seq_id"] : -1;
        int ins_code_col = col_index.count("_atom_site.pdbx_PDB_ins_code") ? col_index["_atom_site.pdbx_PDB_ins_code"] : -1;
        
        std::unordered_set<std::string> seen_residues;
        
        for (const auto& atom_line : atom_site_lines) {
            auto tokens = tokenize_cif_line(atom_line);
            
            if (atom_id_col < 0 || static_cast<size_t>(atom_id_col) >= tokens.size()) continue;
            
            std::string atom_name = tokens[atom_id_col];
            if (atom_name != "CA" && atom_name != "P") continue;
            
            if (chain_col < 0 || static_cast<size_t>(chain_col) >= tokens.size()) continue;
            if (res_name_col < 0 || static_cast<size_t>(res_name_col) >= tokens.size()) continue;
            
            std::string chain_str = tokens[chain_col];
            char chain_id = chain_str.empty() ? ' ' : chain_str[0];
            std::string res_name = tokens[res_name_col];
            
            int res_seq = 0;
            if (res_seq_col >= 0 && static_cast<size_t>(res_seq_col) < tokens.size()) {
                try {
                    if (tokens[res_seq_col] != "." && tokens[res_seq_col] != "?") {
                        res_seq = std::stoi(tokens[res_seq_col]);
                    }
                } catch (...) {}
            }
            
            char ins_code = ' ';
            if (ins_code_col >= 0 && static_cast<size_t>(ins_code_col) < tokens.size()) {
                const auto& ic = tokens[ins_code_col];
                if (!ic.empty() && ic != "." && ic != "?") {
                    ins_code = ic[0];
                }
            }
            
            // Create unique key
            std::string key = chain_str + "_" + std::to_string(res_seq) + "_" + ins_code;
            if (seen_residues.count(key)) continue;
            seen_residues.insert(key);
            
            // Find or create chain
            Chain* chain = nullptr;
            for (auto& c : structure.chains) {
                if (c.id == chain_id) {
                    chain = &c;
                    break;
                }
            }
            
            if (!chain) {
                structure.chains.push_back({chain_id, {}});
                chain = &structure.chains.back();
            }
            
            chain->residues.push_back({res_name, res_seq, ins_code});
        }
    }
    
    return structure;
}

Structure MMCIFParser::parse(const std::string& content) {
    std::istringstream stream(content);
    return parse(stream);
}

std::vector<std::string> MMCIFParser::tokenize_cif_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::string current;
    bool in_quote = false;
    char quote_char = 0;
    
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        
        if (in_quote) {
            if (c == quote_char && (i + 1 >= line.size() || std::isspace(line[i + 1]))) {
                in_quote = false;
                tokens.push_back(current);
                current.clear();
            } else {
                current += c;
            }
        } else if (c == '"' || c == '\'') {
            in_quote = true;
            quote_char = c;
        } else if (std::isspace(c)) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    
    if (!current.empty()) {
        tokens.push_back(current);
    }
    
    return tokens;
}

// Factory functions
std::unique_ptr<Parser> create_parser(FileFormat format) {
    switch (format) {
        case FileFormat::PDB:
            return std::make_unique<PDBParser>();
        case FileFormat::MMCIF:
            return std::make_unique<MMCIFParser>();
        default:
            return std::make_unique<PDBParser>();
    }
}

FileFormat detect_format(const std::string& filename) {
    std::string lower = filename;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    if (lower.find(".cif") != std::string::npos || 
        lower.find(".mmcif") != std::string::npos) {
        return FileFormat::MMCIF;
    }
    return FileFormat::PDB;
}

FileFormat detect_format_from_content(const std::string& content) {
    // Check for mmCIF markers
    if (content.find("_atom_site.") != std::string::npos ||
        content.find("data_") == 0 ||
        content.find("loop_") != std::string::npos) {
        return FileFormat::MMCIF;
    }
    return FileFormat::PDB;
}

} // namespace pdb2fasta
