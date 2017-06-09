#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QUrl>
#include <QtQml/QQmlApplicationEngine>
#include <QQmlContext>
#include <QApplication>
#include <QString>
#include <QtDBus>
#include <iostream>
#include <ctime>

struct Answer_struct{
    QString name;
    QMap<QString, QVariant> porp_map;
};
Q_DECLARE_METATYPE(Answer_struct)

class MainWindow : public QQmlApplicationEngine{
    Q_OBJECT
public:
    MainWindow(QObject *parent = 0);
    ~MainWindow();
    void GetModem();
    void isAnswerValid(QDBusMessage msg);
    Q_INVOKABLE void dialNumber(QString number);
    Q_INVOKABLE void hangUp();
    Q_INVOKABLE void getTime();

private:
    QDBusConnection bus = QDBusConnection::systemBus();
    QString selected_modem;
	QString dialedNumber;
    QObject*  dialingWindow;
    std::clock_t start;
};
#endif // MAINWINDOW_H
