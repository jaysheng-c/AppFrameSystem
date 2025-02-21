/**
  ********************************************************************************
  * @file           : test_logger.cpp
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/31
  ********************************************************************************
  */


#include <QApplication>
#include "src/log/af_logger.h"
#include "src/conf/af_conf_mgr.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


class Family {
public:
    Family() = default;
    QString serialize() const
    {
        QJsonObject obj;
        obj["role"] = role;
        obj["age"] = age;
        return QJsonDocument(obj).toJson(QJsonDocument::Compact);
    }
    void deserialize(const QString &data)
    {
        auto doc = QJsonDocument::fromJson(data.toUtf8());
        if (doc.isNull()) {
            return;
        }
        role = doc.object()["role"].toString();
        age = doc.object()["age"].toInt();
    }
    bool operator==(const Family &other) const
    {
        return role == other.role && age == other.age;
    }

    QString role;
    int age = 0;
};

template<>
class Transformer<QString, Family> {
public:
    Family operator()(const QString &data)
    {
        Family p;
        p.deserialize(data);
        return p;
    }
};

template<>
class Transformer<Family, QString> {
public:
    QString operator()(const Family &data)
    {
        return data.serialize();
    }
};


template<>
class Transformer<QString, QList<Family>> {
public:
    QList<Family> operator()(const QString &data)
    {
        auto doc = QJsonDocument::fromJson(data.toUtf8());
        if (doc.isNull() || !doc.isArray()) {
            return {};
        }
        QList<Family> list;
        auto array = doc.array();
        for (const auto &item : array) {
            list.append(Transformer<QString, Family>()(QJsonDocument(item.toObject()).toJson()));
        }
        return list;
    }
};

template<>
class Transformer<QList<Family>, QString> {
public:
    QString operator()(const QList<Family> &data)
    {
        QJsonArray array;
        for (const auto &d : data) {
            array.append(QJsonDocument::fromJson(Transformer<Family, QString>()(d).toUtf8()).object());
        }
        return QJsonDocument(array).toJson(QJsonDocument::Compact);
    }
};



int main(int argc, char *argv[])
{
//    AFLogger::instance()->initLogger("./");
//    AF_Debug(AFI) << QString("test");
//    qDebug() << "test";


    AFConfMgr::instance()->addConfigure("conf", "./conf.json");

    auto configure = AFConfMgr::instance()->lookup("conf");
    auto user = configure->lookup<QString>("user", "");
    auto password = configure->lookup<QString>("password", "");
    auto sex = configure->lookup<QString>("sex", "");
    auto address = configure->lookup<QString>("address", "");
    auto phone = configure->lookup<QString>("phone", "");
    auto age = configure->lookup<int>("age", 10);
    auto email = configure->lookup<QString>("email", "");
    auto family = configure->lookup<Family>("family", Family());
    auto families = configure->lookup<QList<Family>>("families", QList<Family>());
    qDebug() << configure->loadConfigure();
    qDebug() << user->value();
    qDebug() << password->value();
    qDebug() << sex->value();
    qDebug() << address->value();
    qDebug() << phone->value();
    qDebug() << age->value();
    qDebug() << email->value();
    qDebug() << family->value().serialize();
    qDebug() << "  ";
    for (const auto &f : families->value()) {
        qDebug() << f.serialize();
    }
    configure->updateFile();
    return 0;
}