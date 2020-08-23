//
//  Action.hpp
//  Maze
//
//  Created by Jaraxus on 17/08/2020.
//

#ifndef Action_hpp
#define Action_hpp

#include <cstring>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Node.hpp"
#include "../Core/Logs.hpp"
#include "../System/Callback.hpp"
#include "../System/MathsExt.hpp"

namespace mz {

class Node;

class Action { // TODO: maybe use a poolObject
    friend class Node;
    
    enum ActionType : std::uint16_t { // TODO: Add repeat
        MOVE,
        ROTATE,
        SCALE,
        FOLLOW_PATH,
        REMOVE_FROM_PARENT,
        SEQUENCE,
        GROUP,
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
            std::size_t                 currentPosition;
            float                       distance;
            std::vector<sf::Vector2f>   positions;
        } pathData;
        
        struct {} removeData;
        
        struct {
            std::vector<Action> actions;
        } sequenceData;
        
        struct {
            std::vector<Action> actions;
        } groupData;
        
        struct {
            float speed;
        } speedData;

        struct {} pauseData;
        
        struct {} emptyData;
        
        DataType();
        
        ~DataType();
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
    
    Action operator=(Action const& action) noexcept;
    Action operator=(Action && action) noexcept;
    
    ~Action();
    
    inline float getCurrentProgress();
    
    Action getReversed(Node* node = nullptr) const;
    
    inline bool isCompleted() const;
    
    inline void setDuration(std::uint64_t duration);
    
    inline void setRelativeToParent(bool isRelativeToParent = true);
    
    inline void setSpeed(float speed = 1.f);
    
    inline void setTimingMode(TimingMode timingMode);
    
    static Action Empty();
    
    static Action FollowPath(std::vector<sf::Vector2f> const& path);
    
    static Action FollowPath(std::vector<sf::Vector2f> && path);
    
    static Action Group(std::vector<Action> const& actions);
    
    static Action Group(std::vector<Action> && actions);
    
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
    
    static Action SpeedBy(float speedFactor);
    
    static Action SpeedTo(float speedFactor);
    
private:
    Action(ActionType type, bool m_isRelativeToInitialState);
    
    void completeInit(Node* owner);
    
    void completeInitFollowPath();
    
    void completeInitGroup();
    
    void completeInitMove();
    
    void completeInitOfActions(std::vector<Action>& actions);
    
    void completeInitRotate();
    
    void completeInitScale();
    
    void completeInitSequence();
    
    void completeInitSpeed();
    
    Action getAbsoluteReversed() const;
    
    std::vector<Action> getActionsReversed(std::vector<Action> const& actions, Node* node) const;
    
    inline sf::Vector2f const& getOwnerCurrentPosition() const;
    
    sf::Transformable const& getOwnerCurrentTransform() const;
    
    std::vector<sf::Vector2f> getPathReversed(std::vector<sf::Vector2f> const& path, Node* node) const;
    
    float getProgress(std::uint64_t timeElapsed, std::uint64_t duration);
    
    Action getRelativeReversed(Node* node) const;
    
    void inline setCallback(CompletionCallback && callback);
    
    std::uint64_t update(std::uint64_t timeElapsed);
    
    void updateFollowPath(float progress);
    
    void updateGroup(float progress);
    
    void updateMove(float progress);
    
    void updateRemove();
    
    void updateRotate(float progress);
    
    void updateScale(float progress);
    
    void updateSequence(float progress);
    
    void updateSpeed(float progress);
    
private:
    std::optional<CompletionCallback>   m_completionCallback;
    DataType                            m_data;
    std::uint64_t                       m_duration;
    bool                                m_isRelativeToInitialState;
    bool                                m_isRelativeToParent;
    Node*                               m_owner;
    float                               m_speed;
    std::uint64_t                       m_timeElapsed;
    TimingMode                          m_timingMode;
    const ActionType                    m_type;
};

}

#include "Action.inl"

#endif /* Action_hpp */
