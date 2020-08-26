//
//  Action.inl
//  Maze
//
//  Created by Jaraxus on 19/08/2020.
//

namespace mz {

inline float Action::getCurrentProgress() {
    return getProgress(m_timeElapsed, m_duration);
}

inline bool Action::isCompleted() const {
    return (m_timeElapsed == m_duration);
}

inline void Action::setDuration(std::uint64_t duration) {
    m_duration = std::max(duration, static_cast<std::uint64_t>(1));
}

inline void Action::setRelativeToParent(bool isRelativeToParent) {
    m_isRelativeToParent = isRelativeToParent;
}

inline void Action::setSpeed(float speed) {
    assert(speed > 0.f);
    m_speed = speed;
}

inline void Action::setTimingMode(TimingMode timingMode) {
    m_timingMode = timingMode;
}

inline sf::Vector2f const& Action::getOwnerCurrentPosition() const {
    return getOwnerCurrentTransform().getPosition();
}

inline void Action::setCallback(CompletionCallback && callback) {
    m_completionCallback = std::make_optional(std::forward<CompletionCallback>(callback));
}

}
