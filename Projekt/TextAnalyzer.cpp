#include "TextAnalyzer.h"
#include <cctype>

TextAnalyzer::TextAnalyzer(const wstring& text) : text_(text) {}

const wstring& TextAnalyzer::getText() const { return text_; }

int TextAnalyzer::countCharacters() const {
    int totalCharacters = 0;
    for (auto c : text_) {
        if (c != L' ') {
            ++totalCharacters;
        }
    }
    return totalCharacters;
}

int TextAnalyzer::countWords() const {
    int words = 0;
    bool inWord = false;

    for (auto c : text_) {
        if (iswspace(c)) {
            if (inWord) {
                inWord = false;
                ++words;
            }
        }
        else {
            inWord = true;
        }
    }

    if (inWord) {
        ++words;
    }

    return words;
}

int TextAnalyzer::countLines() const {
    int lines = 0;
    for (auto c : text_) {
        if (c == L'\n') {
            ++lines;
        }
    }
    return lines + 1;
}
