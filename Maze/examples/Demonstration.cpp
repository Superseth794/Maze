//
//  Demonstration.cpp
//  Maze
//
//  Created by Jaraxus on 13/09/2021.
//

#include "Demonstration.hpp"

Demonstration::Demonstration(unsigned width, unsigned height, std::string name):
m_app(width, height, std::move(name)),
m_sceneLoadingCallbacks()
{
}

void Demonstration::registerSceneLoadingCallback(SceneLoadCallback && callback) {
    m_sceneLoadingCallbacks.emplace_back(callback);
}

void Demonstration::run() {
    if (m_sceneLoadingCallbacks.empty())
        throw std::runtime_error("Attempt to launch a demonstration without having registered any scene building callbacks");
    
    using namespace std::literals;
    
    m_app.run([this]() {
        if (m_currentSceneId != m_nextSceneId) {
            // clear scene
            m_currentSceneId = m_nextSceneId;
            m_app.getScene().getMainLayer().removeAllChildrens();
            m_sceneLoadingCallbacks[m_currentSceneId](&m_app.getScene());
            mz::Logs::Global.display("Scene "s + std::to_string(m_currentSceneId) + " has successfully been loaded"s, mz::SUCCESS);
        }
    });
}
