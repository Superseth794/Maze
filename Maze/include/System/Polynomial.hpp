//
//  Polynomial.hpp
//  Maze
//
//  Created by Jaraxus on 25/04/2020.
//

#ifndef Polynomial_h
# define Polynomial_h

# include <cmath>
# include <array>
# include <initializer_list>
# include <string>
# include <sstream>
# include <iomanip>
# include <optional>
# include <complex>

# include <iostream>

# include "ExtraMaths.hpp"

namespace mz {

template <typename T>
struct get_complexe_convertible_type {
private:
    using complex_float = std::complex<float>;
    using complex_double = std::complex<double>;
    using complex_long = std::complex<long double>;
public:
    using value = std::conditional_t<std::is_convertible_v<complex_float, T>, complex_float,
                    std::conditional_t<std::is_convertible_v<complex_double, T>, complex_double,
                        std::conditional_t<std::is_convertible_v<complex_long, T>, complex_long, void>>>;
};

template <>
struct get_complexe_convertible_type<std::complex<float>> {
    using value = std::complex<float>;
};

template <>
struct get_complexe_convertible_type<std::complex<double>> {
    using value = std::complex<double>;
};

template <>
struct get_complexe_convertible_type<std::complex<long double>> {
    using value = std::complex<long double>;
};

template <typename T>
using get_complexe_convertible_type_v = typename get_complexe_convertible_type<T>::value;

template <typename T>
struct is_complexe_convertible : std::conditional_t<!std::is_same_v<get_complexe_convertible_type_v<T>, void>, std::true_type, std::false_type>{};

template <typename T>
inline constexpr bool is_complexe_convertible_v = is_complexe_convertible<T>::value;

template <std::size_t degree, typename Type>
class AbstractPolynomial {
public:
    AbstractPolynomial() {
        std::fill(m_coeficients.begin(), m_coeficients.end(), 0);
    }
    
    AbstractPolynomial(std::array<Type, degree + 1> && coefs) : m_coeficients(std::forward(coefs)) {}
    
    AbstractPolynomial(std::initializer_list<Type> && list) {
        std::size_t maxDegreeGiven = std::min(list.size(), degree + 1);
        for (std::size_t i = 0; i < (degree + 1) - maxDegreeGiven; ++i) {
            m_coeficients[i] = 0;
        }
        std::move(list.begin(), list.begin() + maxDegreeGiven, m_coeficients.begin() + (degree + 1) - maxDegreeGiven);
    }
    
    template <typename ComputationType = Type, typename = std::enable_if_t<std::is_convertible_v<Type, ComputationType>, ComputationType>>
    ComputationType operator()(ComputationType value) const {
        return compute<ComputationType>(value);
    }
    
    /**
     \warning: Result is undefinied if coeficientDegree excess polynomial degree
     */
    const Type& getCoeficient(std::size_t coeficientDegree) const {
        assert(coeficientDegree >= 0 && coeficientDegree <= degree);
        return m_coeficients[degree - coeficientDegree];
    };
    
    /**
     \warning: Result is undefinied if coeficientDegree excess polynomial degree
    */
    void setCoeficient(std::size_t coeficientDegree, Type const& newValue) {
        assert(coeficientDegree >= 0 && coeficientDegree <= degree);
        m_coeficients[degree - coeficientDegree] = newValue;
    }
    
    template <typename ComputationType = Type, typename = std::enable_if_t<std::is_convertible_v<Type, ComputationType>, ComputationType>>
    ComputationType compute(ComputationType value) const {
        ComputationType result = m_coeficients[degree];
        for (size_t i = 1; i <= degree; ++i) {
            result += std::pow(value, i) * static_cast<ComputationType>(m_coeficients[degree - i]);
        }
        return result;
    }
    
    std::string display(std::size_t digitsPrecision) const {
        std::stringstream description;
        description << std::fixed;
        
        bool numberDisplayed = false;
        for (size_t i = degree; i > 0; --i) {
            Type coef = m_coeficients[degree - i];
            if (coef != 0) {
                if (coef != 1) {
                    if (numberDisplayed) {
                        description << (coef > 0 ? " + " : " - ");
                        description << std::setprecision(static_cast<int>(digitsPrecision)) << (coef > 0 ? coef : -coef);
                    } else {
                        description << std::setprecision(static_cast<int>(digitsPrecision)) << coef;
                    }
                } else if (numberDisplayed) {
                    description << " + ";
                }
                description << "x";
                if (i != 1)
                    description << "^" << i;
                numberDisplayed = true;
            }
        }
        if (m_coeficients[degree] != 0 || !numberDisplayed) {
            if (numberDisplayed) {
                description << (m_coeficients[degree] > 0 ? " + " : " - ");
                description << std::setprecision(static_cast<int>(digitsPrecision)) << (m_coeficients[degree] > 0 ? m_coeficients[degree] : -m_coeficients[degree]);
            } else {
                description << std::setprecision(static_cast<int>(digitsPrecision)) << m_coeficients[degree];
            }
        }
        
        return description.str();
    }
    
    Type* begin() {
        return m_coeficients.begin();
    }
    
    const Type* cbegin() const {
        return m_coeficients.cbegin();
    }
    
    Type* end() {
        return m_coeficients.end();
    }
    
    const Type* cend() const {
        return m_coeficients.cend();
    }
    
private:
    std::array<Type, degree + 1> m_coeficients;
};

template <std::size_t degree, typename Type = double>
class Polynomial : public AbstractPolynomial<degree, Type> {
public:
    Polynomial() {};
    Polynomial(std::array<Type, degree + 1> && coefs) : AbstractPolynomial<degree, Type>(std::forward(coefs)) {};
    Polynomial(std::initializer_list<Type> && list) : AbstractPolynomial<degree, Type>(std::move(list)) {};
};

template <typename Type>
class Polynomial<0, Type> : public AbstractPolynomial<0, Type> {
public:
    static constexpr std::size_t degree = 0;
    
    Polynomial() {};
    Polynomial(std::array<Type, degree + 1> && coefs) : AbstractPolynomial<degree, Type>(std::forward(coefs)) {};
    Polynomial(std::initializer_list<Type> && list) : AbstractPolynomial<degree, Type>(std::move(list)) {};
    
    /**
     \details source: https://math.stackexchange.com/questions/200617/how-to-solve-an-nth-degree-polynomial-equation
    */
    template <typename ReturnType = Type>
    std::vector<ReturnType> findRoots() const {
        return std::vector<ReturnType>{};
    }
};

template <typename Type>
class Polynomial<1, Type> : public AbstractPolynomial<1, Type> {
public:
    static constexpr std::size_t degree = 1;
    
    Polynomial() {};
    Polynomial(std::array<Type, degree + 1> && coefs) : AbstractPolynomial<degree, Type>(std::forward(coefs)) {};
    Polynomial(std::initializer_list<Type> && list) : AbstractPolynomial<degree, Type>(std::move(list)) {};
    
    /**
    \details source: https://math.stackexchange.com/questions/200617/how-to-solve-an-nth-degree-polynomial-equation
    */
    template <typename ReturnType = Type>
    std::vector<ReturnType> findRoots() const {
        if (this->getCoeficient(1) != 0) {
            Type x = -this->getCoeficient(0) / this->getCoeficient(1);
            return std::vector<ReturnType>{x};
        } else {
            return std::vector<ReturnType>{static_cast<ReturnType>(0)};
        }
    }
};

template <typename Type>
class Polynomial<2, Type> : public AbstractPolynomial<2, Type> {
public:
    static constexpr std::size_t degree = 2;
    
    Polynomial() {};
    Polynomial(std::array<Type, degree + 1> && coefs) : AbstractPolynomial<degree, Type>(std::forward(coefs)) {};
    Polynomial(std::initializer_list<Type> && list) : AbstractPolynomial<degree, Type>(std::move(list)) {};
    
    /**
    \details source: https://math.stackexchange.com/questions/200617/how-to-solve-an-nth-degree-polynomial-equation
    */
    template <typename ReturnType = Type>
    std::vector<ReturnType> findRoots() const {
        Type c2 {this->getCoeficient(2)}, c1 {this->getCoeficient(1)}, c0 {this->getCoeficient(0)};
        if (c2 != 0) {
            return {};
        } else {
            Polynomial<1, Type> subPoly {c1, c0};
            return subPoly.template findRoots<ReturnType>();
        }
    }
};

template <typename Type>
class Polynomial<3, Type> : public AbstractPolynomial<3, Type> {
public:
    using ComplexConvertibleType = get_complexe_convertible_type<Type>;
    static constexpr std::size_t degree = 3;
    
    Polynomial() {};
    Polynomial(std::array<Type, degree + 1> && coefs) : AbstractPolynomial<degree, Type>(std::forward(coefs)) {};
    Polynomial(std::initializer_list<Type> && list) : AbstractPolynomial<degree, Type>(std::move(list)) {};
    
    /**
    \details source: https://math.stackexchange.com/questions/200617/how-to-solve-an-nth-degree-polynomial-equation
    */
    template <typename ReturnType = Type>
    std::vector<ReturnType> findRoots() const {
        Type b3 {this->getCoeficient(3)}, b2 {this->getCoeficient(2)}, b1 {this->getCoeficient(1)}, b0 {this->getCoeficient(0)};
        if (b3 != 0) {
            return {};
        } else {
            Polynomial<2, Type> subPoly {this->getCoeficient(2), this->getCoeficient(1), this->getCoeficient(0)};
            return subPoly.template findRoots<ReturnType>();
        }
    }
};

template <typename Type>
class Polynomial<4, Type> : public AbstractPolynomial<4, Type> {
public:
    using ComplexConvertibleType = get_complexe_convertible_type<Type>;
    static constexpr std::size_t degree = 4;
    
    Polynomial() {};
    Polynomial(std::array<Type, degree + 1> && coefs) : AbstractPolynomial<degree, Type>(std::forward(coefs)) {};
    Polynomial(std::initializer_list<Type> && list) : AbstractPolynomial<degree, Type>(std::move(list)) {};
    
    /**
     \details source: https://math.stackexchange.com/questions/200617/how-to-solve-an-nth-degree-polynomial-equation
     */
    template <typename ReturnType = Type>
    std::vector<ReturnType> findRoots() const {
        if (this->m_coeficients[4] != 0) {
            
        } else {
            Polynomial<3, Type> subPoly {this->m_coeficients[3], this->m_coeficients[2], this->m_coeficients[1], this->m_coeficients[0]};
            return subPoly.template findRoots<ReturnType>();
        }
    }
};

}

#endif /* Polynomial_h */