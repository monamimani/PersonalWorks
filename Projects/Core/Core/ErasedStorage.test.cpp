
#include <array>
#include <span>
#include <source_location>

#include "fmt/format.h"
#include "TestUtilities/BasicTestsGenerator.h"
#include "TestUtilities/GoogleTest.h"
#include "TestUtilities/TestStruct.test.h"
using namespace TestUtilities;

import Core;
import ErasedStorage;

namespace CoreTests
{
using namespace Core;

template<typename TestedType>
class ErasedStorageF: public testing::Test
{
public:
  auto getDefaultObj()
  {
    return TestedType{};
  }

  auto AreStorageFctsNullptr(const TestedType& obj) const
  {
    return obj.m_storageFcts.m_destroy == nullptr && obj.m_storageFcts.m_copy == nullptr && obj.m_storageFcts.m_move == nullptr;
  }

  auto IsStorage0Initialized(const TestedType& obj) const
  {
    std::array<std::byte, TestedType::SizeT> defaultStorageBytes{};
    return std::ranges::equal(obj.m_storage, defaultStorageBytes);
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testDefaultObj(const TestedType& obj) const
  {

    if (AreStorageFctsNullptr(obj) && IsStorage0Initialized(obj))
    {
      return testing::AssertionSuccess();
    }

    return testing::AssertionFailure();
  }

  static const auto ObjAExpectedVal = 42;
  static const auto ObjBExpectedVal = 151;

  TestedType getObjA()
  {
    auto objA = TestedType{};
    objA.construct(TestStruct{ObjAExpectedVal});
    return objA;
  }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObjA(const TestedType& objA) const
  {
    // if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    //{

    //}
    // else
    {
      // if (AreStorageFctsNullptr(objA) && IsStorage0Initialized(objA))
      //{
      //   return testing::AssertionSuccess();
      // }

      auto objBytes = TestUtilities::objAsBytes(objA);
      std::array<std::byte, sizeof(TestedType)> ObjAExpectedValAsBytes{(std::byte)ObjAExpectedVal};
      if ((objBytes.size() == ObjAExpectedValAsBytes.size()) && std::ranges::equal(objBytes, ObjAExpectedValAsBytes))
      {
        return testing::AssertionSuccess();
      }
    }

    //SCOPED_TRACE(fmt::format("Failed Operation: {}", GetBasicTestsObjOpsStr(basicTestsObjOp)));
    auto sl = std::source_location::current();
    return testing::AssertionFailure() << fmt::format(
               "Failed Operation: {}, {}({}:{}) {}", GetBasicTestsObjOpsStr(basicTestsObjOp), sl.file_name(), sl.line(), sl.column(), sl.function_name());
  }

  // TestedType getObjB()
  //{
  //   return TestedType{};
  // }

  template<TestUtilities::BasicTestsObjOps basicTestsObjOp>
  testing::AssertionResult testObjB(const TestedType& objB) const
  {
    // if constexpr (basicTestsObjOp != TestUtilities::BasicTestsObjOps::Dtor)
    //{
    //   if (!(bool)objB)
    //   {
    //     ADD_FAILURE();
    //     return testing::AssertionFailure();
    //   }

    //  if (objB.a != ObjBExpectedVal)
    //  {
    //    ADD_FAILURE() << "Error here";
    //    return testing::AssertionFailure();
    //  }
    //}
    // else
    //{
    //  auto objBytes = TestUtilities::objAsBytes(objB);
    //  std::array<std::byte, sizeof(TestedType)> ObjBExpectedValAsBytes{(std::byte)ObjBExpectedVal};
    //  if ((objBytes.size() == ObjBExpectedValAsBytes.size()) && std::ranges::equal(objBytes, ObjBExpectedValAsBytes))
    //  {
    //    return testing::AssertionSuccess();
    //  }
    //}

    // return testing::AssertionSuccess();

    return testing::AssertionFailure();
  }
};

template<typename ErasedType>
using ErasedStorage_T = ErasedStorage<sizeof(ErasedType), alignof(ErasedType)>;
auto typedTest = TestUtilities::RegistratorCommonTests<ErasedStorageF,
                                                       TEST_TYPE(ErasedStorage_T<TestStruct>),
                                                       // TEST_TYPE(ErasedStorage_T<const TestStruct>),  // Removing because I don't think it makes sense to
                                                       // have a const type for Erased storage
                                                       TEST_TYPE(ErasedStorage_T<TestStruct&&>)>{};

template<typename T>
class ErasedStorageTestStructF: public testing::Test
{
  template<typename Compound_T>
  using Base_T = std::remove_cv_t<std::remove_pointer_t<std::remove_cvref_t<Compound_T>>>;

public:
protected:
  using ErasedType = T;
  using BaseType = std::remove_pointer_t<std::remove_reference_t<ErasedType>>;
  using ErasedTypePtr = std::add_pointer_t<std::remove_pointer_t<ErasedType>>;
  static constexpr auto isStoredAsPtr = (std::is_pointer_v<std::remove_reference_t<ErasedType>> || std::is_lvalue_reference_v<ErasedType>);
  using ErasedTypedStoredType = std::conditional_t<isStoredAsPtr, ErasedTypePtr, BaseType>;
  using ErasedTypeByte = std::byte;
  using ErasedStorage_T = ErasedStorage<sizeof(ErasedType), alignof(ErasedType)>;

  ErasedStorage_T m_storage;
  SpecialFunctionCallCounter m_counters;

  void SetUp() override
  {
    if constexpr (std::is_rvalue_reference_v<ErasedType>)
    {
      m_storage.construct(BaseType{});
    }
    else
    {
      m_storage.construct<ErasedType>();
    }

    TestStruct::resetStaticCounters();
  }

  void TearDown() override
  {
    TestStruct::resetStaticCounters();
  }

private:
};

using ErasedTypes_T = ::testing::Types<TestStruct, TestStruct&&>;

struct ErasedTypesNameGenerator
{
  template<typename T>
  static std::string GetName(int i)
  {
    static const std::array names = {"TestStruct", "TestStruct&&"};
    return names[i];
  }
};

TYPED_TEST_SUITE(ErasedStorageTestStructF, ErasedTypes_T, ErasedTypesNameGenerator);

TYPED_TEST(ErasedStorageTestStructF, DefaultCtor)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypedStoredType = TestFixture::ErasedTypedStoredType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    // Default Constructor
    ErasedStorage_T storage;
    const auto& typedPtr = storage.asTypedPtr<ErasedTypedStoredType>();

    // I think this is UB because we are accessing the storage before any object was constructed in it.
    // Or it isn't because std::byte is special.
    ASSERT_EQ(reinterpret_cast<const ErasedTypeByte&>(*typedPtr), std::byte{});

    // Destructor
    storage.~ErasedStorage();
    ASSERT_EQ(reinterpret_cast<const ErasedTypeByte&>(*typedPtr), std::byte{});
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructF, CopyCtor)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypedStoredType = TestFixture::ErasedTypedStoredType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    // Copy Constructor
    ErasedStorage_T storage{this->m_storage};
    this->m_counters.m_nbCallCopyConstructor++;

    const auto& typedPtr = storage.asTypedPtr<ErasedTypedStoredType>();

    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);

    this->m_counters.m_nbCallDestructor++;
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructF, MoveCtor)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypedStoredType = TestFixture::ErasedTypedStoredType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    // Move Constructor
    ErasedStorage_T storage{std::move(this->m_storage)};

    if constexpr (!std::is_const_v<ErasedTypedStoredType>)
    {
      this->m_counters.m_nbCallMoveConstructor++;
    }
    else
    {
      // Because the compiler will generate a call to the copy constructor when trying to move out of a const object
      this->m_counters.m_nbCallCopyConstructor++;
    }

    const auto& typedPtr = storage.asTypedPtr<ErasedTypedStoredType>();

    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);

    this->m_counters.m_nbCallDestructor++;
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructF, CopyAssign)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypedStoredType = TestFixture::ErasedTypedStoredType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    // Copy Assignment
    ErasedStorage_T storage;
    storage = this->m_storage;
    this->m_counters.m_nbCallCopyConstructor++;

    const auto& typedPtr = storage.asTypedPtr<ErasedTypedStoredType>();

    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);

    this->m_counters.m_nbCallDestructor++;
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructF, MoveAssign)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypedStoredType = TestFixture::ErasedTypedStoredType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    // Move Assignment
    ErasedStorage_T storage;
    storage = std::move(this->m_storage);
    if constexpr (!std::is_const_v<ErasedType>)
    {
      this->m_counters.m_nbCallMoveConstructor++;
    }
    else
    {
      // Because the compiler will generate a call to the copy constructor when trying to move out of a const object
      this->m_counters.m_nbCallCopyConstructor++;
    }

    const auto& typedPtr = storage.asTypedPtr<ErasedTypedStoredType>();

    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);

    this->m_counters.m_nbCallDestructor++;
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructF, Swap)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using BaseType = TestFixture::BaseType;
  using ErasedTypedStoredType = TestFixture::ErasedTypedStoredType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    const uint8_t value = 51;
    ErasedStorage_T storage;
    this->m_counters.m_nbCallDefaultConstructor++;
    storage.construct<BaseType>(value);

    using std::swap;
    swap(storage, this->m_storage);

    if constexpr (!std::is_const_v<ErasedType>)
    {
      this->m_counters.m_nbCallMoveConstructor += 3;
      this->m_counters.m_nbCallDestructor += 3;
    }
    else
    {
      // Because the compiler will generate a call to the copy constructor when trying to move out of a const object
      this->m_counters.m_nbCallCopyConstructor += 3;
      this->m_counters.m_nbCallDestructor += 3;
    }

    const auto& typedPtrA = storage.asTypedPtr<ErasedTypedStoredType>();
    ASSERT_NE(typedPtrA, nullptr);
    ASSERT_EQ((*typedPtrA).m_value, TestStruct::m_staticValue);

    const auto& typedPtrB = this->m_storage.asTypedPtr<ErasedTypedStoredType>();
    ASSERT_NE(typedPtrB, nullptr);
    ASSERT_EQ((*typedPtrB).m_value, value);

    this->m_counters.m_nbCallDestructor++;
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructF, ConstructDefault)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using BaseType = TestFixture::BaseType;
  using ErasedTypedStoredType = TestFixture::ErasedTypedStoredType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    // Default Constructor
    ErasedStorage_T storage;
    const auto& typedPtr = storage.asTypedPtr<ErasedTypedStoredType>();

    // I think this is UB because we are accessing the storage before any object was constructed in it.
    // Or it isn't because std::byte is special.
    ASSERT_EQ(reinterpret_cast<const ErasedTypeByte&>(*typedPtr), std::byte{});

    const auto* typedStorage = storage.construct<BaseType>();
    this->m_counters.m_nbCallDefaultConstructor++;

    ASSERT_EQ(typedStorage->m_value, TestStruct::m_staticValue);

    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);

    // Destructor
    storage.~ErasedStorage();
    this->m_counters.m_nbCallDestructor++;
    ASSERT_EQ(reinterpret_cast<const ErasedTypeByte&>(*typedPtr), std::byte{});
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructF, ConstructWithArgs)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using BaseType = TestFixture::BaseType;
  using ErasedTypedStoredType = TestFixture::ErasedTypedStoredType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {

    // Parametric Constructor
    ErasedStorage_T storage;
    const auto& typedPtr = storage.asTypedPtr<ErasedTypedStoredType>();

    // I think this is UB because we are accessing the storage before any object was constructed in it.
    // Or it isn't because std::byte is special.
    ASSERT_EQ(reinterpret_cast<const ErasedTypeByte&>(*typedPtr), std::byte{});

    const auto* typedStorage = storage.construct<BaseType>(TestStruct::m_staticValue);
    this->m_counters.m_nbCallDefaultConstructor++;

    ASSERT_EQ(typedStorage->m_value, TestStruct::m_staticValue);

    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);

    // Destructor
    storage.~ErasedStorage();
    this->m_counters.m_nbCallDestructor++;
    ASSERT_EQ(reinterpret_cast<const ErasedTypeByte&>(*typedPtr), std::byte{});
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructF, ConstructWithMoveCtor)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypedStoredType = TestFixture::ErasedTypedStoredType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    ErasedStorage_T storage;
    const auto& typedPtr = storage.asTypedPtr<ErasedTypedStoredType>();

    // I think this is UB because we are accessing the storage before any object was constructed in it.
    // Or it isn't because std::byte is special.
    ASSERT_EQ(reinterpret_cast<const ErasedTypeByte&>(*typedPtr), std::byte{});

    // Construct with Move Constructor
    const auto* typedStorage = storage.construct(TestStruct{});
    this->m_counters.m_nbCallDefaultConstructor++;
    this->m_counters.m_nbCallMoveConstructor++;
    this->m_counters.m_nbCallDestructor++;

    ASSERT_EQ(typedStorage->m_value, TestStruct::m_staticValue);

    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);

    // Destructor
    storage.~ErasedStorage();
    this->m_counters.m_nbCallDestructor++;
    ASSERT_EQ(reinterpret_cast<const ErasedTypeByte&>(*typedPtr), std::byte{});
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

template<typename T>
class ErasedStorageTestStructPtrF: public testing::Test
{
  template<typename Compound_T>
  using Base_T = std::remove_cv_t<std::remove_pointer_t<std::remove_cvref_t<Compound_T>>>;

public:
protected:
  using ErasedType = std::conditional_t<std::is_same_v<T, void*>, TestStruct*, T>;
  using BaseType = std::remove_pointer_t<std::remove_reference_t<ErasedType>>;
  using ErasedTypeByte = std::byte;
  using ErasedStorage_T = ErasedStorage<sizeof(ErasedType), alignof(ErasedType)>;

  BaseType m_testStruct;
  ErasedStorage_T m_storage;
  SpecialFunctionCallCounter m_counters;

  void SetUp() override
  {

    if constexpr (std::is_pointer_v<ErasedType>)
    {
      m_storage.construct<ErasedType>(&m_testStruct);
    }

    TestStruct::resetStaticCounters();
  }

  void TearDown() override
  {
    TestStruct::resetStaticCounters();
  }

private:
};

using ErasedTypesCompound_T = ::testing::Types<TestStruct*, const TestStruct* /*, TestStruct* const*/, void*>;
TYPED_TEST_SUITE(ErasedStorageTestStructPtrF, ErasedTypesCompound_T);

TYPED_TEST(ErasedStorageTestStructPtrF, DefaultCtor)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    // Default Constructor
    ErasedStorage_T storage;
    const auto& typedPtr = storage.asTypedPtr<ErasedType>();

    // I think this is UB because we are accessing the storage before any object was constructed in it.
    // Or it isn't because std::byte is special.
    ASSERT_EQ(typedPtr, nullptr);

    // Destructor
    storage.~ErasedStorage();
    ASSERT_EQ(typedPtr, nullptr);
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructPtrF, CopyCtor)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    // Copy Constructor
    ErasedStorage_T storage{this->m_storage};

    const auto& typedPtr = storage.asTypedPtr<ErasedType>();

    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ(typedPtr, &(this->m_testStruct));
    ASSERT_EQ((*typedPtr), this->m_testStruct);
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructPtrF, MoveCtor)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    // Move Constructor
    ErasedStorage_T storage{std::move(this->m_storage)};

    const auto& typedPtr = storage.asTypedPtr<ErasedType>();

    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ(typedPtr, &(this->m_testStruct));
    ASSERT_EQ((*typedPtr), this->m_testStruct);
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructPtrF, CopyAssign)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    // Copy Assignment
    ErasedStorage_T storage;
    storage = this->m_storage;
    const auto& typedPtr = storage.asTypedPtr<ErasedType>();

    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ(typedPtr, &(this->m_testStruct));
    ASSERT_EQ((*typedPtr), this->m_testStruct);
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructPtrF, MoveAssign)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    // Move Assignment
    ErasedStorage_T storage;
    storage = std::move(this->m_storage);
    const auto& typedPtr = storage.asTypedPtr<ErasedType>();

    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ(typedPtr, &(this->m_testStruct));
    ASSERT_EQ((*typedPtr), this->m_testStruct);
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructPtrF, Swap)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using BaseType = TestFixture::BaseType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    TestStruct value(51);
    ErasedStorage_T storage;
    storage.construct<ErasedType>(&value);
    this->m_counters.m_nbCallDefaultConstructor++;

    using std::swap;
    swap(storage, this->m_storage);

    const auto& typedPtrA = storage.asTypedPtr<ErasedType>();
    ASSERT_NE(typedPtrA, nullptr);
    ASSERT_EQ((*typedPtrA), this->m_testStruct);

    const auto& typedPtrB = this->m_storage.asTypedPtr<ErasedType>();
    ASSERT_NE(typedPtrB, nullptr);
    ASSERT_EQ((typedPtrB), &value);
    ASSERT_EQ((*typedPtrB), value);

    this->m_counters.m_nbCallDestructor++;
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TYPED_TEST(ErasedStorageTestStructPtrF, ConstructWithCopyCtor)
{
  using ErasedStorage_T = TestFixture::ErasedStorage_T;
  using ErasedType = TestFixture::ErasedType;
  using ErasedTypeByte = TestFixture::ErasedTypeByte;

  {
    ErasedStorage_T storage;

    TestStruct testStruct;
    this->m_counters.m_nbCallDefaultConstructor++;

    // Construct with lvalue will store a pointer
    const auto* typedStorage = storage.construct(testStruct);

    ASSERT_EQ(typedStorage->m_value, TestStruct::m_staticValue);

    const auto& typedPtr = storage.asTypedPtr<ErasedType>();
    ASSERT_NE(typedPtr, nullptr);
    ASSERT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);

    this->m_counters.m_nbCallDestructor++;
  }

  ExpectSpecialFunctionCallCounter(this->m_counters);
  ExpectConstructorsAndDestructorsCount(this->m_counters);
}

TEST(ErasedStorageTestStructArray, SpecialMemberFcts)
{
  using ErasedType = const TestStruct[4];
  using ErasedTypeByte = const std::byte;
  using ErasedStorage_T = ErasedStorage<sizeof(ErasedType), alignof(ErasedType)>;

  TestStruct::resetStaticCounters();

  {
    GTEST_SKIP();
  }
}

TEST(ErasedStorageTestStructArrayConst, SpecialMemberFcts)
{
  using ErasedType = const TestStruct[4];
  using ErasedTypeByte = const std::byte;
  using ErasedStorage_T = ErasedStorage<sizeof(ErasedType), alignof(ErasedType)>;

  TestStruct::resetStaticCounters();

  {
    GTEST_SKIP();
  }
}

} // namespace CoreTests
