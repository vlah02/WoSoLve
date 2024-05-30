from flask import Flask, request, render_template, session, redirect, url_for
import random
import re

app = Flask(__name__)
app.secret_key = 'poop'


class WordleState:
    def __init__(self):
        self.available_chars = "abcdefghijklmnopqrstuvwxyz"
        self.green_chars = [' '] * 5
        self.yellow_chars = [set() for _ in range(5)]
        self.required_chars = set()
        self.letter_counts = {}
        self.excluded_chars = set()

    def update_state(self, guess):
        minus_counts = {}
        plus_and_star_counts = {}

        for i in range(0, len(guess), 2):
            letter = guess[i]
            symbol = guess[i + 1]
            position = i // 2

            if symbol == '+':
                self.green_chars[position] = letter
                plus_and_star_counts[letter] = plus_and_star_counts.get(letter, 0) + 1
            elif symbol == '*':
                self.yellow_chars[position].add(letter)
                self.required_chars.add(letter)
                plus_and_star_counts[letter] = plus_and_star_counts.get(letter, 0) + 1
            elif symbol == '-':
                minus_counts[letter] = minus_counts.get(letter, 0) + 1

        for letter, count in minus_counts.items():
            if plus_and_star_counts.get(letter, 0) < count:
                self.available_chars = self.available_chars.replace(letter, '')

        for letter, count in plus_and_star_counts.items():
            self.letter_counts[letter] = max(self.letter_counts.get(letter, 0), count)

    def is_valid_word(self, word):
        # Check excluded characters first
        if any(letter in word for letter in self.excluded_chars):
            return False

        # Check required characters
        for c in self.required_chars:
            if c not in word:
                return False

        # Check the green characters are in the correct positions
        for i, c in enumerate(word):
            if self.green_chars[i] != ' ' and self.green_chars[i] != c:
                return False

        # Check the yellow characters are in the word but not in the specified positions
        for i, char_set in enumerate(self.yellow_chars):
            if any(c == word[i] for c in char_set):
                return False

        # Check the count of required characters
        for letter, count in self.letter_counts.items():
            if word.count(letter) < count:
                return False

        return True


def find_valid_words(word_list, state):
    valid_words = []
    for word in word_list:
        if state.is_valid_word(word):
            valid_words.append(word)
    return valid_words


def state_to_dict(state):
    return {
        "available_chars": state.available_chars,
        "green_chars": state.green_chars,
        "yellow_chars": [list(chars) for chars in state.yellow_chars],
        "required_chars": list(state.required_chars),
        "letter_counts": state.letter_counts
    }


def state_from_dict(state_dict):
    state = WordleState()
    state.available_chars = state_dict["available_chars"]
    state.green_chars = state_dict["green_chars"]
    state.yellow_chars = [set(chars) for chars in state_dict["yellow_chars"]]
    state.required_chars = set(state_dict["required_chars"])
    state.letter_counts = state_dict["letter_counts"]
    return state


@app.route('/', methods=['GET', 'POST'])
def index():
    words = []
    message = ""
    random_word = None

    word_list = []
    with open("words/wordle-la.txt", "r") as la_file:
        word_list.extend([word.strip() for word in la_file])
    with open("words/wordle-ta.txt", "r") as ta_file:
        word_list.extend([word.strip() for word in ta_file])

    if 'state' not in session:
        state = WordleState()
        session['state'] = state_to_dict(state)
        random_word = random.choice(word_list)
        session['random_word'] = random_word
    else:
        state = state_from_dict(session['state'])
        random_word = session['random_word']

    if request.method == 'POST':
        guess = request.form.get('guess')
        if len(guess) == 10 and re.match("([a-z][\+\-\*]){5}", guess):
            state.update_state(guess)
            words = find_valid_words(word_list, state)
            if words:
                random_word = random.choice(words)
                session['random_word'] = random_word
        else:
            message = "ERROR: Incorrect format!"

    session['state'] = state_to_dict(state)

    return render_template('index.html', words=words, message=message, random_word=random_word)


@app.route('/reset')
def reset():
    session.pop('state', None)
    session.pop('random_word', None)
    return redirect(url_for('index'))


if __name__ == '__main__':
    app.run(debug=True)
