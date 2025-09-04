/**
 * Interactive Table of Contents
 */

class InteractiveTOC {
    constructor() {
        this.tocItems = [];
        this.activeItem = null;
        this.init();
    }

    init() {
        // Find or create TOC container
        this.createTOC();
        
        // Set up scroll spy
        this.setupScrollSpy();
        
        // Set up smooth scrolling
        this.setupSmoothScroll();
    }

    createTOC() {
        // Check if a TOC already exists
        let tocContainer = document.querySelector('.interactive-toc');
        if (!tocContainer) {
            // Create floating TOC for pages with multiple sections
            const headers = document.querySelectorAll('.contents h2, .contents h3');
            if (headers.length > 3) {
                tocContainer = document.createElement('div');
                tocContainer.className = 'interactive-toc';
                tocContainer.innerHTML = '<h4>On this page</h4><ul></ul>';
                tocContainer.style.cssText = `
                    position: fixed;
                    right: 20px;
                    top: 100px;
                    width: 250px;
                    max-height: calc(100vh - 200px);
                    overflow-y: auto;
                    background: var(--page-background-color);
                    border: 1px solid var(--separator-color);
                    border-radius: 8px;
                    padding: 20px;
                    z-index: 99;
                    box-shadow: 0 2px 8px rgba(0,0,0,0.1);
                `;

                const list = tocContainer.querySelector('ul');
                list.style.cssText = `
                    list-style: none;
                    padding: 0;
                    margin: 0;
                `;

                headers.forEach(header => {
                    if (!header.id) {
                        header.id = this.generateId(header.textContent);
                    }

                    const item = document.createElement('li');
                    const link = document.createElement('a');
                    link.href = `#${header.id}`;
                    link.textContent = header.textContent;
                    link.style.cssText = `
                        display: block;
                        padding: 5px 10px;
                        color: var(--page-foreground-color);
                        text-decoration: none;
                        border-left: 3px solid transparent;
                        transition: all 0.3s;
                        font-size: 14px;
                    `;

                    if (header.tagName === 'H3') {
                        link.style.paddingLeft = '25px';
                        link.style.fontSize = '13px';
                    }

                    item.appendChild(link);
                    list.appendChild(item);

                    this.tocItems.push({
                        element: header,
                        link: link,
                        id: header.id
                    });
                });

                document.body.appendChild(tocContainer);
                this.tocContainer = tocContainer;

                // Hide on small screens
                const mediaQuery = window.matchMedia('(max-width: 1400px)');
                const handleMediaQuery = (e) => {
                    tocContainer.style.display = e.matches ? 'none' : 'block';
                };
                handleMediaQuery(mediaQuery);
                mediaQuery.addEventListener('change', handleMediaQuery);
            }
        }
    }

    generateId(text) {
        return text
            .toLowerCase()
            .replace(/[^a-z0-9\s-]/g, '')
            .replace(/\s+/g, '-')
            .replace(/-+/g, '-')
            .trim();
    }

    setupScrollSpy() {
        if (this.tocItems.length === 0) return;

        const observerOptions = {
            rootMargin: '-20% 0px -70% 0px',
            threshold: 0
        };

        const observer = new IntersectionObserver((entries) => {
            entries.forEach(entry => {
                const tocItem = this.tocItems.find(item => item.element === entry.target);
                if (tocItem) {
                    if (entry.isIntersecting) {
                        this.setActiveItem(tocItem);
                    }
                }
            });
        }, observerOptions);

        this.tocItems.forEach(item => {
            observer.observe(item.element);
        });

        // Also update on scroll for better accuracy
        let scrollTimeout;
        window.addEventListener('scroll', () => {
            clearTimeout(scrollTimeout);
            scrollTimeout = setTimeout(() => {
                this.updateActiveItemByScroll();
            }, 100);
        });
    }

    updateActiveItemByScroll() {
        const scrollPosition = window.scrollY + window.innerHeight / 3;
        let activeItem = null;

        for (let i = this.tocItems.length - 1; i >= 0; i--) {
            const item = this.tocItems[i];
            const rect = item.element.getBoundingClientRect();
            const absoluteTop = rect.top + window.scrollY;
            
            if (absoluteTop <= scrollPosition) {
                activeItem = item;
                break;
            }
        }

        if (activeItem) {
            this.setActiveItem(activeItem);
        }
    }

    setActiveItem(item) {
        if (this.activeItem === item) return;

        // Remove active state from previous item
        if (this.activeItem) {
            this.activeItem.link.style.borderLeftColor = 'transparent';
            this.activeItem.link.style.color = 'var(--page-foreground-color)';
            this.activeItem.link.style.fontWeight = 'normal';
            this.activeItem.link.style.background = 'transparent';
        }

        // Add active state to new item
        this.activeItem = item;
        item.link.style.borderLeftColor = 'var(--primary-color)';
        item.link.style.color = 'var(--primary-color)';
        item.link.style.fontWeight = 'bold';
        item.link.style.background = 'rgba(33, 150, 243, 0.1)';

        // Ensure active item is visible in TOC
        if (this.tocContainer) {
            const linkRect = item.link.getBoundingClientRect();
            const containerRect = this.tocContainer.getBoundingClientRect();
            
            if (linkRect.top < containerRect.top || linkRect.bottom > containerRect.bottom) {
                item.link.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
            }
        }
    }

    setupSmoothScroll() {
        this.tocItems.forEach(item => {
            item.link.addEventListener('click', (e) => {
                e.preventDefault();
                const target = document.getElementById(item.id);
                if (target) {
                    const offset = 80; // Account for fixed header
                    const targetPosition = target.getBoundingClientRect().top + window.scrollY - offset;
                    
                    window.scrollTo({
                        top: targetPosition,
                        behavior: 'smooth'
                    });

                    // Update URL without jumping
                    history.pushState(null, null, `#${item.id}`);
                }
            });
        });
    }
}

// Progress indicator
class ScrollProgress {
    constructor() {
        this.init();
    }

    init() {
        const progressBar = document.createElement('div');
        progressBar.className = 'scroll-progress';
        progressBar.style.cssText = `
            position: fixed;
            top: 0;
            left: 0;
            height: 3px;
            background: linear-gradient(90deg, var(--primary-color), var(--primary-light-color));
            z-index: 10000;
            transition: width 0.2s;
            width: 0%;
        `;
        
        document.body.appendChild(progressBar);
        
        const updateProgress = () => {
            const scrollHeight = document.documentElement.scrollHeight - document.documentElement.clientHeight;
            const scrollPosition = window.scrollY;
            const progress = (scrollPosition / scrollHeight) * 100;
            progressBar.style.width = `${progress}%`;
        };
        
        window.addEventListener('scroll', updateProgress);
        updateProgress();
    }
}

// Initialize when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', () => {
        new InteractiveTOC();
        new ScrollProgress();
    });
} else {
    new InteractiveTOC();
    new ScrollProgress();
}