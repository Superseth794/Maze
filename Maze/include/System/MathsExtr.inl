//
//  MathsExtr.inl
//  Maze
//
//  Created by Jaraxus on 27/07/2020.
//

namespace mz {

template<typename T> constexpr T fromDegreesToRadian(T angle) {
    return (angle * M_PI / 180.0);
}

template<typename T> constexpr T fromRadianToDegrees(T angle) {
    return (angle * 180.0 / M_PI);
}

template<typename T> constexpr T getScalarProduct(sf::Vector2<T> const& vect1, sf::Vector2<T> const& vect2) {
    return (vect1.x * vect2.x) + (vect1.y * vect2.y);
}

template<typename T> constexpr T getVectorLength(sf::Vector2<T> const& vect) {
    return (std::sqrt(vect.x * vect.x + vect.y * vect.y));
}

template<typename T> constexpr T getVectorLength2(sf::Vector2<T> const& vect) {
    return (vect.x * vect.x + vect.y * vect.y);
}

template<typename T> constexpr bool nearlyEquals(T a, T b, T maxDiff) {
    T diff = std::abs(b - a);
    return (diff <= maxDiff);
}

template<typename T> constexpr sf::Vector2<T> normalizeVector(sf::Vector2<T> const& vect) {
    if (vect.x == 0. && vect.y == 0.)
        return vect;
    T norme = std::sqrt(vect.x * vect.x + vect.y * vect.y);
    return sf::Vector2<T>{vect.x / norme, vect.y / norme};
}

} // namespace mz
