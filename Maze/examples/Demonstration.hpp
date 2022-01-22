//
//  Demonstration.hpp
//  Maze
//
//  Created by Jaraxus on 13/09/2021.
//

#ifndef Demonstration_hpp
#define Demonstration_hpp

#include <string>
#include <vector>

#include "../include/Engine/Application.hpp"
#include "../include/Engine/GameScene.hpp"
#include "../include/System/Callback.hpp"

class Demonstration {
    using SceneLoadCallback = mz::Callback<void(mz::GameScene*)>;
    
public:
    void run();
    
protected:
    Demonstration(unsigned width, unsigned height, std::string name);
    
    void registerSceneLoadingCallback(SceneLoadCallback && callback);
    
//    template <typename F>
//    void registerSceneLoadingCallback(F && callback) {
//        registerSceneLoadingCallback(SceneLoadCallback{callback});
//    };
    
private:
    void goToScene(std::size_t sceneId);
    
private:
    mz::Application                 m_app;
    std::size_t                     m_currentSceneId    = 0;
    std::size_t                     m_nextSceneId       = 0;
    std::vector<SceneLoadCallback>  m_sceneLoadingCallbacks;
};

#endif /* Demonstration_hpp */
