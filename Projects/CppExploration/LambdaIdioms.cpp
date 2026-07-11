#include <format>
#include <functional>
#include <iostream>
#include <variant>
#include <vector>

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

/// @brief Code sample from C++ Lambda Idioms - Timur Doumler - CppCon 2022
/// https://youtu.be/xBAduq0RGes?list=PLHTh1InhhwT6c2JNtUiJkaH8YRqzhU7Ag

TEST_CASE("UnaryPlus", "LambdaIdioms")
{
  // Using the Unary plus operator to convert a lambda to a function pointer;
  [[maybe_unused]] auto* fptr = +[](int i) {
    return i * i;
  }; // Doesn't compiler without the + because error : cannot deduce type for 'auto *' from '"LambdaIdioms"_UnaryPlus_TEST_CASE::TEST_CASEBody::<lambda_1>'
  [[maybe_unused]] auto* fptr2 = static_cast<int (*)(int)>([](int i) {
    return i * i;
  });
}

TEST_CASE("ODRUsed", "LambdaIdioms")
{
  // Static/Global variable are not capturable buy lambda capture. You can access them and modify them.
  // you only capture things that are ODR-used inside the lambda
  constexpr int i = 42;
  [[maybe_unused]] auto f = []() {
    std::cout << i;
  }; // it is not an ODR-used because it is known at compile-time.
}

struct Foo
{};

Foo createFooA()
{
  return {};
}

Foo createFooB()
{
  return {};
}

TEST_CASE("IIFE", "LambdaIdioms")
{
  // Immediately Invoked Function Expression (IIFE)
  // Immediately Invoked Lambda
  [] {
    std::cout << "Hello World\n";
  }();

  bool someCondition = true;
  [[maybe_unused]] const auto foo = [someCondition] {
    if (someCondition)
    {
      return createFooA();
    }
    else
    {
      return createFooB();
    }
  }();

  std::vector<Foo> foos;
  foos.emplace_back([someCondition] {
    if (someCondition)
    {
      return createFooA();
    }
    else
    {
      return createFooB();
    }
  }());

  // or be more explicit about the invocation.
  foos.emplace_back(std::invoke([someCondition] {
    if (someCondition)
    {
      return createFooA();
    }
    else
    {
      return createFooB();
    }
  }));
}

TEST_CASE("CalledOnce", "LambdaIdioms")
{
  // Lambda that is called only once
  struct X
  {
    X()
    {
      [[maybe_unused]] static auto _ = [] {
        std::cout << "called once!\n";
        return 0;
      }();
      // apparently it is faster than std called once.
    }
  };

  X x1;
  X x2;
  X x3;
}

template<typename T>
constexpr auto cCast = [](auto x) {
  return static_cast<T>(x);
};

TEST_CASE("VariableTemplateLambda", "LambdaIdioms")
{
  float f = 42.5f;
  [[maybe_unused]] auto d = cCast<double>(f);
}

TEST_CASE("InitCaptureOptimisation", "LambdaIdioms")
{
  const std::vector<std::string> vs = {"apple", "orange", "foobar", "lemon"};
  const std::string prefix = "foo";

  [[maybe_unused]] auto resultA = std::find_if(vs.begin(), vs.end(), [&prefix](const std::string& s) {
    return s == prefix + "bar"; // The issue is that the string concatenation will be done for each invocation of the lambda
  });

  [[maybe_unused]] auto resultB = std::find_if(vs.begin(), vs.end(), [str = prefix + "bar" /*computed only once*/](const std::string& s) {
    return s == str;
  });
}

template<typename... Ts>
struct OverloadSet: Ts...
{
  using Ts::operator()...;
};

TEST_CASE("LambdaOverloadSet", "LambdaIdioms")
{
  OverloadSet f = {[]([[maybe_unused]] int i) {
                     std::cout << "int thingy\n";
                   },
                   []([[maybe_unused]] float i) {
                     std::cout << "float thingy\n";
                   }};

  f(2);
  f(2.0f);

  std::variant<int, float> v = 2.0f;
  std::visit(f, v); // prints float thingy
}

void myDeleter(Foo* f)
{
  delete f;
}

template<typename T>
using MyPtr = std::unique_ptr<T, decltype([](T* t) {
                                myDeleter(t);
                              })>;

TEST_CASE("DefaultConstructible", "LambdaIdioms")
{
  MyPtr<Foo> ptr;
}

template<auto =
             [] {
             }>
struct X
{};

TEST_CASE("NewLambdaExpression", "LambdaIdioms")
{
  // Every new lambda expression generated a new closure type.
  {
    [[maybe_unused]] auto f1 = [] {
    };
    [[maybe_unused]] auto f2 = [] {
    };
    static_assert(not std::same_as<decltype(f1), decltype(f2)>);
    // f1, and f2 have different types, because they are 2 differents expressions
  }

  {
    [[maybe_unused]] auto f1 = [] {
    };
    [[maybe_unused]] auto f2 = f1;
    static_assert(std::same_as<decltype(f1), decltype(f2)>);
    // f1, and f2 have same types
  }

  {
    using T = decltype([] {
    });
    [[maybe_unused]] T f1;
    [[maybe_unused]] T f2;
    static_assert(std::same_as<decltype(f1), decltype(f2)>);
    // f1, and f2 have same types
  }

  {
    [[maybe_unused]] decltype([] {
    }) f1;
    [[maybe_unused]] decltype([] {
    }) f2;
    static_assert(not std::same_as<decltype(f1), decltype(f2)>);
    // f1, and f2 have differents types
  }

  {
    [[maybe_unused]] X x1;
    [[maybe_unused]] X x2;
    static_assert(not std::same_as<decltype(x1), decltype(x2)>);
    // f1, and f2 have differents types, because each of the Xs are a new template instanciation and the template paramter have a default value
  }
}

TEST_CASE("UniqueTypesGenerator", "LambdaIdioms")
{

  {
    [[maybe_unused]] X x1;
    [[maybe_unused]] X x2;
    static_assert(not std::same_as<decltype(x1), decltype(x2)>);
    // f1, and f2 have differents types, because each of the Xs are a new template instanciation and the template paramter have a default value

    // usefull to generate new types instead of using __Counter__
    // for example registration
  }
}

TEST_CASE("RecursiveLambdas", "LambdaIdioms")
{
  auto factorial = [](this auto&& self, int i) {
    if (i == 0)
    {
      return 1;
    }

    return i * self(i - 1);
  };

  auto value = factorial(5);

  REQUIRE(value == 120);
}

struct Leaf
{};
struct Node;

using Tree = std::variant<Leaf, Node*>;

struct Node
{
  Tree left, right;
};

int countLeaves(const Tree& tree)
{
  OverloadSet f = {[](const Leaf&) {
                     return 1;
                   },
                   [](this const auto& self, const Node* node) -> int {
                     return std::visit(self, node->left) + std::visit(self, node->right);
                   }};
  return std::visit(f, tree);
}

TEST_CASE("RecursiveLambdaOverloadSet", "LambdaIdioms")
{
  // countLeaves
}

////////////////////////////////////////////////

struct TestStruct
{
  inline void fctConstOverloaded(int& x)
  {
    x = 42;
  }

  inline void fctConstOverloaded(int& x) const
  {
    x = 43;
  }

  inline void fctParamOverloaded(int& x)
  {
    x = 43;
  }

  inline void fctParamOverloaded(int& x) const
  {
    x = 43;
  }

  inline void fctParamOverloaded(double& x)
  {
    x = 43;
  }

  inline void fctParamOverloaded(double& x) const
  {
    x = 43;
  }

  inline void fctTemplate(auto& x)
  {
    x = 42;
  }

  inline void fctTemplate(auto& x) const
  {
    x = 42;
  }
};

template<typename Instance_T, typename Ret, typename... Args>
consteval decltype(auto) asFnPtr_freeFunction(Ret (Instance_T::*fct)(Args...))
{
  return fct;
};

template<typename Signature>
class Widget;

template<typename Ret, typename... Args>
class Widget<Ret(Args...)>
{
public:
  static inline decltype(auto) asFnPtr = []<typename Instance_T, typename... ArgBs>(Ret (Instance_T::*fct)(ArgBs...)) consteval {
    return fct;
  };

  static inline decltype(auto) asFnConstPtr = []<typename Instance_T, typename... ArgBs>(Ret (Instance_T::*fct)(ArgBs...) const) consteval {
    return fct;
  };

  template<typename... ArgBs>
  static inline decltype(auto) asFnParamPtr = []<typename Instance_T>(Ret (Instance_T::*fct)(ArgBs...)) consteval {
    return fct;
  };

  // template <typename... Args>
  // static inline decltype(auto) asFnConstParamPtr = []<typename Instance_T, typename Ret>(Ret(Instance_T::*fct)(Args...) const) consteval
  //{
  //   return fct;
  // };
};

using Widget_T = Widget<void(int&)>;

[[maybe_unused]] auto memfct_freeFunction = asFnPtr_freeFunction(&TestStruct::fctConstOverloaded);

[[maybe_unused]] auto memfct = Widget_T::asFnPtr(&TestStruct::fctConstOverloaded);
[[maybe_unused]] auto memfctConst = Widget_T::asFnConstPtr(&TestStruct::fctConstOverloaded);

//[[maybe_unused]] auto memfctTemplate = Widget_T::asFnParamPtr<int&>(&TestStruct::fctTemplate); //Commenting to compile not sure if it ever worked
//[[maybe_unused]] auto memfctTemplateConst = Widget_T::asFnConstPtr(&TestStruct::fctTemplate); //Commenting to compile not sure if it ever worked

//[[maybe_unused]] auto memfctParam = Widget_T::asFnPtr(&TestStruct::fctParamOverloaded); //Commenting to compile not sure if it ever worked

//[[maybe_unused]] auto memfctParam = asFnParamPtr<int>(&TestStruct::fctParamOverloaded);
//[[maybe_unused]] auto memfctParamConst = asFnConstParamPtr<double>(&TestStruct::fctParamOverloaded);
