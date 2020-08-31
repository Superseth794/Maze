//
//  Node.inl
//  Maze
//
//  Created by Jaraxus on 25/08/2020.
//

namespace mz {

inline float Node::getSpeed() const {
    return m_speed;
}

inline bool Node::isPaused() const {
    return m_isPaused;
}

inline void Node::setPaused(bool isPaused) {
    m_isPaused = isPaused;
}

inline void Node::setSpeed(float speed) {
    m_speed = speed;
}

}
