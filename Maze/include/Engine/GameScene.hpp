//
//  GameScene.hpp
//  Maze
//
//  Created by Jaraxus on 13/07/2020.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <SFML/Graphics.hpp>

#include "Camera.hpp"
#include "Layer.hpp"
#include "Node.hpp"
#include "Updatable.hpp"
#include "TraitsExt.hpp"

#include "../Physics/PhysicsWorld.hpp"
#include "../System/ObjectPool.hpp"
#include "../Config.hpp"

namespace mz {

class GameScene : Updatable {
    template <typename F, typename R, typename ...Args>
    using enable_if_building = typename
        std::enable_if<
            std::is_invocable<F, Args...>::value &&
            std::is_same_v<typename mz::function_trait<F>::return_type, R>
        , mz::ActionHandler>::type;
    
public:
    GameScene(unsigned int width, unsigned int height);
    GameScene(GameScene const& scene) = delete;
    GameScene(GameScene && scene) = delete;
    
    GameScene operator=(GameScene const& scene) = delete;
    GameScene operator=(GameScene && scene) = delete;
    
    void display(sf::RenderTarget& texture);
    
    inline ActionHandler getActionHandler(Action const& action);
    
    inline ActionHandler getActionHandler(Action && action);
    
    template <typename F, typename ...Args>
    enable_if_building<F, Action, Args...> buildActionHandler(F const& constructionFunc, Args && ...args);
    
    template <typename F, typename ...Args>
    enable_if_building<F, Action, ActionPool*, Args...> buildActionHandler(F const& constructionFunc, Args && ...args);
    
//    template <typename F, typename ...Args>
//    ActionHandler getActionHandler(std::function<Action(ActionPool*, Args...)>& constructionFunc, Args && ...args);
    
    Layer& getMainLayer();
    
    virtual void update(std::uint64_t timeElapsed) override;
    
private:
    ActionPool      m_actionPool;
    Camera          m_camera; // TODO: return reference to handle it outside
    Layer           m_mainLayer;
    PhysicsWorld    m_physicsWorld;
};

}

#include "GameScene.inl"

#endif /* GameScene_hpp */
