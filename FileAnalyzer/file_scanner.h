#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include "file_item.h"

class FileScanner : public QObject {
    Q_OBJECT

public slots:
    void scanDirectory(const QString &path);
    void stopScanning();
    qint64 countFilesRecursive(const QDir &dir);

signals:
    void fileFound(const FileItem &file);
    void progress(int filesProcessed, const QString &currentPath);
    void finished(const QList<FileItem> &allFiles);
    void error(const QString &message);

private:
    void scanRecursive(const QDir &dir, qint64 totalFiles);
    FileItem createFileItem(const QFileInfo &info);

    QAtomicInt m_shouldStop;
    QList<FileItem> m_foundFiles;
    int m_filesProcessed;
};


#endif // FILE_SCANNER_H
