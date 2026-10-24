# 📊 Grade Tracker (C)

> 🎓 Project for **CS 265 (Advanced Programming Tools and Techniques)** at **Drexel University**

A command-line gradebook written in plain C. It loads grades from a text file into a linked list, lets you print, add, remove, and get stats on them, then saves everything back when you're done.

I didn't use `<string.h>`. The string helpers (`my_strlen`, `my_strcpy`, `my_strcmp`) are written by hand with pointer arithmetic, and all memory for the linked list is managed manually with `malloc` and `free`.

## Features

- **print**: show every entry in a clean table
- **stats `<assignment>`**: min, max, and mean for one assignment
- **add `<id>:<assignment>:<grade>`**: add a grade (duplicates are rejected)
- **remove `<id>:<assignment>`**: delete an entry
- Changes are saved back to the database file on exit

## Build & run

```bash
make
./grades sample.db
```

Then type commands, one per line. Press `Ctrl+D` to save and quit.

## Example

```
$ ./grades sample.db
print
Student ID | Assignment Name      | Grade
-----------------------------------------
1000000001 | HW 1                 | 92
1000000002 | HW 1                 | 78
1000000001 | Lab 1                | 88
1000000003 | HW 1                 | 85
stats HW 1
Grade statistics for HW 1
Min:  78
Max:  92
Mean: 85.00
```

## Database format

One entry per line, `student_id:assignment_name:grade`:

```
1000000001:HW 1:92
1000000002:HW 1:78
```

## Built with

C · structs · singly linked list · manual memory management · Make. Written for CS 265 (Advanced Programming Tools and Techniques) at Drexel University.
