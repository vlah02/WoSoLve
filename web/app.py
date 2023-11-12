# from flask import Flask, request, render_template, session, redirect, url_for
# import random
# import re
# import json

# app = Flask(__name__)
# app.secret_key = 'poop' 

# class WordleState:
#     def __init__(self):
#         self.available_chars = "abcdefghijklmnopqrstuvwxyz"
#         self.green_chars = [' '] * 5
#         self.yellow_chars = [set() for _ in range(5)]
#         self.required_chars = set()
#         self.letter_counts = {}

#     def update_state(self, guess):
#         minus_counts = {}
#         plus_and_star_counts = {}

#         for i in range(0, len(guess), 2):
#             letter = guess[i]
#             symbol = guess[i + 1]
#             position = i // 2

#             if symbol == '+':
#                 self.green_chars[position] = letter
#                 plus_and_star_counts[letter] = plus_and_star_counts.get(letter, 0) + 1
#             elif symbol == '*':
#                 self.yellow_chars[position].add(letter)
#                 self.required_chars.add(letter)
#                 plus_and_star_counts[letter] = plus_and_star_counts.get(letter, 0) + 1
#             elif symbol == '-':
#                 minus_counts[letter] = minus_counts.get(letter, 0) + 1

#         for letter, count in minus_counts.items():
#             if plus_and_star_counts.get(letter, 0) < count:
#                 self.available_chars = self.available_chars.replace(letter, '')

#         for letter, count in plus_and_star_counts.items():
#             self.letter_counts[letter] = count

#     def is_valid_word(self, word):
#         for c in self.required_chars:
#             if c not in word:
#                 return False

#         for letter, count in self.letter_counts.items():
#             if word.count(letter) != count:
#                 return False

#         return True


# def generate_words(characters, current_length, current_word, state, output_set):
#     if current_length == 5:
#         if state.is_valid_word(current_word):
#             output_set.add(current_word)
#         return

#     for c in characters:
#         if state.green_chars[current_length] != ' ' and state.green_chars[current_length] != c:
#             continue
#         if c in state.yellow_chars[current_length]:
#             continue

#         generate_words(characters, current_length + 1, current_word + c, state, output_set)

# def state_to_dict(state):
#     return {
#         "available_chars": state.available_chars,
#         "green_chars": state.green_chars,
#         "yellow_chars": [list(chars) for chars in state.yellow_chars],
#         "required_chars": list(state.required_chars),
#         "letter_counts": state.letter_counts
#     }

# def state_from_dict(state_dict):
#     state = WordleState()
#     state.available_chars = state_dict["available_chars"]
#     state.green_chars = state_dict["green_chars"]
#     state.yellow_chars = [set(chars) for chars in state_dict["yellow_chars"]]
#     state.required_chars = set(state_dict["required_chars"])
#     state.letter_counts = state_dict["letter_counts"]
#     return state


# @app.route('/', methods=['GET', 'POST'])
# def index():
#     message = ""
#     random_word = None
#     words = []

#     if 'state' in session:
#         state = state_from_dict(session['state'])
#     else:
#         state = WordleState()

#     if request.method == 'POST':
#         guess = request.form.get('guess')
#         if len(guess) == 10 and re.match("([a-z][\+\-\*]){5}", guess):
#             state.update_state(guess)
#             output_set = set()
#             generate_words(state.available_chars, 0, "", state, output_set)

#             generated_words = list(output_set)
#             compare_words = []

#             with open("src/words/wordle-ta.txt", "r") as compare_file:
#                 for word in compare_file:
#                     word = word.strip()
#                     if word in generated_words:
#                         compare_words.append(word)

#             words = compare_words
#             if words:
#                 random_word = random.choice(words)
#         else:
#             message = "ERROR: Incorrect format!"

#     session['state'] = state_to_dict(state)

#     return render_template('index.html', words=words, message=message, random_word=random_word)

# @app.route('/reset')
# def reset():
#     session.pop('state', None)
#     return redirect(url_for('index'))


# if __name__ == '__main__':
#     app.run(debug=True)



from flask import Flask, request, render_template, session, redirect, url_for
import random
import re
import json

app = Flask(__name__)
app.secret_key = 'poop' 

class WordleState:
    def __init__(self):
        self.available_chars = "abcdefghijklmnopqrstuvwxyz"
        self.green_chars = [' '] * 5
        self.yellow_chars = [set() for _ in range(5)]
        self.required_chars = set()
        self.letter_counts = {}

    def update_state(self, guess):
        minus_counts = {}
        plus_and_star_counts = {}

        for i in range(0, len(guess), 2):
            letter = guess[i]
            symbol = guess[i + 1]
            position = i // 2

            if symbol in ['+', '*']:
                self.green_chars[position] = letter if symbol == '+' else self.green_chars[position]
                self.yellow_chars[position].add(letter) if symbol == '*' else None
                self.required_chars.add(letter)
                plus_and_star_counts[letter] = plus_and_star_counts.get(letter, 0) + 1
            elif symbol == '-':
                minus_counts[letter] = minus_counts.get(letter, 0) + 1

        for letter, count in minus_counts.items():
            if plus_and_star_counts.get(letter, 0) < count:
                self.available_chars = self.available_chars.replace(letter, '', count - plus_and_star_counts.get(letter, 0))

    def is_valid_word(self, word):
        for c in self.required_chars:
            if c not in word:
                return False

        for i, letter in enumerate(self.green_chars):
            if letter != ' ' and word[i] != letter:
                return False

        for i, chars in enumerate(self.yellow_chars):
            if word[i] in chars:
                return False

        return all(word.count(c) >= self.letter_counts.get(c, 1) for c in self.required_chars)


def generate_words(characters, current_length, current_word, state, output_set):
    if current_length == 5:
        if state.is_valid_word(current_word):
            output_set.add(current_word)
        return

    for c in characters:
        if state.green_chars[current_length] != ' ' and state.green_chars[current_length] != c:
            continue
        if c in state.yellow_chars[current_length]:
            continue

        generate_words(characters, current_length + 1, current_word + c, state, output_set)

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
    message = ""
    random_word = None
    words = []

    if 'state' in session:
        state = state_from_dict(session['state'])
    else:
        state = WordleState()

    if request.method == 'POST':
        guess = request.form.get('guess')
        if len(guess) == 10 and re.match("([a-z][\+\-\*]){5}", guess):
            state.update_state(guess)
            output_set = set()
            generate_words(state.available_chars, 0, "", state, output_set)

            generated_words = list(output_set)
            compare_words = []

            with open("src/words/wordle-ta.txt", "r") as compare_file:
                for word in compare_file:
                    word = word.strip()
                    if word in generated_words:
                        compare_words.append(word)

            words = compare_words
            if words:
                random_word = random.choice(words)
        else:
            message = "ERROR: Incorrect format!"

    session['state'] = state_to_dict(state)

    return render_template('index.html', words=words, message=message, random_word=random_word)

@app.route('/reset')
def reset():
    session.pop('state', None)
    return redirect(url_for('index'))


if __name__ == '__main__':
    app.run(debug=True)