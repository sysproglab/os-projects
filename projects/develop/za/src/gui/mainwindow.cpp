#include "mainwindow.h"
#include <QQmlComponent>
#include <QQuickItem>

MainWindow::MainWindow(QObject *parent)
    : QQmlApplicationEngine(parent)
{
	load(QUrl("qrc:///qml/main.qml"));
	rootContext()->setContextProperty("window", this);
	if(!bus.isConnected())
		exit(1);

    GetModem();
}

MainWindow::~MainWindow(){}

std::vector<Answer_struct> getStructAnswer(const QDBusArgument &dbusArgs) {
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

void MainWindow::isAnswerValid(QDBusMessage msg)
{
    if(QDBusMessage::ErrorMessage == msg.type()){
        qDebug() << msg.errorMessage();
        exit(1);
    }
}

void MainWindow::GetModem(){
    //QDBusConnection bus = QDBusConnection::systemBus();

    if(!bus.isConnected())
        exit(1);

    QDBusInterface dbus_iface("org.ofono", "/", "org.ofono.Manager", bus);
    QDBusMessage modem = dbus_iface.call("GetModems");

    isAnswerValid(modem);

    const QDBusArgument &dbusArgs = modem.arguments().first().value<QDBusArgument>();
    std::vector<Answer_struct> answers = getStructAnswer(dbusArgs);

    if(answers.size() == 0)
        exit(1);

    if(answers.size() == 1)
        selected_modem = answers[0].name;
    else
        for(Answer_struct modem : answers)
            if(modem.name.contains("sim900"))
                selected_modem = modem.name;

    if(selected_modem.isNull() || selected_modem.isEmpty())
        exit(1);
}

void MainWindow::dialNumber(QString call_number){
	if(call_number.isEmpty() || call_number.isNull())
		return;

    load(QUrl("qrc:///qml/call.qml"));

    dialingWindow = this->rootObjects().at(1);
    QObject* object = dialingWindow->findChild<QObject*>("call_number");
    if(object)
        object->setProperty("text", call_number);

    QDBusInterface dbus_iface("org.ofono", selected_modem, "org.ofono.VoiceCallManager", bus);
    auto reply = dbus_iface.call("Dial", QVariant::fromValue(QString(call_number)), QVariant::fromValue(QString("")));
    isAnswerValid(reply);
    start = std::clock();
    getTime();
}

void MainWindow::getTime(){
    //double duration = (std::clock - start) / (double) CLOCKS_PER_SEC;
    QObject* object = dialingWindow->findChild<QObject*>("call_timer");
    if(object)
        object->setProperty("text", "time");//duration);
}

void MainWindow::hangUp(){
    QDBusInterface dbus_iface("org.ofono", selected_modem, "org.ofono.VoiceCallManager", bus);
    auto reply = dbus_iface.call("HangupAll");
    isAnswerValid(reply);

    exit(0);
    //this->rootObjects().removeAt(1);
}
