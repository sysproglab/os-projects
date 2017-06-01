//
// Created by kivi on 01.12.16.
//

#ifndef TASK_MANAGER_UTIL_H
#define TASK_MANAGER_UTIL_H

#include <vector>
#include <cstdio>
#include <linux/limits.h>
#include <cstdlib>
#include <sstream>
#include <pwd.h>
#include <dirent.h>

namespace util {

static bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

static std::vector<int> getProcessList() {
    std::vector<int> pids;
    struct dirent* dent;
    DIR* srcdir = opendir("/proc/");

    while ((dent = readdir(srcdir)) != NULL) {
        std::string dir(dent->d_name);
        if (is_number(dir)) {
            pids.push_back(std::stoi(dir));
        }
    }

    return std::move(pids);
}

static time_t getSystemStartTime() {
    static time_t starttime;
    if (starttime == 0) {
        std::ifstream uptime("/proc/uptime");
        int up;
        uptime >> up;
        time_t now = time(0);
        starttime = now - up;
    }
    return starttime;
}

static std::string getTimeFromClock(const std::string& inp) {
    std::string res;
    try {
        int procUptime = std::stoi(inp);
        time_t procStartTime = util::getSystemStartTime() + (procUptime / CLOCKS_PER_SEC);
        struct tm *tm = localtime(&procStartTime);
        if (not tm) {
            res = "NA";
            return std::move(res);
        }

        char timeView[19];
        sprintf(timeView, "%04d-%02d-%02d %02d:%02d:%02d\n",
                tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
                tm->tm_hour, tm->tm_min, tm->tm_sec);
        res.assign(timeView, 19);
    } catch (std::invalid_argument&) {
        res = "NA";
    }
    return std::move(res);
}

static std::string getOwnerByPID(int pid) {
    std::ifstream status("/proc/" + std::to_string(pid) + "/status");
    std::string line;
    int linenum = 0;
    uid_t uid;
    while (std::getline(status, line)) {
        ++linenum;
        if (linenum == 9) {
            std::stringstream stream;
            stream << line;
            stream >> line >> uid;
            break;
        }
    }

    register struct passwd *pw;
    pw = getpwuid (uid);
    if (pw) {
        return std::move(std::string(pw->pw_name));
    }
    return "NA";
}

}

#endif //TASK_MANAGER_UTIL_H
