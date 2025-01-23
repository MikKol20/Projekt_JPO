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

    // Po��czenia sygna��w z przyciskami
    connect(ui.searchButton, &QPushButton::clicked, this, &GUIApp::onSearchButtonClicked);
    connect(ui.analyzeButton, &QPushButton::clicked, this, &GUIApp::onAnalyzeButtonClicked);
}

GUIApp::~GUIApp()
{
}

void GUIApp::onSearchButtonClicked() {
    setLocaleForPolish(); // Ustawienie odpowiedniego kodowania dla polskich znak�w
    QString folderName = ui.folderNameEdit->text();  // Pobierz nazw� folderu z QLineEdit
    QString extension = ui.extensionLineEdit->text();  // Pobierz rozszerzenie z QLineEdit

    if (extension.isEmpty()) { // Sprawd�, czy rozszerzenie jest puste
        QMessageBox::warning(this, "Error", "Musisz podac rozszerzenie.");
        return;
    }

    // Je�li rozszerzenie nie jest puste, dodaj "." na pocz�tku
    if (!extension.startsWith('.')) {
        extension = "." + extension;
    }

    // Domy�lna �cie�ka to Pulpit
    QString rootPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    if (!folderName.isEmpty()) {
        // U�ycie findSubfolder do znalezienia folderu
        auto foundPath = findSubfolder(rootPath.toStdString(), folderName.toStdString());
        if (!foundPath) {
            QMessageBox::warning(this, "Error", "Nie znaleziono folderu.");
            return;
        }
        rootPath = QString::fromStdString(foundPath->string());
    }

    // Konwersja na separator systemowy
    rootPath = QDir::fromNativeSeparators(rootPath);

    // Przechowuj pe�n� �cie�k� katalogu, gdzie szukamy plik�w
    currentFolderPath = rootPath;

    // Wy�wietl �cie�k� folderu w labelu
    ui.sciezkadostepu->setText("Przeszukiwany folder: " + rootPath);

    // U�ycie DirectoryScanner do przeszukiwania folderu i jego podfolder�w
    DirectoryScanner scanner(rootPath.toStdString(), extension.toStdString());
    std::vector<std::string> files = scanner.getFiles();  // U�ywamy getFiles, kt�re dzia�a rekurencyjnie

    // Wy�wietlanie plik�w, ale przechowujemy pe�n� �cie�k�
    ui.resultList->clear();
    for (const std::string& file : files) {
        QString filePath = QString::fromStdString(file);  // Pe�na �cie�ka pliku

        // Uzyskaj tylko nazw� pliku (bez �cie�ki)
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();  // Tylko nazwa pliku

        QListWidgetItem* item = new QListWidgetItem(fileName);  // Dodaj nazw� pliku
        item->setData(Qt::UserRole, filePath);  // Zapisanie pe�nej �cie�k� do danych itemu
        ui.resultList->addItem(item);
    }

    if (files.empty()) {
        QMessageBox::information(this, "Informacja", "Nie znaleziono plikow o rozszerzeniu " + extension);
    }
}

void GUIApp::onAnalyzeButtonClicked() {
    setLocaleForPolish(); // Ustawienie odpowiedniego kodowania dla polskich znak�w
    QListWidgetItem* selectedItem = ui.resultList->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Error", "Nie wybrano pliku do analizy.");
        return;
    }

    // Pobieranie pe�n� �cie�k� do pliku z danych itemu
    QString filePath = selectedItem->data(Qt::UserRole).toString();  // Pe�na �cie�ka do pliku

    // Sprawdzanie, czy plik istnieje
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        QMessageBox::warning(this, "Error", "Nie znaleziono pliku: " + filePath);
        return;
    }

    // Wy�wietlenie pe�nej �cie�ki pliku w labelu `sciezkadostepu`
    ui.sciezkadostepu->setText("Sciezka pliku: " + filePath);

    try {
        // Odczyt pliku przy u�yciu funkcji do obs�ugi UTF-8
        std::wstring content = readFile(filePath.toStdWString());  //konwersja string <-> wstring
        content = removeBOM(content);  // Usu� BOM

        // Analiza tekstu
        TextAnalyzer analyzer(content);
        int characterCount = analyzer.countCharacters();
        int wordCount = analyzer.countWords();
        int lineCount = analyzer.countLines();

        // Wy�wietlenie wynik�w analizy
        QMessageBox::information(this, "Analiza tekstu",
            "Plik: " + filePath + "\n"
            "Liczba znakow: " + QString::number(characterCount) + "\n"
            "Liczba slow: " + QString::number(wordCount) + "\n"
            "Liczba linii: " + QString::number(lineCount));
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", "Wystapi� blad podczas analizy: " + QString::fromStdString(e.what()));
    }
}

int main(int argc, char* argv[]) {
   
    QApplication app(argc, argv);

   
    GUIApp mainWindow;
    mainWindow.show();  

 
    return app.exec();
}
