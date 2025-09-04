# ICFP 2025 Submission Checklist

## Conference Information
- **Conference**: International Conference on Functional Programming 2025
- **Deadline**: March 2025 (estimated - CHECK WEBSITE)
- **Page Limit**: 25 pages excluding references (currently 41 pages - NEEDS CONDENSING)
- **Format**: ACM double-column format
- **Submission Site**: https://icfp25.hotcrp.com (estimated)
- **Notification**: June 2025 (estimated)

## Required Adaptations

### Content Focus for ICFP
- [ ] Add functional programming examples (Haskell/OCaml/SML)
- [ ] Emphasize category-theoretic foundations more strongly
- [ ] Connect to monads and type classes
- [ ] Add discussion of CBT as embedded DSLs
- [ ] Include functorial properties of transformations
- [ ] Relate to algebraic effects and handlers

### Length Reduction Strategy (41 → 25 pages)
**Sections to Condense**:
1. [ ] Move detailed proofs to supplementary material
2. [ ] Condense implementation details (Section 7)
3. [ ] Reduce individual transform descriptions
4. [ ] Move some experimental results to appendix
5. [ ] Compress related work section

**Sections to Preserve/Expand**:
1. [ ] Category theory foundations
2. [ ] No Free Lunch theorem
3. [ ] Composition properties
4. [ ] Key examples with FP languages

### Functional Programming Additions

#### Add Haskell Example (after line ~500):
```haskell
-- CBT as type class in Haskell
class CBT domain where
  type Original domain :: *
  type Transformed domain :: *
  
  transform :: Original domain -> Transformed domain
  inverse :: Transformed domain -> Original domain
  
-- Logarithmic domain instance  
instance CBT LogDomain where
  type Original LogDomain = Double
  type Transformed LogDomain = LogDouble
  
  transform = LogDouble . log
  inverse (LogDouble x) = exp x

-- Composable transforms via Kleisli composition
```

#### Add ML/OCaml Module Example:
```ocaml
(* CBT as ML module signature *)
module type CBT = sig
  type original
  type transformed
  val transform : original -> transformed
  val inverse : transformed -> original
  val compose : transformed -> transformed -> transformed
end

(* Logarithmic functor *)
module LogarithmicCBT : CBT with type original = float
```

### ICFP-Specific Requirements
- [ ] Anonymous submission (remove author information)
- [ ] ACM format compliance
- [ ] Artifact submission plan (C++ library)
- [ ] Related work must cite ICFP papers:
  - Automatic differentiation in functional languages
  - Domain-specific languages
  - Category theory in FP
  - Algebraic effects

## Submission Materials

### Main Submission
- [ ] `cbt-icfp.tex` - Main paper (25 pages)
- [ ] `cbt-icfp.pdf` - Compiled PDF
- [ ] Supplementary material with full proofs

### Artifact Submission (if accepted)
- [ ] C++ implementation
- [ ] Haskell bindings/examples
- [ ] Docker container with environment
- [ ] Evaluation scripts

## Timeline

### January 2025
- [ ] Condense paper to 25 pages
- [ ] Add functional programming examples
- [ ] Update related work for FP audience

### February 2025
- [ ] Internal review and polish
- [ ] Prepare supplementary materials
- [ ] Test artifact packaging

### March 2025
- [ ] Final proofreading
- [ ] Submit to ICFP 2025
- [ ] Prepare artifact if requested

## Key Messages for ICFP

### Emphasize:
1. **Mathematical Elegance**: Category theory provides natural framework
2. **Compositional Design**: Transforms compose functorially
3. **Type Safety**: Strong typing prevents domain confusion
4. **DSL Perspective**: Each CBT as embedded DSL
5. **Verified Implementation**: Properties proven formally

### Connect to FP Themes:
- Monadic structure of transformations
- Algebraic laws and equational reasoning
- Type-level programming for safety
- Lazy evaluation for infinite precision
- Pure functional implementation possible

## Review Criteria Preparation

### Anticipated Questions:
1. "How does this relate to automatic differentiation?"
   - Address in Section 4, show AD as CBT instance

2. "Why not implement in Haskell/OCaml?"
   - Explain C++ for performance, show FP bindings possible

3. "Connection to algebraic effects?"
   - Add subsection on effects interpretation

4. "Comparison with existing FP approaches?"
   - Expand related work on FP domain transformations

## Author Action Items

**BEFORE SUBMISSION**:
- [ ] Add author information
- [ ] Verify page count ≤ 25
- [ ] Add FP examples as specified
- [ ] Update acknowledgments
- [ ] Prepare artifact repository

## Files in This Folder

```
icfp_2025/
├── cbt.tex (adapted for ICFP)
├── SUBMISSION_CHECKLIST.md (this file)
├── supplementary/
│   ├── full_proofs.pdf
│   └── extended_evaluation.pdf
└── examples/
    ├── cbt.hs (Haskell examples)
    └── cbt.ml (OCaml examples)
```