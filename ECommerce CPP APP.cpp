#include<iostream>
#include<fstream>
#include<cstring>
#include<windows.h>
using namespace std;

const char adminFile[]="admin.txt";
const char userFile[]="users.txt";
const char ordersFile[]="orders.txt";
const char productsFile[]="products.txt";

// Structure to represent a product
struct Product{
    char name[50];
    float price;
    int quantity;
};

// Structure to represent a user
struct User{
    char username[50];
    char password[50];
    char email[50];
};

// Structure to represent an order
struct Order{
    char username[50];
    char productName[50];
    int quantity;
    float totalAmount;
    char status[20];
};

// Function declarations
void adminLogin();
void userLogin();
void userRegistration();
void displayProducts();
void addToCart(const char* username);
void removeFromCart(const char* username);
void viewCart(const char* username);
void makePurchase(const char* username);
void viewHistory(const char* username);
void adminMenu();
void siteBalance();
void adminWithdrawal();
void adminDeposit();
void changeAdminPassword();
void viewAllOrders();
void markOrderDelivered();
void loadProducts(Product products[], int& productCount);
void saveProducts(const Product products[], int productCount);
void userMenu(const char* username);

bool fileExists(const char* fileName);
bool fileExist(const string& fileName);
bool containsAlphabet(const char* str);
bool containsDigits(const char* str);
bool emailValid(const char* str);
int main(){
	
	system("color A");
	
    if (!fileExists(adminFile)) {
        ofstream adminStream(adminFile);
        adminStream.close();
    }
    if (!fileExists(userFile)) {
        ofstream userStream(userFile);
        userStream.close();
    }
    if (!fileExists(ordersFile)) {
        ofstream ordersStream(ordersFile);
        ordersStream.close();
    }
    if (!fileExists(productsFile)) {
        ofstream productsStream(productsFile);
        productsStream.close();
    }
    int choice;
    do{
    	system("color A");
		system("cls");
        cout<<"\n\n\n";
        cout<<"\t 1. Admin login"<<endl;
        cout<<"\t 2. User Login"<<endl;
        cout<<"\t 3. User Registeration"<<endl;
        cout<<"\t 4. Exit"<<endl;
        cout<<"\t ";
        cin >> choice;
        switch (choice){
            case 1:
                adminLogin();
                break;
	            case 2:
	                userLogin();
	                break;
		            case 3:
		                userRegistration();
		                break;
		                case 4:
		                	exit(0);
		                	break;
				            default:
				                cout<<"\t Enter the valid input"<<endl;
				                break;
        }
    } while (choice != 4);

    return 0;
}

bool containsAlphabet(const char* str){
	while(*str){
		if(isalpha(*str))
		return true;
		str++;
		
	}
	return false;
}
bool containsDigits(const char* str){
	while(*str){
		if(isdigit(*str))
		return true;
		str++;
		
	}
	return false;
}
bool fileExist(const string& fileName){
	ifstream file(fileName);
	return file.good();
}
bool emailValid(const char* str){
	return (strstr(str,"@gmail.com")!=nullptr);
} 

void adminLogin(){
	system("cls");
	cout<<"\n\n\n";
    ifstream adminLogin(adminFile);
    if (!adminLogin.is_open()) {
        cout<<"\t Error opening file...."<<endl;
        return;
    }
    char enteredPassword[10];
    char storedPassword[10];
    
    adminLogin >> storedPassword;

    cout<<"\t Enter admin password : ";
    cin>>enteredPassword;
    cout<<endl;
    if (strcmp(enteredPassword, storedPassword) == 0){
    	system("cls");
	    cout<<"\n";
	    cout<<"\n\n\n\t\t\t\t\t\tloading"<<endl;
	    char loading=219;
	    cout<<"\t\t\t\t";
	    for(int i=0;i<=40;i++){
	   		cout<<loading;
	       	Sleep(10);
		}
        adminMenu();
    }
	else{
		system("cls");
	    cout<<"\n";
	    cout<<"\n\n\n\t\t\t\t\t\tloading"<<endl<<endl;
	    char loading=219;
	    cout<<"\t\t\t\t";
	    for(int i=0;i<=40;i++){
	   		cout<<loading;
	       	Sleep(50);
		}
        system("color C");
        cout<<"\n\n";
        cout<<"\t Incorrect password / Try Again"<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
    adminLogin.close();
}

void userRegistration(){
	system("cls");
	cout<<endl;
	cout<<"\t \t\t Press 0 for return to main"<<endl;
	cout<<"\n\n";
	
	string username;
    User newUser;
    ofstream userLogin;
    start1:
    cout<<"\t Enter username : ";
    cin>>username;
    if(username == "0"){
    	main();
	}
    cout<<endl;
    if(fileExist(username+".txt")){
    	cout<<"\t User already exists"<<endl<<endl;
    	cout<<"\t ";
    	system("pause");
    	goto start1;
	}
    if(username.find(' ')!=string::npos){
    	cout<<"\t Spaces are not allowed "<<endl;
    	main();
	}
	start2:
	cin.ignore();
    cout<<"\t Enter password : ";
    cin>>newUser.password;
    if(newUser.password[0] == '0'){
    	main();
	}
	else{
		cout<<endl;
	    if(!containsAlphabet(newUser.password)){
	    	cout<<"\t Password must contains at least one alphabet "<<endl;
	    	cout<<endl;
	    	cout<<"\t ";
	    	system("pause");
	    	goto start2;
		}
		if(!containsDigits(newUser.password)){
	    	cout<<"\t Password must contains at least one digit "<<endl;
	    	cout<<endl;
	    	cout<<"\t ";
	    	system("pause");
	    	goto start2;
		}
	}
    
	start3:
	cin.ignore();
    cout<<"\t Enter your email : ";
    cin>>newUser.email;
    if(newUser.email[0] == '0'){
    	main();
	}
	else{
		cout<<endl;
		if(!emailValid(newUser.email)){
			cout<<"\t Email format is incorrect"<<endl;
	    	cout<<endl;
	    	cout<<"\t ";
	    	system("pause");
	    	goto start3;
		}
	}
    
    userLogin.open(username+".txt");
    if (!userLogin.is_open()) {
        cout<<"\t Error opening file...."<<endl;
        return;
    }
    userLogin<<username<<endl;
	userLogin<<newUser.password<<endl;
	userLogin<<newUser.email<<endl;
    userLogin.close();
    system("cls");
	cout<<"\n";
	cout<<"\n\n\n\t\t\t\t\t\tloading"<<endl;
	char loading=219;
	cout<<"\t\t\t\t";
	for(int i=0;i<=40;i++){
	   	cout<<loading;
	    Sleep(50);
	}
	cout<<"\n\n";

    cout<<"\t User register successful..."<<endl;
    cout<<endl;
    cout<<"\t ";
    system("pause");
    main();
}

void userLogin(){
	system("cls");
	system("color A");
	cout<<"\n\n\n";
    User user;
    ifstream userFile;
    string username;

    cout<<"\t Enter username : ";
    cin>>username;
    if(username == "0"){
    	main();
	}
    cout<<endl;
    cout<<"\t Enter password : ";
    cin>>user.password;
    if(user.password[0] == '0'){
    	main();
	}
    cout<<endl;
    userFile.open(username + ".txt");

    if (!userFile.is_open()) {
    	system("color C");
        cout<<"\t Error opening file / Try Again"<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
    char storedPassword[10];
    char email[50];
    userFile>>user.username;
	userFile>>storedPassword;
	userFile>>email;
    userFile.close();
    if (strcmp(user.password, storedPassword) == 0){
    	system("cls");
	    cout<<"\n";
	    cout<<"\n\n\n\t\t\t\t\t\tloading"<<endl;
	    char loading=219;
	    cout<<"\t\t\t\t";
	    for(int i=0;i<=40;i++){
	   		cout<<loading;
	       	Sleep(10);
		}
        userMenu(user.username);
    }
	else{
		system("cls");
	    cout<<"\n";
	    cout<<"\n\n\n\t\t\t\t\t\tloading"<<endl;
	    char loading=219;
	    cout<<"\t\t\t\t";
	    for(int i=0;i<=40;i++){
	   		cout<<loading;
	       	Sleep(20);
		}
		cout<<"\n\n";
        system("color C");
        cout<<"\t Incorrect username / password / Try Again"<<endl<<endl;
        cout<<"\t ";
        system("pause");
        main();
    }
}
// Function to display products to the user
void displayProducts(){
	system("cls");
	cout<<"\n\n\n";
    Product products[100];
    int productCount = 0;
    loadProducts(products, productCount);
    cout<<"\t\t Avaliable Products"<<endl;
    cout<<"\t ----------------------------------------"<<endl;
    cout<<"\t ID\tName\t\tPrice\tQuantity\n";
    for(int i=0;i<productCount;++i){
        cout<<"\t "<<i+1<<"\t"<<products[i].name<<"\t\t"<<products[i].price<<"\t"<<products[i].quantity<<endl;
    }
    cout<<"\t ----------------------------------------"<<endl<<endl;
    cout<<"\t ";
    system("pause");
}

void addToCart(const char* username){
    Product products[100];
    int productCount = 0;
    int productID;
    int quantity;
    loadProducts(products, productCount);
    displayProducts();
    cout<<"\t Enter the product ID to add to cart : ";
    cin>>productID;
    cout<<endl;
    if(productID>=1&&productID<=productCount){
        cout<<"\t Enter the quantity : ";
        cin>>quantity;
        cout<<endl;
        if(quantity>0&&quantity<=products[productID-1].quantity){
            products[productID-1].quantity -=quantity;
            saveProducts(products,productCount);
    		string usernameStr(username);
    		
    		ofstream cartSystem((usernameStr+"_cart.txt").c_str(),ios::app);
            if(!cartSystem.is_open()){
            	system("color B");
                cout<<"\t Error opening file"<<endl;
                return;
            }
            cartSystem<<products[productID-1].name<<"\t"<<products[productID-1].price<<"\t"<<quantity<<endl;
            cartSystem.close();
            cout<<"\t Product added to cart successfully..."<<endl<<endl;
            cout<<"\t ";
            system("pause");
        }
		else{
			system("color C");
            cout<<"\t Invalid quantity...."<<endl<<endl;
            cout<<"\t ";
            system("pause");
        }
    }
	else{
        system("color C");
        cout<<"\t Invalid product ID...."<<endl<<endl;
        cout<<"\t ";
        system("pause");
    }
}

void removeFromCart(const char* username){
	string usernameStr(username);
    std::ifstream cartStream((usernameStr + "_cart.txt").c_str(), std::ios::app);
    if (!cartStream.is_open()) {
        std::cerr << "\t Error opening cart file.\n";
        return;
    }

    Product products[100];
    int productCount = 0;

    // Load products from file
    loadProducts(products, productCount);

    displayProducts();

    int productID;
    int quantity;

    cout << "\t Enter the product ID to remove from cart: ";
    cin >> productID;

    if (productID >= 1 && productID <= productCount) {
        cout<<"\t Enter the quantity to remove: ";
        cin >> quantity;
        products[productID - 1].quantity += quantity;
        saveProducts(products, productCount);
        ofstream tempStream("temp.txt", std::ios::app);
        char productName[50];
        float price;

        while (cartStream >> productName >> price >> quantity) {
            if (strcmp(products[productID - 1].name, productName) == 0) {
                continue;
            }

            tempStream << productName << "\t" << price << "\t" << quantity << "\n";
        }

        tempStream.close();
        cartStream.close();
         remove((std::string(username) + ".txt").c_str());
    rename("temp.txt", (string(username) + ".txt").c_str());

        cout << "\t Product removed from cart successfully.\n";
    } else {
        cout << "\t Invalid product ID. Try again.\n";
    }
}

void viewCart(const char* username){
	system("cls");
	cout<<"\n\n\n";
	string usernameStr(username);
    ifstream cartDisplay((usernameStr+"_cart.txt").c_str(),ios::app);
    if (!cartDisplay.is_open()) {
    	system("color C");
        cout<<"\t Error opening file"<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
    cout<<"\t Your Cart : "<<endl;
    cout<<"\t ----------------------------------------"<<endl;
    cout<<"\t Name \t\t Price \t Quantity"<<endl;
    char productName[50];
    float price;
    int quantity;
    while(cartDisplay>>productName>>price>>quantity){
        cout<<"\t "<<productName<<"\t\t"<<price<<"\t"<<quantity<<endl;
    }
    cartDisplay.close();
    cout<<endl;
    cout<<"\t ";
    system("pause");
}

void makePurchase(const char* username){
	system("cls");
	cout<<"\n\n\n";
	string usernameStr(username);
    ifstream cartPurchase((usernameStr+"_cart.txt").c_str(),ios::app);
    if(!cartPurchase.is_open()){
        system("color C");
        cout<<"\t Error opening cart file"<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
    Order order;
    order.totalAmount = 0;
    strcpy(order.username, username);
    strcpy(order.status, "Pending");

    char productName[50];
    float price;
    int quantity;
    while (cartPurchase>>productName>>price>>quantity){
        strcpy(order.productName, productName);
        order.quantity=quantity;
        order.totalAmount+=price*quantity;
        ofstream cartPurchase(ordersFile,ios::app);
        cartPurchase<<order.username<<"\t"<<order.productName<<"\t"<<order.quantity<<"\t"<<order.totalAmount<<"\t"<<order.status<<endl;
        cartPurchase.close();
    }

    cartPurchase.close();
    remove((string(username)+"_cart.txt").c_str());
    cout<<"\t Purchase successful..."<<endl;
	cout<<"\t Total amount : "<<order.totalAmount<<endl<<endl;
	cout<<"\t ";
	system("pause");
}

void viewHistory(const char* username){
	system("cls");
	cout<<"\n\n\n";
	string usernameStr(username);
    ifstream historyView((usernameStr+"_history.txt").c_str(),ios::app);
    if(!historyView.is_open()){
        system("color C");
        cout<<"\t Error opening history file"<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
	else{
		cout<<"\t Your Purchase History"<<endl;
    	cout<<"\t ----------------------------------------"<<endl;
		string line;
		while(getline(historyView,line)){
			cout<<"\t "<<line<<endl;
		}
		historyView.close();
	}
	cout<<"\t ";
	system("pause");
}

void adminAddProduct(){
	system("cls");
	cout<<"\n\n\n";
    Product newProduct;
    cout<<"\t Enter the name of the new product : ";
    cin.ignore();
    cin.getline(newProduct.name,sizeof(newProduct.name));
    cout<<endl;
    cout<<"\t Enter the price of the new product : ";
    cin>>newProduct.price;
    cout<<endl;
    cout<<"\t Enter the quantity of the new product : ";
    cin>>newProduct.quantity;

    Product products[100];
    int productCount = 0;
    loadProducts(products, productCount);
    products[productCount++] = newProduct;
    saveProducts(products, productCount);
    cout<<endl;
    cout<<"\t Product added successfully..."<<endl<<endl;
    cout<<"\t ";
    system("pause");
}
// Function to display the admin menu
void adminMenu(){
    int choice;
    do {
    	system("cls");
    	cout<<"\n\n\n";
    	system("color A");
        cout<<"\t Admin Menu"<<endl;
        cout<<"\t 1. Admin Balance"<<endl;
		cout<<"\t 2. Admin Withdrawal"<<endl;
        cout<<"\t 3. Change Admin Password"<<endl;
		cout<<"\t 4. View All Orders"<<endl;
        cout<<"\t 5. Mark Order Delivered"<<endl;
        cout<<"\t 6. Add Products"<<endl;
		cout<<"\t 7. Logout"<<endl;
		cout<<"\t Choice : ";
        cin>>choice;
        switch(choice){
            case 1:
                siteBalance();
                break;
	            case 2:
	                adminWithdrawal();
	                break;
		            case 3:
		                changeAdminPassword();
		                break;
			            case 4:
			                viewAllOrders();
			                break;
				            case 5:
				                markOrderDelivered();
				                break;
					            case 6:
					                adminAddProduct();
					                break;
						            case 7:
						                cout<<"\t Logout"<<endl;
						                break;
									    default:
									        cout<<"\t Enter valid input"<<endl;
									        break;
       	}
    } while (choice != 7);
}

// Function to display the site balance
void siteBalance() {
	system("cls");
	cout<<"\n\n\n";
    float balance = 0;
    ifstream ordersStream(ordersFile);
    if(!ordersStream.is_open()){
        system("color C");
        cout<<"\t Error opening file"<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
    char username[50];
    char productName[50];
    int quantity;
    float totalAmount;
    char status[20];
    while(ordersStream>>username>>productName>>quantity>>totalAmount>>status){
        if(strcmp(status,"Delivered")==0){
            balance+=totalAmount;
        }
    }
    ordersStream.close();
    cout<<"\t Site Balance : "<<balance<<endl<<endl;
    cout<<"\t ";
    system("pause");
    
}

void adminWithdrawal(){
	system("cls");
	cout<<"\n\n\n";
    float withdrawalAmount;
    cout<<"\t Enter the withdrawal amount : ";
    cin>>withdrawalAmount;
    cout<<endl;
    float currentBalance = 0;
    ifstream orderSee(ordersFile);
    if(!orderSee.is_open()){
        system("color C");
        cout<<"\t Error opening file"<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
    char username[50];
    char productName[50];
    int quantity;
    float totalAmount;
    char status[20];
    while(orderSee>>username>>productName>>quantity>>totalAmount>>status){
        if(strcmp(status,"Delivered")==0){
            currentBalance+=totalAmount;
        }
    }
    orderSee.close();
    if(withdrawalAmount>0&&withdrawalAmount<=currentBalance){
        float newBalance=currentBalance-withdrawalAmount;
        ofstream adminStream(adminFile);
        adminStream<<newBalance<<endl;
        adminStream.close();
        cout<<"\t Withdrawal successful...."<<endl;
		cout<<"\t Updated site balance : "<<newBalance<<endl<<endl;
		cout<<"\t ";
		system("pause");
    }
	else{
        system("color C");
        cout<<"\t Invalid Withdrawal amount..."<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
}

void changeAdminPassword(){
	system("cls");
	cout<<"\n\n\n";
    char newPassword[50];
    cout<<"\t Enter the new admin password : ";
    cin>>newPassword;
    cout<<endl;
    ofstream adminPanel(adminFile);
    adminPanel<<newPassword<<endl;
    adminPanel.close();
    cout<<"\t Admin password updated successfully..."<<endl<<endl;
    cout<<"\t ";
    system("pause");
}

void viewAllOrders(){
	system("cls");
	cout<<"\n\n\n";
    ifstream orderSee(ordersFile);
    if(!orderSee.is_open()){
        system("color C");
        cout<<"\t Error opening file..."<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
    cout<<"\t\t All Orders"<<endl;
    cout<<"\t ---------------------------------------------------------------------------------------"<<endl;
    cout<<"\t Username \t Name \t\t Quantity \tTotal Amount \t Status"<<endl;
    char username[50];
    char productName[50];
    int quantity;
    float totalAmount;
    char status[20];
    while(orderSee>>username>>productName>>quantity>>totalAmount>>status){
        cout<<"\t "<<username<<"\t\t"<<productName<<"\t\t"<<quantity<<"\t\t"<<totalAmount<<"\t\t"<<status<<endl;
    }
    cout<<"\t ---------------------------------------------------------------------------------------"<<endl;
    orderSee.close();
    cout<<endl;
    cout<<"\t ";
    system("pause");
}

void markOrderDelivered(){
	system("cls");
	cout<<"\n\n\n";
    ifstream orderDelivered(ordersFile);
    if(!orderDelivered.is_open()){
        system("color C");
        cout<<"\t Error opening order file..."<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
    cout<<"\t Pending Orders"<<endl;
    cout<<"\t ----------------------------------------"<<endl;
    cout<<"\t Username\tName\t\tQuantity\tTotal Amount\tStatus"<<endl;
    char username[50];
    char productName[50];
    int quantity;
    float totalAmount;
    char status[20];
    bool foundPendingOrder = false;
    while(orderDelivered>>username>>productName>>quantity>>totalAmount>>status){
        if(strcmp(status,"Pending")==0){
            foundPendingOrder=true;
            cout<<"\t "<<username<<"\t\t"<<productName<<"\t\t"<<quantity<<"\t\t"<<totalAmount<<"\t\t"<<status<<endl;
        }
    }
    cout<<"\t ----------------------------------------"<<endl;
    orderDelivered.close();
    if(!foundPendingOrder){
        system("color C");
        cout<<"\t No pending orders to mark as delivered..."<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
    string usernameToMark;
    cout<<"\t Enter the username to mark the order as delivered : ";
    cin>>usernameToMark;
    cout<<endl;
    ifstream ordersStream2(ordersFile);
    ofstream tempStream("temp.txt",ios::app);
    while(ordersStream2>>username>>productName>>quantity>>totalAmount>>status){
        if(strcmp(username,usernameToMark.c_str())==0&&strcmp(status,"Pending")==0){
            cout<<"\t Order marked as delivered for user : "<<username<<endl;
            tempStream<<username<<"\t"<<productName<<"\t"<<quantity<<"\t"<<totalAmount<<"\tDelivered"<<endl;
            ofstream ordersStream3((string(username)+"_history.txt"),ios::app);
            ordersStream3<<"\t Product : "<<productName<<endl;
			ordersStream3<<"\t Quantity : "<<quantity<<endl;
			ordersStream3<<"\t Amount : "<<totalAmount<<endl;
            ordersStream3.close();
        }
		else{
            tempStream<<username<<"\t"<<productName<<"\t"<<quantity<<"\t"<<totalAmount<<"\t"<<status<<endl;
        }
    }
    tempStream.close();
    ordersStream2.close();
    remove(ordersFile);
    rename("temp.txt", ordersFile);
    cout<<"\t ";
    system("pause");
}

void loadProducts(Product products[], int& productCount){
    ifstream productsee(productsFile);
    if (!productsee.is_open()) {
        system("color C");
        cout<<"\t Error opening file..."<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
    productCount = 0;
    while(productsee>>products[productCount].name>>products[productCount].price>>products[productCount].quantity){
        ++productCount;
    }
    productsee.close();
}

void saveProducts(const Product products[],int productCount){
    ofstream productSave(productsFile);
    if(!productSave.is_open()){
        system("color C");
        cout<<"\t Error opening file..."<<endl<<endl;
        cout<<"\t ";
        system("pause");
        return;
    }
    for(int i=0;i<productCount;++i){
        productSave<<products[i].name<<"\t"<<products[i].price<<"\t"<<products[i].quantity<<endl;;
    }
    productSave.close();
}

bool fileExists(const char* fileName){
    ifstream file(fileName);
    return file.good();
}
// Function for the user menu
void userMenu(const char* username){
    int choice;
    do {
    	system("color A");
    	system("cls");
    	cout<<"\n\n\n";
        cout<<"\t User Menu"<<endl;
        cout<<"\t 1. Display Products"<<endl;
		cout<<"\t 2. Add to Cart"<<endl;
		cout<<"\t 3. Remove from Cart"<<endl;
        cout<<"\t 4. View Cart"<<endl;
		cout<<"\t 5. Make Purchase"<<endl;
		cout<<"\t 6. View History"<<endl;
		cout<<"\t 7. Logout"<<endl;
		cout<<"\t Choice : ";
        cin>>choice;
        switch(choice){
            case 1:
                displayProducts();
                break;
	            case 2:
	                addToCart(username);
	                break;
		            case 3:
		                removeFromCart(username);
		                break;
			            case 4:
			                viewCart(username);
			                break;
				            case 5:
				                makePurchase(username);
				                break;
					            case 6:
					                viewHistory(username);
					                break;
						            case 7:
						            	{
						            	system("cls");
									    cout<<"\n";
									    cout<<"\n\n\n\t\t\t\t\t\tloading"<<endl;
									    char loading=219;
									    cout<<"\t\t\t\t";
									    for(int i=0;i<=40;i++){
									   		cout<<loading;
									       	Sleep(50);
										}
										cout<<"\n\n";
						                cout<<"\t Logout"<<endl;
						                break;
						            }
							            default:
							                cout<<"\t Enter valid input "<<endl;
							                break;
        }
    } while (choice != 7);
}