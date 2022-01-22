//
//  actionsDemo.cpp
//  Maze
//
//  Created by Jaraxus on 23/08/2021.
//

#include "ActionsDemo.hpp"

ActionsDemo::ActionsDemo(unsigned width, unsigned height) :
Demonstration(width, height, s_name)
{
    registerSceneLoadingCallback({&ActionsDemo::loadScene1}); // TODO: check ???
}

void ActionsDemo::loadScene1(mz::GameScene *scene) {
    
}
