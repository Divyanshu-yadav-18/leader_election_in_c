#pragma once
#include <string>
 
namespace raft {

struct LogEntry {
    int term = 0;
    std::string command;
};
 
}
