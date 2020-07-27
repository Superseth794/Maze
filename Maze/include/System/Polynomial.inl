//
//  Polynomial.inl
//  Maze
//
//  Created by Jaraxus on 27/07/2020.
//

namespace mz {

template <std::size_t degree, typename Type>
AbstractPolynomial<degree, Type>::AbstractPolynomial() {
    std::fill(m_coeficients.begin(), m_coeficients.end(), 0);
}

template <std::size_t degree, typename Type>
AbstractPolynomial<degree, Type>::AbstractPolynomial(std::array<Type, degree + 1> && coefs) :
m_coeficients(std::forward(coefs))
{
}

template <std::size_t degree, typename Type>
AbstractPolynomial<degree, Type>::AbstractPolynomial(std::initializer_list<Type> && list) {
    std::size_t maxDegreeGiven = std::min(list.size(), degree + 1);
    for (std::size_t i = 0; i < (degree + 1) - maxDegreeGiven; ++i) {
        m_coeficients[i] = 0;
    }
    std::move(list.begin(), list.begin() + maxDegreeGiven, m_coeficients.begin() + (degree + 1) - maxDegreeGiven);
}

template <std::size_t degree, typename Type>
template <typename ComputationType, typename _>
ComputationType AbstractPolynomial<degree, Type>::operator()(ComputationType value) const {
    return compute<ComputationType>(value);
}

template <std::size_t degree, typename Type>
Type* AbstractPolynomial<degree, Type>::begin() {
    return m_coeficients.begin();
}

template <std::size_t degree, typename Type>
const Type* AbstractPolynomial<degree, Type>::cbegin() const {
    return m_coeficients.cbegin();
}

template <std::size_t degree, typename Type>
const Type* AbstractPolynomial<degree, Type>::cend() const {
    return m_coeficients.cend();
}

template <std::size_t degree, typename Type>
template <typename ComputationType, typename _>
ComputationType AbstractPolynomial<degree, Type>::compute(ComputationType value) const {
    ComputationType result = m_coeficients[degree];
    for (size_t i = 1; i <= degree; ++i) {
        result += std::pow(value, i) * static_cast<ComputationType>(m_coeficients[degree - i]);
    }
    return result;
}

template <std::size_t degree, typename Type>
std::string AbstractPolynomial<degree, Type>::display(std::size_t digitsPrecision) const {
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

template <std::size_t degree, typename Type>
Type* AbstractPolynomial<degree, Type>::end() {
    return m_coeficients.end();
}

template <std::size_t degree, typename Type>
const Type& AbstractPolynomial<degree, Type>::getCoeficient(std::size_t coeficientDegree) const {
    assert(coeficientDegree >= 0 && coeficientDegree <= degree);
    return m_coeficients[degree - coeficientDegree];
};

template <std::size_t degree, typename Type>
void AbstractPolynomial<degree, Type>::setCoeficient(std::size_t coeficientDegree, Type const& newValue) {
    assert(coeficientDegree >= 0 && coeficientDegree <= degree);
    m_coeficients[degree - coeficientDegree] = newValue;
}

template <std::size_t degree, typename Type>
Polynomial<degree, Type>::Polynomial()
{
}

template <std::size_t degree, typename Type>
Polynomial<degree, Type>::Polynomial(std::array<Type, degree + 1> && coefs) :
AbstractPolynomial<degree, Type>(std::forward(coefs))
{
}

template <std::size_t degree, typename Type>
Polynomial<degree, Type>::Polynomial(std::initializer_list<Type> && list) :
AbstractPolynomial<degree, Type>(std::move(list))
{
}

template <typename Type>
Polynomial<0, Type>::Polynomial()
{
}

template <typename Type>
Polynomial<0, Type>::Polynomial(std::array<Type, degree + 1> && coefs) :
AbstractPolynomial<degree, Type>(std::forward(coefs))
{
}

template <typename Type>
Polynomial<0, Type>::Polynomial(std::initializer_list<Type> && list) :
AbstractPolynomial<degree, Type>(std::move(list))
{
}


template <typename Type> template <typename ReturnType>
std::vector<ReturnType> Polynomial<0, Type>::findRoots() const {
    return std::vector<ReturnType>{};
}

template <typename Type>
Polynomial<1, Type>::Polynomial()
{
}

template <typename Type>
Polynomial<1, Type>::Polynomial(std::array<Type, degree + 1> && coefs) :
AbstractPolynomial<degree, Type>(std::forward(coefs))
{
}
    
template <typename Type>
Polynomial<1, Type>::Polynomial(std::initializer_list<Type> && list) :
AbstractPolynomial<degree, Type>(std::move(list))
{
}
    
template <typename Type> template <typename ReturnType>
std::vector<ReturnType> Polynomial<1, Type>::findRoots() const {
    if (this->getCoeficient(1) != 0) {
        Type x = -this->getCoeficient(0) / this->getCoeficient(1);
        return std::vector<ReturnType>{x};
    } else {
        return std::vector<ReturnType>{static_cast<ReturnType>(0)};
    }
}

template <typename Type>
Polynomial<2, Type>::Polynomial()
{
}

template <typename Type>
Polynomial<2, Type>::Polynomial(std::array<Type, degree + 1> && coefs) :
AbstractPolynomial<degree, Type>(std::forward(coefs))
{
}

template <typename Type>
Polynomial<2, Type>::Polynomial(std::initializer_list<Type> && list) :
AbstractPolynomial<degree, Type>(std::move(list))
{
}

template <typename Type> template <typename ReturnType>
std::vector<ReturnType> Polynomial<2, Type>::findRoots() const {
    Type c2 {this->getCoeficient(2)}, c1 {this->getCoeficient(1)}, c0 {this->getCoeficient(0)};
    if (c2 != 0) {
        return {};
    } else {
        Polynomial<1, Type> subPoly {c1, c0};
        return subPoly.template findRoots<ReturnType>();
    }
}

template <typename Type>
Polynomial<3, Type>::Polynomial()
{
}

template <typename Type>
Polynomial<3, Type>::Polynomial(std::array<Type, degree + 1> && coefs) :
AbstractPolynomial<degree, Type>(std::forward(coefs))
{
}

template <typename Type>
Polynomial<3, Type>::Polynomial(std::initializer_list<Type> && list) :
AbstractPolynomial<degree, Type>(std::move(list))
{
}

template <typename Type> template <typename ReturnType>
std::vector<ReturnType> Polynomial<3, Type>::findRoots() const {
    Type b3 {this->getCoeficient(3)}, b2 {this->getCoeficient(2)}, b1 {this->getCoeficient(1)}, b0 {this->getCoeficient(0)};
    if (b3 != 0) {
        return {};
    } else {
        Polynomial<2, Type> subPoly {this->getCoeficient(2), this->getCoeficient(1), this->getCoeficient(0)};
        return subPoly.template findRoots<ReturnType>();
    }
}

template <typename Type>
Polynomial<4, Type>::Polynomial()
{
}

template <typename Type>
Polynomial<4, Type>::Polynomial(std::array<Type, degree + 1> && coefs) :
AbstractPolynomial<degree, Type>(std::forward(coefs))
{
}

template <typename Type>
Polynomial<4, Type>::Polynomial(std::initializer_list<Type> && list) :
AbstractPolynomial<degree, Type>(std::move(list))
{
}

template <typename Type> template <typename ReturnType>
std::vector<ReturnType> Polynomial<4, Type>::findRoots() const {
    if (this->m_coeficients[4] != 0) {
        
    } else {
        Polynomial<3, Type> subPoly {this->m_coeficients[3], this->m_coeficients[2], this->m_coeficients[1], this->m_coeficients[0]};
        return subPoly.template findRoots<ReturnType>();
    }
}


} // namespace mz
