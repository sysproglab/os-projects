#ifndef GPIO_CLASS_H
#define GPIO_CLASS_H

#include <fstream>
#include <iostream>
#include <string.h>
#include <poll.h>                   // "poll"
#include <unistd.h>                 // "read/write/lseek"
#include <sys/stat.h>
#include <fcntl.h>                  // "O_RDONLY"
#include <linux/limits.h>           // "MAX_PATH"
#include <errno.h>                  //  codes of errors

#define DEFAULT_DIRECT "input"      // default value of direction
#define DEFAULT_EDGE   "both"       // default type of event (edge)

using namespace std;

/* GPIOController  */
class GPIOController
{
public:
    // create a GPIO object that controls GPIO with number == gpioNum
    GPIOController(int gpioNum, string direction=DEFAULT_DIRECT, string event=DEFAULT_EDGE);
    ~GPIOController();              // no comments

    int setDirection(string dir);   // set GPIO Direction (values: in/out)
    int setEvent(string edge);      // set type of event - type of edge (values: none/both/rising/falling)
    int setValue(int value);        // set GPIO Value (only for output GPIO)

    int getValueOnEvent(int timeOutInMSec);   // get GPIO Value on the Event, time in ms

    int getValue();                 // get GPIO Value
    int getGpioNum();               // return the GPIO number associated with the instance of an object
    string getGpioDirection();      // return direction of the GPIO
    string getGpioEvent();          // return event`s type of the GPIO

private:
    string gpioNum;     // GPIO number associated with the instance of an object
    string direction;   // GPIO direction
    string event;
};

#endif
