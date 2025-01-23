#include "GUIApp.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QStringList>
#include <QListWidgetItem>
#include <QString>
#include <QStandardPaths>
#include <QFileInfo>
#include "../../GUIApp/DirectoryScanner.h"
#include "../../GUIApp/FileUtils.h"
#include "../../GUIApp/TextAnalyzer.h"
#include <locale>
#include <codecvt>

GUIApp::GUIApp(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // Po³¹czenia sygna³ów z przyciskami
    connect(ui.searchButton, &QPushButton::clicked, this, &GUIApp::onSearchButtonClicked);
    connect(ui.analyzeButton, &QPushButton::clicked, this, &GUIApp::onAnalyzeButtonClicked);
}

GUIApp::~GUIApp()
{
}

void GUIApp::onSearchButtonClicked() {
    setLocaleForPolish(); // Ustawienie odpowiedniego kodowania dla polskich znaków
    QString folderName = ui.folderNameEdit->text();  // Pobierz nazwê folderu z QLineEdit
    QString extension = ui.extensionLineEdit->text();  // Pobierz rozszerzenie z QLineEdit

    if (extension.isEmpty()) { // SprawdŸ, czy rozszerzenie jest puste
        QMessageBox::warning(this, "Error", "Musisz podac rozszerzenie.");
        return;
    }

    // Jeœli rozszerzenie nie jest puste, dodaj "." na pocz¹tku
    if (!extension.startsWith('.')) {
        extension = "." + extension;
    }

    // Domyœlna œcie¿ka to Pulpit
    QString rootPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    if (!folderName.isEmpty()) {
        // U¿ycie findSubfolder do znalezienia folderu
        auto foundPath = findSubfolder(rootPath.toStdString(), folderName.toStdString());
        if (!foundPath) {
            QMessageBox::warning(this, "Error", "Nie znaleziono folderu.");
            return;
        }
        rootPath = QString::fromStdString(foundPath->string());
    }

    // Konwersja na separator systemowy
    rootPath = QDir::fromNativeSeparators(rootPath);

    // Przechowuj pe³n¹ œcie¿kê katalogu, gdzie szukamy plików
    currentFolderPath = rootPath;

    // Wyœwietl œcie¿kê folderu w labelu
    ui.sciezkadostepu->setText("Przeszukiwany folder: " + rootPath);

    // U¿ycie DirectoryScanner do przeszukiwania folderu i jego podfolderów
    DirectoryScanner scanner(rootPath.toStdString(), extension.toStdString());
    std::vector<std::string> files = scanner.getFiles();  // U¿ywamy getFiles, które dzia³a rekurencyjnie

    // Wyœwietlanie plików, ale przechowujemy pe³n¹ œcie¿kê
    ui.resultList->clear();
    for (const std::string& file : files) {
        QString filePath = QString::fromStdString(file);  // Pe³na œcie¿ka pliku

        // Uzyskaj tylko nazwê pliku (bez œcie¿ki)
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();  // Tylko nazwa pliku

        QListWidgetItem* item = new QListWidgetItem(fileName);  // Dodaj nazwê pliku
        item->setData(Qt::UserRole, filePath);  // Zapisanie pe³nej œcie¿kê do danych itemu
        ui.resultList->addItem(item);
    }

    if (files.empty()) {
        QMessageBox::information(this, "Informacja", "Nie znaleziono plikow o rozszerzeniu " + extension);
    }
}

void GUIApp::onAnalyzeButtonClicked() {
    setLocaleForPolish(); // Ustawienie odpowiedniego kodowania dla polskich znaków
    QListWidgetItem* selectedItem = ui.resultList->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Error", "Nie wybrano pliku do analizy.");
        return;
    }

    // Pobieranie pe³n¹ œcie¿kê do pliku z danych itemu
    QString filePath = selectedItem->data(Qt::UserRole).toString();  // Pe³na œcie¿ka do pliku

    // Sprawdzanie, czy plik istnieje
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        QMessageBox::warning(this, "Error", "Nie znaleziono pliku: " + filePath);
        return;
    }

    // Wyœwietlenie pe³nej œcie¿ki pliku w labelu `sciezkadostepu`
    ui.sciezkadostepu->setText("Sciezka pliku: " + filePath);

    try {
        // Odczyt pliku przy u¿yciu funkcji do obs³ugi UTF-8
        std::wstring content = readFile(filePath.toStdWString());  //konwersja string <-> wstring
        content = removeBOM(content);  // Usuñ BOM

        // Analiza tekstu
        TextAnalyzer analyzer(content);
        int characterCount = analyzer.countCharacters();
        int wordCount = analyzer.countWords();
        int lineCount = analyzer.countLines();

        // Wyœwietlenie wyników analizy
        QMessageBox::information(this, "Analiza tekstu",
            "Plik: " + filePath + "\n"
            "Liczba znakow: " + QString::number(characterCount) + "\n"
            "Liczba slow: " + QString::number(wordCount) + "\n"
            "Liczba linii: " + QString::number(lineCount));
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", "Wystapi³ blad podczas analizy: " + QString::fromStdString(e.what()));
    }
}

int main(int argc, char* argv[]) {
   
    QApplication app(argc, argv);

   
    GUIApp mainWindow;
    mainWindow.show();  

 
    return app.exec();
}
