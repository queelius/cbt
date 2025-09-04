# CBT Documentation

This directory contains the documentation source files for the CBT (Computational Basis Transforms) library.

## Documentation Structure

```
docs/
├── README.md                       # This file
├── quick_start.md                  # Quick start guide
├── api_reference.md               # API reference overview
├── tutorials.md                   # Step-by-step tutorials
├── transform_guides.md            # Detailed transform guides
├── mathematical_foundations.md    # Mathematical theory
├── troubleshooting.md            # Common issues and solutions
├── documentation_index.md        # Documentation index
├── doxygen-custom/               # Doxygen customization files
│   ├── DoxygenLayout.xml        # Custom layout configuration
│   ├── header.html              # Custom HTML header
│   ├── footer.html              # Custom HTML footer
│   ├── doxygen-awesome.css      # Modern theme CSS
│   ├── doxygen-awesome-sidebar-only.css
│   ├── custom.css               # CBT-specific styles
│   └── *.js                     # Interactive features
└── images/                       # Documentation images
    └── logo.png                  # Project logo

```

## Building Documentation

### Prerequisites

- Doxygen (1.9.0 or later recommended)
- Graphviz (for diagrams)
- Python 3 (for local server)

### Local Build

Run the build script from the project root:

```bash
./build_docs.sh
```

This will:
1. Generate the documentation using Doxygen
2. Create output in `doxygen-output/html/`
3. Optionally build the LaTeX paper
4. Offer to open the documentation in your browser

### Manual Build

```bash
# From project root
doxygen Doxyfile

# View locally
cd doxygen-output/html
python3 -m http.server 8000
# Open http://localhost:8000 in your browser
```

## GitHub Pages Deployment

Documentation is automatically deployed to GitHub Pages when changes are pushed to the master branch.

The GitHub Actions workflow (`.github/workflows/docs.yml`) handles:
- Installing dependencies
- Building documentation with Doxygen
- Deploying to GitHub Pages

View the live documentation at: https://queelius.github.io/cbt/

## Customization

### Theme

The documentation uses the Doxygen Awesome theme with custom CBT styling:
- Modern, clean design
- Dark mode support
- Interactive table of contents
- Copy buttons for code blocks
- Responsive layout

### Adding New Pages

1. Create a new markdown file in `docs/`
2. Add it to the `INPUT` list in `Doxyfile`
3. Update `documentation_index.md` to link to it
4. Rebuild the documentation

### Modifying Styles

Edit the CSS files in `docs/doxygen-custom/`:
- `doxygen-awesome.css` - Main theme
- `custom.css` - CBT-specific overrides

## Writing Documentation

### Markdown Guidelines

- Use standard Markdown syntax
- Code blocks should specify the language: ` ```cpp`
- Use `@ref` for cross-references
- Mathematical formulas use LaTeX: `$\log(xy) = \log(x) + \log(y)$`

### API Documentation

Add documentation directly in header files using Doxygen syntax:

```cpp
/**
 * @brief Brief description
 * @details Detailed description
 * @param param_name Parameter description
 * @return Return value description
 * @see Related_Class
 * @note Important notes
 * @warning Warnings
 */
```

### Examples

Place example code in `examples/` directory. They will be automatically included in the documentation.

## Troubleshooting

### Common Issues

1. **Doxygen not found**: Install Doxygen using your package manager
2. **Graphs not generating**: Install Graphviz
3. **Styles not loading**: Check that custom CSS files are in the correct location
4. **GitHub Pages not updating**: Check the Actions tab for build errors

### Support

For documentation issues, please open an issue on GitHub:
https://github.com/queelius/cbt/issues