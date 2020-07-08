//
//  Orientation.cpp
//  Maze
//
//  Created by Jaraxus on 06/02/2020.
//

# include "Orientation.hpp"

namespace mz {

const Orientation Orientation::UP("UP", sf::Vector2f(0, -1));
const Orientation Orientation::RIGHT("RIGHT", sf::Vector2f(1, 0));
const Orientation Orientation::DOWN("DOWN", sf::Vector2f(0, 1));
const Orientation Orientation::LEFT("LEFT", sf::Vector2f(-1, 0));
const Orientation Orientation::UNDEFINED("UNDEFINED", sf::Vector2f(0.f, 0.f));

Orientation::Orientation(std::string const& name, sf::Vector2f vector) :
m_name(name),
m_direction(vector)
{
}

std::string const& Orientation::getName() const{
    return m_name;
}

sf::Vector2f const& Orientation::toVector() const{
    return m_direction;
}

bool Orientation::operator==(Orientation const& orientation) const {
    return m_direction == orientation.m_direction;
}

bool Orientation::operator!=(Orientation const& orientation) const {
    return m_direction != orientation.m_direction;
}

Orientation const& Orientation::getOrientation(std::string const& name) {
    if (name == UP.getName())
        return UP;
    else if (name == RIGHT.getName())
        return RIGHT;
    else if (name == DOWN.getName())
        return DOWN;
    else if (name == LEFT.getName())
        return LEFT;
    else
        return UNDEFINED;
}

Orientation const& Orientation::getOrientation(sf::Vector2f const& direction) {
    if (direction == UP.toVector())
        return UP;
    else if (direction == RIGHT.toVector())
        return RIGHT;
    else if (direction == DOWN.toVector())
        return DOWN;
    else if (direction == LEFT.toVector())
        return LEFT;
    else
        return UNDEFINED;
}

Orientation const& Orientation::getOrientation(sf::Vector2i const& direction) {
    return getOrientation(sf::Vector2f(direction.x, direction.y));
}

}
