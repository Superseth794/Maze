//
//  Player.hpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#ifndef Player_hpp
#define Player_hpp

# include <cmath>

class Player {
public:
    Player() = default;
    ~Player() = default;
    
    Player(Player const& player) = delete;
    Player(Player && player) = delete;
    Player& operator=(Player const& player) = delete;
    
private:
    
};

#endif /* Player_hpp */
