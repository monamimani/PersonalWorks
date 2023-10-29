#pragma once

#include<source_location>
#include<stacktrace>

template<typename Type>
class Exception
{
public:
  Exception(const char* str,
                 const Type& data,
                 const std::source_location& loc = std::source_location::current(),
                 std::stacktrace trace = std::stacktrace::current())
  : m_msg(str)
  , m_data(data)
  , m_location{loc}
  , m_stacktrace{trace}
  {}

  Exception(std::string str,
                 Type& data,
                 const std::source_location& loc = std::source_location::current(),
                 std::stacktrace trace = std::stacktrace::current())
  : m_msg(std::move(str))
  , m_data(std::move(data))
  , m_location{loc}
  , m_stacktrace{trace}
  {}

  Type& Data()
  {
    return m_data;
  }

  const Type& Data() const noexcept
  {
    return m_data;
  }

  std::string& What()
  {
    return m_msg;
  }

  const std::string& What() const noexcept
  {
    return m_msg;
  }

  const std::source_location& Where() const
  {
    return m_location;
  }

  const std::stacktrace& Stacktrace() const
  {
    return m_stacktrace;
  }

private:
  std::string m_msg;
  Type m_data;
  const std::source_location m_location;
  const std::stacktrace m_stacktrace;
};

std::ostream& operator<<(std::ostream& os, const std::source_location& location)
{
  os << location.file_name() << "(" << location.line() << ":" << location.column() << "), function `" << location.function_name() << "`";
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::stacktrace& backtrace)
{
  for (auto iter = backtrace.begin(); iter != (backtrace.end() - 3); ++iter)
  {
    os << iter->source_file() << "(" << iter->source_line() << ") : " << iter->description() << "\n";
  }
  return os;
}
