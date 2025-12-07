#include "pdb2fasta/converter.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace pdb2fasta {

Converter::Converter(ConversionOptions options)
    : options_(std::move(options)) {}

std::string Converter::to_fasta(const Structure& structure) const {
    std::ostringstream result;
    
    for (const auto& chain : structure.chains) {
        if (chain.residues.empty()) continue;
        
        // Write header
        result << ">";
        if (!structure.name.empty()) {
            result << structure.name;
        }
        if (options_.include_chain_id) {
            if (!structure.name.empty()) result << "_";
            result << chain.id;
        }
        result << "\n";
        
        // Write sequence
        std::string sequence = chain_to_sequence(chain);
        result << wrap_sequence(sequence) << "\n";
    }
    
    return result.str();
}

std::string Converter::convert_file(const std::string& filename, FileFormat format) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    FileFormat actual_format = (format == FileFormat::AUTO) ? detect_format(filename) : format;
    auto parser = create_parser(actual_format);
    
    Structure structure = parser->parse(file);
    
    // Extract name from filename
    size_t last_slash = filename.find_last_of("/\\");
    size_t start = (last_slash == std::string::npos) ? 0 : last_slash + 1;
    size_t dot = filename.find_last_of('.');
    if (dot != std::string::npos && dot > start) {
        structure.name = filename.substr(start, dot - start);
    } else {
        structure.name = filename.substr(start);
    }
    
    return to_fasta(structure);
}

std::string Converter::convert_string(const std::string& content, FileFormat format) const {
    FileFormat actual_format = (format == FileFormat::AUTO) ? detect_format_from_content(content) : format;
    auto parser = create_parser(actual_format);
    
    Structure structure = parser->parse(content);
    return to_fasta(structure);
}

std::string Converter::chain_to_sequence(const Chain& chain) const {
    std::string sequence;
    sequence.reserve(chain.residues.size());
    
    for (const auto& residue : chain.residues) {
        char one_letter = residue_to_one_letter(residue.name);
        sequence += (one_letter != 'X') ? one_letter : options_.unknown_residue;
    }
    
    return sequence;
}

std::string Converter::wrap_sequence(const std::string& sequence) const {
    if (options_.line_width <= 0) {
        return sequence;
    }
    
    std::string result;
    result.reserve(sequence.size() + sequence.size() / options_.line_width + 1);
    
    for (size_t i = 0; i < sequence.size(); i += options_.line_width) {
        if (i > 0) result += '\n';
        result += sequence.substr(i, options_.line_width);
    }
    
    return result;
}

} // namespace pdb2fasta
