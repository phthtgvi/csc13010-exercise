# Refactoring guidelines: Adhering to SRP and DRY principles

## 1. Introduction

This document provides an overview of two key software design principles—**Single Responsibility Principle (SRP)** and **Don't Repeat Yourself (DRY)**—and reviews our project to ensure these principles are adhered to. It also describes significant refactoring changes made to address any violations.

## 2. Research overview

### Single Responsibility Principle (SRP)

- **Definition:**  
  SRP states that a class or module should have one, and only one, reason to change. Each class should only have a single responsibility or functionality.
- **Benefits:**
  - Improves code readability and maintainability.
  - Makes the system easier to understand and modify.
  - Reduces the impact of changes, as modifications to one responsibility do not affect others.

### Don't Repeat Yourself (DRY)

- **Definition:**  
  The DRY principle emphasizes that information or logic should be defined in a single place. Repeating code or business logic across multiple parts of a system should be avoided.
- **Benefits:**
  - Reduces code duplication.
  - Enhances maintainability since changes need to be made in only one location.
  - Minimizes the risk of inconsistencies and errors.

## 3. Review of the project for SRP and DRY

### A. Separation of concerns and SRP violations

- **ConfigManager:**
  - **Original issue:** Configuration loading and saving logic were previously mixed with other responsibilities.
  - **Change:** The `ConfigManager` class now solely handles loading and saving configuration data (such as email suffix, phone regex, delete time limit, and enforcement flags). This isolates configuration management and adheres to SRP.
- **StudentRepository:**

  - **Original issue:** Operations like data import/export, student management, and faculty/status/program management were combined in a single class.
  - **Change:** The class is now organized into distinct sections and functions for each responsibility. Although the repository still handles multiple related tasks, efforts have been made to isolate these operations. Future refactoring could further separate these concerns.

- **CertificateGenerator:**
  - **Original issue:** Certificate generation logic was previously interwoven with general file I/O operations.
  - **Change:** The certificate generation code has been isolated into its own module (`CertificateGenerator.hpp`/`.cpp`), ensuring that document creation is managed independently.

### B. Eliminating duplicate code (DRY Violations)

- **RecordIO:**

  - **Original Issue:** File I/O code for CSV/JSON operations was repeated in multiple places.
  - **Change:** The `RecordIO` class consolidates CSV and JSON file operations into a single module, reducing duplication and centralizing the file I/O logic.

- **Common helper functions:**

  - Functions such as `getSafeInput` are now defined once in the repository and reused across various parts of the system, ensuring consistency and reducing redundancy.

- **Centralized configuration management:**
  - Configuration data is loaded once via `ConfigManager::getInstance().loadConfig()` and then reused, avoiding multiple redundant loads and updates.

## 4. Significant changes documentation

- **Splitting classes into separate files:**  
  Many classes (e.g., `RecordIO`, `StatusRulesManager`) have been split into header (`.hpp`) and implementation (`.cpp`) files. This not only improves modularity but also adheres to SRP by clearly separating interfaces from implementations.

- **Validator integration:**  
  The `ConcreteStudentValidator` now uses enforcement flags from `ConfigManager` to determine whether to apply email/phone validations. This centralizes validation logic and eliminates duplicate checks.

- **Refactoring repository functions:**  
  Functions such as `addStudent`, `removeStudent`, `deleteFaculty`, `deleteStatus`, and `deleteProgram` have been refactored to include necessary data integrity checks. Centralizing these checks avoids code repetition and ensures consistency.

- **Centralized configuration management:**  
  The `ConfigManager` class now handles all configuration-related tasks, including enforcement flags for validations and deletion time limits. Changes to configuration behavior are now made in one place only, satisfying the DRY principle.

## 5. Conclusion

By applying the SRP and DRY principles:

- **SRP** ensures that each module has a clear, focused responsibility, making the system easier to understand and maintain.
- **DRY** reduces code duplication by centralizing common functionalities, making future modifications simpler and less error-prone.

These changes have improved the maintainability, readability, and robustness of our project. Future refactoring efforts may further enhance the separation of concerns as the system evolves.
