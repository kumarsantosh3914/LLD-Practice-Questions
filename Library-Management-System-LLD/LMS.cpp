#include <bits/stdc++.h>
using namespace std;

class Book
{
private:
    string title;
    string author;
    string ISBN;
    bool availability; // true if availability, false if checked out

public:
    // constructor to initialize book's information
    Book(string t, string a, string i)
    {
        title = t;
        author = a;
        ISBN = i;
        availability = true;
    }

    // function to check out a book
    void checkOut()
    {
        if (availability)
        {
            availability = false;
            cout << "Book has been checked out." << endl;
        }
        else
        {
            cout << "Book is not available." << endl;
        }
    }

    // function to return a book
    void returnBook()
    {
        if (!availability)
        {
            availability = true;
            cout << "Book has been returned." << endl;
        }
        else
        {
            cout << "Book has not been checked out." << endl;
        }
    }

    // function to get book's title
    string getTitle()
    {
        return title;
    }

    // function to get book's author
    string getAuthor()
    {
        return author;
    }

    // function to getBook's ISBN
    string getISBN()
    {
        return ISBN;
    }

    // function to check if book is available
    bool isAvailable()
    {
        return availability;
    }
};

class Library
{
private:
    vector<Book> books;

public:
    // function to add a book to the library
    void addBook(Book b)
    {
        books.push_back(b);
        cout << "Book has been added to the library." << endl;
    }

    // function to remove a book from the library
    void removeBook(string ISBN)
    {
        for (int i = 0; i < books.size(); i++)
        {
            if (books[i].getISBN() == ISBN)
            {
                books.erase(books.begin() + i);
                cout << "Book has been removed from the library." << endl;
                return;
            }
        }
        cout << "Book with ISBN" << ISBN << " not found in library." << endl;
    }

    // function to search for a book in the library
    Book *searchBook(string ISBN)
    {
        for (int i = 0; i < books.size(); i++)
        {
            if (books[i].getISBN() == ISBN)
            {
                return &books[i];
            }
        }
        cout << "Book with ISBN" << ISBN << " not found in library." << endl;
        return nullptr;
    }

    // function to display all books in the library
    void displayBooks()
    {
        cout << "Book in the library: " << endl;
        for (int i = 0; i < books.size(); i++)
        {
            cout << "Title: " << books[i].getTitle() << endl;
            cout << "Author: " << books[i].getAuthor() << endl;
            cout << "ISBN: " << books[i].getISBN() << endl;
            cout << "Availability: " << (books[i].isAvailable() ? "Available" : "Checked out") << endl;
            cout << endl;
        }
    }
};

class Patron
{
private:
    string name;
    string ID;
    vector<Book *> checkedOutBooks;

public:
    // constructor to initialize patron's information
    Patron(string n, string i)
    {
        name = n;
        ID = i;
    }

    // function to check out a book
    void checkOutBook(Book *book)
    {
        if (book->isAvailable())
        {
            book->checkOut();
            checkedOutBooks.push_back(book);
            cout << "Book has been checked out to " << name << "." << endl;
        }
        else
        {
            cout << "Book is not available." << endl;
        }
    }

    // function to return a book
    void returnBook(Book *book)
    {
        for (int i = 0; i < checkedOutBooks.size(); i++)
        {
            if (checkedOutBooks[i]->getISBN() == book->getISBN())
            {
                book->returnBook();
                checkedOutBooks.erase(checkedOutBooks.begin() + i);
                cout << "Book has been returned by" << name << "." << endl;
                return;
            }
        }
        cout << name << " did not check out this book." << endl;
    }

    // function to get patron's name
    string getName()
    {
        return name;
    }

    // function to get patron's ID
    string getID()
    {
        return ID;
    }
};

class Checkout
{
private:
    map<Book *, pair<Patron *, time_t>> checkouts;
    const int DAY_SECONDS = 60 * 60 * 24;
    const int MAX_FINE = 10;

public:
    // function to check out a book
    void checkOut(Book *book, Patron *patron)
    {
        if (book->isAvailable())
        {
            patron->checkOutBook(book);
            time_t dueDate = time(0) + 60 * 60 * 24 * 7; // 7 days from current time
            checkouts[book] = make_pair(patron, dueDate);
        }
        else
        {
            cout << "Book is not available." << endl;
        }
    }

    // function to return a book
    void returnBook(Book *book)
    {
        if (checkouts.count(book) == 1)
        {
            Patron *patron = checkouts[book].first;
            patron->returnBook(book);
            checkouts.erase(book);
        }
        else
        {
            cout << "Book has not been checked out." << endl;
        }
    }

    // function to display all current checkouts
    void displayCheckouts()
    {
        cout << "Current checkouts: " << endl;
        for (auto const &x : checkouts)
        {
            cout << "Book: " << x.first->getTitle() << endl;
            cout << "Author: " << x.first->getAuthor() << endl;
            cout << "Patron: " << x.second.first->getName() << endl;
            cout << "Due Date: " << ctime(&x.second.second);
            cout << endl;
        }
    }

    time_t getDueDate(Book *book)
    {
        if (checkouts.count(book) == 1)
        {
            return checkouts[book].second;
        }
        else
        {
            cout << "Book has not been checked out." << endl;
            return -1;
        }
    }

    // function to calculate the fine for a book that is overdue
    double calculateFine(Book *book)
    {
        time_t dueDate = this->getDueDate(book);
        time_t currentDate = time(0);
        double secondsLate = difftime(currentDate, dueDate);
        if (secondsLate < 0)
        {
            // book was returned early, no fine
            return 0.0;
        }
        else
        {
            // calculate fine based on how many days late the book is
            int daysLate = secondsLate / DAY_SECONDS;
            double fine = daysLate * 0.50;
            // cap the fine at MAX_FINE
            return min(fine, (double)MAX_FINE);
        }
    }
};

int main()
{
    // create objects of Library, Book, Patron, and Checkout classes
    Library library;
    Book book1("The Great Gatsby", "F.Scott Fitzgerald", "1234567890");
    Book book2("To Kill a Mockingbird", "Harper Lee", "0987654321");
    Patron patron1("John Smith", "123");
    Patron patron2("Jane Doe", "4234");
    Checkout checkout;

    // add books to the library
    library.addBook(book1);
    library.addBook(book2);

    // display all books in the library
    library.displayBooks();

    // check out a book
    checkout.checkOut(&book1, &patron1);

    // get the due date of the checked out book
    time_t dueDate = checkout.getDueDate(&book1);
    if (dueDate != 1)
    {
        cout << "Due Date: " << ctime(&dueDate) << endl;
    }

    // calculate and display the fine for the overdue book
    double fine = checkout.calculateFine(&book1);

    if (fine > 0)
    {
        cout << "Fine for overdue book: Rs" << fine << endl;
    }

    // display all current checkouts
    checkout.displayCheckouts();

    // return a book
    checkout.returnBook(&book1);

    // display all current checkouts
    checkout.displayCheckouts();

    return 0;
}