//
//  Collisions.h
//  Maze
//
//  Created by Jaraxus on 04/05/2020.
//

#ifndef Collisions_VisualTest_h
# define Collisions_VisualTest_h

# ifndef COLLISION_VISUAL_TEST_LOWER_BOUND
#  define COLLISION_VISUAL_TEST_LOWER_BOUND -10.f
# endif // COLLISION_VISUAL_TEST_LOWER_BOUND

# ifndef COLLISION_VISUAL_TEST_UPPER_BOUND
#  define COLLISION_VISUAL_TEST_UPPER_BOUND 10.f
# endif // COLLISION_VISUAL_TEST_UPPER_BOUND

# ifndef COLLISION_VISUAL_TEST_SHIFT
#  define COLLISION_VISUAL_TEST_SHIFT 0.01f
# endif // COLLISION_VISUAL_TEST_SHIFT

# ifndef COLLISION_VISUAL_TEST_SUCCESS_COLOR
#  define COLLISION_VISUAL_TEST_SUCCESS_COLOR sf::Color::Green
# endif // COLLISION_VISUAL_TEST_SUCCESS_COLOR

# ifndef COLLISION_VISUAL_TEST_FAILURE_COLOR
#  define COLLISION_VISUAL_TEST_FAILURE_COLOR sf::Color::Red
# endif // COLLISION_VISUAL_TEST_FAILURE_COLOR

# include <string>
# include <functional>
# include <fstream>
# include <sstream>

# include <SFML/Graphics.hpp>

# include "../../Physics/Collisions.hpp"

// TODO: add in external file

template <typename ...Args>
inline void drawTest(std::string && title, std::function<bool(sf::Vector2f const&, Args...)> const& evaluationFunction, Args const& ...args) {
    static const std::string folderName = "VisualTestsFiles/";
    std::string filename = folderName + title + ".jpeg";
    
    unsigned int size = static_cast<unsigned int>((COLLISION_VISUAL_TEST_UPPER_BOUND - COLLISION_VISUAL_TEST_LOWER_BOUND) / COLLISION_VISUAL_TEST_SHIFT);
    
    sf::Image image {};
    image.create(size, size);
    
    for (unsigned int y = 0; y < size; ++y) {
        for (unsigned int x = 0; x < size; ++x) {
            float subX = COLLISION_VISUAL_TEST_LOWER_BOUND + x * COLLISION_VISUAL_TEST_SHIFT;
            float subY = COLLISION_VISUAL_TEST_LOWER_BOUND + y * COLLISION_VISUAL_TEST_SHIFT;
            bool testResult = evaluationFunction(sf::Vector2f{subX, subY}, args...);
            if (testResult)
                image.setPixel(x, size - y - 1, COLLISION_VISUAL_TEST_SUCCESS_COLOR);
            else if (subX == 0 || subY == 0)
                image.setPixel(x, size - y - 1, sf::Color::Black);
            else
                image.setPixel(x, size - y - 1, COLLISION_VISUAL_TEST_FAILURE_COLOR);
        }
    }
    
    if (!image.saveToFile(filename))
        throw std::runtime_error(filename + " file could not be saved");
    
    std::cout << filename << " generated" << std::endl;
}

inline float generateRandomNumber(float min = COLLISION_VISUAL_TEST_LOWER_BOUND, float max = COLLISION_VISUAL_TEST_UPPER_BOUND, float nbDigits = 1.f) {
    const float range = max - min;
    return (rand() % (static_cast<int>(range * nbDigits))) / nbDigits + min;
}

inline sf::Vector2f generateRandomVector(float min = COLLISION_VISUAL_TEST_LOWER_BOUND, float max = COLLISION_VISUAL_TEST_UPPER_BOUND) {
    return sf::Vector2f {generateRandomNumber(min, max), generateRandomNumber(min, max)};
}

inline std::string toString(float number, std::size_t precision = 2) {
    std::stringstream stream;
    stream << std::setprecision(static_cast<int>(precision)) << number;
    return stream.str();
}

inline std::string toString(sf::Vector2f const& vector, std::size_t precision = 2) {
    std::stringstream stream;
    stream << std::setprecision(static_cast<int>(precision)) << "(" << vector.x << "," << vector.y << ")";
    return stream.str();
}

inline void positionInsideSegmentTest() {
    std::function<bool(sf::Vector2f const&, sf::Vector2f const&, sf::Vector2f const&)> evaluationFunction {mz::Collision::isPositionInsideSegment};
    
    const auto segmentStart {generateRandomVector()}, segmentEnd {generateRandomVector()};
    std::string testTitle = "Position_Inside_Segment_" + toString(segmentStart) + "_" + toString(segmentEnd);
    
    drawTest<sf::Vector2f const&, sf::Vector2f const&>(std::move(testTitle), evaluationFunction, segmentStart, segmentEnd);
}

inline void positionInsideRayTest() {
    std::function<bool(sf::Vector2f const&, sf::Vector2f const&, sf::Vector2f const&)> evaluationFunction {mz::Collision::isPositionInsideRay};
    
    const auto rayStart {generateRandomVector()}, rayDirection {generateRandomVector()};
    std::string testTitle = "Position_Inside_Ray_" + toString(rayStart) + "_" + toString(rayDirection);
    
    drawTest<sf::Vector2f const&, sf::Vector2f const&>(std::move(testTitle), evaluationFunction, rayStart, rayDirection);
}

inline void positionInsideLineTest() {
    std::function<bool(sf::Vector2f const&, sf::Vector2f const&, sf::Vector2f const&)> evaluationFunction {mz::Collision::isPositionInsideLine};
    
    const auto lineBelongingPosition {generateRandomVector()}, lineDirection {generateRandomVector()};
    std::string testTitle = "Position_Inside_Line_" + toString(lineBelongingPosition) + "_" + toString(lineDirection);
    
    drawTest<sf::Vector2f const&, sf::Vector2f const&>(std::move(testTitle), evaluationFunction, lineBelongingPosition, lineDirection);
}

// TODO: ensure shape is inside frame
inline void positionInsideCircleTest() {
    std::function<bool(sf::Vector2f const&, sf::Vector2f const&, float)> evaluationFunction {mz::Collision::isPositionInsideCircle};
    
    const float circleRadius = generateRandomNumber(0.01f, (COLLISION_VISUAL_TEST_UPPER_BOUND - COLLISION_VISUAL_TEST_LOWER_BOUND - 0.1f) / 2.f);
    const auto circleCenter {generateRandomVector(COLLISION_VISUAL_TEST_LOWER_BOUND + circleRadius, COLLISION_VISUAL_TEST_UPPER_BOUND - circleRadius)};
    std::string testTitle = "Position_Inside_Circle_" + toString(circleCenter) + "_" + toString(circleRadius);
    
    drawTest<sf::Vector2f const&, float const>(std::move(testTitle), evaluationFunction, circleCenter, circleRadius);
}

inline void positionInsideTriangleTest() {
    std::function<bool(sf::Vector2f const&, std::array<sf::Vector2f, 3> const&)> evaluationFunction {mz::Collision::isPositionInsideTriangle};
    
    std::array<sf::Vector2f, 3> triangleVertexes {generateRandomVector(), generateRandomVector(), generateRandomVector()};
    std::string testTitle = "Position_Inside_Triangle_" + toString(triangleVertexes[0]) + "_" + toString(triangleVertexes[1]) + "_" + toString(triangleVertexes[2]);
    
    drawTest<std::array<sf::Vector2f, 3> const&>(std::move(testTitle), evaluationFunction, triangleVertexes);
}

inline void positionInsideAABBTest() {
    std::function<bool(sf::Vector2f const&, sf::Vector2f const&, float, float)> evaluationFunction {mz::Collision::isPositionInsideAABB};
    
    const sf::Vector2f AABB_topLeftCorner {generateRandomVector()};
    const float width = generateRandomNumber(0, COLLISION_VISUAL_TEST_UPPER_BOUND - AABB_topLeftCorner.x);
    const float height = generateRandomNumber(0, COLLISION_VISUAL_TEST_UPPER_BOUND - AABB_topLeftCorner.y);
    std::string testTitle = "Position_Inside_AABB_" + toString(AABB_topLeftCorner) + "_" + toString(width) + "_" + toString(height);
    
    drawTest<sf::Vector2f const&, float, float>(std::move(testTitle), evaluationFunction, AABB_topLeftCorner, width, height);
}

// TODO: ensure OOBB is inside frame
inline void positionInsideOOBBTest() {
    std::function<bool(sf::Vector2f const&, sf::Vector2f const&, float, float, float)> evaluationFunction {mz::Collision::isPositionInsideOOBB};
    
    const sf::Vector2f OOBB_topLeftCorner {generateRandomVector()};
    const float width = generateRandomNumber(0, COLLISION_VISUAL_TEST_UPPER_BOUND - OOBB_topLeftCorner.x);
    const float height = generateRandomNumber(0, COLLISION_VISUAL_TEST_UPPER_BOUND - OOBB_topLeftCorner.y);
    const float rotation = generateRandomNumber(0.f, 2.f * M_PI, 4);
    std::string testTitle = "Position_Inside_OOBB_" + toString(OOBB_topLeftCorner) + "_" + toString(width) + "_" + toString(height) + "_" + toString(mz::fromRadianToDegrees(rotation), 7) + "°";
    
    drawTest<sf::Vector2f const&, float, float, float>(std::move(testTitle), evaluationFunction, OOBB_topLeftCorner, width, height, rotation);
}


inline void applyAllTests() {
    positionInsideSegmentTest();
    positionInsideRayTest();
    positionInsideLineTest();
    positionInsideCircleTest();
    positionInsideTriangleTest();
    positionInsideAABBTest();
    positionInsideOOBBTest();
}

#endif /* Collisions_VisualTest_h */
