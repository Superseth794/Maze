//
//  Orientation.inl
//  Maze
//
//  Created by Jaraxus on 27/07/2020.
//

namespace mz {

inline Orientation::Orientation() :
Orientation(mz::Orientation::UNDEFINED)
{
}

inline Orientation::Orientation(std::string const& name, sf::Vector2f vector) :
m_name(name),
m_direction(vector)
{
}

inline bool Orientation::operator==(Orientation const& orientation) const {
    return m_direction == orientation.m_direction;
}

inline bool Orientation::operator!=(Orientation const& orientation) const {
    return m_direction != orientation.m_direction;
}

inline std::string const& Orientation::getName() const{
    return m_name;
}

inline Orientation const& Orientation::getOrientation(std::string const& name) {
    if (name == UP.m_name)
        return UP;
    else if (name == RIGHT.m_name)
        return RIGHT;
    else if (name == DOWN.m_name)
        return DOWN;
    else if (name == LEFT.m_name)
        return LEFT;
    else
        return UNDEFINED;
}

template <typename T>
inline  Orientation const& Orientation::getOrientation(sf::Vector2<T> const& direction) {
    sf::Vector2f convertedDirection {static_cast<sf::Vector2f>(direction)};
    if (convertedDirection == UP.m_direction)
        return UP;
    else if (convertedDirection == RIGHT.m_direction)
        return RIGHT;
    else if (convertedDirection == DOWN.m_direction)
        return DOWN;
    else if (convertedDirection == LEFT.m_direction)
        return LEFT;
    else
        return UNDEFINED;
}

sf::Vector2f const& Orientation::toVector() const{
    return m_direction;
}

inline Orientation const& Orientation::turnClockwise() {
    if (m_direction.x == 0) { // UP || DOWN || UNDEFINED
        if (m_direction.y == UP.m_direction.y)
            return LEFT;
        else if (m_direction.y == DOWN.m_direction.y)
            return RIGHT;
        else
            return UNDEFINED;
    } else { // RIGHT || LEFT
        if (m_direction.x == RIGHT.m_direction.x)
            return UP;
        else
            return DOWN;
    }
}

inline Orientation const& Orientation::turnCounterClockwise() {
    if (m_direction.x == 0) { // UP || DOWN || UNDEFINED
        if (m_direction.y == UP.m_direction.y)
            return RIGHT;
        else if (m_direction.y == DOWN.m_direction.y)
            return LEFT;
        else
            return UNDEFINED;
    } else { // RIGHT || LEFT
        if (m_direction.x == RIGHT.m_direction.x)
            return DOWN;
        else
            return UP;
    }
}

}
