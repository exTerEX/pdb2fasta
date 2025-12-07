#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "pdb2fasta/pdb2fasta.hpp"

namespace py = pybind11;

PYBIND11_MODULE(_pdb2fasta, m) {
    m.doc() = "PDB/mmCIF to FASTA converter";
    
    // Enums
    py::enum_<pdb2fasta::FileFormat>(m, "FileFormat")
        .value("PDB", pdb2fasta::FileFormat::PDB)
        .value("MMCIF", pdb2fasta::FileFormat::MMCIF)
        .value("AUTO", pdb2fasta::FileFormat::AUTO)
        .export_values();
    
    // Residue struct
    py::class_<pdb2fasta::Residue>(m, "Residue")
        .def(py::init<>())
        .def_readwrite("name", &pdb2fasta::Residue::name)
        .def_readwrite("sequence_number", &pdb2fasta::Residue::sequence_number)
        .def_readwrite("insertion_code", &pdb2fasta::Residue::insertion_code)
        .def("__repr__", [](const pdb2fasta::Residue& r) {
            return "<Residue " + r.name + " " + std::to_string(r.sequence_number) + ">";
        });
    
    // Chain struct
    py::class_<pdb2fasta::Chain>(m, "Chain")
        .def(py::init<>())
        .def_readwrite("id", &pdb2fasta::Chain::id)
        .def_readwrite("residues", &pdb2fasta::Chain::residues)
        .def("__repr__", [](const pdb2fasta::Chain& c) {
            return "<Chain " + std::string(1, c.id) + " with " + 
                   std::to_string(c.residues.size()) + " residues>";
        });
    
    // Structure struct
    py::class_<pdb2fasta::Structure>(m, "Structure")
        .def(py::init<>())
        .def_readwrite("name", &pdb2fasta::Structure::name)
        .def_readwrite("chains", &pdb2fasta::Structure::chains)
        .def("__repr__", [](const pdb2fasta::Structure& s) {
            return "<Structure '" + s.name + "' with " + 
                   std::to_string(s.chains.size()) + " chains>";
        });
    
    // ConversionOptions
    py::class_<pdb2fasta::ConversionOptions>(m, "ConversionOptions")
        .def(py::init<>())
        .def_readwrite("line_width", &pdb2fasta::ConversionOptions::line_width)
        .def_readwrite("include_chain_id", &pdb2fasta::ConversionOptions::include_chain_id)
        .def_readwrite("unknown_residue", &pdb2fasta::ConversionOptions::unknown_residue);
    
    // PDB Parser
    py::class_<pdb2fasta::PDBParser>(m, "PDBParser")
        .def(py::init<>())
        .def("parse", py::overload_cast<const std::string&>(&pdb2fasta::PDBParser::parse),
             py::arg("content"), "Parse PDB format string");
    
    // mmCIF Parser
    py::class_<pdb2fasta::MMCIFParser>(m, "MMCIFParser")
        .def(py::init<>())
        .def("parse", py::overload_cast<const std::string&>(&pdb2fasta::MMCIFParser::parse),
             py::arg("content"), "Parse mmCIF format string");
    
    // Converter
    py::class_<pdb2fasta::Converter>(m, "Converter")
        .def(py::init<>())
        .def(py::init<pdb2fasta::ConversionOptions>(), py::arg("options"))
        .def("to_fasta", &pdb2fasta::Converter::to_fasta,
             py::arg("structure"), "Convert structure to FASTA format")
        .def("convert_file", &pdb2fasta::Converter::convert_file,
             py::arg("filename"), py::arg("format") = pdb2fasta::FileFormat::AUTO,
             "Convert a PDB/mmCIF file to FASTA")
        .def("convert_string", &pdb2fasta::Converter::convert_string,
             py::arg("content"), py::arg("format"),
             "Convert PDB/mmCIF string content to FASTA")
        .def("chain_to_sequence", &pdb2fasta::Converter::chain_to_sequence,
             py::arg("chain"), "Get sequence for a single chain");
    
    // Convenience functions
    m.def("convert", &pdb2fasta::convert,
          py::arg("filename"),
          "Convert a PDB/mmCIF file to FASTA format");
    
    m.def("convert_string", &pdb2fasta::convert_string,
          py::arg("content"), py::arg("format"),
          "Convert PDB/mmCIF string content to FASTA format");
    
    m.def("detect_format", &pdb2fasta::detect_format,
          py::arg("filename"),
          "Detect file format from filename extension");
    
    m.def("detect_format_from_content", &pdb2fasta::detect_format_from_content,
          py::arg("content"),
          "Detect file format from content");
    
    m.def("residue_to_one_letter", &pdb2fasta::residue_to_one_letter,
          py::arg("three_letter"),
          "Convert three-letter residue code to one-letter code");
}
