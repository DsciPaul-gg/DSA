#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <regex>
#include <limits>

using namespace std;

struct Property {
    int id;
    string address;
    string ownerName;
    float price;
    string type;
    string status;
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

        if (tokens.size() == 6) {
            Property property;
            property.id = stoi(tokens[0]);
            property.address = tokens[1];
            property.ownerName = tokens[2];
            property.price = stof(tokens[3]);
            property.type = tokens[4];
            property.status = tokens[5];
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
        cin >> value;
        if (cin.fail()) {
            cout << "Error: Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

        regex floatRegex("^[0-9]+(\\.[0-9]+)?$");
        if (regex_match(input, floatRegex)) {
            value = stof(input);
            return value;
        } else {
            cout << "Error: Please enter a valid number (e.g., 123.45).\n";
        }
    }
}

string getStringInput(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (value.empty()) {
            cout << "Error: Input cannot be empty. Try again.\n";
        } else {
            return value;
        }
    }
}

int getValidatedId(const string& prompt) {
    int id;
    while (true) {
        id = getIntInput(prompt);
        string idStr = to_string(id);
        if (idStr.length() > 5) {
            cout << "Error: Property ID must be a maximum of 5 digits.\n";
        } else if (idStr.find_first_not_of('0') == string::npos) {
            cout << "Error: Property ID cannot be all zeros.\n";
        } else if (id <= 0) {
            cout << "Error: Property ID must be a positive integer.\n";
        } else if (isIdTaken(id)) {
            cout << "Error: Property ID " << id << " is already taken. Please enter a unique ID.\n";
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
        if (!regex_match(address, addressRegex)) {
            cout << "Error: Please enter a valid address in the format (House Number, Purok/Street, Barangay, Town/City, Province).\n";
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
        if (!regex_match(name, nameRegex)) {
            cout << "Error: Please enter a valid name (Example: Paul Pogi).\n";
        } else {
            return name;
        }
    }
}

string getValidatedType(const string& prompt) {
    string type;
    while (true) {
        type = getStringInput(prompt);
        transform(type.begin(), type.end(), type.begin(), ::tolower);

        if (type == "house" || type == "House" || type == "H" || type == "h") {
            return "House";
        } else if (type == "apartment" || type == "Apartment" || type == "A" || type == "a") {
            return "Apartment"; 
        } else if (type == "commercial" || type == "Commercial" || type == "C" || type == "c") {
            return "Commercial";
        } else {
            cout << "Error: Please enter 'House', 'Apartment', 'Commercial', or ('H', 'A', 'C').\n";
        }
    }
}

string getValidatedStatus(const string& prompt) {
    string status;
    while (true) {
        status = getStringInput(prompt);
        transform(status.begin(), status.end(), status.begin(), ::tolower);
        
        if (status == "for sale" || status == "fs" || status == "s") {
            return "For SALE";
        } else if (status == "for rent" || status == "fr" || status == "r") {
            return "For RENT";
        } else {
            cout << "Error: Please enter 'For Sale', 'For Rent', or ('S', 'R').\n";
        }
    }
}

void saveProperties(const vector<Property>& properties) {
    ofstream file("properties.txt");
    for (const auto& property : properties) {
        file << property.id << ",\"" << property.address << "\"," << property.ownerName << ","
             << property.price << "," << property.type << "," << property.status << endl;
    }
    file.close();
}

void addProperty() {
    clearScreen();
    Property property;

    property.id = getValidatedId("Enter Property ID (Maximum of 5): ");
    property.address = getValidatedAddress("Enter Property Address (House Number, Street/Purok, Barangay, Town/City, Province): ");
    property.ownerName = getValidatedName("Enter Property Owner's Name (Example: Paul Pogi): ");
    property.price = getFloatInput("Enter Property Listing Price: ");
    property.type = getValidatedType("Enter Property Type (House/Apartment/Commercial or H/A/C): ");
    property.status = getValidatedStatus("Enter Property Status (For Sale/For Rent or S/R): ");

    ofstream file("properties.txt", ios::app);
    file << property.id << ",\"" << property.address << "\"," << property.ownerName << ","
         << property.price << "," << property.type << "," << property.status << endl;

    file.close();
    cout << "Property added successfully!\n";
}

string padTo(const string& str, size_t width) {
    if (str.size() < width) {
        return str + string(width - str.size(), ' ');
    }
    return str;
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

    cout << padTo("ID", idWidth) << "|"
         << padTo("Address", addressWidth) << "|"
         << padTo("Owner", ownerWidth) << "|"
         << padTo("Price", priceWidth) << "|"
         << padTo("Type", typeWidth) << "|"
         << padTo("Status", statusWidth) << endl;

    cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + statusWidth + 5, '-') << endl;

    for (const auto& property : properties) {
        cout << padTo(to_string(property.id), idWidth) << "|"
             << padTo(property.address, addressWidth) << "|"
             << padTo(property.ownerName, ownerWidth) << "|"
             << padTo(to_string(property.price), priceWidth) << "|"
             << padTo(property.type, typeWidth) << "|"
             << padTo(property.status, statusWidth) << endl;
    }
}

void updateProperty() {
    clearScreen(); 
    vector<Property> properties = loadProperties();
    int id = getIntInput("Enter Property ID to update: ");

    bool found = false;
    for (auto& property : properties) {
        if (property.id == id) {
            found = true;
            property.address = getValidatedAddress("Enter new Property Address (House Number, Street/Purok, Barangay, Town/City, Province): ");
            property.ownerName = getValidatedName("Enter new Property Owner's Name: ");
            property.price = getFloatInput("Enter new Property Listing Price: ");
            property.type = getValidatedType("Enter new Property Type (House/Apartment/Commercial) or (H / A / C): ");
            property.status = getValidatedStatus("Enter new Property Status (For Sale/For Rent) or (S / R): ");
            break;
        }
    }

    if (found) {
        saveProperties(properties);
        cout << "Property updated successfully!\n";
    } else {
        cout << "Property with ID " << id << " not found.\n";
    }
}

void deleteProperty() {
    clearScreen(); 
    vector<Property> properties;
    properties = loadProperties();
    string input;

    while (true) {
        cout << "Enter Property ID to delete: ";
        getline(cin, input);

        if (!input.empty() && all_of(input.begin(), input.end(), ::isdigit)) {
            int id = stoi(input);

            auto it = remove_if(properties.begin(), properties.end(),
                                [id](const Property& property) { return property.id == id; });

            if (it != properties.end()) {
                properties.erase(it, properties.end());
                saveProperties(properties);
                cout << "Property deleted successfully!\n";
            } else {
                cout << "Property with ID " << id << " not found.\n";
            }
            break;
        } else {
            cout << "Error: Please enter a valid Property ID (numbers only).\n";
        }
    }
}

void searchProperties() {
    clearScreen();
    vector<Property> properties = loadProperties();
    vector<Property> filteredProperties;

    int choice;
    string input;

    while (true) {
        cout << "Search by:\n1. Price Range\n2. Type\n3. Status\nEnter your choice: ";
        getline(cin, input);

        if (!input.empty() && all_of(input.begin(), input.end(), ::isdigit)) {
            choice = stoi(input);
            if (choice >= 1 && choice <= 3) {
                break; // Valid choice
            } else {
                cout << "Error: Please enter a valid option (1, 2, or 3).\n";
            }
        } else {
            cout << "Error: Please enter a valid option (1, 2, or 3).\n";
        }
    }

    if (choice == 1) {
        float minPrice, maxPrice;
        while (true) {
            minPrice = getFloatInput("Enter minimum price: ");
            maxPrice = getFloatInput("Enter maximum price: ");
            if (minPrice <= maxPrice) {
                break; 
            } else {
                cout << "Error: Minimum price cannot be greater than maximum price. Please try again.\n";
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
            type = getValidatedType("Enter property type (House/Apartment/Commercial or H/A/C): ");
            for (const auto& property : properties) {
                if (property.type == type) {
                    filteredProperties.push_back(property);
                }
            }
            if (!filteredProperties.empty()) {
                break;
            } else {
                cout << "No properties found for type: " << type << ". Please try again.\n";
            }
        }
    } else if (choice == 3) {
        string status;
        while (true) {
            status = getValidatedStatus("Enter status (For Sale/For Rent or S/R): ");
            for (const auto& property : properties) {
                if (property.status == status) {
                    filteredProperties.push_back(property);
                }
            }
            if (!filteredProperties.empty()) {
                break;
            } else {
                cout << "No properties found for status: " << status << ". Please try again.\n";
            }
        }
    }

    if (filteredProperties.empty()) {
        cout << "No properties found matching the criteria.\n";
        return;
    }

    const int idWidth = 10;
    const int addressWidth = 70;
    const int ownerWidth = 25;
    const int priceWidth = 25;
    const int typeWidth = 20;
    const int statusWidth = 20;

    cout << padTo("ID", idWidth) << "|"
         << padTo("Address", addressWidth) << "|"
         << padTo("Owner", ownerWidth) << "|"
         << padTo("Price", priceWidth) << "|"
         << padTo("Type", typeWidth) << "|"
         << padTo("Status", statusWidth) << endl;

    cout << string(idWidth + addressWidth + ownerWidth + priceWidth + typeWidth + statusWidth + 5, '-') << endl;

    for (const auto& property : filteredProperties) {
        cout << padTo(to_string(property.id), idWidth) << "|"
             << padTo(property.address, addressWidth) << "|"
             << padTo(property.ownerName, ownerWidth) << "|"
             << padTo(to_string(property.price), priceWidth) << "|"
             << padTo(property.type, typeWidth) << "|"
             << padTo(property.status, statusWidth) << endl;
    }
}

int getMenuChoice() {
    int choice;
    string input;

    while (true) {
        cout << "Enter your choice: ";
        getline(cin, input);

        if (!input.empty() && all_of(input.begin(), input.end(), ::isdigit)) {
            choice = stoi(input);
            if (choice >= 1 && choice <= 6) {
                return choice;
            } else {
                cout << "Error: Please enter a number between 1 and 6.\n";
            }
        } else {
            cout << "Error: Please enter a valid choice (1 to 6).\n";
        }
    }
}

int main() {
    int choice;

    do {
        clearScreen();
        cout << "\nReal Estate Management System\n";
        cout << "1. Add Property\n";
        cout << "2. Display All Properties\n";
        cout << "3. Update Property\n";
        cout << "4. Delete Property\n";
        cout << "5. Search Properties\n";
        cout << "6. Exit\n\n";

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
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }

        if (choice != 6) {
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
    } while (choice != 6);

    return 0;
}