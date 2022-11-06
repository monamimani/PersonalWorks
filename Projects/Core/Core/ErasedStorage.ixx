module;

#include <concepts>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>

export module ErasedStorage;

import Core.Concepts;

namespace Core
{

template <typename ErasedStorage_t>
struct StorageFcts
{
  //~StorageFcts()
  //{
  //  resetFcts();
  //}

  template <typename ErasedType>
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

export template <std::size_t size, std::size_t alignment>
class ErasedStorage final
{
  template <typename Compound_T>
  using Base_T = std::remove_cv_t<std::remove_pointer_t<std::remove_cvref_t<Compound_T>>>;

  static inline constexpr auto SizeT = size;

public:
  constexpr ErasedStorage() noexcept = default;
  constexpr ~ErasedStorage()
  {
    erase();
  }

  ErasedStorage(const ErasedStorage& other)
  {
    copy(*this, other);
  }

  ErasedStorage(ErasedStorage&& other) noexcept(noexcept(m_storageFcts.m_move(*this, other)))
  {
    move(*this, other);
  }

  ErasedStorage& operator=(const ErasedStorage& other)
  {
    // erase();
    if (m_storageFcts.m_destroy)
    {
      m_storageFcts.m_destroy(*this);
    }
    copy(*this, other);

    return *this;
  }
  ErasedStorage& operator=(ErasedStorage&& other) noexcept(noexcept(m_storageFcts.m_move(*this, other)))
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

  template <typename ErasedType>
  requires std::constructible_from<ErasedType>
  constexpr auto* construct()
  {
    static_assert(sizeof(ErasedType) <= size);
    static_assert(alignof(ErasedType) <= alignment);

    auto* storagePtr = reinterpret_cast<ErasedType*>(std::addressof(m_storage));
    auto storageTypedPtr = std::construct_at(storagePtr);

    m_storageFcts.setFcts<ErasedType>();
    return storageTypedPtr;
  }

  // Clang format has issues with requires clause still
  // clang-format off
  template <typename ErasedType, typename... Args>
  requires std::constructible_from<ErasedType, Args...> &&(sizeof...(Args) > 0) && !std::same_as<Base_T<std::tuple_element_t<0, std::tuple<Args...>>>, Base_T<ErasedType>> 
  constexpr auto* construct(Args && ... args)
  // clang-format on
  {
    static_assert(sizeof(ErasedType) <= size);
    static_assert(alignof(ErasedType) <= alignment);

    auto* storagePtr = reinterpret_cast<ErasedType*>(std::addressof(m_storage));
    auto storageTypedPtr = std::construct_at(storagePtr, std::forward<Args>(args)...);

    m_storageFcts.setFcts<ErasedType>();
    return storageTypedPtr;
  }

  template <typename ErasedType>
  requires std::constructible_from<ErasedType, ErasedType&&>
  constexpr auto* construct(ErasedType&& obj)
  {
    static_assert(sizeof(ErasedType) <= size);
    static_assert(alignof(ErasedType) <= alignment);

    // This will have issues with multiple level of pointer indirection
    using RawType = std::remove_pointer_t<std::remove_reference_t<ErasedType>>;

    m_storageFcts.setFcts<ErasedType>();

    if constexpr (std::is_lvalue_reference_v<ErasedType>)
    {
      static_assert(std::is_lvalue_reference_v<ErasedType> == false, "Creating an erased type of a reference to T is illegal, use T* as the erased type.");
      std::unreachable();

      // This is jus to silence some compilation error, because of the static assert we don't return anything,
      // which the compiler will complain about.
      // using ReturnType = RawType*;
      // return ReturnType(nullptr);

      // At some point I thought this was valid, but I was confused by treating reference like pointers which they are not.
      // For example sizeof(TestStruct&) is the same as doing sizeof(TestStruct)
      // auto** storagePtr = reinterpret_cast<RawType**>(std::addressof(m_storage));
      // auto storageTypedPtr = std::construct_at(storagePtr, std::addressof(obj));
      // return *storageTypedPtr;
    }
    else if constexpr (std::is_rvalue_reference_v<ErasedType>)
    {
      auto* storagePtr = reinterpret_cast<RawType*>(std::addressof(m_storage));
      auto storageTypedPtr = std::construct_at(storagePtr, std::forward<ErasedType>(obj));
      return storageTypedPtr;
    }
    else if constexpr (std::is_pointer_v<ErasedType>)
    {
      auto** storagePtr = reinterpret_cast<RawType**>(m_storage);
      auto storageTypedPtr = std::construct_at(storagePtr, obj);
      return *storageTypedPtr;
    }
    else if constexpr (std::is_object_v<ErasedType>)
    {
      static_assert(std::is_object_v<ErasedType> == false, "We should not get here, other overload of construct should have been called.");
    }
    else
    {
      std::unreachable();
    }
  }

  template <typename ErasedType>
  constexpr auto asTypedPtr()
  {
    // This will have issues with multiple level of pointer indirection
    using RawType = std::remove_pointer_t<std::remove_reference_t<ErasedType>>;

    // TODO std::launder()

    if constexpr (std::is_lvalue_reference_v<ErasedType>)
    {
      static_assert(std::is_lvalue_reference_v<ErasedType> == false, "Creating an erased type of a reference to T is illegal, use T* as the erased type.");
      std::unreachable();
      // return reinterpret_cast<RawType**>(std::addressof(m_storage));
    }
    else if constexpr (std::is_rvalue_reference_v<ErasedType>)
    {
      return reinterpret_cast<RawType*>(std::addressof(m_storage));
    }
    else if constexpr (std::is_pointer_v<ErasedType>)
    {
      return reinterpret_cast<RawType**>(std::addressof(m_storage));
    }
    else if constexpr (std::is_object_v<ErasedType>)
    {
      return reinterpret_cast<RawType*>(std::addressof(m_storage));
    }
    else
    {
      std::unreachable();
    }
  }

  template <typename ErasedType>
  constexpr const auto asTypedPtr() const
  {
    // This will have issues with multiple level of pointer indirection
    using RawType = std::remove_pointer_t<std::remove_reference_t<ErasedType>>;

    // TODO std::launder()

    if constexpr (std::is_lvalue_reference_v<ErasedType>)
    {
      static_assert(std::is_lvalue_reference_v<ErasedType> == false, "Creating an erased type of a reference to T is illegal, use T* as the erased type.");
      std::unreachable();
      //return reinterpret_cast<RawType* const*>(std::addressof(m_storage));
    }
    else if constexpr (std::is_rvalue_reference_v<ErasedType>)
    {
      return reinterpret_cast<const RawType*>(std::addressof(m_storage));
    }
    else if constexpr (std::is_pointer_v<ErasedType>)
    {
      return reinterpret_cast<RawType* const*>(std::addressof(m_storage));
    }
    else if constexpr (std::is_object_v<ErasedType>)
    {
      return reinterpret_cast<const RawType*>(std::addressof(m_storage));
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
  template <typename ErasedType>
  static constexpr void destroyStorage(ErasedStorage& storage)
  {
    std::destroy_at(storage.asTypedPtr<ErasedType>());
  }

  static inline constexpr void move(ErasedStorage& dst, ErasedStorage& src) noexcept /*(noexcept(moveStorage<ErasedType>(dst, src)))*/
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

  template <typename ErasedType>
  static inline constexpr void moveStorage(ErasedStorage& dst, ErasedStorage& src) noexcept(std::is_trivially_copyable_v<ErasedType> ||
                                                                                            noexcept(std::construct_at(dst.asTypedPtr<ErasedType>(), std::move(*src.asTypedPtr<ErasedType>()))))
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

  template <typename ErasedType>
  static constexpr void relocateStorage(ErasedStorage& dst, ErasedStorage& src)
  {
    moveStorage<ErasedType>(dst, src);
    destroyStorage<ErasedType>(src);
  }

  static inline constexpr void copy(ErasedStorage& dst, const ErasedStorage& src) noexcept /*(noexcept(copyStorage<ErasedType>(dst, src)))*/
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

  template <typename ErasedType>
  static constexpr void copyStorage(ErasedStorage& dst, const ErasedStorage& src) noexcept(std::is_trivially_copyable_v<ErasedType> ||
                                                                                           noexcept(std::construct_at(dst.asTypedPtr<ErasedType>(), *src.asTypedPtr<ErasedType>())))
  {
    std::memset(dst.m_storage, 0, size);
    std::construct_at(dst.asTypedPtr<ErasedType>(), *src.asTypedPtr<ErasedType>());
  }

  alignas(alignment) std::byte m_storage[size] = {};

  friend StorageFcts<ErasedStorage>;
  StorageFcts<ErasedStorage> m_storageFcts;
};

} // namespace Core

module :private;