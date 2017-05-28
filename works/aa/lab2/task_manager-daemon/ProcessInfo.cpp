//
// Created by kivi on 01.12.16.
//

#include <fstream>
#include <time.h>
#include "ProcessInfo.h"
#include "Util.h"

ProcessInfo::ProcessInfo(int pid) {
    pid_ = pid;

    std::ifstream myFile("/proc/" + std::to_string(pid_) + "/stat");
    std::string str;

    int indx = 0;
    while (myFile >> str) {
        ++indx;
        switch (indx) {
            case 2: executable_ = str; break;
            case 3: state_ = str; break;
            case 4: parent_ = std::stoi(str); break;
            case 5: groupid_ = std::stoi(str); break;
            case 19: nice_ = std::stoi(str); break;
            case 22: creationTime_ = util::getTimeFromClock(str); break;
        }
    }
    user_ = util::getOwnerByPID(pid);
}

int ProcessInfo::getId() const {
    return pid_;
}
int ProcessInfo::getParent() const {
    return parent_;
}
int ProcessInfo::getGroupId() const {
    return groupid_;
}
int ProcessInfo::getPriority() const {
    return nice_;
}
const std::string& ProcessInfo::getState() const {
    return state_;
}
const std::string& ProcessInfo::getExecutable() const {
    return executable_;
}
const std::string& ProcessInfo::getUser() const {
    return user_;
}
const std::string& ProcessInfo::getCreationTime() const {
    return creationTime_;
}

std::ostream& operator<<(std::ostream& out, const ProcessInfo& info) {
    out << "[" << info.getCreationTime() << "]" << "\t"
        << info.getId() << "\t"
        << info.getExecutable() << "\t"
        << info.getUser() << "\t"
        << info.getParent() << "\t"
        << info.getGroupId() << "\t"
        << info.getPriority() << "\t"
        << info.getState();
    return out;
}