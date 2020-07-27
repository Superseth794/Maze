//
//  Orientation.cpp
//  Maze
//
//  Created by Jaraxus on 27/07/2020.
//

#include "../../include/Core/Orientation.hpp"

namespace mz {

const Orientation   Orientation::UP         ("UP", sf::Vector2f(0.f, -1.f));
const Orientation   Orientation::RIGHT      ("RIGHT", sf::Vector2f(1.f, 0.f));
const Orientation   Orientation::DOWN       ("DOWN", sf::Vector2f(0.f, 1.f));
const Orientation   Orientation::LEFT       ("LEFT", sf::Vector2f(-1.f, 0.f));
const Orientation   Orientation::UNDEFINED  ("UNDEFINED", sf::Vector2f(0.f, 0.f));

} // namespace mz
