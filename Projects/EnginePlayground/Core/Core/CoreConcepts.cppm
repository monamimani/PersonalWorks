module;
#include <concepts>
#include <cstddef>
#include <type_traits>

export module CoreConcepts;

export namespace Core
{

template<typename T>
concept NonCopyableC =
    !std::copy_constructible<T> && !std::assignable_from<T&, T&> && !std::assignable_from<T&, const T&> && !std::assignable_from<T&, const T>;

template<typename F, typename R, typename... ArgsT>
concept InvocableAndReturnC = (std::invocable<F, ArgsT...> && std::convertible_to<std::invoke_result_t<F&, ArgsT...>, R>);

template<auto f, typename R, typename... ArgsT>
concept InvocableAndReturnNTTPC = InvocableAndReturnC<decltype(f), R, ArgsT...>;

template<typename InstanceT, typename R, typename... ArgsT>
concept FunctorAndReturnC = InvocableAndReturnC<InstanceT, R, ArgsT...>;

// Those should probable be moved to a separate header, like a template meta programming and/or Type_traits.
/**
 * @brief A class to use to push around lists of types, nothing more.
 * @tparam Type Types provided by the type list.
 */
template<typename... Type>
struct TypeList
{
  /*! @brief Type list type. */
  // using type = TypeList;
  /*! @brief Compile-time number of elements in the type list. */
  static constexpr auto size = sizeof...(Type);
};

/*! @brief Primary template isn't defined on purpose. */
template<std::size_t, typename>
struct TypeListElement;

/**
 * @brief Provides compile-time indexed access to the types of a type list.
 * @tparam Index Index of the type to return.
 * @tparam Type First type provided by the type list.
 * @tparam Other Other types provided by the type list.
 */
template<std::size_t Index, typename Type, typename... Other>
struct TypeListElement<Index, TypeList<Type, Other...>>: TypeListElement<Index - 1u, TypeList<Other...>>
{};

/**
 * @brief Provides compile-time indexed access to the types of a type list.
 * @tparam Type First type provided by the type list.
 * @tparam Other Other types provided by the type list.
 */
template<typename Type, typename... Other>
struct TypeListElement<0u, TypeList<Type, Other...>>
{
  /*! @brief Searched type. */
  using TypeT = Type;
};

/**
 * @brief Helper type.
 * @tparam Index Index of the type to return.
 * @tparam List Type list to search into.
 */
template<std::size_t Index, typename ListT>
using TypeListElementT = typename TypeListElement<Index, ListT>::TypeT;


template<class From, class To>
using ConstAsT = std::conditional_t<std::is_const<From>{}, const To, To>;
template<class From, class To>
using VolatileAsT = std::conditional_t<std::is_volatile<From>{}, To volatile, To>;
template<class From, class To>
using ConstVolatileAsT = ConstAsT< From, VolatileAsT< From, To>>;

template<typename T>
concept IsImplicitLifetime = requires {
  std::is_scalar_v<T> || std::is_array_v<T> || (std::is_trivially_destructible_v<T> && std::is_trivially_constructible_v<T> && std::is_aggregate_v<T>);
};

} // namespace Core
