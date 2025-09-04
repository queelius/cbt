#!/bin/bash

# Build documentation for CBT

echo "Building CBT Documentation..."

# Create output directory
mkdir -p docs/api

# Generate Doxygen documentation
if command -v doxygen &> /dev/null; then
    echo "Generating API documentation with Doxygen..."
    doxygen Doxyfile
    echo "API documentation generated in docs/api/"
else
    echo "Warning: Doxygen not found. Skipping API documentation."
fi

# Build LaTeX paper if pdflatex is available
if command -v pdflatex &> /dev/null; then
    echo "Building academic paper..."
    cd papers
    pdflatex -interaction=nonstopmode cbt_theory_extended.tex
    bibtex cbt_theory_extended || true
    pdflatex -interaction=nonstopmode cbt_theory_extended.tex
    pdflatex -interaction=nonstopmode cbt_theory_extended.tex
    cd ..
    echo "Paper built: papers/cbt_theory_extended.pdf"
else
    echo "Warning: pdflatex not found. Skipping paper compilation."
fi

echo "Documentation build complete!"
echo ""
echo "Available documentation:"
echo "  - Main docs: docs/index.md"
echo "  - Theory guide: docs/theory.md"  
echo "  - API reference: docs/api/html/index.html"
echo "  - Academic paper: papers/cbt_theory_extended.pdf"