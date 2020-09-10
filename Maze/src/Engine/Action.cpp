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
        case mz::Action::ActionType::HIDE :
            outputData.hideData = std::move(inputData.hideData);
            break;
        case mz::Action::ActionType::REMOVE_FROM_PARENT : break;
        case mz::Action::ActionType::SEQUENCE :
            outputData.sequenceData = std::move(inputData.sequenceData);
            break;
        case mz::Action::ActionType::GROUP :
            outputData.groupData = std::move(inputData.groupData);
            break;
        case mz::Action::ActionType::REPEAT :
            outputData.repeatData = std::move(inputData.repeatData);
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
m_isPaused(action.isPaused()),
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
        case ActionType::HIDE :
            m_data.hideData.hidden = action.m_data.hideData.hidden;
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
        case ActionType::REPEAT :
            m_data.repeatData.callCompletionCallbackEveryRestart = action.m_data.repeatData.callCompletionCallbackEveryRestart;
            m_data.repeatData.initialAction = std::make_unique<Action>(*action.m_data.repeatData.initialAction);
            m_data.repeatData.repeatCount = action.m_data.repeatData.repeatCount;
            m_data.repeatData.repeatForever = action.m_data.repeatData.repeatForever;
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
m_duration(std::move(action.m_duration)),
m_isPaused(std::move(action.isPaused())),
m_isRelativeToInitialState(std::move(action.m_isRelativeToInitialState)),
m_isRelativeToParent(std::move(action.m_isRelativeToParent)),
m_owner(std::move(action.m_owner)),
m_speed(std::move(action.m_speed)),
m_timeElapsed(std::move(action.m_timeElapsed)),
m_timingMode(std::move(action.m_timingMode)),
m_type(std::move(action.m_type))
{
    DataType::exchange(m_type, action.m_data, m_data);
}

Action::~Action() {
    switch (m_type) {
        case ActionType::MOVE :
            m_data.moveData.position.~Vector2();
            break;
        case ActionType::ROTATE : break;
        case ActionType::SCALE :
            m_data.scaleData.scaleFactor.~Vector2();
            break;
        case ActionType::FOLLOW_PATH :
            m_data.pathData.positions.~vector();
            break;
        case ActionType::HIDE : break;
        case ActionType::REMOVE_FROM_PARENT : break;
        case ActionType::SEQUENCE :
            m_data.sequenceData.actions.~vector();
            break;
        case ActionType::GROUP :
            m_data.groupData.actions.~vector();
            break;
        case ActionType::REPEAT :
            m_data.repeatData.currentAction.~unique_ptr();
            m_data.repeatData.initialAction.~unique_ptr();
            break;
        case ActionType::SPEED : break;
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
    Action groupAction {ActionType::GROUP, true};
    for (auto const& action : actions)
        groupAction.m_data.sequenceData.actions.emplace_back(action);
    return groupAction;
}

Action Action::Group(std::vector<Action> && actions) {
    Action groupAction {ActionType::GROUP, true};
    groupAction.m_data.groupData.actions = std::forward<std::vector<Action>>(actions);
    return groupAction;
}

Action Action::Hide() {
    Action hideAction {ActionType::HIDE, false};
    hideAction.m_data.hideData.hidden = true;
    return hideAction;
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

Action Action::Repeat(Action && action, std::size_t count, bool callCompletionCallbackEveryRestart) {
    Action repeatAction {ActionType::REPEAT, false};
    repeatAction.m_data.repeatData.callCompletionCallbackEveryRestart = callCompletionCallbackEveryRestart;
    repeatAction.m_data.repeatData.repeatCount = count;
    repeatAction.m_data.repeatData.repeatForever = false;
    repeatAction.m_data.repeatData.initialAction = std::make_unique<Action>(std::forward<Action>(action));
    return repeatAction;
}

Action Action::Repeat(std::unique_ptr<Action> && action, std::size_t count, bool callCompletionCallbackEveryRestart) {
    Action repeatAction {ActionType::REPEAT, false};
    repeatAction.m_data.repeatData.callCompletionCallbackEveryRestart = callCompletionCallbackEveryRestart;
    repeatAction.m_data.repeatData.repeatCount = count;
    repeatAction.m_data.repeatData.repeatForever = false;
    repeatAction.m_data.repeatData.initialAction = std::forward<std::unique_ptr<Action>>(action);
    return repeatAction;
}

Action Action::RepeatForever(Action && action, bool callCompletionCallbackEveryRestart) {
    Action repeatAction {ActionType::REPEAT, false};
    repeatAction.m_data.repeatData.callCompletionCallbackEveryRestart = callCompletionCallbackEveryRestart;
    repeatAction.m_data.repeatData.repeatForever = true;
    repeatAction.m_data.repeatData.initialAction = std::make_unique<Action>(std::forward<Action>(action));
    return repeatAction;
}

Action Action::RepeatForever(std::unique_ptr<Action> && action, bool callCompletionCallbackEveryRestart) {
    Action repeatAction {ActionType::REPEAT, false};
    repeatAction.m_data.repeatData.callCompletionCallbackEveryRestart = callCompletionCallbackEveryRestart;
    repeatAction.m_data.repeatData.repeatForever = true;
    repeatAction.m_data.repeatData.initialAction = std::forward<std::unique_ptr<Action>>(action);
    return repeatAction;
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
    Action sequenceAction {ActionType::SEQUENCE, true};
    for (auto const& action : actions)
        sequenceAction.m_data.sequenceData.actions.emplace_back(action);
    return sequenceAction;
}

Action Action::Sequence(std::vector<Action> && actions) {
    Action sequenceAction {ActionType::SEQUENCE, true};
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

Action Action::SwitchHidden() {
    return Action {ActionType::HIDE, true};
}

Action Action::Unhide() {
    Action unhideAction {ActionType::HIDE, false};
    unhideAction.m_data.hideData.hidden = false;
    return unhideAction;
}
    
Action::Action(ActionType type, bool isRelativeToInitialState) :
m_completionCallback(std::nullopt),
m_data(),
m_isRelativeToInitialState(isRelativeToInitialState),
m_isRelativeToParent(false),
m_owner(nullptr),
m_timeElapsed(0),
m_type(type)
{
}

void Action::callSubActionsCallbacks() {
    if (m_type == ActionType::SEQUENCE) {
        for (std::size_t actionId = m_data.sequenceData.playedActionId; actionId < m_data.sequenceData.actions.size(); ++actionId)
            m_data.sequenceData.actions[actionId].callCompletionCallbacks();
    } else if (m_type == ActionType::GROUP) {
        for (auto& action : m_data.groupData.actions) {
            if (!action.isCompleted())
                action.callCompletionCallbacks();
        }
    } else if (m_type == ActionType::REPEAT) {
        if (!m_data.repeatData.currentAction->isCompleted())
            m_data.repeatData.currentAction->callCompletionCallbacks();
    }
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
        case ActionType::HIDE :
            completeInitHide();
            break;
        case ActionType::SEQUENCE :
            completeInitSequence();
            break;
        case ActionType::GROUP :
            completeInitGroup();
            break;
        case ActionType::REPEAT :
            completeInitRepeat();
            break;
        case ActionType::SPEED :
            completeInitSpeed();
            break;
        case ActionType::PAUSE : break;
        case ActionType::REMOVE_FROM_PARENT :
        case ActionType::EMPTY :
            resetProperties();
            break;
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
    m_duration = 0;
    for (auto& action : m_data.groupData.actions)
        m_duration = std::max(action.m_duration, m_duration);
}

void Action::completeInitHide() {
    if (m_isRelativeToInitialState)
        m_data.hideData.hidden = !m_owner->isHidden();
}

void Action::completeInitMove() {
    if (m_isRelativeToInitialState)
        m_data.moveData.position = getOwnerCurrentPosition() - m_data.moveData.position;
}

void Action::completeInitOfActions(std::vector<Action>& actions) {
    for (auto& action : actions)
        action.completeInit(m_owner);
}

void Action::completeInitRepeat() {
    m_data.repeatData.currentAction = std::make_unique<Action>(*m_data.repeatData.initialAction);
    m_data.repeatData.currentAction->m_completionCallback = std::move(m_data.repeatData.initialAction->m_completionCallback);
    m_data.repeatData.currentAction->completeInit(m_owner);
    
    if (m_data.repeatData.repeatForever) {
        m_timingMode = TimingMode::LINEAR;
        m_duration = std::numeric_limits<decltype(m_duration)>::max();
        m_data.repeatData.repeatCount = 0;
    } else {
        m_duration = m_data.repeatData.currentAction->m_duration * m_data.repeatData.repeatCount;
    }
    
    assert(!(m_data.repeatData.callCompletionCallbackEveryRestart && !m_completionCallback));
}

void Action::completeInitRotate() {
    if (m_isRelativeToParent)
        m_data.rotateData.rotation = getOwnerCurrentTransform().getRotation() - m_data.rotateData.rotation;
}

void Action::completeInitScale() {
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
    if (m_isRelativeToInitialState)
        m_data.speedData.speed -= m_owner->getSpeed();
}

Action Action::getDataUnrelativeToNodeReversed() const {
    switch (m_type) {
        case ActionType::MOVE :
            return Action::MoveBy(-m_data.moveData.position);
        case ActionType::ROTATE :
            return Action::RotateBy(-m_data.rotateData.rotation);
        case ActionType::SCALE :
            assert(m_data.scaleData.scaleFactor.x > 0.f && m_data.scaleData.scaleFactor.y > 0.f);
            return Action::ScaleBy(1.f / m_data.scaleData.scaleFactor.x, 1.f / m_data.scaleData.scaleFactor.y);
        case ActionType::FOLLOW_PATH : break;
        case ActionType::HIDE :
            return (m_data.hideData.hidden ? Action::Unhide() : Action::Hide());
        case ActionType::REMOVE_FROM_PARENT : break;
        case ActionType::SEQUENCE : break;
        case ActionType::GROUP : break;
        case ActionType::REPEAT :
            return (m_data.repeatData.repeatForever ? Action::RepeatForever(m_data.repeatData.initialAction->getReversed()) : Action::Repeat(m_data.repeatData.initialAction->getReversed(), m_data.repeatData.repeatCount));
        case ActionType::SPEED :
            assert(m_data.speedData.speed > 0.f);
            return Action::SpeedBy(-m_data.speedData.speed);
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

Action Action::getDataRelativeToNodeReversed(Node* node) const {
    auto& nodeTransform = (m_isRelativeToParent ? node->getRelativeTransform() : node->getGlobalTransform());
    switch (m_type) {
        case ActionType::MOVE :
            return Action::MoveBy(nodeTransform.getPosition() - m_data.moveData.position);
        case ActionType::ROTATE :
            return Action::RotateBy(nodeTransform.getRotation() - m_data.rotateData.rotation);
        case ActionType::SCALE :
            return Action::ScaleBy(nodeTransform.getScale() - m_data.scaleData.scaleFactor);
        case ActionType::FOLLOW_PATH :
            return Action::FollowPath(getPathReversed(m_data.pathData.positions, node));
        case ActionType::HIDE :
            return (node->isHidden() ? Action::Unhide() : Action::Hide());
        case ActionType::REMOVE_FROM_PARENT : break;
        case ActionType::SEQUENCE :
            return Action::Sequence(getActionsReversed(m_data.sequenceData.actions, node));
        case ActionType::GROUP :
            return  Action::Group(getActionsReversed(m_data.groupData.actions, node));
        case ActionType::REPEAT : break;
        case ActionType::SPEED :
            return Action::SpeedBy(node->getSpeed() - m_data.speedData.speed);
        case ActionType::PAUSE : break;
        case ActionType::EMPTY : break;
    }
    return Action::Empty();
}

Action Action::getReversedData(Node* node) const {
    if (m_isRelativeToInitialState) {
        if (!node && !(m_type == SEQUENCE || m_type == GROUP))
            return Action::Empty();
        return getDataRelativeToNodeReversed(node);
    } else {
        return getDataUnrelativeToNodeReversed();
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
        case REPEAT :
        case SPEED :
        case PAUSE :
            return (m_timeElapsed + timeElapsed < m_duration ? timeElapsed : m_duration - m_timeElapsed);
        case HIDE :
        case REMOVE_FROM_PARENT :
        case EMPTY :
            return 0;
    }
}

std::uint64_t Action::update(std::uint64_t timeElapsed) {
    if (m_isPaused)
        return timeElapsed;
    
    timeElapsed *= m_owner->getSpeed() * m_speed;
    
    const std::uint64_t timeUsed = getTimeUsed(timeElapsed);
    const float progress = getProgress(m_timeElapsed + timeUsed, m_duration) - getCurrentProgress();
    
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
        case ActionType::HIDE :
            updateHide();
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
        case ActionType::REPEAT :
            updateRepeat(timeUsed, progress);
            break;
        case ActionType::SPEED :
            updateSpeed(progress);
            break;
        case ActionType::PAUSE : break;
        case ActionType::EMPTY : break;
    }
    
    m_timeElapsed += timeUsed;
    
    if (isCompleted()) {
        callCompletionCallbacks();
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

void Action::updateHide() {
    m_owner->setHidden(m_data.hideData.hidden);
}

void Action::updateMove(float progress) {
    m_owner->move(progress * m_data.moveData.position);
}

void Action::updateRemove() {
    m_owner->removeFromParent();
}

void Action::updateRepeat(std::uint64_t timeElapsed, float progress) {
    std::uint64_t timeUsed = (m_data.repeatData.repeatForever ? timeElapsed : static_cast<std::uint64_t>(std::round(progress * m_duration))); // distinction is made because if action repeats forever the duration is (2^64 - 1) which leads to floating-point errors as (timeElapsed << duration -> progress << 1)
    std::uint64_t timeLeft = m_data.repeatData.currentAction->update(timeUsed);
    
    if (timeLeft == 0)
        return;
    
    if (!m_data.repeatData.repeatForever && --m_data.repeatData.repeatCount == 0)
        return;
    
    if (m_data.repeatData.callCompletionCallbackEveryRestart)
        m_completionCallback.value()();
    
    auto callback = std::move(m_data.repeatData.currentAction->m_completionCallback);
    m_data.repeatData.currentAction = std::make_unique<Action>(*m_data.repeatData.initialAction);
    m_data.repeatData.currentAction->m_completionCallback = std::move(callback);
    m_data.repeatData.currentAction->completeInit(m_owner);
    
    update(timeElapsed - timeUsed);
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
    m_owner->setSpeed(m_owner->getSpeed() + m_data.speedData.speed * progress);
}

}
