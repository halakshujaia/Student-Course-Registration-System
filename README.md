# Student Course Registration System

A complete **Student Course Registration System** implemented in **C** as part of the
**COMP2421 – Data Structures and Algorithms** course.

The project manages university students and their course registrations using
an **AVL Tree**, **Linked Lists**, and a **Hash Table with Open Addressing**.

---

## Project Overview

The system stores and manages university course registration records.

Each registration contains:

- Student Name
- Student ID
- Major
- Course Code
- Course Title
- Credit Hours
- Semester

A student may be registered in multiple courses, so each student record maintains
a linked list containing all of their registered courses.

The project is divided into two main parts:

1. **AVL Tree Management**
2. **Hash Table Management**

---

## Data Structures Used

### AVL Tree

The main student records are stored in an **AVL Tree**.

The **Student ID** is used as the key for organizing the tree.

Each AVL node contains:

- Student information
- Pointer to the student's course list
- Left child pointer
- Right child pointer
- Node height

The AVL Tree automatically remains balanced after insertions and deletions.

The implementation supports:

- Left Rotation
- Right Rotation
- Left-Right Rotation
- Right-Left Rotation

---

### Linked Lists

Each student contains a linked list of courses.

Each course node stores:

- Course Code
- Course Title
- Credit Hours
- Semester

This structure allows one student to have multiple course registrations.

Example:

```text
Student
   |
   +-- Course 1
   |
   +-- Course 2
   |
   +-- Course 3
```

---

### Hash Table

After processing the AVL Tree, the system can store the registration data in a
Hash Table.

The student's **name** is used as the hash key.

The implementation uses:

- Open Addressing
- Linear Probing
- EMPTY slots
- OCCUPIED slots
- DELETED slots

The table size used by the implementation is:

```text
101
```

---

## Hash Function

The project uses a string-based hash function for student names.

Conceptually, the hash calculation follows:

```text
h = (h * 32) + key[i]
```

The final table position is:

```text
h % TABLE_SIZE
```

where:

```text
TABLE_SIZE = 101
```

If a collision occurs, the program uses **Linear Probing** to search for another
available location.

---

## AVL Tree Operations

The AVL Tree section provides the following operations:

1. Load registration data from `reg.txt`
2. Insert a new registration
3. Find a student by name
4. Update student information
5. Add a course to an existing student
6. Delete a course
7. Update course information
8. List students registered in the same course
9. Delete a student registration
10. Save AVL Tree records to `students_hash.data`

---

## Student Updates

When a student is found, the program allows updating:

- Student Name
- Major
- Registered Courses

Course management includes:

- Adding a new course
- Deleting a course
- Updating a course

Course information that can be updated includes:

- Course Title
- Credit Hours
- Semester

---

## Hash Table Operations

The Hash Table section supports:

1. Print the entire Hash Table
2. Print the Hash Table size
3. Print the hash function
4. Insert a new registration record
5. Search for a student
6. Display student information
7. Delete a record
8. Save the Hash Table back to `reg.txt`

The table displays all slot states, including:

```text
EMPTY
OCCUPIED
DELETED
```

---

## Input File

The program reads registration information from:

```text
reg.txt
```

The required record format is:

```text
Student Name#Student ID#Major#Course Code#Course Title#Credit Hours#Semester
```

Example:

```text
Sara Mahmoud#1223121#Computer Science#COMP242#Operating Systems#3#Fall 2024
```

---

## Intermediate Data File

AVL Tree records can be saved to:

```text
students_hash.data
```

This file is then used to construct the Hash Table.

---

## Output

After modifying the Hash Table, the registration information can be saved back to:

```text
reg.txt
```

---

## Input Validation

The program performs validation for several types of input, including:

- Student IDs
- Student names
- Majors
- Course IDs
- Credit hours
- Course titles
- Semesters

It also prevents duplicate registration of the same course for the same student
in the same semester.

---

## Main Menu

The program starts with the following main menu:

```text
Student Course Registration System

1. AVL Tree Part
2. Hash Table Part
3. Exit
```

---

## AVL Tree Menu

```text
--- AVL Menu ---

1. Load data from reg.txt
2. Insert new registration
3. Find student by name and update
4. List students in same course
5. Delete student registration
6. Save data to students_hash.data
7. Back
```

---

## Hash Table Menu

```text
--- Hash Table Menu ---

1. Print hash table
2. Print table size
3. Print hash function
4. Insert new record
5. Search for student
6. Delete record
7. Save hash table to reg.txt
8. Back
```

---

## Technologies and Concepts

The project demonstrates the use of:

- C Programming Language
- AVL Trees
- Binary Search Trees
- Tree Rotations
- Linked Lists
- Hash Tables
- Hash Functions
- Open Addressing
- Linear Probing
- Dynamic Memory Allocation
- File Handling
- Structures
- Pointers
- Searching
- Insertion
- Deletion
- Input Validation

---

## Project Files

Main implementation:

```text
main.c
```

Runtime data files:

```text
reg.txt
students_hash.data
```

`students_hash.data` is generated by the application when AVL Tree data is saved.

---

## Author

**Hala Khalil**  
Student ID: **1231019**  
Section: **1**

---

## Course Information

**Course:** COMP2421 – Data Structures and Algorithms  
**Project:** Project No. 2  
**Semester:** Fall 2025/2026  
**Department:** Computer Science  
**University:** Birzeit University

---

## Project Purpose

The purpose of this project is to apply fundamental data structure concepts to a
realistic university course registration system.

It demonstrates how different data structures can be combined within one
application:

- **AVL Trees** provide balanced organization of students by ID.
- **Linked Lists** allow each student to maintain multiple courses.
- **Hash Tables** provide name-based access to registration records.
- **File Handling** provides persistent storage of registration information.
