#include "TestUtilities/BasicTests.h"

#include <array>

namespace TestUtilitiesTests
{
struct BasicTestsStruct
{
  [[nodiscard]] explicit operator bool() const
  {
    return true;
  }

  void operator()() const
  {}

  auto operator<=>(const BasicTestsStruct&) const = default;

  int a = 0;
};

class BasicTestsStructTestF
{
public:
  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObj(const BasicTestsStruct& obj) const
  {
    // if constexpr ((basicTestsObjOp == TestUtilities::BasicTestsObjOps::DefaultCtor) || (basicTestsObjOp == TestUtilities::BasicTestsObjOps::MoveCtor)
    //               || (basicTestsObjOp == TestUtilities::BasicTestsObjOps::CopyAssign))
    {
      if ((bool)obj)
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionFailure();
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObj(const std::span<const std::byte>& objBytes) const
  {
    if constexpr (basicTestsObjOp == TestUtilities::BasicTestsObjOps::Dtor)
    {
      static std::array<std::byte, sizeof(BasicTestsStruct)> basicTestsStructByteArray{};
      if ((objBytes.size() == basicTestsStructByteArray.size()) && std::ranges::equal(objBytes, basicTestsStructByteArray))
      {
        return testing::AssertionSuccess();
      }
    }

    return testing::AssertionFailure();
  }
};

using BasicTestsStruct_T = BasicTestsStruct;
auto test = TestUtilities::RegistratorCommonTests<BasicTestsStructTestF, TEST_TYPE(BasicTestsStruct_T), TEST_TYPE(const BasicTestsStruct_T)>{};

} // namespace TestUtilitiesTests
