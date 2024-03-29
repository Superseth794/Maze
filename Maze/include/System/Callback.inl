//
//  Callback.inl
//  Maze
//
//  Created by Jaraxus on 27/07/2020.
//

namespace mz {

template <class F>
Callback<F>::Callback(F const& callback) noexcept :
m_callback(callback)
{
}

template <class F>
Callback<F>::Callback(F && callback) noexcept :
m_callback(std::forward<F>(callback))
{
}

template <class F>
Callback<F>::Callback(std::function<F> const& callback) noexcept :
m_callback(static_cast<Function_type>(callback))
{
}

template <class F>
Callback<F>::Callback(std::function<F> && callback) noexcept :
m_callback(static_cast<Function_type>(std::forward<std::function<F>>(callback))),
m_callbackArgs(),
m_callbackResult() // TODO: fix
{
}

template <class F> template <class Object>
Callback<F>::Callback(F const& callback, Object && object) noexcept :
m_callback(std::bind(callback, object))
{
}

template <class F> template <class Object>
Callback<F>::Callback(F && callback, Object && object) noexcept :
m_callback(std::bind(callback, object))
{
}

template <class F> template <class Object>
Callback<F>::Callback(std::function<F> const& callback, Object && object) noexcept :
m_callback(std::bind(static_cast<Function_type>(callback), object))
{
}

template <class F> template <class Object>
Callback<F>::Callback(std::function<F> && callback, Object && object) noexcept :
m_callback(std::bind(static_cast<Function_type>(std::forward(callback))), object)
{
}

template<typename F> template <typename ...Args>
typename Callback<F>::Return_type Callback<F>::operator()(Args && ...args) const {
    static_assert(std::is_invocable_v<Function_type, Args...>, "error: Callback must be invokable with given arguments");
    return m_callback(std::forward<Args>(args)...);
}

template<typename F>
typename Callback<F>::Return_type Callback<F>::delayedCall() {
    if (!m_argsSetup)
        throw std::runtime_error("error: delayed callback called without arguments initialized");
    if (!m_sustainableArgs)
        m_argsSetup = false;
    return std::apply(m_callback, m_callbackArgs);
}

template<typename F>
typename Callback<F>::Return_type Callback<F>::delayedCall() const {
    if (!m_argsSetup)
        throw std::runtime_error("error: delayed callback called without arguments initialized");
    if (!m_sustainableArgs)
        throw std::runtime_error("error: delayed callback called as constant object but arguments are not sustainable");
    return std::apply(m_callback, m_callbackArgs);
}

template<typename F> template <typename ...Args>
void Callback<F>::initialize(Args && ...args) {
    m_callbackArgs = std::make_tuple(std::forward<Args>(args)...);
    m_argsSetup = true;
    m_sustainableArgs = false;
}

template<typename F> template <typename ...Args>
void Callback<F>::initializeSustainably(Args && ...args) {
    m_callbackArgs = std::make_tuple(std::forward<Args>(args)...);
    m_argsSetup = true;
    m_sustainableArgs = true;
}

template<typename F> template <typename ...Args>
void Callback<F>::precomputeCallback(Args && ...args) {
    constexpr bool isReturnType = std::is_same_v<Return_type, void>;
    if constexpr (sizeof...(args) != 0 || std::tuple_size<Args_container_type>::value == 0) {
        static_assert(std::is_invocable_v<Function_type, Args...>, "error: Callback must be invokable with given arguments");
        if constexpr (isReturnType)
            m_callback(std::forward<Args>(args)...);
        else
            m_callbackResult = std::make_optional(m_callback(std::forward<Args>(args)...));
    } else { // use initialised args
        if (!m_argsSetup)
            throw std::runtime_error("error: delayed callback called without arguments initialized");
        if constexpr (isReturnType)
            m_callback(std::forward<Args>(args)...);
        else
            m_callbackResult = std::make_optional(std::apply(m_callback, m_callbackArgs));
        if (!m_sustainableArgs)
            m_argsSetup = false;
    }
}

}
