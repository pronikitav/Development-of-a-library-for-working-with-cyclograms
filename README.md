# Development of a Library for Working with Cyclograms

##The structure of the cyclograms library file:
- Source files
   - Cyclogram.cpp
   - Cyclogram.h
   - main.cpp
- Project 3.dev
  
## Introduction

Industrial robotics plays a key role among modern manufacturing technologies. Automation of operations using controlled robots makes it possible to increase labor productivity and ensure repeatability of movements. One of the basic concepts in robot control is a cyclogram — a description of its trajectory in the form of a sequence of reference points with time intervals and spatial coordinates.

In positional control mode, the robot sequentially executes movement commands specified by positions. The combination of such positions forms a movement cyclogram. Storage, loading, and editing of cyclograms are important functions of control system software.

The purpose of this course project is to develop a C++ library for working with cyclograms that implements a complete set of operations for managing reference points of an industrial robot trajectory.

To achieve this goal, the following tasks must be completed:

* develop a data structure for representing a cyclogram reference point;
* design the `Cyclogram` class that encapsulates a set of reference points;
* implement an algorithm for adding a point with automatic resolution of overlapping intervals;
* develop functions for saving and loading a cyclogram from a text file;
* create a command-line user interface;
* test the developed software.

The program works as follows. Initially, it reads data from a file that can either be empty or contain five values on each line. The first two values, separated by a hyphen, represent the time interval during which the robot remains at a specific coordinate, while the remaining three values, separated by spaces, represent the coordinates along the X, Y, and Z axes. If the data is valid, it is added to the cyclogram list, where the coordinates are ordered according to their time intervals.

The user can then add, delete, and edit reference points through the console, as well as save the changes to a file.

The object of the study is software for a robot control system. The subject of the study is methods and algorithms for storing and processing movement cyclograms.

The development tools used are the C++ programming language, the C++ standard library, the `std::vector` container, and input/output streams.

## 1. Analysis of the Problem

### 1.1 Program Functioning Logic

The program works as follows. When launched, it reads data from a file that can either be empty or contain five values on each line. The first two values, separated by a hyphen, represent the time interval during which the robot remains at a specific coordinate. The remaining three values, separated by spaces, represent the robot coordinates along the X, Y, and Z axes.

The program opens the file and, if the data is valid, adds it to the cyclogram list, where the coordinates are ordered according to the time intervals of each position.

When a new point is added, conflicts are resolved automatically. If the specified interval is already occupied by an existing point, it is assigned to the new point, while the corresponding interval of the old point is removed by shortening or completely deleting the old point.

The library provides a programming interface for modules of a robot control system, including trajectory planning, movement program execution, and diagnostics.

### 1.2 Technical Requirements for the Library

Based on the analysis of the task, the following functional requirements were formulated. The library must provide:

1. Storage of a time-ordered set of reference points in memory.
2. Addition of a point with automatic resolution of time interval conflicts.
3. Removal of a reference point by its index.
4. Editing of parameters of an existing point.
5. Display of the current cyclogram contents.
6. Saving a cyclogram to a text file and loading it with validation.

Non-functional requirements include reliable handling of invalid input, use of standard C++ facilities only, and separation of the code into a header file and an implementation file.

The following constraints are applied:

* time values: `[0, 10 000 000]`;
* coordinates: `[-100 000, 100 000]`.

### 1.3 Development Tools Selection

The project is developed in C++ in accordance with the requirements of the task. This choice is justified by the high performance of C++, support for the object-oriented programming paradigm, and widespread use of C++ in industrial robotics.

The standard library provides ready-to-use containers such as `std::vector` and algorithms such as `std::sort`, which simplify development.

The project is divided into three files:

```text
Cyclogram.h
Cyclogram.cpp
main.cpp
```

`Cyclogram.h` contains the interface declaration, `Cyclogram.cpp` contains the implementation of the methods, and `main.cpp` contains the user interface.

The data file is stored in text format for convenient manual editing and debugging.

## 2. Algorithmic Support

### 2.1 Program Structure Diagram

The program consists of three main modules. The `Cyclogram.h` module is the header file that declares the `Point` structure, the `Cyclogram` class, and prototypes of helper functions.

The `Cyclogram.cpp` module contains the implementation of all class methods and helper functions:

* `safeStringToDouble`;
* `parseInterval`;
* `isValidCoordinate`.

The `main.cpp` module implements the interactive command-line interface.

The modules interact as follows: `main.cpp` creates an object of the `Cyclogram` class and calls its methods. When the program starts, an attempt to load data from `data.txt` is automatically performed. When the program finishes, the data is saved to the same file.

All access to cyclogram data is performed exclusively through the public methods of the `Cyclogram` class. This prevents direct incorrect modification of the `points` vector. This approach follows the principle of separating the interface from the implementation and makes the library suitable for integration into larger control systems.

### 2.2 Reference Point Addition Algorithm

The purpose of the `addPoint` method is to insert a new point with a specified interval so that the intervals do not overlap.

The algorithm works as follows: a temporary list called `newPoints` is created. Then, for each existing point `p`, the relationship between its interval and the interval of the new point is analyzed according to five possible cases:

1. No overlap — point `p` is preserved without changes.
2. Point `p` is completely covered by the new point — point `p` is removed.
3. Point `p` completely covers the new point — point `p` is split into two fragments.
4. Partial overlap on the left — the left fragment is preserved.
5. Partial overlap on the right — the right fragment is preserved.

After all points have been processed, the new point is added to the list, which is then sorted by `t_start` using `std::sort`.

### 2.3 File Data Loading Algorithm

The `loadFromFile` method implements loading with complete validation.

The algorithm performs the following steps:

1. Open the file specified by `filename` for reading. If the file is not found, display an error message and terminate the method.
2. Check whether the file is empty. If it is empty, display a message.
3. Clear the current list of points using `points.clear()`.
4. Read the file line by line. Empty lines are skipped.
5. For each non-empty line, extract the time interval and three numbers representing X, Y, and Z.
6. Check the validity of the interval using `parseInterval` and the validity of the coordinates using `isValidCoordinate`.
7. If validation is successful, add the point to the list.
8. After all lines have been read, sort the list by `t_start`.

The `parseInterval` function processes a string in the form `t1-t2`, finding the hyphen as the separator and passing the substrings to `safeStringToDouble` for safe conversion with validation of the remaining characters.

It is additionally checked that `t_start` is not equal to `t_end` and that both values are non-negative. If `t_start > t_end`, the values are automatically swapped using `std::swap`.

This approach makes the program resistant to corrupted files. Even if some lines contain errors, valid data is successfully loaded, while the numbers of problematic lines are displayed to the user for diagnostics.

### 2.4 Variable Descriptions

| Variable    | Description                        |
| ----------- | ---------------------------------- |
| `t_start`   | Start of the time interval         |
| `t_end`     | End of the time interval           |
| `x`         | X coordinate                       |
| `y`         | Y coordinate                       |
| `z`         | Z coordinate                       |
| `points`    | Array of reference points          |
| `newPoints` | Temporary array used in `addPoint` |
| `choice`    | Selected menu option               |
| `idx`       | Point index                        |
| `lineNum`   | File line counter                  |

## 3. Software Implementation

### 3.1 Project Structure and Header File

The project consists of three files: `Cyclogram.h` is the header file containing the definition of the `Point` structure and declaration of the `Cyclogram` class; `Cyclogram.cpp` contains the library implementation; and `main.cpp` contains the user interface.

The `Point` structure contains five `double` fields: `t_start`, `t_end` representing the time interval, and `x`, `y`, `z` representing the coordinates.

The `Cyclogram` class stores the `points` vector in the `private` section. The following methods are declared in the `public` section:

```text
addPoint
removePoint
editPoint
saveToFile
loadFromFile
print
getCount
```

The helper functions `safeStringToDouble`, `parseInterval`, and `isValidCoordinate` are declared in the header file for use from `main.cpp`.

### 3.2 Implementation of the `Cyclogram` Class

The `addPoint` method implements the algorithm for adding a point with conflict resolution. The method accepts a constant reference, which prevents unnecessary copying of the object. A range-based loop is used to iterate through the vector.

After processing, the vector is sorted using `std::sort` with a lambda expression that compares the `t_start` values of two points. The method displays the message `"Готово"` after successful addition.

The `removePoint` method removes a point by its index. The validity of the index is checked using the condition `index >= 0 && index < points.size()`. Removal is performed using the vector's `erase` iterator.

The `editPoint` method implements editing using a combination of `erase` and `addPoint`. This provides automatic conflict resolution without duplicating the existing logic.

The `saveToFile` method opens a file for writing using `std::ofstream`. Each point is written in the format:

```text
t_start-t_end x y z
```

After closing the file, the `fail()` flag is checked to detect writing errors.

The `loadFromFile` method processes lines using `stringstream`. If an incorrect line is encountered, loading of the remaining lines continues, which increases the fault tolerance of the program.

The `print` method displays the index, time interval, and coordinates of each point. If the list is empty, the message `"Пусто"` is displayed.

### 3.3 User Interface and File Format

The program initially reads data from the `data.txt` file, which can either be empty or contain five values on each line. The first two values, separated by a hyphen, represent the time interval during which the robot remains at a specific coordinate. The remaining three values, separated by spaces, represent the robot coordinates along the X, Y, and Z axes.

If the data is valid, it is added to the cyclogram list, where the coordinates are ordered according to the time intervals of each position.

The user can then perform the following actions through the console:

* add a coordinate point for a specified time interval;
* delete a coordinate point by its index;
* edit a specific point by changing its time parameters or coordinates;
* save all changes to the file and terminate the program.

The user interface is implemented as a console menu with six options:

```text
1 — List
2 — Add
3 — Delete
4 — Edit
5 — Save
6 — Exit
```

The main `while(true)` loop processes commands until option `6` is selected. In case of an input error, `clearInput()` resets the `cin` flags and clears the input buffer.

The data file uses the following format:

```text
t_start-t_end X Y Z
```

The lines are ordered by `t_start`.

Example:

```text
0-2 10.5 20.0 5.0
2-5 15.0 25.5 5.0
5-8 10.5 20.0 8.0
```

Empty lines in the file are ignored. When the program starts, `loadFromFile("data.txt")` is automatically called. If the file does not exist, the program continues working with an empty cyclogram.

## 4. Testing and Debugging

### 4.1 Test Scenarios and Results

To verify the correctness of the program, a set of test scenarios covering the main functions and boundary cases was developed. The test scenarios are presented in the table below.

| No. | Test Scenario                        | Input Data                       | Expected / Actual Result                              |
| --: | ------------------------------------ | -------------------------------- | ----------------------------------------------------- |
|   1 | Adding a point to an empty cyclogram | Interval: `1-5`, X=10, Y=20, Z=5 | List contains 1 point `[1-5, (10,20,5)]`. Passed.     |
|   2 | Adding without overlap               | Existing: `1-5`; New: `6-10`     | 2 points in the correct order. Passed.                |
|   3 | Complete coverage                    | Existing: `2-8`; New: `1-10`     | Existing point removed, 1 point `[1-10]`. Passed.     |
|   4 | Splitting into two fragments         | Existing: `0-10`; New: `3-7`     | 3 points: `[0-3]`, `[3-7]`, `[7-10]`. Passed.         |
|   5 | Invalid time interval                | `abc`, `5-5`, `-3-2`             | `"Time error"` displayed, list unchanged. Passed.     |
|   6 | Coordinates outside the range        | X=200000, Y=0, Z=0               | `"Out of limits"` displayed, point not added. Passed. |
|   7 | Saving and loading a file            | 3 points → save → load           | Contents match. Passed.                               |
|   8 | Invalid lines in a file              | 2 valid lines + 1 invalid line   | 2 points loaded, invalid line skipped. Passed.        |

Testing was performed by sequentially entering commands in the interactive program menu and checking whether the output corresponded to the expected results.

Test 1 confirmed that a point can be correctly added to an empty cyclogram.

Test 2 verified adding without overlap: a list of two points was displayed in the correct order according to the increasing `t_start`.

Test 3 confirmed complete coverage: when `[1-10]` was added, the existing `[2-8]` point was removed.

Test 4 verified splitting: adding `[3-7]` to an existing `[0-10]` interval resulted in three records: `[0-3]`, `[3-7]`, and `[7-10]`.

Tests 5 and 6 confirmed reliable handling of invalid input.

Tests 7 and 8 confirmed correct file saving and loading, including handling of files containing invalid lines.

All eight test scenarios were completed successfully. The program correctly handles both normal situations and unusual cases, as well as invalid input, which demonstrates the reliability of the developed software.

## Conclusion

During the course project, a software library for working with cyclograms was developed in C++. The library implements a complete set of operations for managing reference points of an industrial robot trajectory: adding, deleting, editing, saving to a file, and loading from a file.

All assigned tasks were completed:

* the `Point` data structure with a time interval and X, Y, Z coordinates was developed;
* the `Cyclogram` class based on `std::vector` was designed with data encapsulation;
* an algorithm for adding a point with automatic resolution of five types of time interval overlap was implemented;
* functions for saving and loading a cyclogram with complete input validation and protection against invalid values were developed;
* an interactive command-line user interface with input error handling was created;
* testing was performed using eight scenarios, and all tests were successfully passed.

The developed library has the following qualities:

* **Modularity** — separation into a header file and an implementation file.
* **Reliability** — handling of all boundary cases without abnormal termination.
* **Portability** — use of standard C++ facilities only.

During the project, the following concepts were studied and applied in practice: object-oriented programming in C++, working with the `std::vector` container and the `std::sort` algorithm, file input/output using `std::fstream`, and safe input processing using `std::stringstream`.

The acquired skills can be applied to the development of software for robot control systems and other automated systems.

A possible direction for further development of the library is support for a binary file format to speed up reading and writing of large cyclograms.

## References

1. Stroustrup, B. *Programming: Principles and Practice Using C++*. 3rd ed. Moscow: Williams, 2023. 1328 p.
2. Schildt, H. *C++: The Complete Reference*. Moscow: Williams, 2023. 1040 p.
3. Alekseev, N. S., Sorokin, D. E. *Object-Oriented Programming in C++*. Moscow: Bauman Moscow State Technical University, 2024. 180 p.
4. Voronin, A. V., Petrov, S. I. "Programming Industrial Robot Trajectories Using C++." *Robotics and Technical Automation Equipment*, 2023, No. 3, pp. 45–52.

