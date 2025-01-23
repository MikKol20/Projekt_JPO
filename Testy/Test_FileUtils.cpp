#include "gtest/gtest.h"
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include "../../GUIApp/FileUtils.h"

namespace fs = std::filesystem;
using std::string, std::ofstream, std::wstring;

TEST(ReadFileTests, ReadValidFile) {
    // Tworzymy plik testowy
    string testFilePath = "test.txt";
    ofstream testFile(testFilePath);
    testFile << "Projekt_JPO";
    testFile.close();

    // Sprawdzamy, czy funkcja poprawnie odczytuje plik
    try {
        wstring content = readFile(testFilePath);
        EXPECT_EQ(content, L"Projekt_JPO");
    }
    catch (const std::ios_base::failure& e) {
        FAIL() << "Exception thrown: " << e.what();
    }
    fs::remove(testFilePath);
}

TEST(ReadFileTests, ReadInvalidFile) {
    try {
        readFile("non_existent_file.txt");
        FAIL() << "Expected exception not thrown.";
    }
    catch (const std::ios_base::failure& e) {
        EXPECT_STREQ(e.what(), "Nie uda³o siê otworzyæ pliku.: iostream stream error");
    }
}

TEST(RemoveBOMTests, RemoveBOMFromText) {
    wstring input = L"\xFEFFProjekt_JPO";
    wstring expected = L"Projekt_JPO";

    EXPECT_EQ(removeBOM(input), expected);
}

TEST(RemoveBOMTests, NoBOMInText) {
    wstring input = L"Projekt_JPO";
    EXPECT_EQ(removeBOM(input), input);
}
TEST(FindSubfolderTests, FindSubfolderExists) {
    // Tworzymy katalog i podkatalog
    string baseDir = "testDir";
    string subfolder = "subfolder";

    fs::create_directory(baseDir);
    fs::create_directory(baseDir + "/" + subfolder);

    // Sprawdzamy, czy funkcja znajduje podkatalog
    fs::path basePath(baseDir);
    auto foundSubfolder = findSubfolder(basePath, subfolder);
    EXPECT_TRUE(foundSubfolder.has_value());
    EXPECT_EQ(foundSubfolder.value().filename(), subfolder);


    fs::remove_all(baseDir);
}

TEST(FindSubfolderTests, FindSubfolderDoesNotExist) {
    // Tworzymy pusty katalog
    string baseDir = "testDir";
    fs::create_directory(baseDir);

    // Sprawdzamy, czy funkcja nie znajduje nieistniej¹cego podkatalogu
    fs::path basePath(baseDir);
    auto foundSubfolder = findSubfolder(basePath, "non_existent_subfolder");
    EXPECT_FALSE(foundSubfolder.has_value());


    fs::remove(baseDir);
}
TEST(FindFileTests, FindFileExists) {
    // Tworzymy katalog i plik testowy
    string baseDir = "testDir";
    string fileName = "testFile.txt";

    fs::create_directory(baseDir);
    ofstream(baseDir + "/" + fileName) << "Test content";

    // Sprawdzamy, czy funkcja znajduje plik
    fs::path basePath(baseDir);
    auto foundFile = findFile(basePath, fileName);
    EXPECT_TRUE(foundFile.has_value());
    EXPECT_EQ(foundFile.value().filename(), fileName);


    fs::remove_all(baseDir);
}

TEST(FindFileTests, FindFileDoesNotExist) {
    // Tworzymy pusty katalog
    string baseDir = "testDir";
    fs::create_directory(baseDir);

    // Sprawdzamy, czy funkcja nie znajduje nieistniej¹cego pliku
    fs::path basePath(baseDir);
    auto foundFile = findFile(basePath, "non_existent_file.txt");
    EXPECT_FALSE(foundFile.has_value());


    fs::remove(baseDir);
}
TEST(SetLocaleForPolishTests, SetLocaleForPolish) {
    try {
        setLocaleForPolish();

        SUCCEED();
    }
    catch (const std::exception& e) {
        FAIL() << "Exception thrown during locale setup: " << e.what();
    }
}


