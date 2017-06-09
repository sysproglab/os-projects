#ifndef DAEMON_STRUCT_H
#define DAEMON_STRUCT_H

#include <QMetaType>
#include <QString>
#include <QtDBus>
#include <zconf.h>
#include <sys/stat.h>
#include <syslog.h>

#define INFO 0
#define ERROR -1

struct Answer_struct{
    QString name;
    QMap<QString, QVariant> porp_map;
};
Q_DECLARE_METATYPE(Answer_struct)

static std::vector<Answer_struct> getStructAnswer(const QDBusArgument &dbusArgs) {
    QString selected_modem;
    Answer_struct answer_struct;
    std::vector<Answer_struct> answers;
    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs.beginStructure();
        if (dbusArgs.currentType() == 0)
            dbusArgs >> answer_struct.name;
        if (dbusArgs.currentType() == 4)
            dbusArgs >> answer_struct.porp_map;
        dbusArgs.endStructure();
        answers.push_back(answer_struct);
    }
    dbusArgs.endArray();

    return answers;
}

static void writeLog(const char* message, int status) {
    openlog("calls_daemon", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

    switch(status){
        case ERROR:
            syslog(LOG_ERR, message);
            break;
        case INFO:
            syslog(LOG_INFO, message);
            break;
        default:
            syslog(LOG_ALERT, message);
            break;
    }

    closelog();
}

#endif //DAEMON_STRUCT_H
