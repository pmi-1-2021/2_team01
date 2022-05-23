#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "Windows.h"
#define UKR 1251


using namespace std;
namespace fs = std::filesystem;

bool compareFiles(const std::string& p1, const std::string& p2);
set<string> getSetOfCopies(string path);
//1. Display a list of files in the selected directory.
void displayListOfFiles(string path);
//2. Check if there is a file with the entered name.
bool isInDirectory(string path, fs::path my_path);
//3. Display a list of files with the same content in the selected directory.
void listOfFilesWithTheSameContent(string path);
//4. Find all copies in the selected directory and delete them.
void deleteAllCopies(string path);
//5. Find all copies in the selected directory and place them in another directory.
void moveAllCopiesIntoAnotherDirectory(string path);
/*6. Find all copies in the selected directory and place them in another directory,
so that all files will be in one copy only .*/
void moveCopiesIntoAnotherDirectory(string path);
/*7. Delete extra copies in the specified directory 
so that all files will be in one copy only.*/
void deleteExtraCopiesInDirectory(string path);
//8. Delete the file in the specified path.
void deleteFile(string path);
void printMenu();

void main()
{
	SetConsoleCP(UKR);
	SetConsoleOutputCP(UKR);

	printMenu();
}

bool compareFiles(const std::string& p1, const std::string& p2) {
	std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
	std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

	if (f1.fail() || f2.fail()) {
		return false; //file problem
	}

	if (f1.tellg() != f2.tellg()) {
		return false; //size mismatch
	}

	//seek back to beginning and use std::equal to compare contents
	f1.seekg(0, std::ifstream::beg);
	f2.seekg(0, std::ifstream::beg);
	return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
		std::istreambuf_iterator<char>(),
		std::istreambuf_iterator<char>(f2.rdbuf()));
}

set<string> getSetOfCopies(string path)
{
	vector<string> my_vector;
	for (const auto& entry : fs::directory_iterator(path))
	{
		fs::path path = entry.path();
		string path_string{ path.u8string() };
		my_vector.push_back(path_string);
	}
	set<string> my_set;
	for (size_t i = 0; i < my_vector.size() - 1; i++)
	{
		size_t j;
		for (j = 1; j < my_vector.size(); j++)
		{
			if (compareFiles(my_vector[i], my_vector[j]) && i != j)
			{
				my_set.insert(my_vector[i]);
				my_set.insert(my_vector[j]);
			}
		}
	}

	return my_set;
}

void displayListOfFiles(string path)
{
	cout << "\n*********** List of files ***********\n";
	for (const auto& entry : fs::directory_iterator(path))
		std::cout << entry.path()<< std::endl;
}

bool isInDirectory(string path, fs::path my_path)
{
	vector<string> vector;
	for (const auto& entry : fs::directory_iterator(path))
	{
		fs::path path = entry.path();
		string path_string{ path.u8string() };
		vector.push_back(path_string);
	}
	string path_string{ my_path.u8string() };
	if (std::find(vector.begin(), vector.end(), path_string) != vector.end())
		return true;
	else return false;
}

void listOfFilesWithTheSameContent(string path)
{
	set<string> my_set = getSetOfCopies(path);
	for (auto it = my_set.begin(); it !=my_set.end(); ++it)
		cout << *it<<endl;
}

void deleteAllCopies(string path)
{
	set<string> my_set = getSetOfCopies(path);
	for (auto it = my_set.begin(); it != my_set.end(); ++it)
	{
		string temp_file_path = *it;
		char* writable = new char[temp_file_path.size() + 1 ];
		std::copy(temp_file_path.begin(), temp_file_path.end(), writable);
		writable[temp_file_path.size()] = '\0';
		int result = remove(writable);
	}
	cout << "\nAll copies deleted successfully! :)";
}

void moveAllCopiesIntoAnotherDirectory(string path)
{
	cout << "\nEnter the directory where you want to move the copies of the files: ";
	string another_directory;
	cin.ignore();
	getline(cin, another_directory);

	set<string> my_set = getSetOfCopies(path);

	for (auto it = my_set.begin(); it != my_set.end(); ++it)
	{
		fs::path name = *it;
		fs::path my_old_path = path / name;//шлях до файлу, який тре перемістити

		fs::path my_new_path = another_directory / my_old_path.filename();//шлях до файлу, який перемістили

		ofstream file(my_new_path);

		copy_file(my_old_path, my_new_path, fs::copy_options::overwrite_existing);
	}
	deleteAllCopies(path);

	cout << "\nAll copies moved successfully to "<< another_directory << " \nfrom " << path << "! :)";
}

void moveCopiesIntoAnotherDirectory(string path) 
{
	cout << "\nEnter the directory where you want to move the copies of the files: ";
	string another_directory;
	cin.ignore();
	getline(cin, another_directory);


	set<string> my_set = getSetOfCopies(path);

	set<string> my_result_set;

	for (auto i = my_set.begin(); i != my_set.end(); ++i)
	{
		size_t j;
		for (auto it = i; it != my_set.end(); ++it)
		{
			if (compareFiles(*i, *it) && i != it)
			{
				my_result_set.insert(*it);
			}
		}
	}
	for (auto it = my_result_set.begin(); it != my_result_set.end(); ++it)
	{
		fs::path name = *it;
		fs::path my_old_path = path / name;//шлях до файлу, який тре перемістити

		fs::path my_new_path = another_directory / my_old_path.filename();//шлях до файлу, який перемістили

		ofstream file(my_new_path);

		copy_file(my_old_path, my_new_path, fs::copy_options::overwrite_existing);
	}
	deleteExtraCopiesInDirectory(path);

	cout << "\nAll copies moved successfully to " << another_directory << " \nfrom " << path << "! :)";
}

void deleteExtraCopiesInDirectory(string path)
{
	set<string> my_set = getSetOfCopies(path);

	set<string> my_result_set;

	for (auto i = my_set.begin(); i != my_set.end(); ++i)
	{
		size_t j;
		for (auto it = i; it != my_set.end(); ++it)
		{
			if (compareFiles(*i, *it) && i != it)
			{
				my_result_set.insert(*it);
			}
		}
	}
	for (auto itt = my_result_set.begin(); itt != my_result_set.end(); ++itt)
	{
		string temp_file_path = *itt;
		char* writable = new char[temp_file_path.size() + 1];
		std::copy(temp_file_path.begin(), temp_file_path.end(), writable);
		writable[temp_file_path.size()] = '\0';
		int result = remove(writable);
	}
}

void deleteFile(string path)
{
	cout << "\nEnter name of file, you want to delete: ";
	fs::path file_name;
	cin >> file_name;
	fs::path my_path = path / file_name;
	if (isInDirectory(path, my_path))
	{
		remove(my_path);
		cout << "\nFile removed successfully! :)";
	}
	else
		cout << "\nThis file is not in the directory :(\n";
}

void printMenu()
{
	cout << "Enter path to directory: ";
	string path = "";
	getline(cin, path);

	menuAgain:
	cout << "\nHere is the MENU: " << endl;
	cout << "- - - - - - - - - - -" << endl;
	cout << "1. Display a list of files in the selected directory. \n";
	cout << "2. Check if there is a file with the entered name. \n";
	cout << "3. Display a list of files with the same content in the selected directory. \n";
	cout << "4. Find all copies in the selected directory and delete them. \n";
	cout << "5. Find all copies in the selected directory and place them in another directory. \n";
	cout << "6. Find all copies in the selected directory and place them in another directory, \nso that all files will be in one copy only. \n";
	cout << "7. Delete extra copies in the selected directory so that all files will be in one copy only. \n";
	cout << "8. Delete the file in the specified path. \n";
	cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;

option:
	cout << "Enter number of option that you want to call: ";
	size_t option;
	cin >> option;

	switch (option)
	{
		case 1:
		{
			displayListOfFiles(path);
			break;
		}
		case 2:
		{
			cout << "\nEnter the file name: ";
			fs::path file_name;
			cin >> file_name;
			fs::path my_path = path / file_name;
			if (isInDirectory(path, my_path))
				cout << "Yay!This file is in the directory:)\n\n";
			else
				cout << "This file is not in the directory:( \n\n";
			break;
		}
		case 3:
		{
			listOfFilesWithTheSameContent(path);
			break;
		}
		case 4:
		{
			deleteAllCopies(path);
			break;
		}
		case 5:
		{
			moveAllCopiesIntoAnotherDirectory(path);
			break;
		}
		case 6:
		{
			moveCopiesIntoAnotherDirectory(path);
			break;
		}
		case 7:
		{
			deleteExtraCopiesInDirectory(path);
			cout << "\nAll copies deleted successfully! :)";
			break;
		}
		case 8:
		{
			deleteFile(path);
			break;
		}
		default:
		{
			cout << "Try again. You did not enter 1-6 number..";
			goto option;
		}
	}
	tryAgain:
	cout << "\nIf you want to see the menu again, enter 1. Else enter 0: ";
	size_t want_to_see_the_menu;
	cin >> want_to_see_the_menu;
	if (want_to_see_the_menu==1)
		goto menuAgain;
	else if (want_to_see_the_menu != 0)
	{
		cout << "\nYou did not enter 0 or 1. Try again\n";
		goto tryAgain;
	}
}


//C:\Users\1\OneDrive - lnu.edu.ua\Documents\Test1
//C:\Users\1\OneDrive - lnu.edu.ua\Documents\Test
//C:\Users\1\OneDrive - lnu.edu.ua\Documents\To