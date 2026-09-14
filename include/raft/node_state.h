#pragma once

namespace raft {
  enum class NodeState{
    FOLLOWER,
    CANDIDATE,
    LEADER
  }

  inline const char* toString(NodeState s){
    switch(s){
    case  NodeState::FOLLOWER: return "FOLLOWER";
    case  NodeState::CANDIDATE: return "CANDIDATE";
    case  NodeState::LEADER: return "LEADER";
     
    }
    return "UNKNOWN"
  }
}
