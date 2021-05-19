#include "LoginScreen.h"

LoginScreen::LoginScreen() {
    usersList = loadUsersListFromFile();
    
    system("cls");
    cout << "login Screen: \n";
    cout << "1. Sign in\n";
    cout << "2. Sign up\n";
    cout << "3. Exit\n";
}

vector<User> LoginScreen::loadUsersListFromFile()
{
    vector<User> usersList;
    fstream file;
    string fileLine;

    file.open("usersList.txt", ios::in);
    if (file.good() == 0) return usersList;

    while (getline(file, fileLine)) {
        usersList.push_back(splitFileLineToUserVector(fileLine));
    }

    file.close();
    return usersList;
}

int LoginScreen::signIn(vector<User>& usersList) {    
    string username = "";
    bool doesUserExists = false;
    string password = "";
    bool isPasswordCorrect = false;
    int loggedUserId = 0;

    while (doesUserExists == false) {
        cout << "Username: ";
        getline(cin, username);
        doesUserExists = checkUsername(username,usersList);
        if (username == "0") return -1;
    }    

    while (isPasswordCorrect == false) {
        cout << "Password: ";
        getline(cin, password);
        isPasswordCorrect = checkPassword(username, password, loggedUserId, usersList);
        if (password == "0") return -1;
    }    
    
    return loggedUserId;
}

bool LoginScreen::checkUsername(string username, vector<User>& usersList) {
    int listSize = usersList.size();

    for (int i = 0; i < listSize; i++) {
        if (username != usersList[i].name && i == listSize - 1) {
            cout << "User with that name does not exist, try again. Enter username: 0 to abort.\n";
            return false;
        }
        else if (username == usersList[i].name) {
            return true;
        }        
    }
}

bool LoginScreen::checkPassword(string username,string password,int& loggedUserId, vector<User>& usersList) {
    int listSize = usersList.size();

    for (int i = 0; i < listSize; i++) {
        if (username == usersList[i].name && password == usersList[i].password) {
            cout << endl << "Correct username nad password. User is now logged in." << endl;
            cout << "Press enter to continue . . .";
            loggedUserId = usersList[i].id;
            cin.get();            

            return true;
        }
        else if (i == listSize - 1) {
            cout << "Incorrect password, try again. Enter password: 0 to abort." << endl;
            return false;
        }        
    }    
}

void LoginScreen::signUp(vector<User>& usersList) {
    User newUser;
    bool doesUserExist = false;
    
    do {
        cout << "New users name: ";
        getline(cin, newUser.name);
        doesUserExist = checkIfUsernameExists(newUser.name, usersList);
    } while (doesUserExist == true);

    
    cout << "New users password: ";
    getline(cin, newUser.password);

    cout << endl << "New user '" << newUser.name << "' added succsefully. Press enter to return to login screen . . .";
    cin.get();

    newUser.id = getNewUserId();
    saveUserToFile(newUser);
    usersList.push_back(newUser);
}

bool LoginScreen::checkIfUsernameExists(string newUsername, vector<User>& usersList) {
    int listSize = usersList.size();

    for (int i = 0; i < listSize; i++) {
        if (newUsername == usersList[i].name) {
            cout << "User with that name already exists, try again. Enter username: 0 to abort.\n";
            return true;
        }        
    }
    return false;
}

int LoginScreen::getNewUserId()
{
    vector<User> usersList;
    fstream file;
    file.open("usersList.txt", ios::in);
    if (file.good() == 0) return 1;

    vector<int> listOfIds;
    string fileLine;

    while (getline(file, fileLine))
    {
        User tempEntry = splitFileLineToUserVector(fileLine);
        listOfIds.push_back(tempEntry.id);
    }
    if (listOfIds.size() == 0) {
        return 1;
    }
    int newId = listOfIds[listOfIds.size() - 1] + 1;

    return newId;
}

User LoginScreen::splitFileLineToUserVector(string fileLine) {
    int lineLength = fileLine.length();
    string tempInfoArray[3];
    int tempArrayIndex = 0;
    User newUser;

    for (int i = 0; i < lineLength; i++) {
        if (fileLine[i] == 124) {
            tempInfoArray[tempArrayIndex] = fileLine.substr(0, i);
            fileLine = fileLine.substr(i + 1, fileLine.length());
            tempArrayIndex++;
            lineLength = fileLine.length();
            i = 0;
        }
    }

    newUser.id = stoi(tempInfoArray[0]);
    newUser.name = tempInfoArray[1];
    newUser.password = tempInfoArray[2];

    return newUser;
}

void LoginScreen::saveUserToFile(User newUser) {
    fstream file;
    file.open("usersList.txt", ios::out | ios::app);

    file << mergeUserToFileLine(newUser) << endl;

    file.close();
}

string LoginScreen::mergeUserToFileLine(User newUser) {
    string tempInfoArray[3];
    string newFileLine;

    tempInfoArray[0] = to_string(newUser.id);
    tempInfoArray[1] = newUser.name;
    tempInfoArray[2] = newUser.password;

    for (int i = 0; i < 3; i++) {
        newFileLine = newFileLine + tempInfoArray[i] + "|";
    }

    return newFileLine;
}

vector<User>& LoginScreen::getUsersList()
{
    return usersList;
}

void LoginScreen::changeUserPassword(int loggedUserId, vector<User>& usersList) {
    int usersListSize = usersList.size();
    int currentUserNumberInList = -1;
    for (int i = 0; i < usersListSize; i++) {
        if (usersList[i].id == loggedUserId) {
            currentUserNumberInList = i;
            break;
        }
    }

    string newPassword;
    cout << endl << "New password: ";
    getline(cin, newPassword);
    if (currentUserNumberInList >= 0 && currentUserNumberInList < usersListSize) {
        if (usersList[currentUserNumberInList].password == newPassword) {
            cout << endl << "Same password as current, no changes made. Press enter to return to main menu . . .\n";
            cin.get();
        }
        else {
            usersList[currentUserNumberInList].password = newPassword;
            editUserInFile(usersList[currentUserNumberInList]);
            cout << endl << "Password changed successfully. Press enter to return to main menu . . .\n";
            cin.get();
        }
    }
    else if (currentUserNumberInList == -1) {
        cout << endl << "Something went wrong. Press enter to return to main menu . . .\n";
        cin.get();
    }
}

void LoginScreen::editUserInFile(User userToEdit) {
    fstream oldFile;
    fstream newFile;

    oldFile.open("usersList.txt", ios::in);
    if (oldFile.good() == 0) {
        cout << "error message" << endl;
        return;
    }
    newFile.open("temp.txt", ios::out);

    string currentOldFileLine;
    User tempUser;

    while (getline(oldFile, currentOldFileLine))
    {
        tempUser = splitFileLineToUserVector(currentOldFileLine);
        if (tempUser.id == userToEdit.id) {
            newFile << mergeUserToFileLine(userToEdit) << endl;
        }
        else {
            newFile << currentOldFileLine << endl;
        }
    }

    oldFile.close();
    remove("userslist.txt");

    newFile.close();
    bool renaming = rename("temp.txt", "usersList.txt");
}

int LoginScreen::exit() {
    return 0;
}
