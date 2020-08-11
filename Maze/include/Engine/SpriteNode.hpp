//
//  SpriteNode.hpp
//  Maze
//
//  Created by Jaraxus on 11/08/2020.
//

#ifndef SpriteNode_hpp
#define SpriteNode_hpp

#include <memory>

#include <SFML/Graphics.hpp>

#include "Node.hpp"

namespace mz {

class SpriteNode : Node {
public:
    SpriteNode(sf::Sprite && sprite, std::shared_ptr<sf::RenderTexture> && texture);
    SpriteNode(sf::Sprite && sprite, std::shared_ptr<sf::RenderTexture> && texture, sf::IntRect const& rectangle);
    SpriteNode(SpriteNode && spriteNode) noexcept = default;
    
    SpriteNode& operator=(SpriteNode && spriteNode) noexcept = default;
    
    virtual void draw(Camera const& camera) override; // TODO: optimise with frustrum
    
    inline const sf::Color& getColor() const;
    
    inline sf::FloatRect getGlobalBounds() const; // TODO: return oobb
    
    inline sf::FloatRect getLocalBounds() const;
    
    inline std::shared_ptr<sf::RenderTexture> const& getTexture() const;
    
    inline sf::IntRect const& getTextureRect() const;
    
    inline void setColor(const sf::Color& color);
    
    inline void setTexture(const sf::Texture& texture, bool resetRect = false);
    
    inline void setTextureRect(const sf::IntRect& rectangle);
    
private:
    sf::Sprite                          m_sprite;
    std::shared_ptr<sf::RenderTexture>  m_texture;
};

};

#include "SpriteNode.inl"

#endif /* SpriteNode_hpp */
