#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <stdio.h>

#include "LoginScreen.h"

using namespace std;

#undef max
#define p system("pause")
#define c cout <<
#define e << endl

struct ContactListEntry {
    int id;
    int loggedUserId;
    string name;
    string surname;
    string address;
    string email;
    string phone;
};

int chooseOption(int numberOfOptions);
bool checkInputForMenus(string chosenOption, int numberOfOptions);

int logIn(LoginScreen loginScreen);

vector<ContactListEntry> loadContactListFromFile(int loggedUserId);
bool checkFileLineForLoggedUserId(string fileLine, int loggedUserId);
ContactListEntry splitFileLineToEntryVector(string fileLine);
void mainMenu();
void AddressBook(int loggedUserId, LoginScreen& loginScreen);
int getNewEntryId();
ContactListEntry addEntry(int newId, int loggedUserId);
void saveEntryToFile(ContactListEntry newContactListEntry);
string mergeVectorToFileLine(ContactListEntry newEntry);
void searchByName(vector<ContactListEntry> contactList);
void searchBySurname(vector<ContactListEntry> contactList);
void wholeList(vector<ContactListEntry> contactList);
void deleteEntry(vector<ContactListEntry>& contactList);
void deleteFromFile(int entryId);
void showContactToEditOrDelete(ContactListEntry entryToShow);
void editEntry(vector<ContactListEntry>& contactList);
ContactListEntry editGivenContact(ContactListEntry contactToEdit);
void editEntryInFile(ContactListEntry contactToEdit);/*
void changeUserPassword(int loggedUserId,vector<User>& usersList);
void editUserInFile(User userToEdit);*/

int chooseOption(int numberOfOptions)
{
    string chosenOption;
    bool correctInput = false;
    cout << endl;
    cout << "Choose action: ";
    while (!correctInput)
    {
        getline(cin, chosenOption);
        correctInput = checkInputForMenus(chosenOption, numberOfOptions);
        if (correctInput == 0)
        {
            cout << "\33[A" << "\33[2K" << "\r";
            cout << "Wrong input! Try again: ";
        }
    }
    return stoi(chosenOption);
}

bool checkInputForMenus(string chosenOption, int numberOfOptions)
{
    int len = chosenOption.length();
    if (len > 1) return 0;
    if (chosenOption[0] >= 49 &&
        chosenOption[0] < 49 + numberOfOptions)
        return 1;
    else return 0;
}

int logIn(LoginScreen loginScreen) {
    
    int chosenOption;
    int loggedUserId = 0;
            
    chosenOption = chooseOption(3);
    cout << endl;
    switch (chosenOption) {
    case 1:
    {
        loggedUserId = loginScreen.signIn(loginScreen.getUsersList());
        if (loggedUserId > 0) {
            return loggedUserId;
        }
    } break;
    case 2:
    {
        loginScreen.signUp(loginScreen.getUsersList());
        return -1;
    } break;
    case 3:
    {
        return 0;
    }
    }    
}

int checkUsersNameAndPassword(vector<User>& usersList) {

    string username;
    string password;
    int listSize = usersList.size();    

    bool isUsernameCorrect = false;
    bool isPasswordCorrect = false;
    int loggedUserId = 0;

    while (!isUsernameCorrect) {
        cout << "Username: ";
        getline(cin, username);

        for (int i = 0; i < listSize; i++) {
            if (username != usersList[i].name && i == listSize - 1) {
                cout << "User with that name does not exist, try again. Enter username: 0 to abort.\n";                
            }
            else if (username == usersList[i].name) {
                isUsernameCorrect = true;
                break;
            }
            else if (username == "0") {
                cout << endl << "Logging in aborted. Press enter to return to login screen . . .";
                cin.get();
                return -1;
            }
        }
    }

    while (!isPasswordCorrect) {
        cout << "Password: ";
        getline(cin, password);

        for (int i = 0; i < listSize; i++) {
            if (username == usersList[i].name && password == usersList[i].password) {
                cout << endl << "Correct username nad password. User is now logged in." << endl;
                cout << "Press enter to continue . . .";
                cin.get();
                isPasswordCorrect = true;
                loggedUserId = usersList[i].id;
                break;
            }
            else if (i == listSize-1) {
                cout << "Incorrect password, try again. Enter password: 0 to abort." << endl;                
            }
            else if (password == "0") {
                cout << endl << "Logging in aborted. Press enter to return to login screen . . .";
                cin.get();
                return -1;
            }
        }
    }        
    
    return loggedUserId;
}

vector<ContactListEntry> loadContactListFromFile(int loggedUserId)
{
    vector<ContactListEntry> contactList;
    fstream file;
    string fileLine;
    bool isThisLineForLoggedUser = false;

    file.open("contactlist.txt", ios::in);
    if (file.good() == 0) return contactList;

    while (getline(file, fileLine)) {
        isThisLineForLoggedUser = checkFileLineForLoggedUserId(fileLine, loggedUserId);
        if (isThisLineForLoggedUser == true) {
            contactList.push_back(splitFileLineToEntryVector(fileLine));
        }
    }

    file.close();
    return contactList;
}

bool checkFileLineForLoggedUserId(string fileLine, int loggedUserId) {
    ContactListEntry checkedEntry;
    checkedEntry = splitFileLineToEntryVector(fileLine);
    
    if (checkedEntry.loggedUserId == loggedUserId) {
        return true;
    }

    return false;
}

ContactListEntry splitFileLineToEntryVector(string fileLine) {
    int lineLength = fileLine.length();
    string tempInfoArray[7];
    int tempArrayIndex = 0;
    ContactListEntry newEntry;

    for (int i = 0; i < lineLength; i++) {
        if (fileLine[i] == 124) {
            tempInfoArray[tempArrayIndex] = fileLine.substr(0, i);
            fileLine = fileLine.substr(i + 1, fileLine.length());
            tempArrayIndex++;
            lineLength = fileLine.length();
            i = 0;
        }
    }

    newEntry.id = stoi(tempInfoArray[0]);
    newEntry.loggedUserId = stoi(tempInfoArray[1]);
    newEntry.name = tempInfoArray[2];
    newEntry.surname = tempInfoArray[3];
    newEntry.phone = tempInfoArray[4];
    newEntry.email = tempInfoArray[5];
    newEntry.address = tempInfoArray[6];

    return newEntry;
}

void mainMenu() {
    system("cls");
    cout << "Address Book: \n";
    cout << "1. Add new contact\n";
    cout << "2. Search contacts by name\n";
    cout << "3. Search contacts by surname\n";
    cout << "4. Contacts list\n";
    cout << "5. Delete contact\n";
    cout << "6. Edit contact\n";
    cout << "7. Change password\n";
    cout << "8. Log out\n";
    cout << "9. Exit\n";
}

void AddressBook(int loggedUserId, LoginScreen& loginScreen) {
    vector<ContactListEntry> contactList;
    contactList = loadContactListFromFile(loggedUserId);
    int chosenOption;

    while (1) {
        mainMenu();             
        chosenOption = chooseOption(9);
        switch (chosenOption) {
        case 1:
        {
            int newId = getNewEntryId();
            contactList.push_back(addEntry(newId, loggedUserId));
            cin.ignore();
        } break;
        case 2:
        {
            searchByName(contactList);
            cin.ignore();
        } break;
        case 3:
        {
            searchBySurname(contactList);
            cin.ignore();
        } break;
        case 4:
        {
            wholeList(contactList);
            cin.ignore();
        } break;
        case 5:
        {
            deleteEntry(contactList);
            cin.ignore();
        } break;
        case 6:
        {
            editEntry(contactList);
        } break;
        case 7:
        {
            loginScreen.changeUserPassword(loggedUserId, loginScreen.getUsersList());
        } break;
        case 8:
        {
            return;
        } break;
        case 9: exit(0);
        }
    }
}

int getNewEntryId()
{
    vector<ContactListEntry> contactList;
    fstream file;
    file.open("contactlist.txt", ios::in);
    if (file.good() == 0) return 1;

    vector<int> listOfIds;
    string fileLine;

    while (getline(file, fileLine))
    {
        ContactListEntry tempEntry = splitFileLineToEntryVector(fileLine);
        listOfIds.push_back(tempEntry.id);
    }
    if (listOfIds.size() == 0) {
        return 1;
    }
    int newId = listOfIds[listOfIds.size() - 1] + 1;

    return newId;
}

ContactListEntry addEntry(int newId, int loggedUserId)
{
    ContactListEntry newContactListEntry;

    cout << endl << "Type in following informations\n";
    cout << "Name: ";
    getline(cin, newContactListEntry.name);
    cout << "Surname: ";
    getline(cin, newContactListEntry.surname);
    cout << "Phone number: ";
    getline(cin, newContactListEntry.phone);
    cout << "Email address: ";
    getline(cin, newContactListEntry.email);
    cout << "Address: ";
    getline(cin, newContactListEntry.address);
    cout << endl;
    cout << "New entry added. Press enter to return to main menu . . .";
    newContactListEntry.id = newId;
    newContactListEntry.loggedUserId = loggedUserId;
    
    saveEntryToFile(newContactListEntry);

    return newContactListEntry;
}

void saveEntryToFile(ContactListEntry newContactListEntry) {
    fstream file;
    file.open("contactlist.txt", ios::out | ios::app);

    file << mergeVectorToFileLine(newContactListEntry) << endl;

    file.close();
}

string mergeVectorToFileLine(ContactListEntry newEntry) {
    string tempInfoArray[7];
    string newFileLine;

    tempInfoArray[0] = to_string(newEntry.id);
    tempInfoArray[1] = to_string(newEntry.loggedUserId);
    tempInfoArray[2] = newEntry.name;
    tempInfoArray[3] = newEntry.surname;
    tempInfoArray[4] = newEntry.phone;
    tempInfoArray[5] = newEntry.email;
    tempInfoArray[6] = newEntry.address;

    for (int i = 0; i < 7; i++) {
        newFileLine = newFileLine + tempInfoArray[i] + "|";
    }

    return newFileLine;
}

void searchByName(vector<ContactListEntry> contactList) {
    string name;
    cout << endl;
    cout << "Type in name to search by: ";
    cin >> name;
    cout << endl;

    bool dontExist = true;
    int contactNumber = 0;
    for (int i = 0; i < contactList.size(); i++) {
        if (contactList[i].name == name) {
            contactNumber++;
            cout << contactNumber << ". Name: " << contactList[i].name << endl;
            cout << "   Surname: " << contactList[i].surname << endl;
            cout << "   Phone number: " << contactList[i].phone << endl;
            cout << "   Email address: " << contactList[i].email << endl;
            cout << "   Address: " << contactList[i].address << endl;
            dontExist = false;
        }
        if (i == contactList.size() - 1 && dontExist == true) {
            cout << "Therse is no contact with name: '" << name << "' in your contact list.\n";
        }
    }
    cout << endl;
    cout << "Press enter to return to main menu . . .";
    cin.ignore();
}

void searchBySurname(vector<ContactListEntry> contactList) {
    string surname;
    cout << endl;
    cout << "Type in name to search by: ";
    cin >> surname;
    cout << endl;

    bool dontExist = true;
    int contactNumber = 0;
    for (int i = 0; i < contactList.size(); i++) {
        if (contactList[i].surname == surname) {
            contactNumber++;
            cout << contactNumber << ". Name: " << contactList[i].name << endl;
            cout << "   Surname: " << contactList[i].surname << endl;
            cout << "   Phone number: " << contactList[i].phone << endl;
            cout << "   Email address: " << contactList[i].email << endl;
            cout << "   Address: " << contactList[i].address << endl;
            dontExist = false;
        }
        if (i == contactList.size() - 1 && dontExist == true) {
            cout << "Therse is no contact with surname: '" << surname << "' in your contact list.\n";
        }
    }
    cout << endl;
    cout << "Press enter to return to main menu . . .";
    cin.ignore();
}

void wholeList(vector<ContactListEntry> contactList) {
    int contactNumber = 0;
    for (int i = 0; i < contactList.size(); i++) {
        contactNumber++;
        cout << contactNumber << ". Name: " << contactList[i].name << endl;
        cout << "   Surname: " << contactList[i].surname << endl;
        cout << "   Phone number: " << contactList[i].phone << endl;
        cout << "   Email address: " << contactList[i].email << endl;
        cout << "   Address: " << contactList[i].address << endl;
    }

    cout << endl;
    cout << "Press enter to return to main menu . . .";
}

void deleteEntry(vector<ContactListEntry>& contactList) {
    int listSize = contactList.size();
    if (listSize == 0) {
        cout << endl;
        cout << "Contacts list is empty. Press enter to return to main menu . . .\n";
        return;
    }

    int entryId;
    cout << endl << "id: ";
    cin >> entryId;

    for (int i = 0; i < contactList.size(); i++) {
        if (contactList[i].id == entryId) {
            showContactToEditOrDelete(contactList[i]);
        }
    }

    int listIndexToDelete = 0;
    for (int i = 0; i < listSize; i++) {
        if (entryId == contactList[i].id) {
            listIndexToDelete = i;
            break;
        }
        else if (i + 1 == listSize && entryId != contactList[i].id) {
            listIndexToDelete = listSize;
        }
    }

    if (listIndexToDelete < listSize) {
        int confirmationKey;
        cout << endl;
        cout << "Are You sure You want to delete above contact?\n";
        cout << "Type 't' and press enter to confirm. Press enter to return to main menu . . .\n";
        cin.ignore();
        confirmationKey = getchar();
        if ((char)confirmationKey == 't') {
            contactList.erase(contactList.begin() + listIndexToDelete);
            deleteFromFile(entryId);
            cout << endl;
            cout << "Contact with id: " << entryId << " was removed from contact list. "
                << "Press enter to return to main menu . . .\n";
            cin.ignore();
        }
        else {
            cout << "No contact was deleted. Press enter to return to main menu . . .\n";
        }

    }
    else {
        cout << endl;
        cout << "Contact with this Id does not exist. Press enter return to main menu . . .\n";
        cin.ignore();
        return;
    }
}

void deleteFromFile(int entryId)
{
    fstream oldFile;
    fstream newFile;

    oldFile.open("contactlist.txt", ios::in);
    if (oldFile.good() == 0) {
        cout << "error message" << endl;
        return;
    }
    newFile.open("temp.txt", ios::out);

    string currentOldFileLine;

    while (getline(oldFile, currentOldFileLine))
    {
        ContactListEntry tempEntry = splitFileLineToEntryVector(currentOldFileLine);
        if (tempEntry.id != entryId)
        {
            newFile << currentOldFileLine << endl;
        }
    }

    oldFile.close();
    remove("contactlist.txt");

    newFile.close();
    bool renaming = rename("temp.txt", "contactlist.txt");
}

void showContactToEditOrDelete(ContactListEntry entryToShow) {
    cout << "Name: " << entryToShow.name << endl;
    cout << "Surname: " << entryToShow.surname << endl;
    cout << "Phone number: " << entryToShow.phone << endl;
    cout << "Email address: " << entryToShow.email << endl;
    cout << "Address: " << entryToShow.address << endl;
}

void editEntry(vector<ContactListEntry>& contactList) {
    int listSize = contactList.size();
    if (listSize == 0) {
        cout << endl;
        cout << "Contacts list is empty. Press enter to return to main menu . . .\n";
        cin.ignore();
        return;
    }

    int entryId;
    cout << endl << "Id: ";
    cin >> entryId;

    vector<int> listOfIds;
    int listIndexToEdit = -1;
    for (int i = 0; i < listSize; i++) {
        if (entryId == contactList[i].id) {
            listOfIds.push_back(contactList[i].id);
            listIndexToEdit = i;
        }
        else {
            listOfIds.push_back(contactList[i].id);
        }
    }

    if (listIndexToEdit == -1) {
        cout << endl;
        cout << "Contact with this Id does not exist. Press enter to return to main menu . . .\n";
        cin.get(); cin.get();
        return;
    }
    else {
        contactList[listIndexToEdit] = editGivenContact(contactList[listIndexToEdit]);
        editEntryInFile(contactList[listIndexToEdit]);
    }
}

ContactListEntry editGivenContact(ContactListEntry contactToEdit) {
    cin.ignore();

    int chosenOption = 0;
    while (chosenOption != 6)
    {
        system("cls");
        cout << "Id: " << contactToEdit.id << endl;
        showContactToEditOrDelete(contactToEdit);
        cout << endl;

        cout << "Chose information to edit:\n";
        cout << "1. Name\n";
        cout << "2. Surname\n";
        cout << "3. Phone number\n";
        cout << "4. Email address\n";
        cout << "5. Address\n";
        cout << "6. Return to main menu\n";

        chosenOption = chooseOption(6);
        if (chosenOption == 1) {
            cout << endl << "New name: ";
            getline(cin, contactToEdit.name);
        }
        else if (chosenOption == 2) {
            cout << endl << "New surname: ";
            getline(cin, contactToEdit.surname);
        }
        else if (chosenOption == 3) {
            cout << endl << "New phone number: ";
            getline(cin, contactToEdit.phone);
        }
        else if (chosenOption == 4) {
            cout << endl << "New email address: ";
            getline(cin, contactToEdit.email);
        }
        else if (chosenOption == 5) {
            cout << endl << "New address: ";
            getline(cin, contactToEdit.address);
        }
    }

    return contactToEdit;
}

void editEntryInFile(ContactListEntry contactToEdit) {
    fstream oldFile;
    fstream newFile;

    oldFile.open("contactlist.txt", ios::in);
    if (oldFile.good() == 0) {
        cout << "error message" << endl;
        return;
    }
    newFile.open("temp.txt", ios::out);

    string currentOldFileLine;

    while (getline(oldFile, currentOldFileLine))
    {
        ContactListEntry tempEntry = splitFileLineToEntryVector(currentOldFileLine);
        if (tempEntry.id == contactToEdit.id) {
            newFile << mergeVectorToFileLine(contactToEdit) << endl;
        }
        else {
            newFile << currentOldFileLine << endl;
        }
    }

    oldFile.close();
    remove("contactlist.txt");

    newFile.close();
    bool renaming = rename("temp.txt", "contactlist.txt");
}

int main() {
    int loggedUserId = -1;    

    while (loggedUserId != 0) {   
        LoginScreen loginScreen;
        loggedUserId = -1;
        loggedUserId = logIn(loginScreen);

        if (loggedUserId > 0) {
            AddressBook(loggedUserId, loginScreen);
        }        
    }   

    return 0;
}