#pragma once
#include "raft/log_entry.h"
#include <vector>

namespace raft {

enum class MessageType {
  REQUEST_VOTE,
  REQUEST_VOTE_RESPONSE,
  APPEND_ENTRIES,
  APPEND_ENTRIES_RESPONSE
};

struct RaftMessage {
  MessageType type;

  int term = 0;

  int senderId = -1;
  int receiverId = -1;

  int lastLogIndex = 0;
  int lastLogTerm = 0;

  bool voteGranted = false;

  int leaderId = -1;
  int prevLogIndex = 0;
  int prevLogTerm = 0;
  std::vector<LogEntry> entries;
  int leaderCommit = 0;

  bool success = false;
};

} // namespace raft
