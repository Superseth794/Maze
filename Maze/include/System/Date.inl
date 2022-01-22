//
//  Date.inl
//  Maze
//
//  Created by Jaraxus on 30/06/2021.
//

#ifndef Date_inl
#define Date_inl

namespace mz::Date {

constexpr Day::Day(unsigned index) noexcept :
m_index(static_cast<std::uint8_t>(index))
{
}

constexpr Day& Day::operator++() noexcept {
    ++m_index;
    return *this;
}

constexpr Day Day::operator++(int) noexcept {
    Day day = *this;
    m_index++;
    return day;
}

constexpr Day& Day::operator--() noexcept {
    --m_index;
    return *this;
}

constexpr Day Day::operator--(int) noexcept {
    Day day = *this;
    --m_index;
    return day;
}

constexpr Day& Day::operator+=(Days const& days) noexcept {
    m_index += days.count();
    return *this;
}

constexpr Day& Day::operator-=(Days const& days) noexcept {
    m_index -= days.count();
    return *this;
}

constexpr Day::operator unsigned() const noexcept {
    return static_cast<unsigned>(m_index);
}

constexpr bool Day::ok() const noexcept {
    return (s_rangeMin <= m_index && m_index <= s_rangeMax);
}

constexpr Month& Month::operator++() noexcept {
    ++m_index;
    return *this;
}

constexpr Month Month::operator++(int) noexcept {
    Month month = *this;
    ++m_index;
    return month;
}

constexpr Month& Month::operator--() noexcept {
    --m_index;
    return *this;
}

constexpr Month Month::operator--(int) noexcept {
    Month month = *this;
    --m_index;
    return month;
}

constexpr Month& Month::operator+=(Months const& months) noexcept {
    m_index += months.count();
    return *this;
}

constexpr Month& Month::operator-=(Months const& months) noexcept {
    m_index -= months.count();
    return *this;
}

constexpr Month::operator unsigned() const noexcept {
    return static_cast<unsigned>(m_index);
}

inline const Month& Month::fromString(std::string const& name) {
    for (std::size_t i = s_rangeMin; i <= s_rangeMax; ++i) {
        if (name == s_months[i] || name == s_shortMonths[i])
            return mz::Date::AllMonths[i];
    }
    return InvalidMonth;
}

constexpr const std::string& Month::getName() const noexcept {
    return (ok() ? s_months[m_index] : s_months[s_invalidIndex]);
}

constexpr const std::string& Month::getShortName() const noexcept {
    return (ok() ? s_shortMonths[m_index] : s_shortMonths[s_invalidIndex]);
}

constexpr bool Month::ok() const noexcept {
    return (s_rangeMin <= m_index && m_index <= s_rangeMax);
}

} // mz::date

#endif /* Date_inl */
