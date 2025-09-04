#!/bin/bash
# Script to prepare arXiv submission package for CBT paper
# Run this after adding author information to cbt.tex

echo "CBT Paper - arXiv Submission Preparation Script"
echo "================================================"

# Check if we're in the right directory
if [ ! -f "cbt.tex" ]; then
    echo "Error: cbt.tex not found. Please run from arxiv_2025 directory."
    exit 1
fi

echo ""
echo "Step 1: Checking for author information..."
if grep -q "Anonymous Authors" cbt.tex; then
    echo "⚠️  WARNING: Author information still shows 'Anonymous Authors'"
    echo "   Please update lines 58-63 in cbt.tex before proceeding."
    read -p "Continue anyway? (y/n) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
else
    echo "✓ Author information found"
fi

echo ""
echo "Step 2: Checking for acknowledgments..."
if grep -q "TODO: Add specific grant numbers" cbt.tex; then
    echo "⚠️  WARNING: Acknowledgments contain TODO"
    echo "   Please update line 1623 in cbt.tex"
fi

echo ""
echo "Step 3: Checking for repository URL..."
if grep -q "\[anonymized\]" cbt.tex; then
    echo "⚠️  WARNING: GitHub URL still shows [anonymized]"
    echo "   Please update line 1552 in cbt.tex"
fi

echo ""
echo "Step 4: Compiling LaTeX (this may take a moment)..."
# Run pdflatex twice for proper references
pdflatex -interaction=nonstopmode cbt.tex > compile1.log 2>&1
if [ $? -ne 0 ]; then
    echo "❌ First compilation failed. Check compile1.log for errors."
    tail -20 compile1.log
    exit 1
fi

pdflatex -interaction=nonstopmode cbt.tex > compile2.log 2>&1
if [ $? -ne 0 ]; then
    echo "❌ Second compilation failed. Check compile2.log for errors."
    tail -20 compile2.log
    exit 1
fi

echo "✓ LaTeX compilation successful"

echo ""
echo "Step 5: Checking PDF size..."
pdf_size=$(stat -f%z "cbt.pdf" 2>/dev/null || stat -c%s "cbt.pdf" 2>/dev/null)
pdf_size_mb=$((pdf_size / 1048576))
if [ $pdf_size_mb -gt 10 ]; then
    echo "⚠️  WARNING: PDF size is ${pdf_size_mb}MB (limit is 10MB)"
else
    echo "✓ PDF size is acceptable (${pdf_size_mb}MB)"
fi

echo ""
echo "Step 6: Creating submission package..."
# Clean up auxiliary files
rm -f *.aux *.log *.out *.bbl *.blg *.toc *.lof *.lot

# Create tar.gz with just the necessary file
tar czf cbt-arxiv.tar.gz cbt.tex
echo "✓ Created cbt-arxiv.tar.gz"

echo ""
echo "Step 7: Verifying package..."
tar_size=$(stat -f%z "cbt-arxiv.tar.gz" 2>/dev/null || stat -c%s "cbt-arxiv.tar.gz" 2>/dev/null)
tar_size_kb=$((tar_size / 1024))
echo "✓ Package size: ${tar_size_kb}KB"

echo ""
echo "================================================"
echo "SUBMISSION READY!"
echo "================================================"
echo ""
echo "Files prepared:"
echo "  • cbt-arxiv.tar.gz (upload this to arXiv)"
echo "  • cbt.pdf (for your reference)"
echo ""
echo "Next steps:"
echo "  1. Go to https://arxiv.org/submit"
echo "  2. Login with your arXiv account"
echo "  3. Click 'Start New Submission'"
echo "  4. Upload: cbt-arxiv.tar.gz"
echo "  5. Select categories:"
echo "     - Primary: cs.DS"
echo "     - Cross-list: cs.NA, cs.PL"
echo "  6. Copy abstract from arxiv_metadata.txt"
echo "  7. Complete submission"
echo ""
echo "After submission:"
echo "  - You'll receive an arXiv ID (e.g., arXiv:2025.XXXXX)"
echo "  - Paper typically appears within 24-48 hours"
echo "  - Share the arXiv link when available"
echo ""
echo "Good luck with your submission!"