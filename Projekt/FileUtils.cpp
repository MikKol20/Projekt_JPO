#include "FileUtils.h"
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <cwchar>
#include <locale>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <optional>

namespace fs = std::filesystem;
using std::ifstream, std::ios, std::ios_base, std::vector, std::wstring, std::string, std::wifstream, std::locale, std::runtime_error, std::optional;

// Funkcja do odczytu pliku w formacie wstring
wstring readFile(const wstring& filePath) {
    wifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        throw ios_base::failure("Nie udalo siê otworzyc pliku.");
    }
    file.imbue(locale("")); // Ustawienie lokalizacji na domyœln¹
    std::wstringstream buffer;
    buffer << file.rdbuf();
    return removeBOM(buffer.str());
}

// Funkcja do ustawienia polskiej lokalizacji

void setLocaleForPolish() {
    locale::global(std::locale("pl_PL.UTF-8"));
}


// Funkcja do konwersji std::wstring na std::string
string wstringToString(const wstring& wstr) {
    try {
        string result(wstr.begin(), wstr.end());
        return result;
    }
    catch (...) {
        throw runtime_error("B³¹d konwersji wstring na string.");
    }
}

// Funkcja do znalezienia podfolderu w katalogu
optional<fs::path> findSubfolder(const fs::path& baseDir, const string& folderName) {
    for (const auto& entry : fs::recursive_directory_iterator(baseDir)) {
        if (entry.is_directory() && entry.path().filename() == folderName) {
            return entry.path();
        }
    }
    return std::nullopt;
}

// Funkcja do znalezienia pliku w katalogu
optional<fs::path> findFile(const fs::path& baseDir, const string& fileName) {
    for (const auto& entry : fs::recursive_directory_iterator(baseDir)) {
        if (entry.is_regular_file() && entry.path().filename() == fileName) {
            return entry.path();
        }
    }
    return std::nullopt;
}

// Funkcja do usuniêcia BOM z wstring
wstring removeBOM(const wstring& text) {
    if (!text.empty() && text[0] == 0xFEFF) {
        return text.substr(1); // Usuniêcie BOM
    }
    return text;
}

// Funkcja do odczytu pliku i zwrócenia go jako wstring
wstring readFile(const string& filePath) {
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        throw ios_base::failure("Nie uda³o siê otworzyæ pliku.");
    }

    vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    wstring result;

    try {
        mbstate_t state = mbstate_t(); // Stan konwersji
        const char* data = buffer.data();
        const char* end = data + buffer.size();
        wchar_t wideChar;

        while (data < end) {
            size_t len = mbrtowc(&wideChar, data, end - data, &state);
            if (len == static_cast<size_t>(-1) || len == static_cast<size_t>(-2)) {
                throw ios_base::failure("B³¹d konwersji UTF-8 na wstring.");
            }
            result += wideChar;
            data += len;
        }
    }
    catch (...) {
        throw ios_base::failure("B³¹d przy dekodowaniu UTF-8.");
    }

    return removeBOM(result);
}
