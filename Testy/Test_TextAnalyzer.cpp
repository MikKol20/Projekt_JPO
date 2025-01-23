#include "pch.h"
#include "../../GUIApp/TextAnalyzer.h"

TEST(TextAnalyzerTests, CountCharacters) {
    // Test pustego tekstu
    TextAnalyzer analyzer1(L"");
    EXPECT_EQ(analyzer1.countCharacters(), 0);

    // Test tekstu bez spacji
    TextAnalyzer analyzer2(L"projekt_JPO");
    EXPECT_EQ(analyzer2.countCharacters(), 11);

    // Test tekstu ze spacjami
    TextAnalyzer analyzer3(L"Projekt JPO");
    EXPECT_EQ(analyzer3.countCharacters(), 10);

    // Test tekstu z polskimi znakami 
    TextAnalyzer analyzer4(L"üÊπÍ≥ø");
    EXPECT_EQ(analyzer4.countCharacters(), 6);
}

TEST(TextAnalyzerTests, CountWords) {
    // Test pustego tekstu
    TextAnalyzer analyzer1(L"");
    EXPECT_EQ(analyzer1.countWords(), 0);

    // Test jednego s≥owa
    TextAnalyzer analyzer2(L"JPO");
    EXPECT_EQ(analyzer2.countWords(), 1);

    // Test kilku s≥Ûw oddzielonych spacjami
    TextAnalyzer analyzer3(L"Projekt JPO Analizator Tekstu");
    EXPECT_EQ(analyzer3.countWords(), 4);

    // Test tekstu z polskimi znakami
    TextAnalyzer analyzer4(L"è≥o ∆wierÊ kπsaÊ");
    EXPECT_EQ(analyzer4.countWords(), 3);

    // Test tekstu z wieloma spacjami
    TextAnalyzer analyzer5(L"  Projekt    jpo    ");
    EXPECT_EQ(analyzer5.countWords(), 2);
}

TEST(TextAnalyzerTests, CountLines) {
    // Test pustego tekstu
    TextAnalyzer analyzer1(L"");
    EXPECT_EQ(analyzer1.countLines(), 1);

    // Test jednego wiersza
    TextAnalyzer analyzer2(L"Projekt JPO");
    EXPECT_EQ(analyzer2.countLines(), 1);

    // Test dwÛch wierszy
    TextAnalyzer analyzer3(L"Projekt JPO\nAnalizator Tekstu");
    EXPECT_EQ(analyzer3.countLines(), 2);

    // Test wielu wierszy zakoÒczonych nowπ liniπ
    TextAnalyzer analyzer4(L"Projekt\nJPO\nAnalizator\n");
    EXPECT_EQ(analyzer4.countLines(), 4);

    // Test tekstu z pustymi wierszami
    TextAnalyzer analyzer5(L"Projekt\n\nJPO\n\n");
    EXPECT_EQ(analyzer5.countLines(), 5);
}

TEST(TextAnalyzerTests, GetText) {
    // Test, czy tekst jest poprawnie przechowywany
    std::wstring inputText = L"sample text";
    TextAnalyzer analyzer(inputText);
    EXPECT_EQ(analyzer.getText(), inputText);
}
