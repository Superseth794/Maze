//
//  Node.inl
//  Maze
//
//  Created by Jaraxus on 25/08/2020.
//

namespace mz {

inline std::size_t Node::getActionsCount() const {
    return m_actions.size() + m_namedActions.size();
}

inline float Node::getSpeed() const {
    return m_speed;
}

inline bool Node::isPaused() const {
    return m_isPaused;
}

inline std::size_t Node::removeAllActions() {
    auto actionsCount = getActionsCount();
    
    m_actions.clear();
    m_namedActions.clear();

    return actionsCount;
}

inline void Node::setPaused(bool isPaused) {
    m_isPaused = isPaused;
}

inline void Node::setSpeed(float speed) {
    m_speed = speed;
}

}
