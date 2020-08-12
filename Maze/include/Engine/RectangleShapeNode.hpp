//
//  RectangleShapeNode.hpp
//  Maze
//
//  Created by Jaraxus on 12/08/2020.
//

#ifndef RectangleShapeNode_hpp
#define RectangleShapeNode_hpp

#include <SFML/Graphics.hpp>

#include "AbstractShapeNode.hpp"

namespace mz {

class RectangleShapeNode;

template <>
struct get_shape_node_drawable_type<RectangleShapeNode> {
    using type = sf::RectangleShape;
};

class RectangleShapeNode : public AbstractShapeNode<RectangleShapeNode> {
public:
    explicit RectangleShapeNode(sf::Vector2f const& size = sf::Vector2f{0.f, 0.f}, std::shared_ptr<sf::Texture> && texture = nullptr);
    explicit RectangleShapeNode(float width = 0, float height = 0, std::shared_ptr<sf::Texture> && texture = nullptr);
    
    virtual ~RectangleShapeNode() = default;
    
    inline sf::Vector2f const& getSize() const;
    
    inline void setSize(sf::Vector2f const& size);
};

}

#include "RectangleShapeNode.inl"

#endif /* RectangleShapeNode_hpp */
