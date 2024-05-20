#include <bits/stdc++.h>
using namespace std;

class Book
{
private:
    int bookID;
    string title;
    string author;
    string genre;
    string content;

public:
    Book(int id, const string &bookTitle, const string &bookAuthor, const string &bookGenre, const string &bookContent)
        : bookID(id), title(bookTitle), author(bookAuthor), genre(bookGenre), content(bookContent) {}

    int getBookID() const
    {
        return bookID;
    }

    string getTitle() const
    {
        return title;
    }

    string getAuthor() const
    {
        return author;
    }

    string getGenre() const
    {
        return genre;
    }

    string getContent() const
    {
        return content;
    }
};

class Library
{
private:
    vector<Book> books;

public:
    void addBook(const Book &book)
    {
        books.push_back(book);
    }

    void removeBook(const Book &book)
    {
        // Find and remove the book from the library
        books.erase(remove_if(books.begin(), books.end(), [&](const Book &b)
                              { return b.getBookID() == book.getBookID(); }),
                    books.end());
    }

    vector<Book> searchByTitle(const string &title)
    {
        vector<Book> result;

        // Search for books with matching titles
        for (const Book &book : books)
        {
            if (book.getTitle() == title)
            {
                result.push_back(book);
            }
        }

        return result;
    }

    vector<Book> searchByAuthor(const string &author)
    {
        vector<Book> result;

        // Search for books with matching authors
        for (const Book &book : books)
        {
            if (book.getAuthor() == author)
            {
                result.push_back(book);
            }
        }

        return result;
    }
};

class User
{
private:
    int userID;
    string name;
    string email;
    string password;
    Library personalLibrary;

public:
    User(int id, const string &userName, const string &userEmail, const string &userPassword)
        : userID(id), name(userName), email(userEmail), password(userPassword) {}

    void addToUsersLibrary(const Book &book)
    {
        personalLibrary.addBook(book);
    }

    void removeFromUsersLibrary(const Book &book)
    {
        personalLibrary.removeBook(book);
    }

    vector<Book> searchByTitleInUserLibrary(const string &title)
    {
        return personalLibrary.searchByTitle(title);
    }

    vector<Book> searchByAuthorInUserLibrary(const string &author)
    {
        return personalLibrary.searchByAuthor(author);
    }
};

class OnlineBookReader
{
private:
    vector<User> users;
    Library onlineBookStore;

public:
    void registerUser(const User &user)
    {
        users.push_back(user);
    }

    User *loginUser(const string &email, const string &password)
    {
        for (User &user : users)
        {
            if (user.getEmail() == email && user.getPassword() == password)
            {
                return &user;
            }
        }
        return nullptr; // User not found
    }

    void addToUserLibrary(User *user, const Book &book)
    {
        if (user)
        {
            user->addToUsersLibrary(book);
        }
    }

    void removeFromUserLibrary(User *user, const Book &book)
    {
        if (user)
        {
            user->removeFromUsersLibrary(book);
        }
    }

    vector<Book> searchByTitleInBookStore(const string &title)
    {
        return onlineBookStore.searchByTitle(title);
    }

    vector<Book> searchByAuthorInBookStore(const string &author)
    {
        return onlineBookStore.searchByAuthor(author);
    }
};

int main()
{
    // Create some books
    Book book1(1, "Book1", "Author1", "Genre1", "Content1");
    Book book2(2, "Book2", "Author2", "Genre2", "Content2");
    Book book3(3, "Book3", "Author3", "Genre3", "Content3");

    // Create a library
    Library library;
    library.addBook(book1);
    library.addBook(book2);
    library.addBook(book3);

    // Create users
    User user1(1, "User1", "user1@example.com", "password1");
    User user2(2, "User2", "user2@example.com", "password2");

    // Add books to user's library
    user1.addToUsersLibrary(book1);
    user1.addToUsersLibrary(book2);
    user2.addToUsersLibrary(book3);

    // Search books by title
    vector<Book> booksByTitle = user1.searchByTitleInUserLibrary("Book2");
    cout << "Books found by title:" << endl;
    for (const Book &book : booksByTitle)
    {
        cout << "Title: " << book.getTitle() << ", Author: " << book.getAuthor() << endl;
    }

    // Search books by author
    vector<Book> booksByAuthor = user2.searchByAuthorInUserLibrary("Author3");
    cout << "Books found by author:" << endl;
    for (const Book &book : booksByAuthor)
    {
        cout << "Title: " << book.getTitle() << ", Author: " << book.getAuthor() << endl;
    }

    return 0;
}