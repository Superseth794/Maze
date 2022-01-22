//
//  Date.hpp
//  Maze
//
//  Created by Jaraxus on 30/06/2021.
//

#ifndef Date_hpp
#define Date_hpp

#include <array>
#include <chrono>
#include <cstdint>
#include <ostream>
#include <string>

/**
 \brief Pre-increments the day index by 1
 */
/**
 \brief Post-increments the day index by 1
 */
/**
 \brief Pre-decrements the day index by 1
 */
/**
 \brief Post-decrements the day index by 1
 */

namespace mz::Date {

class WeekdayIndexed;
class WeekdayLast;
class YearMonthDayLast;

/**
 \brief tag class indicating the last day or weekday of a month
 */
struct last_spec {
    explicit last_spec() = default;
};
struct local_t {};

using Nanoseeconds  = std::chrono::nanoseconds;
using Microseconds  = std::chrono::microseconds;
using Milliseconds  = std::chrono::microseconds;
using Seconds       = std::chrono::seconds;
using Minutes       = std::chrono::minutes;
using Hours         = std::chrono::hours;
using Days          = std::chrono::duration<long int, std::ratio<86400>>;
using Weeks         = std::chrono::duration<long int, std::ratio<604800>>;
using Months        = std::chrono::duration<long int, std::ratio<2629746>>;
using Years         = std::chrono::duration<long int, std::ratio<31556952>>;

template <typename Duration>
using SystemTime    = std::chrono::time_point<std::chrono::system_clock, Duration>;
using SystemSecond  = SystemTime<Seconds>;
using SystemDays    = SystemTime<Days>;

template <typename Duration>
using LocalTime     = std::chrono::time_point<local_t, Duration>;
using LocalSecond   = LocalTime<Seconds>;
using LocalDays     = LocalTime<Days>;

/**
 \brief The class Day represents a day in a month. Its normal range is [1, 31], but it may hold any number in [0, 255].
 \see https://en.cppreference.com/w/cpp/chrono/day for original specifications
 */
class Day {
public:
    /**
     \brief Constructs a day with given index
     */
    explicit constexpr Day(unsigned index) noexcept;
    
    /**
     \brief Pre-increments the day index by 1
     */
    constexpr Day& operator++() noexcept;
    
    /**
     \brief Post-increments the day index by 1
     */
    constexpr Day operator++(int) noexcept;
    
    /**
     \brief Pre-decrements the day index by 1
     */
    constexpr Day& operator--() noexcept;
    
    /**
     \brief Post-decrements the day index by 1
     */
    constexpr Day operator--(int) noexcept;
    
    /**
     \brief Adds the given number of days to the day index
     \param days number of days to add
     */
    constexpr Day& operator+=(Days const& days) noexcept;
    
    /**
     \brief Removes the given number of days to the day index
     \param days number of days to remove
     */
    constexpr Day& operator-=(Days const& days) noexcept;
    
    /**
     \brief Retrieves the stored value
     */
    explicit constexpr operator unsigned() const noexcept;
    
    /**
     \brief Checks if the current index is valid
     */
    constexpr bool ok() const noexcept;

private:
    std::uint8_t m_index;
    
    static constexpr std::uint8_t s_rangeMin = 1;
    static constexpr std::uint8_t s_rangeMax = 31;
};

/**
 \brief The class Month represents a month in a year. Its normal range is [1, 12], but it may hold any number in [0, 255]. Twelve named constants are predefined in the mz::date namespace for the twelve months of the year.
 \see https://en.cppreference.com/w/cpp/chrono/month for original specifications
 */
class Month {
public:
    /**
     \brief Constructs a month
     */
    explicit constexpr Month(unsigned index) noexcept : m_index(static_cast<std::uint8_t>(index)) {}
    
    /**
     \brief Pre-increments the month index by 1
     */
    constexpr Month& operator++() noexcept;
    
    /**
     \brief Post-increments the month index by 1
     */
    constexpr Month operator++(int) noexcept;
    
    /**
     \brief Pre-decrements the month index by 1
     */
    constexpr Month& operator--() noexcept;
    
    /**
     \brief Post-decrements the month index by 1
     */
    constexpr Month operator--(int) noexcept;
    
    /**
     \brief Adds the given number of months to the month index
     \param months number of months to add
     */
    constexpr Month& operator+=(Months const& months) noexcept;
    
    /**
     \brief Removes the given number of months to the month index
     \param months number of months to remove
     */
    constexpr Month& operator-=(Months const& months) noexcept;
    
    /**
     \brief Retrieves the stored month value
     */
    explicit constexpr operator unsigned() const noexcept;
    
    /**
     \brief Gets the month with the given name
     */
    static inline const Month& fromString(std::string const& name);
    
    /**
     \brief Retrives the name of the month
     */
    constexpr const std::string& getName() const noexcept;
    
    /**
     \brief Retrives the short name of the month
     */
    constexpr const std::string& getShortName() const noexcept;
    
    /**
     \brief checks if the stored value is in the normal range
     */
    constexpr bool ok() const noexcept;
    
private:
    std::uint8_t m_index;
    
    static constexpr std::size_t    s_invalidIndex = 0;
    static constexpr std::size_t    s_rangeMin = 1;
    static constexpr std::size_t    s_rangeMax = 12;
    
public:
    static constexpr std::size_t    NUMBER_OF_MONTHS = s_rangeMax - s_rangeMin + 1;

private:
    static inline std::array<std::string, NUMBER_OF_MONTHS + 1> s_months {
        "not a valid month"
        "Januaray",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "Aout",
        "September",
        "October",
        "November",
        "December"
    };
    static inline std::array<std::string, NUMBER_OF_MONTHS + 1> s_shortMonths = {
        "not a valid month"
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aou",
        "Sep",
        "Oct",
        "Nov",
        "Dec"
    };
};

class Year {
public:
    constexpr Year(int index) noexcept;
    
    constexpr Year& operator++() noexcept;
    
    constexpr Year operator++(int) noexcept;
    
    constexpr Year& operator--() noexcept;
    
    constexpr Year operator--(int) noexcept;
    
    constexpr Year& operator+=(Month const& month) noexcept;
    
    constexpr Year& operator-=(Month const& month) noexcept;
    
    constexpr Year& operator+(Month const& month) noexcept;
    
    constexpr Year& operator-(Month const& month) noexcept;
    
    explicit constexpr operator int() const noexcept;
    
    constexpr bool isLeap() const noexcept;
    
    constexpr bool ok() const noexcept;
    
    static constexpr std::int16_t max() noexcept;
    
    static constexpr std::int16_t min() noexcept;
    
private:
    std::int16_t m_index;
    
    static constexpr std::int16_t maxValue = std::int16_t(32767);
    static constexpr std::int16_t minValue = std::int16_t(-32767);
};

class Weekday {
public:
    constexpr Weekday(unsigned index) noexcept : m_index(static_cast<std::uint8_t>(index)) {}
    
    constexpr Weekday& operator++() noexcept;
    
    constexpr Weekday operator++(int) noexcept;
    
    constexpr Weekday& operator--() noexcept;
    
    constexpr Weekday operator--(int) noexcept;
    
    constexpr Weekday& operator+=(Weekday const& weekday) noexcept;
    
    constexpr Weekday& operator-=(Weekday const& weekday) noexcept;
    
    constexpr WeekdayIndexed operator[](unsigned index) const noexcept;
    
    constexpr WeekdayLast operator[](last_spec /*last_spec*/) const noexcept;
    
    constexpr unsigned cIncoding() const noexcept;
    
    constexpr const std::string& getName() const noexcept;
    
    constexpr const std::string& getShortName() const noexcept;
    
    constexpr unsigned isoEncoding() const noexcept;
    
    constexpr bool ok() const noexcept;

private:
    std::uint8_t m_index;
    
    static constexpr std::size_t s_numberOfMonths = 7;
    static inline std::array<std::string, s_numberOfMonths + 1> weekdays = {
        "not a valid weekday"
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
        "Sunday"
    };
    static inline std::array<std::string, s_numberOfMonths + 1> shortweekdays = {
        "not a valid weekday"
        "Mon",
        "Tue",
        "Wed",
        "Thu",
        "Fri",
        "Sat",
        "Sun"
    };
};

class WeekdayIndexed {
public:
    constexpr WeekdayIndexed(Weekday const& weekday, unsigned index) noexcept;
    
    constexpr const Weekday& getWeekday() const noexcept;
    
    constexpr unsigned getIndex() const noexcept;
    
    constexpr bool ok() const noexcept;
    
private:
    std::uint8_t    m_index;
    const Weekday&  m_weekday;
};

class WeekdayLast {
public:
    explicit constexpr WeekdayLast(Weekday const& weekday) noexcept;
    
    constexpr const Weekday& getWeekday() const noexcept;
    
    constexpr bool ok() const noexcept;
    
private:
    Weekday m_weekday;
};

class MonthDay {
public:
    constexpr MonthDay(Month const& month, Day const& day) noexcept;
    
    constexpr const Day& getDay() const noexcept;
    
    constexpr const Month& getMonth() const noexcept;
    
    constexpr bool ok() const noexcept;
    
private:
    Day     m_day;
    Month   m_month;
};

class MonthDayLast {
public:
    constexpr MonthDayLast(Month const& month) noexcept;
    
    constexpr const Month& getMonth() const noexcept;
    
    constexpr bool ok() const noexcept;
    
private:
    Month m_month;
};

class MonthWeekday {
public:
    constexpr MonthWeekday(Month const& month, WeekdayIndexed const& weekdayIndexed) noexcept;
    
    constexpr const Month& getMonth() const noexcept;
    
    constexpr const WeekdayIndexed& getWeekdayIndexed() const noexcept;
    
    constexpr bool ok() const noexcept;
    
private:
    Month           m_month;
    WeekdayIndexed  m_weekday;
};

class MonthWeekdayLast {
public:
    constexpr MonthWeekdayLast(Month const& month, WeekdayLast const& weekdayLast) noexcept;
    
    constexpr const Month& getMonth() const noexcept;
    
    constexpr const WeekdayLast& getWeekdayLast() const noexcept;
    
    constexpr bool ok() const noexcept;
    
private:
    Month       m_month;
    WeekdayLast m_weekdayLast;
};

class YearMonth {
public:
    constexpr YearMonth(Year const& year, Month const& month) noexcept;
    
    constexpr YearMonth& operator+=(Months const& months) noexcept;
    
    constexpr YearMonth& operator+=(Years const& years) noexcept;

    constexpr YearMonth& operator-=(Months const& Months) noexcept;

    constexpr YearMonth& operator-=(Years const& years) noexcept;
    
    constexpr const Month& getMonth() const noexcept;
    
    constexpr const Year& getYear() const noexcept;
    
private:
    Year    m_year;
    Month   m_month;
};

class YearMonthDay {
public:
    constexpr YearMonthDay(Year const& year, Month const& month, Day const& day) noexcept;
    
    constexpr YearMonthDay(YearMonthDayLast const& yearMonthDayLast) noexcept;
    
    constexpr YearMonthDay(SystemDays const& days) noexcept;
    
    constexpr YearMonthDay(LocalDays const days) noexcept;
    
    constexpr YearMonth& operator+=(Months const& months) noexcept;
    
    constexpr YearMonth& operator+=(Years const& years) noexcept;
    
    constexpr YearMonth& operator-=(Months const& Months) noexcept;

    constexpr YearMonth& operator-=(Years const& years) noexcept;
    
    constexpr Day getDay() const noexcept;
    
    constexpr Month getMonth() const noexcept;
    
    constexpr Year getYear() const noexcept;
    
    constexpr bool ok() const noexcept;
    
    constexpr LocalDays toLocalDays() const noexcept;
    
    constexpr SystemDays toSystemDays() const noexcept;
    
private:
    Day     m_day;
    Month   m_month;
    Year    m_year;
};

class YearMonthDayLast {
public:
    constexpr YearMonthDayLast(Year const& year, MonthDayLast const& monthDayLast) noexcept;
    
    constexpr YearMonthDayLast& operator+=(Months const& months) noexcept;
    
    constexpr YearMonthDayLast& operator+=(Years const& years) noexcept;
    
    constexpr YearMonthDayLast& operator-=(Months const& Months) noexcept;

    constexpr YearMonthDayLast& operator-=(Years const& years) noexcept;
    
    constexpr Day getDay() const noexcept;
    
    constexpr Month getMonth() const noexcept;
    
    constexpr MonthDayLast getMonthDayLast() const noexcept;
    
    constexpr Year getYear() const noexcept;
    
    constexpr bool ok() const noexcept;
    
    constexpr LocalDays toLocalDays() const noexcept;
    
    constexpr SystemDays toSystemDays() const noexcept;
    
private:
    MonthDayLast    m_monthDayLast;
    Year            m_year;
};

class YearMonthWeekday {
public:
    constexpr YearMonthWeekday(Year const& year, Month const& month, Weekday const& weekday) noexcept;
    
    constexpr YearMonthWeekday(LocalDays const& localDays) noexcept;
    
    constexpr YearMonthWeekday(SystemDays const& systemDays) noexcept;
    
    constexpr YearMonthWeekday& operator+=(Months const& months) noexcept;
    
    constexpr YearMonthWeekday& operator+=(Years const& years) noexcept;
    
    constexpr YearMonthWeekday& operator-=(Months const& Months) noexcept;

    constexpr YearMonthWeekday& operator-=(Years const& years) noexcept;
    
    constexpr unsigned getIndex() const noexcept;
    
    constexpr const Month& getMonth() const noexcept;
    
    constexpr WeekdayIndexed getWeekdayIndexed() const noexcept;
    
    constexpr const Weekday& getWeekday() const noexcept;
    
    constexpr const Year& getYear() const noexcept;
    
    constexpr bool ok() const noexcept;
    
    constexpr LocalDays toLocalDays() const noexcept;
    
    constexpr SystemDays toSystemDays() const noexcept;
    
private:
    Month   m_month;
    Weekday m_weekday;
    Year    m_year;
};

class YearMonthWeekdayLast {
public:
    constexpr YearMonthWeekdayLast(Year const& year, Month const& month, Day const& day) noexcept;
    
    constexpr YearMonthWeekdayLast& operator+=(Months const& months) noexcept;
    
    constexpr YearMonthWeekdayLast& operator+=(Years const& years) noexcept;
    
    constexpr YearMonthWeekdayLast& operator-=(Months const& Months) noexcept;

    constexpr YearMonthWeekdayLast& operator-=(Years const& years) noexcept;
    
    constexpr const Month& getMonth() const noexcept;
    
    constexpr const Weekday& getWeekday() const noexcept;
    
    constexpr const WeekdayLast& getWeekdayLast() const noexcept;
    
    constexpr const Year& getYear() const noexcept;
    
    constexpr bool ok() const noexcept;
    
    constexpr LocalDays toLocalDays() const noexcept;
    
    constexpr SystemDays toSystemDays() const noexcept;
    
private:
    Month   m_month;
    Weekday m_weekday;
    Year    m_year;
};

constexpr bool operator==(Day const& lhs, Day const& rhs) noexcept;

constexpr bool operator==(Month const& lhs, Month const& rhs) noexcept;

constexpr bool operator==(Year const& lhs, Year const& rhs) noexcept;

constexpr bool operator==(Weekday const& lhs, Weekday const& rhs) noexcept;

constexpr bool operator==(WeekdayIndexed const& lhs, WeekdayIndexed const& rhs) noexcept;

constexpr bool operator==(WeekdayLast const& lhs, WeekdayLast const& rhs) noexcept;

constexpr bool operator==(MonthDay const& lhs, MonthDay const& rhs) noexcept;

constexpr bool operator==(MonthDayLast const& lhs, MonthDayLast const& rhs) noexcept;

constexpr bool operator==(MonthWeekday const& lhs, MonthWeekday const& rhs) noexcept;

constexpr bool operator==(MonthWeekdayLast const& lhs, MonthWeekdayLast const& rhs) noexcept;

constexpr bool operator==(YearMonth const& lhs, YearMonth const& rhs) noexcept;

constexpr bool operator==(YearMonthDay const& lhs, YearMonthDay const& rhs) noexcept;

constexpr bool operator==(YearMonthDayLast const& lhs, YearMonthDayLast const& rhs) noexcept;

constexpr bool operator==(YearMonthWeekday const& lhs, YearMonthWeekday const& rhs) noexcept;

constexpr bool operator==(YearMonthWeekdayLast const& lhs, YearMonthWeekdayLast const& rhs) noexcept;

constexpr Day operator+(Day const& lhs, Day const& rhs) noexcept;

constexpr Month operator+(Month const& lhs, Month const& rhs) noexcept;

constexpr Year operator+(Year const& lhs, Year const& rhs) noexcept;

constexpr Year operator+(Weekday const& lhs, Weekday const& rhs) noexcept;

constexpr YearMonth operator+(YearMonth const& yearMonth, Months const& months) noexcept;

constexpr YearMonth operator+(Month const& months, YearMonth const& yearMonth) noexcept;

constexpr YearMonth operator+(YearMonth const& yearMonth, Years const& years) noexcept;

constexpr YearMonth operator+(Years const& years, YearMonth const& yearMonth) noexcept;

constexpr YearMonthDay operator+(YearMonthDay const& yearMonthDay, Months const& months) noexcept;

constexpr YearMonthDay operator+(Month const& months, YearMonthDay const& yearMonthDay) noexcept;

constexpr YearMonthDay operator+(YearMonthDay const& yearMonthDay, Years const& years) noexcept;

constexpr YearMonthDay operator+(Years const& years, YearMonthDay const& yearMonthDay) noexcept;

constexpr YearMonthDayLast operator+(YearMonthDayLast const& yearMonthDayLast, Months const& months) noexcept;

constexpr YearMonthDayLast operator+(Month const& months, YearMonthDayLast const& yearMonthDayLast) noexcept;

constexpr YearMonthDayLast operator+(YearMonthDayLast const& yearMonthDayLast, Years const& years) noexcept;

constexpr YearMonthDayLast operator+(Years const& years, YearMonthDayLast const& yearMonthDayLast) noexcept;

constexpr YearMonthWeekday operator+(YearMonthWeekday const& yearMonthWeekday, Months const& months) noexcept;

constexpr YearMonthWeekday operator+(Month const& months, YearMonthWeekday const& yearMonthWeekday) noexcept;

constexpr YearMonthWeekday operator+(YearMonthWeekday const& yearMonthWeekday, Years const& years) noexcept;

constexpr YearMonthWeekday operator+(Years const& years, YearMonthWeekday const& yearMonthWeekday) noexcept;

constexpr YearMonthWeekdayLast operator+(YearMonthWeekdayLast const& yearMonthWeekdayLast, Months const& months) noexcept;

constexpr YearMonthWeekdayLast operator+(Month const& months, YearMonthWeekdayLast const& yearMonthWeekdayLast) noexcept;

constexpr YearMonthWeekdayLast operator+(YearMonthWeekdayLast const& yearMonthWeekdayLast, Years const& years) noexcept;

constexpr YearMonthWeekdayLast operator+(Years const& years, YearMonthWeekdayLast const& yearMonthWeekdayLast) noexcept;

constexpr Day operator-(Day const& lhs, Day const& rhs) noexcept;

constexpr Month operator-(Month const& lhs, Month const& rhs) noexcept;

constexpr Year operator-(Year const& lhs, Year const& rhs) noexcept;

constexpr Month operator-(Weekday const& lhs, Weekday const& rhs) noexcept;

constexpr YearMonth operator-(YearMonth const& yearMonth, Months const& months) noexcept;

constexpr YearMonth operator-(YearMonth const& yearMonth, Years const& years) noexcept;

constexpr Months operator-(YearMonth const& rhs, YearMonth const& lhs) noexcept;

constexpr YearMonthDay operator-(YearMonthDay const& yearMonthDay, Months const& months) noexcept;

constexpr YearMonthDay operator-(YearMonthDay const& yearMonthDay, Years const& years) noexcept;

constexpr YearMonthDayLast operator-(YearMonthDayLast const& yearMonthDayLast, Months const& months) noexcept;

constexpr YearMonthDayLast operator-(YearMonthDayLast const& yearMonthDayLast, Years const& years) noexcept;

constexpr YearMonthWeekday operator-(YearMonthWeekday const& yearMonthWeekday, Months const& months) noexcept;

constexpr YearMonthWeekday operator-(YearMonthWeekday const& yearMonthWeekday, Years const& years) noexcept;

constexpr YearMonthWeekdayLast operator-(YearMonthWeekdayLast const& yearMonthWeekdayLast, Months const& months) noexcept;

constexpr YearMonthWeekdayLast operator-(YearMonthWeekdayLast const& yearMonthWeekdayLast, Years const& years) noexcept;

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Day const& day);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Month const& month);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Year const& year);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, Weekday const& weekday);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, WeekdayIndexed const& weekdayIndexed);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, WeekdayLast const& weekdayLast);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, MonthDay const& monthDay);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, MonthDayLast const& monthDayLast);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, MonthWeekday const& monthDayLast);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, MonthWeekdayLast const& monthDayLast);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, YearMonth const& yearMonth);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, YearMonthDay const& yearMonthDay);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, YearMonthDayLast const& yearMonthDayLast);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, YearMonthWeekday const& yearMonthWeekday);

template <typename CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, YearMonthWeekdayLast const& yearMonthWeekdayLast);

// TODO: https://en.cppreference.com/w/cpp/chrono/month/from_stream

constexpr auto operator/(const Year& y,
                         const Month& m) noexcept
    -> YearMonth;

constexpr auto operator/(const Year& y, int m) noexcept
    -> YearMonth;

constexpr auto operator/(const Month& m,
                         const Day& d) noexcept
    -> MonthDay;

constexpr auto operator/(const Month& m, int d) noexcept
    -> MonthDay;

constexpr auto operator/(int m, const Day& d) noexcept
    -> MonthDay;

constexpr auto operator/(const Day& d,
                         const Month& m) noexcept
   -> MonthDay;

constexpr auto operator/(const Day& d, int m) noexcept
    -> MonthDay;

constexpr auto operator/(const Month& m,
                         last_spec) noexcept
    -> MonthDayLast;

constexpr auto operator/(int m, last_spec) noexcept
    -> MonthDayLast;

constexpr auto operator/(last_spec,
                         const Month& m) noexcept
    -> MonthDayLast;

constexpr auto operator/(last_spec, int m) noexcept
    -> MonthDayLast;

constexpr auto operator/(const Month& m,
                         const WeekdayIndexed& wdi) noexcept
    -> MonthWeekday;

constexpr auto operator/(int m, const WeekdayIndexed& wdi) noexcept
    -> MonthWeekday;

constexpr auto operator/(const WeekdayIndexed& wdi,
                         const Month& m) noexcept
    -> MonthWeekday;

constexpr auto operator/(const WeekdayIndexed& wdi, int m) noexcept
    -> MonthWeekday;

constexpr auto operator/(const Month& m,
                         const WeekdayLast& wdl) noexcept
    -> MonthWeekdayLast;

constexpr auto operator/(int m, const WeekdayLast& wdl) noexcept
    -> MonthWeekdayLast;

constexpr auto operator/(const WeekdayLast& wdl,
                         const Month& m) noexcept
    -> MonthWeekdayLast;

constexpr auto operator/(const WeekdayLast& wdl, int m) noexcept
    -> MonthWeekdayLast;

constexpr auto operator/(const YearMonth& ym,
                         const Day& d) noexcept
    -> YearMonthDay;

constexpr auto operator/(const YearMonth& ym, int d) noexcept
    -> YearMonthDay;

constexpr auto operator/(const Year& y,
                         const MonthDay& md) noexcept
    -> YearMonthDay;

constexpr auto operator/(int y, const MonthDay& md) noexcept
    -> YearMonthDay;

constexpr auto operator/(const MonthDay& md,
                         const Year& y) noexcept
    -> YearMonthDay;

constexpr auto operator/(const MonthDay& md, int y) noexcept
    -> YearMonthDay;

constexpr auto operator/(const YearMonth& ym,
                         last_spec) noexcept
    -> YearMonthDayLast;

constexpr auto operator/(const Year& y,
                         const MonthDayLast& mdl) noexcept
    -> YearMonthDayLast;

constexpr auto operator/(int y, const MonthDayLast& mdl) noexcept
    -> YearMonthDayLast;

constexpr auto operator/(const MonthDayLast& mdl,
                         const Year& y) noexcept
    -> YearMonthDayLast;

constexpr auto operator/(const MonthDayLast& mdl, int y) noexcept
    -> YearMonthDayLast;

constexpr auto operator/(const YearMonth& ym,
                         const WeekdayIndexed& wdi) noexcept
    -> YearMonthWeekday;

constexpr auto operator/(const Year& y,
                         const MonthWeekday& mwd) noexcept
    -> YearMonthWeekday;

constexpr auto operator/(int y, const MonthWeekday& mwd) noexcept
    -> YearMonthWeekday;

constexpr auto operator/(const MonthWeekday& mwd,
                         const Year& y) noexcept
    -> YearMonthWeekday;

constexpr auto operator/(const MonthWeekday& mwd, int y) noexcept
    -> YearMonthWeekday;

constexpr auto operator/(const YearMonth& ym,
                         const WeekdayLast& wdl) noexcept
    -> YearMonthWeekdayLast;

constexpr auto operator/(const Year& y,
                         const MonthWeekdayLast& mwdl) noexcept
    -> YearMonthWeekdayLast;

constexpr auto operator/(int y, const MonthWeekdayLast& mwdl) noexcept
    -> YearMonthWeekdayLast;

constexpr auto operator/(const MonthWeekdayLast& mwdl,
                         const Year& y) noexcept
    -> YearMonthWeekdayLast;

constexpr auto operator/(const MonthWeekdayLast& mwdl, int y) noexcept
    -> YearMonthWeekdayLast;

namespace literals {

constexpr Day operator "" _d(unsigned long long d) noexcept {
    return Day(static_cast<unsigned>(d));
}

constexpr Month operator "" _m(unsigned long long d) noexcept {
    return Month(static_cast<unsigned>(d));
}

constexpr Year operator "" _y(unsigned long long d) noexcept {
    return Year(static_cast<int>(d));
}

} // literals

static constexpr last_spec last     {};

static constexpr Month InvalidMonth {0};
static constexpr Month January      {1};
static constexpr Month February     {2};
static constexpr Month March        {3};
static constexpr Month April        {4};
static constexpr Month May          {5};
static constexpr Month June         {6};
static constexpr Month July         {7};
static constexpr Month Aout         {8};
static constexpr Month September    {9};
static constexpr Month October      {10};
static constexpr Month November     {11};
static constexpr Month December     {12};

static constexpr auto AllMonths = std::array {
    January,
    February,
    March,
    April,
    May,
    June,
    July,
    Aout,
    September,
    October,
    November,
    December
};

static constexpr Weekday Monday     {1};
static constexpr Weekday Tueday     {2};
static constexpr Weekday Wednesday  {3};
static constexpr Weekday Tuesday    {4};
static constexpr Weekday Friday     {5};
static constexpr Weekday Saturday   {6};
static constexpr Weekday Sunday     {7};

} // mz::date

#include "Date.inl"

#endif /* Date_hpp */
 
