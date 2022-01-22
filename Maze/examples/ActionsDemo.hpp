//
//  ActionsDemo.hpp
//  Maze
//
//  Created by Jaraxus on 13/09/2021.
//

#ifndef ActionsDemo_hpp
#define ActionsDemo_hpp

#include "Demonstration.hpp"

#include "../include/Engine/CircleShapeNode.hpp"

class ActionsDemo : public Demonstration {
public:
    ActionsDemo(unsigned width, unsigned height);
    
private:
    static void loadScene1(mz::GameScene* scene);
    
private:
    static inline std::string s_name = "Actions demo";
};

#endif /* ActionsDemo_h */
