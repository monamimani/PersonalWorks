#include <array>
#include <utility>
#include <format>

#include "TestUtilities/BasicTestsGenerator.h"

// NOLINTBEGIN(*-magic-numbers,readability-identifier-length)

namespace TestUtilitiesTests
{
struct Sample
{
  [[nodiscard]] explicit operator bool() const
  {
    return true;
  }

  void operator()() const
  {}

  auto operator<=>(const Sample&) const = default;

  int m_value = 0;

  friend void swap(Sample& lhs, Sample& rhs) noexcept
  {
    using std::swap;
    swap(lhs.m_value, rhs.m_value);
  }
};

template<typename TestedTypeT>
class SampleTestF: public testing::Test
{
public:
  TestedTypeT getDefaultObj()
  {
    return TestedTypeT{};
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testDefaultObj(const TestedTypeT& obj) const
  {
    if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    {
      if ((bool)obj)
      {
        return testing::AssertionSuccess();
      }
    }
    else
    {
      auto objBytes = TestUtilities::objAsBytes(obj);
      std::array<std::byte, sizeof(TestedTypeT)> defaultObjAsBytes{};
      if ((objBytes.size() == defaultObjAsBytes.size()) && std::ranges::equal(objBytes, defaultObjAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionFailure();
  }

  static const auto objAExpectedVal = 42;
  static const auto objBExpectedVal = 151;

  TestedTypeT getObjA()
  {
    return TestedTypeT{objAExpectedVal};
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObjA(const TestedTypeT& objA) const
  {
    if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    {
      if (!(bool)objA)
      {
        ADD_FAILURE();
        return testing::AssertionFailure();
      }

      if (objA.m_value != objAExpectedVal)
      {
        ADD_FAILURE() << "Error here";
        return testing::AssertionFailure();
      }
    }
    else
    {
      auto objBytes = TestUtilities::objAsBytes(objA);
      std::array<std::byte, sizeof(TestedTypeT)> objAExpectedValAsBytes{(std::byte)objAExpectedVal};
      if ((objBytes.size() == objAExpectedValAsBytes.size()) && std::ranges::equal(objBytes, objAExpectedValAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionSuccess();
  }

  TestedTypeT getObjB()
  {
    return TestedTypeT{objBExpectedVal};
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObjB(const TestedTypeT& objB) const
  {
    if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    {
      if (!(bool)objB)
      {
        ADD_FAILURE();
        return testing::AssertionFailure();
      }

      if (objB.m_value != objBExpectedVal)
      {
        ADD_FAILURE() << "Error here";
        return testing::AssertionFailure();
      }
    }
    else
    {
      auto objBytes = TestUtilities::objAsBytes(objB);
      std::array<std::byte, sizeof(TestedTypeT)> objBExpectedValAsBytes{(std::byte)objBExpectedVal};
      if ((objBytes.size() == objBExpectedValAsBytes.size()) && std::ranges::equal(objBytes, objBExpectedValAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionSuccess();
  }
};

template<typename TestedTypeT>
class SampleParamTestF: public testing::TestWithParam<std::tuple<Sample, Sample>>
{
public:
  static constexpr auto makeTestName = [](const testing::TestParamInfo<ParamType>& info) {
    auto [sampleA, sampleB] = info.param;

    const std::string name = std::format("SampleAVal{}SampleBVal{}", sampleA.m_value, sampleB.m_value);

    return name;
  };

  explicit SampleParamTestF(ParamType param)
  {
    std::tie(m_sampleA, m_sampleB) = param;
  }

  TestedTypeT getDefaultObj()
  {
    return TestedTypeT{};
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testDefaultObj(const TestedTypeT& obj) const
  {
    if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    {
      if ((bool)obj)
      {
        return testing::AssertionSuccess();
      }
    }
    else
    {
      auto objBytes = TestUtilities::objAsBytes(obj);
      std::array<std::byte, sizeof(TestedTypeT)> defaultObjAsBytes{};
      if ((objBytes.size() == defaultObjAsBytes.size()) && std::ranges::equal(objBytes, defaultObjAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionFailure();
  }

  TestedTypeT getObjA()
  {
    return m_sampleA;
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObjA(const TestedTypeT& objA) const
  {
    if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    {
      if (!(bool)objA)
      {
        ADD_FAILURE();
        return testing::AssertionFailure();
      }

      if (objA.m_value != m_sampleA.m_value)
      {
        ADD_FAILURE() << "Error here";
        return testing::AssertionFailure();
      }
    }
    else
    {
      auto objBytes = TestUtilities::objAsBytes(objA);
      std::array<std::byte, sizeof(TestedTypeT)> objAExpectedValAsBytes{(std::byte)m_sampleA.m_value};
      if ((objBytes.size() == objAExpectedValAsBytes.size()) && std::ranges::equal(objBytes, objAExpectedValAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionSuccess();
  }

  TestedTypeT getObjB()
  {
    return m_sampleB;
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObjB(const TestedTypeT& objB) const
  {
    if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    {
      if (!(bool)objB)
      {
        ADD_FAILURE();
        return testing::AssertionFailure();
      }

      if (objB.m_value != m_sampleB.m_value)
      {
        ADD_FAILURE() << "Error here";
        return testing::AssertionFailure();
      }
    }
    else
    {
      auto objBytes = TestUtilities::objAsBytes(objB);
      std::array<std::byte, sizeof(TestedTypeT)> objBExpectedValAsBytes{(std::byte)m_sampleB.m_value};
      if ((objBytes.size() == objBExpectedValAsBytes.size()) && std::ranges::equal(objBytes, objBExpectedValAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }
    return testing::AssertionSuccess();
  }

private:
  Sample m_sampleA;
  Sample m_sampleB;
};

// typedef Sample Sample_T;
// constexpr auto testDesc = TEST_TYPE(Sample);
constexpr const char* sampleStr = "Sample";
constexpr auto testDesc = TestUtilities::TypedTestDesc<Sample>{.m_typeName = sampleStr};
//auto typedTest = TestUtilities::RegistratorCommonTests<SampleTestF, testDesc, testDesc>{};

static const auto paramA = ::testing::Values(Sample{42}, Sample{16});
static const auto paramB = ::testing::Values(Sample{151});
static const auto paramGenerator = ::testing::Combine(paramA, paramB);
//auto parametricTest = TestUtilities::RegistratorCommonTests<SampleParamTestF, testDesc>{paramGenerator, SampleParamTestF<Sample>::makeTestName};

} // namespace TestUtilitiesTests

// NOLINTEND(*-magic-numbers,readability-identifier-length)
