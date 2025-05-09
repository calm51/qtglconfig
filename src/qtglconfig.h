#ifndef QTGLCONFIG_H
#define QTGLCONFIG_H

#include "qtglconfig_global.h"

#include <QJsonDocument>
#include <QJsonObject>

class QTGLCONFIG_EXPORT Qtglconfig {
  public:
    Qtglconfig(const QString &filepath);

    QString filepath;
    QJsonObject json;

    std::function<QByteArray(QByteArray)> encryptor;
    std::function<QByteArray(QByteArray)> decryptor;

    bool load(bool ignore_inexistent = true);
    bool save(QJsonDocument::JsonFormat format);

    template<typename T>
    inline void insert(const QString &key, const T &value) {
        this->json.insert(key, value);
    }
    template<typename T>
    inline void insert(QList<QString> paths, const QString &key, const T &value) {
        return _insert(this->json, paths, key, value);
    }
    template<typename T>
    void _insert(QJsonObject &json, QList<QString> paths, const QString &key, const T &value) {
        QJsonValueRef KEY = json[paths.at(0)];
        QJsonObject VALUE = KEY.toObject();
        paths.takeFirst();
        if (paths.count() > 0) {
            this->_insert(VALUE, paths, key, value);
        } else {
            VALUE.insert(key, value);
        }
        KEY = VALUE;
    }

    inline QJsonValue value(const QString &key) { return _value(this->json, QList<QString>{key}); }
    inline QJsonValue value(QList<QString> paths) { return _value(this->json, paths); }
    QJsonValue _value(const QJsonObject &json, QList<QString> paths);

    void _clear_array(QJsonArray &array);
};

#endif // QTGLCONFIG_H
