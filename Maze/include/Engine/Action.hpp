//
//  Action.hpp
//  Maze
//
//  Created by Jaraxus on 17/08/2020.
//

#ifndef Action_hpp
#define Action_hpp

#include <algorithm>
#include <cmath>
#include <cstring>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Node.hpp"
#include "../Core/Logs.hpp"
#include "../System/Callback.hpp"
#include "../System/MathsExt.hpp"

namespace mz {
class Action;
} // namespace mz

namespace std {
void swap(mz::Action& lhs, mz::Action& rhs) noexcept;
} // namespace std

namespace mz {

class Node;

class Action { // TODO: maybe use a poolObject
    friend class Node;
    
    friend void std::swap(Action& lhs, Action& rhs) noexcept; // TODO: update for C++20
    
    enum ActionType : std::uint16_t { // TODO: Add repeat
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
    
    union DataType {
        struct {
            sf::Vector2f position;
        } moveData;

        struct {
            float rotation;
        } rotateData;

        struct {
            sf::Vector2f scaleFactor;
        } scaleData;

        struct {
            float                       distance;
            std::vector<sf::Vector2f>   positions;
            std::size_t                 targetPositionId;
        } pathData;
        
        struct {
            bool hidden;
        } hideData;
        
        struct {} removeData;
        
        struct {
            std::vector<Action> actions;
            std::size_t         playedActionId;
        } sequenceData;
        
        struct {
            std::vector<Action> actions;
        } groupData;
        
        struct {
            bool                    callCompletionCallbackEveryRestart;
            std::unique_ptr<Action> currentAction;
            std::unique_ptr<Action> initialAction;
            std::size_t             repeatCount;
            bool                    repeatForever;
        } repeatData;
        
        struct {
            float speed;
        } speedData;

        struct {} pauseData;
        
        struct {} emptyData;
        
        DataType();
        
        ~DataType();
        
        static void exchange(mz::Action::ActionType inputType, mz::Action::DataType& inputData, mz::Action::DataType& outputData);
    };
    
public:
    using CompletionCallback = Callback<void()>;
    
    enum TimingMode {
        LINEAR,
        EASE_IN,
        EASE_OUT,
        EASE_IN_OUT
    };
    
public:
    Action(Action const& action) noexcept;
    Action(Action && action) noexcept;
    
    Action& operator=(Action const& action) = delete;
    Action& operator=(Action && action) = delete;
    
    ~Action();
    
    inline float getCurrentProgress();
    
    Action getReversed(Node* node = nullptr) const;
    
    inline bool isCompleted() const;
    
    inline bool isPaused() const;
    
    void inline setCallback(CompletionCallback && callback);
    
    inline void setDuration(std::uint64_t duration);
    
    inline void setPaused(bool isPaused);
    
    inline void setRelativeToParent(bool isRelativeToParent = true);
    
    inline void setSpeed(float speed);
    
    inline void setTimingMode(TimingMode timingMode);
    
    static Action Empty();
    
    static Action FollowPath(std::vector<sf::Vector2f> const& path);
    
    static Action FollowPath(std::vector<sf::Vector2f> && path);
    
    template <typename ...Vertexes> // TODO: use C++20 concepts
    static Action FollowPath(Vertexes && ...vertexes);
    
    static Action Group(std::vector<Action> const& actions);
    
    static Action Group(std::vector<Action> && actions);
    
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
    
    static Action RepeatForever(Action && action, bool callCompletionCallbackEveryRestart = false);
    
    static Action RepeatForever(std::unique_ptr<Action> && action, bool callCompletionCallbackEveryRestart = false);
    
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
    
    static Action Sequence(std::vector<Action> && actions);
    
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
    
    void completeInitOfActions(std::vector<Action>& actions);
    
    void completeInitRepeat();
    
    void completeInitRotate();
    
    void completeInitScale();
    
    void completeInitSequence();
    
    void completeInitSpeed();
    
    Action getDataUnrelativeToNodeReversed() const;
    
    std::vector<Action> getActionsReversed(std::vector<Action> const& actions, Node* node) const;
    
    inline sf::Vector2f const& getOwnerCurrentPosition() const;
    
    sf::Transformable const& getOwnerCurrentTransform() const;
    
    std::vector<sf::Vector2f> getPathReversed(std::vector<sf::Vector2f> const& path, Node* node) const;
    
    float getProgress(std::uint64_t timeElapsed, std::uint64_t duration);
    
    Action getDataRelativeToNodeReversed(Node* node) const;
    
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
    float                               m_speed                     = 1.f;
    std::uint64_t                       m_timeElapsed;
    TimingMode                          m_timingMode                = TimingMode::LINEAR;
    ActionType                          m_type;
};

} // namespace mz

#include "Action.inl"

#endif /* Action_hpp */
