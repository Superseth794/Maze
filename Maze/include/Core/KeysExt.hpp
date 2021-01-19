//
//  KeysExt.hpp
//  Maze
//
//  Created by Jaraxus on 07/07/2020.
//

#ifndef InputsExt_h
#define InputsExt_h

#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>

namespace mz {

class KeysExt {
public:
    KeysExt() = delete;
    
    static constexpr sf::Keyboard::Key getKeyFromName(std::string const& name);
    
    static constexpr std::string getNameFromKey(sf::Keyboard::Key const& key);
    
private:
    static std::unordered_map<sf::Keyboard::Key, std::string>   s_keysMap;
};

} // namespace mz

#include "KeysExt.inl"

#endif /* InputsExt_h */
