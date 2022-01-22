//
//  Application.inl
//  Maze
//
//  Created by Jaraxus on 13/09/2021.
//

namespace mz {

inline const GameScene& Application::getScene() const noexcept {
    return m_gameScene;
}

inline GameScene& Application::getScene() noexcept {
    return m_gameScene;
}

template <typename F>
void Application::run(F && callback) {
    while (runOnce())
        callback();
}

inline void Application::quit() {
    m_exiting = true;
}

inline std::uint64_t Application::getCurrentFrameTime() const {
    return static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
}

} // mz
