#include "book_database.hpp"
#include "filters.hpp"
#include "statsistics.hpp"
#include <gtest/gtest.h>

TEST(BookDatabase, DefaultConstructible) {
    bookdb::BookDatabase<> db;
    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.empty());
}

TEST(BookDatabase, CopyConstructible) {
    bookdb::BookDatabase<> db;
    auto copy_db(db);
    auto assign_db = db;

    EXPECT_EQ(db, copy_db);
    EXPECT_EQ(db, assign_db);
    EXPECT_EQ(copy_db, assign_db);
}

TEST(BookDatabase, InitListConstructible) {
    bookdb::BookDatabase<> db = {
        {"George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4., 190},
        {"George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143},
        {"F. Scott Fitzgerald", "The Great Gatsby", 1925, bookdb::Genre::Fiction, 4.5, 120},
    };

    EXPECT_EQ(db.size(), 3);
    EXPECT_FALSE(db.empty());
}

TEST(BookDatabase, PushBack) {
    bookdb::BookDatabase<> db;
    db.PushBack({"George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4., 190});
    db.PushBack({"George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143});
    db.PushBack({"F. Scott Fitzgerald", "The Great Gatsby", 1925, bookdb::Genre::Fiction, 4.5, 120});

    EXPECT_EQ(db.size(), 3);
    EXPECT_FALSE(db.empty());

    EXPECT_EQ(db[0].author, "George Orwell");
    EXPECT_EQ(db[0].title, "1984");
    EXPECT_EQ(db[0].year, 1949);
    EXPECT_EQ(db[0].genre, bookdb::Genre::SciFi);
    EXPECT_EQ(db[0].rating, 4.);
    EXPECT_EQ(db[0].read_count, 190);

    EXPECT_EQ(db[1].author, "George Orwell");
    EXPECT_EQ(db[1].title, "Animal Farm");
    EXPECT_EQ(db[1].year, 1945);
    EXPECT_EQ(db[1].genre, bookdb::Genre::Fiction);
    EXPECT_EQ(db[1].rating, 4.4);
    EXPECT_EQ(db[1].read_count, 143);

    EXPECT_EQ(db[2].author, "F. Scott Fitzgerald");
    EXPECT_EQ(db[2].title, "The Great Gatsby");
    EXPECT_EQ(db[2].year, 1925);
    EXPECT_EQ(db[2].genre, bookdb::Genre::Fiction);
    EXPECT_EQ(db[2].rating, 4.5);
    EXPECT_EQ(db[2].read_count, 120);
}

TEST(BookDatabase, EmplaceBack) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4., 190);
    db.EmplaceBack("George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143);
    db.EmplaceBack("F. Scott Fitzgerald", "The Great Gatsby", 1925, bookdb::Genre::Fiction, 4.5, 120);

    EXPECT_EQ(db.size(), 3);
    EXPECT_FALSE(db.empty());

    EXPECT_EQ(db[0].author, "George Orwell");
    EXPECT_EQ(db[0].title, "1984");
    EXPECT_EQ(db[0].year, 1949);
    EXPECT_EQ(db[0].genre, bookdb::Genre::SciFi);
    EXPECT_EQ(db[0].rating, 4.);
    EXPECT_EQ(db[0].read_count, 190);

    EXPECT_EQ(db[1].author, "George Orwell");
    EXPECT_EQ(db[1].title, "Animal Farm");
    EXPECT_EQ(db[1].year, 1945);
    EXPECT_EQ(db[1].genre, bookdb::Genre::Fiction);
    EXPECT_EQ(db[1].rating, 4.4);
    EXPECT_EQ(db[1].read_count, 143);

    EXPECT_EQ(db[2].author, "F. Scott Fitzgerald");
    EXPECT_EQ(db[2].title, "The Great Gatsby");
    EXPECT_EQ(db[2].year, 1925);
    EXPECT_EQ(db[2].genre, bookdb::Genre::Fiction);
    EXPECT_EQ(db[2].rating, 4.5);
    EXPECT_EQ(db[2].read_count, 120);
}

TEST(BookDatabase, SizeAndEmpty) {
    bookdb::BookDatabase<> db;
    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.empty());

    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4., 190);
    EXPECT_EQ(db.size(), 1);
    EXPECT_FALSE(db.empty());

    db.PushBack({"George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143});
    EXPECT_EQ(db.size(), 2);
    EXPECT_FALSE(db.empty());

    db.Clear();
    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.empty());
}

TEST(BookDatabase, PushBackDanglingAuthor) {
    bookdb::BookDatabase<> db;
    {
        bookdb::Book book1{"George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4., 190};
        db.PushBack(book1);

        bookdb::Book book2{"George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143};
        db.EmplaceBack(book2.author, book2.title, book2.year, book2.genre, book2.rating, book2.read_count);
    }

    EXPECT_EQ(db[0].author, "George Orwell");
    EXPECT_EQ(db[0].title, "1984");
    EXPECT_EQ(db[0].year, 1949);
    EXPECT_EQ(db[0].genre, bookdb::Genre::SciFi);
    EXPECT_EQ(db[0].rating, 4.);
    EXPECT_EQ(db[0].read_count, 190);

    EXPECT_EQ(db[1].author, "George Orwell");
    EXPECT_EQ(db[1].title, "Animal Farm");
    EXPECT_EQ(db[1].year, 1945);
    EXPECT_EQ(db[1].genre, bookdb::Genre::Fiction);
    EXPECT_EQ(db[1].rating, 4.4);
    EXPECT_EQ(db[1].read_count, 143);
}

TEST(BookDatabase, AuthorHistogram) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4., 190);
    db.EmplaceBack("George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143);
    db.EmplaceBack("F. Scott Fitzgerald", "The Great Gatsby", 1925, bookdb::Genre::Fiction, 4.5, 120);
    db.EmplaceBack("Leo Tolstoy", "War and Peace", 1867, bookdb::Genre::Fiction, 4.1, 156);
    db.EmplaceBack("Jane Austen", "Pride and Prejudice", 1813, bookdb::Genre::Fiction, 4.7, 178);
    db.EmplaceBack("Leo Tolstoy", "Anna Karenina", 1878, bookdb::Genre::Fiction, 3.9, 112);
    db.EmplaceBack("Aldous Huxley", "Brave New World", 1932, bookdb::Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Charlotte Brontë", "Jane Eyre", 1847, bookdb::Genre::Fiction, 4.6, 110);
    db.EmplaceBack("J.R.R. Tolkien", "The Hobbit", 1937, bookdb::Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Leo Tolstoy", "Childhood", 1852, bookdb::Genre::Biography, 3.8, 89);
    db.EmplaceBack("Leo Tolstoy", "Youth", 1856, bookdb::Genre::Biography, 3.7, 95);
    db.EmplaceBack("John Steinbeck", "Of Mice and Men", 1937, bookdb::Genre::NonFiction, 3.9, 56);
    db.EmplaceBack("Khaled Hosseini", "The Kite Runner", 2003, bookdb::Genre::NonFiction, 4.36, 358);

    auto histogram = bookdb::buildAuthorHistogramFlat(db);
    EXPECT_EQ(histogram.size(), 9);
    EXPECT_EQ(histogram["Leo Tolstoy"], 4);
    EXPECT_EQ(histogram["George Orwell"], 2);
    EXPECT_EQ(histogram["F. Scott Fitzgerald"], 1);
    EXPECT_EQ(histogram["Jane Austen"], 1);
    EXPECT_EQ(histogram["Aldous Huxley"], 1);
    EXPECT_EQ(histogram["Charlotte Brontë"], 1);
    EXPECT_EQ(histogram["J.R.R. Tolkien"], 1);
    EXPECT_EQ(histogram["John Steinbeck"], 1);
    EXPECT_EQ(histogram["Khaled Hosseini"], 1);
}

TEST(BookDatabase, CalculateGenreRatings) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4., 190);
    db.EmplaceBack("George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143);
    db.EmplaceBack("F. Scott Fitzgerald", "The Great Gatsby", 1925, bookdb::Genre::Fiction, 4.5, 120);
    db.EmplaceBack("Leo Tolstoy", "War and Peace", 1867, bookdb::Genre::Fiction, 4.1, 156);
    db.EmplaceBack("Jane Austen", "Pride and Prejudice", 1813, bookdb::Genre::Fiction, 4.7, 178);
    db.EmplaceBack("Leo Tolstoy", "Anna Karenina", 1878, bookdb::Genre::Fiction, 3.9, 112);
    db.EmplaceBack("Aldous Huxley", "Brave New World", 1932, bookdb::Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Charlotte Brontë", "Jane Eyre", 1847, bookdb::Genre::Fiction, 4.6, 110);
    db.EmplaceBack("J.R.R. Tolkien", "The Hobbit", 1937, bookdb::Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Leo Tolstoy", "Childhood", 1852, bookdb::Genre::Biography, 3.8, 89);
    db.EmplaceBack("Leo Tolstoy", "Youth", 1856, bookdb::Genre::Biography, 3.7, 95);
    db.EmplaceBack("John Steinbeck", "Of Mice and Men", 1937, bookdb::Genre::NonFiction, 3.9, 56);
    db.EmplaceBack("Khaled Hosseini", "The Kite Runner", 2003, bookdb::Genre::NonFiction, 4.36, 358);

    auto genre_ratings = bookdb::calculateGenreRatings(db.begin(), db.end());
    EXPECT_EQ(genre_ratings.size(), 4);
    EXPECT_NEAR(genre_ratings[bookdb::Genre::Fiction], 31.1 / 7.0, 1e-6);
    EXPECT_NEAR(genre_ratings[bookdb::Genre::SciFi], 8.5 / 2.0, 1e-6);
    EXPECT_NEAR(genre_ratings[bookdb::Genre::Biography], 7.5 / 2.0, 1e-6);
    EXPECT_NEAR(genre_ratings[bookdb::Genre::NonFiction], 8.26 / 2.0, 1e-6);
    EXPECT_DOUBLE_EQ(genre_ratings[bookdb::Genre::Mystery], 0.0);
    EXPECT_DOUBLE_EQ(genre_ratings[bookdb::Genre::Unknown], 0.0);
}

TEST(BookDatabase, CalculateAverageRating) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4., 190);
    db.EmplaceBack("George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143);
    db.EmplaceBack("F. Scott Fitzgerald", "The Great Gatsby", 1925, bookdb::Genre::Fiction, 4.5, 120);
    db.EmplaceBack("Leo Tolstoy", "War and Peace", 1867, bookdb::Genre::Fiction, 4.1, 156);
    db.EmplaceBack("Jane Austen", "Pride and Prejudice", 1813, bookdb::Genre::Fiction, 4.7, 178);
    db.EmplaceBack("Leo Tolstoy", "Anna Karenina", 1878, bookdb::Genre::Fiction, 3.9, 112);
    db.EmplaceBack("Aldous Huxley", "Brave New World", 1932, bookdb::Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Charlotte Brontë", "Jane Eyre", 1847, bookdb::Genre::Fiction, 4.6, 110);
    db.EmplaceBack("J.R.R. Tolkien", "The Hobbit", 1937, bookdb::Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Leo Tolstoy", "Childhood", 1852, bookdb::Genre::Biography, 3.8, 89);
    db.EmplaceBack("Leo Tolstoy", "Youth", 1856, bookdb::Genre::Biography, 3.7, 95);
    db.EmplaceBack("John Steinbeck", "Of Mice and Men", 1937, bookdb::Genre::NonFiction, 3.9, 56);
    db.EmplaceBack("Khaled Hosseini", "The Kite Runner", 2003, bookdb::Genre::NonFiction, 4.36, 358);

    auto avg_rating = bookdb::calculateAverageRating(db);
    EXPECT_NEAR(avg_rating, 55.36 / 13.0, 1e-6);
}

TEST(BookDatabase, sampleRandomBooks) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4., 190);
    db.EmplaceBack("George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143);
    db.EmplaceBack("F. Scott Fitzgerald", "The Great Gatsby", 1925, bookdb::Genre::Fiction, 4.5, 120);
    db.EmplaceBack("Leo Tolstoy", "War and Peace", 1867, bookdb::Genre::Fiction, 4.1, 156);
    db.EmplaceBack("Jane Austen", "Pride and Prejudice", 1813, bookdb::Genre::Fiction, 4.7, 178);
    db.EmplaceBack("Leo Tolstoy", "Anna Karenina", 1878, bookdb::Genre::Fiction, 3.9, 112);
    db.EmplaceBack("Aldous Huxley", "Brave New World", 1932, bookdb::Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Charlotte Brontë", "Jane Eyre", 1847, bookdb::Genre::Fiction, 4.6, 110);
    db.EmplaceBack("J.R.R. Tolkien", "The Hobbit", 1937, bookdb::Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Leo Tolstoy", "Childhood", 1852, bookdb::Genre::Biography, 3.8, 89);
    db.EmplaceBack("Leo Tolstoy", "Youth", 1856, bookdb::Genre::Biography, 3.7, 95);
    db.EmplaceBack("John Steinbeck", "Of Mice and Men", 1937, bookdb::Genre::NonFiction, 3.9, 56);
    db.EmplaceBack("Khaled Hosseini", "The Kite Runner", 2003, bookdb::Genre::NonFiction, 4.36, 358);

    auto sampled_books = bookdb::sampleRandomBooks(db, 3);
    EXPECT_EQ(sampled_books.size(), 3);
    for (const auto &ref : sampled_books) {
        bool found = std::any_of(db.cbegin(), db.cend(), [&](const auto &book) { return book == ref.get(); });
        EXPECT_TRUE(found);
    }
}

TEST(BookDatabase, getTopNByRating) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4., 190);
    db.EmplaceBack("George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143);
    db.EmplaceBack("F. Scott Fitzgerald", "The Great Gatsby", 1925, bookdb::Genre::Fiction, 4.5, 120);
    db.EmplaceBack("Leo Tolstoy", "War and Peace", 1867, bookdb::Genre::Fiction, 4.1, 156);
    db.EmplaceBack("Jane Austen", "Pride and Prejudice", 1813, bookdb::Genre::Fiction, 4.7, 178);
    db.EmplaceBack("Leo Tolstoy", "Anna Karenina", 1878, bookdb::Genre::Fiction, 3.9, 112);
    db.EmplaceBack("Aldous Huxley", "Brave New World", 1932, bookdb::Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Charlotte Brontë", "Jane Eyre", 1847, bookdb::Genre::Fiction, 4.6, 110);
    db.EmplaceBack("J.R.R. Tolkien", "The Hobbit", 1937, bookdb::Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Leo Tolstoy", "Childhood", 1852, bookdb::Genre::Biography, 3.8, 89);
    db.EmplaceBack("Leo Tolstoy", "Youth", 1856, bookdb::Genre::Biography, 3.7, 95);
    db.EmplaceBack("John Steinbeck", "Of Mice and Men", 1937, bookdb::Genre::NonFiction, 3.9, 56);
    db.EmplaceBack("Khaled Hosseini", "The Kite Runner", 2003, bookdb::Genre::NonFiction, 4.36, 358);

    auto top_ratings = bookdb::getTopNBy(db, 5);
    EXPECT_EQ(top_ratings.size(), 5);
    EXPECT_DOUBLE_EQ(top_ratings[0].get().rating, 4.9);
    EXPECT_DOUBLE_EQ(top_ratings[1].get().rating, 4.7);
    EXPECT_DOUBLE_EQ(top_ratings[2].get().rating, 4.6);
    EXPECT_DOUBLE_EQ(top_ratings[3].get().rating, 4.5);
    EXPECT_DOUBLE_EQ(top_ratings[4].get().rating, 4.5);
}

TEST(BookDatabase, FilterBooks) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4., 190);
    db.EmplaceBack("George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143);
    db.EmplaceBack("F. Scott Fitzgerald", "The Great Gatsby", 1925, bookdb::Genre::Fiction, 4.5, 120);
    db.EmplaceBack("Leo Tolstoy", "War and Peace", 1867, bookdb::Genre::Fiction, 4.1, 156);
    db.EmplaceBack("Jane Austen", "Pride and Prejudice", 1813, bookdb::Genre::Fiction, 4.7, 178);
    db.EmplaceBack("Leo Tolstoy", "Anna Karenina", 1878, bookdb::Genre::Fiction, 3.9, 112);
    db.EmplaceBack("Aldous Huxley", "Brave New World", 1932, bookdb::Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Charlotte Brontë", "Jane Eyre", 1847, bookdb::Genre::Fiction, 4.6, 110);
    db.EmplaceBack("J.R.R. Tolkien", "The Hobbit", 1937, bookdb::Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Leo Tolstoy", "Childhood", 1852, bookdb::Genre::Biography, 3.8, 89);
    db.EmplaceBack("Leo Tolstoy", "Youth", 1856, bookdb::Genre::Biography, 3.7, 95);
    db.EmplaceBack("John Steinbeck", "Of Mice and Men", 1937, bookdb::Genre::NonFiction, 3.9, 56);
    db.EmplaceBack("Khaled Hosseini", "The Kite Runner", 2003, bookdb::Genre::NonFiction, 4.36, 358);

    auto filter = bookdb::filters::all_of(bookdb::filters::YearBetween(1900, 1950), bookdb::filters::RatingAbove(4.5));
    auto filtered_books = bookdb::filterBooks(db.begin(), db.end(), filter);
    EXPECT_EQ(filtered_books.size(), 3);
    EXPECT_EQ(filtered_books[0].get().title, "The Great Gatsby");
    EXPECT_EQ(filtered_books[1].get().title, "Brave New World");
    EXPECT_EQ(filtered_books[2].get().title, "The Hobbit");
}

TEST(BookDatabase, BuildAuthorHistogramEmpty) {
    bookdb::BookDatabase<> db;
    auto histogram = bookdb::buildAuthorHistogramFlat(db);
    EXPECT_TRUE(histogram.empty());
}

TEST(BookDatabase, BuildAuthorHistogramSingleAuthor) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("Author A", "Book 1", 2000, bookdb::Genre::Fiction, 4.0, 100);
    db.EmplaceBack("Author A", "Book 2", 2001, bookdb::Genre::Fiction, 4.5, 200);
    db.EmplaceBack("Author A", "Book 3", 2002, bookdb::Genre::SciFi, 3.5, 50);

    auto histogram = bookdb::buildAuthorHistogramFlat(db);
    EXPECT_EQ(histogram.size(), 1);
    EXPECT_EQ(histogram["Author A"], 3);
}

TEST(BookDatabase, CalculateAverageRatingSingleBook) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4.0, 190);
    auto avg = bookdb::calculateAverageRating(db);
    EXPECT_DOUBLE_EQ(avg, 4.0);
}

TEST(BookDatabase, SampleRandomBooksZero) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4.0, 190);
    auto sampled = bookdb::sampleRandomBooks(db, 0);
    EXPECT_TRUE(sampled.empty());
}

TEST(BookDatabase, SampleRandomBooksMoreThanSize) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4.0, 190);
    db.EmplaceBack("George Orwell", "Animal Farm", 1945, bookdb::Genre::Fiction, 4.4, 143);
    auto sampled = bookdb::sampleRandomBooks(db, 5);
    EXPECT_EQ(sampled.size(), 2);
}

TEST(BookDatabase, SampleRandomBooksFromEmpty) {
    bookdb::BookDatabase<> db;
    auto sampled = bookdb::sampleRandomBooks(db, 3);
    EXPECT_TRUE(sampled.empty());
}

TEST(BookDatabase, FilterBooksNoMatches) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4.0, 190);
    auto filter = bookdb::filters::RatingAbove(5.0);
    auto filtered = bookdb::filterBooks(db.begin(), db.end(), filter);
    EXPECT_TRUE(filtered.empty());
}

TEST(BookDatabase, FilterBooksEmptyDB) {
    bookdb::BookDatabase<> db;
    auto filter = bookdb::filters::RatingAbove(3.0);
    auto filtered = bookdb::filterBooks(db.begin(), db.end(), filter);
    EXPECT_TRUE(filtered.empty());
}

TEST(BookDatabase, FilterYearBetweenInvalidRange) {
    EXPECT_THROW(bookdb::filters::YearBetween(2000, 1900), std::invalid_argument);
}

TEST(BookDatabase, FilterRatingAboveInvalidRating) {
    EXPECT_THROW(bookdb::filters::RatingAbove(-1.0), std::invalid_argument);
    EXPECT_THROW(bookdb::filters::RatingAbove(6.0), std::invalid_argument);
}

TEST(BookDatabase, FilterRatingAboveBoundaryValues) {
    EXPECT_NO_THROW(bookdb::filters::RatingAbove(0.0));
    EXPECT_NO_THROW(bookdb::filters::RatingAbove(5.0));
}

TEST(BookDatabase, FilterAnyOf) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("Author A", "SciFi Book", 2000, bookdb::Genre::SciFi, 4.0, 100);
    db.EmplaceBack("Author B", "Fiction Book", 2001, bookdb::Genre::Fiction, 4.5, 200);
    db.EmplaceBack("Author C", "Biography Book", 2002, bookdb::Genre::Biography, 3.8, 89);

    auto filter = bookdb::filters::any_of(bookdb::filters::GenreIs(bookdb::Genre::SciFi),
                                          bookdb::filters::GenreIs(bookdb::Genre::Biography));
    auto filtered = bookdb::filterBooks(db.begin(), db.end(), filter);
    EXPECT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0].get().title, "SciFi Book");
    EXPECT_EQ(filtered[1].get().title, "Biography Book");
}

TEST(BookDatabase, FilterGenreIs) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("Author A", "SciFi Book", 2000, bookdb::Genre::SciFi, 4.0, 100);
    db.EmplaceBack("Author B", "Fiction Book", 2001, bookdb::Genre::Fiction, 4.5, 200);
    db.EmplaceBack("Author C", "Another SciFi", 2002, bookdb::Genre::SciFi, 3.5, 50);

    auto filter = bookdb::filters::GenreIs(bookdb::Genre::SciFi);
    auto filtered = bookdb::filterBooks(db.begin(), db.end(), filter);
    EXPECT_EQ(filtered.size(), 2);
    EXPECT_EQ(filtered[0].get().title, "SciFi Book");
    EXPECT_EQ(filtered[1].get().title, "Another SciFi");
}

TEST(BookDatabase, GetTopNByAllBooks) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("Author A", "Book A", 2000, bookdb::Genre::Fiction, 3.0, 100);
    db.EmplaceBack("Author B", "Book B", 2001, bookdb::Genre::Fiction, 5.0, 200);
    db.EmplaceBack("Author C", "Book C", 2002, bookdb::Genre::Fiction, 4.0, 150);

    auto top = bookdb::getTopNBy(db, 3);
    EXPECT_EQ(top.size(), 3);
    EXPECT_DOUBLE_EQ(top[0].get().rating, 5.0);
    EXPECT_DOUBLE_EQ(top[1].get().rating, 4.0);
    EXPECT_DOUBLE_EQ(top[2].get().rating, 3.0);
}

TEST(BookDatabase, GetTopNBySingleBook) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("Author A", "Book A", 2000, bookdb::Genre::Fiction, 4.5, 100);

    auto top = bookdb::getTopNBy(db, 1);
    EXPECT_EQ(top.size(), 1);
    EXPECT_EQ(top[0].get().title, "Book A");
}

TEST(BookDatabase, ClearAndReuse) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("George Orwell", "1984", 1949, bookdb::Genre::SciFi, 4.0, 190);
    EXPECT_EQ(db.size(), 1);

    db.Clear();
    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.empty());

    db.EmplaceBack("Jane Austen", "Pride and Prejudice", 1813, bookdb::Genre::Fiction, 4.7, 178);
    EXPECT_EQ(db.size(), 1);
    EXPECT_EQ(db[0].author, "Jane Austen");
    EXPECT_EQ(db[0].title, "Pride and Prejudice");
}

TEST(BookDatabase, InitListEmpty) {
    bookdb::BookDatabase<> db = {};
    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.empty());
}

TEST(BookDatabase, EqualityAfterSameInsertions) {
    bookdb::BookDatabase<> db1;
    bookdb::BookDatabase<> db2;
    db1.EmplaceBack("Author A", "Book A", 2000, bookdb::Genre::Fiction, 4.0, 100);
    db2.EmplaceBack("Author A", "Book A", 2000, bookdb::Genre::Fiction, 4.0, 100);
    EXPECT_EQ(db1, db2);
}

TEST(BookDatabase, InequalityDifferentBooks) {
    bookdb::BookDatabase<> db1;
    bookdb::BookDatabase<> db2;
    db1.EmplaceBack("Author A", "Book A", 2000, bookdb::Genre::Fiction, 4.0, 100);
    db2.EmplaceBack("Author B", "Book B", 2001, bookdb::Genre::SciFi, 3.5, 50);
    EXPECT_NE(db1, db2);
}

TEST(BookDatabase, InequalityDifferentSizes) {
    bookdb::BookDatabase<> db1;
    bookdb::BookDatabase<> db2;
    db1.EmplaceBack("Author A", "Book A", 2000, bookdb::Genre::Fiction, 4.0, 100);
    EXPECT_NE(db1, db2);
}

TEST(BookDatabase, FilterAllOfNoPredicates) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("Author A", "Book A", 2000, bookdb::Genre::Fiction, 4.0, 100);

    auto filter = bookdb::filters::all_of();
    auto filtered = bookdb::filterBooks(db.begin(), db.end(), filter);
    EXPECT_EQ(filtered.size(), 1);
}

TEST(BookDatabase, FilterAllMatchesReturnAll) {
    bookdb::BookDatabase<> db;
    db.EmplaceBack("Author A", "Book A", 2000, bookdb::Genre::Fiction, 4.0, 100);
    db.EmplaceBack("Author B", "Book B", 2001, bookdb::Genre::Fiction, 4.5, 200);

    auto filter = bookdb::filters::RatingAbove(3.0);
    auto filtered = bookdb::filterBooks(db.begin(), db.end(), filter);
    EXPECT_EQ(filtered.size(), 2);
}

TEST(BookDatabase, GenreToStringAndBack) {
    EXPECT_EQ(bookdb::GenreFromString("Fiction"), bookdb::Genre::Fiction);
    EXPECT_EQ(bookdb::GenreFromString("NonFiction"), bookdb::Genre::NonFiction);
    EXPECT_EQ(bookdb::GenreFromString("SciFi"), bookdb::Genre::SciFi);
    EXPECT_EQ(bookdb::GenreFromString("Biography"), bookdb::Genre::Biography);
    EXPECT_EQ(bookdb::GenreFromString("Mystery"), bookdb::Genre::Mystery);
    EXPECT_EQ(bookdb::GenreFromString("Unknown"), bookdb::Genre::Unknown);

    EXPECT_EQ(bookdb::GenreToString(bookdb::Genre::Fiction), "Fiction");
    EXPECT_EQ(bookdb::GenreToString(bookdb::Genre::NonFiction), "NonFiction");
    EXPECT_EQ(bookdb::GenreToString(bookdb::Genre::SciFi), "SciFi");
    EXPECT_EQ(bookdb::GenreToString(bookdb::Genre::Biography), "Biography");
    EXPECT_EQ(bookdb::GenreToString(bookdb::Genre::Mystery), "Mystery");
    EXPECT_EQ(bookdb::GenreToString(bookdb::Genre::Unknown), "Unknown");
}

TEST(BookDatabase, GenreFromStringUnknownDefault) {
    auto genre = bookdb::GenreFromString("InvalidGenre");
    EXPECT_EQ(genre, bookdb::Genre{});
}

TEST(BookDatabase, BookEquality) {
    bookdb::Book a{"Author", "Title", 2000, bookdb::Genre::Fiction, 4.0, 100};
    bookdb::Book b{"Author", "Title", 2000, bookdb::Genre::Fiction, 4.0, 100};
    bookdb::Book c{"Other", "Title", 2000, bookdb::Genre::Fiction, 4.0, 100};
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

TEST(BookDatabase, BookConstructFromStringGenre) {
    bookdb::Book book{"Author", "Title", 2000, "SciFi", 4.0, 100};
    EXPECT_EQ(book.genre, bookdb::Genre::SciFi);
}
