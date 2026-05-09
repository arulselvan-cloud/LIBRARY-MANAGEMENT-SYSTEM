#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

// ─────────────────────────────────────────────
//  Book Structure
// ─────────────────────────────────────────────
struct Book {
    int  bookId;
    char title[60];
    char author[40];
    char category[30];
    int  totalCopies;
    int  availableCopies;
};

// ─────────────────────────────────────────────
//  Member Structure
// ─────────────────────────────────────────────
struct Member {
    int  memberId;
    char name[50];
    char email[50];
    int  issuedBookId;   // 0 = no book issued
};

const string BOOK_FILE   = "books.dat";
const string MEMBER_FILE = "members.dat";

// ─────────────────────────────────────────────
//  Utility helpers
// ─────────────────────────────────────────────
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printHeader(const string& title) {
    cout << "\n  ╔══════════════════════════════════════════╗\n";
    cout << "  ║   " << left << setw(41) << title << "║\n";
    cout << "  ╚══════════════════════════════════════════╝\n\n";
}

bool bookIdExists(int id) {
    ifstream fin(BOOK_FILE, ios::binary);
    Book b;
    while (fin.read(reinterpret_cast<char*>(&b), sizeof(b)))
        if (b.bookId == id) { fin.close(); return true; }
    fin.close(); return false;
}

bool memberIdExists(int id) {
    ifstream fin(MEMBER_FILE, ios::binary);
    Member m;
    while (fin.read(reinterpret_cast<char*>(&m), sizeof(m)))
        if (m.memberId == id) { fin.close(); return true; }
    fin.close(); return false;
}

// ─────────────────────────────────────────────
//  BOOK FUNCTIONS
// ─────────────────────────────────────────────
void addBook() {
    clearScreen();
    printHeader("ADD NEW BOOK");

    Book b;
    cout << "  Enter Book ID      : "; cin >> b.bookId;
    if (bookIdExists(b.bookId)) {
        cout << "\n  ✖  Book ID already exists!\n"; pause(); return;
    }
    cin.ignore();
    cout << "  Enter Title        : "; cin.getline(b.title, 60);
    cout << "  Enter Author       : "; cin.getline(b.author, 40);
    cout << "  Enter Category     : "; cin.getline(b.category, 30);
    cout << "  Total Copies       : "; cin >> b.totalCopies;
    b.availableCopies = b.totalCopies;

    ofstream fout(BOOK_FILE, ios::binary | ios::app);
    fout.write(reinterpret_cast<char*>(&b), sizeof(b));
    fout.close();

    cout << "\n  ✔  Book added successfully!\n";
    pause();
}

void displayAllBooks() {
    clearScreen();
    printHeader("ALL BOOKS");

    ifstream fin(BOOK_FILE, ios::binary);
    if (!fin) { cout << "  No book records found.\n"; pause(); return; }

    Book b;
    int count = 0;
    cout << "  " << string(80, '-') << "\n";
    cout << "  " << left
         << setw(6)  << "ID"
         << setw(30) << "Title"
         << setw(20) << "Author"
         << setw(12) << "Category"
         << setw(6)  << "Total"
         << setw(6)  << "Avail"
         << "\n";
    cout << "  " << string(80, '-') << "\n";

    while (fin.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        cout << "  " << left
             << setw(6)  << b.bookId
             << setw(30) << b.title
             << setw(20) << b.author
             << setw(12) << b.category
             << setw(6)  << b.totalCopies
             << setw(6)  << b.availableCopies
             << "\n";
        count++;
    }
    fin.close();
    cout << "  " << string(80, '-') << "\n";
    if (count == 0) cout << "  No books found.\n";
    else            cout << "  Total Books: " << count << "\n";
    pause();
}

void searchBook() {
    clearScreen();
    printHeader("SEARCH BOOK");

    cout << "  Search by: 1. Title   2. Author\n  Choice: ";
    int ch; cin >> ch;
    cin.ignore();

    char keyword[60];
    if (ch == 1) { cout << "  Enter Title  : "; cin.getline(keyword, 60); }
    else         { cout << "  Enter Author : "; cin.getline(keyword, 60); }

    ifstream fin(BOOK_FILE, ios::binary);
    Book b;
    bool found = false;

    cout << "\n  ─── Search Results ──────────────────────────\n";
    while (fin.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        string field = (ch == 1) ? b.title : b.author;
        // case-insensitive partial match
        string f = field, k = keyword;
        for (auto& c : f) c = tolower(c);
        for (auto& c : k) c = tolower(c);
        if (f.find(k) != string::npos) {
            found = true;
            cout << "  ID: "    << b.bookId
                 << "  Title: " << b.title
                 << "  Author: "<< b.author
                 << "  Available: " << b.availableCopies << "/" << b.totalCopies << "\n";
        }
    }
    fin.close();
    if (!found) cout << "  No matching books found.\n";
    cout << "  ─────────────────────────────────────────────\n";
    pause();
}

void deleteBook() {
    clearScreen();
    printHeader("DELETE BOOK");

    int id;
    cout << "  Enter Book ID to delete : "; cin >> id;

    ifstream fin(BOOK_FILE, ios::binary);
    if (!fin) { cout << "\n  No records.\n"; pause(); return; }

    ofstream fout("tempB.dat", ios::binary);
    Book b; bool found = false;

    while (fin.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.bookId == id) { found = true; continue; }
        fout.write(reinterpret_cast<char*>(&b), sizeof(b));
    }
    fin.close(); fout.close();
    remove(BOOK_FILE.c_str());
    rename("tempB.dat", BOOK_FILE.c_str());

    if (found) cout << "\n  ✔  Book deleted.\n";
    else       cout << "\n  ✖  Book not found.\n";
    pause();
}

// ─────────────────────────────────────────────
//  MEMBER FUNCTIONS
// ─────────────────────────────────────────────
void addMember() {
    clearScreen();
    printHeader("ADD NEW MEMBER");

    Member m;
    cout << "  Enter Member ID : "; cin >> m.memberId;
    if (memberIdExists(m.memberId)) {
        cout << "\n  ✖  Member ID already exists!\n"; pause(); return;
    }
    cin.ignore();
    cout << "  Enter Name      : "; cin.getline(m.name, 50);
    cout << "  Enter Email     : "; cin.getline(m.email, 50);
    m.issuedBookId = 0;

    ofstream fout(MEMBER_FILE, ios::binary | ios::app);
    fout.write(reinterpret_cast<char*>(&m), sizeof(m));
    fout.close();

    cout << "\n  ✔  Member added successfully!\n";
    pause();
}

void displayAllMembers() {
    clearScreen();
    printHeader("ALL MEMBERS");

    ifstream fin(MEMBER_FILE, ios::binary);
    if (!fin) { cout << "  No member records found.\n"; pause(); return; }

    Member m;
    int count = 0;
    cout << "  " << string(70, '-') << "\n";
    cout << "  " << left
         << setw(8)  << "ID"
         << setw(22) << "Name"
         << setw(28) << "Email"
         << setw(12) << "Issued Book"
         << "\n";
    cout << "  " << string(70, '-') << "\n";

    while (fin.read(reinterpret_cast<char*>(&m), sizeof(m))) {
        cout << "  " << left
             << setw(8)  << m.memberId
             << setw(22) << m.name
             << setw(28) << m.email
             << setw(12) << (m.issuedBookId == 0 ? "None" : to_string(m.issuedBookId))
             << "\n";
        count++;
    }
    fin.close();
    cout << "  " << string(70, '-') << "\n";
    if (count == 0) cout << "  No members found.\n";
    else            cout << "  Total Members: " << count << "\n";
    pause();
}

// ─────────────────────────────────────────────
//  ISSUE & RETURN BOOK
// ─────────────────────────────────────────────
void issueBook() {
    clearScreen();
    printHeader("ISSUE BOOK");

    int memberId, bookId;
    cout << "  Enter Member ID : "; cin >> memberId;
    cout << "  Enter Book ID   : "; cin >> bookId;

    // Update member
    fstream mfile(MEMBER_FILE, ios::binary | ios::in | ios::out);
    Member m; bool mFound = false;
    while (mfile.read(reinterpret_cast<char*>(&m), sizeof(m))) {
        if (m.memberId == memberId) {
            mFound = true;
            if (m.issuedBookId != 0) {
                cout << "\n  ✖  Member already has Book ID " << m.issuedBookId << " issued!\n";
                mfile.close(); pause(); return;
            }
            m.issuedBookId = bookId;
            mfile.seekp(-static_cast<int>(sizeof(m)), ios::cur);
            mfile.write(reinterpret_cast<char*>(&m), sizeof(m));
            break;
        }
    }
    mfile.close();
    if (!mFound) { cout << "\n  ✖  Member not found.\n"; pause(); return; }

    // Update book
    fstream bfile(BOOK_FILE, ios::binary | ios::in | ios::out);
    Book b; bool bFound = false;
    while (bfile.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.bookId == bookId) {
            bFound = true;
            if (b.availableCopies <= 0) {
                cout << "\n  ✖  No copies available!\n";
                bfile.close(); pause(); return;
            }
            b.availableCopies--;
            bfile.seekp(-static_cast<int>(sizeof(b)), ios::cur);
            bfile.write(reinterpret_cast<char*>(&b), sizeof(b));
            break;
        }
    }
    bfile.close();
    if (!bFound) { cout << "\n  ✖  Book not found.\n"; pause(); return; }

    cout << "\n  ✔  Book ID " << bookId << " issued to Member ID " << memberId << " successfully!\n";
    pause();
}

void returnBook() {
    clearScreen();
    printHeader("RETURN BOOK");

    int memberId;
    cout << "  Enter Member ID : "; cin >> memberId;

    fstream mfile(MEMBER_FILE, ios::binary | ios::in | ios::out);
    Member m; bool mFound = false;
    int returnedBookId = 0;

    while (mfile.read(reinterpret_cast<char*>(&m), sizeof(m))) {
        if (m.memberId == memberId) {
            mFound = true;
            if (m.issuedBookId == 0) {
                cout << "\n  ✖  This member has no book issued.\n";
                mfile.close(); pause(); return;
            }
            returnedBookId = m.issuedBookId;
            m.issuedBookId = 0;
            mfile.seekp(-static_cast<int>(sizeof(m)), ios::cur);
            mfile.write(reinterpret_cast<char*>(&m), sizeof(m));
            break;
        }
    }
    mfile.close();
    if (!mFound) { cout << "\n  ✖  Member not found.\n"; pause(); return; }

    // Update book availability
    fstream bfile(BOOK_FILE, ios::binary | ios::in | ios::out);
    Book b;
    while (bfile.read(reinterpret_cast<char*>(&b), sizeof(b))) {
        if (b.bookId == returnedBookId) {
            b.availableCopies++;
            bfile.seekp(-static_cast<int>(sizeof(b)), ios::cur);
            bfile.write(reinterpret_cast<char*>(&b), sizeof(b));
            break;
        }
    }
    bfile.close();

    cout << "\n  ✔  Book ID " << returnedBookId << " returned successfully!\n";
    pause();
}

// ─────────────────────────────────────────────
//  MAIN MENU
// ─────────────────────────────────────────────
void showMenu() {
    clearScreen();
    cout << "\n  ╔══════════════════════════════════════════╗\n";
    cout << "  ║        LIBRARY MANAGEMENT SYSTEM         ║\n";
    cout << "  ╚══════════════════════════════════════════╝\n\n";
    cout << "    ── Books ──\n";
    cout << "       1.  Add Book\n";
    cout << "       2.  Display All Books\n";
    cout << "       3.  Search Book (Title / Author)\n";
    cout << "       4.  Delete Book\n";
    cout << "\n    ── Members ──\n";
    cout << "       5.  Add Member\n";
    cout << "       6.  Display All Members\n";
    cout << "\n    ── Transactions ──\n";
    cout << "       7.  Issue Book\n";
    cout << "       8.  Return Book\n";
    cout << "\n       9.  Exit\n\n";
    cout << "  Enter your choice: ";
}

int main() {
    int choice;
    do {
        showMenu();
        while (!(cin >> choice)) {
            cout << "  Invalid input. Enter a number: ";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        switch (choice) {
            case 1: addBook();          break;
            case 2: displayAllBooks();  break;
            case 3: searchBook();       break;
            case 4: deleteBook();       break;
            case 5: addMember();        break;
            case 6: displayAllMembers();break;
            case 7: issueBook();        break;
            case 8: returnBook();       break;
            case 9:
                clearScreen();
                cout << "\n  Thank you for using Library Management System!\n\n";
                break;
            default:
                cout << "\n  ✖  Invalid choice! Enter 1-9.\n";
                pause();
        }
    } while (choice != 9);
    return 0;
}
