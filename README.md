# Student Management System

This is a console-based Student Management System written in C++ that allows you to manage a list of students with functionalities to add, remove, update, and search student information. Student data is persisted in a JSON file.

## Table of Contents

1.  [Description](#description)
2.  [Source Code Structure](#source-code-structure)
3.  [Design Patterns](#design-patterns)
4.  [Dependencies](#dependencies)
5.  [Installation](#installation)
6.  [Compilation (Windows)](#compilation-windows)
7.  [Usage](#usage)
8.  [Screenshots](#screenshots)
9.  [Contributing](#contributing)
10. [License](#license)

## Description

The Student Management System provides the following functionalities:

### v1.0

- **Add New Student:** Allows adding a new student to the system with details such as ID, name, date of birth, gender, faculty, course, program, address, email, phone number, and status.
- **Delete Student:** Removes a student from the system based on their Student ID.
- **Update Student Information:** Updates the information of an existing student based on their Student ID.
- **Search Student:** Searches for students by name or Student ID.

### v2.0

- **Data storage:** Using JSON for storing directly to local files.
- **Advanced Search:** Allow students searching by Faculty filter.
- **Import/export records:** Allow user to import records as CSV/JSON and vice versa.
- **Logging mechanism:** System automatically logs any meaningful operations or errors for troubleshooting issues.
- **Development build information:** Display build version and build date at the top of the console application, all information is stored in `version_info.json` file.
- **Dynamic attributes:** Such attributes like Faculty, Program, Status is allowed to be added or audited by administrator. The system will provide 5 Faculty entities, 4 Status entities and 3 Program entities by default.

All student data is stored in a JSON file (`students.json`) for persistence. The system performs data validation on email, phone number, faculty, and student status.

### v3.0

- **Unique Student ID**: Ensures that each Student ID (MSSV) is unique. When adding or updating student records, duplicate Student IDs are not allowed.

- **Configurable Email Domain**: The system enforces email validation based on a specific domain, which can be configured dynamically.
  Example: Only accepts emails ending with @student.university.edu.vn.
- **Phone Number Validation by Country**: Validates phone numbers based on country-specific formats, which are configurable. Example: Vietnam phone numbers must start with +84 or 0[3|5|7|8|9]xxxxxxxx.
- **Restricted Student Status Transitions**: Student status can only be changed following predefined rules, which are configurable. Example: "Đang học" → "Bảo lưu", "Tốt nghiệp", "Đình chỉ" (valid transitions). "Đã tốt nghiệp" cannot be reverted to "Đang học".

All these enhancements ensure data integrity and improve system configurability while maintaining the JSON-based storage approach.

### v4.0

- **University branding:** Display university headlines at the top of CLI.
- **Configurable time policy for deleting after account creation:** Add a new option in menu options to specify the time policy for deleting after account creation.

- **Enable/Disable university settings:** Add an option to specify whether the university settings are enabled or disabled.

- **Allow to delete attributes (Faculty, Program, Status) in case of no accounts are associated with that attribute:** Allow to delete attributes in case of no accounts are associated with that attribute.

- **Student certificate generation:** Add an option to generate a certificate for the student for the purpose of job, postgraduate with full information of the student and the university in Markdown/DOCX.

- **Unit testing:** Refer to **Unit testing** and [Testing documentation](./docs/testing/).

- **Refactor code:** Refer to **Refactoring guidelines** and [Refactoring documentation](./docs/refactoring/).

## Source Code Structure

The source code is structured as follows:

- `main.cpp`: Contains the main function and the program's user interface.
- `Student` class: Represents a student with attributes like ID, name, date of birth, etc., and methods for displaying information and serializing/deserializing to/from JSON.
- `StudentValidator` class: An interface for validating student data.
- `ConcreteStudentValidator` class: Implements the `StudentValidator` interface and provides concrete validation rules for email, phone number, faculty, and status.
- `StudentRepository` class: A Singleton class responsible for managing the list of students, including adding, removing, searching, and updating student information. It also handles loading and saving data to the `students.json` file.
- `nlohmann/json.hpp`: A header-only library for JSON manipulation, located in the `nlohmann` folder.
- `Logger.hpp`: Provides a Logger class following the Singleton pattern to log system events into the `student_management.log` file.
- `ConfigManager.hpp`: Manages system configuration, including valid email suffixes and phone number regex patterns. The configuration is stored and loaded from the `config.json` file.
- `RecordIO.hpp`: Provides functions for exporting and importing data in CSV and JSON formats, enabling easy storage and retrieval of student information from files.
- `StatusRulesManager.hpp`: Manages student status transition rules, such as from "Active" to "Graduated." These rules are stored and loaded from the `status_rules.json` file.
- `CertificateGenerator.hpp/CertificateGenerator.cpp`: provide the core functionality for generating certificate documents for students. These files define a set of functions that take a structured data object (typically a CertificateData structure containing information such as student details, university details, and certificate-specific fields) and produce a formatted certificate output in Markdown or Docx.

The application enforces the following validation rules:

- **Email:** Checks for a valid email format (e.g., using a regular expression).
- **Phone Number:** Validates that the phone number matches a specific format (e.g., using a regular expression or length check).
- **Faculty:** Ensures that the entered faculty matches one of the allowed values:
  - Faculty of Law (FL)
  - Faculty of Business English (FBE)
  - Faculty of Japanese (FJPN)
  - Faculty of French (FFR)
- **Student Status:** Validates that the entered status matches one of the allowed values:
  - Active
  - Graduated
  - Leave
  - Absent

## Design Patterns

This project utilizes the following design patterns to improve its structure, maintainability, and extensibility:

- **Singleton:** The `StudentRepository` class is implemented as a Singleton. This ensures that only one instance of the repository exists throughout the application's lifetime. This is beneficial for managing the student data in a centralized and controlled manner, preventing inconsistencies that might arise from multiple instances modifying the data.

  - **Purpose:** Ensures a class has only one instance and provides a global point of access to it.
  - **Implementation:** A private constructor prevents direct instantiation. A static method (`getInstance()`) provides access to the single instance, creating it if it doesn't already exist.

- **Strategy:** The `StudentValidator` interface and its concrete implementation (`ConcreteStudentValidator`) demonstrate the Strategy pattern. This pattern allows you to define a family of algorithms (in this case, validation rules) and make them interchangeable at runtime.

  - **Purpose:** Defines a family of algorithms, encapsulates each one, and makes them interchangeable. Strategy lets the algorithm vary independently from clients that use it.
  - **Implementation:** The `StudentValidator` interface defines the `isValid()` method. `ConcreteStudentValidator` implements this method with specific validation logic. The `StudentRepository` uses the `StudentValidator` interface, allowing it to easily switch between different validation strategies (e.g., a more strict validator, a validator that checks for data consistency with external systems, etc.) without modifying the `StudentRepository` class itself. This makes the validation process more flexible and extensible.

## Dependencies

- **C++ Compiler:** A C++11 compatible compiler (e.g., MinGW with g++, Microsoft Visual C++).
- **nlohmann/json library:** A header-only JSON library for C++, located in the `nlohmann` folder within the project directory. The `json.hpp` file should be present in the `nlohmann` folder.

## Installation

No explicit installation is required. Simply clone or download the source code and ensure you have the necessary dependencies. Specifically, make sure the `nlohmann` folder containing `json.hpp` is in the same directory as `main.cpp`.

## Compilation (Windows)

Here's how to compile the program using either CodeBlocks or Visual Studio on Windows:

### CodeBlocks

1.  **Open CodeBlocks.**
2.  **Create a New Project:**
    - Go to `File -> New -> Project...`
    - Select `Console application` and click `Go`.
    - Click `Next`.
    - Select `C++` as the language.
    - Give your project a name (e.g., `StudentManager`) and choose a location. Click `Next`.
    - Ensure that the compiler is set to `GNU GCC Compiler`. Click `Finish`.
3.  **Add Source File:**
    - In the `Project` pane (usually on the left), right-click on `Sources` and select `Add files...`
    - Navigate to the location of your `main.cpp` file and select it. Click `Open`.
4.  **Include Directory:**
    - Right-click on your project's name in the `Projects` pane and select `Build options...`
    - Select the compiler (e.g., `GNU GCC Compiler`).
    - Go to the `Search directories` tab.
    - Under the `Compiler` tab, click `Add`.
    - Browse to the `nlohmann` folder (the one containing `json.hpp`) and select it. Click `OK`.
    - Click `OK` to close the `Project build options` window.
5.  **Enable C++11:**
    - Right-click on your project's name in the `Projects` pane and select `Build options...`
    - Select the compiler (e.g., `GNU GCC Compiler`).
    - Go to the "Compiler settings" tab
    - Check the box "Have g++ follow the C++11 ISO standard"
    - Click OK
6.  **Build the Project:**
    - Go to `Build -> Build` (or press `Ctrl+F9`).
7.  **Run the Program:**
    - Go to `Build -> Run` (or press `Ctrl+F10`).

### Visual Studio

1.  **Open Visual Studio.**
2.  **Create a New Project:**
    - Go to `File -> New -> Project...`
    - Select `Console App` (or `Console Application`).
    - Give your project a name (e.g., `StudentManager`) and choose a location. Click `Create`.
3.  **Add Source File:**
    - In the `Solution Explorer` pane (usually on the right), right-click on `Source Files` and select `Add -> Existing Item...`
    - Navigate to the location of your `main.cpp` file and select it. Click `Add`.
4.  **Include Directory:**
    - In the `Solution Explorer` pane, right-click on your project's name and select `Properties`.
    - In the `Property Pages` window, select `C/C++ -> General`.
    - In the `Additional Include Directories` field, add the path to the `nlohmann` folder (the one containing `json.hpp`). For example: `$(SolutionDir)nlohmann`.
    - Click `Apply` and then `OK`.
5.  **Enable C++11 (or later):**
    - In the `Property Pages` window, select `C/C++ -> Language`.
    - Set `C++ Language Standard` to `ISO C++11 Standard` (or a later standard like C++14 or C++17).
    - Click `Apply` and then `OK`.
6.  **Build the Solution:**
    - Go to `Build -> Build Solution` (or press `Ctrl+Shift+B`).
7.  **Run the Program:**
    - Go to `Debug -> Start Without Debugging` (or press `Ctrl+F5`).

## Usage

To run the compiled program:

1.  **Navigate to the directory containing the executable (if not running from the IDE):** The executable will typically be located in a `Debug` or `Release` folder within your project directory.

2.  **Execute the program:**

    - From the command prompt or file explorer, run the executable file (e.g., `StudentManager.exe`).
    - If running from the IDE, the program will usually start automatically after building.

3.  **Follow the on-screen menu to interact with the program:**

    The program will present a menu with options to add, delete, update, and search students. Student data will be saved in the `students.json` file in the same directory as the executable.

## Screenshots

### v1.0

#### Menu

![ex-screenshot](screenshots/screen1.png)

#### Add a record

![ex-screenshot](screenshots/screen2.png)

#### Delete a record

![ex-screenshot](screenshots/screen4.png)
![ex-screenshot](screenshots/screen5.png)

#### Edit a record

![ex-screenshot](screenshots/screen6.png)
![ex-screenshot](screenshots/screen7.png)

#### Search a record

![ex-screenshot](screenshots/screen8.png)

### v2.0

#### Build information

![ex-screenshot](screenshots/screen9.png)

#### Logging mechanism

![ex-screenshot](screenshots/screen10.png)

#### Advanced search

![ex-screenshot](screenshots/screen11.png)

![ex-screenshot](screenshots/screen12.png)

#### Import/export records

![ex-screenshot](screenshots/screen13.png)

![ex-screenshot](screenshots/screen14.png)

![ex-screenshot](screenshots/screen15.png)

#### Dynamic attributes

![ex-screenshot](screenshots/screen16.png)

![ex-screenshot](screenshots/screen17.png)

### v3.0

#### Unique student ID

![ex-screenshot](screenshots/screen18.png)

#### Configurable phone number prefix

![ex-screenshot](screenshots/screen20.png)

#### Configurable email suffix

![ex-screenshot](screenshots/screen19.png)

#### Configurable status rules

![ex-screenshot](screenshots/screen21.png)

#### Unit testing

![ex-screenshot](screenshots/screen22.png)

### v4.0

#### University branding

![ex-screenshot](screenshots/screen23.png)

#### Configurable time policy for deleting after account creation

![ex-screenshot](screenshots/screen24.png)

![ex-screenshot](screenshots/screen25.png)

#### Enable/Disable university settings

![ex-screenshot](screenshots/screen26.png)

#### Allow to delete attributes (Faculty, Program, Status) in case of no accounts are associated with that attribute

![ex-screenshot](screenshots/screen27.png)

![ex-screenshot](screenshots/screen28.png)

#### Student certificate generation

![ex-screenshot](screenshots/screen29.png)

![ex-screenshot](screenshots/screen30.png)

## Convention

The coding style is followed to the C++ coding standards. Use meaningful variable names, provide comments where necessary, and ensure the code is well-formatted.

## License

This project is licensed under the [MIT License](LICENSE).
