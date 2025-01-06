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

/**
 * @brief Transcribes the constness of a type to another type.
 * @tparam To The type to which to transcribe the constness.
 * @tparam From The type from which to transcribe the constness.
 */
template<typename To, typename From>
struct ConstAs
{
  /*! @brief The type resulting from the transcription of the constness. */
  using TypeT = std::remove_const_t<To>;
};

/*! @copydoc ConstAs */
template<typename To, typename From>
struct ConstAs<To, const From>
{
  /*! @brief The type resulting from the transcription of the constness. */
  using TypeT = std::add_const_t<To>;
};

/**
 * @brief Alias template to facilitate the transcription of the constness.
 * @tparam To The type to which to transcribe the constness.
 * @tparam From The type from which to transcribe the constness.
 */
template<typename To, typename From>
using ConstAsT = typename ConstAs<To, From>::type;

template<typename U, typename V>
using CopyConstnessC = std::conditional_t<std::is_const_v<std::remove_reference_t<U>>, const V, V>;

// NOLINTBEGIN(readability-identifier-naming)
template<typename T>
concept is_implicit_lifetime = requires {
  std::is_scalar_v<T> || std::is_array_v<T> || (std::is_trivially_destructible_v<T> && std::is_trivially_constructible_v<T> && std::is_aggregate_v<T>);
};
// NOLINTEND(readability-identifier-naming)

} // namespace Core
