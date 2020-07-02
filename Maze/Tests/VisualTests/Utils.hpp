//
//  Utils.hpp
//  Maze
//
//  Created by Jaraxus on 01/07/2020.
//

#ifndef Utils_h
# define Utils_h

# define MAZE_VISUAL_TESTS_LOWER_BOUND (-10.f)
# define MAZE_VISUAL_TESTS_UPPER_BOUND (10.f)
# define MAZE_VISUAL_TESTS_RANGE 20.f

# define MAZE_VISUAL_TEST_SHIFT 0.01f

# define MAZE_VISUAL_TEST_FAILURE_COLOR sf::Color{199, 199, 199}

# ifndef MAZE_VISUAL_TEST_OUTPUT_DIRECTORY
# define MAZE_VISUAL_TEST_OUTPUT_DIRECTORY "./"
# endif

# include <cmath>
# include <functional>
# include <random>
# include <string>

# include "../../Utils/Callback.hpp"

namespace mz::Test {

// TODO: use c++20 concepts
/**
 \warning macro MAZE_VISUAL_TEST_OUTPUT_DIRECTORY must be defined before every call of function
 */
template <class F, typename ...Args, typename _ = std::enable_if_t<
                                                    std::is_invocable_v<F, sf::Vector2f, Args...> &&
                                                    std::is_same_v<typename mz::function_trait<F>::return_type, sf::Color>>>
inline void drawTest(std::string && testName, F const& evaluationFunction, Args const& ...args) {
    using namespace std::literals;
    
    unsigned int size = static_cast<unsigned int>(MAZE_VISUAL_TESTS_RANGE / MAZE_VISUAL_TEST_SHIFT);
    std::string filename = "VisualTestsFiles/"s + MAZE_VISUAL_TEST_OUTPUT_DIRECTORY + testName + ".jpg";
     
    sf::Image image {};
    image.create(size, size);
    
    for (unsigned int y = 0; y < size; ++y) {
        for (unsigned int x = 0; x < size; ++x) {
            float subX = MAZE_VISUAL_TESTS_LOWER_BOUND + x * MAZE_VISUAL_TEST_SHIFT;
            float subY = MAZE_VISUAL_TESTS_LOWER_BOUND + y * MAZE_VISUAL_TEST_SHIFT;
            sf::Color testResultColor = evaluationFunction(sf::Vector2f{subX, subY}, args...);
            if (testResultColor != sf::Color::Transparent)
                image.setPixel(x, size - y - 1, testResultColor);
            else if (subX == 0 || subY == 0)
                image.setPixel(x, size - y - 1, sf::Color::Black);
            else
                image.setPixel(x, size - y - 1, MAZE_VISUAL_TEST_FAILURE_COLOR);
        }
    }
    
    if (!image.saveToFile(filename))
        throw std::runtime_error(filename + " file could not be saved");
    
    std::cout << filename << " generated" << std::endl;
}

template <typename T = float>
inline T generateRandomNumber(T min = MAZE_VISUAL_TESTS_LOWER_BOUND, T max = MAZE_VISUAL_TESTS_UPPER_BOUND, unsigned int nbDigits = 1u) {
    T digitsGenerator = std::pow<T>(T{10}, nbDigits);
    
    std::random_device device {};
    std::mt19937 seed {device()};
    std::uniform_real_distribution<T> distrib {min * digitsGenerator, max * digitsGenerator};
    
    return distrib(seed) / digitsGenerator;
}

template <typename T = float>
inline sf::Vector2<T> generateRandomVector(T min = MAZE_VISUAL_TESTS_LOWER_BOUND, T max = MAZE_VISUAL_TESTS_UPPER_BOUND, unsigned int nbDigits = 1u) {
    return sf::Vector2<T> {generateRandomNumber(min, max, nbDigits), generateRandomNumber(min, max, nbDigits)};
}

inline std::string toString(float number, std::size_t precision = 2) { // Add in external String file
    std::stringstream stream;
    stream << std::setprecision(static_cast<int>(precision)) << number;
    return stream.str();
}

inline std::string toString(sf::Vector2f const& vector, std::size_t precision = 2) { // Add in external String file
    std::stringstream stream;
    stream << std::setprecision(static_cast<int>(precision)) << "(" << vector.x << "," << vector.y << ")";
    return stream.str();
}


}

#endif /* Utils_h */
