#pragma once

#include <print>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    using value_type = typename BookContainer::value_type;
    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;
    using size_type = typename BookContainer::size_type;
    using difference_type = typename BookContainer::difference_type;

    auto begin() { return books_.begin(); }
    auto end() { return books_.end(); }
    auto cbegin() const { return books_.cbegin(); }
    auto cend() const { return books_.cend(); }

    auto size() const { return books_.size(); }
    auto empty() const { return books_.empty(); }

    using AuthorContainer = std::unordered_set<std::string>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> init) {
        for (auto &book : init) {
            PushBack(book);
        }
    }

    auto operator==(const BookDatabase &other) const {
        if (size() != other.size()) {
            return false;
        }

        return books_ == other.books_ && authors_ == other.authors_;
    }

    auto operator[](size_type index) { return books_[index]; }
    auto operator[](size_type index) const { return books_[index]; }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    void PushBack(const value_type &book) {
        auto [it, is_inserted] = authors_.emplace(book.author);

        try {
            books_.push_back(value_type{*it, book.title, book.year, book.genre, book.rating, book.read_count});
        } catch (const std::exception &e) {
            authors_.erase(*it);
            throw;
        }
    }

    void EmplaceBack(const std::string_view author, const std::string &title, int year,
                     Genre genre, double rating, int read_count) {
        auto [it, is_inserted] = authors_.emplace(author);

        try {
            books_.emplace_back(*it, title, year, genre, rating, read_count);
        } catch (const std::exception &e) {
            authors_.erase(*it);
            throw;
        }
    }

    const BookContainer& GetBooks() const { return books_; }
    const AuthorContainer& GetAuthors() const { return authors_; }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
