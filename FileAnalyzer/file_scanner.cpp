#include "file_scanner.h"

void FileScanner::scanDirectory(const QString &path) {
    m_shouldStop.storeRelease(false);
    m_foundFiles.clear();
    m_filesProcessed = 0;

    QDir root(path);
    if(!root.exists()){
        emit error(tr("Directory does not exist: %1").arg(path));
        emit finished(m_foundFiles);
    }

    int total = countFilesRecursive(root);
    try{
        scanRecursive(root, total);
        emit finished(m_foundFiles);
    } catch(...) {
        emit error(tr("Unexpected error while scanning."));
        emit finished(m_foundFiles);
    }
}

void FileScanner::stopScanning(){
    m_shouldStop.storeRelease(true);
}

void FileScanner::scanRecursive(const QDir &dir, qint64 totalFiles){
    if(m_shouldStop.loadAcquire()) return;

    QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries, QDir::DirsFirst | QDir::Name);

    for(const QFileInfo &info : entries){
        if(m_shouldStop.loadAcquire()) return;

        if(info.isDir()){
            scanRecursive(QDir(info.absoluteFilePath()), totalFiles);
        } else{
            FileItem item = createFileItem(info);
            m_foundFiles.append(item);
            m_filesProcessed++;
            emit fileFound(item);
            emit progress((100 * m_filesProcessed) / qMax(1, totalFiles), info.absoluteFilePath());
        }
    }
}

FileItem FileScanner::createFileItem(const QFileInfo &info){
    QString type = info.isDir() ? "Folder" : info.suffix();
    return FileItem(info.fileName(), info.absolutePath(), info.size(), info.lastModified(), type);
}

qint64 FileScanner::countFilesRecursive(const QDir &dir){
    int count = 0;
    QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries, QDir::DirsFirst | QDir::Name);

    for(const QFileInfo &info : entries){
        if(info.isDir()){
            count += countFilesRecursive(QDir(info.absoluteFilePath()));
        } else{
            count++;
        }
    }
    return count;
}
