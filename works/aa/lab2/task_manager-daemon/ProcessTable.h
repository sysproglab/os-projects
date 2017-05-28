//
// Created by kivi on 01.12.16.
//

#ifndef TASK_MANAGER_PROCESSTABLE_H
#define TASK_MANAGER_PROCESSTABLE_H

#include <map>

#include "ProcessInfo.h"
#include "Logger.h"

class ProcessTable {

public:
    // ассоциативный массив, позволяющий быстро по PID получить структуру данных процесса
    using processMap = std::map<unsigned int, ProcessInfo>;

    ProcessTable();

    // указать системе логирования лог-файл
    void setUpLogger(const std::string& logfile);
    // обновить таблицу процессов
    void updateTable();

private:
    // построить новую таблицу процессов
    void initProcessTable();

private:

    processMap table_;	// таблица процессов
    logger::Logger log_;	// система логирования
};


#endif //TASK_MANAGER_PROCESSTABLE_H
