#!/bin/bash

# CBT Documentation Build Script
# Builds the documentation locally for testing

set -e  # Exit on error

echo "====================================="
echo "CBT Documentation Build Script"
echo "====================================="
echo ""

# Check if Doxygen is installed
if ! command -v doxygen &> /dev/null; then
    echo "Error: Doxygen is not installed."
    echo "Please install Doxygen first:"
    echo "  Ubuntu/Debian: sudo apt-get install doxygen graphviz"
    echo "  macOS: brew install doxygen graphviz"
    exit 1
fi

# Check if Graphviz is installed (for diagrams)
if ! command -v dot &> /dev/null; then
    echo "Warning: Graphviz is not installed. Diagrams will not be generated."
    echo "Install Graphviz for full documentation features:"
    echo "  Ubuntu/Debian: sudo apt-get install graphviz"
    echo "  macOS: brew install graphviz"
    echo ""
fi

# Create necessary directories
echo "Creating directories..."
mkdir -p doxygen-output
mkdir -p docs/images

# Generate logo if it doesn't exist
if [ ! -f docs/images/logo.png ]; then
    echo "Creating placeholder logo..."
    cat > docs/images/logo.svg << 'EOF'
<svg width="64" height="64" xmlns="http://www.w3.org/2000/svg">
  <rect width="64" height="64" fill="#2e7d32" rx="8"/>
  <text x="32" y="40" font-family="Arial" font-size="24" font-weight="bold" fill="white" text-anchor="middle">CBT</text>
</svg>
EOF
    
    # Try to convert SVG to PNG if ImageMagick is available
    if command -v convert &> /dev/null; then
        convert docs/images/logo.svg docs/images/logo.png
        echo "Logo created: docs/images/logo.png"
    else
        cp docs/images/logo.svg docs/images/logo.png
        echo "Logo created: docs/images/logo.png (SVG copy)"
    fi
fi

# Clean previous build
if [ -d "doxygen-output/html" ]; then
    echo "Cleaning previous documentation build..."
    rm -rf doxygen-output/html
fi

# Run Doxygen
echo ""
echo "Running Doxygen..."
echo "====================================="
doxygen Doxyfile

# Check if documentation was generated
if [ -d "doxygen-output/html" ]; then
    echo ""
    echo "====================================="
    echo "Documentation generated successfully!"
    echo "====================================="
    
    # Create .nojekyll for GitHub Pages
    touch doxygen-output/html/.nojekyll
    
    # Count files
    FILE_COUNT=$(find doxygen-output/html -type f | wc -l)
    DIR_SIZE=$(du -sh doxygen-output/html | cut -f1)
    
    echo ""
    echo "Statistics:"
    echo "  Total files: $FILE_COUNT"
    echo "  Total size: $DIR_SIZE"
    echo ""
    echo "To view the documentation:"
    echo "  1. Open in browser: doxygen-output/html/index.html"
    echo "  2. Or run a local server:"
    echo "     cd doxygen-output/html && python3 -m http.server 8000"
    echo "     Then open: http://localhost:8000"
    echo ""
    
    # Optionally build the paper
    if [ -d "paper" ] && [ -f "paper/Makefile" ]; then
        read -p "Do you want to build the LaTeX paper? (y/n) " -n 1 -r
        echo ""
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            echo "Building LaTeX paper..."
            cd paper
            make
            cd ..
            if [ -f "paper/cbt.pdf" ]; then
                cp paper/cbt.pdf doxygen-output/html/
                echo "Paper PDF copied to documentation"
            fi
        fi
    fi
    
    # Ask if user wants to open in browser
    read -p "Open documentation in browser? (y/n) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        if command -v xdg-open &> /dev/null; then
            xdg-open doxygen-output/html/index.html
        elif command -v open &> /dev/null; then
            open doxygen-output/html/index.html
        else
            echo "Please open doxygen-output/html/index.html manually"
        fi
    fi
else
    echo ""
    echo "====================================="
    echo "Error: Documentation was not generated"
    echo "====================================="
    echo "Check the Doxygen output above for errors"
    exit 1
fi