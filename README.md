# pdb2fasta

Convert PDB and mmCIF structure files to FASTA format.

## Features

- Parse PDB format files
- Parse mmCIF format files
- Auto-detect file format
- Configurable output options
- C++ library with Python bindings
- Command-line interface

## Installation

### Python Package

```bash
pip install pdb2fasta
```

### From Source

**Requirements:**
- CMake >= 3.15
- C++17 compiler
- Python >= 3.10
- pybind11

**Build and install:**

```bash
# Install in development mode (recommended for testing)
pip install -e .

# Or install normally
pip install .

# With test dependencies
pip install -e ".[test]"
```

**Build the C++ extension:**

The Python package uses scikit-build-core to automatically build the C++ extension during installation. However, if you need to rebuild:

```bash
# Clean and rebuild
pip install --no-build-isolation --force-reinstall -e .
```

### C++ Library Only

```bash
mkdir build && cd build
cmake .. -DBUILD_CLI=ON -DBUILD_PYTHON=OFF
make
make install
```

## Usage

### Python

```python
import pdb2fasta

# Convert a file
fasta = pdb2fasta.convert("structure.pdb")
print(fasta)

# Convert from string
pdb_content = open("structure.pdb").read()
fasta = pdb2fasta.pdb_to_fasta(pdb_content)

# Convert mmCIF
cif_content = open("structure.cif").read()
fasta = pdb2fasta.mmcif_to_fasta(cif_content)

# With options
fasta = pdb2fasta.pdb_to_fasta(
    pdb_content,
    line_width=60,
    include_chain_id=True
)

# Using the Converter class
options = pdb2fasta.ConversionOptions()
options.line_width = 80
converter = pdb2fasta.Converter(options)
fasta = converter.convert_file("structure.pdb")

# Parse and inspect structure
parser = pdb2fasta.PDBParser()
structure = parser.parse(pdb_content)
for chain in structure.chains:
    print(f"Chain {chain.id}: {len(chain.residues)} residues")
```

### Command Line

```bash
# Basic usage
pdb2fasta-cli structure.pdb

# Multiple files
pdb2fasta-cli *.pdb *.cif

# With options
pdb2fasta-cli -w 60 -f mmcif structure.cif

# Options:
#   -h, --help          Show help message
#   -f, --format <fmt>  Force input format (pdb, mmcif, auto)
#   -w, --width <n>     Line width for FASTA output (default: 80)
#   -n, --no-chain      Don't include chain ID in header
```

### C++

```cpp
#include <pdb2fasta/pdb2fasta.hpp>
#include <iostream>

int main() {
    // Simple conversion
    std::string fasta = pdb2fasta::convert("structure.pdb");
    std::cout << fasta;
    
    // With options
    pdb2fasta::ConversionOptions options;
    options.line_width = 60;
    
    pdb2fasta::Converter converter(options);
    fasta = converter.convert_file("structure.cif");
    
    return 0;
}
```

## Development

### Running Tests

First, build and install the package:

```bash
pip install -e ".[test]"
```

Then run tests:

```bash
pytest
# or
uv run pytest
```

### Troubleshooting

If you get `ModuleNotFoundError: No module named '_pdb2fasta'`:

1. Make sure you've installed the package: `pip install -e .`
2. Check that the build completed successfully
3. Verify CMake and a C++ compiler are available
4. Try a clean rebuild: `pip install --no-build-isolation --force-reinstall -e .`

## Supported Formats

### Input
- PDB (.pdb, .ent)
- mmCIF (.cif, .mmcif)

### Output
- FASTA format

## License

MIT License
