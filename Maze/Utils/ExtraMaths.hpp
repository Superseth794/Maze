//
//  ExtraMaths.hpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#ifndef ExtraMaths_hpp
# define ExtraMaths_hpp

# include <cmath>

# include <SFML/System.hpp>

# ifndef M_PI
#  define M_PI 3.14159265359 //> pi
# endif

# ifndef M_PI_2
#  define M_PI_2 1.57079632679 //> sqrt(pi)
# endif

# ifndef M_PI2
#  define M_PI2 9.86960440109 //> pi^2
# endif

# ifndef M_PI3
#  define M_PI3 31.0062766803 //> pi^3
# endif

# ifndef M_E
#  define M_E 2.71828182846 //> e
# endif

# ifndef M_SQRT2
#  define M_SQRT2 1.4142135623730950488016887 //> sqrt(2)
# endif

namespace mz {

template<typename T> constexpr bool nearlyEquals(T a, T b) {
    T diff = std::abs(b - a);
    return (diff <= std::numeric_limits<T>::epsilon());
}

template<typename T> constexpr bool nearlyEquals(T a, T b, T maxDiff) {
    T diff = std::abs(b - a);
    return (diff <= maxDiff);
}

template<typename T> constexpr T fromDegreesToRadian(T angle) {
    return (angle * M_PI / 180.0);
}

template<typename T> constexpr T fromRadianToDegrees(T angle) {
    return (angle * 180.0 / M_PI);
}

template<typename T> constexpr sf::Vector2<T> normalizeVector(sf::Vector2<T> const& vect) {
    if (vect.x == 0. && vect.y == 0.)
        return vect;
    T norme = std::sqrt(vect.x * vect.x + vect.y * vect.y);
    return sf::Vector2<T>{vect.x / norme, vect.y / norme};
}

template<typename T> constexpr T getVectorLength(sf::Vector2<T> const& vect) {
    return (std::sqrt(vect.x * vect.x + vect.y * vect.y));
}

template<typename T> constexpr T getVectorLength2(sf::Vector2<T> const& vect) {
    return (vect.x * vect.x + vect.y * vect.y);
}

template<typename T> constexpr T getScalarProduct(sf::Vector2<T> const& vect1, sf::Vector2<T> const& vect2) {
    return (vect1.x * vect2.x) + (vect1.y * vect2.y);
}

}

#endif /* ExtraMaths_hpp */
