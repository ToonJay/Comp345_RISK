# COMP345_RISK – Risk-like Game Implementation

This project is based on **COMP 345** (Advanced C++), a course from Concordia University. It’s a solo implementation of a team-based semester project that was originally completed in a past year. 
I am **not currently enrolled in the course**, and this assignment comes from a different semester than the one during which I took the class — meaning I had **never done this version before**.

---

## 📌 Overview

The project simulates the mechanics of **Warzone**, a Risk-like strategy board game published by Hasbro. Although the original assignment spans **three phases**,
this implementation includes only **Assignments 1 and 2**, which already encompass the **complete core game mechanics**.

**Assignment 3** introduces additional AI player behaviors and tournament features. However, the instructions were vague, open-ended, and difficult to interpret without access to a course instructor.
Since the design pattern involved — the **Strategy** pattern — is a simple and easy-to-understand concept, I saw no added value in implementing Assignment 3 under unclear requirements. 
Therefore, I opted to consider the project complete after A2, as it already provided a complete and functioning game logic system.

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

I made the following **intentional deviations** from the constraints to preserve sound design principles:

### 🔹 LoggingObserver Classes (`LoggingObserver.h/.cpp`)

- Rules **1** and **4** were **not** followed.
- Reason: To maintain the integrity of the **Observer Design Pattern**.
- Rationale:
  - Deep copying or assigning `Subject` / `Observer` objects could **break observer relationships** (e.g., duplicate or dangling observers).
  - As such, **copy and assignment operations are explicitly disallowed**.

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
- Since each subclass customizes its effect string internally, additional stream output logic was unnecessary at the subclass level.

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
