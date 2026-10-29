# Vehicle Parking Management System (C++)

A console-based C++ application for managing vehicle parking slots. It supports vehicle entry and exit, automatic slot assignment, parking-fee calculation, live slot status, and saving records to a local file.

## Features

- Register a vehicle using its registration number and vehicle type.
- Automatically assign the first available parking slot.
- Prevent duplicate active parking entries for the same registration number.
- Record vehicle entry and exit times.
- Generate a parking receipt with an hourly fee calculation.
- View slot availability and currently parked vehicles.
- Save records in `parking_records.txt` so active parking information remains available after restarting the program.
- Validate menu input and handle malformed saved records safely.

## Requirements

- A C++ compiler supporting C++17 (GCC, Clang, or a recent MSVC).
- A terminal or IDE such as Visual Studio Code.

## How to compile and run

### Windows (MinGW / GCC)

Open a terminal in the project folder and run:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o parking
.\parking.exe
```

### Linux / macOS

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o parking
./parking
```

In VS Code, open the project folder, open `main.cpp`, then use the integrated terminal to run the appropriate commands above.

## How to use

1. Choose **1** to register a vehicle entry.
2. Enter the registration number and vehicle type.
3. The system assigns a slot and records the entry time.
4. Choose **3** to view total, occupied, and available slots.
5. Choose **4** to view all currently parked vehicles.
6. Choose **2** and enter the registration number to record exit and generate a receipt.
7. Choose **5** to exit.

## Parking rules used in this demo

- Total capacity: 10 slots.
- Parking rate: ₹20 per started hour.
- Minimum billable duration: 1 hour.
- The fee is calculated using the program's system clock.
- Data is stored locally in `parking_records.txt` in the working directory.

These values are constants near the top of `main.cpp` and can be changed for a different demonstration.

## Project structure

```text
Vehicle-Parking-Management-System/
├── main.cpp
├── README.md
└── .gitignore
```

`parking_records.txt` is created automatically when the application saves data; it is intentionally not included in the repository.

## Concepts demonstrated

- Classes and objects
- Structures and vectors
- Encapsulation and member functions
- Loops and conditional statements
- File handling (`ifstream` / `ofstream`)
- Input validation and exception handling
- Time handling and fee calculation
- Basic separation of program logic into functions

## Limitations and possible improvements

This is an educational, single-user console application. It is not a production parking platform: it does not provide a graphical interface, database, authentication, payment gateway, or concurrent access support. Future improvements could include different rates by vehicle type, a database, admin login, daily revenue reports, and a GUI.

## Author

Add your name, course, section, and university details here before submission.
