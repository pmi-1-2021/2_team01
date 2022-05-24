#pragma once
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

vector<fs::path> getVectorOfFilesInAllDirectories(fs::path directory);
bool compareFiles(const fs::path& first_path, const fs::path & second_path);
set<fs::path> getSetOfCopies(fs::path path);
//1. Display a list of files in the selected directory.
void displayListOfFiles(fs::path path);
//2. Check if there is a file with the entered name.
bool isInDirectory(fs::path path, fs::path file_name);
//3. Display a list of files with the same content in the selected directory.
void listOfFilesWithTheSameContent(fs::path path);
//4. Find all copies in the selected directory and delete them.
void deleteAllCopies(fs::path path);
//5. Find all copies in the selected directory and place them in another directory.
void moveAllCopiesIntoAnotherDirectory(fs::path path);
/*6. Find all copies in the selected directory and place them in another directory,
so that all files will be in one copy only .*/
void moveCopiesIntoAnotherDirectory(fs::path path);
/*7. Delete extra copies in the specified directory
so that all files will be in one copy only.*/
void deleteExtraCopiesInDirectory(fs::path path);
//8. Delete the file in the specified path.
void deleteFile(fs::path path);
void printMenu();
