#include <fstream>
#include <QString>
#include <QtDBus>
#include <glib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>
#include "Struct.h"

#define INFO 0
#define ERROR -1

bool isAnswerValid(QDBusMessage);
int callsMonitor();
QVariant isModemEnabled = "false";
void writeLog(const char*, int);
int setupHandler();
DBusHandlerResult call_added_callback(DBusConnection*, DBusMessage*, void *);

int main() {

    writeLog("Start calls daemon", INFO);
    QDBusConnection bus = QDBusConnection::systemBus();

    if(!bus.isConnected())
        exit(1);

    QDBusInterface dbus_iface("org.ofono", "/", "org.ofono.Manager", bus);
    QDBusMessage modem = dbus_iface.call("GetModems");

    if(!isAnswerValid(modem))
        exit(1);

    const QDBusArgument &dbusArgs = modem.arguments().first().value<QDBusArgument>();
    std::vector<Answer_struct> answers = getStructAnswer(dbusArgs);
    QString selected_modem;

    if(answers.size() == 0){
        writeLog("Answer is NULL", ERROR);
        exit(1);
    }


    if(answers.size() == 1) {
        selected_modem = answers[0].name;
        isModemEnabled = answers[0].porp_map["Powered"];
        writeLog("Modem powered: " + isModemEnabled.toString().toLatin1(), INFO);
    }else
        for(Answer_struct modem : answers)
            if(modem.name.contains("sim900")){
                selected_modem = modem.name;
                isModemEnabled = modem.porp_map["Powered"].toBool();
                writeLog("Modem powered: " + isModemEnabled.toString().toLatin1(), INFO);
            }

    if(selected_modem.isNull() || selected_modem.isEmpty()) {
        writeLog("No modem was selected", ERROR);
        exit(1);
    }


    writeLog("Selected modem: " + selected_modem.toLatin1(), INFO);

    if(isModemEnabled == "false") {
        QDBusInterface modem_iface("org.ofono", "/", "org.ofono.Modem", bus);
        QList<QVariant> argumentList;
        auto reply = modem_iface.call(QString("SetProperty"), QVariant::fromValue(QString("Powered")), 
															QVariant::fromValue(QDBusVariant(true)));

        if(!isAnswerValid(reply)){
            writeLog(reply.errorMessage().toLatin1(), ERROR);
            exit(1);
        }
        writeLog("Modem succesffuly enabled", INFO);

    }

    QDBusInterface network_iface("org.ofono", selected_modem, "org.ofono.NetworkRegistration", bus);
    QList<QVariant> argumentList;
    QDBusPendingReply<> operators= network_iface
            .asyncCallWithArgumentList(QStringLiteral("GetOperators"), argumentList);
    auto reply = operators.argumentAt(0).value<QDBusArgument>();
    answers = getStructAnswer(reply);
    QString networkOperator;
    for(Answer_struct answer : answers)
        networkOperator = answer.porp_map["Name"].toString();

    std::ofstream operName;
    operName.open("~//operator.txt");
    operName << networkOperator.toStdString();
    operName.close();

    qDebug() << "Operator: " << networkOperator;

    int pid = fork();
    if(pid == -1) {
        writeLog("Daemon launching failed.\n", ERROR);
        return -1;
    }
    else if(!pid){
        writeLog("Daemon lauched", INFO);
        umask(0);
        setsid();
        chdir("/");

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        return callsMonitor();

    } else
        return 0;
}

bool isAnswerValid(QDBusMessage msg) {
    if(QDBusMessage::ErrorMessage == msg.type()){
        writeLog(msg.errorMessage().toLatin1(), ERROR);
        return false;
    }
    return true;
}

int callsMonitor() {
    QDBusInterface calls_inface("org.ofono", "/", "org.ofono.Manager", QDBusConnection::systemBus());
    QDBusMessage modem = calls_inface.call("GetCalls");
    setupHandler();
    writeLog("Daemon ends", ERROR);
}

DBusHandlerResult call_added_callback(DBusConnection *con, DBusMessage *msg, void *user_data){
    if(dbus_message_is_signal(msg, "org.ofono.VoiceCallManager", "CallAdded"))
        writeLog("CallAdded callback", INFO);


    if(dbus_message_is_signal(msg, "org.ofono.VoiceCallManager", "CallRemoved"))
        writeLog("CallRemoved callback", INFO);

    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int setupHandler() {
    writeLog("Handler settings", INFO);
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    DBusError error;
    writeLog("DBusError error", INFO);
    dbus_error_init(&error);
    DBusConnection *conn = dbus_bus_get(DBUS_BUS_SYSTEM, &error);

    if(dbus_error_is_set(&error)){
        writeLog(strcat("Cannot get System BUS connection: ", error.message), ERROR);
        dbus_error_free(&error);
        return EXIT_FAILURE;
    }
    writeLog("Succesfull System BUS connection", INFO);
    dbus_connection_setup_with_g_main(conn, NULL);

    char *rule = "type='signal', interface='org.ofono.VoiceCallManager'";
    dbus_bus_add_match(conn, rule, &error);

    if(dbus_error_is_set(&error)){
        writeLog(strcat("Cannot add D-BUS match rule, cause: ", error.message), ERROR);
        dbus_error_free(&error);
        return EXIT_FAILURE;
    }

    Answer_struct callAddedStruct;
    writeLog("Listenning to D-BUS signals using a connection filter", INFO);
    dbus_connection_add_filter(conn, call_added_callback, &callAddedStruct, NULL);

    g_main_loop_run(loop);

    return EXIT_SUCCESS;
}
