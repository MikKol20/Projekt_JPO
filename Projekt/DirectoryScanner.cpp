#include "DirectoryScanner.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
using std::string, std::vector, std::cerr, std::endl;


DirectoryScanner::DirectoryScanner(const string& directory, const string& extension)
    : directory(directory), extension(extension) {
}

vector<string> DirectoryScanner::getFiles() const {
    vector<string> files;

    try {
        // Przeszukiwanie katalogów i podkatalogów
        for (const auto& entry : fs::recursive_directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().extension() == extension) {
                files.push_back(entry.path().string());
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        cerr << "Blad dostepu do katalogu: " << e.what() <<endl;
    }

    return files;
}
