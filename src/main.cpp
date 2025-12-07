#include "pdb2fasta/pdb2fasta.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

void print_usage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " [options] <input_file(s)>\n"
              << "\nOptions:\n"
              << "  -h, --help          Show this help message\n"
              << "  -f, --format <fmt>  Force input format (pdb, mmcif, auto)\n"
              << "  -w, --width <n>     Line width for FASTA output (default: 80, 0 for no wrap)\n"
              << "  -n, --no-chain      Don't include chain ID in header\n"
              << "\nSupported formats:\n"
              << "  PDB (.pdb, .ent)\n"
              << "  mmCIF (.cif, .mmcif)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    pdb2fasta::ConversionOptions options;
    pdb2fasta::FileFormat format = pdb2fasta::FileFormat::AUTO;
    std::vector<std::string> input_files;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-f" || arg == "--format") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --format requires an argument\n";
                return 1;
            }
            std::string fmt = argv[++i];
            if (fmt == "pdb") {
                format = pdb2fasta::FileFormat::PDB;
            } else if (fmt == "mmcif" || fmt == "cif") {
                format = pdb2fasta::FileFormat::MMCIF;
            } else if (fmt == "auto") {
                format = pdb2fasta::FileFormat::AUTO;
            } else {
                std::cerr << "Error: Unknown format '" << fmt << "'\n";
                return 1;
            }
        } else if (arg == "-w" || arg == "--width") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --width requires an argument\n";
                return 1;
            }
            options.line_width = std::stoi(argv[++i]);
        } else if (arg == "-n" || arg == "--no-chain") {
            options.include_chain_id = false;
        } else if (arg[0] == '-') {
            std::cerr << "Error: Unknown option '" << arg << "'\n";
            return 1;
        } else {
            input_files.push_back(arg);
        }
    }
    
    if (input_files.empty()) {
        std::cerr << "Error: No input files specified\n";
        return 1;
    }
    
    pdb2fasta::Converter converter(options);
    
    for (const auto& filename : input_files) {
        try {
            std::string fasta = converter.convert_file(filename, format);
            std::cout << fasta;
        } catch (const std::exception& e) {
            std::cerr << "Error processing '" << filename << "': " << e.what() << "\n";
            return 1;
        }
    }
    
    return 0;
}
