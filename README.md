# Academic Code Portfolio

This repository is a collection of engineering projects and laboratory exercises completed during my studies at the **Lodz University of Technology**, Faculty of Electrical, Electronic, Computer and Control Engineering. The code reflects my learning progress in various programming paradigms and hardware description languages.

## Project Overviews

### 1. Simple Game (C)
This project is a 2D arcade game developed in pure C using a basic graphical library. It was designed to practice procedural programming and real-time game logic.
* **Key Features:** Implementation of a main game loop, real-time user input handling, and basic physics for collision detection.
* **Logic:** Use of structures to manage the state of multiple game objects like the player's ship, enemies, and projectiles.

### 2. Sensor Management System (C++)
An object-oriented system designed to simulate a base station managing various types of environmental sensors. This project focused on building a scalable architecture for hardware simulation.
* **OOP Principles:** Extensive use of **Polymorphism** and **Inheritance** through an abstract base class `CSensor`.
* **Error Handling:** Implementation of a custom `CException` class to manage runtime errors, such as exceeding the sensor array size.
* **Technical Detail:** Use of operator overloading (e.g., `+=`, `-=`) for intuitive management of sensor objects within the base station.

### 3. FIFO Buffer Module (VHDL)
A digital logic project involving the design and verification of a First-In-First-Out hardware buffer. The main goal was to understand signal synchronization and data flow in digital systems.
* **Design:** Implementation of a behavioral model with write/read pointers and internal state monitoring.
* **Synchronization:** Handling of synchronous reset and clock-edge triggered data transitions.
* **Verification:** Development of a comprehensive **Testbench** to validate the buffer's behavior under various conditions, including overflow and empty states.
