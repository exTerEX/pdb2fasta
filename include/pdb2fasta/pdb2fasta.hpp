#ifndef PDB2FASTA_HPP
#define PDB2FASTA_HPP

#include "types.hpp"
#include "parser.hpp"
#include "converter.hpp"

namespace pdb2fasta {

// Convenience function for simple conversions
inline std::string convert(const std::string& filename) {
    Converter converter;
    return converter.convert_file(filename);
}

inline std::string convert_string(const std::string& content, FileFormat format) {
    Converter converter;
    return converter.convert_string(content, format);
}

} // namespace pdb2fasta

#endif // PDB2FASTA_HPP
