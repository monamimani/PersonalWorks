module;
#include <concepts>

export module Core.Concepts;

export namespace Core
{
struct NonCopyable
{
  NonCopyable() = default;
  ~NonCopyable() = default;

  NonCopyable(NonCopyable&&) = default;
  NonCopyable& operator=(NonCopyable&&) = default;

  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};

template <typename T>
concept NonCopyable = !std::copy_constructible<T> && !std::assignable_from<T&, T&> && !std::assignable_from<T&, const T&> && !std::assignable_from<T&, const T>;

template <typename F, typename R, typename... Args>
concept InvocableAndReturn = (std::invocable<F, Args...> && std::convertible_to<std::invoke_result_t<F&, Args...>, R>);

template <auto f, typename R, typename... Args>
concept InvocableAndReturnNTTP = InvocableAndReturn<decltype(f), R, Args...>;

template <typename Instance_T, typename R, typename... Args>
concept FunctorAndReturn = InvocableAndReturn<Instance_T, R, Args...>;

// Those should probable be moved to a separate header, like a template meta programming and/or Type_traits.
/**
 * @brief A class to use to push around lists of types, nothing more.
 * @tparam Type Types provided by the type list.
 */
template <typename... Type>
struct type_list
{
  /*! @brief Type list type. */
  using type = type_list;
  /*! @brief Compile-time number of elements in the type list. */
  static constexpr auto size = sizeof...(Type);
};

/*! @brief Primary template isn't defined on purpose. */
template <std::size_t, typename>
struct type_list_element;

/**
 * @brief Provides compile-time indexed access to the types of a type list.
 * @tparam Index Index of the type to return.
 * @tparam Type First type provided by the type list.
 * @tparam Other Other types provided by the type list.
 */
template <std::size_t Index, typename Type, typename... Other>
struct type_list_element<Index, type_list<Type, Other...>> : type_list_element<Index - 1u, type_list<Other...>>
{
};

/**
 * @brief Provides compile-time indexed access to the types of a type list.
 * @tparam Type First type provided by the type list.
 * @tparam Other Other types provided by the type list.
 */
template <typename Type, typename... Other>
struct type_list_element<0u, type_list<Type, Other...>>
{
  /*! @brief Searched type. */
  using type = Type;
};

/**
 * @brief Helper type.
 * @tparam Index Index of the type to return.
 * @tparam List Type list to search into.
 */
template <std::size_t Index, typename List>
using type_list_element_t = typename type_list_element<Index, List>::type;

/**
 * @brief Transcribes the constness of a type to another type.
 * @tparam To The type to which to transcribe the constness.
 * @tparam From The type from which to transcribe the constness.
 */
template <typename To, typename From>
struct constness_as
{
  /*! @brief The type resulting from the transcription of the constness. */
  using type = std::remove_const_t<To>;
};

/*! @copydoc constness_as */
template <typename To, typename From>
struct constness_as<To, const From>
{
  /*! @brief The type resulting from the transcription of the constness. */
  using type = std::add_const_t<To>;
};

/**
 * @brief Alias template to facilitate the transcription of the constness.
 * @tparam To The type to which to transcribe the constness.
 * @tparam From The type from which to transcribe the constness.
 */
template <typename To, typename From>
using constness_as_t = typename constness_as<To, From>::type;

} // namespace Core