module;

#include <concepts>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>

#include "TestUtilities/TestsFriend.h"

export module ErasedStorage;

import CoreUtility;
import CoreConcepts;

TEST_FIXTURE_FORWARD_DECLARE_NS(CoreTests, ErasedStorageF);

namespace Core
{

template<typename ErasedStorage_t>
struct StorageFcts
{
  //~StorageFcts()
  //{
  //  resetFcts();
  //}

  template<typename ErasedType>
  void setFcts()
  {
    if constexpr (!std::is_trivially_copyable_v<ErasedType>)
    {
      m_destroy = &ErasedStorage_t::template destroyStorage<ErasedType>;
      m_copy = &ErasedStorage_t::template copyStorage<ErasedType>;
      m_move = &ErasedStorage_t::template moveStorage<ErasedType>;
      // m_relocate = &ErasedStorage_t::template relocateStorage<ErasedType>;
    }
    else
    {
      m_destroy = nullptr;
      m_copy = nullptr;
      m_move = nullptr;
      // m_relocate = nullptr;
    }
  }

  void resetFcts()
  {
    m_destroy = nullptr;
    m_copy = nullptr;
    m_move = nullptr;
    // m_relocate = nullptr;
  }

  bool operator==(const StorageFcts&) const = default;

  using DestroyStorageFunction = void (*)(ErasedStorage_t&);
  using CopyStorageFunction = void (*)(ErasedStorage_t&, const ErasedStorage_t&);
  using MoveStorageFunction = void (*)(ErasedStorage_t&, ErasedStorage_t&);
  // using RelocateStorageFunction = void (*)(ErasedStorage_t&, ErasedStorage_t&);

  DestroyStorageFunction m_destroy = nullptr;
  CopyStorageFunction m_copy = nullptr;
  MoveStorageFunction m_move = nullptr;
  // RelocateStorageFunction m_relocate = nullptr;
};

export template<std::size_t size, std::size_t alignment>
class ErasedStorage final
{
  template<typename Compound_T>
  using Base_T = std::remove_cv_t<std::remove_pointer_t<std::remove_cvref_t<Compound_T>>>;

  static inline constexpr auto SizeT = size;

public:
  constexpr ErasedStorage() noexcept = default;

  constexpr ~ErasedStorage() noexcept
  {
    erase();
  }

  ErasedStorage(const ErasedStorage& other) noexcept(noexcept(copy(*this, other)))
  : m_storage{}
  {
    copy(*this, other);
  }

  ErasedStorage(ErasedStorage&& other) noexcept(noexcept(move(*this, other)))
  {
    move(*this, other);
  }

  ErasedStorage& operator=(const ErasedStorage& other) noexcept(noexcept(copy(*this, other)) && noexcept(m_storageFcts.m_destroy(*this)))
  {
    // erase();
    if (m_storageFcts.m_destroy)
    {
      m_storageFcts.m_destroy(*this);
    }
    copy(*this, other);

    return *this;
  }

  ErasedStorage& operator=(ErasedStorage&& other) noexcept(noexcept(move(*this, other)))
  {
    // erase();
    if (m_storageFcts.m_destroy)
    {
      m_storageFcts.m_destroy(*this);
    }
    move(*this, other);

    return *this;
  }

  constexpr void erase()
  {
    if (m_storageFcts.m_destroy)
    {
      m_storageFcts.m_destroy(*this);
      m_storageFcts.resetFcts();
    }

    std::memset(m_storage, 0, size);
  }

  template<typename ErasedType>
  requires std::constructible_from<ErasedType>
  constexpr auto* construct()
  {
    static_assert(sizeof(ErasedType) <= size);
    static_assert(alignof(ErasedType) <= alignment);
    static_assert(std::is_same<typename std::remove_cv<ErasedType>::type, ErasedType>::value, "ErasedType must be a non-const, non-volatile type");

    auto* storagePtr = start_lifetime_as<ErasedType>(std::addressof(m_storage));
    // auto* storagePtr = reinterpret_cast<ErasedType*>(std::addressof(m_storage));
    auto storageTypedPtr = std::construct_at(storagePtr);

    m_storageFcts.template setFcts<ErasedType>();
    return storageTypedPtr;
  }

  template<typename ErasedType, typename... Args>
  requires(std::constructible_from<ErasedType, Args...> && (sizeof...(Args) > 0)
           && !std::same_as<Base_T<std::tuple_element_t<0, std::tuple<Args...>>>, Base_T<ErasedType>>)
  constexpr auto* construct(Args&&... args)
  {
    static_assert(sizeof(ErasedType) <= size);
    static_assert(alignof(ErasedType) <= alignment);
    static_assert(std::is_same<typename std::remove_cv<ErasedType>::type, ErasedType>::value, "ErasedType must be a non-const, non-volatile type");

    auto* storagePtr = start_lifetime_as<ErasedType>(std::addressof(m_storage));
    // auto* storagePtr = reinterpret_cast<ErasedType*>(std::addressof(m_storage));
    auto storageTypedPtr = std::construct_at(storagePtr, std::forward<Args>(args)...);

    m_storageFcts.template setFcts<ErasedType>();
    return storageTypedPtr;
  }

  template<typename ErasedType>
  requires std::constructible_from<ErasedType, ErasedType&&>
  constexpr auto* construct(ErasedType&& obj)
  {
    static_assert(sizeof(ErasedType) <= size);
    static_assert(alignof(ErasedType) <= alignment);
    static_assert(std::is_same<typename std::remove_cv<ErasedType>::type, ErasedType>::value, "ErasedType must be a non-const, non-volatile type");

    // This will have issues with multiple level of pointer indirection
    using RawType = std::remove_pointer_t<std::remove_reference_t<ErasedType>>;

    if constexpr (std::is_lvalue_reference_v<ErasedType>)
    {
      m_storageFcts.template setFcts<std::add_pointer_t<RawType>>();

      auto** storagePtr = start_lifetime_as<RawType*>(std::addressof(m_storage));
      auto storageTypedPtr = std::construct_at(storagePtr, &obj);
      return *storageTypedPtr;
    }
    else if constexpr (std::is_object_v<ErasedType>)
    {
      m_storageFcts.template setFcts<ErasedType>();
      if constexpr (std::is_pointer_v<ErasedType>)
      {
        auto** storagePtr = start_lifetime_as<RawType*>(std::addressof(m_storage));
        ;
        auto storageTypedPtr = std::construct_at(storagePtr, obj);
        return *storageTypedPtr;
      }
      else
      {
        auto* storagePtr = start_lifetime_as<RawType>(std::addressof(m_storage));
        auto storageTypedPtr = std::construct_at(storagePtr, std::forward<ErasedType>(obj));
        return storageTypedPtr;
      }
    }
    else
    {
      std::unreachable();
    }
  }

  template<typename ErasedType>
  constexpr auto asTypedPtr()
  {
    // This will have issues with multiple level of pointer indirection
    using RawType = std::remove_pointer_t<std::remove_reference_t<ErasedType>>;

    // TODO std::launder()
    // static_assert(std::is_lvalue_reference_v<ErasedType> == false, "Creating an erased type of a reference to T is illegal, use T* as the erased type.");
    // static_assert(std::is_object_v<ErasedType>); // this is true for int, int* and false for references, void and function
    if constexpr (std::is_lvalue_reference_v<ErasedType>)
    {
      return *reinterpret_cast<RawType**>(std::addressof(m_storage));
    }
    else if constexpr (std::is_object_v<ErasedType>)
    {
      if constexpr (std::is_pointer_v<ErasedType>)
      {
        return *reinterpret_cast<RawType**>(std::addressof(m_storage));
      }
      else
      {
        return reinterpret_cast<RawType*>(std::addressof(m_storage));
      }
    }
    else
    {
      std::unreachable();
    }
  }

  template<typename ErasedType>
  constexpr const auto asTypedPtr() const
  {
    // This will have issues with multiple level of pointer indirection
    using RawType = std::remove_pointer_t<std::remove_reference_t<ErasedType>>;

    // TODO std::launder()
    // static_assert(std::is_lvalue_reference_v<ErasedType> == false, "Creating an erased type of a reference to T is illegal, use T* as the erased type.");
    // static_assert(std::is_object_v<ErasedType>); // this is true for int, int* and false for references, void and function

    if constexpr (std::is_lvalue_reference_v<ErasedType>)
    {
      return *reinterpret_cast<RawType**>(std::addressof(m_storage));
    }
    else if constexpr (std::is_object_v<ErasedType>)
    {
      if constexpr (std::is_pointer_v<ErasedType>)
      {
        return *reinterpret_cast<RawType* const*>(std::addressof(m_storage));
      }
      else
      {
        return reinterpret_cast<const RawType*>(std::addressof(m_storage));
      }
    }
    else
    {
      std::unreachable();
    }
  }

  void swap(ErasedStorage& other)
  {
    // ErasedStorage storageTmp = std::move(other);
    // other = std::move(*this);
    //*this = std::move(storageTmp);

    ErasedStorage storageTmp = std::move(other);

    if (other.m_storageFcts.m_destroy)
    {
      other.m_storageFcts.m_destroy(other);
    }
    move(other, *this);

    if (m_storageFcts.m_destroy)
    {
      m_storageFcts.m_destroy(*this);
    }
    move(*this, storageTmp);
  }

  friend inline void swap(ErasedStorage& lhs, ErasedStorage& rhs)
  {
    lhs.swap(rhs);
  }

  bool operator==(const ErasedStorage&) const = default;

private:
  template<typename ErasedType>
  static constexpr void destroyStorage(ErasedStorage& storage)
  {
    std::destroy_at(storage.asTypedPtr<ErasedType>());
  }

  static inline constexpr void move(ErasedStorage& dst, ErasedStorage& src)
  {
    if (src.m_storageFcts.m_move)
    {
      src.m_storageFcts.m_move(dst, src);
    }
    else
    {
      std::memcpy(dst.m_storage, src.m_storage, size);
    }

    dst.m_storageFcts = src.m_storageFcts;
  }

  template<typename ErasedType>
  static inline constexpr void moveStorage(ErasedStorage& dst, ErasedStorage& src) noexcept(
      std::is_trivially_copyable_v<ErasedType> || noexcept(std::construct_at(dst.asTypedPtr<ErasedType>(), std::move(*src.asTypedPtr<ErasedType>()))))
  {
    std::memset(dst.m_storage, 0, size);
    std::construct_at(dst.asTypedPtr<ErasedType>(), std::move(*src.asTypedPtr<ErasedType>()));
  }

  // static constexpr void relocate(ErasedStorage& dst, ErasedStorage& src)
  //{
  //   if (src.m_relocate)
  //   {
  //     src.m_relocate(dst, src);
  //   }
  // }

  template<typename ErasedType>
  static constexpr void relocateStorage(ErasedStorage& dst, ErasedStorage& src)
  {
    moveStorage<ErasedType>(dst, src);
    destroyStorage<ErasedType>(src);
  }

  static inline constexpr void copy(ErasedStorage& dst, const ErasedStorage& src)
  {
    if (src.m_storageFcts.m_copy)
    {
      src.m_storageFcts.m_copy(dst, src);
    }
    else
    {
      std::memcpy(dst.m_storage, src.m_storage, size);
    }

    dst.m_storageFcts = src.m_storageFcts;
  }

  template<typename ErasedType>
  static constexpr void copyStorage(ErasedStorage& dst, const ErasedStorage& src) noexcept(
      std::is_trivially_copyable_v<ErasedType> || noexcept(std::construct_at(dst.asTypedPtr<ErasedType>(), *src.asTypedPtr<ErasedType>())))
  {
    std::memset(dst.m_storage, 0, size);
    std::construct_at(dst.asTypedPtr<ErasedType>(), *src.asTypedPtr<ErasedType>());
  }

  TEST_FIXTURE_FRIEND_NS(CoreTests, ErasedStorageF);
  friend class TestUtilities::TestFriend;

  friend StorageFcts<ErasedStorage>;
  StorageFcts<ErasedStorage> m_storageFcts;

  static_assert(size > 0, "Size must be bigger that 0.");
  alignas(alignment) std::byte m_storage[size] = {};
};

} // namespace Core

module :private;
