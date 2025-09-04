/**
 * Paragraph linking for better navigation
 */

class ParagraphLink {
    constructor() {
        this.init();
    }

    init() {
        // Add links to all headers
        this.addLinksToHeaders();
        
        // Handle dynamically added content
        const observer = new MutationObserver(() => {
            this.addLinksToHeaders();
        });

        observer.observe(document.body, {
            childList: true,
            subtree: true
        });
    }

    addLinksToHeaders() {
        const headers = document.querySelectorAll('h1, h2, h3, h4, h5, h6');
        
        headers.forEach(header => {
            // Skip if link already exists
            if (header.querySelector('.header-link')) {
                return;
            }

            // Skip if header doesn't have an id
            if (!header.id) {
                // Try to generate an id from the text content
                const text = header.textContent.trim();
                if (text) {
                    header.id = this.generateId(text);
                } else {
                    return;
                }
            }

            // Create link element
            const link = document.createElement('a');
            link.className = 'header-link';
            link.href = `#${header.id}`;
            link.innerHTML = this.getLinkIcon();
            link.title = 'Copy link to this section';
            link.setAttribute('aria-label', 'Link to this section');
            
            // Style the link
            link.style.cssText = `
                display: inline-block;
                margin-left: 0.5em;
                color: var(--primary-color);
                opacity: 0;
                transition: opacity 0.2s;
                text-decoration: none;
                vertical-align: middle;
            `;

            // Show on hover
            header.style.position = 'relative';
            header.addEventListener('mouseenter', () => {
                link.style.opacity = '1';
            });
            header.addEventListener('mouseleave', () => {
                link.style.opacity = '0';
            });

            // Copy link on click
            link.addEventListener('click', (e) => {
                e.preventDefault();
                this.copyLink(header.id, link);
            });

            header.appendChild(link);
        });
    }

    generateId(text) {
        return text
            .toLowerCase()
            .replace(/[^a-z0-9\s-]/g, '')
            .replace(/\s+/g, '-')
            .replace(/-+/g, '-')
            .trim();
    }

    getLinkIcon() {
        return `<svg width="16" height="16" viewBox="0 0 16 16" fill="currentColor">
            <path d="M4.715 6.542 3.343 7.914a3 3 0 1 0 4.243 4.243l1.828-1.829A3 3 0 0 0 8.586 5.5L8 6.086a1.002 1.002 0 0 0-.154.199 2 2 0 0 1 .861 3.337L6.88 11.45a2 2 0 1 1-2.83-2.83l.793-.792a4.018 4.018 0 0 1-.128-1.287z"/>
            <path d="M6.586 4.672A3 3 0 0 0 7.414 9.5l.775-.776a2 2 0 0 1-.896-3.346L9.12 3.55a2 2 0 1 1 2.83 2.83l-.793.792c.112.42.155.855.128 1.287l1.372-1.372a3 3 0 1 0-4.243-4.243L6.586 4.672z"/>
        </svg>`;
    }

    async copyLink(id, linkElement) {
        const url = `${window.location.origin}${window.location.pathname}#${id}`;
        
        try {
            if (navigator.clipboard) {
                await navigator.clipboard.writeText(url);
            } else {
                // Fallback
                const textarea = document.createElement('textarea');
                textarea.value = url;
                textarea.style.position = 'fixed';
                textarea.style.opacity = '0';
                document.body.appendChild(textarea);
                textarea.select();
                document.execCommand('copy');
                document.body.removeChild(textarea);
            }

            // Show success feedback
            const originalOpacity = linkElement.style.opacity;
            linkElement.style.color = '#4CAF50';
            linkElement.style.opacity = '1';
            
            // Create tooltip
            const tooltip = document.createElement('span');
            tooltip.textContent = 'Link copied!';
            tooltip.style.cssText = `
                position: absolute;
                top: -30px;
                left: 50%;
                transform: translateX(-50%);
                background: #4CAF50;
                color: white;
                padding: 4px 8px;
                border-radius: 4px;
                font-size: 12px;
                white-space: nowrap;
                pointer-events: none;
                z-index: 1000;
            `;
            
            linkElement.parentElement.appendChild(tooltip);
            
            setTimeout(() => {
                tooltip.remove();
                linkElement.style.color = '';
                linkElement.style.opacity = originalOpacity;
            }, 2000);

        } catch (err) {
            console.error('Failed to copy link:', err);
        }
    }
}

// Initialize when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', () => new ParagraphLink());
} else {
    new ParagraphLink();
}