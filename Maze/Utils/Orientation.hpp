//
//  Orientation.hpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#ifndef Orientation_h
# define Orientation_h

# include <SFML/Graphics.hpp>

namespace mz {

class Orientation {
public:
    
    static const Orientation UP;
    static const Orientation RIGHT;
    static const Orientation DOWN;
    static const Orientation LEFT;
    static const Orientation UNDEFINED;
    
    std::string const& getName() const;
    sf::Vector2f const& toVector() const;
    
    bool operator==(Orientation const& orientation) const;
    bool operator!=(Orientation const& orientation) const;
    
    static Orientation const& getOrientation(std::string const& name);
    static Orientation const& getOrientation(sf::Vector2f const& direction); // template
    static Orientation const& getOrientation(sf::Vector2i const& direction);
    
    // turn clockwise / anticlockwise
    
private:
    Orientation(std::string const& name, sf::Vector2f vector);
    
private:
    std::string m_name;
    sf::Vector2f m_direction;
    
};


}

#endif /* Orientation_h */
