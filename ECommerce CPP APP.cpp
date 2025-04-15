#include <iostream>
#include <fstream>
#include <cstring>
#include <queue>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

using namespace std;

// UI Framework - Cross-platform compatible

namespace UI {
    // Formatting
    const string RESET = "\033[0m";
    const string BOLD = "\033[1m";
    const string DIM = "\033[2m";
    
    // Colors
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string YELLOW = "\033[33m";
    const string BLUE = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN = "\033[36m";
    
    void printSuccess(const string& message) {
        cout << GREEN << BOLD << "[+] " << message << RESET << endl;
    }
    
    void printError(const string& message) {
        cout << RED << BOLD << "[-] " << message << RESET << endl;
    }
    
    void printWarning(const string& message) {
        cout << YELLOW << BOLD << "[!] " << message << RESET << endl;
    }
    
    void printInfo(const string& message) {
        cout << CYAN << BOLD << "[i] " << message << RESET << endl;
    }
    
    void drawHorizontalLine(int width = 50) {
        cout << "+" << string(width, '-') << "+" << endl;
    }
    
    void clearScreen() {
        cout << "\033[2J\033[1;1H";  // ANSI escape codes for clear screen
    }
    
    void sleepMilliseconds(int ms) {
        #ifdef _WIN32
            Sleep(ms);
        #else
            usleep(ms * 1000);
        #endif
    }
    
    void showLoadingAnimation(int seconds = 2) {
        cout << BLUE << BOLD << "Loading ";
        for (int i = 0; i < seconds * 2; i++) {
            cout << ">";
            cout.flush();
            sleepMilliseconds(500);
        }
        cout << RESET << endl;
    }
}


// Data Structures

const char adminFile[] = "data/admin.txt";
const char userFile[] = "data/users.txt";
const char ordersFile[] = "data/orders.txt";
const char productsFile[] = "data/products.txt";

struct Product {
    int id;
    char name[50];
    float price;
    int quantity;
    Product* next;
};

struct User {
    char username[50];
    char password[50];
    char email[50];
};

struct Order {
    char username[50];
    char productName[50];
    int quantity;
    float totalAmount;
    char status[20];
    int priority;
    
    bool operator<(const Order& other) const {
        return priority < other.priority;
    }
};

struct CartItem {
    char productName[50];
    float price;
    int quantity;
};


// Global Variables

priority_queue<Order> orderQueue;
Product* productHead = NULL;
map<string, User> userMap;
float siteBalance = 0.0f;
vector<CartItem> currentCart;
int nextProductId = 1;


// Utility Functions

bool fileExists(const char* fileName) {
    ifstream file(fileName);
    return file.good();
}

bool containsAlphabet(const string& str) {
    for (char c : str) {
        if (isalpha(c)) return true;
    }
    return false;
}

bool containsDigits(const string& str) {
    for (char c : str) {
        if (isdigit(c)) return true;
    }
    return false;
}

bool emailValid(const string& email) {
    return email.find('@') != string::npos && 
           email.find('.') != string::npos;
}

string getInput(const string& prompt, bool (*validator)(const string&) = nullptr, 
               const string& errorMsg = "Invalid input!") {
    string input;
    while (true) {
        cout << UI::BOLD << prompt << UI::RESET;
        getline(cin, input);
        
        if (validator == nullptr || validator(input)) {
            return input;
        }
        UI::printError(errorMsg);
    }
}

void ensureDataDirectoryExists() {
    #ifdef _WIN32
    _mkdir("data");
    #else
    mkdir("data", 0777);
    #endif
}


// Data Management

void loadUsers() {
    ifstream in(userFile);
    if (!in) return;
    
    User user;
    while (in >> user.username >> user.password >> user.email) {
        userMap[user.username] = user;
    }
}

void saveUsers() {
    ofstream out(userFile);
    if (!out) {
        UI::printError("Error saving users!");
        return;
    }
    
    for (const auto& pair : userMap) {
        out << pair.second.username << '\t' 
            << pair.second.password << '\t' 
            << pair.second.email << '\n';
    }
}

void addProductToLinkedList(const Product& product) {
    Product* newProduct = new Product;
    newProduct->id = product.id;
    strcpy(newProduct->name, product.name);
    newProduct->price = product.price;
    newProduct->quantity = product.quantity;
    newProduct->next = NULL;

    if (productHead == NULL) {
        productHead = newProduct;
    } else {
        Product* current = productHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newProduct;
    }
    
    if (product.id >= nextProductId) {
        nextProductId = product.id + 1;
    }
}

void loadProducts() {
    ifstream in(productsFile);
    if (!in) return;
    
    // Clear existing list
    Product* current = productHead;
    while (current != NULL) {
        Product* temp = current;
        current = current->next;
        delete temp;
    }
    productHead = NULL;

    Product temp;
    while (in >> temp.id >> temp.name >> temp.price >> temp.quantity) {
        addProductToLinkedList(temp);
    }
}

void saveProducts() {
    ofstream out(productsFile);
    if (!out) {
        UI::printError("Error saving products!");
        return;
    }
    
    Product* current = productHead;
    while (current != NULL) {
        out << current->id << '\t' 
            << current->name << '\t' 
            << current->price << '\t' 
            << current->quantity << '\n';
        current = current->next;
    }
}

void loadOrders() {
    ifstream in(ordersFile);
    if (!in) return;
    
    Order order;
    while (in >> order.username >> order.productName 
           >> order.quantity >> order.totalAmount
           >> order.status >> order.priority) {
        orderQueue.push(order);
        if (strcmp(order.status, "Delivered") == 0) {
            siteBalance += order.totalAmount;
        }
    }
}

void saveOrders() {
    ofstream out(ordersFile);
    if (!out) {
        UI::printError("Error saving orders!");
        return;
    }
    
    // Temporary vector to hold orders
    vector<Order> orders;
    while (!orderQueue.empty()) {
        orders.push_back(orderQueue.top());
        orderQueue.pop();
    }
    
    // Write to file
    for (const Order& order : orders) {
        out << order.username << '\t' << order.productName << '\t'
           << order.quantity << '\t' << order.totalAmount << '\t'
           << order.status << '\t' << order.priority << '\n';
        orderQueue.push(order); // Restore queue
    }
}


// UI Components

void displayMenu(const vector<string>& options, const string& title = "MENU") {
    int width = 40;
    cout << "+" << string(width, '-') << "+" << endl;
    cout << "|" << UI::BOLD << setw(width) << left << ("  " + title) << UI::RESET << "|" << endl;
    cout << "+" << string(width, '-') << "+" << endl;
    
    for (size_t i = 0; i < options.size(); i++) {
        cout << "| " << UI::BOLD << setw(2) << left << (to_string(i+1) + ".") 
             << UI::RESET << setw(width-3) << options[i] << "|" << endl;
    }
    
    cout << "+" << string(width, '-') << "+" << endl;
    cout << UI::BOLD << "Enter your choice: " << UI::RESET;
}

void displayProductTable() {
    if (productHead == NULL) {
        UI::printWarning("No products available!");
        return;
    }

    // Header
    cout << "+------+----------------------+-----------+-----------+" << endl;
    cout << "| " << left << setw(4) << "ID" << " | " 
         << setw(20) << "Name" << " | " 
         << setw(9) << "Price" << " | " 
         << setw(9) << "Quantity" << " |" << endl;
    cout << "+------+----------------------+-----------+-----------+" << endl;
    
    // Products
    Product* current = productHead;
    while (current != NULL) {
        cout << "| " << UI::BOLD << setw(4) << current->id << UI::RESET << " | " 
             << setw(20) << current->name << " | " 
             << setw(9) << "$" + to_string(current->price).substr(0, 5) << " | " 
             << setw(9) << current->quantity << " |" << endl;
        current = current->next;
    }
    
    // Footer
    cout << "+------+----------------------+-----------+-----------+" << endl;
}

void displayCart() {
    if (currentCart.empty()) {
        UI::printWarning("Your cart is empty!");
        return;
    }

    float total = 0.0f;
    
    cout << UI::YELLOW << UI::BOLD << "Your Shopping Cart" << UI::RESET << endl;
    cout << "+------------------------+-------+-----------+" << endl;
    cout << "| " << left << setw(24) << "Product" 
         << "| " << setw(5) << "Qty" 
         << "| " << setw(9) << "Subtotal" << "|" << endl;
    cout << "+------------------------+-------+-----------+" << endl;
    
    for (const auto& item : currentCart) {
        float subtotal = item.price * item.quantity;
        cout << "| " << left << setw(24) << item.productName 
             << "| " << setw(5) << item.quantity 
             << "| " << setw(9) << "$" + to_string(subtotal).substr(0, 6) 
             << "|" << endl;
        total += subtotal;
    }
    
    cout << "+------------------------+-------+-----------+" << endl;
    cout << "| " << left << setw(31) << "Total:" 
         << " $" << setw(6) << total << "|" << endl;
    cout << "+------------------------+-------+-----------+" << endl;
}


// Core Functions

void adminLogin();
void userLogin();
void userRegistration();
void adminMenu();
void userMenu(const string& username);
void addProduct();
void processOrder(const string& username);


// Main Application

int main() {
    ensureDataDirectoryExists();
    
    // Initialize required files
    const char* files[] = {adminFile, userFile, ordersFile, productsFile};
    for (const char* file : files) {
        if (!fileExists(file)) {
            ofstream out(file);
            if (strcmp(file, adminFile) == 0) {
                out << "admin123\n"; // Default admin password
            }
            out.close();
        }
    }

    // Load data
    loadUsers();
    loadProducts();
    loadOrders();

    UI::clearScreen();
    cout << UI::MAGENTA << UI::BOLD << "=== E-Commerce System ===" << UI::RESET << endl << endl;

    int choice;
    do {
        vector<string> mainMenu = {
            "Admin Login",
            "User Login",
            "User Registration",
            "Exit"
        };
        
        displayMenu(mainMenu, "MAIN MENU");
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: adminLogin(); break;
            case 2: userLogin(); break;
            case 3: userRegistration(); break;
            case 4: 
                cout << "\nExiting...\n";
                break;
            default:
                UI::printError("Invalid choice!");
                UI::sleepMilliseconds(1000);
        }
    } while (choice != 4);

    // Cleanup
    Product* current = productHead;
    while (current != NULL) {
        Product* temp = current;
        current = current->next;
        delete temp;
    }

    return 0;
}

void adminLogin() {
    UI::clearScreen();
    ifstream in(adminFile);
    if (!in) {
        UI::printError("Error opening admin file!");
        return;
    }

    string storedPassword;
    in >> storedPassword;
    in.close();

    string enteredPassword = getInput("Enter admin password: ");

    if (enteredPassword == storedPassword) {
        UI::showLoadingAnimation(2);
        UI::printSuccess("Login successful!");
        UI::sleepMilliseconds(1000);
        adminMenu();
    } else {
        UI::showLoadingAnimation(2);
        UI::printError("Incorrect password!");
        UI::sleepMilliseconds(1500);
    }
}

void userRegistration() {
    UI::clearScreen();
    cout << UI::BOLD << "USER REGISTRATION\n" << UI::RESET;
    UI::drawHorizontalLine(30);
    
    User newUser;
    
    string username = getInput("Enter username: ", [](const string& s) {
        return !s.empty() && userMap.find(s) == userMap.end();
    }, "Username already exists or is invalid!");
    strcpy(newUser.username, username.c_str());
    
    string password = getInput("Enter password: ", [](const string& s) {
        return s.length() >= 6 && containsAlphabet(s) && containsDigits(s);
    }, "Password must be at least 6 characters with both letters and numbers!");
    strcpy(newUser.password, password.c_str());
    
    string email = getInput("Enter email: ", emailValid, "Invalid email format!");
    strcpy(newUser.email, email.c_str());

    userMap[newUser.username] = newUser;
    saveUsers();

    UI::showLoadingAnimation(2);
    UI::printSuccess("Registration successful!");
    UI::sleepMilliseconds(1500);
}

void userLogin() {
    UI::clearScreen();
    cout << UI::BOLD << "USER LOGIN\n" << UI::RESET;
    UI::drawHorizontalLine(30);
    
    string username = getInput("Enter username: ");
    string password = getInput("Enter password: ");

    auto it = userMap.find(username);
    if (it != userMap.end() && strcmp(it->second.password, password.c_str()) == 0) {
        UI::showLoadingAnimation(2);
        UI::printSuccess("Login successful!");
        UI::sleepMilliseconds(1000);
        userMenu(username);
    } else {
        UI::showLoadingAnimation(2);
        UI::printError("Invalid username or password!");
        UI::sleepMilliseconds(1500);
    }
}

void addProduct() {
    UI::clearScreen();
    cout << UI::BOLD << "ADD NEW PRODUCT\n" << UI::RESET;
    UI::drawHorizontalLine(30);
    
    Product newProduct;
    newProduct.id = nextProductId++;
    
    string name = getInput("Enter product name: ");
    strcpy(newProduct.name, name.c_str());
    
    string priceStr = getInput("Enter product price: ", [](const string& s) {
        try {
            stof(s);
            return true;
        } catch (...) {
            return false;
        }
    }, "Invalid price! Enter a number.");
    newProduct.price = stof(priceStr);
    
    string quantityStr = getInput("Enter product quantity: ", [](const string& s) {
        try {
            stoi(s);
            return true;
        } catch (...) {
            return false;
        }
    }, "Invalid quantity! Enter a whole number.");
    newProduct.quantity = stoi(quantityStr);
    
    addProductToLinkedList(newProduct);
    saveProducts();
    
    UI::showLoadingAnimation(2);
    UI::printSuccess("Product added successfully!");
    UI::sleepMilliseconds(1500);
}

void addToCart(const string& username) {
    displayProductTable();
    if (productHead == NULL) {
        UI::sleepMilliseconds(1500);
        return;
    }

    string productIdStr = getInput("Enter product ID to add to cart (0 to cancel): ", [](const string& s) {
        try {
            stoi(s);
            return true;
        } catch (...) {
            return false;
        }
    }, "Invalid ID! Enter a number.");
    
    int productId = stoi(productIdStr);
    if (productId == 0) return;

    // Find product
    Product* current = productHead;
    while (current != NULL && current->id != productId) {
        current = current->next;
    }

    if (current == NULL) {
        UI::printError("Product not found!");
        UI::sleepMilliseconds(1500);
        return;
    }

    string quantityStr = getInput("Enter quantity: ", [](const string& s) {
        try {
            int q = stoi(s);
            return q > 0;
        } catch (...) {
            return false;
        }
    }, "Invalid quantity! Enter a positive number.");
    
    int quantity = stoi(quantityStr);
    
    if (quantity > current->quantity) {
        UI::printError("Not enough stock available!");
        UI::sleepMilliseconds(1500);
        return;
    }

    // Add to cart
    CartItem item;
    strcpy(item.productName, current->name);
    item.price = current->price;
    item.quantity = quantity;
    currentCart.push_back(item);

    // Update product quantity
    current->quantity -= quantity;
    saveProducts();

    UI::printSuccess("Product added to cart!");
    UI::sleepMilliseconds(1500);
}

void checkout(const string& username) {
    if (currentCart.empty()) {
        UI::printWarning("Your cart is empty!");
        UI::sleepMilliseconds(1500);
        return;
    }

    displayCart();
    cout << "\nConfirm checkout? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore();
    
    if (tolower(confirm) != 'y') {
        UI::printInfo("Checkout cancelled.");
        UI::sleepMilliseconds(1500);
        return;
    }

    // Create orders
    float totalAmount = 0.0f;
    for (const auto& item : currentCart) {
        Order order;
        strcpy(order.username, username.c_str());
        strcpy(order.productName, item.productName);
        order.quantity = item.quantity;
        order.totalAmount = item.price * item.quantity;
        strcpy(order.status, "Pending");
        order.priority = (username.find("premium") != string::npos) ? 2 : 1;
        
        orderQueue.push(order);
        totalAmount += order.totalAmount;
    }

    saveOrders();
    currentCart.clear();

    UI::showLoadingAnimation(3);
    UI::printSuccess("Checkout successful! Total: $" + to_string(totalAmount).substr(0, 6));
    UI::sleepMilliseconds(2000);
}

void viewOrderHistory(const string& username) {
    UI::clearScreen();
    cout << UI::BOLD << "ORDER HISTORY FOR " << username << "\n" << UI::RESET;
    UI::drawHorizontalLine(50);
    
    bool found = false;
    vector<Order> orders;
    
    while (!orderQueue.empty()) {
        orders.push_back(orderQueue.top());
        orderQueue.pop();
    }
    
    for (const Order& order : orders) {
        if (string(order.username) == username && string(order.status) == "Delivered") {
            found = true;
            cout << "Product: " << order.productName << endl;
            cout << "Quantity: " << order.quantity << endl;
            cout << "Amount: $" << order.totalAmount << endl;
            cout << "Status: " << order.status << endl;
            UI::drawHorizontalLine(50);
        }
        orderQueue.push(order);
    }
    
    if (!found) {
        UI::printWarning("No order history found!");
    }
    
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}

void adminMenu() {
    int choice;
    do {
        UI::clearScreen();
        vector<string> options = {
            "View Site Balance",
            "Withdraw Funds",
            "Add Funds",
            "Change Password",
            "View All Orders",
            "Mark Order as Delivered",
            "Add Product",
            "Logout"
        };
        displayMenu(options, "ADMIN DASHBOARD");
        
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: {
                UI::clearScreen();
                cout << UI::BOLD << "SITE BALANCE\n" << UI::RESET;
                UI::drawHorizontalLine(20);
                cout << "Current balance: $" << siteBalance << "\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                break;
            }
            case 2: {
                UI::clearScreen();
                cout << UI::BOLD << "WITHDRAW FUNDS\n" << UI::RESET;
                UI::drawHorizontalLine(20);
                cout << "Current balance: $" << siteBalance << "\n";
                
                string amountStr = getInput("Enter amount to withdraw: ", [](const string& s) {
                    try {
                        float amt = stof(s);
                        return amt > 0;
                    } catch (...) {
                        return false;
                    }
                }, "Invalid amount! Enter a positive number.");
                
                float amount = stof(amountStr);
                
                if (amount > siteBalance) {
                    UI::printError("Insufficient funds!");
                    UI::sleepMilliseconds(1500);
                    break;
                }
                
                siteBalance -= amount;
                UI::printSuccess("Withdrawal successful! New balance: $" + to_string(siteBalance).substr(0, 6));
                UI::sleepMilliseconds(2000);
                break;
            }
            case 3: {
                UI::clearScreen();
                cout << UI::BOLD << "ADD FUNDS\n" << UI::RESET;
                UI::drawHorizontalLine(20);
                
                string amountStr = getInput("Enter amount to deposit: ", [](const string& s) {
                    try {
                        float amt = stof(s);
                        return amt > 0;
                    } catch (...) {
                        return false;
                    }
                }, "Invalid amount! Enter a positive number.");
                
                float amount = stof(amountStr);
                siteBalance += amount;
                
                UI::printSuccess("Deposit successful! New balance: $" + to_string(siteBalance).substr(0, 6));
                UI::sleepMilliseconds(2000);
                break;
            }
            case 4: {
                UI::clearScreen();
                cout << UI::BOLD << "CHANGE ADMIN PASSWORD\n" << UI::RESET;
                UI::drawHorizontalLine(30);
                
                ifstream in(adminFile);
                string currentPassword;
                in >> currentPassword;
                in.close();
                
                string entered = getInput("Enter current password: ");
                if (entered != currentPassword) {
                    UI::printError("Incorrect password!");
                    UI::sleepMilliseconds(1500);
                    break;
                }
                
                string newPass = getInput("Enter new password: ", [](const string& s) {
                    return s.length() >= 6 && containsAlphabet(s) && containsDigits(s);
                }, "Password must be at least 6 characters with both letters and numbers!");
                
                ofstream out(adminFile);
                out << newPass;
                out.close();
                
                UI::printSuccess("Password changed successfully!");
                UI::sleepMilliseconds(1500);
                break;
            }
            case 5: {
                UI::clearScreen();
                cout << UI::BOLD << "ALL ORDERS\n" << UI::RESET;
                
                vector<Order> orders;
                while (!orderQueue.empty()) {
                    orders.push_back(orderQueue.top());
                    orderQueue.pop();
                }
                
                cout << "+----------------------+---------+---------+--------------+" << endl;
                cout << "| " << left << setw(20) << "Customer" << "| " 
                     << setw(7) << "Product" << "| " 
                     << setw(7) << "Quantity" << "| " 
                     << setw(12) << "Status" << "|" << endl;
                
                for (const Order& order : orders) {
                    cout << "+----------------------+---------+---------+--------------+" << endl;
                    cout << "| " << setw(20) << order.username << "| " 
                         << setw(7) << order.productName << "| " 
                         << setw(7) << order.quantity << "| " 
                         << setw(12) << order.status << "|" << endl;
                    orderQueue.push(order);
                }
                
                cout << "+----------------------+---------+---------+--------------+" << endl;
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 6: {
                UI::clearScreen();
                cout << UI::BOLD << "MARK ORDER AS DELIVERED\n" << UI::RESET;
                
                vector<Order> pendingOrders;
                vector<Order> allOrders;
                
                while (!orderQueue.empty()) {
                    Order order = orderQueue.top();
                    if (string(order.status) == "Pending") {
                        pendingOrders.push_back(order);
                    }
                    allOrders.push_back(order);
                    orderQueue.pop();
                }
                
                if (pendingOrders.empty()) {
                    UI::printWarning("No pending orders!");
                    UI::sleepMilliseconds(1500);
                    // Restore queue
                    for (const Order& order : allOrders) {
                        orderQueue.push(order);
                    }
                    break;
                }
                
                cout << "+----------------------+---------+---------+--------------+" << endl;
                cout << "| " << left << setw(20) << "Customer" << "| " 
                     << setw(7) << "Product" << "| " 
                     << setw(7) << "Quantity" << "| " 
                     << setw(12) << "Amount" << "|" << endl;
                
                for (const Order& order : pendingOrders) {
                    cout << "+----------------------+---------+---------+--------------+" << endl;
                    cout << "| " << setw(20) << order.username << "| " 
                         << setw(7) << order.productName << "| " 
                         << setw(7) << order.quantity << "| " 
                         << setw(12) << "$" + to_string(order.totalAmount).substr(0, 6) << "|" << endl;
                }
                
                cout << "+----------------------+---------+---------+--------------+" << endl;
                cout << endl;
                
                string username = getInput("Enter customer username to mark as delivered: ");
                
                bool found = false;
                for (Order& order : allOrders) {
                    if (string(order.username) == username && string(order.status) == "Pending") {
                        strcpy(order.status, "Delivered");
                        siteBalance += order.totalAmount;
                        found = true;
                    }
                    orderQueue.push(order);
                }
                
                if (found) {
                    saveOrders();
                    UI::printSuccess("Order marked as delivered!");
                } else {
                    UI::printError("No pending orders found for that username!");
                }
                UI::sleepMilliseconds(1500);
                break;
            }
            case 7: {
                addProduct();
                break;
            }
            case 8: {
                UI::printInfo("Logging out...");
                UI::sleepMilliseconds(1000);
                break;
            }
            default: {
                UI::printError("Invalid choice!");
                UI::sleepMilliseconds(1000);
            }
        }
    } while (choice != 8);
}

void userMenu(const string& username) {
    int choice;
    do {
        UI::clearScreen();
        vector<string> options = {
            "View Products",
            "Add to Cart",
            "View Cart",
            "Checkout",
            "View Order History",
            "Logout"
        };
        displayMenu(options, "USER MENU - " + username);
        
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: {
                displayProductTable();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 2: {
                addToCart(username);
                break;
            }
            case 3: {
                displayCart();
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 4: {
                checkout(username);
                break;
            }
            case 5: {
                viewOrderHistory(username);
                break;
            }
            case 6: {
                UI::printInfo("Logging out...");
                UI::sleepMilliseconds(1000);
                break;
            }
            default: {
                UI::printError("Invalid choice!");
                UI::sleepMilliseconds(1000);
            }
        }
    } while (choice != 6);
}