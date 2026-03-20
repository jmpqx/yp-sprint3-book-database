#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = std::is_default_constructible_v<T> && std::is_copy_constructible_v<T> && requires(T c) {
    typename T::value_type;
    typename T::reference;
    typename T::const_reference;
    typename T::iterator;
    typename T::const_iterator;
    typename T::difference_type;
    typename T::size_type;

    { c.begin() } -> std::same_as<typename T::iterator>;
    { c.end() } -> std::same_as<typename T::iterator>;
    { c.cbegin() } -> std::same_as<typename T::const_iterator>;
    { c.cend() } -> std::same_as<typename T::const_iterator>;

    { c.size() } -> std::same_as<typename T::size_type>;
    { c.empty() } -> std::same_as<bool>;
};

template <typename T>
concept BookIterator = std::forward_iterator<T> && std::same_as<typename std::iterator_traits<T>::value_type, Book>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I> && std::same_as<typename std::iterator_traits<I>::value_type, Book>;

template <typename P>
concept BookPredicate = std::predicate<P, Book>;

template <typename C>
concept BookComparator = std::predicate<C, Book, Book>;

}  // namespace bookdb
