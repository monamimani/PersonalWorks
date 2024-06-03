#include <concepts>
#include <tuple>
#include <variant>
#include <vector>

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

/// @brief Code sample from Using Modern C++ to Eliminate Virtual Functions - Jonathan Gopel - CppCon 2022
/// https://www.youtube.com/watch?v=gTNJXVmuRRA

template <typename T>
concept CFoo = requires(T foo) {
  { T::staticFct() } -> std::integral;
  { foo.funcA() } -> std::same_as<int>;
  { foo.funcB() } -> std::integral;
};

template <typename T, typename... Ts>
concept isInTypeList = (... or std::same_as<T, Ts>);

struct Foo
{
  [[nodiscard]] static auto staticFct()
  {
    return 2;
  }
  [[nodiscard]] auto funcA()
  {
    return 42;
  }

  [[nodiscard]] auto funcB()
  {
    return 151;
  }
};

static_assert(CFoo<Foo>);

struct Widget
{
  [[nodiscard]] static auto staticFct()
  {
    return 2;
  }
  [[nodiscard]] auto funcA()
  {
    return 42;
  }

  [[nodiscard]] auto funcB()
  {
    return 151;
  }
};

static_assert(CFoo<Widget>);

void bar([[maybe_unused]] CFoo auto& foo)
{
  [[maybe_unused]] auto a = foo.funcA();
  [[maybe_unused]] auto b = foo.funcB();
}

TEST_CASE("BindingInterface", "StaticDispatch")
{
  Foo foo;
  bar(foo);
}

template <CFoo... Foo_Ts>
class Bar
{
public:
  constexpr Bar(isInTypeList<Foo_Ts...> auto var)
  : m_foo{var}
  {
  }

  constexpr void set(isInTypeList<Foo_Ts...> auto var)
  {
    m_foo = var;
  }

private:
  std::variant<Foo_Ts...> m_foo{};
};

TEST_CASE("OwningPolymorphicTypeVariant", "StaticDispatch")
{
  using Bar_T = Bar<Foo>;
  Foo foo;
  auto bar = Bar_T{foo};
  bar.set(foo);
}

template <CFoo... Foo_Ts>
class BarContainer
{
public:
  template <isInTypeList<Foo_Ts...> T>
  constexpr auto add(T var)
  {
    return std::get<std::vector<T>>(m_tuple).push_back(var);
  }

  void update()
  {
    std::apply(
      [this](auto& ...tuple) {
        (updateTupleElement(tuple), ...);
      }, m_tuple
    );
  }

private:
  void updateTupleElement(auto& tupleElem)
  {
    for (auto& element: tupleElem)
    {
      [[maybe_unused]] auto a = element.funcA();
      [[maybe_unused]] auto b = element.funcB();
    }
  }

  std::tuple<std::vector<Foo_Ts>...> m_tuple{};
};

TEST_CASE("StoringMultipleTypes", "StaticDispatch")
{
  using Bar_T = BarContainer<Foo, Widget>;
  Foo foo;
  auto bar = Bar_T{};
  bar.add(Widget{});
  bar.add(foo);

  bar.update();
}
