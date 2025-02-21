/**
  ********************************************************************************
  * @file           : af_logger.h
  * @author         : jaysheng
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/31
  ********************************************************************************
  */

#ifndef AF_LOGGER_H
#define AF_LOGGER_H

#include <QMap>
#include "src/af_nocopy.h"

class QFile;

class AFLogger : public AFNoCopy {
public:
    static AFLogger *instance();

    void initLogger(const QString &logPath);
    int addLogger(const QString &logName, const QString &logPath);
    int addLoggers(const QMap<QString, QString> &logs);

protected:
    AFLogger() = default;
    QFile *loggerFile(const QString &name);
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);


    bool m_init { false };
    QString m_logPath;
    QMap<QString, QFile*> m_logs;
};

#define AF_Debug(MODEL) QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, #MODEL).debug()
#define AF_Info(MODEL) QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, #MODEL).info()
#define AF_Warning(MODEL) QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, #MODEL).warning()
#define AF_Critical(MODEL) QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, #MODEL).critical()
#define AF_Fatal(MODEL) QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, #MODEL).fatal()

#endif //AF_LOGGER_H
