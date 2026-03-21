#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
namespace filters {
inline auto YearBetween(uint from, uint to) {
    if (from > to) {
        throw std::invalid_argument(std::format("Invalid year range: from {} > to {}", from, to));
    }

    return [from, to](const Book &book) { return book.year >= from && book.year <= to; };
}

inline auto RatingAbove(double rating) {
    if (rating < 0. || rating > 5.) {
        throw std::invalid_argument(std::format("Invalid rating: {}. Rating should be between 0 and 5.", rating));
    }

    return [rating](const Book &book) { return book.rating >= rating; };
}

inline auto GenreIs(Genre genre) {
    return [genre](const Book &book) { return book.genre == genre; };
}

template <BookPredicate... Predicates>
inline auto all_of(Predicates... preds) {
    return [preds...](const Book &book) { return (preds(book) && ...); };
}

template <BookPredicate... Predicates>
inline auto any_of(Predicates... preds) {
    return [preds...](const Book &book) { return (preds(book) || ...); };
}
}  // namespace filters

template <BookIterator It, BookSentinel<It> Sent, BookPredicate Predicate>
auto filterBooks(It first, Sent last, Predicate pred) {
    std::vector<std::reference_wrapper<const Book>> result;

    std::for_each(first, last, [&](const auto &book) {
        if (pred(book)) {
            result.emplace_back(book);
        }
    });

    return result;
}

}  // namespace bookdb