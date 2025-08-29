#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();

    m_scanThread = new QThread(this);
    m_scanner = new FileScanner();
    m_scanner->moveToThread(m_scanThread);

    connect(m_scanThread, &QThread::finished, m_scanner, &QObject::deleteLater);

    connect(this, &MainWindow::destroyed, m_scanThread, &QThread::quit);

    connect(this->m_scanButton, &QPushButton::clicked, this, &MainWindow::startScan);
    connect(this->m_stopButton, &QPushButton::clicked, this, &MainWindow::stopScan);
    connect(this->m_selectButton, &QPushButton::clicked, this, &MainWindow::selectDirectory);
    connect(this->m_exportButton, &QPushButton::clicked, this, &MainWindow::exportResults);

    connect(m_scanner, &FileScanner::fileFound, this, &MainWindow::onFileFound);
    connect(m_scanner, &FileScanner::progress, this, &MainWindow::onScanProgress);
    connect(m_scanner, &FileScanner::finished, this, &MainWindow::onScanFinished);
    connect(m_scanner, &FileScanner::error, this, &MainWindow::onScanError);

    connect(m_fileTable, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);

    m_scanThread->start();
}

MainWindow::~MainWindow()
{
    if(m_scanThread){
        QMetaObject::invokeMethod(m_scanner, "stopScanning", Qt::QueuedConnection);
        m_scanThread->quit();
        m_scanThread->wait(1000);
    }
    delete ui;
}

void MainWindow::setupUI(){
    setWindowTitle(tr("Сканер файлів"));

    m_fileTable = findChild<QTableWidget*>("fileTable");
    m_progressBar = findChild<QProgressBar*>("progressBar");
    m_statusLabel = findChild<QLabel*>("statusLabel");

    // Вказівники на QLabel статистики
    m_barFilesCount = findChild<QLabel*>("barFilesCount");
    m_barTotalSize = findChild<QLabel*>("barTotalSize");
    m_totalSize = findChild<QLabel*>("totalSize");
    m_largestFile = findChild<QLabel*>("largestFile");
    m_mostCommonType = findChild<QLabel*>("mostCommonType");
    m_filesCount = findChild<QLabel*>("filesCount");

    // Вказівники на кноппки
    m_selectButton = findChild<QPushButton*>("selectButton");
    m_scanButton = findChild<QPushButton*>("scanButton");
    m_stopButton = findChild<QPushButton*>("stopButton");
    m_exportButton = findChild<QPushButton*>("exportButton");

    // Ініціалізація стовпців
    m_fileTable->setColumnCount(4);
    m_fileTable->setHorizontalHeaderLabels({
        tr("Назва"), tr("Розмір"), tr("Дата"), tr("Тип")
    });
    m_fileTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_fileTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_fileTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_fileTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    m_fileTable->setSortingEnabled(true);
    m_fileTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_fileTable->setContextMenuPolicy(Qt::CustomContextMenu);

    m_statusLabel->setText(tr("Статус: Готовий."));

    m_stopButton->setEnabled(false);
    m_exportButton->setEnabled(false);
}

void MainWindow::selectDirectory(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Оберіть папку"), QString(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()){
        m_selectedPath = dir;
        m_statusLabel->setText(tr("Статус: Обрано %1").arg(dir));
    }
}

void MainWindow::startScan(){
    if (m_selectedPath.isEmpty()){
        selectDirectory();
        if(m_selectedPath.isEmpty()) return;
    }

    m_timer.start();

    m_files.clear();
    m_fileTable->setRowCount(0);
    m_statusLabel->setText(tr("Статус: Сканування..."));
    m_exportButton->setEnabled(false);
    m_stopButton->setEnabled(true);
    m_scanButton->setEnabled(false);
    m_selectButton->setEnabled(false);

    QMetaObject::invokeMethod(m_scanner, "scanDirectory", Qt::QueuedConnection, Q_ARG(QString, m_selectedPath));
}

void MainWindow::stopScan(){
    m_scanner->stopScanning();
}

void MainWindow::exportResults(){
    if(m_files.empty()) return;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Експорт"), QDir::home().filePath("files.csv"), tr("CSV files (*.csv)"));

    if(fileName.isEmpty()) return;

    QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("Експорт"), tr("Неможливо записати до %1").arg(fileName));
    }

    QTextStream out(&f);
    out << "Назва,Розмір,Дата,Тип\n";
    for(const FileItem &it : m_files){
        out << it.name() << ','
            << it.size() << ','
            << it.modifiedString() << ','
            << it.type() << '\n';
    }
    f.close();
    m_statusLabel->setText(tr("Статус: Експортовано до %1").arg(fileName));
}

// Допоміжний класс для коректного сортування розмірів файлів
class cstmTableWidgetItem : public QTableWidgetItem {
public:
    cstmTableWidgetItem(const QString &text, qint64 value)
        : QTableWidgetItem(text), m_value(value) {}

    bool operator<(const QTableWidgetItem &other) const override{
        auto otherItem = dynamic_cast<const cstmTableWidgetItem*>(&other);
        if(otherItem){
            return m_value < otherItem->m_value;
        }
        return QTableWidgetItem::operator<(other);
    }
private:
    qint64 m_value;
};

void MainWindow::onFileFound(const FileItem &file){
    int row = m_fileTable->rowCount();
    m_fileTable->insertRow(row);

    m_fileTable->setItem(row, 0, new QTableWidgetItem(file.name()));
    m_fileTable->setItem(row, 1, new cstmTableWidgetItem(file.sizeString(), file.size()));
    m_fileTable->setItem(row, 2, new QTableWidgetItem(file.modifiedString()));
    m_fileTable->setItem(row, 3, new QTableWidgetItem(file.type()));

    m_files.append(file);
    updateStatistics();
}

void MainWindow::onScanProgress(int percent, const QString &currentPath){
    m_statusLabel->setText(tr("Статус: Сканується: %1").arg(currentPath));
    m_progressBar->setValue(percent);
}

void MainWindow::onScanFinished(const QList<FileItem> &allFiles){
    m_progressBar->maximum();
    m_statusLabel->setText(tr("Статус: Сканування завершено за %1 сек").arg(m_timer.elapsed() / 1000.0, 0 ,'f', 1));
    m_scanButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_exportButton->setEnabled(true);
    m_selectButton->setEnabled(true);
    updateStatistics();
}

void MainWindow::onScanError(const QString &error){
    QMessageBox::warning(this, tr("Помилка сканування!"), error);
}

void MainWindow::updateStatistics(){
    qint64 totalSize = 0;
    qint64 maxSize = 0;
    QString maxFile;
    QHash<QString, int> typeCount;

    // Підрахунок кілкості типів файлів та знаходження найбільшого файлу
    for(const FileItem &f : m_files) {
        totalSize += f.size();
        if(f.size() > maxSize) maxFile = f.name();
        typeCount[f.type()]++;
    }

    // Знаходження найпоширенішого типу
    QString commonType = "N/A";
    int maxCount = 0;
    for(auto it = typeCount.begin(); it != typeCount.end(); ++it){
        if(it.value() > maxCount){
            maxCount = it.value();
            commonType = it.key();
        }
    }
    m_mostCommonType->setText(commonType);

    FileItem tmp("", "", totalSize, QDateTime(), "");
    m_largestFile->setText(maxFile);

    m_totalSize->setText(tmp.sizeString());
    m_barTotalSize->setText(tr("Розмір: %1").arg(tmp.sizeString()));

    m_filesCount->setText(QString::number(m_files.size()));
    m_barFilesCount->setText(tr("Знайдено: %1 файл(а\\ів)").arg(QString::number(m_files.size())));
}

void MainWindow::showContextMenu(const QPoint &pos){
    QModelIndex index = m_fileTable->indexAt(pos);
    if(!index.isValid()) return;

    int row = index.row();
    FileItem file = m_files[row];

    QMenu menu(this);
    QAction *openFile = menu.addAction("Відкрити файл");
    QAction *openFolder = menu.addAction("Відкрити розташування файлу");

    QAction *selected = menu.exec(m_fileTable->viewport()->mapToGlobal(pos));
    if(!selected) return;

    if(selected == openFile){
        QDesktopServices::openUrl(QUrl::fromLocalFile(file.fullPath()));
    } else if(selected == openFolder){
        QDesktopServices::openUrl(QUrl::fromLocalFile(file.path()));
    }
}
