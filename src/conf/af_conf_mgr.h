/**
  ********************************************************************************
  * @file           : af_conf_mgr.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/1
  ********************************************************************************
  */

#ifndef AF_CONF_MGR_H
#define AF_CONF_MGR_H

#include "af_configure.h"

class AFConfMgr : public AFNoCopy {
public:
    static AFConfMgr *instance();

    int addConfigures(const QMap<QString, QString> &configures);
    int addConfigure(const QString &name, const QString &file);
    AFConfigure::ptr lookup(const QString &name);

private:
    AFConfMgr() = default;

    QMap<QString, AFConfigure::ptr> m_configures;
};


#endif //AF_CONF_MGR_H
