//
//  Action.cpp
//  Maze
//
//  Created by Jaraxus on 17/08/2020.
//

#include "../../include/Engine/Action.hpp"

namespace std {

void swap(mz::Action& lhs, mz::Action& rhs) noexcept {
    std::swap(lhs.m_completionCallback,         rhs.m_completionCallback);
    std::swap(lhs.m_data,                       rhs.m_data);
    std::swap(lhs.m_duration,                   rhs.m_duration);
    std::swap(lhs.m_isRelativeToInitialState,   rhs.m_isRelativeToInitialState);
    std::swap(lhs.m_isRelativeToParent,         rhs.m_isRelativeToParent);
    std::swap(lhs.m_owner,                      rhs.m_owner);
    std::swap(lhs.m_parentPool,                 rhs.m_parentPool);
    std::swap(lhs.m_speed,                      rhs.m_speed);
    std::swap(lhs.m_timeElapsed,                rhs.m_timeElapsed);
    std::swap(lhs.m_timingMode,                 rhs.m_timingMode);
    std::swap(lhs.m_type,                       rhs.m_type);
}

} // namespace std

namespace mz {

Action::Action(Action const& action) noexcept :
m_completionCallback(std::nullopt),
m_data(),
m_duration(action.m_duration),
m_isPaused(action.isPaused()),
m_isRelativeToInitialState(action.m_isRelativeToInitialState),
m_isRelativeToParent(action.m_isRelativeToParent),
m_owner(action.m_owner),
m_parentPool(action.m_parentPool),
m_speed(action.m_speed),
m_timeElapsed(action.m_timeElapsed),
m_timingMode(action.m_timingMode),
m_type(action.m_type)
{
    switch (m_type) {
        case ActionType::MOVE :
            m_data.emplace<MoveData>(std::get<MoveData>(action.m_data));
            break;
        case ActionType::ROTATE :
            m_data.emplace<RotateData>(std::get<RotateData>(action.m_data));
            break;
        case ActionType::SCALE :
            m_data.emplace<ScaleData>(std::get<ScaleData>(action.m_data));
            break;
        case ActionType::FOLLOW_PATH :
            m_data.emplace<PathData>(std::get<PathData>(action.m_data));
            break;
        case ActionType::HIDE :
            m_data.emplace<HideData>(std::get<HideData>(action.m_data));
            break;
        case ActionType::REMOVE_FROM_PARENT :
            m_data.emplace<RemoveData>();
            break;
        case ActionType::SEQUENCE :
        {
            m_data.emplace<SequenceData>();
            ActionsList const& actionsList = std::get<SequenceData>(action.m_data).actionsList;
            ActionsList& constructedActionsList = std::get<SequenceData>(m_data).actionsList;
            constructedActionsList.actionsHandled = actionsList.actionsHandled;
            
            if (constructedActionsList.actionsHandled) {
                for (auto const& action : constructedActionsList.actions.emplace<HandledActions>())
                    std::get<HandledActions>(constructedActionsList.actions).emplace_back(m_parentPool->New(*action));
            } else {
                for (auto const& action : constructedActionsList.actions.emplace<UnhandledActions>())
                    std::get<UnhandledActions>(constructedActionsList.actions).push_back(action);
            }
        }
            break;
        case ActionType::GROUP :
        {
            m_data.emplace<GroupData>();
            ActionsList const& actionsList = std::get<GroupData>(action.m_data).actionsList;
            ActionsList& constructedActionsList = std::get<GroupData>(m_data).actionsList;
            constructedActionsList.actionsHandled = actionsList.actionsHandled;
            
            if (constructedActionsList.actionsHandled) {
                constructedActionsList.actions.emplace<HandledActions>();
                for (auto const& action : std::get<HandledActions>(actionsList.actions))
                    std::get<HandledActions>(constructedActionsList.actions).emplace_back(m_parentPool->New(*action));
            } else {
                constructedActionsList.actions.emplace<UnhandledActions>();
                for (auto const& action : std::get<UnhandledActions>(actionsList.actions))
                    std::get<UnhandledActions>(constructedActionsList.actions).push_back(action);
            }
        }
            break;
        case ActionType::REPEAT :
        {
            RepeatData repeatData{};
            repeatData.callCompletionCallbackEveryRestart = std::get<RepeatData>(action.m_data).callCompletionCallbackEveryRestart;
            repeatData.initialAction = std::make_unique<Action>(*std::get<RepeatData>(action.m_data).initialAction); // TODO: use parentPool
            repeatData.repeatCount = std::get<RepeatData>(action.m_data).repeatCount;
            repeatData.repeatForever = std::get<RepeatData>(action.m_data).repeatForever;
            m_data.emplace<RepeatData>(std::move(repeatData));
        }
            break;
        case ActionType::SPEED :
            m_data.emplace<SpeedData>(std::get<SpeedData>(action.m_data));
            break;
        case ActionType::PAUSE :
            m_data.emplace<PauseData>();
            break;
        case ActionType::EMPTY :
            m_data.emplace<EmptyData>();
            break;
    }
}

Action::Action(Action && action) noexcept :
m_completionCallback(std::move(action.m_completionCallback)),
m_data(std::move(action.m_data)),
m_duration(action.m_duration),
m_isPaused(action.isPaused()),
m_isRelativeToInitialState(action.m_isRelativeToInitialState),
m_isRelativeToParent(action.m_isRelativeToParent),
m_owner(action.m_owner),
m_parentPool(action.m_parentPool),
m_speed(action.m_speed),
m_timeElapsed(action.m_timeElapsed),
m_timingMode(action.m_timingMode),
m_type(action.m_type)
{
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
    followAction.m_data.emplace<PathData>();
    std::get<PathData>(followAction.m_data).positions = path;
    return followAction;
}

Action Action::FollowPath(std::vector<sf::Vector2f> && path) {
    Action followAction {ActionType::FOLLOW_PATH, true};
    followAction.m_data.emplace<PathData>();
    std::get<PathData>(followAction.m_data).positions = std::move(path);
    return followAction;
}

Action Action::Group(std::vector<Action> const& actions) {
    Action groupAction {ActionType::GROUP, true};
    groupAction.m_data.emplace<GroupData>();
    
    ActionsList& actionsList = std::get<GroupData>(groupAction.m_data).actionsList;
    actionsList.actions.emplace<UnhandledActions>();
    actionsList.actionsHandled = false;
    
    for (auto const& action : actions)
        std::get<UnhandledActions>(actionsList.actions).emplace_back(action);
    
    return groupAction;
}

Action Action::Group(ActionPool* parentPool, std::vector<Action> const& actions) {
    assert(parentPool);
    Action groupAction = Group(actions);
    groupAction.m_parentPool = parentPool;
    return groupAction;
}

Action Action::Group(ActionPool* parentPool, std::vector<ActionHandler> const& actions) {
    assert(parentPool);
    Action groupAction {ActionType::GROUP, true};
    groupAction.m_data.emplace<GroupData>();
    
    ActionsList& actionsList = std::get<GroupData>(groupAction.m_data).actionsList;
    actionsList.actions.emplace<HandledActions>();
    actionsList.actionsHandled = true;
    
    for (auto const& action : actions)
        std::get<HandledActions>(actionsList.actions).emplace_back(parentPool->New(*action));
    
    groupAction.m_parentPool = parentPool;
    return groupAction;
}

Action Action::Group(std::vector<Action> && actions) {
    Action groupAction {ActionType::GROUP, true};
    groupAction.m_data.emplace<GroupData>();
    
    ActionsList& actionsList = std::get<GroupData>(groupAction.m_data).actionsList;
    actionsList.actions.emplace<UnhandledActions>(std::move(actions));
    actionsList.actionsHandled = false;
    
    return groupAction;
}

Action Action::Group(ActionPool* parentPool, std::vector<Action> && actions) {
    assert(parentPool);
    Action groupAction = Group(std::move(actions));
    groupAction.m_parentPool = parentPool;
    return groupAction;
}

Action Action::Group(ActionPool* parentPool, std::vector<ActionHandler> && actions) {
    assert(parentPool);
    Action groupAction {ActionType::GROUP, true};
    groupAction.m_data.emplace<GroupData>();
    
    ActionsList& actionsList = std::get<GroupData>(groupAction.m_data).actionsList;
    actionsList.actions.emplace<HandledActions>(std::move(actions));
    actionsList.actionsHandled = false;
    
    groupAction.m_parentPool = parentPool;
    return groupAction;
}

Action Action::Hide() {
    Action hideAction {ActionType::HIDE, false};
    hideAction.m_data.emplace<HideData>(HideData{true});
    return hideAction;
}

Action Action::Action::MoveBy(sf::Vector2f const& deltaPos) {
    Action moveAction {ActionType::MOVE, false};
    moveAction.m_data.emplace<MoveData>(MoveData{deltaPos});
    return moveAction;
}

Action Action::MoveBy(float x, float y) {
    Action moveAction {ActionType::MOVE, false};
    moveAction.m_data.emplace<MoveData>(MoveData{sf::Vector2f{x, y}});
    return moveAction;
}

Action Action::MoveByX(float x) {
    Action moveAction {ActionType::MOVE, false};
    moveAction.m_data.emplace<MoveData>(MoveData{sf::Vector2f{x, 0.f}});
    return moveAction;
}

Action Action::MoveByY(float y) {
    Action moveAction {ActionType::MOVE, false};
    moveAction.m_data.emplace<MoveData>(MoveData{sf::Vector2f{0.f, y}});
    return moveAction;
}

Action Action::MoveTo(sf::Vector2f const& position) {
    Action moveAction {ActionType::MOVE, true};
    moveAction.m_data.emplace<MoveData>(MoveData{position});
    return moveAction;
}

Action Action::MoveTo(float x, float y) {
    Action moveAction {ActionType::MOVE, true};
    moveAction.m_data.emplace<MoveData>(MoveData{sf::Vector2f{x, y}});
    return moveAction;
}

Action Action::MoveToX(float x) {
    Action moveAction {ActionType::MOVE, true};
    moveAction.m_data.emplace<MoveData>(MoveData{sf::Vector2f{x, 0.f}});
    return moveAction;
}

Action Action::MoveToY(float y) {
    Action moveAction {ActionType::MOVE, true};
    moveAction.m_data.emplace<MoveData>(MoveData{sf::Vector2f{0.f, y}});
    return moveAction;
}

Action Action::Pause(std::uint64_t duration) {
    Action pauseAction {ActionType::PAUSE, false};
    pauseAction.m_data.emplace<PauseData>();
    pauseAction.setDuration(duration);
    return pauseAction;
}

Action Action::RemoveFromParent() {
    Action removeAction {ActionType::REMOVE_FROM_PARENT, false};
    removeAction.m_data.emplace<RemoveData>();
    return removeAction;
}

Action Action::Repeat(Action && action, std::size_t count, bool callCompletionCallbackEveryRestart) {
    Action repeatAction {ActionType::REPEAT, false};
    
    RepeatData repeatData {};
    repeatData.callCompletionCallbackEveryRestart = callCompletionCallbackEveryRestart;
    repeatData.repeatCount = count;
    repeatData.repeatForever = false;
    repeatData.initialAction = std::make_unique<Action>(std::forward<Action>(action));
    
    repeatAction.m_data.emplace<RepeatData>(std::move(repeatData));
    return repeatAction;
}

Action Action::Repeat(std::unique_ptr<Action> && action, std::size_t count, bool callCompletionCallbackEveryRestart) {
    Action repeatAction {ActionType::REPEAT, false};
    
    RepeatData repeatData {};
    repeatData.callCompletionCallbackEveryRestart = callCompletionCallbackEveryRestart;
    repeatData.repeatCount = count;
    repeatData.repeatForever = false;
    repeatData.initialAction = std::forward<std::unique_ptr<Action>>(action);
    
    repeatAction.m_data.emplace<RepeatData>(std::move(repeatData));
    return repeatAction;
}

Action Action::RepeatForever(Action && action, bool callCompletionCallbackEveryRestart) {
    Action repeatAction {ActionType::REPEAT, false};
    
    RepeatData repeatData {};
    repeatData.callCompletionCallbackEveryRestart = callCompletionCallbackEveryRestart;
    repeatData.repeatForever = true;
    repeatData.initialAction = std::make_unique<Action>(std::forward<Action>(action));
    
    repeatAction.m_data.emplace<RepeatData>(std::move(repeatData));
    return repeatAction;
}

Action Action::RepeatForever(std::unique_ptr<Action> && action, bool callCompletionCallbackEveryRestart) {
    Action repeatAction {ActionType::REPEAT, false};
    
    RepeatData repeatData {};
    repeatData.callCompletionCallbackEveryRestart = callCompletionCallbackEveryRestart;
    repeatData.repeatForever = true;
    repeatData.initialAction = std::forward<std::unique_ptr<Action>>(action);
    
    repeatAction.m_data.emplace<RepeatData>(std::move(repeatData));
    return repeatAction;
}

Action Action::RotateBy(float rotation) {
    Action rotateAction {ActionType::ROTATE, false};
    rotateAction.m_data.emplace<RotateData>(RotateData{rotation});
    return rotateAction;
}

Action Action::RotateTo(float rotation) {
    Action rotateAction {ActionType::ROTATE, true};
    rotateAction.m_data.emplace<RotateData>(RotateData{rotation});
    return rotateAction;
}

Action Action::ScaleBy(sf::Vector2f const& scaleFactor) {
    Action scaleAction {ActionType::SCALE, false};
    scaleAction.m_data.emplace<ScaleData>(ScaleData{scaleFactor});
    return scaleAction;
}

Action Action::ScaleBy(float x, float y) {
    Action scaleAction {ActionType::SCALE, false};
    scaleAction.m_data.emplace<ScaleData>(ScaleData{sf::Vector2f{x, y}});
    return scaleAction;
}

Action Action::ScaleByX(float x) {
    Action scaleAction {ActionType::SCALE, false};
    scaleAction.m_data.emplace<ScaleData>(ScaleData{sf::Vector2f{x, 0.f}});
    return scaleAction;
}

Action Action::ScaleByY(float y) {
    Action scaleAction {ActionType::SCALE, false};
        scaleAction.m_data.emplace<ScaleData>(ScaleData{sf::Vector2f{0.f, y}});
    return scaleAction;
}

Action Action::ScaleTo(sf::Vector2f const& scaleFactor) {
    Action scaleAction {ActionType::SCALE, true};
    scaleAction.m_data.emplace<ScaleData>(ScaleData{scaleFactor});
    return scaleAction;
}

Action Action::ScaleTo(float x, float y) {
    Action scaleAction {ActionType::SCALE, true};
    scaleAction.m_data.emplace<ScaleData>(ScaleData{sf::Vector2f{x, y}});
    return scaleAction;
}

Action Action::ScaleToX(float x) {
    Action scaleAction {ActionType::SCALE, true};
    scaleAction.m_data.emplace<ScaleData>(ScaleData{sf::Vector2f{x, 0.f}});
    return scaleAction;
}

Action Action::ScaleToY(float y) {
    Action scaleAction {ActionType::SCALE, true};
    scaleAction.m_data.emplace<ScaleData>(ScaleData{sf::Vector2f{0.f, y}});
    return scaleAction;
}

Action Action::Sequence(std::vector<Action> const& actions) {
    Action sequenceAction {ActionType::SEQUENCE, true};
    sequenceAction.m_data.emplace<SequenceData>();
    
    ActionsList& actionsList = std::get<SequenceData>(sequenceAction.m_data).actionsList;
    actionsList.actions.emplace<UnhandledActions>();
    actionsList.actionsHandled = false;
    
    for (auto const& action : actions)
        std::get<UnhandledActions>(actionsList.actions).emplace_back(action);
    
    return sequenceAction;
}

Action Action::Sequence(ActionPool* pool, std::vector<Action> const& actions) {
    Action sequenceAction = Sequence(actions);
    sequenceAction.m_parentPool = pool;
    return sequenceAction;
}

Action Action::Sequence(ActionPool* pool, std::vector<ActionHandler> const& actions) {
    Action sequenceAction {ActionType::SEQUENCE, true};
    sequenceAction.m_data.emplace<SequenceData>();
    
    ActionsList& actionsList = std::get<SequenceData>(sequenceAction.m_data).actionsList;
    actionsList.actions.emplace<UnhandledActions>();
    actionsList.actionsHandled = true;
    
    for (auto const& action : actions)
        std::get<HandledActions>(actionsList.actions).emplace_back(pool->New(*action));
    
    sequenceAction.m_parentPool = pool;
    return sequenceAction;
}

Action Action::Sequence(std::vector<Action> && actions) {
    Action sequenceAction {ActionType::SEQUENCE, true};
    sequenceAction.m_data.emplace<SequenceData>();
    
    ActionsList& actionsList = std::get<SequenceData>(sequenceAction.m_data).actionsList;
    actionsList.actions.emplace<UnhandledActions>(std::move(actions));
    actionsList.actionsHandled = false;
    
    return sequenceAction;
}

Action Action::Sequence(ActionPool* pool, std::vector<ActionHandler> && actions) {
    Action sequenceAction {ActionType::SEQUENCE, true};
    sequenceAction.m_data.emplace<SequenceData>();
    
    ActionsList& actionsList = std::get<SequenceData>(sequenceAction.m_data).actionsList;
    actionsList.actions.emplace<HandledActions>(std::move(actions));
    actionsList.actionsHandled = true;
    
    sequenceAction.m_parentPool = pool;
    return sequenceAction;
}

Action Action::SpeedBy(float speedFactor) {
    Action speedAction {ActionType::SPEED, false};
    speedAction.m_data.emplace<SpeedData>(SpeedData{speedFactor});
    return speedAction;
}

Action Action::SpeedTo(float speedFactor) {
    Action speedAction {ActionType::SPEED, true};
    speedAction.m_data.emplace<SpeedData>(SpeedData{speedFactor});
    return speedAction;
}

Action Action::SwitchHidden() {
    Action hideAction {ActionType::HIDE, true};
    hideAction.m_data.emplace<HideData>();
    return hideAction;
}

Action Action::Unhide() {
    Action unhideAction {ActionType::HIDE, false};
    unhideAction.m_data.emplace<HideData>(HideData{false});
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
    switch (m_type) {
        case ActionType::MOVE :
            m_data.emplace<MoveData>();
            break;
        case ActionType::ROTATE :
            m_data.emplace<RotateData>();
            break;
        case ActionType::SCALE :
            m_data.emplace<ScaleData>();
            break;
        case ActionType::FOLLOW_PATH :
            m_data.emplace<PathData>();
            break;
        case ActionType::HIDE :
            m_data.emplace<HideData>();
            break;
        case ActionType::REMOVE_FROM_PARENT :
            m_data.emplace<RemoveData>();
            break;
        case ActionType::SEQUENCE :
            m_data.emplace<SequenceData>();
            break;
        case ActionType::GROUP :
            m_data.emplace<GroupData>();
            break;
        case ActionType::REPEAT :
            m_data.emplace<RepeatData>();
            break;
        case ActionType::SPEED :
            m_data.emplace<SpeedData>();
            break;
        case ActionType::PAUSE :
            m_data.emplace<PauseData>();
            break;
        case ActionType::EMPTY :
            m_data.emplace<EmptyData>();
            break;
    }
}

void Action::callSubActionsCallbacks() {
    if (m_type == ActionType::SEQUENCE) {
        SequenceData& sequenceData = std::get<SequenceData>(m_data);
        if (sequenceData.actionsList.actionsHandled) {
            HandledActions& actions = std::get<HandledActions>(sequenceData.actionsList.actions);
            for (std::size_t actionId = sequenceData.playedActionId; actionId < actions.size(); ++actionId)
                actions[actionId]->callCompletionCallbacks();
        } else {
            UnhandledActions& actions = std::get<UnhandledActions>(sequenceData.actionsList.actions);
            for (std::size_t actionId = sequenceData.playedActionId; actionId < actions.size(); ++actionId)
                actions[actionId].callCompletionCallbacks();
        }
    } else if (m_type == ActionType::GROUP) {
        ActionsList& actionsList = std::get<GroupData>(m_data).actionsList;
        if (actionsList.actionsHandled) {
            for (auto& action : std::get<HandledActions>(actionsList.actions)) {
                if (!action->isCompleted())
                    action->callCompletionCallbacks();
            }
        } else {
            for (auto& action : std::get<UnhandledActions>(actionsList.actions)) {
                if (!action.isCompleted())
                    action.callCompletionCallbacks();
            }
        }
    } else if (m_type == ActionType::REPEAT) {
        RepeatData& repeatData = std::get<RepeatData>(m_data);
        if (!repeatData.currentAction->isCompleted())
            repeatData.currentAction->callCompletionCallbacks();
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
    PathData& pathData = std::get<PathData>(m_data);
    for (auto const& nextPosition : pathData.positions) {
        distance += getVectorLength(nextPosition - previousPosition);
        previousPosition = nextPosition;
    }
    pathData.distance = distance;
    pathData.targetPositionId = std::size_t(0);
}

void Action::completeInitGroup() {
    GroupData& groupData = std::get<GroupData>(m_data);
    
    completeInitOfActions(groupData.actionsList);
    m_duration = 0;
    
    if (groupData.actionsList.actionsHandled) {
        for (auto& action : std::get<HandledActions>(groupData.actionsList.actions))
            m_duration = std::max(static_cast<std::uint64_t>(std::ceil(action->m_duration / action->m_speed)), m_duration);
    } else {
        for (auto& action : std::get<UnhandledActions>(groupData.actionsList.actions))
            m_duration = std::max(static_cast<std::uint64_t>(std::ceil(action.m_duration / action.m_speed)), m_duration);
    }
}

void Action::completeInitHide() {
    if (m_isRelativeToInitialState)
        std::get<HideData>(m_data).hidden = !m_owner->isHidden();
}

void Action::completeInitMove() {
    MoveData& moveData = std::get<MoveData>(m_data);
    if (m_isRelativeToInitialState)
        moveData.position = getOwnerCurrentPosition() - moveData.position;
}

void Action::completeInitOfActions(ActionsList& actionsList) {
    if (actionsList.actionsHandled) {
        for (auto& action : std::get<HandledActions>(actionsList.actions))
            action->completeInit(m_owner);
    } else {
        for (auto& action : std::get<UnhandledActions>(actionsList.actions))
            action.completeInit(m_owner);
    }
}

void Action::completeInitRepeat() {
    RepeatData& repeatData = std::get<RepeatData>(m_data);
    
    repeatData.currentAction = std::make_unique<Action>(*repeatData.initialAction);
    repeatData.currentAction->m_completionCallback = std::move(repeatData.initialAction->m_completionCallback);
    repeatData.currentAction->completeInit(m_owner);
    
    if (repeatData.repeatForever) {
        m_timingMode = TimingMode::LINEAR;
        m_duration = std::numeric_limits<decltype(m_duration)>::max();
        repeatData.repeatCount = 0;
    } else {
        m_duration = repeatData.currentAction->m_duration * repeatData.repeatCount;
    }
    
    assert(!(repeatData.callCompletionCallbackEveryRestart && !m_completionCallback));
}

void Action::completeInitRotate() {
    RotateData& rotateData = std::get<RotateData>(m_data);
    if (m_isRelativeToParent)
        rotateData.rotation = getOwnerCurrentTransform().getRotation() - rotateData.rotation;
}

void Action::completeInitScale() {
    ScaleData& scaleData = std::get<ScaleData>(m_data);
    if (m_isRelativeToParent)
        scaleData.scaleFactor = getOwnerCurrentTransform().getScale() - scaleData.scaleFactor;
}

void Action::completeInitSequence() {
    SequenceData& sequenceData = std::get<SequenceData>(m_data);
    
    completeInitOfActions(sequenceData.actionsList);
    sequenceData.playedActionId = 0;
    m_duration = 0.f;
    
    if (sequenceData.actionsList.actionsHandled) {
        for (auto const& action : std::get<HandledActions>(sequenceData.actionsList.actions))
            m_duration += static_cast<std::uint64_t>(std::ceil(action->m_duration / action->m_speed));
    } else {
        for (auto const& action : std::get<UnhandledActions>(sequenceData.actionsList.actions))
            m_duration += static_cast<std::uint64_t>(std::ceil(action.m_duration / action.m_speed));
    }
}

void Action::completeInitSpeed() {
    if (m_isRelativeToInitialState)
        std::get<SpeedData>(m_data).speed -= m_owner->getSpeed();
}

Action::ActionsList Action::getActionsReversed(ActionsList const& actionsList, Node* node) const {
    ActionsList reversedActionsList {};
    
    if (actionsList.actionsHandled) {
        HandledActions const& actions = std::get<HandledActions>(actionsList.actions);
        HandledActions reversedActions {};
        for (auto actionIt = std::rbegin(actions); actionIt != std::rend(actions); ++actionIt)
            reversedActions.emplace_back(m_parentPool->New((*actionIt)->getReversed(node)));
    } else {
        UnhandledActions const& actions = std::get<UnhandledActions>(actionsList.actions);
        UnhandledActions reversedActions {};
        for (auto actionIt = std::rbegin(actions); actionIt != std::rend(actions); ++actionIt)
            reversedActions.emplace_back((*actionIt).getReversed(node));
    }

    return reversedActionsList;
}

Action Action::getDataUnrelativeToNodeReversed() const {
    switch (m_type) {
        case ActionType::MOVE :
            return Action::MoveBy(-std::get<MoveData>(m_data).position);
        case ActionType::ROTATE :
            return Action::RotateBy(-std::get<RotateData>(m_data).rotation);
        case ActionType::SCALE :
        {
            ScaleData const& scaleData = std::get<ScaleData>(m_data);
            assert(scaleData.scaleFactor.x > 0.f && scaleData.scaleFactor.y > 0.f);
            return Action::ScaleBy(1.f / scaleData.scaleFactor.x, 1.f / scaleData.scaleFactor.y);
        }
        case ActionType::FOLLOW_PATH : break;
        case ActionType::HIDE :
            return (std::get<HideData>(m_data).hidden ? Action::Unhide() : Action::Hide());
        case ActionType::REMOVE_FROM_PARENT : break;
        case ActionType::SEQUENCE : break;
        case ActionType::GROUP : break;
        case ActionType::REPEAT :
        {
            RepeatData const& repeatData = std::get<RepeatData>(m_data);
            return (repeatData.repeatForever ? Action::RepeatForever(repeatData.initialAction->getReversed()) : Action::Repeat(repeatData.initialAction->getReversed(), repeatData.repeatCount));
        }
        case ActionType::SPEED :
            assert(std::get<SpeedData>(m_data).speed > 0.f);
            return Action::SpeedBy(-std::get<SpeedData>(m_data).speed);
        case ActionType::PAUSE :
            return Action::Pause(m_duration);
        case ActionType::EMPTY : break;
    }
    return Action::Empty();
}

Action Action::getDataRelativeToNodeReversed(Node* node) const {
    auto& nodeTransform = (m_isRelativeToParent ? node->getRelativeTransform() : node->getGlobalTransform());
    switch (m_type) {
        case ActionType::MOVE :
            return Action::MoveBy(nodeTransform.getPosition() - std::get<MoveData>(m_data).position);
        case ActionType::ROTATE :
            return Action::RotateBy(nodeTransform.getRotation() - std::get<RotateData>(m_data).rotation);
        case ActionType::SCALE :
            return Action::ScaleBy(nodeTransform.getScale() - std::get<ScaleData>(m_data).scaleFactor);
        case ActionType::FOLLOW_PATH :
            return Action::FollowPath(getPathReversed(std::get<PathData>(m_data).positions, node));
        case ActionType::HIDE :
            return (node->isHidden() ? Action::Unhide() : Action::Hide());
        case ActionType::REMOVE_FROM_PARENT : break;
        case ActionType::SEQUENCE :
        {
            ActionsList list = getActionsReversed(std::get<SequenceData>(m_data).actionsList, node);
            if (list.actionsHandled)
                return Action::Sequence(m_parentPool, std::move(std::get<HandledActions>(list.actions)));
            else
                return Action::Sequence(std::move(std::get<UnhandledActions>(list.actions)));
        }
        case ActionType::GROUP :
        {
            ActionsList list = getActionsReversed(std::get<SequenceData>(m_data).actionsList, node);
            if (list.actionsHandled)
                return Action::Group(m_parentPool, std::move(std::get<HandledActions>(list.actions)));
            else
                return Action::Group(std::move(std::get<UnhandledActions>(list.actions)));
        }
        case ActionType::REPEAT : break;
        case ActionType::SPEED :
            return Action::SpeedBy(node->getSpeed() - std::get<SpeedData>(m_data).speed);
        case ActionType::PAUSE : break;
        case ActionType::EMPTY : break;
    }
    return Action::Empty();
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
        case TimingMode::EASE_IN :
            return bezierCurveDegree1At(ratio, s_easeInControlValues);
        case TimingMode::EASE_OUT:
            return bezierCurveDegree1At(ratio, s_easeOutControlValues);
        case TimingMode::EASE_IN_OUT :
            return bezierCurveDegree1At(ratio, s_easeInEaseOutControlValues);
    }
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
    PathData& pathData = std::get<PathData>(m_data);
    float movingDistanceLeft = progress * pathData.distance;
    
    while (pathData.targetPositionId < pathData.positions.size()) {
        auto const& currentPosition = getOwnerCurrentPosition();
        const sf::Vector2f deltaPos = pathData.positions[pathData.targetPositionId] - currentPosition;
        const float distanceToTarget = getVectorLength(deltaPos);
        
        if (movingDistanceLeft > distanceToTarget) {
            m_owner->move(deltaPos);
            movingDistanceLeft -= distanceToTarget;
            ++pathData.targetPositionId;
        } else {
            m_owner->move(deltaPos * (movingDistanceLeft / distanceToTarget));
            break;
        }
    }
}

void Action::updateGroup(float progress) {
    std::uint64_t timeElapsed = static_cast<std::uint64_t>(progress * m_duration);
    ActionsList& actionsList = std::get<GroupData>(m_data).actionsList;
    
    if (actionsList.actionsHandled) {
        for (auto& action : std::get<HandledActions>(actionsList.actions)) {
            if (!action->isCompleted())
                action->update(timeElapsed);
        }
    } else {
        for (auto& action : std::get<UnhandledActions>(actionsList.actions)) {
            if (!action.isCompleted())
                action.update(timeElapsed);
        }
    }
}

void Action::updateHide() {
    m_owner->setHidden(std::get<HideData>(m_data).hidden);
}

void Action::updateMove(float progress) {
    m_owner->move(progress * std::get<MoveData>(m_data).position);
}

void Action::updateRemove() {
    m_owner->removeFromParent();
}

void Action::updateRepeat(std::uint64_t timeElapsed, float progress) {
    RepeatData& repeatData = std::get<RepeatData>(m_data);
    std::uint64_t timeUsed = (repeatData.repeatForever ? timeElapsed : static_cast<std::uint64_t>(std::round(progress * m_duration))); // distinction is made because if action repeats forever the duration is (2^64 - 1) which leads to floating-point errors as (timeElapsed << duration -> progress << 1)
    std::uint64_t timeLeft = repeatData.currentAction->update(timeUsed);
    
    if (timeLeft == 0)
        return;
    
    if (!repeatData.repeatForever && --repeatData.repeatCount == 0)
        return;
    
    if (repeatData.callCompletionCallbackEveryRestart)
        m_completionCallback.value()();
    
    auto callback = std::move(repeatData.currentAction->m_completionCallback);
    repeatData.currentAction = std::make_unique<Action>(*repeatData.initialAction);
    repeatData.currentAction->m_completionCallback = std::move(callback);
    repeatData.currentAction->completeInit(m_owner);
    
    update(timeElapsed - timeUsed);
}

void Action::updateRotate(float progress) {
    float rotation = progress * std::get<RotateData>(m_data).rotation;
    m_owner->rotate(rotation);
}

void Action::updateScale(float progress) {
    auto scale =  sf::Vector2f(1.f, 1.f) - progress * std::get<ScaleData>(m_data).scaleFactor;
    m_owner->scale(scale);
}

void Action::updateSequence(float progress) {
    std::uint64_t timeLeft = static_cast<std::uint64_t>(progress * m_duration);
    SequenceData& sequenceData = std::get<SequenceData>(m_data);
    
    if (sequenceData.actionsList.actionsHandled) {
        HandledActions& actions = std::get<HandledActions>(sequenceData.actionsList.actions);
        while (sequenceData.playedActionId < actions.size()) {
            auto& action = actions[sequenceData.playedActionId];
            timeLeft = action->update(timeLeft);
            if (action->isCompleted())
                ++sequenceData.playedActionId;
            else
                break;
        }
    } else {
        UnhandledActions& actions = std::get<UnhandledActions>(sequenceData.actionsList.actions);
        while (sequenceData.playedActionId < actions.size()) {
            auto& action = actions[sequenceData.playedActionId];
            timeLeft = action.update(timeLeft);
            if (action.isCompleted())
                ++sequenceData.playedActionId;
            else
                break;
        }
    }
}

void Action::updateSpeed(float progress) {
    m_owner->setSpeed(m_owner->getSpeed() + std::get<SpeedData>(m_data).speed * progress);
}

}
