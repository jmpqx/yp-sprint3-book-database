#pragma once

#include <algorithm>
#include <cassert>
#include <flat_map>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book_database.hpp"
#include "comparators.hpp"
#include "concepts.hpp"

#include <print>

namespace std {
template <typename Key, typename Tp, typename Comparator, typename KeyContainer, typename MappedContainer>
struct formatter<std::flat_map<Key, Tp, Comparator, KeyContainer, MappedContainer>, char> {
    template <typename FormatContext>
    auto format(const std::flat_map<Key, Tp, Comparator, KeyContainer, MappedContainer> &cont,
                FormatContext &fc) const {
        format_to(fc.out(), "{{\n");
        for (const auto &[key, value] : cont) {
            format_to(fc.out(), "{{{} -> {}}}\n", key, value);
        }
        format_to(fc.out(), "}}");

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

template <typename Tp, typename Allocator>
struct formatter<std::vector<Tp, Allocator>, char> {
    template <typename FormatContext>
    auto format(const std::vector<Tp, Allocator> &cont, FormatContext &fc) const {
        format_to(fc.out(), "{{\n");
        for (const auto &elem : cont) {
            format_to(fc.out(), "{{{}}}\n", elem);
        }
        format_to(fc.out(), "}}");

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

template <typename T>
struct formatter<std::reference_wrapper<T>, char> {
    template <typename FormatContext>
    auto format(const std::reference_wrapper<T> &ref, FormatContext &fc) const {
        return format_to(fc.out(), "{}", ref.get());
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};
}  // namespace std

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    using AuthorCount = std::pair<std::string_view, size_t>;
    std::vector<AuthorCount> number_of_occurrences;
    decltype(auto) authors = cont.GetAuthors();
    number_of_occurrences.reserve(authors.size());

    std::transform(authors.begin(), authors.end(), std::back_inserter(number_of_occurrences), [&](const auto &author) {
        auto count = std::count_if(cont.cbegin(), cont.cend(), [&](const auto &book) { return book.author == author; });
        return AuthorCount{author, count};
    });

    std::sort(number_of_occurrences.begin(), number_of_occurrences.end(),
              [&](const AuthorCount &lhs, const AuthorCount &rhs) { return comp(lhs.first, rhs.first); });

    std::flat_map<std::string_view, size_t, Comparator> histogram;
    std::vector<std::string_view> keys;
    keys.reserve(number_of_occurrences.size());
    std::vector<size_t> values;
    values.reserve(number_of_occurrences.size());

    std::for_each(number_of_occurrences.begin(), number_of_occurrences.end(), [&](const AuthorCount &ac) {
        keys.push_back(ac.first);
        values.push_back(ac.second);
    });

    histogram.replace(std::move(keys), std::move(values));

    return histogram;
}

template <BookIterator It, BookSentinel<It> Sent>
auto calculateGenreRatings(It begin, Sent end) {
    std::flat_map<Genre, double> genre_ratings;
    std::flat_map<Genre, size_t> genre_counts;

    std::for_each(begin, end, [&](const auto &book) {
        if (!genre_ratings.contains(book.genre)) {
            genre_ratings.try_emplace(book.genre, book.rating);
            genre_counts.try_emplace(book.genre, 1);
            return;
        }

        genre_ratings.at(book.genre) += book.rating;
        genre_counts.at(book.genre)++;
    });

    std::for_each(genre_ratings.begin(), genre_ratings.end(), [&](auto &&pair) {
        auto &[genre, rating] = pair;
        rating /= genre_counts.at(genre);
    });

    return genre_ratings;
}

template <BookContainerLike T>
auto calculateAverageRating(const BookDatabase<T> &cont) {
    auto avg_rating = std::transform_reduce(cont.cbegin(), cont.cend(), 0., std::plus<double>(),
                                            [](const auto &book) { return book.rating; }) /
                      cont.size();
    return avg_rating;
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t number) {
    std::vector<std::reference_wrapper<const Book>> result;
    result.reserve(number);

    std::sample(cont.cbegin(), cont.cend(), std::back_inserter(result), number, std::mt19937{std::random_device{}()});

    return result;
}

template <BookContainerLike T, BookComparator Comparator = comp::GreaterByRating>
auto getTopNBy(BookDatabase<T> &cont, size_t number, Comparator comp = {}) {
    std::partial_sort(cont.begin(), std::next(cont.begin(), number), cont.end(), comp);

    std::vector<std::reference_wrapper<const Book>> result{cont.begin(), std::next(cont.begin(), number)};

    return result;
}

}  // namespace bookdb
