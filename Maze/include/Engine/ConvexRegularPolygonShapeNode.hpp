//
//  ConvexRegularPolygonShapeNode.hpp
//  Maze
//
//  Created by Jaraxus on 12/08/2020.
//

#ifndef ConvexRegularPolygonShapeNode_hpp
#define ConvexRegularPolygonShapeNode_hpp

#include <cmath>

#include <SFML/Graphics.hpp>

#include "AbstractShapeNode.hpp"
#include "../System/MathsExt.hpp"

namespace mz {

template <std::size_t EdgesCount>
class ConvexRegularPolygonShapeNode : public AbstractShapeNode<ConvexRegularPolygonShapeNode<EdgesCount>> {
    using isConstructible = typename std::enable_if<(EdgesCount > 2)>::type;
    
public:
    explicit ConvexRegularPolygonShapeNode(float size = 0, std::shared_ptr<sf::Texture> && texture = nullptr);
    
    virtual ~ConvexRegularPolygonShapeNode() = default;
    
    inline float getSize() const;
    
    inline ConvexRegularPolygonShapeNode& setSize(float size);
    
private:
    constexpr float getRadiusFromSize(float size);
    
private:
    float m_size;
};

template <std::size_t EdgesCount>
struct get_shape_node_drawable_type<ConvexRegularPolygonShapeNode<EdgesCount>> {
    using type = sf::CircleShape;
};

using TriangleShapeNode = ConvexRegularPolygonShapeNode<3>;
using SquareShapeNode = ConvexRegularPolygonShapeNode<4>;
using PentagonShapeNode = ConvexRegularPolygonShapeNode<5>;
using ExagonShapeNode = ConvexRegularPolygonShapeNode<6>;
using HeptagonShapeNode = ConvexRegularPolygonShapeNode<7>;
using OctogonShapeNode = ConvexRegularPolygonShapeNode<8>;
using EnneagonShapeNode = ConvexRegularPolygonShapeNode<9>;
using DecagonShapeNode = ConvexRegularPolygonShapeNode<10>;
using HendecagonShapeNode = ConvexRegularPolygonShapeNode<11>;
using DodecagonShapeNode = ConvexRegularPolygonShapeNode<12>;
using IcosagonShapeNode = ConvexRegularPolygonShapeNode<20>;
using HectogonShapeNode = ConvexRegularPolygonShapeNode<100>;

} // mz

#include "ConvexRegularPolygonShapeNode.inl"

#endif /* ConvexRegularPolygonShapeNode_hpp */
