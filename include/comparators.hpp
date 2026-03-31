#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    auto operator()(const Book &lhs, const Book &rhs) const { return lhs.author < rhs.author; }
};

struct LessByPopularity {
    auto operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count < rhs.read_count; }
};

struct LessByRating {
    auto operator()(const Book &lhs, const Book &rhs) const { return lhs.rating < rhs.rating; }
};

struct GreaterByRating {
    auto operator()(const Book &lhs, const Book &rhs) const { return lhs.rating > rhs.rating; }
};

}  // namespace bookdb::comp
