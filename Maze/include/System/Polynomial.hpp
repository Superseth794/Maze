//
//  Polynomial.hpp
//  Maze
//
//  Created by Jaraxus on 25/04/2020.
//

#ifndef Polynomial_h
#define Polynomial_h

#include <array>
#include <cmath>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include "MathsExtr.hpp"
#include "TraitsExtr.hpp"

namespace mz {

template <std::size_t degree, typename Type>
class AbstractPolynomial {
public:
    AbstractPolynomial();
    
    AbstractPolynomial(std::array<Type, degree + 1> && coefs);
    
    AbstractPolynomial(std::initializer_list<Type> && list);
    
    template <typename ComputationType = Type, typename _ = std::enable_if_t<
                                                                std::is_convertible_v<Type, ComputationType>, ComputationType>>
    ComputationType operator()(ComputationType value) const;
    
    Type* begin();
    
    const Type* cbegin() const;
    
    const Type* cend() const;
    
    template <typename ComputationType = Type, typename _ = std::enable_if_t<
                                                                std::is_convertible_v<Type, ComputationType>, ComputationType>>
    ComputationType compute(ComputationType value) const;
    
    std::string display(std::size_t digitsPrecision) const;
    
    Type* end();
    
    /**
     \warning: Result is undefinied if coeficientDegree excess polynomial degree
     */
    const Type& getCoeficient(std::size_t coeficientDegree) const;
    
    /**
     \warning: Result is undefinied if coeficientDegree excess polynomial degree
    */
    void setCoeficient(std::size_t coeficientDegree, Type const& newValue);
    
private:
    std::array<Type, degree + 1>    m_coeficients;
};

template <std::size_t degree, typename Type = double>
class Polynomial : public AbstractPolynomial<degree, Type> {
public:
    Polynomial();
    
    Polynomial(std::array<Type, degree + 1> && coefs);
    
    Polynomial(std::initializer_list<Type> && list);
};

template <typename Type>
class Polynomial<0, Type> : public AbstractPolynomial<0, Type> {
public:
    static constexpr std::size_t degree = 0;

public:
    Polynomial();
    
    Polynomial(std::array<Type, degree + 1> && coefs);
    
    Polynomial(std::initializer_list<Type> && list);
    
    /**
     \see source: https://math.stackexchange.com/questions/200617/how-to-solve-an-nth-degree-polynomial-equation
    */
    template <typename ReturnType = Type>
    std::vector<ReturnType> findRoots() const;
};

template <typename Type>
class Polynomial<1, Type> : public AbstractPolynomial<1, Type> {
public:
    static constexpr std::size_t degree = 1;
    
public:
    Polynomial();
    
    Polynomial(std::array<Type, degree + 1> && coefs);
    
    Polynomial(std::initializer_list<Type> && list);
    
    /**
    \details source: https://math.stackexchange.com/questions/200617/how-to-solve-an-nth-degree-polynomial-equation
    */
    template <typename ReturnType = Type>
    std::vector<ReturnType> findRoots() const;
};

template <typename Type>
class Polynomial<2, Type> : public AbstractPolynomial<2, Type> {
public:
    static constexpr std::size_t degree = 2;
    
public:
    Polynomial();
    
    Polynomial(std::array<Type, degree + 1> && coefs);
    
    Polynomial(std::initializer_list<Type> && list);
    
    /**
    \details source: https://math.stackexchange.com/questions/200617/how-to-solve-an-nth-degree-polynomial-equation
    */
    template <typename ReturnType = Type>
    std::vector<ReturnType> findRoots() const;
};

template <typename Type>
class Polynomial<3, Type> : public AbstractPolynomial<3, Type> {

    using ComplexConvertibleType = get_complexe_convertible_type<Type>;

public:
    static constexpr std::size_t degree = 3;
    
public:
    Polynomial();
    
    Polynomial(std::array<Type, degree + 1> && coefs);
    
    Polynomial(std::initializer_list<Type> && list);
    
    /**
    \details source: https://math.stackexchange.com/questions/200617/how-to-solve-an-nth-degree-polynomial-equation
    */
    template <typename ReturnType = Type>
    std::vector<ReturnType> findRoots() const;
};

template <typename Type>
class Polynomial<4, Type> : public AbstractPolynomial<4, Type> {
    
    using ComplexConvertibleType = get_complexe_convertible_type<Type>;
    
public:
    static constexpr std::size_t degree = 4;
    
public:
    Polynomial();
    
    Polynomial(std::array<Type, degree + 1> && coefs);
    
    Polynomial(std::initializer_list<Type> && list);
    
    /**
     \details source: https://math.stackexchange.com/questions/200617/how-to-solve-an-nth-degree-polynomial-equation
     */
    template <typename ReturnType = Type>
    std::vector<ReturnType> findRoots() const;
};

}

#include "Polynomial.inl"

#endif /* Polynomial_h */
