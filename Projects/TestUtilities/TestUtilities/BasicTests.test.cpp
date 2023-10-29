#include "TestUtilities/BasicTestsGenerator.h"

#include <array>
#include <utility>

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

  int a = 0;

  friend void swap(Sample& lhs, Sample& rhs)
  {
    using std::swap;
    swap(lhs.a, rhs.a);
  }
};

template<typename TestedType>
class SampleTestF: public testing::Test
{
public:
  TestedType getDefaultObj()
  {
    return TestedType{};
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testDefaultObj(const TestedType& obj) const
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
      std::array<std::byte, sizeof(TestedType)> defaultObjAsBytes{};
      if ((objBytes.size() == defaultObjAsBytes.size()) && std::ranges::equal(objBytes, defaultObjAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionFailure();
  }

  static const auto ObjAExpectedVal = 42;
  static const auto ObjBExpectedVal = 151;

  TestedType getObjA()
  {
    return TestedType{ObjAExpectedVal};
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObjA(const TestedType& objA) const
  {
    if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    {
      if (!(bool)objA)
      {
        ADD_FAILURE();
        return testing::AssertionFailure();
      }

      if (objA.a != ObjAExpectedVal)
      {
        ADD_FAILURE() << "Error here";
        return testing::AssertionFailure();
      }
    }
    else
    {
      auto objBytes = TestUtilities::objAsBytes(objA);
      std::array<std::byte, sizeof(TestedType)> ObjAExpectedValAsBytes{(std::byte)ObjAExpectedVal};
      if ((objBytes.size() == ObjAExpectedValAsBytes.size()) && std::ranges::equal(objBytes, ObjAExpectedValAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionSuccess();
  }

  TestedType getObjB()
  {
    return TestedType{ObjBExpectedVal};
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObjB(const TestedType& objB) const
  {
    if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    {
      if (!(bool)objB)
      {
        ADD_FAILURE();
        return testing::AssertionFailure();
      }

      if (objB.a != ObjBExpectedVal)
      {
        ADD_FAILURE() << "Error here";
        return testing::AssertionFailure();
      }
    }
    else
    {
      auto objBytes = TestUtilities::objAsBytes(objB);
      std::array<std::byte, sizeof(TestedType)> ObjBExpectedValAsBytes{(std::byte)ObjBExpectedVal};
      if ((objBytes.size() == ObjBExpectedValAsBytes.size()) && std::ranges::equal(objBytes, ObjBExpectedValAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionSuccess();
  }
};

template<typename TestedType>
class SampleParamTestF: public testing::TestWithParam<std::tuple<Sample, Sample>>
{
public:
  static constexpr auto makeTestName = [](const testing::TestParamInfo<ParamType>& info) {
    auto [sampleA, sampleB] = info.param;

    std::string name = fmt::format("SampleAVal{}SampleBVal{}", sampleA.a, sampleB.a);

    return name;
  };

  SampleParamTestF(ParamType param)
  {
    std::tie(m_sampleA, m_sampleB) = param;
  }

  TestedType getDefaultObj()
  {
    return TestedType{};
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testDefaultObj(const TestedType& obj) const
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
      std::array<std::byte, sizeof(TestedType)> defaultObjAsBytes{};
      if ((objBytes.size() == defaultObjAsBytes.size()) && std::ranges::equal(objBytes, defaultObjAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionFailure();
  }

  TestedType getObjA()
  {
    return m_sampleA;
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObjA(const TestedType& objA) const
  {
    if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    {
      if (!(bool)objA)
      {
        ADD_FAILURE();
        return testing::AssertionFailure();
      }

      if (objA.a != m_sampleA.a)
      {
        ADD_FAILURE() << "Error here";
        return testing::AssertionFailure();
      }
    }
    else
    {
      auto objBytes = TestUtilities::objAsBytes(objA);
      std::array<std::byte, sizeof(TestedType)> ObjAExpectedValAsBytes{(std::byte)m_sampleA.a};
      if ((objBytes.size() == ObjAExpectedValAsBytes.size()) && std::ranges::equal(objBytes, ObjAExpectedValAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionSuccess();
  }

  TestedType getObjB()
  {
    return m_sampleB;
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObjB(const TestedType& objB) const
  {
    if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    {
      if (!(bool)objB)
      {
        ADD_FAILURE();
        return testing::AssertionFailure();
      }

      if (objB.a != m_sampleB.a)
      {
        ADD_FAILURE() << "Error here";
        return testing::AssertionFailure();
      }
    }
    else
    {
      auto objBytes = TestUtilities::objAsBytes(objB);
      std::array<std::byte, sizeof(TestedType)> ObjBExpectedValAsBytes{(std::byte)m_sampleB.a};
      if ((objBytes.size() == ObjBExpectedValAsBytes.size()) && std::ranges::equal(objBytes, ObjBExpectedValAsBytes))
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

using Sample_T = Sample;
auto typedTest = TestUtilities::RegistratorCommonTests<SampleTestF, TEST_TYPE(Sample_T), TEST_TYPE(const Sample_T)>{};

static const auto paramA = ::testing::Values(Sample_T{42}, Sample_T{16});
static const auto paramB = ::testing::Values(Sample_T{151});
static const auto paramGenerator = ::testing::Combine(paramA, paramB);
auto parametricTest = TestUtilities::RegistratorCommonTests<SampleParamTestF, TEST_TYPE(Sample_T)>{paramGenerator, SampleParamTestF<Sample_T>::makeTestName};

} // namespace TestUtilitiesTests
