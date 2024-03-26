#pragma once

#include <ostream>
#include <source_location>
#include <stacktrace>

#include "fmt/format.h"
#include "fmt/ranges.h"

namespace Core
{

class ExceptionBase
{
protected:
  ExceptionBase(std::string&& str, const std::source_location& loc, const std::stacktrace& trace)
  : m_msg(std::move(str))
  , m_location{loc}
  , m_stacktrace{trace}
  {}

public:
  std::string& what()
  {
    return m_msg;
  }

  const std::string& what() const noexcept
  {
    return m_msg;
  }

  const std::source_location& where() const
  {
    return m_location;
  }

  const std::stacktrace& stacktrace() const
  {
    return m_stacktrace;
  }

private:
  std::string m_msg;
  const std::source_location m_location;
  const std::stacktrace m_stacktrace;
};

template<typename Type>
class Exception: public ExceptionBase
{
public:
  Exception(Type& data,
            std::string str,
            const std::source_location& loc = std::source_location::current(),
            const std::stacktrace& trace = std::stacktrace::current())
  : ExceptionBase(std::move(str), loc, trace)
  , m_data(std::move(data))
  {}

  Type& data()
  {
    return m_data;
  }

  const Type& data() const noexcept
  {
    return m_data;
  }

private:
  Type m_data;
};

template<>
class Exception<void>: public ExceptionBase
{
public:
  Exception(std::string str, const std::source_location& loc = std::source_location::current(), const std::stacktrace& trace = std::stacktrace::current())
  : ExceptionBase(std::move(str), loc, trace)
  {}
};

Exception(std::string) -> Exception<void>;

// template<typename Type>
// Exception(const Type&, const char*, const std::source_location&, const std::stacktrace&) -> Exception<Type>;
//
// template<typename Type>
// Exception(const Type&, std::string, const std::source_location&, const std::stacktrace&) -> Exception<Type>;

} // namespace Core

template<>
struct fmt::formatter<std::source_location>: fmt::formatter<std::string_view>
{
  auto format(const std::source_location& location, format_context& ctx) const
  {
    return fmt::format_to(ctx.out(), "{}({},{}), function `{}`\n", location.file_name(), location.line(), location.column(), location.function_name());
  }
};

template<>
struct std::formatter<std::source_location>: std::formatter<std::string_view>
{
  auto format(const std::source_location& location, format_context& ctx) const
  {
    return std::format_to(ctx.out(), "{}({},{}), function `{}`\n", location.file_name(), location.line(), location.column(), location.function_name());
  }
};

template<>
struct fmt::formatter<std::stacktrace_entry>: fmt::formatter<std::string_view>
{
  auto format(const std::stacktrace_entry& entry, format_context& ctx) const
  {
    return fmt::format_to(ctx.out(), "{}({}): {}\n", entry.source_file(), entry.source_line(), entry.description());
  }
};

inline std::ostream& operator<<(std::ostream& os, const std::source_location& location)
{

  os << fmt::format("{}\n", location);
  // os << fmt::format("{}({},{}), function `{}`\n", location.file_name(), location.line(), location.column(), location.function_name());
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const std::stacktrace& backtrace)
{
  os << fmt::format("{}\n", backtrace);

  for (auto iter = backtrace.begin(); iter != backtrace.end(); ++iter)
  {
    os << fmt::format("{}\n", *iter);
    // os << fmt::format("{}({}): {}\n", iter->source_file(), iter->source_line(), iter->description());
  }
  return os;
}
