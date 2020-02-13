//
//  ExtraMaths.hpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#ifndef ExtraMaths_hpp
# define ExtraMaths_hpp

# include <cmath>

# include <SFML/Graphics.hpp>

namespace mz {

inline sf::Vector2f normalize(sf::Vector2f const& vect) {
    float norme = std::sqrt(vect.x + vect.y);
    return sf::Vector2f{vect.x / norme, vect.y / norme};
}

}

#endif /* ExtraMaths_hpp */
