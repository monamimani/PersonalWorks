#pragma once

#include <concepts>
#include <type_traits>

#ifdef _MSC_VER
  #define NOINLINE __declspec(noinline)
#elif __clang__
  #define NOINLINE [[clang::noinline]]
#elif __GNUC__
  #define NOINLINE [[gnu::noinline]]
#else
  #error Compiler not supported.
#endif

namespace Test
{

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

struct BMStruct
{
  constexpr static int staticValue = 420;
  int value = staticValue;

  inline void fct(int& x)
  {
    x = 0;
    for (int i = 0; i < value; i++)
    {
      x++;
    }
  }

  inline void fctConst(int& x) const
  {
    x = 0;
    for (int i = 0; i < value; i++)
    {
      x++;
    }
  }

  inline void fctConstOverloaded(int& x)
  {
    x = 0;
    for (int i = 0; i < value; i++)
    {
      x++;
    }
  }

  inline void fctConstOverloaded(int& x) const
  {
    x = 0;
    for (int i = 0; i < value; i++)
    {
      x++;
    }
  }

  inline void fctParamOverloaded(int& x) const
  {
    x = 0;
    for (int i = 0; i < value; i++)
    {
      x++;
    }
  }

  inline void fctParamOverloaded(double& x) const
  {
    x = 0;
    for (int i = 0; i < value; i++)
    {
      x++;
    }
  }

  inline void fctTemplate(Number auto& x) const
  {
    x = 0;
    for (int i = 0; i < value; i++)
    {
      x++;
    }
  }

  inline void operator()(int& x)
  {
    x = 0;
    for (int i = 0; i < value; i++)
    {
      x++;
    }
  }

  inline void operator()(int& x) const
  {
    x = 0;
    for (int i = 0; i < value; i++)
    {
      x++;
    }
  }
};

static inline void freeFunction(int& x)
{
  x = 0;
  for (int i = 0; i < BMStruct::staticValue; i++)
  {
    x++;
  }
}

static auto lambdaGlobal = [](auto& x)
{
  x = 0;
  for (int i = 0; i < BMStruct::staticValue; i++)
  {
    x++;
  }
};

static auto lambdaStatefullGlobal = [bmStruct = BMStruct{}](auto& x)
{
  bmStruct(x);
};

struct BMStructOutOfLine
{
  constexpr static int staticValue = 420;
  int value = staticValue;

  void fct(int& x);
  void fctConst(int& x) const;

  void fctConstOverloaded(int& x);
  void fctConstOverloaded(int& x) const;

  void fctParamOverloaded(int& x) const;
  void fctParamOverloaded(double& x) const;

  void operator()(int& x);
  void operator()(int& x) const;
};

void freeFunctionOutOfLine(int& x);

} // namespace Test