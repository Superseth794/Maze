//
//  MathsExtr.hpp
//  Maze
//
//  Created by Jaraxus on 13/02/2020.
//

#ifndef MathsExtr_hpp
# define MathsExtr_hpp

# include <cmath>

# include <SFML/System.hpp>

# ifndef M_E
#  define M_E 2.71828182845904523536028747135266250 //> e
# endif

# ifndef M_LOG2E
#  define M_LOG2E 1.44269504088896340735992468100189214 //> log2(e)
# endif

# ifndef M_LOG10E
#  define M_LOG10E 0.434294481903251827651128918916605082 //> log10(e)
# endif

# ifndef M_LN2
#  define M_LN2 0.693147180559945309417232121458176568 //> ln(2)
# endif

# ifndef M_LN10
#  define M_LN10 2.30258509299404568401799145468436421 //> ln(10)
# endif

# ifndef M_PI
#  define M_PI 3.14159265358979323846264338327950288 //> pi
# endif

# ifndef M_PI_2
#  define M_PI_2 1.57079632679489661923132169163975144 //> pi/2
# endif

# ifndef M_PI_4
#  define M_PI_4 0.785398163397448309615660845819875721 //> pi/4
# endif

# ifndef M_1_PI
#  define M_1_PI 0.318309886183790671537767526745028724 //> 1/pi
# endif

# ifndef M_2_PI
#  define M_2_PI 0.636619772367581343075535053490057448 //> 2/pi
# endif

# ifndef M_PI2
#  define M_PI2 9.869604401089358618834490999876151135 //> pi^2
# endif

# ifndef M_PI3
#  define M_PI3 31.006276680299820175476315067101395202 //> pi^3
# endif

# ifndef M_SQRTPI
#  define M_SQRTPI 1.7724538509055160272981674833411451827 //> sqrt(pi)
# endif

# ifndef M_1_SQRTPI
#  define M_1_SQRTPI 0.5641895835477562869480794515607725858 //> 1/sqrt(pi)
# endif

# ifndef M_2_SQRTPI
#  define M_2_SQRTPI 1.12837916709551257389615890312154517 //> 2/sqrt(pi)
# endif

# ifndef M_SQRT2
#  define M_SQRT2 1.41421356237309504880168872420969808 //> sqrt(2)
# endif

# ifndef M_1_SQRT2
#  define M_1_SQRT2 0.707106781186547524400844362104849039 //> 1/sqrt(2)
# endif

# ifndef M_SQRT3
#  define M_SQRT3 1.732050807568877293527446341505872366 //> sqrt(3)
# endif

# ifndef M_1_SQRT3
#  define M_1_SQRT3 0.577350269189625764509148780501957455 //> 1/sqrt(3)
# endif

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

template<typename T> constexpr bool nearlyEquals(T a, T b, T maxDiff = std::numeric_limits<T>::epsilon()) {
    T diff = std::abs(b - a);
    return (diff <= maxDiff);
}

template<typename T> constexpr sf::Vector2<T> normalizeVector(sf::Vector2<T> const& vect) {
    if (vect.x == 0. && vect.y == 0.)
        return vect;
    T norme = std::sqrt(vect.x * vect.x + vect.y * vect.y);
    return sf::Vector2<T>{vect.x / norme, vect.y / norme};
}

}

#endif /* MathsExtr_hpp */
