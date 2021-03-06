#pragma once
#ifndef BE_CORE_MEMORY_HPP_
#define BE_CORE_MEMORY_HPP_
#ifndef BE_CORE_BE_HPP_
#include "be.hpp"
#endif
#include BE_NATIVE_CORE(memory.hpp)
#include <memory>

namespace be {
namespace immovable_detail {  // protection from unintended ADL

class Immovable {
protected:
   constexpr Immovable() = default;
   ~Immovable() = default;
   Immovable(const Immovable&) = delete;
   Immovable& operator=(const Immovable&) = delete;
};

class Movable {
protected:
   constexpr Movable() = default;
   ~Movable() = default;
   Movable(const Movable&) = delete;
   Movable(Movable&&) = default;
   Movable& operator=(const Movable&) = delete;
   Movable& operator=(Movable&&) = default;
};

} // be::immovable_detail

using Immovable = immovable_detail::Immovable;
using Movable = immovable_detail::Movable;

using UC = unsigned char;
using SC = signed char;

using UI = unsigned int;
using SI = signed int;

namespace bo {
// Byte-order conversion

template <typename T, std::size_t N = sizeof(T), bool = std::is_integral<T>::value, bool = std::is_floating_point<T>::value>
struct Converter;

} // be::bo

template <typename T>
SV type_name() noexcept {
   using namespace std::string_view_literals;
   SV name = SV(typeid(T).name());
   SV test = "class "sv;
   if (name.substr(0, test.size()) == test) {
      name.remove_prefix(test.size());
   } else {
      test = "struct "sv;
      if (name.substr(0, test.size()) == test) {
         name.remove_prefix(test.size());
      }
   }
   return name;
}

template <std::uintptr_t A, typename T>
constexpr T* aligned_ptr(T* ptr) {
   std::uintptr_t u = reinterpret_cast<std::uintptr_t>(ptr);
   static_assert(0ull == (A & (A - std::uintptr_t(1))), "Alignment must be a power of 2");
   return reinterpret_cast<T*>((u - std::uintptr_t(1)) & ~(A - std::uintptr_t(1)) + A);
}

template <typename T>
T* aligned_ptr(T* ptr, std::uintptr_t alignment) {
   std::uintptr_t u = reinterpret_cast<std::uintptr_t>(ptr);
   assert(std::uintptr_t(0) == (alignment & (alignment - std::uintptr_t(1))));
   return reinterpret_cast<T*>(((u - std::uintptr_t(1)) & ~(alignment - std::uintptr_t(1))) + alignment);
}

template <std::size_t A, typename T>
constexpr T aligned_size(T size) {
   static_assert(std::is_unsigned<T>::value, "Type must be unsigned");
   static_assert(0ull == (A & (A - std::size_t(1))), "Alignment must be a power of 2");
   return T(((std::size_t(size) - std::size_t(1)) & ~(A - std::size_t(1))) + A);
}

template <typename T>
T aligned_size(T size, T alignment) {
   static_assert(std::is_unsigned<T>::value, "Type must be unsigned");
   assert(T(0) == (alignment & (alignment - T(1))));
   return ((size - T(1)) & ~(alignment - T(1))) + alignment;
}

//namespace detail {
//
//// TODO replace with a3 allocators
//template <typename T, std::size_t N, std::size_t A = alignof(T)> class NodeAllocator;
//template <typename T, std::size_t N, std::size_t A = alignof(T)> class ThreadNodeAllocator;
//template <typename T, std::size_t N, std::size_t A = alignof(T)> class SharedNodeAllocator;
//
//} // be::detail
//
//// Don't forget to include "be/detail/node_allocator.h", "be/detail/thread_node_allocator.h" or "be/detail/shared_node_allocator.h" when using these!
//template <typename T, std::size_t A = 0> using NodeAllocator = detail::NodeAllocator<T, 64, A>;
//template <typename T, std::size_t A = 0> using NodeAllocatorLarge = detail::NodeAllocator<T, 1024, A>;
//template <typename T, std::size_t A = 0> using ThreadNodeAllocator = detail::ThreadNodeAllocator<T, 64, A>;
//template <typename T, std::size_t A = 0> using ThreadNodeAllocatorLarge = detail::ThreadNodeAllocator<T, 1024, A>;
//template <typename T, std::size_t A = 0> using SharedNodeAllocator = detail::SharedNodeAllocator<T, 64, A>;
//template <typename T, std::size_t A = 0> using SharedNodeAllocatorLarge = detail::SharedNodeAllocator<T, 1024, A>;
//
//#define BE_ALLOC_SET(Type, Allocator)                       std::set<Type, std::less<Type>, Allocator<Type>>
//#define BE_ALLOC_MAP(KeyType, ValueType, Allocator)         std::map<KeyType, ValueType, std::less<KeyType>, Allocator<std::pair<KeyType const, ValueType>>>
//#define BE_ALLOC_MULTISET(Type, Allocator)                  std::multiset<Type, std::less<Type>, Allocator<Type>>
//#define BE_ALLOC_MULTIMAP(KeyType, ValueType, Allocator)    std::multimap<KeyType, ValueType, std::less<KeyType>, Allocator<std::pair<KeyType const, ValueType>>>
//
//#define BE_ALLOC_UO_SET(Type, Allocator)                    std::unordered_set<Type, std::hash<Type>, std::equal_to<Type>, Allocator<Type>>
//#define BE_ALLOC_UO_MAP(KeyType, ValueType, Allocator)      std::unordered_map<KeyType, ValueType, std::hash<KeyType>, std::equal_to<KeyType>, Allocator<std::pair<KeyType const, ValueType>>>
//#define BE_ALLOC_UO_MULTISET(Type, Allocator)               std::unordered_multiset<Type, std::hash<Type>, std::equal_to<Type>, Allocator<Type>>
//#define BE_ALLOC_UO_MULTIMAP(KeyType, ValueType, Allocator) std::unordered_multimap<KeyType, ValueType, std::hash<KeyType>, std::equal_to<KeyType>, Allocator<std::pair<KeyType const, ValueType>>>

} // be

#endif
