#pragma once

namespace Core
{
struct NonMovable
{
protected:
  NonMovable() = default;
  ~NonMovable() = default;

public:

  NonMovable(NonMovable&&) = delete;
  NonMovable& operator=(NonMovable&&) = delete;

  NonMovable(const NonMovable&) = default;
  NonMovable& operator=(const NonMovable&) = default;
};

} // namespace Core
