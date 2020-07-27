//
//  Callback.hpp
//  Maze
//
//  Created by Jaraxus on 12/03/2020.
//

#ifndef Callback_h
#define Callback_h

#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <tuple>

#include "TraitsExt.hpp"

namespace mz {

template <class F>
class Callback {
    
    using Return_type           = typename function_trait<F>::return_type;
    using Function_type         = typename function_trait<F>::function_type;
    using Args_container_type   = typename function_trait<F>::args_container_type;
    
public:
    
    Callback(F const& callback) noexcept;
    
    Callback(F && callback) noexcept;
    
    Callback(std::function<F> const& callback) noexcept;
    
    Callback(std::function<F> && callback) noexcept;
    
    template <class Object>
    Callback(F const& callback, Object && object) noexcept;
    
    template <class Object>
    Callback(F && callback, Object && object) noexcept;
    
    template <class Object>
    Callback(std::function<F> const& callback, Object && object) noexcept;
    
    template <class Object>
    Callback(std::function<F> && callback, Object && object) noexcept;
    
    Callback operator=(F && callback) = delete;
    
    Callback operator=(F const& callback) = delete;
    
    template <typename ...Args>
    Return_type operator()(Args && ...args) const;
    
    Return_type delayedCall();
    
    Return_type delayedCall() const;
    
    template <typename ...Args>
    void initialize(Args && ...args);
    
    template <typename ...Args>
    void initializeSustainably(Args && ...args);
    
    template <typename ...Args>
    void precomputeCallback(Args && ...args) const; // TODO: handle empty args
    
    void precomputeCallback();
    
private:
    bool                        m_argsSetup         = false;
    Function_type               m_callback;
    Args_container_type         m_callbackArgs;
    std::optional<Return_type>  m_callbackResult;
    bool                        m_sustainableArgs   = false;
};

}

#include "Callback.inl"

#endif /* Callback_h */
