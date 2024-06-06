# E-commerce System

This C++ code implements an e-commerce framework with administrator and user roles. The program starts creating the necessary information if there are no administrators, users, orders and products. Provides interfaces for user registration, login, products, cart management and ordering. Users can sign up with a username, password and email to ensure the password is strong and the email is valid. Administrators can log in, view site balances, withdraw funds, change admin passwords, view all orders, mark orders as shipped, and add new products to inventory. The program includes functions to manage loading and saving, checking the availability of files, checking email format and making sure passwords contain letters and numbers. It also has a simple user interface to view items, add and remove items to the shopping cart, view the shopping cart, make purchases, and view purchase history. The main function controls the flow of the program, displays the menu and calls the necessary functions according to the user's preference. Data I/O policies ensure data consistency across segments by storing user data, product data, and order data. The system uses simple console-based navigation and output and uses commands to provide some visual feedback, such as changing console colors and simulating loading animations.

## Features

### User Functions
- **Registration:** Users can register with a unique username, password, and email, ensuring password complexity and email validity.
- **Login:** Secure user authentication.
- **Product Display:** View available products.
- **Cart Management:** Add, remove items, and view cart.
- **Order Processing:** Make purchases and view purchase history.

### Admin Functions
- **Login:** Secure admin authentication.
- **Site Balance Management:** View and withdraw site funds.
- **Password Management:** Change admin password.
- **Order Management:** View all orders and mark them as delivered.
- **Product Management:** Add new products to the inventory.

## Key Functionalities

- **File Initialization:** Initializes necessary files for admin, users, orders, and products if they do not exist.
- **Product Handling:** Functions to load and save products.
- **Validation:** Ensures email format validity and password complexity (contains both alphabets and digits).
- **User Interface:** Console-based navigation and outputs, including visual feedback using system commands like changing console color and simulating loading animations.

## Data Persistence

The system uses file I/O for storing user data, product information, and order records, ensuring data persistence across sessions.

## Program Flow

The main function controls the flow of the program, presenting menus and invoking appropriate functions based on user choices.

## Getting Started

1. **Compile the code:** Use a C++ compiler to compile the code.
2. **Run the program:** Execute the compiled program.
3. **Follow the prompts:** Navigate through the console-based interface to use the system.

## System Requirements

- C++ compiler
- Console environment

- ## Menu 1:

<img src="https://github.com/AtharIbrahim/ECommerce-Cpp/blob/main/Screenshot/Menu%201.png" alt="CryptoMatrix Logo" style="max-width: 100%; height: auto; margin-bottom: 20px;">

- ## Admin Menu:

<img src="https://github.com/AtharIbrahim/ECommerce-Cpp/blob/main/Screenshot/Admin%20Menu.png" alt="CryptoMatrix Logo" style="max-width: 100%; height: auto; margin-bottom: 20px;">

- ## User Menu:

<img src="https://github.com/AtharIbrahim/ECommerce-Cpp/blob/main/Screenshot/User%20Menu.png" alt="CryptoMatrix Logo" style="max-width: 100%; height: auto; margin-bottom: 20px;">

- ## Loading System:

<img src="https://github.com/AtharIbrahim/ECommerce-Cpp/blob/main/Screenshot/Loading%20System.png" alt="CryptoMatrix Logo" style="max-width: 100%; height: auto; margin-bottom: 20px;">
