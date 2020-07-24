//
//  Collisions.hpp
//  Maze
//
//  Created by Jaraxus on 21/03/2020.
//

#ifndef Collisions_h
# define Collisions_h

# include <array>
# include <cmath>
# include <memory>
# include <vector>

# include <SFML/Graphics.hpp>

# include "../System/MathsExtr.hpp"

namespace mz {

namespace Collision {

// TODO: Add query result structure / params

using CollisionData = std::unique_ptr<std::vector<sf::Vector2f>>;

/*------------------------------------------------------------------*/
/*-------------- Positions inside shapes computations --------------*/
/*------------------------------------------------------------------*/

inline bool isPositionInsideSegment(sf::Vector2f const& position, sf::Vector2f const& segmentStartPosition, sf::Vector2f const& segmentEndPosition) {
    const sf::Vector2f vectU {segmentEndPosition - segmentStartPosition};
    const sf::Vector2f vectV {position - segmentStartPosition};
    
    if (vectU.x * vectV.y != vectU.y * vectV.x) // ensure position belongs to line
        return false;
    
    const sf::Vector2f vectW {position.x - segmentEndPosition.x, position.y - segmentEndPosition.y};
    const float scal1 = vectV.x * vectU.x + vectV.y * vectU.y;
    const float scal2 = vectW.x * vectU.x + vectW.y * vectU.y;
    
    if (scal1 >= 0 && scal2 <= 0) // position is between the segment bounds
        return true;
    else
        return false;
}

inline bool isPositionInsideRay(sf::Vector2f const& position, sf::Vector2f const& rayStartPosition, sf::Vector2f const& rayDirection) {
    const sf::Vector2f vectV {position - rayStartPosition};
    
    if (vectV.x * rayDirection.y != vectV.y * rayDirection.x) // ensure position belongs to line
        return false;
    
    const float scalar = vectV.x * rayDirection.x + vectV.y * rayDirection.y;
    if (scalar >= 0) // position is in same direction
        return true;
    else
        return false;
}

inline bool isPositionInsideLine(sf::Vector2f const& position, sf::Vector2f const& lineBelongingPosition, sf::Vector2f const& lineDirection) {
    const sf::Vector2f vectV {position - lineBelongingPosition};
    
    if (vectV.x * lineDirection.y == vectV.y * lineDirection.x) // ensure position belongs to line
        return true;
    else
        return false;
}

inline bool isPositionInsideCircle(sf::Vector2f const& position, sf::Vector2f const& circleCenter, float circleRadius) {
    const float distance2 = (position.x - circleCenter.x) * (position.x - circleCenter.x) + (position.y - circleCenter.y) * (position.y - circleCenter.y);
    return distance2 <= circleRadius * circleRadius;
}

inline bool isPositionInsideTriangle(sf::Vector2f const& position, std::array<sf::Vector2f, 3> const& triangleVertexes) {
    const sf::Vector2f XA {position - triangleVertexes[0]}, XB {position - triangleVertexes[1]}, XC {position - triangleVertexes[2]};
    const float normXA = mz::getVectorLength(XA), normXB = mz::getVectorLength(XB), normXC = mz::getVectorLength(XC);
    
    const float angleAXC = std::acos(mz::getScalarProduct(XA, XC) / (normXA * normXC));
    const float angleCXB = std::acos(mz::getScalarProduct(XC, XB) / (normXC * normXB));
    const float angleBXA = std::acos(mz::getScalarProduct(XB, XA) / (normXB * normXA));
    
    const float angleSum = angleAXC + angleCXB + angleBXA;
    
    if (nearlyEquals<float>(angleSum, M_PI * 2.f, 1e-4f)) // ensure the sum of the angles corresponds to a hole turn
        return true;
    else
        return false;
}

inline bool isPositionInsideAABB(sf::Vector2f const& position, sf::Vector2f const& AABB_topLeftCorner, float AABB_width, float AABB_height) {
    return (position.x >= AABB_topLeftCorner.x &&
            position.x <= AABB_topLeftCorner.x + AABB_width &&
            position.y >= AABB_topLeftCorner.y &&
            position.y <= AABB_topLeftCorner.y + AABB_height);
}

inline bool isPositionInsideOOBB(sf::Vector2f const& position, sf::Vector2f const& OOBB_topLeftCorner, float OOBB_width, float OOBB_height, float OOBB_rotation) {
    const float rotationCos = std::cos(OOBB_rotation);
    const float rotationSin = std::sin(OOBB_rotation);

    const sf::Vector2f topRightCorner {
        OOBB_topLeftCorner.x + rotationCos * OOBB_width,
        OOBB_topLeftCorner.y + rotationSin * OOBB_width
    };
    const sf::Vector2f bottomLeftCorner {
        OOBB_topLeftCorner.x - rotationSin * OOBB_height,
        OOBB_topLeftCorner.y + rotationCos * OOBB_height
    };
    
    const auto AX {position - OOBB_topLeftCorner};
    const auto BX {position - topRightCorner};
    const auto DX {position - bottomLeftCorner};
    const auto AB {topRightCorner - OOBB_topLeftCorner};
    const auto AD {bottomLeftCorner - OOBB_topLeftCorner};
    
    return (getScalarProduct(AX, AB) >= 0 &&
            getScalarProduct(BX, -AB) >= 0 &&
            getScalarProduct(AX, AD) >= 0 &&
            getScalarProduct(DX, -AD) >= 0);
}

inline bool isPositionInsideConvexPolygone(sf::Vector2f const& position, std::vector<sf::Vector2f> const& convexPolygoneVertexes) {
    return false;
}

inline bool isPositionInsidePolygone(sf::Vector2f const& position, std::vector<sf::Vector2f> const& polygoneVertexes) {
    return false;
}

/*------------------------------------------------------------------*/
/*----------- Segment shapes intersections computations ------------*/
/*------------------------------------------------------------------*/

inline bool isCollisionSegmentSegment(sf::Vector2f const& segmentA_startPotion, sf::Vector2f const& segmentA_endPosition, sf::Vector2f const& segmentB_startPosition, sf::Vector2f const& segmentB_endPosition) {
    return false;
}

inline bool isCollisionSegmentRay(sf::Vector2f const& segmentStartPotion, sf::Vector2f const& segmentEndPosition, sf::Vector2f const& rayStartPosition, sf::Vector2f const& rayDirection) {
    return false;
}

inline bool isCollisionSegmentLine(sf::Vector2f const& segmentStartPotion, sf::Vector2f const& segmentEndPosition, sf::Vector2f const& lineBelongingPosition, sf::Vector2f const& lineDirection) {
    return false;
}

inline bool isCollisionSegmentCircle(sf::Vector2f const& segmentStartPotion, sf::Vector2f const& segmentEndPosition, sf::Vector2f const& circleCenter, float circleRadius) {
    return false;
}

inline bool isCollisionSegmentTriangle(sf::Vector2f const& segmentStartPotion, sf::Vector2f const& segmentEndPosition, std::array<sf::Vector2f, 3> const& triangleVertexes) {
    return false;
}

inline bool isCollisionSegmentAABB(sf::Vector2f const& segmentStartPotion, sf::Vector2f const& segmentEndPosition, sf::Vector2f const& AABB_topLeftCorner, float AABB_width, float AABB_height) {
    return false;
}

inline bool isCollisionSegmentOOBB(sf::Vector2f const& segmentStartPotion, sf::Vector2f const& segmentEndPosition, sf::Vector2f const& OOBB_topLeftCorner, float OOBB_width, float OOBB_height, float OOBB_rotation) {
    return false;
}

inline bool isCollisionSegmentConvexPolygone(sf::Vector2f const& segmentStartPotion, sf::Vector2f const& segmentEndPosition, std::vector<sf::Vector2f> const& convexPolygoneVertexes) {
    return false;
}

inline bool isCollisionSegmentPolygone(sf::Vector2f const& segmentStartPotion, sf::Vector2f const& segmentEndPosition, std::vector<sf::Vector2f> const& polygoneVertexes) {
    return false;
}

/*------------------------------------------------------------------*/
/*------------- Ray shapes intersections computations --------------*/
/*------------------------------------------------------------------*/

inline bool isCollisionRayRay(sf::Vector2f const& rayA_startPosition, sf::Vector2f const& rayA_direction, sf::Vector2f const& rayB_startPosition, sf::Vector2f const& rayB_direction) {
    return false;
}

inline bool isCollisionRayLine(sf::Vector2f const& rayStartPosition, sf::Vector2f const& rayDirection, sf::Vector2f const& lineBelongingPosition, sf::Vector2f const& lineDirection) {
    return false;
}

inline bool isCollisionRayCircle(sf::Vector2f const& rayStartPosition, sf::Vector2f const& rayDirection, sf::Vector2f const& circleCenter, float circleRadius) {
    return false;
}

inline bool isCollisionRayTriangle(sf::Vector2f const& rayStartPosition, sf::Vector2f const& rayDirection, std::array<sf::Vector2f, 3> const& triangleVertexes) {
    return false;
}

inline bool isCollisionRayAABB(sf::Vector2f const& rayStartPosition, sf::Vector2f const& rayDirection, sf::Vector2f const& AABB_topLeftCorner, float AABB_width, float AABB_height) {
    return false;
}

inline bool isCollisionRayOOBB(sf::Vector2f const& rayStartPosition, sf::Vector2f const& rayDirection, sf::Vector2f const& OOBB_topLeftCorner, float OOBB_width, float OOBB_height, float OOBB_rotation) {
    return false;
}

inline bool isCollisionRayConvexPolygone(sf::Vector2f const& rayStartPosition, sf::Vector2f const& rayDirection, std::vector<sf::Vector2f> const& convexPolygoneVertexes) {
    return false;
}

inline bool isCollisionRayPolygone(sf::Vector2f const& rayStartPosition, sf::Vector2f const& rayDirection, std::vector<sf::Vector2f> const& polygoneVertexes) {
    return false;
}

/*------------------------------------------------------------------*/
/*------------- Line shapes intersections computations -------------*/
/*------------------------------------------------------------------*/

inline bool isCollisionLineLine(sf::Vector2f const& lineA_belongingPosition, sf::Vector2f const& lineA_direction, sf::Vector2f const& lineB_belongingPosition, sf::Vector2f const& lineB_direction) {
    return false;
}

inline bool isCollisionLineCircle(sf::Vector2f const& lineBelongingPosition, sf::Vector2f const& lineDirection, sf::Vector2f const& circleCenter, float circleRadius) {
    return false;
}

inline bool isCollisionLineTriangle(sf::Vector2f const& lineBelongingPosition, sf::Vector2f const& lineDirection, std::array<sf::Vector2f, 3> const& triangleVertexes) {
    return false;
}

inline bool isCollisionLineAABB(sf::Vector2f const& lineBelongingPosition, sf::Vector2f const& lineDirection, sf::Vector2f const& AABB_topLeftCorner, float AABB_width, float AABB_height) {
    return false;
}

inline bool isCollisionLineOOBB(sf::Vector2f const& lineBelongingPosition, sf::Vector2f const& lineDirection, sf::Vector2f const& OOBB_topLeftCorner, float OOBB_width, float OOBB_height, float OOBB_rotation) {
    return false;
}

inline bool isCollisionLineConvexPolygone(sf::Vector2f const& lineBelongingPosition, sf::Vector2f const& lineDirection, std::vector<sf::Vector2f> const& convexPolygoneVertexes) {
    return false;
}

inline bool isCollisionLinePolygone(sf::Vector2f const& lineBelongingPosition, sf::Vector2f const& lineDirection, std::vector<sf::Vector2f> const& polygoneVertexes) {
    return false;
}

/*------------------------------------------------------------------*/
/*------------ Circle shapes intersections computations ------------*/
/*------------------------------------------------------------------*/

inline bool isCollisionCircleCircle(sf::Vector2f const& circleA_center, float circleA_radius, sf::Vector2f const& circleB_center, float circleB_radius) {
    return false;
}

inline bool isCollisionCircleTriangle(sf::Vector2f const& circleCenter, float circleRadius, std::array<sf::Vector2f, 3> const& triangleVertexes) {
    return false;
}

inline bool isCollisionCircleAABB(sf::Vector2f const& circleCenter, float circleRadius, sf::Vector2f const& AABB_topLeftCorner, float AABB_width, float AABB_height) {
    return false;
}

inline bool isCollisionCircleOOBB(sf::Vector2f const& circleCenter, float circleRadius, sf::Vector2f const& OOBB_topLeftCorner, float OOBB_width, float OOBB_height, float OOBB_rotation) {
    return false;
}

inline bool isCollisionCircleConvexPolygone(sf::Vector2f const& circleCenter, float circleRadius, std::vector<sf::Vector2f> const& convexPolygoneVertexes) {
    return false;
}

inline bool isCollisionCirclePolygone(sf::Vector2f const& circleCenter, float circleRadius, std::vector<sf::Vector2f> const& polygoneVertexes) {
    return false;
}

/*------------------------------------------------------------------*/
/*----------- Triangle shapes intersections computations -----------*/
/*------------------------------------------------------------------*/

inline bool isCollisionTriangleTriangle(std::array<sf::Vector2f, 3> const& triangleA_vertexes, std::array<sf::Vector2f, 3> const& triangleB_vertexes) {
    return false;
}

inline bool isCollisionTriangleAABB(std::array<sf::Vector2f, 3> const& triangleVertexes, sf::Vector2f const& AABB_topLeftCorner, float AABB_width, float AABB_height) {
    return false;
}

inline bool isCollisionTriangleOOBB(std::array<sf::Vector2f, 3> const& triangleVertexes, sf::Vector2f const& OOBB_topLeftCorner, float OOBB_width, float OOBB_height, float OOBB_rotation) {
    return false;
}

inline bool isCollisionTriangleConvexPolygone(std::array<sf::Vector2f, 3> const& triangleVertexes, std::vector<sf::Vector2f> const& convexPolygoneVertexes) {
    return false;
}

inline bool isCollisionTrianglePolygone(std::array<sf::Vector2f, 3> const& triangleVertexes, std::vector<sf::Vector2f> const& polygoneVertexes) {
    return false;
}

/*------------------------------------------------------------------*/
/*------------- AABB shapes intersections computations -------------*/
/*------------------------------------------------------------------*/

inline bool isCollisionABB_AABB(sf::Vector2f const& AABB_A_topLeftCorner, float AABB_A_width, float AABB_A_height, sf::Vector2f const& AABB_B_topLeftCorner, float AABB_B_width, float AABB_B_height) {
    return false;
}

inline bool isCollisionAABB_OOBB(sf::Vector2f const& AABB_topLeftCorner, float AABB_width, float AABB_height, sf::Vector2f const& OOBB_topLeftCorner, float OOBB_width, float OOBB_height, float OOBB_rotation) {
    return false;
}

inline bool isCollisionAABB_ConvexPolygone(sf::Vector2f const& AABB_topLeftCorner, float AABB_width, float AABB_height, std::vector<sf::Vector2f> const& convexPolygoneVertexes) {
    return false;
}

inline bool isCollisionAABB_Polygone(sf::Vector2f const& AABB_topLeftCorner, float AABB_width, float AABB_height, std::vector<sf::Vector2f> const& polygoneVertexes) {
    return false;
}

/*------------------------------------------------------------------*/
/*------------- OOBB shapes intersections computations -------------*/
/*------------------------------------------------------------------*/

inline bool isCollisionOOBB_OOBB(sf::Vector2f const& OOBB_A_topLeftCorner, float OOBB_A_width, float OOBB_A_height, float OOBB_A_rotation, sf::Vector2f const& OOBB_B_topLeftCorner, float OOBB_B_width, float OOBB_B_height, float OOBB_B_rotation) {
    return false;
}

inline bool isCollisionOOBB_ConvexPolygone(sf::Vector2f const& OOBB_topLeftCorner, float OOBB_width, float OOBB_height, float OOBB_rotation, std::vector<sf::Vector2f> const& convexPolygoneVertexes) {
    return false;
}

inline bool isCollisionOOBB_Polygone(sf::Vector2f const& OOBB_topLeftCorner, float OOBB_width, float OOBB_height, float OOBB_rotation, std::vector<sf::Vector2f> const& polygoneVertexes) {
    return false;
}

/*------------------------------------------------------------------*/
/*------- Convex Polygon shapes intersections computations ---------*/
/*------------------------------------------------------------------*/

inline bool isCollisionConvexPolygoneConvexPolygone(std::vector<sf::Vector2f> const& polygoneA_vertexes, std::vector<sf::Vector2f> const& polygoneB_vertexes) {
    return false;
}

inline bool isCollisionConvexPolygonePolygone(std::vector<sf::Vector2f> const& convexPolygoneVertexes, std::vector<sf::Vector2f> const& polygoneVertexes) {
    return false;
}

/*------------------------------------------------------------------*/
/*------- Concave Polygon shapes intersections computations --------*/
/*------------------------------------------------------------------*/

inline bool isCollisionPolygonePolygone(std::vector<sf::Vector2f> const& polygoneA_vertexes, std::vector<sf::Vector2f> const& polygoneB_vertexes) {
    return false;
}

}

}


#endif /* Collisions_h */
