//
//  Action.hpp
//  Maze
//
//  Created by Jaraxus on 17/08/2020.
//

#ifndef Action_hpp
#define Action_hpp

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <functional>
#include <utility>
#include <variant>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Node.hpp"
#include "../Core/Logs.hpp"
#include "../System/Callback.hpp"
#include "../System/MathsExt.hpp"
#include "../System/ObjectPool.hpp"

namespace mz {
class Action;
} // namespace mz

namespace std {
void swap(mz::Action& lhs, mz::Action& rhs) noexcept;
} // namespace std

namespace mz {

using ActionPool    = ObjectPool<Action>;
using ActionHandler = ObjectHandler<Action>;

class Node;

class Action {
    friend class Node;
    friend void std::swap(Action& lhs, Action& rhs) noexcept; // TODO: update for C++20
    
    enum ActionType : std::uint16_t {
        MOVE,
        ROTATE,
        SCALE,
        FOLLOW_PATH,
        HIDE,
        REMOVE_FROM_PARENT,
        SEQUENCE,
        GROUP,
        REPEAT,
        SPEED,
        PAUSE,
        EMPTY
    };
    
    using ActionPtr         = std::unique_ptr<Action>;
    using UnhandledActions  = std::vector<Action>;
    using HandledActions    = std::vector<ActionHandler>;
    
    struct ActionDataMember {
        std::variant<
            ActionPtr,
            ActionHandler
        >       action;
        bool    actionHandled;
    };
    
    struct ActionsList {
        std::variant<
            UnhandledActions,
            HandledActions
        >       actions;
        bool    actionsHandled;
    };
    
    struct MoveData {
        sf::Vector2f position;
    };

    struct RotateData {
        float rotation;
    };

    struct ScaleData {
        sf::Vector2f scaleFactor;
    };

    struct PathData {
        float                       distance;
        std::vector<sf::Vector2f>   positions;
        std::size_t                 targetPositionId;
    };
    
    struct HideData {
        bool hidden;
    };
    
    struct RemoveData {};
    
    struct SequenceData {
        ActionsList actionsList;
        std::size_t playedActionId;
    };
    
    struct GroupData {
        ActionsList actionsList;
    };
    
    struct RepeatData {
        bool                callCompletionCallbackEveryRestart;
//        ActionDataMember    currentAction;
//        ActionDataMember    initialAction;
        std::unique_ptr<Action>    currentAction;
        std::unique_ptr<Action>    initialAction;
        std::size_t         repeatCount;
        bool                repeatForever;
    };
    
    struct SpeedData {
        float speed;
    };

    struct PauseData {};
    
    struct EmptyData {};
    
    using DataType = std::variant<
        MoveData,
        RotateData,
        ScaleData,
        PathData,
        HideData,
        RemoveData,
        SequenceData,
        GroupData,
        RepeatData,
        SpeedData,
        PauseData,
        EmptyData
    >;
    
public:
    using CompletionCallback = Callback<void()>;
    
    enum TimingMode {
        LINEAR,
        EASE_IN,
        EASE_OUT,
        EASE_IN_OUT
    };
    
public:
    Action() = delete; // TODO: Fix after ObjectPool tests are complete
    
    Action(Action const& action) noexcept;
    Action(Action && action) noexcept;
    
    Action& operator=(Action const& action) = delete;
    Action& operator=(Action && action) = delete;
    
    inline float getCurrentProgress();
    
    Action getReversed(Node* node = nullptr) const;
    
    inline bool isCompleted() const;
    
    inline bool isPaused() const;
    
    inline Action& setCallback(CompletionCallback && callback);
    
    inline Action& setDuration(std::uint64_t duration);
    
    inline Action& setDuration(sf::Time duration);
    
    template <typename DurationT, typename _ = typename std::enable_if<mz::is_duration<DurationT>::value>::type>
    inline Action& setDuration(DurationT duration);
    
    inline Action& setPaused(bool isPaused);
    
    inline Action& setRelativeToParent(bool isRelativeToParent = true);
    
    inline Action& setSpeed(float speed);
    
    inline Action& setTimingMode(TimingMode timingMode);
    
    static Action Empty();
    
    static Action FollowPath(std::vector<sf::Vector2f> const& path);
    
    static Action FollowPath(std::vector<sf::Vector2f> && path);
    
    template <typename ...Vertexes> // TODO: use C++20 concepts
    static Action FollowPath(Vertexes && ...vertexes);
    
    static Action Group(std::vector<Action> const& actions);
    
    static Action Group(ActionPool* parentPool, std::vector<Action> const& actions);
    
    static Action Group(ActionPool* parentPool, std::vector<ActionHandler> const& actions);
    
    static Action Group(std::vector<Action> && actions);
    
    static Action Group(ActionPool* parentPool, std::vector<Action> && actions);
    
    static Action Group(ActionPool* parentPool, std::vector<ActionHandler> && actions);
    
    template <typename... Actions> // TODO: use C++20 concepts
    static Action Group(Actions && ...actions);
    
    static Action Hide();
    
    static Action MoveBy(sf::Vector2f const& deltaPos);
    
    static Action MoveBy(float x, float y);
    
    static Action MoveByX(float x);
    
    static Action MoveByY(float y);
    
    static Action MoveTo(sf::Vector2f const& position);
    
    static Action MoveTo(float x, float y);
    
    static Action MoveToX(float x);
    
    static Action MoveToY(float y);
    
    static Action Pause(std::uint64_t duration);
    
    static Action RemoveFromParent();
    
    static Action Repeat(Action && action, std::size_t count, bool callCompletionCallbackEveryRestart = false);
    
    static Action Repeat(std::unique_ptr<Action> && action, std::size_t count, bool callCompletionCallbackEveryRestart = false);
    
    static Action Repeat(ActionPool& pool, Action && action, std::size_t count, bool callCompletionCallbackEveryRestart = false);
    
    static Action Repeat(ActionPool& pool, ActionHandler && action, std::size_t count, bool callCompletionCallbackEveryRestart = false);
    
    static Action RepeatForever(Action && action, bool callCompletionCallbackEveryRestart = false);
    
    static Action RepeatForever(std::unique_ptr<Action> && action, bool callCompletionCallbackEveryRestart = false);
    
    static Action RepeatForever(ActionPool& pool, Action && action, bool callCompletionCallbackEveryRestart = false);
    
    static Action RepeatForever(ActionPool& pool, ActionHandler && action, bool callCompletionCallbackEveryRestart = false);
    
    static Action RotateBy(float rotation);
    
    static Action RotateTo(float rotation);
    
    static Action ScaleBy(sf::Vector2f const& scaleFactor);
    
    static Action ScaleBy(float x, float y);
    
    static Action ScaleByX(float x);
    
    static Action ScaleByY(float y);
    
    static Action ScaleTo(sf::Vector2f const& scaleFactor);
    
    static Action ScaleTo(float x, float y);
    
    static Action ScaleToX(float x);
    
    static Action ScaleToY(float y);
    
    static Action Sequence(std::vector<Action> const& actions);
    
    static Action Sequence(ActionPool* pool, std::vector<Action> const& actions);
    
    static Action Sequence(ActionPool* pool, std::vector<ActionHandler> const& actions);
    
    static Action Sequence(std::vector<Action> && actions);
    
    static Action Sequence(ActionPool* pool, std::vector<ActionHandler> && actions);
    
    template <typename ...Actions> // TODO: use C++20 concepts
    static Action Sequence(Actions && ...actions);
    
    static Action SpeedBy(float speedFactor);
    
    static Action SpeedTo(float speedFactor);
    
    static Action SwitchHidden();
    
    static Action Unhide();
    
private:
    Action(ActionType type, bool m_isRelativeToInitialState);
    
    inline void callCompletionCallbacks();
    
    void callSubActionsCallbacks();
    
    void completeInit(Node* owner);
    
    void completeInitFollowPath();
    
    void completeInitGroup();
    
    void completeInitHide();
    
    void completeInitMove();
    
    void completeInitOfActions(ActionsList& actionsList);
    
    void completeInitRepeat();
    
    void completeInitRotate();
    
    void completeInitScale();
    
    void completeInitSequence();
    
    void completeInitSpeed();
    
    ActionsList getActionsReversed(ActionsList const& actionsList, Node* node) const;
    
    Action getDataUnrelativeToNodeReversed() const;
    
    Action getDataRelativeToNodeReversed(Node* node) const;
    
    inline sf::Vector2f const& getOwnerCurrentPosition() const;
    
    sf::Transformable const& getOwnerCurrentTransform() const;
    
    std::vector<sf::Vector2f> getPathReversed(std::vector<sf::Vector2f> const& path, Node* node) const;
    
    float getProgress(std::uint64_t timeElapsed, std::uint64_t duration);
    
    Action getReversedData(Node* node) const;
    
    std::uint64_t getTimeUsed(std::uint64_t timeElapsed);
    
    inline void resetProperties();
    
    std::uint64_t update(std::uint64_t timeElapsed);
    
    void updateFollowPath(float progress);
    
    void updateGroup(float progress);
    
    void updateHide();
    
    void updateMove(float progress);
    
    void updateRemove();
    
    void updateRepeat(std::uint64_t timeElapsed, float progress);
    
    void updateRotate(float progress);
    
    void updateScale(float progress);
    
    void updateSequence(float progress);
    
    void updateSpeed(float progress);
    
private:
    std::optional<CompletionCallback>   m_completionCallback;
    DataType                            m_data;
    std::uint64_t                       m_duration                  = 0;
    bool                                m_isPaused                  = false;
    bool                                m_isRelativeToInitialState;
    bool                                m_isRelativeToParent;
    Node*                               m_owner;
    ActionPool*                         m_parentPool;
    float                               m_speed                     = 1.f;
    std::uint64_t                       m_timeElapsed;
    TimingMode                          m_timingMode                = TimingMode::LINEAR;
    ActionType                          m_type;
    
    static constexpr std::array<float, 4> s_easeInControlValues {0.f, 0.42f, 1.f, 1.f};
    static constexpr std::array<float, 4> s_easeInEaseOutControlValues {0.f, 0.f, 1.f, 1.f};
    static constexpr std::array<float, 4> s_easeOutControlValues {0.f, 0.f, 0.58f, 1.f};
};

} // namespace mz

#include "Action.inl"

#endif /* Action_hpp */
