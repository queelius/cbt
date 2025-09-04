/**
 * Dark mode toggle for Doxygen Awesome
 */

class DarkModeToggle {
    constructor() {
        this.storageKey = 'doxygen-awesome-dark-mode';
        this.darkModeClass = 'dark-mode';
        this.init();
    }

    init() {
        // Create toggle button
        const button = document.createElement('button');
        button.className = 'dark-mode-toggle';
        button.innerHTML = this.getIcon(!this.isDarkMode());
        button.setAttribute('aria-label', 'Toggle dark mode');
        button.style.cssText = `
            position: fixed;
            bottom: 20px;
            right: 20px;
            z-index: 1000;
            background: var(--primary-color);
            color: white;
            border: none;
            border-radius: 50%;
            width: 50px;
            height: 50px;
            cursor: pointer;
            box-shadow: 0 2px 8px rgba(0,0,0,0.2);
            transition: transform 0.3s;
            display: flex;
            align-items: center;
            justify-content: center;
        `;

        button.addEventListener('click', () => this.toggle());
        button.addEventListener('mouseover', () => {
            button.style.transform = 'scale(1.1)';
        });
        button.addEventListener('mouseout', () => {
            button.style.transform = 'scale(1)';
        });

        document.body.appendChild(button);
        this.button = button;

        // Apply saved preference
        this.applySavedPreference();

        // Listen for system preference changes
        if (window.matchMedia) {
            window.matchMedia('(prefers-color-scheme: dark)').addEventListener('change', (e) => {
                if (!localStorage.getItem(this.storageKey)) {
                    this.applyDarkMode(e.matches);
                }
            });
        }
    }

    getIcon(isDark) {
        if (isDark) {
            return `<svg width="24" height="24" viewBox="0 0 24 24" fill="currentColor">
                <path d="M12 3c-4.97 0-9 4.03-9 9s4.03 9 9 9 9-4.03 9-9c0-.46-.04-.92-.1-1.36-.98 1.37-2.58 2.26-4.4 2.26-3 0-5.4-2.42-5.4-5.4 0-1.81.89-3.42 2.26-4.4-.44-.06-.9-.1-1.36-.1z"/>
            </svg>`;
        } else {
            return `<svg width="24" height="24" viewBox="0 0 24 24" fill="currentColor">
                <path d="M12 7c-2.76 0-5 2.24-5 5s2.24 5 5 5 5-2.24 5-5-2.24-5-5-5zM12 2v4M12 18v4M4.93 4.93l2.83 2.83M16.24 16.24l2.83 2.83M2 12h4M18 12h4M4.93 19.07l2.83-2.83M16.24 7.76l2.83-2.83"/>
            </svg>`;
        }
    }

    isDarkMode() {
        return document.documentElement.classList.contains(this.darkModeClass);
    }

    applySavedPreference() {
        const savedMode = localStorage.getItem(this.storageKey);
        if (savedMode !== null) {
            this.applyDarkMode(savedMode === 'true');
        } else if (window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches) {
            this.applyDarkMode(true);
        }
    }

    applyDarkMode(enable) {
        if (enable) {
            document.documentElement.classList.add(this.darkModeClass);
        } else {
            document.documentElement.classList.remove(this.darkModeClass);
        }
        if (this.button) {
            this.button.innerHTML = this.getIcon(!enable);
        }
    }

    toggle() {
        const newState = !this.isDarkMode();
        this.applyDarkMode(newState);
        localStorage.setItem(this.storageKey, newState);
    }
}

// Initialize when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', () => new DarkModeToggle());
} else {
    new DarkModeToggle();
}