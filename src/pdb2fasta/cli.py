"""Command-line interface for pdb2fasta."""

import sys
import argparse
from pathlib import Path
from . import Converter, ConversionOptions, FileFormat # type: ignore


def main():
    """Main entry point for the CLI."""
    parser = argparse.ArgumentParser(
        description="Convert PDB and mmCIF files to FASTA format",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Supported formats:
  PDB (.pdb, .ent)
  mmCIF (.cif, .mmcif)

Examples:
  %(prog)s structure.pdb
  %(prog)s *.pdb *.cif
  %(prog)s -w 60 -f mmcif structure.cif
        """
    )
    
    parser.add_argument(
        'files',
        nargs='+',
        type=Path,
        help='Input PDB/mmCIF file(s)'
    )
    
    parser.add_argument(
        '-f', '--format',
        choices=['pdb', 'mmcif', 'cif', 'auto'],
        default='auto',
        help='Force input format (default: auto)'
    )
    
    parser.add_argument(
        '-w', '--width',
        type=int,
        default=80,
        help='Line width for FASTA output (default: 80, 0 for no wrap)'
    )
    
    parser.add_argument(
        '-n', '--no-chain',
        action='store_true',
        help="Don't include chain ID in header"
    )
    
    args = parser.parse_args()
    
    # Convert format string to enum
    format_map = {
        'pdb': FileFormat.PDB,
        'mmcif': FileFormat.MMCIF,
        'cif': FileFormat.MMCIF,
        'auto': FileFormat.AUTO
    }
    file_format = format_map[args.format]
    
    # Set up options
    options = ConversionOptions()
    options.line_width = args.width
    options.include_chain_id = not args.no_chain
    
    converter = Converter(options)
    
    # Process files
    for filepath in args.files:
        if not filepath.exists():
            print(f"Error: File not found: {filepath}", file=sys.stderr)
            return 1
        
        try:
            fasta = converter.convert_file(str(filepath), file_format)
            print(fasta, end='')
        except Exception as e:
            print(f"Error processing '{filepath}': {e}", file=sys.stderr)
            return 1
    
    return 0


if __name__ == '__main__':
    sys.exit(main())
