//
//  AbstractShapeNode.hpp
//  Maze
//
//  Created by Jaraxus on 11/08/2020.
//

#ifndef AbstractShapeNode_hpp
#define AbstractShapeNode_hpp

#include <memory>

#include <SFML/Graphics.hpp>

#include "Node.hpp"

namespace mz {

template <class ShapeT>
class AbstractShapeNode : Node {
public:
    virtual void draw(Camera const& camera) override; // TODO: optimise with frustrum
    
    inline sf::Color const& getFillColor() const;
    
    inline sf::FloatRect getGlobalBounds() const;
    
    inline sf::FloatRect getLocalBounds() const;
    
    inline sf::Color const& getOutlineColor() const;
    
    inline float getOutlineThickness() const;
    
    inline std::shared_ptr<const sf::Texture> getTexture() const;
    
    inline sf::IntRect const& getTextureRect() const;
    
    inline void setFillColor(sf::Color const& color);
    
    inline void setOutlineColor(sf::Color const& color);
    
    inline void setOutlineThickness(float thickness);
    
    inline void setTexture(std::shared_ptr<sf::Texture> const& texture, bool resetRect = false);
    
    inline void setTextureRect(sf::IntRect const& rect);
    
protected:
    AbstractShapeNode(ShapeT && shape, std::shared_ptr<sf::Texture> && texture = nullptr);
    
    // TODO: add static polymorphic function to check accurately if thse undelining ShapeNode is inside camera frustrum
    
private:
    ShapeT                          m_shape;
    std::shared_ptr<sf::Texture>    m_texture;
};

}

#include "AbstractShapeNode.inl"

#endif /* AbstractShapeNode_hpp */
