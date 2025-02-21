/**
  ********************************************************************************
  * @file           : af_logger.cpp
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/31
  ********************************************************************************
  */

#include "af_logger.h"
#include <iostream>
#include <QDebug>
#include <QMutex>
#include <QFile>
#include <QDateTime>

namespace {
    QMutex s_mutex;
}

AFLogger *AFLogger::instance()
{
    static AFLogger logger;
    return &logger;
}

void AFLogger::initLogger(const QString &logPath)
{
    QMutexLocker locker(&s_mutex);
    if (!m_init) {
        m_init = true;
        m_logPath = logPath;
        qInstallMessageHandler(AFLogger::messageHandler);
    }
}

int AFLogger::addLogger(const QString &logName, const QString &logPath)
{
    return addLoggers({{logName, logPath}});
}

int AFLogger::addLoggers(const QMap<QString, QString> &logs)
{
    int success = 0;
    for (auto it = logs.begin(); it != logs.end(); ++it) {
        if (!m_logs.contains(it.key())) {
            auto file = new QFile(it.value());
            if (file->open(QIODevice::WriteOnly | QIODevice::Append)) {
                m_logs.insert(it.key(), file);
                ++success;
            } else {
                delete file;
            }
        }
    }
    return success;
}

QFile *AFLogger::loggerFile(const QString &name)
{
    if (!m_logs.contains(name)) {
        throw std::runtime_error(QString("logger [%1] not found").arg(name).toStdString());
    }
    return m_logs.value(name);
}

void AFLogger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString prefix;
    switch (type) {
        case QtDebugMsg: prefix = "[Debug] "; break;
        case QtInfoMsg: prefix = "[Info] "; break;
        case QtWarningMsg: prefix = "[Warning] "; break;
        case QtCriticalMsg: prefix = "[Critical] "; break;
        case QtFatalMsg: prefix = "[Fatal] "; break;
    }

    {
        QMutexLocker locker(&s_mutex);
        if (AFLogger::instance()->m_init) {
            try {
                auto file = AFLogger::instance()->loggerFile(context.category);
                QTextStream out(file);
#ifdef QT_DEBUG
                out << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << "] "
                    << context.file << ":" << context.line << " " << prefix
                    << context.function << ": " << msg << "\n";
#else
                out << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << "] "
                        << prefix << context.function << ": " << msg << "\n";
#endif
            } catch (std::exception &e) {
#ifdef QT_DEBUG
                std::cerr << e.what() << " " << context.category << std::endl;
#endif
            }
        }
    }
#ifdef QT_DEBUG
    switch (type) {
        case QtDebugMsg:
        case QtInfoMsg:
            std::cout << context.file << ":" << context.line << " " << qPrintable(prefix)
                      << context.function << ": " << qPrintable(msg) << std::endl;
            break;
        default:
            std::cerr << context.file << ":" << context.line << " " << qPrintable(prefix)
                      << context.function << ": " << qPrintable(msg) << std::endl;
            break;
    }
#endif
    if (type == QtFatalMsg) {
        abort();
    }
}
