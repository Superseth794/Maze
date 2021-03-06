//
//  Node.hpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#pragma once

#ifndef Node_hpp
#define Node_hpp

#include <algorithm>
#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Action.hpp"
#include "Layer.hpp"
#include "../System/Callback.hpp"

namespace mz {

class Action;

class Node : public Layer {
public:
    using ActionCompletionCallback = Callback<void()>;
    
public:
    Node() = default;
    Node(Node const& node) = delete;
    Node(Node && node) noexcept = default;
    
    Node& operator=(Node const& node) = delete;
    Node& operator=(Node && node) noexcept = default;
    
    virtual ~Node() = default;
    
    virtual void draw(Camera const& camera) override;
    
    inline std::size_t getActionsCount() const;
    
    inline float getSpeed() const;
    
    inline bool isPaused() const;
    
    bool removeAction(std::string const& actionIdentifier);
    
    inline std::size_t removeAllActions();
    
    void run(Action && action);
    
    void run(Action && action, std::string && name);
    
    void run(Action && action, ActionCompletionCallback && callback);
    
    void run(Action && action, std::string && name, ActionCompletionCallback && callback);
    
    bool setActionPaused(std::string const& actionIdentifier, bool isPaused);
    
    inline void setPaused(bool isPaused);
    
    inline Node& setSpeed(float speed);
    
    virtual void update(std::uint64_t timeElapsed) override;
    
private:
    std::vector<Action>                     m_actions;
    bool                                    m_isPaused      = false;
    std::unordered_map<std::string, Action> m_namedActions;
    float                                   m_speed         = 1.f;
};

}

#include "Node.inl"

#endif /* Node_hpp */
