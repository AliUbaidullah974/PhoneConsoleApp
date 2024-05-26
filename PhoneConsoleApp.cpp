#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Contact struct
struct Contact {
    int id;
    string firstName;
    string lastName;
    unordered_map<string, string> phoneNumbers;
    string email;
    unordered_map<string, string> additionalFields;
    int age;
};

// Abstract base class
class ContactManagerBase {
public:
    virtual void addContact(const Contact& contact) = 0;
    virtual vector<Contact> searchContacts(const string& query) = 0;
    virtual void sortContactsByName() = 0;
    virtual void deleteContact(int id) = 0;
    virtual void displayContacts(const vector<Contact>& contactList) = 0;
    virtual void displayAllContacts() = 0;
    virtual void editContactNumber(int id, const string& phoneType, const string& phoneNumber) = 0;
    virtual ~ContactManagerBase() = default;
};

// ContactManager class
class ContactManager : public ContactManagerBase {
private:
    vector<Contact> contacts;
    unordered_map<int, Contact*> contactMap;
    int nextId = 1;

public:
    void addContact(const Contact& contact) override {
        contacts.push_back(contact);
        contacts.back().id = nextId;
        contactMap[nextId] = &contacts.back();
        cout << "Contact added with ID: " << nextId << endl;
        nextId++;
    }

    vector<Contact> searchContacts(const string& query) override {
        vector<Contact> results;
        for (const auto& contact : contacts) {
            if (contact.firstName.find(query) != string::npos || contact.lastName.find(query) != string::npos) {
                results.push_back(contact);
            }
        }
        return results;
    }

    void sortContactsByName() override {
        sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
            if (a.lastName == b.lastName) {
                return a.firstName < b.firstName;
            }
            return a.lastName < b.lastName;
            });
    }

    void deleteContact(int id) override {
        auto it = find_if(contacts.begin(), contacts.end(), [id](const Contact& contact) {
            return contact.id == id;
            });
        if (it != contacts.end()) {
            contactMap.erase(it->id);
            contacts.erase(it);
            cout << "Contact with ID " << id << " deleted." << endl;
        }
        else {
            cout << "Contact with ID " << id << " not found." << endl;
        }
    }

    void displayContacts(const vector<Contact>& contactList) override {
        for (const auto& contact : contactList) {
            cout << "ID: " << contact.id << endl;
            cout << "Name: " << contact.firstName << " " << contact.lastName << endl;
            cout << "Age: " << contact.age << endl;
            cout << "Email: " << contact.email << endl;
            for (const auto& phone : contact.phoneNumbers) {
                cout << phone.first << ": " << phone.second << endl;
            }
            for (const auto& field : contact.additionalFields) {
                cout << field.first << ": " << field.second << endl;
            }
            cout << "-------------------\n";
        }
    }

    void displayAllContacts() override {
        if (contacts.empty()) {
            cout << " " << endl;
            cout << "-------There are no contacts in the list. Please enter a new contact.-----" << endl;
            cout << " " << endl;
        }
        else {
            displayContacts(contacts);
        }
    }


    void editContactNumber(int id, const string& phoneType, const string& phoneNumber) override {
        auto it = contactMap.find(id);
        if (it != contactMap.end()) {
            Contact* contact = it->second;
            contact->phoneNumbers[phoneType] = phoneNumber;
            cout << "Updated phone number for contact ID " << id << ": " << phoneType << " -> " << phoneNumber << endl;
        }
        else {
            cout << "Contact with ID " << id << " not found." << endl;
        }
    }
};

Contact getContactFromUser() {
    string firstName, lastName, email;
    unordered_map<string, string> phoneNumbers;
    unordered_map<string, string> additionalFields;
    int age;

    cout << "Enter first name: ";
    cin >> firstName;
    cout << "Enter last name: ";
    cin >> lastName;
    cout << "Enter age: ";
    cin >> age;
    cout << "Enter email: ";
    cin >> email;

    int phoneCount;
    cout << "Enter number of phone numbers: ";
    cin >> phoneCount;
    for (int i = 0; i < phoneCount; ++i) {
        string type, number;
        cout << "Enter phone type (e.g., mobile, home): ";
        cin >> type;
        cout << "Enter phone number: ";
        cin >> number;
        phoneNumbers[type] = number;
    }

    int addressCount;
    cout << "Enter number of addresses: ";
    cin >> addressCount;
    cin.ignore(); // Ignore remaining newline character
    for (int i = 0; i < addressCount; ++i) {
        string type, address;
        cout << "Enter address type (e.g., home, work): ";
        cin >> type;
        cin.ignore(); // Ignore remaining newline character
        cout << "Enter address: ";
        getline(cin, address);
        additionalFields[type] = address;
    }

    return Contact{ 0, firstName, lastName, phoneNumbers, email, additionalFields, age };
}

void displayMenu() {
    cout << "1. Add Contact\n";
    cout << "2. Display All Contacts\n";
    cout << "3. Search Contacts\n";
    cout << "4. Edit Contact Number\n";
    cout << "5. Delete Contact\n";
    cout << "6. Sort Contacts by Name\n";
    cout << "7. Exit\n";
    cout << "Enter your choice (only int from 1 to 7): ";
}

int main() {
    

    ContactManager manager;
    int choice;
    cout << "Name:      ------Ali Ubaidullah------" << endl;
    cout << "Roll No:   ------S23BDOCS1M01125-----" << endl;

    while (true) { 
        
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            Contact contact = getContactFromUser();
            manager.addContact(contact);
            break;
        }
        case 2: {
            cout << "All Contacts:" << endl;
            manager.displayAllContacts();
            break;
        }
        case 3: {
            string searchQuery;
            cout << "Enter search query: ";
            cin >> searchQuery;
            auto results = manager.searchContacts(searchQuery);
            cout << "Search Results:" << endl;
            manager.displayContacts(results);
            break;
        }
        case 4: {
            int editId;
            string phoneType, newNumber;
            cout << "Enter contact ID to edit: ";
            cin >> editId;
            cout << "Enter phone type to edit (e.g., mobile, home): ";
            cin >> phoneType;
            cout << "Enter new phone number: ";
            cin >> newNumber;
            manager.editContactNumber(editId, phoneType, newNumber);
            break;
        }
        case 5: {
            int deleteId;
            cout << "Enter contact ID to delete: ";
            cin >> deleteId;
            manager.deleteContact(deleteId);
            break;
        }
        case 6: {
            manager.sortContactsByName();
            cout << "Contacts sorted by name:" << endl;
            manager.displayAllContacts();
            break;
        }
        case 7: {
            cout << "Exiting..." << endl;
            return 0;
        }
        default:
            cout << "------------*Invalid choice. Please try again.*---------" << endl;
            break;
        }
    }
}