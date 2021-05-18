#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "User.h"

using namespace std;

class LoginScreen {

	vector<User> usersList;

public:
	LoginScreen();
	vector<User> loadUsersListFromFile();
	int signIn(vector<User>& usersList);
	bool checkUsername(string username, vector<User>& usersList);
	bool checkPassword(string username, string password,int& loggedUserId, vector<User>& usersList);
	void signUp(vector<User>& usersList);
	int getNewUserId();
	User splitFileLineToUserVector(string fileLine);
	void saveUserToFile(User newUser);	
	string mergeUserToFileLine(User newUser);
	int exit();
};

