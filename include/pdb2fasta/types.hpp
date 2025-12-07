#ifndef PDB2FASTA_TYPES_HPP
#define PDB2FASTA_TYPES_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace pdb2fasta {

struct Residue {
    std::string name;
    int sequence_number;
    char insertion_code;
};

struct Chain {
    char id;
    std::vector<Residue> residues;
};

struct Structure {
    std::string name;
    std::vector<Chain> chains;
};

// Three-letter to one-letter amino acid code mapping
inline const std::unordered_map<std::string, char>& get_residue_map() {
    static const std::unordered_map<std::string, char> map = {
        {"ALA", 'A'}, {"ARG", 'R'}, {"ASN", 'N'}, {"ASP", 'D'},
        {"CYS", 'C'}, {"GLN", 'Q'}, {"GLU", 'E'}, {"GLY", 'G'},
        {"HIS", 'H'}, {"ILE", 'I'}, {"LEU", 'L'}, {"LYS", 'K'},
        {"MET", 'M'}, {"PHE", 'F'}, {"PRO", 'P'}, {"SER", 'S'},
        {"THR", 'T'}, {"TRP", 'W'}, {"TYR", 'Y'}, {"VAL", 'V'},
        // Non-standard amino acids
        {"MSE", 'M'}, {"SEC", 'U'}, {"PYL", 'O'},
        // DNA
        {"DA", 'A'}, {"DC", 'C'}, {"DG", 'G'}, {"DT", 'T'},
        // RNA
        {"A", 'A'}, {"C", 'C'}, {"G", 'G'}, {"U", 'U'}
    };
    return map;
}

inline char residue_to_one_letter(const std::string& three_letter) {
    const auto& map = get_residue_map();
    auto it = map.find(three_letter);
    return (it != map.end()) ? it->second : 'X';
}

} // namespace pdb2fasta

#endif // PDB2FASTA_TYPES_HPP
