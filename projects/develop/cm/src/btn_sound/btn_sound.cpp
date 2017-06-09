/*
    sndInc  - to increase sound if pressed (lvl.1)
    sndRdc - to reduce sound, if pressed (lvl.1)
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

// QT
#include <QtCore/QStringList>
#include <QtDBus/QtDBus>

#include "GPIOController/GPIOController.h"
#include "SetProperty/SetProperty.h"

#define GPIO_SND_INC        4
#define GPIO_SND_RDC        17
#define ACTIV_LVL           0   // state for pressed button;
#define SENSITIVITY         10  // sensitivity of button`s press
                                // 1 clikc will change soundLvl on 10%
#define MAX_FREQ_EVENT_SND  250 // max frequency of event (in ms) for sound daemon


using namespace std;


int soundLvl = 100;         // in percents
bool SIGTEMT_event = false;
QString dBusPath; 

void SIGINT_handler(int sig) {
    if (sig == 2) {
        cout << "\nSIGINT signal!\n" << endl;
        SIGTEMT_event = true;
    }
    else
        cout << "\nUnsupported signal!\n" << endl;
}

// function for Increasing and Reducing of the sound lvl
void *thrdSndCtrlFunction(void *arg);
// function for getting of path to SIM800L
int getDBusPath(QString &path);


int main (void) {
    // set handler for signal SIGINT
    struct sigaction sig_struct;
    sig_struct.sa_handler = SIGINT_handler;
    sig_struct.sa_flags = 0;
    sigemptyset(&sig_struct.sa_mask);

    if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
        cout << "Problem with sigaction" << endl;
        cout << "Button Daemon Error#1: errno=" << errno << "; " << 
                "Could not set sigaction harder" << endl;
        exit(-1);
    }

    // thread env.
    int statusAddr;
    pthread_t thrdRdc;  // thread for reducing of Sound; Increasing occurs in main thread;
    GPIOController *sndInc = NULL;  // obj. sndInc serves for increasing of sound
    GPIOController *sndRdc = NULL;  // obj. sndRdc serves for reducing of sound

    
    sndInc = new GPIOController(    // obj. sndInc serves for increasing of sound
        GPIO_SND_INC,   // number of GPIO
        "in",           // mode of work: input
        "falling"       // type of event: when input value falls from 1 to 0 (press of button)
    );

    sndRdc = new GPIOController(    // obj. sndRdc serves for reducing of sound
        GPIO_SND_RDC,   // number of GPIO
        "in",           // mode of work: input
        "falling"       // type of event: when input value falls from 1 to 0 (press of button)
    );


    // get dbus path
    if (getDBusPath(dBusPath) != 0) {
        cout << "Sound Daemon Error#2: errno=" << errno << "; " << 
                "Invalid DBus path;" << endl;
        delete sndInc;  // destruction of gpio objects
        delete sndRdc;
        exit(-2);
    }

    // Reducing thread. Creation and start
    if (pthread_create(&thrdRdc, NULL, thrdSndCtrlFunction, (void*) sndRdc) != 0) {
        cout << "Sound Daemon Error#3: errno=" << errno << "; " << 
                "Could not create reducing threads;" << endl;
        delete sndInc;  // destruction of gpio objects
        delete sndRdc;
        exit(-3);  
    }

    // Increasing thread. Start in main
    thrdSndCtrlFunction(sndInc);    
    
    // wainting finish of all threads (increasing and reducing)
    pthread_join(thrdRdc, (void**)&statusAddr);

    delete sndInc;  // destruction of gpio objects
    delete sndRdc;

    cout << "Succesfully completed." << endl;
    return 0;
}

// function for Increasing and Reducing of the sound lvl
void *thrdSndCtrlFunction(void *arg) {
    
    int value;  // current value of GPIO
    GPIOController *gpio = (GPIOController *)arg;
    
    QDBusConnection bus = QDBusConnection::systemBus();
    if(!bus.isConnected()){
        qDebug() << "Invalid connectnion#12" << endl;
        SIGTEMT_event = true;
        return NULL;
    }
    QDBusInterface cv("org.ofono", dBusPath, "org.ofono.CallVolume", bus);
    OrgOfonoCallVolumeInterface setPro("org.ofono", dBusPath, bus);


    while (!SIGTEMT_event) {
        // waiting the event (failing)
        value = gpio->getValueOnEvent(3000); // timeOut 3 sec

        // checking the interruption
        if (SIGTEMT_event) break;

        // checking: is event or timeOut?
        if (value != ACTIV_LVL) continue;     // it`s timeOut

        // it`s event
        if (gpio->getGpioNum() == GPIO_SND_INC) {
            soundLvl + SENSITIVITY <= 100 ? soundLvl += SENSITIVITY : soundLvl = 100;
            cout << "+soundLvl: " << soundLvl << endl;
        }
        else {
            soundLvl - SENSITIVITY > 0 ? soundLvl -= SENSITIVITY : soundLvl = 0;
            cout << "-soundLvl: " << soundLvl << endl;
        }


        // send through DBus;
        auto reply = setPro.SetProperty("SpeakerVolume", QDBusVariant(qVariantFromValue(quint8(10))));
        reply.waitForFinished();

        // checking of errors
        if(reply.isError()) qDebug() << reply.error().name().toLatin1();

        // checking of success
        if(reply.isValid()) qDebug() << "Sound succesfully changed";

        // it`s regulate the max frequency of button`s event
        usleep(1000*MAX_FREQ_EVENT_SND);            // usleep works with microSec;
    }
    cout << "Thrd is finished." << endl;
    return NULL;
}


// function for getting of path to SIM800L
int getDBusPath(QString &path) {
    QDBusConnection bus = QDBusConnection::systemBus();
    if(!bus.isConnected()){
        qDebug() << "Invalid connectnion#1" << endl;//.value();
        return -1;
    }

    // default path
    QDBusInterface dbus_iface("org.ofono", "/", "org.ofono.Manager", bus);
    QDBusMessage modem = dbus_iface.call("GetModems");

    if(!modem.errorMessage().isNull() || !modem.errorMessage().isEmpty())
        return -2;

    QList<QVariant> outArgs = modem.arguments();
    const QDBusArgument &dbusArgs = outArgs.at(0).value<QDBusArgument>();

    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs.beginStructure();
        while (!dbusArgs.atEnd()) {
            dbusArgs >> path;
            break;
            sleep(1);
        }
        dbusArgs.endStructure();
        break;
    }
    dbusArgs.endArray();

    return 0;
}
