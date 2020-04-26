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

namespace mz {

template <std::size_t degree, typename Type = double>
class Polynomial {
public:
    Polynomial() {
        std::fill(m_coeficients.begin(), m_coeficients.end(), 0);
    }
    
    Polynomial(std::array<Type, degree + 1> && coefs) : m_coeficients(std::forward(coefs)) {}
    
    Polynomial(std::initializer_list<Type> && list) {
        std::size_t maxDegreeGiven = std::min(list.size(), degree + 1);
        for (std::size_t i = 0; i < (degree + 1) - maxDegreeGiven; ++i) {
            m_coeficients[i] = 0;
        }
        std::move(list.begin(), list.begin() + maxDegreeGiven, m_coeficients.begin() + (degree + 1) - maxDegreeGiven);
    }
    
    std::optional<const Type&> getCoeficient(std::size_t coeficientDegree) const {
        if (coeficientDegree > degree)
            return std::nullopt;
        else
            return std::make_optional(m_coeficients[degree - coeficientDegree]);
    };
    
    bool setCoeficient(std::size_t coeficientDegree, Type const& newValue) {
        if (coeficientDegree > degree)
            return false;
        else {
            m_coeficients[coeficientDegree] = newValue;
            return true;
        }
    }
    
    Type compute(Type value) {
        Type result = m_coeficients[degree];
        for (size_t i = 1; i <= degree; ++i) {
            result += std::pow(value, i) * m_coeficients[degree - i];
        }
        return result;
    }
    
    std::string display(std::size_t digitsPrecision) {
        std::stringstream description;
        description << std::fixed;
        
        bool numberDisplayed = false;
        for (size_t i = 0; i < degree; ++i) {
            Type coef = m_coeficients[i];
            if (coef != 0) {
                if (coef != 1) {
                    if (numberDisplayed) {
                        description << (coef > 0 ? " + " : " - ");
                        description << std::setprecision(static_cast<int>(digitsPrecision)) << (coef > 0 ? coef : -coef);
                    } else {
                        description << std::setprecision(static_cast<int>(digitsPrecision)) << coef;
                    }
                }
                description << "x";
                if (i != degree - 1)
                    description << "^" << degree - i;
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
    
    const Type* cbegin() {
        return m_coeficients.cbegin();
    }
    
    Type* end() {
        return m_coeficients.end();
    }
    
    Type* cend() {
        return m_coeficients.cend();
    }
    
private:
    std::array<Type, degree + 1> m_coeficients;
};

}

#endif /* Polynomial_h */
