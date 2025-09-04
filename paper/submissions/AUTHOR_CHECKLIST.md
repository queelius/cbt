# Author Action Items for CBT Paper Submission

## IMMEDIATE ACTIONS REQUIRED (For arXiv Submission)

### 1. Author Information
**Location**: `/paper/submissions/arxiv_2025/cbt.tex` lines 58-63

Please provide:
- [ ] Full name(s) of author(s)
- [ ] Email address (at least corresponding author)
- [ ] Institution name(s)
- [ ] Department name(s)
- [ ] ORCID iDs (recommended for scholarly tracking)

Example format:
```latex
\author{
    John Doe\footnote{Corresponding author: john.doe@university.edu}\\
    \textit{Department of Computer Science}\\
    \textit{University Name}\\
    ORCID: 0000-0000-0000-0000
    \and
    Jane Smith\\
    \textit{Department of Mathematics}\\
    \textit{Another University}\\
    ORCID: 0000-0000-0000-0001
}
```

### 2. Acknowledgments and Funding
**Location**: `/paper/submissions/arxiv_2025/cbt.tex` line 1620-1623

Please provide:
- [ ] Grant numbers and funding agency names
- [ ] Institutional support
- [ ] Names of people to thank (reviewers, colleagues)
- [ ] Any computational resources used

Example:
```latex
\section*{Acknowledgments}
We thank A. Person and B. Colleague for insightful discussions. 
This work was supported by NSF Grant CCF-XXXXXXX and DARPA Grant XXXXX. 
Computational resources were provided by [Institution] HPC Center.
```

### 3. Code Repository
**Location**: `/paper/submissions/arxiv_2025/cbt.tex` line 1552

- [ ] Decide on making repository public
- [ ] If yes, provide GitHub URL
- [ ] If no, update to "available upon request"

Current placeholder: `https://github.com/[anonymized]`
Should be: `https://github.com/yourusername/cbt`

### 4. arXiv Account Setup
- [ ] Do you have an arXiv account? If not, create at https://arxiv.org/user/register
- [ ] Do you have endorsement for cs.DS category? (needed for first submission)
- [ ] Preferred email for arXiv notifications?

## SUBMISSION TIMELINE & DECISIONS

### Phase 1: Immediate (This Week)
**arXiv Submission**
- [ ] Complete author information above
- [ ] Approve arXiv categories (cs.DS primary, cs.NA and cs.PL cross-list)
- [ ] Review and approve abstract
- [ ] Choose license (suggested: CC BY 4.0)
- [ ] Ready to submit by: [DATE]

### Phase 2: Conference Strategy (Jan-Mar 2025)

**Decision Required**: Which conference to target first?

#### Option A: ICFP 2025 (Recommended)
- **Deadline**: March 2025
- **Pros**: Perfect fit for category theory approach, strong FP community
- **Cons**: Need to add FP examples, condense to 25 pages
- **Your decision**: [ ] Target ICFP 2025

#### Option B: POPL 2026
- **Deadline**: July 2025
- **Pros**: Top PL venue, more time to prepare
- **Cons**: Very competitive, later timeline
- **Your decision**: [ ] Target POPL 2026

#### Option C: Journal First (ACM TOMS)
- **Deadline**: Anytime
- **Pros**: No page limit, comprehensive treatment
- **Cons**: Longer review cycle (3-6 months)
- **Your decision**: [ ] Submit to journal

### Phase 3: Supporting Materials

**Optional but Recommended**:
- [ ] Create project website (can host on GitHub Pages)
- [ ] Prepare slides for talks (useful for conferences)
- [ ] Draft blog post for publicity
- [ ] Prepare Twitter thread announcement
- [ ] Create demo videos/notebooks

## INFORMATION COLLECTION FORM

Please fill out for submission metadata:

### Author 1 (Corresponding)
- Full Name: _____________________
- Email: _____________________
- Affiliation: _____________________
- ORCID: _____________________
- Homepage: _____________________

### Author 2 (if applicable)
- Full Name: _____________________
- Email: _____________________
- Affiliation: _____________________
- ORCID: _____________________
- Homepage: _____________________

### Paper Metadata
- Preferred contact email: _____________________
- Keywords (approve/edit): computational basis transforms, domain transformations, category theory, algorithmic complexity
- License choice: [ ] CC BY 4.0 [ ] CC BY-SA 4.0 [ ] Other: _____
- Code availability: [ ] Public now [ ] Upon acceptance [ ] Upon request

### Funding Information
- Grant 1: Agency: _____ Number: _____ Title: _____
- Grant 2: Agency: _____ Number: _____ Title: _____
- Computational resources: _____________________

### Review Preferences (for journal)
- Suggested reviewers (3-5 experts): _____________________
- Reviewers to exclude (conflicts): _____________________

## QUICK START COMMANDS

Once author information is added:

### 1. Build arXiv Package
```bash
cd /home/spinoza/github/released/cbt/paper/submissions/arxiv_2025
# Update author info in cbt.tex first
pdflatex cbt.tex
pdflatex cbt.tex  # Run twice for references
tar czf cbt-arxiv.tar.gz cbt.tex
```

### 2. Submit to arXiv
1. Go to https://arxiv.org/submit
2. Upload `cbt-arxiv.tar.gz`
3. Select categories: cs.DS (primary), cs.NA, cs.PL (cross-list)
4. Paste abstract from `arxiv_metadata.txt`
5. Submit and note the arXiv ID

### 3. Prepare Conference Version (if targeting ICFP)
```bash
cd /home/spinoza/github/released/cbt/paper/submissions/icfp_2025
# Edit cbt.tex to condense and add FP examples
# Build and verify page count
```

## NEXT STEPS CHECKLIST

### Today/Tomorrow:
- [ ] Review this checklist
- [ ] Provide author information
- [ ] Decide on conference strategy
- [ ] Approve arXiv submission

### This Week:
- [ ] Submit to arXiv
- [ ] Share arXiv link when available
- [ ] Begin conference paper adaptation

### This Month:
- [ ] Complete conference version
- [ ] Prepare supplementary materials
- [ ] Consider blog post/publicity

## QUESTIONS TO CONSIDER

1. **Authorship**: Is the author list complete and ordered correctly?
2. **Timing**: Any conflicts with the suggested timeline?
3. **Venue**: Do you have a preference for conference vs journal?
4. **Repository**: Ready to make code public, or prefer to wait?
5. **Publicity**: Interest in blog post or social media announcement?
6. **Presentation**: Planning to give talks about this work?

## CONTACT & SUPPORT

For any questions about the submission process or to provide the information above, please respond with the completed form or ask for clarification on any items.

The submission folders are organized and ready. Once you provide the author information, the paper can be submitted to arXiv within hours.