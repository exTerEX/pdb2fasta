#ifndef PDB2FASTA_CONVERTER_HPP
#define PDB2FASTA_CONVERTER_HPP

#include "types.hpp"
#include "parser.hpp"
#include <string>
#include <vector>

namespace pdb2fasta {

struct ConversionOptions {
    int line_width = 80;
    bool include_chain_id = true;
    char unknown_residue = 'X';
};

class Converter {
public:
    explicit Converter(ConversionOptions options = {});
    
    // Convert structure to FASTA format
    std::string to_fasta(const Structure& structure) const;
    
    // Convert file to FASTA
    std::string convert_file(const std::string& filename, FileFormat format = FileFormat::AUTO) const;
    
    // Convert string content to FASTA
    std::string convert_string(const std::string& content, FileFormat format) const;
    
    // Get sequence for a single chain
    std::string chain_to_sequence(const Chain& chain) const;
    
private:
    ConversionOptions options_;
    std::string wrap_sequence(const std::string& sequence) const;
};

} // namespace pdb2fasta

#endif // PDB2FASTA_CONVERTER_HPP
