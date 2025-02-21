/**
  ********************************************************************************
  * @file           : configure.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/31
  ********************************************************************************
  */

#ifndef AF_CONFIGURE_H
#define AF_CONFIGURE_H

#include <QReadWriteLock>
#include "src/af_nocopy.h"
#include "af_conf_var.h"

class AFConfigure : public AFNoCopy {
public:
    using ptr = QSharedPointer<AFConfigure>;
    using VarMap = QMap<QString, AFVar::ptr>;
    AFConfigure(const QString &file) : m_configureFile(file) {}
    ~AFConfigure() override = default;


    template<class T>
    typename AFConfVar<T>::ptr lookup(const QString &name, const T &defaultValue, const QString &desc = "")
    {
        QWriteLocker locker(&m_rwLock);
        if (m_dataMap.contains(name)) {
            auto tmp = m_dataMap.value(name).dynamicCast<AFConfVar<T>>();
            if (tmp) {
                return tmp;
            } else {
                return nullptr;
            }
        }
        auto stdName = name.toStdString();
        if(stdName.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678") != std::string::npos) {
            qWarning() << "Lookup name invalid " << name;
            throw std::invalid_argument(stdName);
        }
        auto v = QSharedPointer<AFConfVar<T>>::create(name, defaultValue, desc);
        m_dataMap.insert(name, v);
        return v;
    }
    
    template<class T>
    typename AFConfVar<T>::ptr lookup(const QString &name) {
        auto var = lookupBase(name);
        if (var) {
            return var.dynamicCast<AFConfVar<T>>();
        }
        return nullptr;
    }

    AFVar::ptr lookupBase(const QString &name);
    virtual int loadConfigure();
    virtual int updateFile();

private:
    QReadWriteLock m_rwLock;
    VarMap m_dataMap;
    QString m_configureFile;
};


#endif //AF_CONFIGURE_H
