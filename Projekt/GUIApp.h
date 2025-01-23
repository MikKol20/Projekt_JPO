#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GUIApp.h"

class GUIApp : public QMainWindow
{
    Q_OBJECT

public:
    GUIApp(QWidget* parent = nullptr);
    ~GUIApp();

private slots:
    void onSearchButtonClicked();
    void onAnalyzeButtonClicked();
private:
    QString currentFolderPath;  
    QString findFolder(const QString& rootPath, const QString& folderName);
    QString rootPath; 
    
    
private:
    Ui::GUIAppClass ui;
};
