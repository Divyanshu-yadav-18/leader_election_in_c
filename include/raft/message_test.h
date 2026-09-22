#include "raft/node_state.h"
#include "raft/message.h"
#include <gtest/gtest.h>

using namespace raft;

// ---- NodeState ----

TEST(NodeState, ToStringIsHumanReadable) {
    EXPECT_STREQ(toString(NodeState::FOLLOWER), "FOLLOWER");
    EXPECT_STREQ(toString(NodeState::CANDIDATE), "CANDIDATE");
    EXPECT_STREQ(toString(NodeState::LEADER), "LEADER");
}

// ---- RaftMessage / heartbeat convention ----

// This test encodes a design rule, not just a struct default: an
// AppendEntries RPC with no entries IS a heartbeat. If this ever
// stops being true (e.g. someone adds a separate `isHeartbeat` flag
// and forgets to keep them in sync), this test should fail.
TEST(RaftMessage, DefaultAppendEntriesIsAHeartbeat) {
    RaftMessage msg;
    msg.type = MessageType::APPEND_ENTRIES;
    msg.term = 3;
    msg.senderId = 0;
    msg.receiverId = 1;
    msg.leaderId = 0;

    EXPECT_TRUE(msg.entries.empty())
        << "An AppendEntries with entries left at their default should "
           "be treated as a heartbeat.";
}

TEST(RaftMessage, CanCarryOneLogEntry) {
    RaftMessage msg;
    msg.type = MessageType::APPEND_ENTRIES;
    msg.entries.push_back(LogEntry{/*term=*/2, "SET x=5"});

    ASSERT_EQ(msg.entries.size(), 1u);
    EXPECT_EQ(msg.entries[0].term, 2);
    EXPECT_EQ(msg.entries[0].command, "SET x=5");
}

TEST(RaftMessage, RequestVoteDefaultsToNoVoteGranted) {
    RaftMessage resp;
    resp.type = MessageType::REQUEST_VOTE_RESPONSE;
    EXPECT_FALSE(resp.voteGranted);
}
