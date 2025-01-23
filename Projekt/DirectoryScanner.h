#pragma once
#include <string>
#include <vector>

class DirectoryScanner {
public:
    DirectoryScanner(const std::string& directory, const std::string& extension);
    std::vector<std::string> getFiles() const;

private:
    std::string directory;
    std::string extension;
};
