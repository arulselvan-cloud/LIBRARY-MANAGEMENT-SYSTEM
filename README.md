# Library Management System 📚

A console-based Library Management System built in **C++** that efficiently manages books, members, and borrowing records using structured and object-oriented programming.

---

## 📌 Project Overview

This project is developed as part of the **Thiranex C++ Internship Program**. It simulates a real-world library system with full book and member management, including book issue and return functionality.

---

## ✨ Features

### 📖 Book Management
- ➕ **Add Book** — Add new books with ID, title, author, category, and copies
- 📋 **Display All Books** — View all books with availability status
- 🔍 **Search Book** — Search by title or author (partial match supported)
- 🗑️ **Delete Book** — Remove a book from the system

### 👤 Member Management
- ➕ **Add Member** — Register new library members
- 📋 **Display All Members** — View all members and their issued books

### 🔄 Transactions
- 📤 **Issue Book** — Issue a book to a member with availability check
- 📥 **Return Book** — Return a book and update availability automatically
- 💾 **Persistent Storage** — Data stored in binary files (`books.dat`, `members.dat`)

---

## 🛠️ Tech Stack

- **Language:** C++
- **Concepts Used:** Structs, Binary File I/O (`fstream`), OOP principles, String search, Menu-driven operations

---

## 🚀 How to Run

### Prerequisites
- A C++ compiler (g++ recommended)

### Compile & Run

```bash
# Compile
g++ main.cpp -o library_management

# Run on Linux/Mac
./library_management

# Run on Windows
library_management.exe
```

---

## 📂 File Structure

```
Library-management-system-cpp/
│
├── main.cpp          # Main source code
├── README.md         # Project documentation
└── .gitignore        # Ignores generated data files
```

---

## 📸 Menu Preview

```
  ╔══════════════════════════════════════════╗
  ║        LIBRARY MANAGEMENT SYSTEM        ║
  ╚══════════════════════════════════════════╝

    ── Books ──
       1.  Add Book
       2.  Display All Books
       3.  Search Book (Title / Author)
       4.  Delete Book

    ── Members ──
       5.  Add Member
       6.  Display All Members

    ── Transactions ──
       7.  Issue Book
       8.  Return Book

       9.  Exit
```

---

## 📝 Expected Outcome

A functional application that enables book addition, issue and return processing, and search by title or author for streamlined library operations.

---

## 👨‍💻 Author

Developed by **ARULSELVAN** as part of the Thiranex Internship Program.
