# C++ Style Guide Summary

The source of truth for the style guide is the clang-format style file `.clang-format` and the naming and check options defined in `.clang-tidy`.

## 1. Naming
- **General:** Optimize for readability. Be descriptive but concise. Use inclusive language.
- **Files:** PascalCase (`MyFile.h`, `MyFile.cpp`).
- **Namespaces:** CamelCase (`MyNamespace`).
- **Types:** CamelCase (`MyClass`, `MyStruct`, `MyEnum`, `MyAlias`). Use `int32_t` type family by default; use `<cstdint>` (`int32_t`) if size matters.
- **Concepts:** CamelCase with `C` suffix (`MyConceptC`).
- **Template Parameters:** CamelCase with `T` suffix (`MyTypeT`).
- **Functions & Methods:** camelBack (`myFunction()`, `myMethod()`).
- **Class Members:** `m_` prefix + camelBack (`m_myMember`).
- **Variables (Local/Global):** camelBack (`myVariable`).
- **Global Constants:** camelBack (`myGlobalConstant`).
- **Parameters:** camelBack (`myParameter`).
- **Enum Constants:** CamelCase (`MyEnumConstant`).
- **Macros:** ALL_CAPS (`MY_MACRO`).

## 2. Header Files
- **General:** Headers must be self-contained.
- **Guards:** Use `#pragma once`.
- **IWYU:** Direct includes only. Do not rely on transitive includes.
- **Forward Decls:** Avoid. Include headers instead. **Never** forward declare `std::` symbols.
- **Include Order:**
  1. Related header (main header for the `.cpp`)
  2. C system headers (`<.*\.h>`)
  3. C++ standard library headers (`<.*`)
  4. Other library/project headers. (`"base/logging.h"`)
  *Regrouping and alphabetical sorting within groups are enforced. Separate groups with blank lines.*

## 3. Formatting
- **Indentation:** 2 spaces.
- **Line Length:** 160 chars.
- **Non-ASCII:** Rare, use UTF-8. Avoid `u8` prefix if possible.
- **Literals:** Floating-point must have radix point (`1.0f`).
- **Braces:** Braces for functions, classes, namespaces, and control statements (`if`, `while`, `for`, `switch`) go on the **next line**.
- **Namespace Indentation:** Indent the content of namespaces (`NamespaceIndentation: Inner`).
- **Vertical Whitespace:** Use sparingly. Separate related chunks.
- **Return:** No parens `return result;`.
- **Pointers/References:** Left-aligned (`T* p`, `T& ref`).
- **Preprocessor:** Indent directives before the hash (`  #`).
- **Integer Literals:** Use digit separators (`1'000'000`, `0b1111'0000`).
- **Alignment:** Align operands after operators, align trailing comments.
- **Class Order:** `public`, `protected`, `private`.
- **Bin Packing:** Do not bin pack arguments (prefer one per line if they don't fit).

## 4. Classes
- **Ownership:** Use smart pointers for ownership management (`std::unique_ptr`, `std::shared_ptr`).
- **Constructors:** `explicit` for single-arg and conversion operators. **Exception:** `std::initializer_list`. No virtual calls in ctors. Use factories for fallible init.
- **Structs:** Only for passive data. Prefer `struct` over `std::pair` or `std::tuple`.
- **Rule of 5:** If you define one of the special member functions, define them all (can be `= default` or `= delete`).
- **Inheritance:** `public` only. Composition > Inheritance. Use `override` (omit `virtual`). No multiple implementation inheritance.
- **Operator Overloading:** Judicious use only. Binary ops as non-members. Never overload `&&`, `||`, `,`, or unary `&`. No User-Defined Literals.
- **Access:** Data members `private` (except structs/constants).
- **Declaration Order:** `public` before `protected` before `private`. Within sections: Types, Constants, Factory, Constructors, Destructor, Methods, Data Members.

## 5. Functions
- **Error Handling:** Prefer `std::expected` and coroutine-based error management (`co_yield`, `co_return`) over exceptions for logic errors.
- **Parameters:** Inputs (`const T&`, `std::string_view`, `std::span` or value) first, then outputs. **Ordering:** Inputs before outputs. Prefer return values for outputs.
- **Outputs:** Prefer return values/`std::optional`/`std::expected`. For non-optional outputs, use references. For optional outputs, use pointers.
- **Optional Inputs:** Use `std::optional` for by-value, `const T*` for reference.
- **Nonmember vs Static:** Prefer nonmember functions in namespaces over static member functions.
- **Length:** Prefer small (<40 lines).
- **Overloading:** Use only when behavior is obvious. Document overload sets with a single umbrella comment.
- **Default Args:** Allowed on non-virtual functions only (value must be fixed/constant).
- **Trailing Return:** Only when necessary (lambdas).

## 6. Scoping
- **Namespaces:** No `using namespace`. Use `using std::string`. Never add to `namespace std`.
- **Internal:** Use anonymous namespaces or `static` in `.cpp` files. Avoid in headers.
- **Locals:** Narrowest scope. Initialize at declaration. **Exception:** Declare complex objects outside loops.
- **Static/Global:** Must be **trivially destructible** (e.g., `constexpr`, raw pointers, arrays). No global `std::string`, `std::map`, smart pointers. Dynamic initialization allowed only for function-static variables.
- **Thread Local:** `thread_local` must be `constinit` if global. Prefer `thread_local` over other mechanisms.

## 7. Modern C++ Features
- **Version:** Target at least **C++23** but prefer the latest stable version if possible.
- **Standard Library:** Use modern C++ standard library features (`std::span`, `std::string_view`, `std::ranges`, `std::expected`).
- **Concepts:** Use C++20 Concepts to constrain template parameters over `std::enable_if`.
- **R-Value References:** Use only for move ctors/assignment, perfect forwarding, or consuming `*this`.
- **Coroutines:** Used for asynchronous operations and advanced error handling patterns.
- **Auto:** Use when the type is obvious or complex (e.g., iterators, `std::make_unique`).
- **CTAD:** Use only if explicitly supported (deduction guides exist).
- **Structured Bindings:** Use for pairs/tuples. Comment aliased field names.
- **Nullptr:** Use `nullptr`, never `NULL` or `0`.
- **Constexpr:** Use `constexpr`/`consteval` for constants/functions whenever possible. Use `constinit` for static initialization.
- **Noexcept:** Specify when useful/correct.
- **Lambdas:** Prefer explicit captures (`[&x]`) if escaping scope. Avoid `std::bind`.
- **Initialization:** Prefer brace init. **Designated Initializers:** Allowed.
- **Casts:** Use C++ casts (`static_cast`). Use `std::bit_cast` for type punning.
- **Loops:** Prefer range-based `for`.
- **Attributes:** Use `[[nodiscard]]` on functions that should not be ignored. Use `[[likely]]`/`[[unlikely]]` when appropriate.

## 8. Best Practices
- **Const:** Use `const` and `constexpr` whenever possible to enforce immutability and enable compile-time evaluation.
- **Exceptions:** Allowed, but not the primary mechanism for error management in core logic. `main` is allowed to throw.
- **Macros:** Avoid macros. Use `constexpr`, `inline`, or `template` instead.
- **0 and nullptr:** Use `nullptr` for pointers, `\0` for chars, not `0`.
- **Streams:** Use streams primarily for logging. Prefer printf-style formatting using `std::format`.
- **Types:** Avoid `unsigned` for non-negativity. No `long double`.
- **Pre-increment:** Prefer `++i` over `i++`.
- **Noexcept:** Use `noexcept` for functions that are guaranteed not to throw.
- **Sizeof:** Prefer `sizeof(varname)` over `sizeof(type)`.
- **Friends:** Allowed, usually defined in the same file.
- **Aliases:** Use `using` instead of `typedef`. Public aliases must be documented.
- **Ownership:** Single fixed owner. Transfer via smart pointers.
- **Aliases:** Document intent. Don't use in public API for convenience. `using` > `typedef`.
- **Switch:** Always include `default`. Use `[[fallthrough]]` for explicit fallthrough.
- **Comments:** Document File, Class, Function (params/return). Use `//` or `/* */`. Implementation comments for tricky code. `TODO(user):` format.
- **Consistency:** Follow existing project patterns and always check `.clang-format` and `.clang-tidy` warnings.

**BE CONSISTENT.** Follow existing code style.

*Source: .clang-format, .clang-tidy*
