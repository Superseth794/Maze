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

inline bool Action::isPaused() const {
    return m_isPaused;
}

inline void Action::setCallback(CompletionCallback && callback) {
    m_completionCallback = std::make_optional(std::forward<CompletionCallback>(callback));
}

inline void Action::setDuration(std::uint64_t duration) {
    m_duration = std::max(duration, static_cast<std::uint64_t>(1));
}

inline void Action::setPaused(bool isPaused) {
    m_isPaused = isPaused;
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
    groupAction.m_data.groupData.actions.emplace_back(std::forward<Actions...>(actions)...);
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
