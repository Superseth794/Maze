//
//  Collisions.h
//  Maze
//
//  Created by Jaraxus on 04/05/2020.
//

#ifndef Collisions_VisualTest_h
# define Collisions_VisualTest_h

# include <string>
# include <functional>
# include <fstream>
# include <sstream>
# include <stdexcept>

# include <SFML/Graphics.hpp>

# include "../Utils.hpp"
# include "../../../Utils/Callback.hpp"
# include "../../../Physics/Collisions.hpp"

# ifdef MAZE_VISUAL_TEST_OUTPUT_DIRECTORY
#  undef MAZE_VISUAL_TEST_OUTPUT_DIRECTORY
# endif /* MAZE_VISUAL_TEST_OUTPUT_DIRECTORY */
# define MAZE_VISUAL_TEST_OUTPUT_DIRECTORY "Physics/Positions_Inside_Shapes/"

namespace mz::Test {

template <class F, typename ...Args, typename _ = std::enable_if<
                                                    std::is_invocable_v<F, sf::Vector2f, Args...> &&
                                                    std::is_same_v<typename mz::function_trait<F>::return_type, bool>>>
inline static std::function<sf::Color(sf::Vector2f const&)> generateEvaluationFunction(F const& function, Args const& ...args) {
    std::function<sf::Color(sf::Vector2f const&)> evaluationFunction = [&](sf::Vector2f const& position) -> sf::Color {
        bool result = function(position, args...);
        if (result)
            return sf::Color::Green;
        else
            return sf::Color::Transparent;
    };
    return evaluationFunction;
}
    
inline void positionInsideSegmentTest() {
    const auto segmentStart {generateRandomVector()}, segmentEnd {generateRandomVector()};
    std::string testTitle = "Position_Inside_Segment_" + toString(segmentStart) + "_" + toString(segmentEnd);
    
    const auto evaluationFunction {generateEvaluationFunction(mz::Collision::isPositionInsideSegment, segmentStart, segmentEnd)};
    
    drawTest(std::move(testTitle), evaluationFunction);
}

inline void positionInsideRayTest() {
    const auto rayStart {generateRandomVector()}, rayDirection {generateRandomVector()};
    std::string testTitle = "Position_Inside_Ray_" + toString(rayStart) + "_" + toString(rayDirection);
    
    const auto evaluationFunction {generateEvaluationFunction(mz::Collision::isPositionInsideRay, rayStart, rayDirection)};
    
    drawTest(std::move(testTitle), evaluationFunction);
}

inline void positionInsideLineTest() {
    const auto lineBelongingPosition {generateRandomVector()}, lineDirection {generateRandomVector()};
    std::string testTitle = "Position_Inside_Line_" + toString(lineBelongingPosition) + "_" + toString(lineDirection);
    
    const auto evaluationFunction {generateEvaluationFunction(mz::Collision::isPositionInsideLine, lineBelongingPosition, lineDirection)};
    
    drawTest(std::move(testTitle), evaluationFunction);
}

// TODO: ensure shape is inside frame
inline void positionInsideCircleTest() {
    const float circleRadius = generateRandomNumber(0.01f, (MAZE_VISUAL_TESTS_RANGE - 0.1f) / 2.f);
    const auto circleCenter {generateRandomVector(MAZE_VISUAL_TESTS_LOWER_BOUND + circleRadius, MAZE_VISUAL_TESTS_UPPER_BOUND - circleRadius)};
    std::string testTitle = "Position_Inside_Circle_" + toString(circleCenter) + "_" + toString(circleRadius);
    
    const auto evaluationFunction {generateEvaluationFunction(mz::Collision::isPositionInsideCircle, circleCenter, circleRadius)};
    
    drawTest(std::move(testTitle), evaluationFunction);
}

inline void positionInsideTriangleTest() {
    std::array<sf::Vector2f, 3> triangleVertexes {generateRandomVector(), generateRandomVector(), generateRandomVector()};
    std::string testTitle = "Position_Inside_Triangle_" + toString(triangleVertexes[0]) + "_" + toString(triangleVertexes[1]) + "_" + toString(triangleVertexes[2]);
    
    const auto evaluationFunction {generateEvaluationFunction(mz::Collision::isPositionInsideTriangle, triangleVertexes)};
    
    drawTest(std::move(testTitle), evaluationFunction);
}

inline void positionInsideAABBTest() {
    const sf::Vector2f AABB_topLeftCorner {generateRandomVector()};
    const float width = generateRandomNumber(0.01f, MAZE_VISUAL_TESTS_UPPER_BOUND - AABB_topLeftCorner.x);
    const float height = generateRandomNumber(0.01f, MAZE_VISUAL_TESTS_UPPER_BOUND - AABB_topLeftCorner.y);
    std::string testTitle = "Position_Inside_AABB_" + toString(AABB_topLeftCorner) + "_" + toString(width) + "_" + toString(height);
    
    const auto evaluationFunction {generateEvaluationFunction(mz::Collision::isPositionInsideAABB, AABB_topLeftCorner, width, height)};
    
    drawTest(std::move(testTitle), evaluationFunction);
}

// TODO: ensure OOBB is inside frame
inline void positionInsideOOBBTest() {
    const auto OOBB_topLeftCorner {generateRandomVector()};
    const float width = generateRandomNumber(0.01f, MAZE_VISUAL_TESTS_UPPER_BOUND - OOBB_topLeftCorner.x);
    const float height = generateRandomNumber(0.01f, MAZE_VISUAL_TESTS_UPPER_BOUND - OOBB_topLeftCorner.y);
    const float rotation = generateRandomNumber(0.f, 2.f * static_cast<float>(M_PI), 4u);
    std::string testTitle = "Position_Inside_OOBB_" + toString(OOBB_topLeftCorner) + "_" + toString(width) + "_" + toString(height) + "_" + toString(mz::fromRadianToDegrees(rotation), 7) + "°";
    
    const auto evaluationFunction {generateEvaluationFunction(mz::Collision::isPositionInsideOOBB, OOBB_topLeftCorner, width, height, rotation)};
    
    drawTest(std::move(testTitle), evaluationFunction);
}

}

# undef MAZE_VISUAL_TEST_OUTPUT_DIRECTORY

#endif /* Collisions_VisualTest_h */
