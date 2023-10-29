#pragma once

#include<type_traits>

template<typename... Types>
struct Overloaded: Types...
{
  using Types::operator()...;
};

template<typename... Types>
Overloaded(Types&&...) -> Overloaded<std::decay_t<Types>...>;
