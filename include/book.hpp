#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

static constexpr std::string_view fiction_sv = "Fiction";
static constexpr std::string_view non_fiction_sv = "NonFiction";
static constexpr std::string_view sci_fi_sv = "SciFi";
static constexpr std::string_view biography_sv = "Biography";
static constexpr std::string_view mystery_sv = "Mystery";
static constexpr std::string_view unknown_sv = "Unknown";

constexpr std::string_view GenreToString(Genre g) {
    // clang-format off
    switch (g) {
        case Genre::Fiction:    return fiction_sv;
        case Genre::Mystery:    return mystery_sv;
        case Genre::NonFiction: return non_fiction_sv;
        case Genre::SciFi:      return sci_fi_sv;
        case Genre::Biography:  return biography_sv;
        case Genre::Unknown:    return unknown_sv;
        default:
            throw std::logic_error{"Unsupported bookdb::Genre"};
        }
    // clang-format on
}

constexpr Genre GenreFromString(std::string_view s) {
    if (s == fiction_sv) {
        return Genre::Fiction;
    } else if (s == mystery_sv) {
        return Genre::Mystery;
    } else if (s == non_fiction_sv) {
        return Genre::NonFiction;
    } else if (s == sci_fi_sv) {
        return Genre::SciFi;
    } else if (s == biography_sv) {
        return Genre::Biography;
    } else if (s == unknown_sv) {
        return Genre::Unknown;
    }

    return Genre{};
}

struct Book {
    using sv = std::string_view;
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    sv author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    constexpr Book(sv author, const std::string &title, int year, Genre genre, double rating, int read_count)
        : author(author), title(title), year(year), genre(genre), rating(rating), read_count(read_count) {
    }

    constexpr Book(sv author, const std::string &title, int year, sv genre, double rating, int read_count)
        : author(author), title(title), year(year), genre(GenreFromString(genre)), rating(rating), read_count(read_count) {
    }

    auto operator==(const Book &other) const {
        return author == other.author && title == other.title && year == other.year && genre == other.genre &&
               rating == other.rating && read_count == other.read_count;
    }
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        return format_to(fc.out(), "{}", bookdb::GenreToString(g));
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book &b, FormatContext &fc) const {
        return format_to(fc.out(), "{{author: {}, title: {}, year: {}, genre: {}, rating: {}, read_count: {}}}",
                         b.author, b.title, b.year, b.genre, b.rating, b.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();
    }
};

}  // namespace std
