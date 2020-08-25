//
//  Action.cpp
//  Maze
//
//  Created by Jaraxus on 17/08/2020.
//

#include "../../include/Engine/Action.hpp"

namespace std {

void swap(mz::Action& lhs, mz::Action& rhs) noexcept {
    mz::Action::DataType tmpData {};
    mz::Action::ActionType tmpDataType = lhs.m_type;
    
    mz::Action::DataType::exchange(lhs.m_type,  lhs.m_data, tmpData);
    mz::Action::DataType::exchange(rhs.m_type,  rhs.m_data, lhs.m_data);
    mz::Action::DataType::exchange(tmpDataType, tmpData,    rhs.m_data);
    
    std::swap(lhs.m_completionCallback,         rhs.m_completionCallback);
    std::swap(lhs.m_duration,                   rhs.m_duration);
    std::swap(lhs.m_isRelativeToInitialState,   rhs.m_isRelativeToInitialState);
    std::swap(lhs.m_isRelativeToParent,         rhs.m_isRelativeToParent);
    std::swap(lhs.m_owner,                      rhs.m_owner);
    std::swap(lhs.m_speed,                      rhs.m_speed);
    std::swap(lhs.m_timeElapsed,                rhs.m_timeElapsed);
    std::swap(lhs.m_timingMode,                 rhs.m_timingMode);
    std::swap(lhs.m_type,                       rhs.m_type);
}

} // namespace std

namespace mz {

Action::DataType::DataType() {
    std::memset(this, 0, sizeof(DataType));
}

Action::DataType::~DataType() {}

void Action::DataType::exchange(mz::Action::ActionType type, mz::Action::DataType& inputData, mz::Action::DataType& outputData) {
    switch (type) {
        case mz::Action::ActionType::MOVE :
            outputData.moveData = std::move(inputData.moveData);
            break;
        case mz::Action::ActionType::ROTATE :
            outputData.rotateData.rotation = inputData.rotateData.rotation;
            break;
        case mz::Action::ActionType::SCALE :
            outputData.scaleData = std::move(inputData.scaleData);
            break;
        case mz::Action::ActionType::FOLLOW_PATH :
            outputData.pathData = std::move(inputData.pathData);
            break;
        case mz::Action::ActionType::REMOVE_FROM_PARENT : break;
        case mz::Action::ActionType::SEQUENCE :
            outputData.sequenceData = std::move(inputData.sequenceData);
            break;
        case mz::Action::ActionType::GROUP :
            outputData.groupData = std::move(inputData.groupData);
            break;
        case mz::Action::ActionType::SPEED :
            outputData.speedData.speed = inputData.speedData.speed;
            break;
        case mz::Action::ActionType::PAUSE : break;
        case mz::Action::ActionType::EMPTY : break;
    }
}

Action::Action(Action const& action) noexcept :
m_completionCallback(std::nullopt),
m_data(),
m_duration(action.m_duration),
m_isRelativeToInitialState(action.m_isRelativeToInitialState),
m_isRelativeToParent(action.m_isRelativeToParent),
m_owner(action.m_owner),
m_speed(action.m_speed),
m_timeElapsed(action.m_timeElapsed),
m_timingMode(action.m_timingMode),
m_type(action.m_type)
{
    switch (m_type) {
        case ActionType::MOVE :
            m_data.moveData.position = action.m_data.moveData.position;
            break;
        case ActionType::ROTATE :
            m_data.rotateData.rotation = action.m_data.rotateData.rotation;
            break;
        case ActionType::SCALE :
            m_data.scaleData.scaleFactor = action.m_data.scaleData.scaleFactor;
            break;
        case ActionType::FOLLOW_PATH :
            m_data.pathData.positions = action.m_data.pathData.positions;
            break;
        case ActionType::REMOVE_FROM_PARENT : break;
        case ActionType::SEQUENCE :
            for (auto const& action : action.m_data.sequenceData.actions)
                m_data.sequenceData.actions.emplace_back(action);
            break;
        case ActionType::GROUP :
            for (auto const& action : action.m_data.sequenceData.actions)
                m_data.sequenceData.actions.emplace_back(action);
            break;
        case ActionType::SPEED :
            m_data.speedData.speed = action.m_data.speedData.speed;
            break;
        case ActionType::PAUSE : break;
        case ActionType::EMPTY : break;
    }
}

Action::Action(Action && action) noexcept :
m_completionCallback(std::move(action.m_completionCallback)),
m_data(),
m_duration(action.m_duration),
m_isRelativeToInitialState(action.m_isRelativeToInitialState),
m_isRelativeToParent(action.m_isRelativeToParent),
m_owner(action.m_owner),
m_speed(action.m_speed),
m_timeElapsed(action.m_timeElapsed),
m_timingMode(action.m_timingMode),
m_type(action.m_type)
{
    DataType::exchange(m_type, action.m_data, m_data);
}

Action::~Action() {
    switch (m_type) {
        case ActionType::MOVE :
            m_data.moveData.position.~Vector2();
            break;
        case ActionType::ROTATE :
            break;
        case ActionType::SCALE :
            m_data.scaleData.scaleFactor.~Vector2();
            break;
        case ActionType::FOLLOW_PATH :
            m_data.pathData.positions.~vector();
            break;
        case ActionType::REMOVE_FROM_PARENT : break;
        case ActionType::SEQUENCE :
            m_data.sequenceData.actions.~vector();
            break;
        case ActionType::GROUP :
            m_data.groupData.actions.~vector();
            break;
        case ActionType::SPEED :
            break;
        case ActionType::PAUSE : break;
        case ActionType::EMPTY : break;
    }
}

Action Action::getReversed(Node* node) const {
    Action reversedAction = getReversedData(node);
    
    if (reversedAction.m_type == ActionType::EMPTY)
        mz::Logs::Global.display("Could not generate reversed action", LogMessageType::ERROR);
    
    reversedAction.setDuration(m_duration);
    reversedAction.setRelativeToParent(m_isRelativeToParent);
    reversedAction.setSpeed(m_speed);
    
    return reversedAction;
}

Action Action::Empty() {
    return Action(ActionType::EMPTY, false);
}

Action Action::FollowPath(std::vector<sf::Vector2f> const& path) {
    Action followAction {ActionType::FOLLOW_PATH, true};
    followAction.m_data.pathData.positions = path;
    return followAction;
}

Action Action::FollowPath(std::vector<sf::Vector2f> && path) {
    Action followAction {ActionType::FOLLOW_PATH, true};
    followAction.m_data.pathData.positions = std::forward<std::vector<sf::Vector2f>>(path);
    return followAction;
}

Action Action::Group(std::vector<Action> const& actions) {
    Action groupAction {ActionType::GROUP, false};
    for (auto const& action : actions)
        groupAction.m_data.sequenceData.actions.emplace_back(action);
    return groupAction;
}

Action Action::Group(std::vector<Action> && actions) {
    Action groupAction {ActionType::GROUP, false};
    groupAction.m_data.groupData.actions = std::forward<std::vector<Action>>(actions);
    return groupAction;
}

Action Action::Action::MoveBy(sf::Vector2f const& deltaPos) {
    Action moveAction {ActionType::MOVE, false};
    moveAction.m_data.moveData.position = deltaPos;
    return moveAction;
}

Action Action::MoveBy(float x, float y) {
    Action moveAction {ActionType::MOVE, false};
    moveAction.m_data.moveData.position = sf::Vector2f{x, y};
    return moveAction;
}

Action Action::MoveByX(float x) {
    Action moveAction {ActionType::MOVE, false};
    moveAction.m_data.moveData.position = sf::Vector2f{x, 0.f};
    return moveAction;
}

Action Action::MoveByY(float y) {
    Action moveAction {ActionType::MOVE, false};
    moveAction.m_data.moveData.position = sf::Vector2f{0.f, y};
    return moveAction;
}

Action Action::MoveTo(sf::Vector2f const& position) {
    Action moveAction {ActionType::MOVE, true};
    moveAction.m_data.moveData.position = position;
    return moveAction;
}

Action Action::MoveTo(float x, float y) {
    Action moveAction {ActionType::MOVE, true};
    moveAction.m_data.moveData.position = sf::Vector2f{x, y};
    return moveAction;
}

Action Action::MoveToX(float x) {
    Action moveAction {ActionType::MOVE, true};
    moveAction.m_data.moveData.position = sf::Vector2f{x, 0.f};
    return moveAction;
}

Action Action::MoveToY(float y) {
    Action moveAction {ActionType::MOVE, true};
    moveAction.m_data.moveData.position = sf::Vector2f{0.f, y};
    return moveAction;
}

Action Action::Pause(std::uint64_t duration) {
    Action pauseAction {ActionType::PAUSE, false};
    pauseAction.setDuration(duration);
    return pauseAction;
}

Action Action::RemoveFromParent() {
    return Action{ActionType::REMOVE_FROM_PARENT, false};
}

Action Action::RotateBy(float rotation) {
    Action rotateAction {ActionType::ROTATE, false};
    rotateAction.m_data.rotateData.rotation = rotation;
    return rotateAction;
}

Action Action::RotateTo(float rotation) {
    Action rotateAction {ActionType::ROTATE, true};
    rotateAction.m_data.rotateData.rotation = rotation;
    return rotateAction;
}

Action Action::ScaleBy(sf::Vector2f const& scaleFactor) {
    Action scaleAction {ActionType::SCALE, false};
    scaleAction.m_data.scaleData.scaleFactor = scaleFactor;
    return scaleAction;
}

Action Action::ScaleBy(float x, float y) {
    Action scaleAction {ActionType::SCALE, false};
    scaleAction.m_data.scaleData.scaleFactor = sf::Vector2f{x, y};
    return scaleAction;
}

Action Action::ScaleByX(float x) {
    Action scaleAction {ActionType::SCALE, false};
    scaleAction.m_data.scaleData.scaleFactor = sf::Vector2f{x, 0.f};
    return scaleAction;
}

Action Action::ScaleByY(float y) {
    Action scaleAction {ActionType::SCALE, false};
    scaleAction.m_data.scaleData.scaleFactor = sf::Vector2f{0.f, y};
    return scaleAction;
}

Action Action::ScaleTo(sf::Vector2f const& scaleFactor) {
    Action scaleAction {ActionType::SCALE, true};
    scaleAction.m_data.scaleData.scaleFactor = scaleFactor;
    return scaleAction;
}

Action Action::ScaleTo(float x, float y) {
    Action scaleAction {ActionType::SCALE, true};
    scaleAction.m_data.scaleData.scaleFactor = sf::Vector2f{x, y};
    return scaleAction;
}

Action Action::ScaleToX(float x) {
    Action scaleAction {ActionType::SCALE, true};
    scaleAction.m_data.scaleData.scaleFactor = sf::Vector2f{x, 0.f};
    return scaleAction;
}

Action Action::ScaleToY(float y) {
    Action scaleAction {ActionType::SCALE, true};
    scaleAction.m_data.scaleData.scaleFactor = sf::Vector2f{0.f, y};
    return scaleAction;
}

Action Action::Sequence(std::vector<Action> const& actions) {
    Action sequenceAction {ActionType::SEQUENCE, false};
    for (auto const& action : actions)
        sequenceAction.m_data.sequenceData.actions.emplace_back(action);
    return sequenceAction;
}

Action Action::Sequence(std::vector<Action> && actions) {
    Action sequenceAction {ActionType::SEQUENCE, false};
    sequenceAction.m_data.sequenceData.actions = std::forward<std::vector<Action>>(actions);
    return sequenceAction;
}

Action Action::SpeedBy(float speedFactor) {
    Action speedAction {ActionType::SPEED, false};
    speedAction.m_data.speedData.speed = speedFactor;
    return speedAction;
}

Action Action::SpeedTo(float speedFactor) {
    Action speedAction {ActionType::SPEED, true};
    speedAction.m_data.speedData.speed = speedFactor;
    return speedAction;
}
    
Action::Action(ActionType type, bool isRelativeToInitialState) :
m_completionCallback(std::nullopt),
m_data(),
m_duration(0.f),
m_isRelativeToInitialState(isRelativeToInitialState),
m_isRelativeToParent(true),
m_owner(nullptr),
m_speed(1.f),
m_timeElapsed(0),
m_timingMode(TimingMode::LINEAR),
m_type(type)
{
}

void Action::completeInit(Node* owner) {
    assert(owner);
    m_owner = owner;
    
    switch (m_type) {
        case ActionType::MOVE :
            completeInitMove();
            break;
        case ActionType::ROTATE :
            completeInitRotate();
            break;
        case ActionType::SCALE :
            completeInitScale();
            break;
        case ActionType::FOLLOW_PATH :
            completeInitFollowPath();
            break;
        case ActionType::REMOVE_FROM_PARENT : break;
        case ActionType::SEQUENCE :
            completeInitSequence();
            break;
        case ActionType::GROUP :
            completeInitGroup();
            break;
        case ActionType::SPEED :
            completeInitSpeed();
            break;
        case ActionType::PAUSE : break;
        case ActionType::EMPTY : break;
    }
}

void Action::completeInitFollowPath() {
    sf::Vector2f previousPosition = getOwnerCurrentPosition();
    float distance = 0.f;
    for (auto const& nextPosition : m_data.pathData.positions) {
        distance += getVectorLength(nextPosition - previousPosition);
        previousPosition = nextPosition;
    }
    m_data.pathData.distance = distance;
    m_data.pathData.targetPositionId = std::size_t(0);
}

void Action::completeInitGroup() {
    completeInitOfActions(m_data.groupData.actions);
    m_duration = 0.f;
    for (auto& action : m_data.groupData.actions)
        m_duration = std::max(action.m_duration, m_duration);
}

void Action::completeInitMove() {
    assert(m_duration != 0.f);
    if (m_isRelativeToInitialState)
        m_data.moveData.position = getOwnerCurrentPosition() - m_data.moveData.position;
}

void Action::completeInitOfActions(std::vector<Action>& actions) {
    for (auto& action : actions)
        action.completeInit(m_owner);
}

void Action::completeInitRotate() {
    assert(m_duration != 0.f);
    if (m_isRelativeToParent)
        m_data.rotateData.rotation = getOwnerCurrentTransform().getRotation() - m_data.rotateData.rotation;
}

void Action::completeInitScale() {
    assert(m_duration != 0.f);
    if (m_isRelativeToParent)
        m_data.scaleData.scaleFactor = getOwnerCurrentTransform().getScale() - m_data.scaleData.scaleFactor;
}

void Action::completeInitSequence() {
    completeInitOfActions(m_data.sequenceData.actions);
    m_data.sequenceData.playedActionId = 0;
    m_duration = 0.f;
    for (auto const& action : m_data.sequenceData.actions)
        m_duration += action.m_duration;
}

void Action::completeInitSpeed() {
    // TODO: implement speed
}

Action Action::getAbsoluteReversed() const {
    switch (m_type) {
        case ActionType::MOVE :
            return Action::MoveBy(-m_data.moveData.position);
        case ActionType::ROTATE :
            return Action::RotateBy(-m_data.rotateData.rotation);
        case ActionType::SCALE :
            assert(m_data.scaleData.scaleFactor.x > 0.f && m_data.scaleData.scaleFactor.y > 0.f);
            return Action::ScaleBy(1.f / m_data.scaleData.scaleFactor.x, 1.f / m_data.scaleData.scaleFactor.y);
        case ActionType::FOLLOW_PATH : break;
        case ActionType::REMOVE_FROM_PARENT : break;
        case ActionType::SEQUENCE : break;
        case ActionType::GROUP : break;
        case ActionType::SPEED :
            assert(m_data.speedData.speed > 0.f);
            return Action::SpeedBy(1.f / m_data.speedData.speed);
        case ActionType::PAUSE : break;
        case ActionType::EMPTY : break;
    }
    return Action::Empty();
}

std::vector<Action> Action::getActionsReversed(std::vector<Action> const& actions, Node* node) const {
    std::vector<Action> reversedActions;
    for (std::size_t i = 0; i < actions.size(); ++i)
        reversedActions.emplace_back(actions[actions.size() - i - 1].getReversed(node));
    return reversedActions;
}

sf::Transformable const& Action::getOwnerCurrentTransform() const {
    return (m_isRelativeToParent ? m_owner->getRelativeTransform() : m_owner->getGlobalTransform());
}

std::vector<sf::Vector2f> Action::getPathReversed(std::vector<sf::Vector2f> const& path, Node* node) const {
    std::vector<sf::Vector2f> pathReversed;
    for (std::size_t i = 0; i < path.size(); ++i)
        pathReversed.emplace_back(path[path.size() - i - 2]);
    
    if (m_isRelativeToParent)
        pathReversed.emplace_back(node->getRelativeTransform().getPosition());
    else
        pathReversed.emplace_back(node->getGlobalTransform().getPosition());
    
    return pathReversed;
}

float Action::getProgress(std::uint64_t timeElapsed, std::uint64_t duration) {
    float ratio = static_cast<float>(timeElapsed) / static_cast<float>(duration);
    switch (m_timingMode) {
        case TimingMode::LINEAR :
            return ratio;
        case TimingMode::EASE_IN : // TODO: use bezier curves
            return ratio;
        case TimingMode::EASE_OUT : // TODO: use bezier curves
            return ratio;
        case TimingMode::EASE_IN_OUT : // TODO: use bezier curves
            return ratio;
    }
}

Action Action::getRelativeReversed(Node* node) const {
    auto& nodeTransform = (m_isRelativeToParent ? node->getRelativeTransform() : node->getGlobalTransform());
    switch (m_type) {
        case ActionType::MOVE :
            return Action::MoveTo(nodeTransform.getPosition());
        case ActionType::ROTATE :
            return Action::RotateTo(nodeTransform.getRotation());
        case ActionType::SCALE :
            return Action::ScaleTo(nodeTransform.getScale());
        case ActionType::FOLLOW_PATH : break;
        case ActionType::REMOVE_FROM_PARENT : break;
        case ActionType::SEQUENCE : break;
        case ActionType::GROUP : break;
        case ActionType::SPEED :
            return Action::SpeedTo(m_data.speedData.speed);
        case ActionType::PAUSE : break;
        case ActionType::EMPTY : break;
    }
    return Action::Empty();
}

Action Action::getReversedData(Node* node) const {
    if (m_type == ActionType::SEQUENCE) {
        return Action::Sequence(getActionsReversed(m_data.sequenceData.actions, node));
    } else if (m_type == ActionType::GROUP) {
        return  Action::Group(getActionsReversed(m_data.groupData.actions, node));
    } else if (m_isRelativeToInitialState && node) {
        if (m_type == FOLLOW_PATH)
            return Action::FollowPath(getPathReversed(m_data.pathData.positions, node));
        else
            return getRelativeReversed(node);
    } else {
        return getAbsoluteReversed();
    }
    return Action::Empty();
}

std::uint64_t Action::getTimeUsed(std::uint64_t timeElapsed) {
    switch (m_type) {
        case MOVE :
        case ROTATE :
        case SCALE :
        case FOLLOW_PATH :
        case SEQUENCE :
        case GROUP :
        case SPEED :
        case PAUSE :
            return (m_timeElapsed + timeElapsed < m_duration ? timeElapsed : m_duration - m_timeElapsed);
        case REMOVE_FROM_PARENT :
        case EMPTY :
            return 0;
    }
}

std::uint64_t Action::update(std::uint64_t timeElapsed) {
    const std::uint64_t timeUsed = getTimeUsed(timeElapsed);
    const float progress = getProgress(m_timeElapsed + timeUsed, m_duration) - getCurrentProgress();
    
    std::cout << m_timeElapsed << " " << m_duration << " " << timeUsed << " " << timeElapsed << "\n";
    
    switch (m_type) {
        case ActionType::MOVE :
            updateMove(progress);
            break;
        case ActionType::ROTATE :
            updateRotate(progress);
            break;
        case ActionType::SCALE :
            updateScale(progress);
            break;
        case ActionType::FOLLOW_PATH :
            updateFollowPath(progress);
            break;
        case ActionType::REMOVE_FROM_PARENT :
            updateRemove();
            break;
        case ActionType::SEQUENCE :
            updateSequence(progress);
            break;
        case ActionType::GROUP :
            updateGroup(progress);
            break;
        case ActionType::SPEED :
            updateSpeed(progress);
            break;
        case ActionType::PAUSE : break;
        case ActionType::EMPTY : break;
    }
    
    m_timeElapsed += timeUsed;
    
    if (isCompleted()) {
        if (m_completionCallback)
            m_completionCallback.value()();
        return timeElapsed - timeUsed;
    }
    
    return 0;
}

void Action::updateFollowPath(float progress) {
    float movingDistanceLeft = progress * m_data.pathData.distance;
    
    while (m_data.pathData.targetPositionId < m_data.pathData.positions.size()) {
        auto const& currentPosition = getOwnerCurrentPosition();
        const sf::Vector2f deltaPos = m_data.pathData.positions[m_data.pathData.targetPositionId] - currentPosition;
        const float distanceToTarget = getVectorLength(deltaPos);
        
        if (movingDistanceLeft > distanceToTarget) {
            m_owner->move(deltaPos);
            movingDistanceLeft -= distanceToTarget;
            ++m_data.pathData.targetPositionId;
        } else {
            m_owner->move(deltaPos * (movingDistanceLeft / distanceToTarget));
            break;
        }
    }
}

void Action::updateGroup(float progress) {
    std::uint64_t timeElapsed = static_cast<std::uint64_t>(progress * m_duration);
    for (auto& action : m_data.groupData.actions) {
        if (!action.isCompleted())
            action.update(timeElapsed);
    }
}

void Action::updateMove(float progress) {
    m_owner->move(progress * m_data.moveData.position);
}

void Action::updateRemove() {
    m_owner->removeFromParent();
}

void Action::updateRotate(float progress) {
    float rotation = progress * m_data.rotateData.rotation;
    m_owner->rotate(rotation);
}

void Action::updateScale(float progress) {
    auto scale =  sf::Vector2f(1.f, 1.f) - progress * m_data.scaleData.scaleFactor;
    m_owner->scale(scale);
}

void Action::updateSequence(float progress) {
    std::uint64_t timeLeft = static_cast<std::uint64_t>(progress * m_duration);
    while (m_data.sequenceData.playedActionId < m_data.sequenceData.actions.size()) {
        auto& action = m_data.sequenceData.actions[m_data.sequenceData.playedActionId];
        timeLeft = action.update(timeLeft);
        if (action.isCompleted())
            ++m_data.sequenceData.playedActionId;
        else
            break;
    }
}

void Action::updateSpeed(float progress) {
    
}

}
