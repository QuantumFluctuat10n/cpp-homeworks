#include "file_item.h"

FileItem::FileItem(const QString &name, const QString &path,
                   qint64 size, const QDateTime &modified, const QString &type)
        : m_name(name), m_path(path), m_size(size), m_modified(modified), m_type(type) {}

QString FileItem::sizeString() const {
    const double KB = 1024;
    const double MB = KB * 1024;
    const double GB = MB * 1024;
    const double TB = GB * 1024;

    QLocale loc;
    if(m_size < KB) return loc.toString(m_size) + " B";
    if(m_size < MB) return loc.toString(m_size / KB, 'f', 1) + " KB";
    if(m_size < GB) return loc.toString(m_size / MB, 'f', 1) + " MB";
    if(m_size < TB) return loc.toString(m_size / GB, 'f', 1) + " GB";
    return loc.toString(m_size / TB, 'f', 1) + " TB";
}

QString FileItem::modifiedString() const{ return QLocale().toString(m_modified, QLocale::ShortFormat); }
