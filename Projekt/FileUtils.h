#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <cstdlib>
namespace fs = std::filesystem;
using std::string, std::wstring, std::optional;


wstring readFile(const std::wstring& filePath);
void setLocaleForPolish();
optional<fs::path> findSubfolder(const fs::path& baseDir, const std::string& folderName);
optional<fs::path> findFile(const fs::path& baseDir, const std::string& fileName);
string wstringToString(const std::wstring& wstr);
wstring removeBOM(const wstring& text);
wstring readFile(const string& filePath);

#endif // FILEUTILS_H
