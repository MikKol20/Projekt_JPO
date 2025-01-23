#ifndef TEXTANALYZER_H
#define TEXTANALYZER_H

#include <string>

using std::wstring;

class TextAnalyzer {
public:
    TextAnalyzer(const wstring& text);
    const wstring& getText() const;
    int countCharacters() const;
    int countWords() const;
    int countLines() const;

private:
    wstring text_;
};

#endif // TEXTANALYZER_H
