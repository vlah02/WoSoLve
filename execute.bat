@echo off
echo Hi, this is WAT (Wordle Assistant Tool), how about you first try a random word?

:: Generate a random guess by indexing guesses.txt and printing a random line
g++ helper/random.cpp -o helper/random.exe
if %ERRORLEVEL% == 0 (
    .\helper\random.exe
) else (
    echo Compilation failed.
)

echo:

:: Call the function for guessing the target word
g++ src/generate.cpp -o src/generate.exe
if %ERRORLEVEL% == 0 (
    .\src\generate.exe
) else (
    echo Compilation failed.
)


pause