//
//  Callback.hpp
//  Maze
//
//  Created by Jaraxus on 12/03/2020.
//

#ifndef Callback_h
# define Callback_h

# include <functional>
# include <iostream>
# include <memory>
# include <optional>
# include <tuple>

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

template <class T> // TODO: class F
class Callback {
    
    using Return_type           = typename function_trait<T>::return_type;
    using Function_type         = typename function_trait<T>::function_type;
    using Args_container_type   = typename function_trait<T>::args_container_type;
    
public:
    
    Callback(T const& callback) noexcept : m_callback(callback) {}
    
    Callback(T && callback) noexcept : m_callback(std::forward<T>(callback)) {}
    
    Callback(std::function<T> const& callback) : m_callback(static_cast<Function_type>(callback)) {}
    
    Callback(std::function<T> && callback) : m_callback(static_cast<Function_type>(std::forward<std::function<T>>(callback))) {}
    
    template <class Object>
    Callback(T const& callback, Object && object) noexcept : m_callback(std::bind(callback, object)) {}
    
    template <class Object>
    Callback(T && callback, Object && object) noexcept : m_callback(std::bind(callback, object)) {}
    
    template <class Object>
    Callback(std::function<T> const& callback, Object && object) : m_callback(std::bind(static_cast<Function_type>(callback), object)) {}
    
    template <class Object>
    Callback(std::function<T> && callback, Object && object) : m_callback(std::bind(static_cast<Function_type>(std::forward(callback))), object) {}
    
    Callback operator=(T && callback) = delete;
    
    Callback operator=(T const& callback) = delete;
    
    template <typename ...Args>
    Return_type operator()(Args && ...args) const {
        return m_callback(std::forward<Args...>(args)...);
    }
    
    Return_type delayedCall() {
        if (!m_argsSetup)
            throw std::runtime_error("error: delayed callback called without arguments initialized");
        if (!m_sustainableArgs)
            m_argsSetup = false;
        return std::apply(m_callback, m_callbackArgs);
    }
    
    Return_type delayedCall() const {
        if (!m_argsSetup)
            throw std::runtime_error("error: delayed callback called without arguments initialized");
        if (!m_sustainableArgs)
            throw std::runtime_error("error: delayed callback called as constant object but arguments are not sustainable");
        return std::apply(m_callback, m_callbackArgs);
    }
    
    template <typename ...Args>
    void initialize(Args && ...args) {
        m_callbackArgs = std::make_tuple(std::forward<Args>(args)...);
        m_argsSetup = true;
        m_sustainableArgs = false;
    }
    
    template <typename ...Args>
    void initializeSustainably(Args && ...args) {
        m_callbackArgs = std::make_tuple(std::forward<Args>(args)...);
        m_argsSetup = true;
        m_sustainableArgs = true;
    }
    
    template <typename ...Args>
    void precomputeCallback(Args && ...args) const {
        m_callbackResult = std::make_optional(m_callback(std::forward<Args>(args)...));
    }
    
    void precomputeCallback() {
        if (!m_argsSetup) // TODO: assert better ????
            throw std::runtime_error("error: delayed callback called without arguments initialized");
        m_callbackResult = std::make_optional(std::apply(m_callback, m_callbackArgs));
    }
    
private:
    bool                        m_argsSetup = false;
    Function_type               m_callback;
    Args_container_type         m_callbackArgs;
    std::optional<Return_type>  m_callbackResult;
    bool                        m_sustainableArgs = false;
};

}

#endif /* Callback_h */
