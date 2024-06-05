var selectedLetter = '';
var lastSelectedLetter = '';
var preventModalClose = false;

function addLetterToInput(letter, colorCode) {
    var guessContainer = document.getElementById('guessContainer');
    var hiddenGuess = document.getElementById('hiddenGuess');

    var placeholderBoxes = guessContainer.querySelectorAll('.placeholder-box');
    if (guessContainer.children.length - placeholderBoxes.length < 5) {
        var box = document.createElement('div');
        box.classList.add('guess-box');
        box.textContent = letter.toUpperCase();
        applyColorToBox(box, colorCode);
        guessContainer.replaceChild(box, placeholderBoxes[0]);

        box.style.opacity = 0;
        requestAnimationFrame(() => {
            box.style.opacity = 1;
        });

        hiddenGuess.value += letter + colorCode;
    }
}

function applyColorToBox(box, colorCode) {
    switch(colorCode) {
        case '+':
            box.style.backgroundColor = '#48bb78';
            break;
        case '*':
            box.style.backgroundColor = '#ecc94b';
            break;
        case '-':
            box.style.backgroundColor = '#f56565';
            break;
    }
}

function removeLastLetter() {
    var guessContainer = document.getElementById('guessContainer');
    var hiddenGuess = document.getElementById('hiddenGuess');

    var guessBoxes = guessContainer.querySelectorAll('.guess-box');
    if (guessBoxes.length > 0) {
        var lastBox = guessBoxes[guessBoxes.length - 1];
        lastBox.style.opacity = 0;
        lastBox.addEventListener('transitionend', function() {
            var placeholderBox = document.createElement('div');
            placeholderBox.classList.add('placeholder-box');
            guessContainer.replaceChild(placeholderBox, lastBox);
        });

        if (hiddenGuess.value.length > 0) {
            hiddenGuess.value = hiddenGuess.value.slice(0, -2);
        }
    }
}

function chooseColor(letter, event) {
    selectedLetter = letter;
    var colorPopup = document.getElementById('colorPopup');

    if (letter === lastSelectedLetter && colorPopup.classList.contains('visible')) {
        colorPopup.classList.remove('visible');
        lastSelectedLetter = '';
        return;
    }

    lastSelectedLetter = letter;

    if (event) {
        var buttonRect = event.target.getBoundingClientRect();
        colorPopup.style.top = `${buttonRect.top + window.scrollY - 5}px`;
        colorPopup.style.left = `${buttonRect.left + buttonRect.width / 2 - colorPopup.offsetWidth / 2}px`;
        colorPopup.style.transform = 'translateY(-100%)';
    }

    colorPopup.classList.add('visible');
}

function chooseColorFromKey(letter, event) {
    var colorPopup = document.getElementById('colorPopup');

    var keyElement = document.querySelector(`.key[data-key="${letter}"]`);
    if (keyElement) {
        if (keyElement.classList.contains('selected')) {
            keyElement.classList.remove('selected');
            colorPopup.classList.remove('visible');
            selectedLetter = '';
        } else {
            var previouslySelectedKey = document.querySelector('.key.selected');
            if (previouslySelectedKey) {
                previouslySelectedKey.classList.remove('selected');
            }
            selectedLetter = letter;
            keyElement.classList.add('selected');

            var buttonRect = keyElement.getBoundingClientRect();
            colorPopup.style.top = `${buttonRect.top + window.scrollY - 5}px`;
            colorPopup.style.left = `${buttonRect.left + buttonRect.width / 2 - colorPopup.offsetWidth / 2}px`;
            colorPopup.style.transform = 'translateY(-100%)';

            colorPopup.classList.add('visible');
        }
    }
}

function addColorToInput(symbol) {
    if (!selectedLetter) {
        showErrorPopup("Please select a letter");
        return;
    }
    addLetterToInput(selectedLetter, symbol);
    document.getElementById('colorPopup').classList.remove('visible');
    var selectedKey = document.querySelector(`.key[data-key="${selectedLetter}"]`);
    if (selectedKey) {
        selectedKey.classList.remove('selected');
    }
    lastSelectedLetter = '';
    selectedLetter = '';
}

function showErrorPopup(message) {
    var errorPopup = document.getElementById('errorPopup');
    errorPopup.textContent = message;
    errorPopup.classList.add('visible');
    setTimeout(() => {
        errorPopup.classList.remove('visible');
    }, 2000);
}

window.addEventListener('keydown', function(event) {
    const key = event.key.toLowerCase();
    const isPrintableChar = key.length === 1 && /[a-z]/i.test(key);
    var colorPopup = document.getElementById('colorPopup');

    if (isPrintableChar) {
        event.preventDefault();
        chooseColorFromKey(key, event);
    } else if (event.key.toLowerCase() === 'backspace') {
        if (colorPopup.classList.contains('visible')) {
            colorPopup.classList.remove('visible');
            lastSelectedLetter = '';
        } else {
            event.preventDefault();
            removeLastLetter();
        }
    } else if (key === 'enter') {
        event.preventDefault();
        document.querySelector('[type="submit"]').click();
    } else if (key === 'escape') {
        window.location.href='/reset';
    } else if (key === '1') {
        addColorToInput('+');
    } else if (key === '2') {
        addColorToInput('*');
    } else if (key === '3') {
        addColorToInput('-');
    }
});

document.addEventListener('click', function(event) {
    var colorPopup = document.getElementById('colorPopup');
    var isClickInsideElement = colorPopup.contains(event.target) || event.target.classList.contains('key');
    if (!isClickInsideElement) {
        colorPopup.classList.remove('visible');
        lastSelectedLetter = '';
        selectedLetter = '';
        var previouslySelectedKey = document.querySelector('.key.selected');
        if (previouslySelectedKey) {
            previouslySelectedKey.classList.remove('selected');
        }
    }
});

document.querySelectorAll('.key').forEach(button => {
    button.addEventListener('click', function(event) {
        chooseColorFromKey(this.textContent.toLowerCase(), event);
    });
});

document.querySelector('#backspace-key').addEventListener('click', function() {
    removeLastLetter();
});

document.querySelector('form').addEventListener('submit', function(event) {
    var guessValue = document.getElementById('hiddenGuess').value;
    if (!guessValue.match(/^([a-z][\+\-\*]){5}$/)) {
        event.preventDefault();
        showErrorPopup("ERROR: Incorrect format!");
    }
});

const themeToggleBtn = document.getElementById('theme-toggle');
const themeIcon = document.getElementById('theme-icon');
const currentTheme = localStorage.getItem('theme') || 'light';

if (currentTheme === 'dark') {
    document.body.classList.add('dark-mode');
    themeIcon.classList.remove('fa-moon');
    themeIcon.classList.add('fa-sun');
}

themeToggleBtn.addEventListener('click', () => {
    document.body.classList.toggle('dark-mode');
    const theme = document.body.classList.contains('dark-mode') ? 'dark' : 'light';
    localStorage.setItem('theme', theme);

    if (theme === 'dark') {
        themeIcon.classList.add('icon-transition-rotate');
        setTimeout(() => {
            themeIcon.classList.remove('fa-moon');
            themeIcon.classList.add('fa-sun');
            themeIcon.classList.remove('icon-transition-rotate');
        }, 250);
    } else {
        themeIcon.classList.add('icon-transition-scale');
        setTimeout(() => {
            themeIcon.classList.remove('fa-sun');
            themeIcon.classList.add('fa-moon');
            themeIcon.classList.remove('icon-transition-scale');
        }, 250);
    }
});

var gif = document.getElementById('movingGif');
var width = window.innerWidth;
var height = window.innerHeight;
var x = Math.random() * (width - gif.clientWidth);
var y = Math.random() * (height - gif.clientHeight);
var vx = 2;
var vy = 2;

function moveGif() {
    x += vx;
    y += vy;

    if (x + gif.clientWidth >= width || x <= 0) {
        vx *= -1;
    }

    if (y + gif.clientHeight >= height || y <= 0) {
        vy *= -1;
    }

    gif.style.left = x + 'px';
    gif.style.top = y + 'px';

    requestAnimationFrame(moveGif);
}

let clickCount = 0;
const logo = document.getElementById('logo');

logo.addEventListener('click', () => {
    clickCount += 1;
    if (clickCount >= 15) {
        gif.style.display = 'block';
        moveGif();
    }
});

window.addEventListener('resize', () => {
    width = window.innerWidth;
    height = window.innerHeight;
});

window.addEventListener('keyup', function(event) {
    const key = event.key.toLowerCase();
    const keyElement = document.querySelector(`.key[data-key="${key}"]`);
    if (keyElement) {
        keyElement.classList.remove('pressed');
    }
});

const customCheckbox = document.getElementById('customCheckbox');
const hiddenCheckbox = document.getElementById('includeExtended');

customCheckbox.addEventListener('click', () => {
    customCheckbox.classList.toggle('checked');
    hiddenCheckbox.checked = !hiddenCheckbox.checked;
    localStorage.setItem('includeExtended', hiddenCheckbox.checked);
});

if (localStorage.getItem('includeExtended') === 'true') {
    customCheckbox.classList.add('checked');
    hiddenCheckbox.checked = true;
}

document.getElementById('howToPlay').addEventListener('click', function() {
    preventModalClose = true;
    const modal = document.getElementById('howToPlayModal');
    modal.classList.remove('fade-out');
    modal.style.display = 'flex';
    setTimeout(() => {
        modal.style.opacity = '1';
        preventModalClose = false;
    }, 10);
    if (document.body.classList.contains('dark-mode')) {
        modal.querySelector('.modal-content').classList.add('dark-mode');
    } else {
        modal.querySelector('.modal-content').classList.remove('dark-mode');
    }
});

document.addEventListener('click', function(event) {
    const modal = document.getElementById('howToPlayModal');
    const modalContent = document.querySelector('.modal-content');
    if (!preventModalClose && modal.style.display === 'flex' && !modalContent.contains(event.target)) {
        closeModal();
    }
});

function closeModal() {
    const modal = document.getElementById('howToPlayModal');
    modal.classList.add('fade-out');
    setTimeout(() => {
        modal.style.display = 'none';
        modal.classList.remove('fade-out');
    }, 300);
}
