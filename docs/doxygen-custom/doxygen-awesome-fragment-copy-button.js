/**
 * Copy button for code fragments
 */

class FragmentCopyButton {
    constructor() {
        this.init();
    }

    init() {
        // Add copy buttons to all code fragments
        document.querySelectorAll('.fragment').forEach(fragment => {
            this.addCopyButton(fragment);
        });

        // Also handle fragments added dynamically
        const observer = new MutationObserver(mutations => {
            mutations.forEach(mutation => {
                mutation.addedNodes.forEach(node => {
                    if (node.nodeType === 1) { // Element node
                        if (node.classList && node.classList.contains('fragment')) {
                            this.addCopyButton(node);
                        }
                        // Check descendants
                        const fragments = node.querySelectorAll ? node.querySelectorAll('.fragment') : [];
                        fragments.forEach(fragment => this.addCopyButton(fragment));
                    }
                });
            });
        });

        observer.observe(document.body, {
            childList: true,
            subtree: true
        });
    }

    addCopyButton(fragment) {
        // Skip if button already exists
        if (fragment.querySelector('.fragment-copy-button')) {
            return;
        }

        // Create wrapper if needed
        if (!fragment.style.position || fragment.style.position === 'static') {
            fragment.style.position = 'relative';
        }

        // Create copy button
        const button = document.createElement('button');
        button.className = 'fragment-copy-button';
        button.innerHTML = this.getCopyIcon();
        button.title = 'Copy to clipboard';
        button.setAttribute('aria-label', 'Copy code to clipboard');
        
        button.addEventListener('click', (e) => {
            e.preventDefault();
            this.copyCode(fragment, button);
        });

        fragment.appendChild(button);
    }

    getCopyIcon() {
        return `<svg width="16" height="16" viewBox="0 0 16 16" fill="currentColor">
            <path d="M13 0H6a2 2 0 0 0-2 2 2 2 0 0 0-2 2v10a2 2 0 0 0 2 2h7a2 2 0 0 0 2-2 2 2 0 0 0 2-2V2a2 2 0 0 0-2-2zm0 13V4a2 2 0 0 0-2-2H5a1 1 0 0 1 1-1h7a1 1 0 0 1 1 1v10a1 1 0 0 1-1 1zM3 4a1 1 0 0 1 1-1h7a1 1 0 0 1 1 1v10a1 1 0 0 1-1 1H4a1 1 0 0 1-1-1V4z"/>
        </svg>`;
    }

    getCheckIcon() {
        return `<svg width="16" height="16" viewBox="0 0 16 16" fill="currentColor">
            <path d="M13.854 3.646a.5.5 0 0 1 0 .708l-7 7a.5.5 0 0 1-.708 0l-3.5-3.5a.5.5 0 1 1 .708-.708L6.5 10.293l6.646-6.647a.5.5 0 0 1 .708 0z"/>
        </svg>`;
    }

    extractCode(fragment) {
        // Get all line elements
        const lines = fragment.querySelectorAll('.line');
        if (lines.length > 0) {
            return Array.from(lines)
                .map(line => line.textContent)
                .join('\n');
        }
        
        // Fallback to text content
        const button = fragment.querySelector('.fragment-copy-button');
        const tempFragment = fragment.cloneNode(true);
        if (button) {
            const tempButton = tempFragment.querySelector('.fragment-copy-button');
            if (tempButton) {
                tempButton.remove();
            }
        }
        return tempFragment.textContent.trim();
    }

    async copyCode(fragment, button) {
        const code = this.extractCode(fragment);
        
        try {
            if (navigator.clipboard) {
                await navigator.clipboard.writeText(code);
            } else {
                // Fallback for older browsers
                const textarea = document.createElement('textarea');
                textarea.value = code;
                textarea.style.position = 'fixed';
                textarea.style.opacity = '0';
                document.body.appendChild(textarea);
                textarea.select();
                document.execCommand('copy');
                document.body.removeChild(textarea);
            }

            // Show success feedback
            const originalHTML = button.innerHTML;
            button.innerHTML = this.getCheckIcon();
            button.style.background = '#4CAF50';
            
            setTimeout(() => {
                button.innerHTML = originalHTML;
                button.style.background = '';
            }, 2000);

        } catch (err) {
            console.error('Failed to copy:', err);
            // Show error feedback
            button.style.background = '#f44336';
            setTimeout(() => {
                button.style.background = '';
            }, 2000);
        }
    }
}

// Initialize when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', () => new FragmentCopyButton());
} else {
    new FragmentCopyButton();
}