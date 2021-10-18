#ifndef H5CORE_ENUM_OPERATORS_H
#define H5CORE_ENUM_OPERATORS_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include <type_traits>

#define ENABLE_BITMASK_OPERATORS(x)  \
  template<>                         \
  struct EnableBitMaskOperators<x>{  \
  static const bool enable = true;}; \

namespace h5geo
{
template<typename Enum>
struct EnableBitMaskOperators{
  static const bool enable = false;
};

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator |(Enum lhs, Enum rhs)
{
  using underlying = typename std::underlying_type<Enum>::type;
  return static_cast<Enum> (
        static_cast<underlying>(lhs) |
        static_cast<underlying>(rhs)
        );
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator &(Enum lhs, Enum rhs)
{
  using underlying = typename std::underlying_type<Enum>::type;
  return static_cast<Enum> (
        static_cast<underlying>(lhs) &
        static_cast<underlying>(rhs)
        );
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator ^(Enum lhs, Enum rhs)
{
  using underlying = typename std::underlying_type<Enum>::type;
  return static_cast<Enum> (
        static_cast<underlying>(lhs) ^
        static_cast<underlying>(rhs)
        );
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator ~(Enum rhs)
{
  using underlying = typename std::underlying_type<Enum>::type;
  return static_cast<Enum> (
        ~static_cast<underlying>(rhs)
        );
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator |=(Enum &lhs, Enum rhs)
{
  using underlying = typename std::underlying_type<Enum>::type;
  lhs = static_cast<Enum> (
      static_cast<underlying>(lhs) |
      static_cast<underlying>(rhs)
  );
  return lhs;
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator &=(Enum &lhs, Enum rhs)
{
  using underlying = typename std::underlying_type<Enum>::type;
  lhs = static_cast<Enum> (
      static_cast<underlying>(lhs) &
      static_cast<underlying>(rhs)
  );
  return lhs;
}

template<typename Enum>
typename std::enable_if<EnableBitMaskOperators<Enum>::enable, Enum>::type
operator ^=(Enum &lhs, Enum rhs)
{
  using underlying = typename std::underlying_type<Enum>::type;
  lhs = static_cast<Enum> (
      static_cast<underlying>(lhs) ^
      static_cast<underlying>(rhs)
  );
  return lhs;
}

}


#endif // H5CORE_ENUM_OPERATORS_H
