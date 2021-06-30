//
//  MathsExt.inl
//  Maze
//
//  Created by Jaraxus on 27/07/2020.
//

namespace mz {

template<typename T> constexpr T bernsteinPolynomial(T k, T n, T x) {
    return nChooseK(n, k) * std::pow(x, k) * std::pow(1 - x, n - k);
}

// use c++20 concepts
template <typename T, typename array_T> constexpr T bezierCurveDegree1At(T value, array_T const& controlValues) {
    std::size_t nbControlValues = std::size(controlValues);
    T resultValue {0};
    for (std::size_t i = 0; i < nbControlValues; ++i)
        resultValue += bernsteinPolynomial<float>(i, nbControlValues - 1, value) * controlValues[i];
    return resultValue;
}

// use C++20 concepts
template <typename T, typename array_T> constexpr sf::Vector2<T> bezierCurveDegree2At(T value, array_T const& controlPoints) {
    std::size_t nbControlPoints = std::size(controlPoints);
    sf::Vector2<T> resultPoint {0, 0};
    for (std::size_t i = 0; i < nbControlPoints; ++i) {
        float coef = bernsteinPolynomial<float>(i, nbControlPoints - 1, value);
        resultPoint.x += static_cast<T>(coef * controlPoints[i].x);
        resultPoint.y += static_cast<T>(coef * controlPoints[i].y);
    }
    return resultPoint;
}

// use C++20 concepts
template <typename T, template <typename> class array_T> constexpr sf::Vector2<T> bezierCurveDegree2DerivativeAt(T value, array_T<sf::Vector2<T>> const& controlPoints) {
    std::size_t nbControlPoints = std::size(controlPoints);
    sf::Vector2<T> resultPoint = {0, 0};
    for (std::size_t i = 0; i < nbControlPoints - 1; ++i) {
        T coef = bernsteinPolynomial<float>(i, nbControlPoints - 2, value);
        resultPoint.x += static_cast<T>(coef * (controlPoints[i+1].x - controlPoints[i].x));
        resultPoint.y += static_cast<T>(coef * (controlPoints[i+1].y - controlPoints[i].y));
    }
    resultPoint *= static_cast<T>(nbControlPoints);
    return resultPoint;
}

/**
 https://quick-bench.com/q/d2hY2HRADbhxAOP_3mNsptOXC1Y
 */
template <std::size_t N> constexpr std::size_t findFirstFreeBit(std::bitset<N> const& bitset) {
//#if defined MAZE_STDLIB_LIBSTDCPP || (defined(MAZE_COMPILER_CLANG) && __has_builtin(__builtin_ctzl))
#if defined(MAZE_COMPILER_CLANG) && __has_builtin(__builtin_ctzl)
    return bset._Find_first();
#else // default implementation without optimization
    if (bitset.none())
        return std::numeric_limits<std::size_t>::max();
    for (std::size_t i = 0; i < N; ++i) {
        if (bitset[i])
            return i;
    }
    return std::numeric_limits<std::size_t>::max();
#endif
}

template<typename T> constexpr T fromDegreesToRadian(T angle) {
    return (angle * M_PI / 180.0);
}

template<typename T> constexpr T fromRadianToDegrees(T angle) {
    return (angle * static_cast<T>(180.0 / M_PI));
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

/**
 \see https://stackoverflow.com/questions/15301885/calculate-value-of-n-choose-k
 */
template<typename T> constexpr T nChooseK(T n, T k) {
    float result = 1.f;
    if (k > n - k)
        k = n - k;
    for (T i = 1; i <= k; i += 1) {
        result *= (n + 1 - i) / static_cast<float>(i);
    }
    return static_cast<T>(result);
}

template<typename T> constexpr bool nearlyEquals(T a, T b, T maxDiff) {
    T diff = std::abs(b - a);
    return (diff <= maxDiff);
}

template<typename T> constexpr sf::Vector2<T> normalizeVector(sf::Vector2<T> const& vect) {
    if (vect.x == 0 && vect.y == 0)
        return vect;
    T norme = std::sqrt(vect.x * vect.x + vect.y * vect.y);
    return sf::Vector2<T>{vect.x / norme, vect.y / norme};
}

} // namespace mz
