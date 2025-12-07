.PHONY: help install install-dev build clean clean-all test lint format check run-cli

# Default target
help:
	@echo "pdb2fasta - Makefile commands"
	@echo ""
	@echo "Setup and Build:"
	@echo "  make install        - Install the package in editable mode"
	@echo "  make install-dev    - Install with development dependencies"
	@echo ""
	@echo "Testing:"
	@echo "  make test           - Run tests with pytest"
	@echo "  make test-verbose   - Run tests with verbose output"
	@echo ""
	@echo "Cleanup:"
	@echo "  make clean          - Remove build artifacts"
	@echo "  make clean-all      - Remove all generated files including .venv"

# Installation targets
install:
	uv sync

# Testing targets
test: install
	uv run pytest

test-verbose: install
	uv run pytest -v

# Cleanup targets
clean:
	rm -rf build/
	rm -rf dist/
	rm -rf *.egg-info
	rm -rf src/*.egg-info
	rm -rf .pytest_cache
	rm -rf htmlcov/
	find . -type d -name __pycache__ -exec rm -rf {} + 2>/dev/null || true
	find . -type f -name "*.pyc" -delete
	find . -type f -name "*.pyo" -delete
	find . -type f -name "*.so" -delete
	find . -type d -name "*.dSYM" -exec rm -rf {} + 2>/dev/null || true

clean-all: clean
	rm -rf .venv
	rm -rf .uv

# Info target
info:
	@echo "Python version:"
	@python --version
	@echo ""
	@echo "UV version:"
	@uv --version
	@echo ""
	@echo "CMake version:"
	@cmake --version | head -n1
	@echo ""
	@echo "C++ compiler:"
	@$(CXX) --version | head -n1 || echo "C++ compiler not found"
