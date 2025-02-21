/**
  ********************************************************************************
  * @file           : af_conf_mgr.cpp
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/1
  ********************************************************************************
  */

#include "af_conf_mgr.h"

namespace {
    QReadWriteLock gRWLock;
}


AFConfMgr *AFConfMgr::instance()
{
    static AFConfMgr instance;
    return &instance;
}

int AFConfMgr::addConfigures(const QMap<QString, QString> &configures)
{
    if (configures.isEmpty()) {
        return 0;
    }
    QWriteLocker locker(&gRWLock);
    int count = 0;
    for (auto iter = configures.begin(); iter != configures.end(); ++iter) {
        auto name = iter.key().toLower();
        auto file = iter.value();
        if (name.isEmpty() || file.isEmpty()) {
            continue;
        }
        if (m_configures.contains(name)) {
            continue;
        }
        m_configures.insert(name, QSharedPointer<AFConfigure>::create(file));
        ++count;
    }
    return count;
}

int AFConfMgr::addConfigure(const QString &name, const QString &file)
{
    return addConfigures({ { name, file } });

}

AFConfigure::ptr AFConfMgr::lookup(const QString &name)
{
    QReadLocker locker(&gRWLock);
    return m_configures.value(name.toLower(), nullptr);
}
