//
//  Callback.hpp
//  Maze
//
//  Created by Jaraxus on 12/03/2020.
//

#ifndef Callback_h
# define Callback_h

# include <iostream>
# include <functional>

template<class T>
struct function_trait : public function_trait<decltype(&std::decay_t<T>::operator())> {};

template <class Class, class Return_type, class... Args>
struct function_trait<Return_type(Class::*)(Args...) const>
{
    using return_type = Return_type;
    using function_type = std::function<return_type(Args...)>;

    template <unsigned int N>
    using arg_type = typename std::tuple_element<N, std::tuple<Args...>>::type;
};

template <class T>
class Callback : public Callback<decltype(&std::decay_t<T>::operator())> {
public:
    Callback() {
        std::cout << "V1" << std::endl;
    };
    ~Callback() = default;

    void process() {
        // Does nothing in default implementation
        std::cout << "No callback implemention" << std::endl;
    }
};

template <typename T_Return, typename ...Args>
class Callback<T_Return(Args...) const> {
public:
    
    Callback(std::function<T_Return(Args...)> const& callback) : m_callback(callback) {
        std::cout << "V2" << std::endl;
    };
    ~Callback() = default;

    T_Return process(Args... args) {
        return m_callback(std::forward(args...));
    }

private:
    std::function<T_Return(Args...)> m_callback;
};

template <class C_Object, typename T_Return, typename ...Args>
class Callback<T_Return(C_Object::*)(Args...) const> {
public:
    Callback(C_Object* object, T_Return const& callback) : m_object(object), m_callback(callback) {
        std::cout << "V3" << std::endl;
    };
    ~Callback() = default;

    T_Return process(Args... args) {
        // TODO
    }

private:
    C_Object* m_object;
    std::function<T_Return(Args...)> m_callback;
};

//template <typename ...Args>
//class A {
//public:
//    A() {
//        std::cout << "V1" << std::endl;
//    };
//};
//
//template <class C_Object, typename ...Args>
//class A<C_Object, Args...> {
//public:
//    A(int a) {
//        std::cout << "V2" << std::endl;
//    };
//};

#endif /* Callback_h */
