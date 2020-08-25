//
//  Node.cpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#include "../../include/Engine/Node.hpp"

namespace mz {

void Node::draw(Camera const& camera) {
    Layer::draw(camera);
}

void Node::run(Action && action) {
    auto& actionRef = m_actions.emplace_back(std::forward<Action>(action));
    actionRef.completeInit(this);
}

void Node::run(Action && action, std::string && name) {
    auto& actionRef = m_namedActions.emplace(std::make_pair(std::forward<std::string>(name), std::forward<Action>(action))).first->second;
    actionRef.completeInit(this);
}

void Node::run(Action && action, Action::CompletionCallback && callback) {
    auto& actionRef = m_actions.emplace_back(std::forward<Action>(action));
    actionRef.completeInit(this);
    actionRef.setCallback(std::forward<Action::CompletionCallback>(callback));
}

void Node::run(Action && action, std::string && name, Action::CompletionCallback && callback) {
    auto& actionRef = m_namedActions.emplace(std::make_pair(std::forward<std::string>(name), std::forward<Action>(action))).first->second;
    actionRef.completeInit(this);
    actionRef.setCallback(std::forward<Action::CompletionCallback>(callback));
}

void Node::update(std::uint64_t timeElapsed) {
    Layer::update(timeElapsed);
    
    for (auto& action : m_actions)
        action.update(timeElapsed);
    
    for (auto& [name, action] : m_namedActions)
        action.update(timeElapsed);
    
    for (std::size_t id = 0; id < m_actions.size(); ) {
        auto& action = m_actions[id];
        if (action.isCompleted()) {
            if (id != m_actions.size() - 1)
                std::swap(action, m_actions.back());
            m_actions.pop_back();
        } else {
            ++id;
        }
    }
    
    for (auto it = m_namedActions.begin(); it != m_namedActions.end(); ) {
        if (it->second.isCompleted())
            it = m_namedActions.erase(it);
        else
            ++it;
    }
}

}
