#ifndef PDB2FASTA_PARSER_HPP
#define PDB2FASTA_PARSER_HPP

#include "types.hpp"
#include <memory>
#include <string>
#include <istream>

namespace pdb2fasta {

enum class FileFormat {
    PDB,
    MMCIF,
    AUTO
};

class Parser {
public:
    virtual ~Parser() = default;
    virtual Structure parse(std::istream& input) = 0;
    virtual Structure parse(const std::string& content) = 0;
};

class PDBParser : public Parser {
public:
    Structure parse(std::istream& input) override;
    Structure parse(const std::string& content) override;
    
private:
    void parse_atom_line(const std::string& line, Structure& structure);
};

class MMCIFParser : public Parser {
public:
    Structure parse(std::istream& input) override;
    Structure parse(const std::string& content) override;
    
private:
    void parse_atom_site(const std::vector<std::string>& lines, Structure& structure);
    std::vector<std::string> tokenize_cif_line(const std::string& line);
};

// Factory function to create appropriate parser
std::unique_ptr<Parser> create_parser(FileFormat format);
FileFormat detect_format(const std::string& filename);
FileFormat detect_format_from_content(const std::string& content);

} // namespace pdb2fasta

#endif // PDB2FASTA_PARSER_HPP
