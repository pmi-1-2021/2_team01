#include "Header.h"

vector<fs::path> getVectorOfFilesInAllDirectories(fs::path path)
{
	vector<fs::path> vector_of_files;
	for (fs::recursive_directory_iterator i(path), end; i != end; ++i)
		if (!is_directory(i->path()))
		{
			vector_of_files.push_back(i->path());
		}
	return vector_of_files;
}

vector<fs::path> getVectorOfFileNames(fs::path path)
{
	vector<fs::path> vector_of_files = getVectorOfFilesInAllDirectories(path);
	vector<fs::path> vector_of_filenames;

	for (vector<fs::path>::const_iterator i = vector_of_files.begin(); i != vector_of_files.end(); ++i)
		vector_of_filenames.push_back(i->filename());
	return vector_of_filenames;
}

bool compareFiles(const fs::path& first_path, const fs::path& second_path) {
	std::ifstream first_file(first_path, std::ifstream::binary | std::ifstream::ate);
	std::ifstream second_file(second_path, std::ifstream::binary | std::ifstream::ate);

	if (first_file.fail() || second_file.fail()) {
		return false; //file problem
	}

	if (first_file.tellg() != second_file.tellg()) {
		return false; //size mismatch
	}

	//seek back to beginning and use std::equal to compare contents
	first_file.seekg(0, std::ifstream::beg);
	second_file.seekg(0, std::ifstream::beg);
	return std::equal(std::istreambuf_iterator<char>(first_file.rdbuf()),
		std::istreambuf_iterator<char>(),
		std::istreambuf_iterator<char>(second_file.rdbuf()));
}

set<fs::path> getSetOfCopies(fs::path path)
{
	vector<fs::path> my_vector = getVectorOfFilesInAllDirectories(path);
	set<fs::path> my_set;
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

void displayListOfFiles(fs::path path)
{
	cout << "\n*********** List of files ***********\n";
	vector<fs::path> vector_of_files=getVectorOfFilesInAllDirectories(path);
	for (vector<fs::path>::const_iterator i = vector_of_files.begin(); i != vector_of_files.end(); ++i)
		cout << *i << endl;
}

bool isInDirectory(fs::path path, fs::path file_name)
{
	vector<fs::path> vector_of_files = getVectorOfFilesInAllDirectories(path);
	vector<fs::path> vector_of_filenames = getVectorOfFileNames(path);

	return std::find(vector_of_filenames.begin(), vector_of_filenames.end(), file_name) != vector_of_filenames.end();
}

void listOfFilesWithTheSameContent(fs::path path)
{
	set<fs::path> my_set = getSetOfCopies(path);
	for (auto it = my_set.begin(); it !=my_set.end(); ++it)
		cout << *it<<endl;
}

void deleteAllCopies(fs::path path)
{
	set<fs::path> my_set = getSetOfCopies(path);
	for (auto it = my_set.begin(); it != my_set.end(); ++it)
	{
		fs::path temp_file_path = *it;
		remove(temp_file_path);
	}
}

void moveAllCopiesIntoAnotherDirectory(fs::path path)
{
	cout << "\nEnter the directory where you want to move the copies of the files: ";
	string another_directory;
	cin.ignore();
	getline(cin, another_directory);

	set<fs::path> my_set = getSetOfCopies(path);

	for (auto it = my_set.begin(); it != my_set.end(); ++it)
	{
		fs::path name = *it;
		fs::path my_old_path = path / name;//шлях до файлу, який тре перемістити

		fs::path my_new_path = another_directory / my_old_path.filename();//шлях до файлу, який перемістили

		ofstream file(my_new_path);

		copy_file(my_old_path, my_new_path, fs::copy_options::overwrite_existing);
	}
	deleteAllCopies(path);

	cout << "\nAll copies moved successfully to "<< another_directory << " \nfrom " << path << "! :) "<< endl;
}

void moveCopiesIntoAnotherDirectory(fs::path path)
{
	cout << "\nEnter the directory where you want to move the copies of the files: ";
	string another_directory;
	cin.ignore();
	getline(cin, another_directory);


	set<fs::path> my_set = getSetOfCopies(path);

	set<fs::path> my_result_set;

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

	cout << "\nAll copies moved successfully to " << another_directory << " \nfrom " << path << "! :) "<<endl<<endl;
}

void deleteExtraCopiesInDirectory(fs::path path)
{
	set<fs::path> my_set = getSetOfCopies(path);

	set<fs::path> my_result_set;

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
		fs::path temp_file_path = *itt;
		remove(temp_file_path);
	}
}

void deleteFile(fs::path path)
{
	cout << "\nEnter name of file, you want to delete: ";
	fs::path file_name;
	cin >> file_name;
	vector<fs::path> vector_of_files = getVectorOfFilesInAllDirectories(path);

	for (vector<fs::path>::const_iterator i = vector_of_files.begin(); i != vector_of_files.end(); ++i)
		if (i->filename() == file_name)
		{
			remove(*i);
			cout << "\nFile removed successfully! :)";
			return;
		}

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
			if (isInDirectory(path, file_name))
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
			cout << "\nAll copies deleted successfully! :)";
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
