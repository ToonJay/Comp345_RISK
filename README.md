# COMP345_RISK – Risk-like Game Implementation

This project is based on **COMP 345** (Advanced C++), a course from Concordia University. It’s a solo implementation of a semester project from a past year that's usually done in teams of 5.
I am **not currently enrolled in the course**, and this assignment comes from a different semester than the one during which I took the class — meaning I had **never done this version before**.

---

## 📌 Overview

The project simulates the mechanics of **Warzone**, a Risk-like strategy board game published by Hasbro. Although the original project is done through **3 assignments**,
this implementation includes only **Assignments 1 and 2**, which already encompass the **complete core game mechanics**.

**Assignment 3** introduces AI player behaviors and a tournament feature. However, the instructions were vague, open-ended, and difficult to interpret without being able to talk to a teacher/instructor.
Additionally, the assignment is mainly about the **Strategy Design Pattern**, which is simple and easy to understand. As such, I decided not to implement Assignment 3 and to instead consider the project complete after Assignment 2,
as it already provides a complete implementation of the game mechanics.

---

## 📐 Project Constraints

The following rules were enforced to align with the COMP 345 assignment guidelines:

1. All data members of user-defined class type must be of **pointer type**.
2. All file names and file content must follow the **specifications provided**.
3. All modules must be split into **separate `.h` / `.cpp` files** (no inline functions).
4. All classes must implement a:
   - **Copy constructor**
   - **Assignment operator**
   - **Stream insertion operator**
5. **Memory leaks** must be avoided.
6. Code must be **fully commented** (classes, methods, operators, etc.).

---

## ⚠️ Rule Exceptions & Design Notes

I made the following **exceptions** from the constraints in order to integrate better design principles:

### 🔹 LoggingObserver Classes (`LoggingObserver.h/.cpp`)

- Rules **1** and **4** were **not** followed.
- Reason: To maintain the integrity of the **Observer Design Pattern**.
- Typically, this design pattern **doesn't include any of what rules 1 and 4 are asking.**

### 🔹 Order Subclasses

- Do **not** implement:
  - Assignment operators
  - Stream insertion operators
- Instead:
  - Each defines a **copy constructor**
  - Each implements a **`clone()` method** for polymorphic behavior
- Display logic is handled by the **base `Order` class**, which logs:
  - `OrderDescription`
  - `OrderEffect`
- Since each subclass **sets its effect string internally**, additional stream output logic was unnecessary for them.

---

## 🧠 Final Thoughts

This project helped reinforce important C++ concepts like:
- Design patterns (Observer, Strategy, etc.)
- Memory management and ownership
- Deep vs. shallow copying
- Separation of concerns and modular architecture
- Abstract base classes and polymorphism
- Smart pointers vs. raw pointers (and safe manual memory management)
- Custom copy constructors, assignment operators, and destructor (Rule of Three)
- Operator overloading (e.g., stream insertion)
- File I/O and parsing (e.g., reading maps or commands)
- State machines and transitions
- Encapsulation and access control
- Working with the STL (e.g., `vector`, `map`, `find_if`, etc.)
- Defensive programming and validation
- Logging and debugging practices

Although Assignment 3 remains untouched, Assignments 1 and 2 form a **complete and playable game logic system**, meeting all the mechanical requirements of the original Warzone game.
