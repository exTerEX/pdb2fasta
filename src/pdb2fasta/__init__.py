"""
pdb2fasta - Convert PDB and mmCIF files to FASTA format.
"""

try:
    from _pdb2fasta import (
        # Enums
        FileFormat,
        
        # Classes
        Residue,
        Chain,
        Structure,
        ConversionOptions,
        PDBParser,
        MMCIFParser,
        Converter,
        
        # Functions
        convert,
        convert_string,
        detect_format,
        detect_format_from_content,
        residue_to_one_letter,
    )
except ImportError as e:
    raise ImportError(
        "The C++ extension module '_pdb2fasta' could not be imported. "
        "Please build the package first using: pip install -e . or pip install ."
    ) from e

__version__ = "v0.1.0"
__all__ = [
    "FileFormat",
    "Residue",
    "Chain",
    "Structure",
    "ConversionOptions",
    "PDBParser",
    "MMCIFParser",
    "Converter",
    "convert",
    "convert_string",
    "detect_format",
    "detect_format_from_content",
    "residue_to_one_letter",
    "pdb_to_fasta",
    "mmcif_to_fasta",
]


def pdb_to_fasta(content: str, **kwargs) -> str:
    """
    Convert PDB format string to FASTA.
    
    Args:
        content: PDB format string
        **kwargs: Options passed to ConversionOptions
            - line_width (int): Line width for output (default: 80)
            - include_chain_id (bool): Include chain ID in header (default: True)
            - unknown_residue (str): Character for unknown residues (default: 'X')
    
    Returns:
        FASTA format string
    """
    options = ConversionOptions()
    if 'line_width' in kwargs:
        options.line_width = kwargs['line_width']
    if 'include_chain_id' in kwargs:
        options.include_chain_id = kwargs['include_chain_id']
    if 'unknown_residue' in kwargs:
        options.unknown_residue = kwargs['unknown_residue']
    
    converter = Converter(options)
    return converter.convert_string(content, FileFormat.PDB)


def mmcif_to_fasta(content: str, **kwargs) -> str:
    """
    Convert mmCIF format string to FASTA.
    
    Args:
        content: mmCIF format string
        **kwargs: Options passed to ConversionOptions
            - line_width (int): Line width for output (default: 80)
            - include_chain_id (bool): Include chain ID in header (default: True)
            - unknown_residue (str): Character for unknown residues (default: 'X')
    
    Returns:
        FASTA format string
    """
    options = ConversionOptions()
    if 'line_width' in kwargs:
        options.line_width = kwargs['line_width']
    if 'include_chain_id' in kwargs:
        options.include_chain_id = kwargs['include_chain_id']
    if 'unknown_residue' in kwargs:
        options.unknown_residue = kwargs['unknown_residue']
    
    converter = Converter(options)
    return converter.convert_string(content, FileFormat.MMCIF)
