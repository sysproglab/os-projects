#include "GPIOController.h"

using namespace std;

// constructor creates and registers new GPIO with number "gpioNum" into the system
GPIOController::GPIOController(int gpioNum, string direction, string event) {
    // Instatiate GPIOController object for GPIO with current gpioNum number
    this->gpioNum = to_string(gpioNum);

    // open export file for adding new GPIO
    ofstream gpioExport("/sys/class/gpio/export");
    if (!gpioExport.is_open()){
        cout << "GPIOController Error#1: errno=" << errno << "; " << 
            "Unable to export GPIO"
             << this->gpioNum << ";" << endl;
        this->gpioNum = to_string(-1);
        return;
    }

    // add new GPIO to configuration and close file
    gpioExport << this->gpioNum ;
    gpioExport.close();

    // set dicrection for gpioNum
    setDirection(direction);

    // set type of event
    setEvent(event);
}

// destructor unregisters and removes GPIO with number "this->gpioNum" from system
GPIOController::~GPIOController() {
    // open unExport file for removing GPIO
    ofstream gpioUnExport("/sys/class/gpio/unexport");
    if (!gpioUnExport.is_open()){
        cout << "GPIOController Error#3: errno=" << errno << "; " << 
            "Unable to unexport GPIO"
             << this->gpioNum << ";" << endl;
        return;
    }

    // remove gpioNum from GPIO configuration
    gpioUnExport << this->gpioNum ;
    gpioUnExport.close();
    this->gpioNum = -1;
}

// function setDirection sets direction of GPIO work: "in" or "out" mode
int GPIOController::setDirection(string dir) {
    // open direction file for GPIO
    string gpioDirPath ="/sys/class/gpio/gpio" + this->gpioNum + "/direction";
    ofstream gpioDirFile(gpioDirPath.c_str());
    if (!gpioDirFile.is_open()){
        cout << "GPIOController Error#2: errno=" << errno << "; " << 
            "Unable to set direction for GPIO"
             << this->gpioNum << ";" << endl;
        return -1;
    }

    // write the direction to special file of direction
    gpioDirFile << dir;
    gpioDirFile.close();
    this->direction = dir;
    return 0;
}

// function setEvent sets type of event  (none, both, rising or falling)
int GPIOController::setEvent(string event) {
    // open file, which contain event of GPIO (0 or 1)
    string gpioEventPath = "/sys/class/gpio/gpio" + this->gpioNum + "/edge";
    ofstream gpioEventFile(gpioEventPath.c_str());
    if (!gpioEventFile.is_open()) {
        cout << "GPIOController Error#4: errno=" << errno << "; " << 
            "Unable to set event (edge) for GPIO"
             << this->gpioNum << ";" << endl;
        return -2;
    }

    // set type of event into file of event
    gpioEventFile << event;
    gpioEventFile.close();

    // also remember as property of GPIO
    this->event = event;
    return 0;
}

// function setValue sets value of GPIO
int GPIOController::setValue(int value) {
    // open file, which should contain value of GPIO (0 or 1)
    string gpioValuePath = "/sys/class/gpio/gpio" + this->gpioNum + "/value";
    ofstream gpioValueFile(gpioValuePath.c_str());
    if (!gpioValueFile.is_open()) {
        cout << "GPIOController Error#5: errno=" << errno << "; " << 
            "Unable to set value of GPIO"
             << this->gpioNum << ";" << endl;
        return -2;
    }

    // set value into gpioValueFile
    gpioValueFile << value;
    gpioValueFile.close();
    return 0;
}

// function getValue gets value of GPIO
int GPIOController::getValue() {
    // open file, which contain value of GPIO (0 or 1)
    string gpioValuePath = "/sys/class/gpio/gpio" + this->gpioNum + "/value";
    ifstream gpioValueFile(gpioValuePath.c_str());
    if (!gpioValueFile.is_open()){
        cout << "GPIOController Error#6: errno=" << errno << "; " << 
            "Unable to get the value of GPIO"
             << this->gpioNum << ";" << endl;
        return -2;
    }
    // read GPIO
    string tmp;
    gpioValueFile >> tmp ;
    gpioValueFile.close();

    // if 0 return 0 else return 1;
    return strcmp(tmp.c_str(), "0") == 0 ? 0 : 1;
}

// function getValue gets value of GPIO on the event (look function setEvent)
int GPIOController::getValueOnEvent(int timeOutInMSec) {
    // check (and may be set) type of event
    if (strcmp(this->event.c_str(), "both") != 0
     && strcmp(this->event.c_str(), "rising") != 0
     && strcmp(this->event.c_str(), "falling") != 0) {
        // try to reset default type of event
        if (setEvent(DEFAULT_EDGE) < 0)
            return -2;
        cout << "GPIOController Warning. Type of event is not defined." << endl
             << "Default value (" << DEFAULT_EDGE << ") is set." << endl;
    }

    char gpioValueFile[PATH_MAX];
    int fd;
    char c;
    int err;
    struct pollfd pollfd[1];

    // polling the line
    snprintf(gpioValueFile, sizeof(gpioValueFile), "/sys/class/gpio/gpio%s/value", this->gpioNum.c_str());
    fd = open(gpioValueFile, O_RDONLY);
    if (fd < 0) {
        cout << "GPIOController Error#7: errno=" << errno << "; " << 
            "Unable to open the file with value of GPIO"
             << this->gpioNum << ";" << endl;
        return -3;
    }
    read(fd, &c, sizeof(c));

    pollfd[0].fd = fd;
    pollfd[0].events = POLLPRI | POLLERR;
    pollfd[0].revents = 0;

    // waiting of event
    errno = 0;  // clear
    err =  poll(pollfd, 1, timeOutInMSec);
    if(err != 1 && errno !=0 && errno !=4) { // 4 - Interrupted system call
        cout << "GPIOController Error#8: errno=" << errno << "; " << 
            "Unable to poll the value of GPIO"
             << this->gpioNum << ";" << endl;
        return -4;
    }

    // backing to start of value`s file
    lseek(fd, 0, SEEK_SET);
    read(fd, &c, sizeof(c));

    return c - '0';
}

// get number of current GPIO
int GPIOController::getGpioNum() {
    return stoi(this->gpioNum);
}

// get direction of current GPIO
string GPIOController::getGpioDirection() {
    return this->direction;
}

// get type of event of current GPIO
string GPIOController::getGpioEvent() {
    return this->event;
}