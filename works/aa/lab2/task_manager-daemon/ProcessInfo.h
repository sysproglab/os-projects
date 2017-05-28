//
// Created by kivi on 01.12.16.
//

#include <string>
#include <iostream>
#include <fstream>

#ifndef TASK_MANAGER_PROCESSINFO_H
#define TASK_MANAGER_PROCESSINFO_H


class ProcessInfo {
public:

    ProcessInfo() = default;
    ProcessInfo(int pid);

    int getId() const;
    int getParent() const;
    int getGroupId() const;
    int getPriority() const;
    const std::string& getState() const;
    const std::string& getExecutable() const;
    const std::string& getUser() const;
    const std::string& getCreationTime() const;

    friend std::ostream& operator<<(std::ostream& out, const ProcessInfo& info);

private:

    int pid_;				// идентификатор процесса
    int parent_;			// идентификатор родительского процесса
    int groupid_;           // идентификатор группы процессов
    int nice_;              // приоритет
    std::string state_;     // состояние процесса
    std::string executable_;		// исполняемый файл
    std::string user_;				// пользователь, запустивший процесс
    std::string creationTime_;		// время запуска процесса

};


#endif //TASK_MANAGER_PROCESSINFO_H
