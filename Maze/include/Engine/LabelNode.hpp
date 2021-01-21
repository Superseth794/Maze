//
//  LabelNode.hpp
//  Maze
//
//  Created by Jaraxus on 14/08/2020.
//

#ifndef LabelNode_hpp
#define LabelNode_hpp

#include <string>

#include <SFML/Graphics.hpp>

#include "Node.hpp"

namespace mz {

class LabelNode : public Node {
public:
    explicit LabelNode();
    explicit LabelNode(std::string const& string, sf::Font const& font, unsigned int characterSize = 30);
    
    virtual void draw(Camera const& camera) override;
    
    inline sf::Vector2f findCharacterPos(std::size_t index) const;
    
    inline unsigned int getCharacterSize() const;
    
    inline sf::Color const& getFillColor() const;
    
    inline sf::Font const* getFont() const;
    
    inline sf::FloatRect getGlobalBounds() const;
    
    inline sf::FloatRect getLocalBounds() const;
    
    inline float getLetterSpacing() const;
    
    inline float getLineSpacing() const;
    
    inline sf::String const& getString() const;
    
    inline sf::Uint32 getStyle() const;
    
    inline sf::Color const& getOutlineColor() const;
    
    inline float getOutlineThickness() const;
    
    inline LabelNode& setCharacterSize(unsigned int size);
    
    inline LabelNode& setFillColor(sf::Color const& color);
    
    inline LabelNode& setFont(sf::Font const& font);
    
    inline LabelNode& setLetterSpacing(float spacingFactor);
    
    inline LabelNode& setLineSpacing(float spacingFactor);
    
    inline LabelNode& setOutlineColor(sf::Color const& color);
    
    inline LabelNode& setOutlineThickness(float thickness);
    
    inline LabelNode& setString(std::string const& string);
    
    inline LabelNode& setStyle(std::uint32_t style);
    
private:
    sf::Text m_text;
};

} // namespace mz

#include "LabelNode.inl"

#endif /* LabelNode_hpp */
