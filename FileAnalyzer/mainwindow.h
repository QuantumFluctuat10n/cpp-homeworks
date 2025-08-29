#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "file_scanner.h"
#include "file_item.h"
#include <QThread>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QList>
#include <QTableWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QDateTime>
#include <QComboBox>
#include <QLineEdit>
#include <QElapsedTimer>
#include <QMenu>
#include <QUrl>
#include <QProcess>
#include <QDesktopServices>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectDirectory();
    void startScan();
    void stopScan();
    void exportResults();
    void showContextMenu(const QPoint &pos);

    // Слоти для worker
    void onFileFound(const FileItem &file);
    void onScanProgress(int filesProcessed, const QString &currentPath);
    void onScanFinished(const QList<FileItem> &allFiles);
    void onScanError(const QString &error);
private:
    void setupUI();
    void updateStatistics();

    Ui::MainWindow *ui;

    QElapsedTimer m_timer;

    // Threading
    QThread *m_scanThread;
    FileScanner *m_scanner;

    // Данні
    QList<FileItem> m_files;
    QString m_selectedPath;

    // UI компоненти
    QTableWidget *m_fileTable;
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;

    QLabel *m_barFilesCount;
    QLabel *m_barTotalSize;
    QLabel *m_totalSize;
    QLabel *m_largestFile;
    QLabel *m_mostCommonType;
    QLabel *m_filesCount;

    // Кнопки
    QPushButton *m_selectButton;
    QPushButton *m_scanButton;
    QPushButton *m_stopButton;
    QPushButton *m_exportButton;

    QLineEdit *m_filterEdit;
};

#endif // MAINWINDOW_H
