//
// Created by kivi on 01.12.16.
//

#include "ProcessTable.h"
#include "Util.h"

#include <iostream>
#include <vector>

ProcessTable::ProcessTable() {
    initProcessTable();
}

void ProcessTable::setUpLogger(const std::string& logfile) {
    log_.setLogFile(logfile);
}

void ProcessTable::updateTable() {
    std::map<int, bool> visited;

    auto&& pids = util::getProcessList();
    for(auto&& pid : pids) {
        if (table_.find(pid) == table_.end()) {
            table_[pid] = ProcessInfo(pid);
            log_.write(table_[pid]);
        }
        visited[pid] = true;
    }

    // find exited processes
    std::vector<int> toDelete;
    for (auto&& it : table_) {
        if ((visited.find(it.first) == visited.end()) || !visited[it.first]) toDelete.push_back(it.first);
    }
    // delete old processes
    for (auto&& pid : toDelete) table_.erase(pid);
}

void ProcessTable::initProcessTable() {
    processMap newTable;

    auto&& pids = util::getProcessList();
    for (auto&& pid : pids)
        table_[pid] = ProcessInfo(pid);
}