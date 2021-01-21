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
	assert(m_timeElapsed <= m_duration);
    return (m_timeElapsed == m_duration);
}

inline bool Action::isPaused() const {
    return m_isPaused;
}

inline Action& Action::setCallback(CompletionCallback && callback) {
    m_completionCallback = std::make_optional(std::forward<CompletionCallback>(callback));
    return *this;
}

inline Action& Action::setDuration(std::uint64_t duration) {
    m_duration = std::max(duration, static_cast<std::uint64_t>(1));
    return *this;
}

inline Action& Action::setDuration(sf::Time duration) {
    m_duration = std::max(static_cast<std::uint64_t>(duration.asMicroseconds()), static_cast<std::uint64_t>(1));
    return *this;
}

template <typename DurationT, typename _>
inline Action& Action::setDuration(DurationT duration) {
    m_duration = std::max(static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()), static_cast<std::uint64_t>(1));
    return *this;
}

inline Action& Action::setPaused(bool isPaused) {
    m_isPaused = isPaused;
    return *this;
}

inline Action& Action::setRelativeToParent(bool isRelativeToParent) {
    m_isRelativeToParent = isRelativeToParent;
    return *this;
}

inline Action& Action::setSpeed(float speed) {
    assert(speed > 0.f);
    m_speed = speed;
    return *this;
}

inline Action& Action::setTimingMode(TimingMode timingMode) {
    m_timingMode = timingMode;
    return *this;
}

inline sf::Vector2f const& Action::getOwnerCurrentPosition() const {
    return getOwnerCurrentTransform().getPosition();
}

template <typename ...Vertexes>
Action Action::FollowPath(Vertexes && ...vertexes) {
    static_assert(((std::is_same_v<Vertexes, sf::Vector2f> || std::is_convertible_v<Vertexes, sf::Vector2f>), ...), "Vertexes must be (convertible to) sf::Vector2f");
    
    Action followAction {ActionType::FOLLOW_PATH, true};
    (followAction.m_data.pathData.positions.emplace_back(std::forward<Vertexes>(vertexes)), ...);
    return followAction;
}

template <typename ...Actions>
Action Action::Group(Actions && ...actions) {
    static_assert((std::is_same_v<Actions, Action>, ...), "Actions must all be of type Action");
    
    Action groupAction {ActionType::GROUP, true};
    (groupAction.m_data.groupData.actions.emplace_back(std::forward<Actions>(actions)), ...);
    return groupAction;
}

template <typename ...Actions>
Action Action::Sequence(Actions && ...actions) {
    static_assert((std::is_same_v<Actions, Action>, ...), "Actions must all be of type Action");
    
    Action sequenceAction {ActionType::SEQUENCE, true};
    (sequenceAction.m_data.sequenceData.actions.emplace_back(std::forward<Actions>(actions)), ...);
    return sequenceAction;
}

inline void Action::callCompletionCallbacks() {
    callSubActionsCallbacks();
    if (m_completionCallback)
        m_completionCallback.value()();
}

inline void Action::resetProperties() {
    m_duration = 0;
    m_isPaused = false;
}

}
