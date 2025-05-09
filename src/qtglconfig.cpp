#include "qtglconfig.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>

Qtglconfig::Qtglconfig(const QString &filepath) {
    this->filepath = filepath;
}

bool Qtglconfig::load(bool ignore_inexistent) {
    if (this->filepath.isEmpty()) {
        if (ignore_inexistent) {
            return true;
        }
        return false;
    }

    QFile file(this->filepath);

    if (!file.exists()) {
        if (ignore_inexistent) {
            return true;
        }
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QTextStream text(&file);
    text.setCodec("utf8");
    QJsonParseError error;
    QJsonDocument jsonDocument;
    if (this->decryptor) {
        jsonDocument = QJsonDocument::fromJson(this->decryptor(text.readAll().toUtf8()), &error);
    } else {
        jsonDocument = QJsonDocument::fromJson(text.readAll().toUtf8(), &error);
    }
    if (QJsonParseError::NoError != error.error || jsonDocument.isNull()) {
        qDebug() << "QGC Error: " << error.offset << error.errorString();
        return false;
    } else {
        this->json = jsonDocument.object();
    }
    file.close();
    return true;
}

bool Qtglconfig::save(QJsonDocument::JsonFormat format) {
    if (this->filepath.isEmpty()) {
        return false;
    }

    QFileInfo fi(this->filepath);
    if (fi.isDir()) {
        return false;
    }

    QFile temp_file(fi.absoluteDir().absoluteFilePath(QString(".qgc_new.%1").arg(fi.fileName())));
    QFile temp_file2(fi.absoluteDir().absoluteFilePath(QString(".qgc_bak.%1").arg(fi.fileName())));

    if (temp_file.exists()) {
        if (!temp_file.remove()) {
            return false;
        }
    }
    if (temp_file2.exists()) {
        if (!temp_file2.remove()) {
            return false;
        }
    }

    if (!temp_file.open(QFile::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    QTextStream text(&temp_file);
    text.setCodec("utf8");
    QJsonDocument doc(this->json);
    QString data;
    if (this->encryptor) {
        data = this->encryptor(doc.toJson(format));
    } else {
        data = doc.toJson(format);
    }
    text << data << Qt::flush;
    temp_file.close();

    if (fi.isFile()) {
        QFile::rename(fi.absoluteFilePath(), temp_file2.fileName());
    }
    fi.refresh();
    if (fi.isFile()) {
        return false;
    }

    if (!QFile::rename(temp_file.fileName(), fi.absoluteFilePath())) {
        QFile::rename(temp_file2.fileName(), fi.absoluteFilePath());
        return false;
    }

    QFile::remove(temp_file2.fileName());

    return true;
}

QJsonValue Qtglconfig::_value(const QJsonObject &json, QList<QString> paths) {
    QJsonValue VALUE = json.value(paths.first());
    paths.takeFirst();
    if (paths.count() > 0) {
        return this->_value(VALUE.toObject(), paths);
    } else {
        return VALUE;
    }
}

void Qtglconfig::_clear_array(QJsonArray &array) {
    for (int i = array.count(); i > 0; i--) {
        array.takeAt(i - 1);
    }
}
