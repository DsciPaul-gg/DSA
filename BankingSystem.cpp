#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime>   

using namespace std;

struct Account {
    int accountNumber;
    string customerName;
    int age;
    string sex;
    string address;
    string dateOfBirth;
    string email;
    string phoneNumber;
    string sourceOfIncome;
    double initialDeposit;
};

void clearScreen() {
    
    system("cls");
}

int generateAccountNumber() {
    
    return rand() % 10;
}

void createAccount(vector<Account> &accounts) {

    clearScreen();

    Account newAccount;
    newAccount.accountNumber = generateAccountNumber();

    cout << "Generated Account Number: " << newAccount.accountNumber << endl;

    cout << "Enter Customer Name: ";
    cin.ignore();
    getline(cin, newAccount.customerName);

    cout << "Enter Age: ";
    cin >> newAccount.age;

    cin.ignore();

    cout << "Enter Sex: ";
    getline(cin, newAccount.sex);

    cout << "Enter Address: ";
    getline(cin, newAccount.address);

    cout << "Enter Date of Birth (DD/MM/YYYY): ";
    getline(cin, newAccount.dateOfBirth);

    cout << "Enter Email: ";
    getline(cin, newAccount.email);

    cout << "Enter Phone Number: ";
    getline(cin, newAccount.phoneNumber);

    cout << "Enter Source of Income: ";
    getline(cin, newAccount.sourceOfIncome);

    cout << "Enter Initial Deposit: ";
    cin >> newAccount.initialDeposit;

    cin.ignore();

    accounts.push_back(newAccount);
    cout << "Account created successfully." << endl;
}



int findAccount(const vector<Account> &accounts, int accNum) {
    
    for (int i = 0; i < accounts.size(); i++) {
        if (accounts[i].accountNumber == accNum) {
            return i;
        }
    }
    return -1;
}

void deposit(vector<Account> &accounts) {
    
    clearScreen(); 

    int accNum, index;
    double amount;

    cout << "Enter Account Number: ";
    cin >> accNum;

    index = findAccount(accounts, accNum);
    if (index != -1) {
        cout << "Enter Deposit Amount: ";
        cin >> amount;
        accounts[index].initialDeposit += amount;
        cout << "Deposit successful. New balance: " << accounts[index].initialDeposit << endl;
    } else {
        cout << "Account not found." << endl;
    }
}

void displayAccount(const vector<Account> &accounts) {
    
    clearScreen();

    int accNum, index;

    cout << "Enter Account Number: ";
    cin >> accNum;

    index = findAccount(accounts, accNum);
    if (index != -1) {
        cout << "-----DETAILS OF CLIENTS-----" << endl << endl;
        cout << "Account Number: " << accounts[index].accountNumber << endl;
        cout << "Customer Name: " << accounts[index].customerName << endl;
        cout << "Age: " << accounts[index].age << endl;
        cout << "Sex: " << accounts[index].sex << endl;
        cout << "Address: " << accounts[index].address << endl;
        cout << "Date of Birth: " << accounts[index].dateOfBirth << endl;
        cout << "Email: " << accounts[index].email << endl;
        cout << "Phone Number: " << accounts[index].phoneNumber << endl;
        cout << "Source of Income: " << accounts[index].sourceOfIncome << endl;
        cout << "Account Balance: " << accounts[index].initialDeposit << endl;
        cout << "\n--------------------------------";
    } else {
        cout << "Account not found." << endl;
    }
}

void deleteAccount(vector<Account> &accounts) {
    clearScreen();

    int accNum;
    cout << "Enter Account Number to delete: ";
    cin >> accNum;

    int index = findAccount(accounts, accNum);
    if (index != -1) {
        accounts.erase(accounts.begin() + index);
        cout << "Account deleted successfully." << endl;
    } else {
        cout << "Account not found." << endl;
    }
}

void saveAccountsToFile(const vector<Account> &accounts, const string &filename) {
    
    ofstream outFile(filename);

    for (const auto &account : accounts) {
        outFile << account.accountNumber << "\n"
                << account.customerName << "\n"
                << account.age << "\n"
                << account.sex << "\n"
                << account.address << "\n"
                << account.dateOfBirth << "\n"
                << account.email << "\n"
                << account.phoneNumber << "\n"
                << account.sourceOfIncome << "\n"
                << account.initialDeposit << "\n";
    }
    outFile.close();
    cout << "Accounts saved to file successfully." << endl;
}

void loadAccountsFromFile(vector<Account> &accounts, const string &filename) {
    
    ifstream inFile(filename);

    if (!inFile) {
        cout << "File not found. Starting with an empty database." << endl;
        return;
    }

    Account tempAccount;
    while (inFile >> tempAccount.accountNumber) {
        inFile.ignore();
        getline(inFile, tempAccount.customerName);
        inFile >> tempAccount.age;
        inFile.ignore();
        getline(inFile, tempAccount.sex);
        getline(inFile, tempAccount.address);
        getline(inFile, tempAccount.dateOfBirth);
        getline(inFile, tempAccount.email);
        getline(inFile, tempAccount.phoneNumber);
        getline(inFile, tempAccount.sourceOfIncome);
        inFile >> tempAccount.initialDeposit;
        inFile.ignore(); 
        accounts.push_back(tempAccount);
    }
    inFile.close();
    cout << "Accounts loaded from file successfully." << endl;
}

void displayMenu() {
    
    cout << "\n------MAIN MENU------" << endl;
    cout << "\n1. Create Account" << endl;
    cout << "2. Deposit" << endl;
    cout << "3. Display Account" << endl;
    cout << "4. Delete Account" << endl;
    cout << "5. Save & Exit" << endl;
    cout << "\nEnter your choice: ";

}

void handleMenuOption(int choice, vector<Account> &accounts, const string &filename) {
    
    switch (choice) {
    case 1:
        createAccount(accounts);
        break;

    case 2:
        deposit(accounts);
        break;

    case 3:
        displayAccount(accounts);
        break;

    case 4:
        deleteAccount(accounts);
        break;

    case 5:
        saveAccountsToFile(accounts, filename);
        cout << "Exiting program..." << endl;
        exit(0);

    default:
        cout << "Invalid choice! Please try again." << endl;
    }
}

int main() {
    
    vector<Account> accounts;
    string filename = "BankoDeOro.txt";
    int choice;

    srand(time(0));
    loadAccountsFromFile(accounts, filename);

    while (true) {
        displayMenu(); 
        cin >> choice;
        handleMenuOption(choice, accounts, filename);
    }

    return 0;
}
