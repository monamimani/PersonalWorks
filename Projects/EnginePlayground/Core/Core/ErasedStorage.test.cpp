
#include <array>
#include <source_location>
#include <span>
#include <format>

#include "TestUtilities/GoogleTest.h"
#include "TestUtilities/TestStruct.test.h"
#include "TestUtilities/TestsFriend.h"
using namespace TestUtilities;

import Core;
import ErasedStorage;

namespace CoreTests
{
using namespace Core;

/**
 * @brief Fixture for testing ErasedStorage with a TestStruct (or TestStruct&&).
 */
template<typename T>
class ErasedStorageTestStructF: public testing::Test
{
public:
protected:
  using ErasedType = T;
  using BaseType = std::remove_pointer_t<std::remove_reference_t<ErasedType>>;
  using ErasedTypePtr = std::add_pointer_t<std::remove_pointer_t<ErasedType>>;
  
  static constexpr auto isStoredAsPtr = (std::is_pointer_v<std::remove_reference_t<ErasedType>> || std::is_lvalue_reference_v<ErasedType>);
  using ErasedTypedStoredType = std::conditional_t<isStoredAsPtr, ErasedTypePtr, BaseType>;
  
  using ErasedStorage_T = ErasedStorage<sizeof(ErasedType), alignof(ErasedType)>;

  ErasedStorage_T m_storage;
  SpecialFunctionCallCounter m_expected;

  void SetUp() override
  {
    TestStruct::resetStaticCounters();
    m_expected = {};

    if constexpr (std::is_rvalue_reference_v<ErasedType>)
    {
      m_storage.construct(BaseType{});
      m_expected.m_nbCallDefaultConstructor++;
      m_expected.m_nbCallMoveConstructor++;
      m_expected.m_nbCallDestructor++;
    }
    else
    {
      m_storage.template construct<ErasedType>();
      m_expected.m_nbCallDefaultConstructor++;
    }
  }

  void verifyAndCleanup()
  {
    m_storage.erase();
    m_expected.m_nbCallDestructor++;
    
    // Robustly verify zero-initialization after erase() using C++23 start_lifetime_as on bytes (implicit-lifetime)
    auto* byteView = std::start_lifetime_as<const std::byte[sizeof(ErasedType)]>(m_storage.template asTypedPtr<std::byte>());
    for (auto b : *byteView) { EXPECT_EQ(b, std::byte{0}); }

    TestStruct::assertSpecialFunctionCallCounter(m_expected);
  }
};

using ErasedTypes_T = ::testing::Types<TestStruct, TestStruct&&>;

struct ErasedTypesNameGenerator
{
  template<typename T>
  static std::string GetName(int i)
  {
    static const std::array names = {"TestStruct", "TestStruct_RValRef"};
    return names[i];
  }
};

TYPED_TEST_SUITE(ErasedStorageTestStructF, ErasedTypes_T, ErasedTypesNameGenerator);

TYPED_TEST(ErasedStorageTestStructF, Lifecycle_EmptyStorage)
{
  using ErasedStorage_T = typename TestFixture::ErasedStorage_T;
  
  {
    ErasedStorage_T emptyStorage;
    
    // Initial state check
    auto* byteView = std::start_lifetime_as<const std::byte[sizeof(typename TestFixture::ErasedType)]>(emptyStorage.template asTypedPtr<std::byte>());
    for (auto b : *byteView) { EXPECT_EQ(b, std::byte{0}); }

    emptyStorage.~ErasedStorage();
  }

  this->verifyAndCleanup();
}

TYPED_TEST(ErasedStorageTestStructF, CopyConstruction)
{
  using ErasedStorage_T = typename TestFixture::ErasedStorage_T;
  using ErasedTypedStoredType = typename TestFixture::ErasedTypedStoredType;

  {
    ErasedStorage_T storageCopy{this->m_storage};
    this->m_expected.m_nbCallCopyConstructor++;

    const auto& typedPtr = storageCopy.template asTypedPtr<ErasedTypedStoredType>();
    ASSERT_NE(typedPtr, nullptr);
    EXPECT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);
    
    this->m_expected.m_nbCallDestructor++;
  }

  this->verifyAndCleanup();
}

TYPED_TEST(ErasedStorageTestStructF, MoveConstruction)
{
  using ErasedStorage_T = typename TestFixture::ErasedStorage_T;
  using ErasedTypedStoredType = typename TestFixture::ErasedTypedStoredType;

  {
    ErasedStorage_T storageMoved{std::move(this->m_storage)};

    if constexpr (!std::is_const_v<ErasedTypedStoredType>)
    {
      this->m_expected.m_nbCallMoveConstructor++;
    }
    else
    {
      this->m_expected.m_nbCallCopyConstructor++;
    }

    const auto& typedPtr = storageMoved.template asTypedPtr<ErasedTypedStoredType>();
    ASSERT_NE(typedPtr, nullptr);
    EXPECT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);
    
    this->m_expected.m_nbCallDestructor++;
  }

  this->verifyAndCleanup();
}

TYPED_TEST(ErasedStorageTestStructF, CopyAssignment)
{
  using ErasedStorage_T = typename TestFixture::ErasedStorage_T;
  using ErasedTypedStoredType = typename TestFixture::ErasedTypedStoredType;

  {
    ErasedStorage_T target;
    
    auto* byteView = std::start_lifetime_as<const std::byte[sizeof(typename TestFixture::ErasedType)]>(target.template asTypedPtr<std::byte>());
    for (auto b : *byteView) { EXPECT_EQ(b, std::byte{0}); }

    target = this->m_storage;
    this->m_expected.m_nbCallCopyConstructor++;

    const auto& typedPtr = target.template asTypedPtr<ErasedTypedStoredType>();
    ASSERT_NE(typedPtr, nullptr);
    EXPECT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);
    
    this->m_expected.m_nbCallDestructor++;
  }

  this->verifyAndCleanup();
}

TYPED_TEST(ErasedStorageTestStructF, MoveAssignment)
{
  using ErasedStorage_T = typename TestFixture::ErasedStorage_T;
  using ErasedType = typename TestFixture::ErasedType;
  using ErasedTypedStoredType = typename TestFixture::ErasedTypedStoredType;

  {
    ErasedStorage_T target;
    target = std::move(this->m_storage);

    if constexpr (!std::is_const_v<ErasedType>)
    {
      this->m_expected.m_nbCallMoveConstructor++;
    }
    else
    {
      this->m_expected.m_nbCallCopyConstructor++;
    }

    const auto& typedPtr = target.template asTypedPtr<ErasedTypedStoredType>();
    ASSERT_NE(typedPtr, nullptr);
    EXPECT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);
    
    this->m_expected.m_nbCallDestructor++;
  }

  this->verifyAndCleanup();
}

TYPED_TEST(ErasedStorageTestStructF, Swap)
{
  using ErasedStorage_T = typename TestFixture::ErasedStorage_T;
  using ErasedType = typename TestFixture::ErasedType;
  using BaseType = typename TestFixture::BaseType;
  using ErasedTypedStoredType = typename TestFixture::ErasedTypedStoredType;

  {
    const uint8_t altValue = 51;
    ErasedStorage_T other;
    other.template construct<BaseType>(altValue);
    this->m_expected.m_nbCallDefaultConstructor++;

    using std::swap;
    swap(other, this->m_storage);

    if constexpr (!std::is_const_v<ErasedType>)
    {
      this->m_expected.m_nbCallMoveConstructor += 3;
      this->m_expected.m_nbCallDestructor += 3;
    }
    else
    {
      this->m_expected.m_nbCallCopyConstructor += 3;
      this->m_expected.m_nbCallDestructor += 3;
    }

    const auto& typedPtrOther = other.template asTypedPtr<ErasedTypedStoredType>();
    ASSERT_NE(typedPtrOther, nullptr);
    EXPECT_EQ((*typedPtrOther).m_value, TestStruct::m_staticValue);

    const auto& typedPtrFixture = this->m_storage.template asTypedPtr<ErasedTypedStoredType>();
    ASSERT_NE(typedPtrFixture, nullptr);
    EXPECT_EQ((*typedPtrFixture).m_value, altValue);

    this->m_expected.m_nbCallDestructor++;
  }

  this->verifyAndCleanup();
}

TYPED_TEST(ErasedStorageTestStructF, Construct_Default)
{
  using ErasedStorage_T = typename TestFixture::ErasedStorage_T;
  using ErasedType = typename TestFixture::ErasedType;
  using BaseType = typename TestFixture::BaseType;
  using ErasedTypedStoredType = typename TestFixture::ErasedTypedStoredType;

  {
    ErasedStorage_T local;
    
    auto* byteView = std::start_lifetime_as<const std::byte[sizeof(ErasedType)]>(local.template asTypedPtr<std::byte>());
    for (auto b : *byteView) { EXPECT_EQ(b, std::byte{0}); }

    local.template construct<BaseType>();
    this->m_expected.m_nbCallDefaultConstructor++;

    const auto& typedPtr = local.template asTypedPtr<ErasedTypedStoredType>();
    ASSERT_NE(typedPtr, nullptr);
    EXPECT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);

    this->m_expected.m_nbCallDestructor++;
  }

  this->verifyAndCleanup();
}

TYPED_TEST(ErasedStorageTestStructF, Construct_WithRValue)
{
  using ErasedStorage_T = typename TestFixture::ErasedStorage_T;
  using ErasedTypedStoredType = typename TestFixture::ErasedTypedStoredType;

  {
    ErasedStorage_T local;
    local.construct(TestStruct{});
    this->m_expected.m_nbCallDefaultConstructor++;
    this->m_expected.m_nbCallMoveConstructor++;
    this->m_expected.m_nbCallDestructor++;

    const auto& typedPtr = local.template asTypedPtr<ErasedTypedStoredType>();
    ASSERT_NE(typedPtr, nullptr);
    EXPECT_EQ((*typedPtr).m_value, TestStruct::m_staticValue);

    this->m_expected.m_nbCallDestructor++;
  }

  this->verifyAndCleanup();
}

/**
 * @brief Fixture for testing ErasedStorage when it stores a pointer.
 */
template<typename T>
class ErasedStorageTestStructPtrF: public testing::Test
{
public:
protected:
  using ErasedType = std::conditional_t<std::is_same_v<T, void*>, TestStruct*, T>;
  using BaseType = std::remove_pointer_t<std::remove_reference_t<ErasedType>>;
  using ErasedStorage_T = ErasedStorage<sizeof(ErasedType), alignof(ErasedType)>;

  BaseType m_testStruct;
  ErasedStorage_T m_storage;
  SpecialFunctionCallCounter m_expected;

  void SetUp() override
  {
    TestStruct::resetStaticCounters();
    m_expected = {};

    if constexpr (std::is_pointer_v<ErasedType>)
    {
      m_storage.template construct<ErasedType>(&m_testStruct);
    }
  }

  void verifyAndCleanup()
  {
    m_storage.erase();
    auto* byteView = std::start_lifetime_as<const std::byte[sizeof(ErasedType)]>(m_storage.template asTypedPtr<std::byte>());
    for (auto b : *byteView) { EXPECT_EQ(b, std::byte{0}); }

    TestStruct::assertSpecialFunctionCallCounter(m_expected);
  }
};

using ErasedTypesCompound_T = ::testing::Types<TestStruct*, const TestStruct*, void*>;
TYPED_TEST_SUITE(ErasedStorageTestStructPtrF, ErasedTypesCompound_T);

TYPED_TEST(ErasedStorageTestStructPtrF, Lifecycle_DefaultCtor)
{
  using ErasedStorage_T = typename TestFixture::ErasedStorage_T;
  using ErasedType = typename TestFixture::ErasedType;

  {
    ErasedStorage_T empty;
    auto* byteView = std::start_lifetime_as<const std::byte[sizeof(ErasedType)]>(empty.template asTypedPtr<std::byte>());
    for (auto b : *byteView) { EXPECT_EQ(b, std::byte{0}); }
    
    EXPECT_EQ(empty.template asTypedPtr<ErasedType>(), nullptr);
  }

  this->verifyAndCleanup();
}

TYPED_TEST(ErasedStorageTestStructPtrF, Lifecycle_Copy_And_Move)
{
  using ErasedStorage_T = typename TestFixture::ErasedStorage_T;
  using ErasedType = typename TestFixture::ErasedType;

  {
    ErasedStorage_T storageCopy{this->m_storage};
    EXPECT_EQ(storageCopy.template asTypedPtr<ErasedType>(), &(this->m_testStruct));

    ErasedStorage_T storageMove{std::move(storageCopy)};
    EXPECT_EQ(storageMove.template asTypedPtr<ErasedType>(), &(this->m_testStruct));
  }

  this->verifyAndCleanup();
}

TYPED_TEST(ErasedStorageTestStructPtrF, Lifecycle_Swap)
{
  using ErasedStorage_T = typename TestFixture::ErasedStorage_T;
  using ErasedType = typename TestFixture::ErasedType;

  {
    TestStruct altStruct(51);
    this->m_expected.m_nbCallDefaultConstructor++;

    ErasedStorage_T other;
    other.template construct<ErasedType>(&altStruct);
    
    using std::swap;
    swap(other, this->m_storage);

    EXPECT_EQ(other.template asTypedPtr<ErasedType>(), &(this->m_testStruct));
    EXPECT_EQ(this->m_storage.template asTypedPtr<ErasedType>(), &altStruct);

    this->m_expected.m_nbCallDestructor++;
  }

  this->verifyAndCleanup();
}

} // namespace CoreTests
