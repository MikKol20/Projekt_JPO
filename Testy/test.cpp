#include "pch.h"
#include "../../GUIApp/DirectoryScanner.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

// Funkcja pomocnicza do tworzenia pliku w testowym katalogu
void createTestFile(const std::string& path) {
    std::ofstream file(path);
    file << "Test content";
    file.close();
}

// Test dla DirectoryScanner
TEST(DirectoryScannerTests, GetFilesWithValidExtension) {
    // Przygotowanie testowych plików
    std::string testDir = "test_directory";
    fs::create_directory(testDir);
    std::ofstream(testDir + "/file1.txt").close();
    std::ofstream(testDir + "/file2.txt").close();
    std::ofstream(testDir + "/file3.log").close();

    // Testowanie DirectoryScanner
    DirectoryScanner scanner(testDir, ".txt");
    auto files = scanner.getFiles();

    // Oczekiwane pliki
    auto file1 = (fs::path(testDir) / "file1.txt").string();
    auto file2 = (fs::path(testDir) / "file2.txt").string();

    EXPECT_NE(std::find(files.begin(), files.end(), file1), files.end());
    EXPECT_NE(std::find(files.begin(), files.end(), file2), files.end());

    // Sprz¹tanie
    fs::remove_all(testDir);
}


TEST(DirectoryScannerTests, GetFilesWithEmptyDirectory) {
    // Utworzenie pustego katalogu
    std::string testDir = "empty_dir";
    fs::create_directory(testDir);

    // Tworzenie instancji DirectoryScanner
    DirectoryScanner scanner(testDir, ".txt");
    std::vector<std::string> files = scanner.getFiles();

    // Oczekujemy pustej listy
    ASSERT_TRUE(files.empty());

    // Sprz¹tanie po teœcie
    fs::remove_all(testDir);
}

TEST(DirectoryScannerTests, GetFilesWithInvalidDirectory) {
    // Podajemy nieistniej¹cy katalog
    std::string invalidDir = "non_existing_dir";

    // Tworzenie instancji DirectoryScanner
    DirectoryScanner scanner(invalidDir, ".txt");

    // Metoda powinna zwróciæ pust¹ listê
    std::vector<std::string> files = scanner.getFiles();
    ASSERT_TRUE(files.empty());
}

TEST(DirectoryScannerTests, GetFilesWithNoMatchingExtension) {
    // Utworzenie katalogu z plikami o innych rozszerzeniach
    std::string testDir = "test_dir_no_match";
    fs::create_directory(testDir);
    createTestFile(testDir + "/file1.cpp");
    createTestFile(testDir + "/file2.md");

    // Tworzenie instancji DirectoryScanner z rozszerzeniem ".txt"
    DirectoryScanner scanner(testDir, ".txt");
    std::vector<std::string> files = scanner.getFiles();

    // Oczekujemy pustej listy, bo nie ma plików z rozszerzeniem .txt
    ASSERT_TRUE(files.empty());

    // Sprz¹tanie po teœcie
    fs::remove_all(testDir);
}
