//
//  TraitsExtr.hpp
//  Maze
//
//  Created by Jaraxus on 24/07/2020.
//

#ifndef TraitsExt_hpp
#define TraitsExt_hpp

#include <chrono>
#include <complex>
#include <functional>

namespace mz {

template <class T>
struct function_trait : public function_trait<decltype(&std::decay_t<T>::operator())> {};

template <class ReturnType, typename ...Args>
struct function_trait<ReturnType(Args...)> {
    using return_type           = ReturnType;
    using function_type         = std::function<ReturnType(Args...)>;
    using args_container_type   = std::tuple<Args...>;
};

template <class Object, class ReturnType, typename ...Args>
struct function_trait<ReturnType(Object::*)(Args...)> {
    using return_type           = ReturnType;
    using function_type         = std::function<ReturnType(Args...)>;
    using args_container_type   = std::tuple<Args...>;
};

template <class Object, class ReturnType, typename ...Args>
struct function_trait<ReturnType(Object::*)(Args...) const> {
    using return_type           = ReturnType;
    using function_type         = std::function<ReturnType(Args...)>;
    using args_container_type   = std::tuple<Args...>;
};

template <typename T>
struct get_complexe_convertible_type {
private:
    
    using complex_float     = std::complex<float>;
    using complex_double    = std::complex<double>;
    using complex_long      = std::complex<long double>;
    
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
struct is_complexe_convertible : std::conditional_t<
                                    !std::is_same_v<get_complexe_convertible_type_v<T>, void>, std::true_type, std::false_type>{};

template <typename T>
inline constexpr bool is_complexe_convertible_v = is_complexe_convertible<T>::value;

template <typename T>
struct is_duration : std::false_type {};

template <typename Rep, class Period>
struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type {};

}

#endif /* TraitsExt_hpp */
