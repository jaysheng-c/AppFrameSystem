/**
  ********************************************************************************
  * @file           : af_conf_var.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/1
  ********************************************************************************
  */

#ifndef AF_CONF_VAR_H
#define AF_CONF_VAR_H

#include <utility>

#include "src/log/af_logger.h"
#include "af_transformer.h"


class AFVar {
public:
    using ptr = QSharedPointer<AFVar>;

    explicit AFVar(const QString &name, QString desc = "")
            : m_name(name.toLower()), m_description(std::move(desc)) {}
    virtual ~AFVar() = default;

    virtual const QString &name() const { return m_name; }
    virtual const QString &description() const { return m_description; }
    virtual const QString toString() const = 0;
    virtual bool fromString(const QString &val) = 0;
    virtual QString typeName() const = 0;
    int type() const { return m_type; }
    void setType(int type) { m_type = type; }

protected:
    QString m_name;
    QString m_description;
    int m_type = 0;
};


template <class T, class FromStr = Transformer<QString, T>, class ToStr = Transformer<T, QString>>
class AFConfVar : public AFVar {
public:
    using ptr = QSharedPointer<AFConfVar>;
    explicit AFConfVar(const QString &name, const T &default_val, const QString &desc = "")
            : AFVar(name, desc), m_value(default_val) {}

    const QString toString() const override
    {
        try {
            QReadLocker locker(&m_rwLock);
            return ToStr()(m_value);
        } catch (const std::exception &e) {
            AF_Warning(CONFIGURE) << "exception [" + QString(e.what()) + "] convert to string name=" + m_name;
        }
        return {};
    }

    bool fromString(const QString &str) override
    {
        try {
            setValue(FromStr()(str));
        } catch (const std::exception &e) {
            AF_Warning(CONFIGURE) << "exception [" + QString(e.what()) + "] convert from string name=" + m_name
                                     + " - " + str;
            return false;
        }
        return true;
    }

    const T &value() const
    {
        QReadLocker locker(&m_rwLock);
        return m_value;
    }

    void setValue(const T &value)
    {
        {
            QReadLocker locker(&m_rwLock);
            if (value == m_value) {
                return;
            }
        }
        QWriteLocker locker(&m_rwLock);
        m_value = value;
    }

    QString typeName() const override { return QString(typeid(T).name()); }

protected:
    mutable QReadWriteLock m_rwLock;
    T m_value;
};


#endif //AF_CONF_VAR_H
