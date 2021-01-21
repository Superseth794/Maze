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

template <class ShapeNodeT>
struct get_shape_node_drawable_type;

/**
 \tparam ShapeNodeT Child class that must provide a specialisation for trait get_shape_node_drawable_type
 */
template <class ShapeNodeT>
class AbstractShapeNode : public Node {
    
    using ShapeT = typename get_shape_node_drawable_type<ShapeNodeT>::type;
    
public:
    virtual void draw(Camera const& camera) override; // TODO: optimise with frustrum
    
    inline sf::Color const& getFillColor() const;
    
    inline sf::FloatRect getGlobalBounds() const;
    
    inline sf::FloatRect getLocalBounds() const;
    
    inline sf::Color const& getOutlineColor() const;
    
    inline float getOutlineThickness() const;
    
    inline std::shared_ptr<const sf::Texture> getTexture() const;
    
    inline sf::IntRect const& getTextureRect() const;
    
    inline ShapeNodeT& setFillColor(sf::Color const& color);
    
    inline ShapeNodeT& setOutlineColor(sf::Color const& color);
    
    inline ShapeNodeT& setOutlineThickness(float thickness);
    
    inline ShapeNodeT& setTexture(std::shared_ptr<sf::Texture> const& texture, bool resetRect = false);
    
    inline ShapeNodeT& setTextureRect(sf::IntRect const& rect);
    
protected:
    template <typename ...Args>
    AbstractShapeNode(std::shared_ptr<sf::Texture> && texture, Args && ...args);
    
    // TODO: add static polymorphic function to check accurately if the undelining ShapeNode is inside camera frustrum

protected:
    ShapeT                          m_shape;
    
private:
    std::shared_ptr<sf::Texture>    m_texture;
};

}

#include "AbstractShapeNode.inl"

#endif /* AbstractShapeNode_hpp */
