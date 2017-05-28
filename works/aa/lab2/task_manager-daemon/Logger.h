//
// Created by kivi on 01.12.16.
//

#ifndef TASK_MANAGER_LOGGER_H
#define TASK_MANAGER_LOGGER_H

#include <string>
#include <iostream>
#include <fstream>

#include "ProcessInfo.h"

namespace logger {

// система логирования
class Logger {
public:

    Logger() = default;
    Logger(const std::string& filename) : log_(filename) {
        printTitle();
    }

    ~Logger() {
        log_.close();
    }

    void printTitle() {
        log_ << "CREATION TIME \t PID \t EXE \t USER NAME \t PARENT \t TGID \t NICE \t STATE" << std::endl;
    }

    void setLogFile(const std::string& logfile) {
        log_.close();
        log_.open(logfile);
        printTitle();
    }

    void write(const ProcessInfo& obj) {
        log_ << obj << std::endl;
    }

    void write(const std::string& obj) {
        log_ << obj << std::endl;
    }


private:

    std::ofstream log_;		// поток вывода в лог-файл

};

}

#endif //TASK_MANAGER_LOGGER_H
