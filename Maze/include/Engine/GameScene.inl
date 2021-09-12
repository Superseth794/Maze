//
//  GameScene.inl
//  Maze
//
//  Created by Jaraxus on 20/08/2021.
//

inline mz::ActionHandler mz::GameScene::getActionHandler(Action const& action) {
    return m_actionPool.New(action);
}

inline mz::ActionHandler mz::GameScene::getActionHandler(Action && action) {
    return m_actionPool.New(std::move(action));
}

template <typename F, typename ...Args>
mz::GameScene::enable_if_building<F, mz::Action, Args...> mz::GameScene::buildActionHandler(F const& constructionFunc, Args && ...args) {
    Action action = constructionFunc(std::forward<Args>(args)...);
    return m_actionPool.New(std::move(action));
}

template <typename F, typename ...Args>
mz::GameScene::enable_if_building<F, mz::Action, mz::ActionPool*, Args...> mz::GameScene::buildActionHandler(F const& constructionFunc, Args && ...args) {
    Action action = constructionFunc(&m_actionPool, std::forward<Args>(args)...);
    return m_actionPool.New(std::move(action));
}
