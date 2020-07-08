//
//  DrawableEntity.hpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#ifndef DrawableEntity_hpp
# define DrawableEntity_hpp

# include <memory>

# include <SFML/Graphics.hpp>

# include "../Entity.hpp"

namespace mz {

class DrawableEntity : public Entity {
public:
    DrawableEntity() = default;
    
    virtual ~DrawableEntity() = default;
    
    virtual std::shared_ptr<sf::RenderTexture> draw() = 0;
};

}

#endif /* DrawableEntity_hpp */
