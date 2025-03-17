#include <iostream>

#define PRINT(...) \
  std::cout << "\033[36m" #__VA_ARGS__ ":\033[0m " << (__VA_ARGS__) << std::endl;

#include <vector>
#include <list>

template <typename T>
struct is_vector_1 {
  static constexpr bool value = false;
};

template <typename T>
struct is_vector_1<std::vector<T>> {
  static constexpr bool value = true;
};

#include <type_traits>

template <typename T>
struct is_vector_2: std::false_type { };

template <typename T>
struct is_vector_2<std::vector<T>>: std::true_type { };

// Variable template (since C++14)

template <typename T>
constexpr bool is_vector_3 = false;

template <typename T>
constexpr bool is_vector_3<std::vector<T>> = true;


template <typename T>
struct type_constant { using type = T; };

template <typename T>
constexpr bool is_vector_f(type_constant<T>) { return false; }

template <typename T>
constexpr bool is_vector_f(type_constant<std::vector<T>>) { return true; }

static_assert( !is_vector_f(type_constant<int>{}) );
static_assert( !is_vector_f(type_constant<std::list  <int>>{}) );
static_assert(  is_vector_f(type_constant<std::vector<int>>{}) );

// template <
//   class T,
//   class Allocator = std::allocator<T>
// > class vector;

template <typename T>
struct MyAllocator {
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using size_type = size_t;
  using difference_type = ptrdiff_t;

  T* allocate(size_t n) {
    return static_cast<T*>(malloc(n * sizeof(T)));
  }

  void deallocate(T* p, size_t n) {
    free(p);
  }
};

template <typename T>
constexpr bool is_vector_4 = std::is_same_v<
  T,
  std::vector<
    typename T::value_type,
    typename T::allocator_type
  >
>;

template <typename T>
constexpr bool is_vector_5 = std::is_same_v<
  std::decay_t<T>,
  std::vector<
    typename std::decay_t<T>::value_type,
    typename std::decay_t<T>::allocator_type
  >
>;

// template <typename T>
// struct is_vector_6<T, std::enable_if_t<!std::is_class_v<std::decay_t<T>>>>
// : std::false_type { };

template <typename, typename = void>
struct is_vector_6: std::false_type { };

template <typename T>
struct is_vector_6<
  T,
  std::void_t<
    typename std::decay_t<T>::value_type,
    typename std::decay_t<T>::allocator_type
  >
>
: std::is_same<
  std::decay_t<T>,
  std::vector<
    typename std::decay_t<T>::value_type,
    typename std::decay_t<T>::allocator_type
  >
>
{ };

/*
template <typename>
struct is_vector_6: std::false_type { };

template <typename T>
requires requires {
    typename std::decay_t<T>::value_type;
    typename std::decay_t<T>::allocator_type;
}
struct is_vector_6<T>
: std::is_same<
  std::decay_t<T>,
  std::vector<
    typename std::decay_t<T>::value_type,
    typename std::decay_t<T>::allocator_type
  >
>
{ };
*/

template <typename T>
constexpr bool is_vector_6_v = is_vector_6<T>::value;

static_assert( !is_vector_6_v< int > );
static_assert( !is_vector_6_v< std::list  <int>  > );
static_assert(  is_vector_6_v< std::vector<int>  > );
static_assert(  is_vector_6_v< std::vector<int>& > );
static_assert(  is_vector_6_v< std::vector<int, MyAllocator<int>> > );

/*
template <typename>
constexpr bool is_vector_7_v = false;

template <typename T>
requires requires {
    typename std::decay_t<T>::value_type;
    typename std::decay_t<T>::allocator_type;
}
constexpr bool is_vector_7_v = std::is_same_v<
  std::decay_t<T>,
  std::vector<
    typename std::decay_t<T>::value_type,
    typename std::decay_t<T>::allocator_type
  >
>;

static_assert( !is_vector_7_v< int > );
static_assert( !is_vector_7_v< std::list  <int>  > );
static_assert(  is_vector_7_v< std::vector<int>  > );
static_assert(  is_vector_7_v< std::vector<int>& > );
static_assert(  is_vector_7_v< std::vector<int, MyAllocator<int>> > );
*/

int main() {
  std::cout << std::boolalpha;

  PRINT(is_vector_1<int>::value)
  PRINT(is_vector_1<std::vector<int>>::value)
  puts("");

  PRINT(is_vector_2<int>::value)
  PRINT(is_vector_2<std::vector<int>>::value)
  puts("");

  PRINT(is_vector_3<int>)
  PRINT(is_vector_3<std::vector<int>>)
  puts("");

  PRINT(is_vector_1<std::vector<int, MyAllocator<int>>>::value)
  PRINT(is_vector_2<std::vector<int, MyAllocator<int>>>::value)
  PRINT(is_vector_3<std::vector<int, MyAllocator<int>>>)
  puts("");

  // PRINT(is_vector_4<int>)
  PRINT(is_vector_4<std::vector<int>>)
  PRINT(is_vector_4<std::vector<int, MyAllocator<int>>>)
  puts("");

  PRINT(is_vector_1<std::vector<int>&>::value)
  PRINT(is_vector_2<std::vector<int>&>::value)
  PRINT(is_vector_3<std::vector<int>&>)
  // PRINT(is_vector_4<std::vector<int>&>)
  PRINT(is_vector_5<std::vector<int>&>)
  puts("");

  PRINT(is_vector_6_v<int>)
  PRINT(is_vector_6_v<std::array<int,2>>)
  PRINT(is_vector_6_v<std::list<int>>)
  PRINT(is_vector_6_v<std::vector<int>>)
  PRINT(is_vector_6_v<std::vector<int, MyAllocator<int>>>)
  PRINT(is_vector_6_v<std::vector<int>&>)
  puts("");
}
