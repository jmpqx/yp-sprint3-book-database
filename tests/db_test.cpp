#include "book_database.hpp"
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
    std::println("Author histogram: {}", histogram);
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

    auto histogram = bookdb::calculateGenreRatings(db);
    std::println("Genre ratings: {}", histogram);
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
    std::println("Average rating: {}", avg_rating);
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
    std::println("Sampled books: {}", sampled_books);
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
    std::println("Top ratings: {}", top_ratings);
}
