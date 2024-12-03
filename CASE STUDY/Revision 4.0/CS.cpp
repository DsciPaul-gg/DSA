#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <regex>
#include <limits>
#include <iomanip>

using namespace std;

const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m"; 
const string GREEN = "\033[32m"; 
const string YELLOW = "\033[33m"; 
const string BLUE = "\033[34m";  
const string MAGENTA = "\033[35m"; 
const string CYAN = "\033[36m";  
const string WHITE = "\033[37m"; 

struct Property {
    int id;
    string address;
    string ownerName;
    float price;
    string type;
    string status;
    string contactNumber;
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

vector<string> split(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    bool insideQuotes = false;

    for (char ch : line) {
        if (ch == '\"') {
            insideQuotes = !insideQuotes;
        } else if (ch == delimiter && !insideQuotes) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += ch;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

vector<Property> loadProperties() {
    vector<Property> properties;
    ifstream file("properties.txt");
    string line;

    while (getline(file, line)) {
        vector<string> tokens = split(line, ',');

        if (tokens.size() == 7) {
            Property property;
            property.id = stoi(tokens[0]);
            property.address = tokens[1];
            property.ownerName = tokens[2];
            property.price = stof(tokens[3]);
            property.type = tokens[4];
            property.status = tokens[5];
            property.contactNumber = tokens[6];
            properties.push_back(property);
        } else {
            cout << "Invalid line: " << line << endl;
        }
    }

    file.close();
    return properties;
}

bool isIdTaken(int id) {
    vector<Property> properties = loadProperties();
    for (const auto& property : properties) {
        if (property.id == id) {
            return true;
        }
    }
    return false;
}

int getIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        if (input == "M" || input == "m") {
            return -1;
        }
        stringstream ss(input);
        ss >> value;
        if (ss.fail() || !ss.eof()) {
            cout << RED << "Error: Please enter a valid number.\n" << RESET;
        } else {
            return value;
        }
    }
}

float getFloatInput(const string& prompt) {
    string input;
    float value;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input == "M" || input == "m") {
            return -1;
        }
        regex floatRegex("^[0-9]+(\\.[0-9]+)?$");
        if (regex_match(input, floatRegex)) {
            value = stof(input);
            return value;
        } else {
            cout << RED << "Error: Please enter a valid number (e.g., 123.45).\n" << RESET;
        }
    }
}

string getStringInput(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (value == "M" || value == "m") {
            return "";
        }
        if (value.empty()) {
            cout << RED << "Error: Input cannot be empty. Try again.\n" << RESET;
        } else {
            return value;
        }
    }
}

int getValidatedId(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input == "M" || input == "m") {
            return -1;
        }

        if (input.empty() || !all_of(input.begin(), input.end(), ::isdigit)) {
            cout << RED << "Error: Property ID must be a positive integer without any non-numeric characters.\n" << RESET;
            continue;
        }

        int id = stoi(input);
        string idStr = to_string(id);

        if (idStr.length() > 5) {
            cout << RED << "Error: Property ID must be a maximum of 5 digits.\n" << RESET;
        } else if (idStr.find_first_not_of('0') == string::npos) {
            cout << RED << "Error: Property ID cannot be all zeros.\n" << RESET;
        } else if (id <= 0) {
            cout << RED << "Error: Property ID must be a positive integer.\n" << RESET;
 } else if (isIdTaken(id)) {
            cout << RED <<"Error: Property ID " << id << " is already taken. Please enter a unique ID.\n" << RESET;
        } else {
            return id;
        }
    }
}

string getValidatedAddress(const string& prompt) {
    regex addressRegex("^[0-9]{1,5}, [a-zA-Z0-9 ]+, [a-zA-Z0-9 ]+, [a-zA-Z ]+, [a-zA-Z ]+");
    string address;
    while (true) {
        address = getStringInput(prompt);
        if (address == "") {
            return "";
        }
        if (!regex_match(address, addressRegex)) {
            cout << RED << "Error: Please enter a valid address in the format of (House Number, Purok/Street, Barangay, Town/City, Province).\n" << RESET;
        } else {
            return address;
        }
    }
}

string getValidatedName(const string& prompt) {
    regex nameRegex("^[a-zA-Z]+( [a-zA-Z]+)*$"); 
    string name;
    while (true) {
        name = getStringInput(prompt);
        if (name == "") {
            return "";
        }
        if (!regex_match(name, nameRegex)) {
            cout << RED << "Error: Please enter a valid name (Example: Paul Pogi).\n" << RESET;
        } else {
            return name;
        }
    }
}

string getValidatedType(const string& prompt) {
    string type;
    while (true) {
        type = getStringInput(prompt);
        if (type == "") {
            return "";
        }
        transform(type.begin(), type.end(), type.begin(), ::tolower);

        if (type == "house" || type == "House" || type == "H" || type == "h") {
            return "House";
        } else if (type == "apartment" || type == "Apartment" || type == "A" || type == "a") {
            return "Apartment"; 
        } else if (type == "commercial" || type == "Commercial" || type == "C" || type == "c") {
            return "Commercial";
        } else {
            cout << RED << "Error: Please enter 'House', 'Apartment', 'Commercial', or ('H', 'A', 'C').\n" << RESET;
        }
    }
}

string getValidatedStatus(const string& prompt) {
    string status;
    while (true) {
        status = getStringInput(prompt);
        if (status == "") {
            return "";
        }
        transform(status.begin(), status.end(), status.begin(), ::tolower);
        
        if (status == "for sale" || status == "fs" || status == "s") {
            return GREEN + "For Sale" + RESET;
        } else if (status == "for rent" || status == "fr" || status == "r") {
            return YELLOW + "For Rent" + RESET;
        } else {
            cout << RED << "Error: Please enter 'For Sale', 'For Rent', or ('S', 'R').\n" << RESET;
        }
    }
}

string getValidatedContactNumber(const string& prompt) {
    string contactNumber;
    regex contactRegex("^09[0-9]{9}$");
    while (true) {
        cout << prompt;
        getline(cin, contactNumber);
        
        if (contactNumber == "M" || contactNumber == "m") {
            return "";
        }

        if (regex_match(contactNumber, contactRegex)) {
            return contactNumber;
        } else {
            cout << RED <<"Error: Please enter a valid contact number starting with '09' and exactly 11 digits long.\n" << RESET;
        }
    }
}

void saveProperties(const vector<Property>& properties) {
    ofstream file("properties.txt");
    for (const auto& property : properties) {
        file << property.id << ",\"" << property.address << "\"," << property.ownerName << ","
             << property.price << "," << property.type << "," << property.status << ","
             << property.contactNumber << endl;
    }
    file.close();
}

string padTo(const string& str, size_t width) {
    if (str.size() < width) {
        return str + string(width - str.size(), ' ');
    }
    return str;
}

void addProperty() {
    clearScreen();
    Property property;

    cout << BOLD << CYAN <<"\n\t\tEnter the letter 'M' or 'm' to go back to the Main Menu" << endl << endl << RESET;

    property.id = getValidatedId("\tEnter Property ID (Maximum of 5): ");
    if (property.id == -1) return;

    property.address = getValidatedAddress("\tEnter Property Address (House Number, Street/Purok, Barangay, Town/City, Province): ");
    if (property.address == "") return;

    property.ownerName = getValidatedName("\tEnter Property Owner's Name (Example: Paul Pogi): ");
    if (property.ownerName == "") return;

    property.price = getFloatInput("\tEnter Property Listing Price: ");
    if (property.price == -1) return;

    property.type = getValidatedType("\tEnter Property Type (House/Apartment/Commercial or H/A/C): ");
    if (property.type == "") return;

    property.status = getValidatedStatus("\tEnter Property Status (For Sale/For Rent or S/R): ");
    if (property.status == "") return;

    property.contactNumber = getValidatedContactNumber("\tEnter Contact Number (Example: 0922 Bahala Ka Na sa Pitu): ");
    if (property.contactNumber == "") return;

    ofstream file("properties.txt", ios::app);
    file << property.id << ",\"" << property.address << "\"," << property.ownerName << ","
         << property.price << "," << property.type << "," << property.status << ","
         << property.contactNumber << endl;

    file.close();
    cout << GREEN <<"\tProperty added successfully!\n" << RESET;
}

void displayProperties() {
    clearScreen();
    vector<Property> properties = loadProperties();

    const int idWidth = 10;
    const int addressWidth = 70;
    const int ownerWidth = 25;
    const int priceWidth = 25;
    const int typeWidth = 20;
    const int statusWidth = 20;
    const int contactWidth = 20;

    cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;

    cout << BOLD << CYAN
         << padTo("ID", idWidth) << "|"
         << padTo("Address", addressWidth) << "|"
         << padTo("Owner", ownerWidth) << "|"
         << padTo("Price", priceWidth) << "|"
         << padTo("Type", typeWidth) << "|"
         << padTo("Contact", contactWidth) << "|"
         << padTo("Status", statusWidth) << RESET << endl;

    cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;

    for (const auto& property : properties) {
        stringstream priceStream;
        priceStream << fixed << setprecision(2) << property.price;

        cout << padTo(to_string(property.id), idWidth) << "|"
             << padTo(property.address, addressWidth) << "|"
             << padTo(property.ownerName, ownerWidth) << "|"
             << padTo(priceStream.str(), priceWidth) << "|"
             << padTo(property.type, typeWidth) << "|"
             << padTo(property.contactNumber, contactWidth) << "|"
             << padTo(property.status, statusWidth) << endl;
    }
    
    cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;
}

void updateProperty() {
    clearScreen();
    vector<Property> properties = loadProperties();

    const int idWidth = 10;
    const int addressWidth = 70;
    const int ownerWidth = 25;
    const int priceWidth = 25;
    const int typeWidth = 20;
    const int statusWidth = 20;
    const int contactWidth = 20;

    cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;

    cout << BOLD << CYAN
         << padTo("ID", idWidth) << "|"
         << padTo("Address", addressWidth) << "|"
         << padTo("Owner", ownerWidth) << "|"
         << padTo("Price", priceWidth) << "|"
         << padTo("Type", typeWidth) << "|"
         << padTo("Contact", contactWidth) << "|"
         << padTo("Status", statusWidth) << RESET << endl;

    cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;

    for (const auto& property : properties) {
        stringstream priceStream;
        priceStream << fixed << setprecision(2) << property.price;

        cout << padTo(to_string(property.id), idWidth) << "|"
             << padTo(property.address, addressWidth) << "|"
             << padTo(property.ownerName, ownerWidth) << "|"
             << padTo(priceStream.str(), priceWidth) << "|"
             << padTo(property.type, typeWidth) << "|"
             << padTo(property.contactNumber, contactWidth) << "|"
             << padTo(property.status, statusWidth) << endl;
    }
    
    cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;

    cout << endl;

    cout << "\n\t\tEnter the letter 'M' or 'm' to go back to the Main Menu" << endl << endl;

    int id = getIntInput("\tEnter Property ID to update: ");
    if (id == -1) return;

    bool found = false;
    for (auto& property : properties) {
        if (property.id == id) {
            found = true;
            property.ownerName = getValidatedName("\tEnter new Property Owner's Name: ");
            if (property.ownerName == "") return;

            property.price = getFloatInput("\tEnter new Property Listing Price: ");
            if (property.price == -1) return;

            property.status = getValidatedStatus("\tEnter new Property Status (For Sale/For Rent or S/R): ");
            if (property.status == "") return;

            property.contactNumber = getValidatedContactNumber("\tEnter new Contact Number (Example: 09123456789): ");
            if (property.contactNumber == "") return;
            break;
        }
    }

    if (found) {
        saveProperties(properties);
        cout << GREEN << "\n\tProperty updated successfully!\n" << RESET;
    } else {
        cout << RED <<"Property with ID " << id << " not found.\n" << RESET;
    }
}

void deleteProperty() {
    clearScreen(); 
    vector<Property> properties = loadProperties();
    string input;

    const int idWidth = 10;
    const int addressWidth = 70;
    const int ownerWidth = 25;
    const int priceWidth = 25;
    const int typeWidth = 20;
    const int statusWidth = 20;
    const int contactWidth = 20;

    cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;

    cout << BOLD << CYAN
         << padTo("ID", idWidth) << "|"
         << padTo("Address", addressWidth) << "|"
         << padTo("Owner", ownerWidth) << "|"
         << padTo("Price", priceWidth) << "|"
         << padTo("Type", typeWidth) << "|"
         << padTo("Contact", contactWidth) << "|"
         << padTo("Status", statusWidth) << RESET << endl;

    cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;

    for (const auto& property : properties) {
        stringstream priceStream;
        priceStream << fixed << setprecision(2) << property.price;

        cout << padTo(to_string(property.id), idWidth) << "|"
             << padTo(property.address, addressWidth) << "|"
             << padTo(property.ownerName, ownerWidth) << "|"
             << padTo(priceStream.str(), priceWidth) << "|"
             << padTo(property.type, typeWidth) << "|"
             << padTo(property.contactNumber, contactWidth) << "|"
             << padTo(property.status, statusWidth) << endl;
    }
    
    cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;

    cout << endl;

        cout << "\n\t\tEnter the letter 'M' or 'm' to go back to the Main Menu" << endl << endl;

    while (true) {
        cout << "\tEnter Property ID to delete: ";
        getline(cin, input);
        if (input == "M" || input == "m") return;

        if (!input.empty() && all_of(input.begin(), input.end(), ::isdigit)) {
            int id = stoi(input);

            auto it = remove_if(properties.begin(), properties.end(),
                                [id](const Property& property) { return property.id == id; });

            if (it != properties.end()) {
                properties.erase(it, properties.end());
                saveProperties(properties);
                cout << GREEN <<"\n\tProperty deleted successfully!\n" << RESET;
                break;
        } else {
                cout << RED <<"Property with ID " << id << " not found. Please try again.\n" << RESET;
            }
        } else {
            cout << RED << "Error: Please enter a valid Property ID (numbers only).\n" << RESET;
        }
    }
}

void searchProperties() {
    clearScreen();
    vector<Property> properties = loadProperties();
    vector<Property> filteredProperties;

    string input;
    bool continueSearching = true;

    while (continueSearching) {
        clearScreen();
        filteredProperties.clear();
        cout << BOLD << CYAN <<"\n\t\tEnter the letter 'M' or 'm' to go back to the Main Menu\n\n" << RESET;
        cout << BOLD << YELLOW << "\tSearch by:\n\t1. Price Range\n\t2. Type\n\t3. Status\n" << RESET;

        while (true) {
            cout << "\n\tEnter your choice: ";
            getline(cin, input);
            if (input == "M" || input == "m") return;

            if (!input.empty() && all_of(input.begin(), input.end(), ::isdigit)) {
                int choice = stoi(input);
                if (choice >= 1 && choice <= 3) {
                    if (choice == 1) {
                        
                        float minPrice, maxPrice;
                        while (true) {
                            minPrice = getFloatInput("\tEnter minimum price: ");
                            if (minPrice == -1) return;

                            maxPrice = getFloatInput("\tEnter maximum price: ");
                            if (maxPrice == -1) return;

                            if (minPrice <= maxPrice) {
                                break; 
                            } else {
                                cout << RED << "Error: Minimum price cannot be greater than maximum price.\n" << RESET;
                            }
                        }

                        for (const auto& property : properties) {
                            if (property.price >= minPrice && property.price <= maxPrice) {
                                filteredProperties.push_back(property);
                            }
                        }
                    } else if (choice == 2) {
                       
                        string type;
                        while (true) {
                            type = getValidatedType("\tEnter property type (House/Apartment/Commercial or H/A/C): ");
                            if (type == "") return; 

                            for (const auto& property : properties) {
                                if (property.type == type) {
                                    filteredProperties.push_back(property);
                                }
                            }
                            if (!filteredProperties.empty()) {
                                break;
                            } else {
                                cout << RED <<"No properties found for type: " << type << ". Please try again.\n" << RESET;
                            }
                        }
                    } else if (choice == 3) {
                        
                        string status;
                        while (true) {
                            status = getValidatedStatus("\tEnter status (For Sale/For Rent or S/R): ");
                            if (status == "") return; 

                            for (const auto& property : properties) {
                                if (property.status == status) {
                                    filteredProperties.push_back(property);
                                }
                            }
                            if (!filteredProperties.empty()) {
                                break;
                            } else {
                                cout << RED <<"No properties found for status: " << status << ". Please try again.\n" << RESET;
                            }
                        }
                    }
                    break;
                } else {
                    cout << RED << "Error: Please enter a valid option (1, 2, or 3).\n" << RESET;
                }
            } else {
                cout << RED << "Error: Please enter a valid option (1, 2, or 3).\n" << RESET;
            }
        }

        if (filteredProperties.empty()) {
            cout << RED << "No properties found matching the criteria.\n" << RESET;
        } else {
            const int idWidth = 10;
            const int addressWidth = 70;
            const int ownerWidth = 25;
            const int priceWidth = 25;
            const int typeWidth = 20;
            const int statusWidth = 20;
            const int contactWidth = 20;

            cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;

            cout << BOLD << CYAN
                 << padTo("ID", idWidth) << "|"
                 << padTo("Address", addressWidth) << "|"
                 << padTo("Owner", ownerWidth) << "|"
                 << padTo("Price", priceWidth) << "|"
                 << padTo("Type", typeWidth) << "|"
                 << padTo("Contact", contactWidth) << "|"
                 << padTo("Status", statusWidth) << RESET << endl;

            cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;

                for (const auto& property : properties) {
                stringstream priceStream;
                priceStream << fixed << setprecision(2) << property.price;

            cout << padTo(to_string(property.id), idWidth) << "|"
                 << padTo(property.address, addressWidth) << "|"
                 << padTo(property.ownerName, ownerWidth) << "|"
                 << padTo(priceStream.str(), priceWidth) << "|"
                 << padTo(property.type, typeWidth) << "|"
                 << padTo(property.contactNumber, contactWidth) << "|"
                 << padTo(property.status, statusWidth) << endl;
            }   
    
            cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + contactWidth + statusWidth + 6, '-') << endl;
        }

        while (true) {
            cout << "\n\tDo you want to search again? (Yes or No): ";
            getline(cin, input);

            transform(input.begin(), input.end(), input.begin(), ::tolower);
            
            if (input == "y" || input == "yes") {
                continueSearching = true;
                break;
            } else if (input == "n" || input == "no") {
                continueSearching = false;
                break;
            } else {
                cout << RED << "Error: Please enter Yes/No or Y/N.\n" << RESET;
            }
        }
    }
}

int getMenuChoice() {
    int choice;
    string input;

    while (true) {
        cout << "\tEnter your choice: ";
        getline(cin, input);

        if (!input.empty() && all_of(input.begin(), input.end(), ::isdigit)) {
            choice = stoi(input);
            if (choice >= 1 && choice <= 6) {
                return choice;
            } else {
                cout << RED << "Error: Please enter a number between 1 and 6.\n" << RESET;
            }
        } else {
            cout << RED << "Error: Please enter a valid choice (1 to 6).\n" << RESET;
        }
    }
}

int main() {
    int choice;

    do {
        clearScreen();
        cout << BOLD << BLUE <<"\n\t\t | Real Estate Management System | \n\n" << RESET;
        cout << BOLD << YELLOW << "\t1. Add Property\n" << RESET;
        cout << BOLD << YELLOW << "\t2. Display All Properties\n" << RESET;
        cout << BOLD << YELLOW << "\t3. Update Property\n" << RESET;
        cout << BOLD << YELLOW << "\t4. Delete Property\n" << RESET;
        cout << BOLD << YELLOW << "\t5. Search Properties\n" << RESET;
        cout << BOLD << MAGENTA << "\t6. Exit\n\n" << RESET;

        choice = getMenuChoice();

        switch (choice) {
            case 1:
                addProperty();
                break;
            case 2:
                displayProperties();
                break;
            case 3:
                updateProperty();
                break;
            case 4:
                deleteProperty();
                break;
            case 5:
                searchProperties();
                break;
            case 6:
                cout << BOLD << RED << "Exiting program. Goodbye!\n" << RESET;
                break;
            default:
                cout << BOLD << RED << "Invalid choice.\n" << RESET;
                break;
        }

        if (choice != 6) {
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
    } while (choice != 6);

    return 0;
}