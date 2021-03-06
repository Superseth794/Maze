//
//  CircleShapeNode.hpp
//  Maze
//
//  Created by Jaraxus on 11/08/2020.
//

#ifndef CircleShapeNode_hpp
#define CircleShapeNode_hpp

#include <SFML/Graphics.hpp>

#include "AbstractShapeNode.hpp"

namespace mz {

class CircleShapeNode;

template <>
struct get_shape_node_drawable_type<CircleShapeNode> {
    using type = sf::CircleShape;
};

class CircleShapeNode : public AbstractShapeNode<CircleShapeNode> {
public:
    explicit CircleShapeNode(float radius = 0, std::shared_ptr<sf::Texture> && texture = nullptr);
    explicit CircleShapeNode(float radius, std::size_t pointCount, std::shared_ptr<sf::Texture> && texture = nullptr);
    
    virtual ~CircleShapeNode() = default;
    
    inline float getRadius() const;
    
    inline void setPointCount(std::size_t count);
    
    inline void setRadius(float radius);
};

}

#include "CircleShapeNode.inl"

#endif /* CircleShapeNode_hpp */
