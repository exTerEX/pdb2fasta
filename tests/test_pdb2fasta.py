import pdb2fasta


PDB_CONTENT = """ATOM      1  N   ALA A   1       0.000   0.000   0.000  1.00  0.00           N
ATOM      2  CA  ALA A   1       1.458   0.000   0.000  1.00  0.00           C
ATOM      3  N   GLY A   2       2.000   1.000   0.000  1.00  0.00           N
ATOM      4  CA  GLY A   2       3.458   1.000   0.000  1.00  0.00           C
ATOM      5  N   VAL A   3       4.000   2.000   0.000  1.00  0.00           N
ATOM      6  CA  VAL A   3       5.458   2.000   0.000  1.00  0.00           C
END
"""

MMCIF_CONTENT = """data_test
loop_
_atom_site.group_PDB
_atom_site.id
_atom_site.type_symbol
_atom_site.label_atom_id
_atom_site.label_comp_id
_atom_site.label_asym_id
_atom_site.label_seq_id
_atom_site.Cartn_x
_atom_site.Cartn_y
_atom_site.Cartn_z
ATOM 1 N N ALA A 1 0.000 0.000 0.000
ATOM 2 C CA ALA A 1 1.458 0.000 0.000
ATOM 3 N N GLY A 2 2.000 1.000 0.000
ATOM 4 C CA GLY A 2 3.458 1.000 0.000
ATOM 5 N N VAL A 3 4.000 2.000 0.000
ATOM 6 C CA VAL A 3 5.458 2.000 0.000
"""


class TestPDBParser:
    def test_parse_pdb(self):
        parser = pdb2fasta.PDBParser()
        structure = parser.parse(PDB_CONTENT)

        assert len(structure.chains) == 1
        assert structure.chains[0].id == "A"
        assert len(structure.chains[0].residues) == 3

    def test_residue_names(self):
        parser = pdb2fasta.PDBParser()
        structure = parser.parse(PDB_CONTENT)

        residues = structure.chains[0].residues
        assert residues[0].name == "ALA"
        assert residues[1].name == "GLY"
        assert residues[2].name == "VAL"


class TestMMCIFParser:
    def test_parse_mmcif(self):
        parser = pdb2fasta.MMCIFParser()
        structure = parser.parse(MMCIF_CONTENT)

        assert len(structure.chains) == 1
        assert structure.chains[0].id == "A"
        assert len(structure.chains[0].residues) == 3


class TestConverter:
    def test_convert_pdb_string(self):
        converter = pdb2fasta.Converter()
        fasta = converter.convert_string(PDB_CONTENT, pdb2fasta.FileFormat.PDB)

        assert ">_A" in fasta or ">A" in fasta
        assert "AGV" in fasta

    def test_convert_mmcif_string(self):
        converter = pdb2fasta.Converter()
        fasta = converter.convert_string(MMCIF_CONTENT, pdb2fasta.FileFormat.MMCIF)

        assert "AGV" in fasta

    def test_line_width(self):
        options = pdb2fasta.ConversionOptions()
        options.line_width = 2
        converter = pdb2fasta.Converter(options)
        fasta = converter.convert_string(PDB_CONTENT, pdb2fasta.FileFormat.PDB)

        lines = [line for line in fasta.split("\n") if line and not line.startswith(">")]
        assert all(len(line) <= 2 for line in lines)


class TestConvenienceFunctions:
    def test_pdb_to_fasta(self):
        fasta = pdb2fasta.pdb_to_fasta(PDB_CONTENT)
        assert "AGV" in fasta

    def test_mmcif_to_fasta(self):
        fasta = pdb2fasta.mmcif_to_fasta(MMCIF_CONTENT)
        assert "AGV" in fasta

    def test_residue_to_one_letter(self):
        assert pdb2fasta.residue_to_one_letter("ALA") == "A"
        assert pdb2fasta.residue_to_one_letter("GLY") == "G"
        assert pdb2fasta.residue_to_one_letter("VAL") == "V"
        assert pdb2fasta.residue_to_one_letter("XXX") == "X"


class TestFormatDetection:
    def test_detect_pdb(self):
        assert pdb2fasta.detect_format("protein.pdb") == pdb2fasta.FileFormat.PDB
        assert pdb2fasta.detect_format("protein.ent") == pdb2fasta.FileFormat.PDB

    def test_detect_mmcif(self):
        assert pdb2fasta.detect_format("protein.cif") == pdb2fasta.FileFormat.MMCIF
        assert pdb2fasta.detect_format("protein.mmcif") == pdb2fasta.FileFormat.MMCIF

    def test_detect_from_content(self):
        assert (
            pdb2fasta.detect_format_from_content(PDB_CONTENT)
            == pdb2fasta.FileFormat.PDB
        )
        assert (
            pdb2fasta.detect_format_from_content(MMCIF_CONTENT)
            == pdb2fasta.FileFormat.MMCIF
        )
