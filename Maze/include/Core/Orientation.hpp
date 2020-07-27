//
//  Orientation.hpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

#ifndef Orientation_hpp
#define Orientation_hpp

#include <SFML/Graphics.hpp>

namespace mz {

class Orientation {
public:
    inline Orientation();
    inline Orientation(Orientation const& orientation) = default;
    inline Orientation(Orientation && orientation) = default;
    
    inline Orientation& operator=(Orientation const& orientation) = default;
    inline Orientation& operator=(Orientation && orientation) = default;
    
    inline bool operator==(Orientation const& orientation) const;
    inline bool operator!=(Orientation const& orientation) const;
    
    inline std::string const& getName() const;
    
    static inline Orientation const& getOrientation(std::string const& name);
    
    template <typename T = float>
    static inline Orientation const& getOrientation(sf::Vector2<T> const& direction);
    
    inline sf::Vector2f const& toVector() const;
    
    inline Orientation const& turnClockwise();
    
    inline Orientation const& turnCounterClockwise();
    
private:
    inline Orientation(std::string const& name, sf::Vector2f vector);
    
public:
    static const Orientation    UP;
    static const Orientation    RIGHT;
    static const Orientation    DOWN;
    static const Orientation    LEFT;
    static const Orientation    UNDEFINED;
    
private:
    std::string                 m_name;
    sf::Vector2f                m_direction;
};

}

#include "Orientation.inl"

#endif /* Orientation_hpp */
