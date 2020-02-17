/************************************************************
 *                                                          *
 *  CSCI 480            Assignment 7          Fall   2019   *
 *                                                          *
 *  Programmer:  Dominykas Karalius - Z1809478              *
 *                                                          *
 *  Date Due:    11:59 PM on Monday, 12/02/2019             *
 *                                                          *
 *  Assign7.cc                                              *
 *                                                          *
 ***********************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

//Constants
#define BLOCKSIZE 512
#define HOWOFTEN  6
#define MAXENTRIES 12
#define MAXPRINT 240

using namespace std;

//Class definition for a file
class File
{
	string name;
	int size;
	vector<int> blocks;
};

int FAT[4096] = { }; //Array that will hold "files" in order
vector<File> files; //Vector that will hold information about file entries

//Function prototypes
void newFile(string name, int size);
void copyFile(string name, string name2);
void deleteFile(string name);
void modifyFile(string name, int size);
void renameFIle(string name, string name2);
void print();


int main(int argc, char *argv[])
{
	for(int i = 0; i < FAT.size(); i ++)
	{
		FAT[i] = 0;
	}

	//Open the file
	ifstream file;
	file.open("/home/turing/t90hch1/csci480/Assign7/data7.txt");

	if(file.fail())
	{
		cerr << endl << "Error opening the file.";
		exit(-1);
	}

	newFile(".", 512);
	newFile("..",0);

	string line;
	int count = 0;
	print();
	
	while(!file.eof()) 
	{	
		int size;     //Size of file
		string name;  //Name of first file
		string name2; //Name of second file
		char type;    //Type of transaction

		//Print info every HOWOFTEN times (6)
		if(count % HOWOFTEN == 0)
		{
			print();
		}
		
		//Get a line from file
		getline(file,line);
		istringstream token(line);
		
		//If transaction type is C
		if(line.at(0) == 'C') 
		{
			token >> type >> name >> name2;
			cout << "Transaction:  Copy a file" << endl;
			copyFile(name, name2);
		}

		//If transaction type is D
		else if(line.at(0) == 'D') 
		{	
			token >> type >> name;
			cout << "Transaction:  Delete a file" << endl;
			deleteFile(name);
		}

		//If transaction type is N
		else if(line.at(0) == 'N') 
		{
			token >> type >> name >> size;
			cout << "Transaction:  Add a new file" << endl;
			newFile(name, size);
		}

		
		//If transaction type is M
		else if(line.at(0) == 'M') 
		{
			token >> type >> name >> size;
			cout << "Transaction:  Modify a file" << endl;
			modifyFile(name, size);
		}

		//If transaction type is R
		else if(line.at(0) == 'R')
		{
			token >> type >> name >> name 2;
			cout << "Transaction:  Rename a file" << endl;
			renameFile(name, name2);
		}
		
		//If transaction type is ?
		else if(line.at(0) == '?')
		{
			break;
		}

		count++;
	}

	file.close();

	print();
	return 0;
}
/***************************************************************
newFile()

Use: 

Parameters: string name
	    int size

Returns: Nothing
***************************************************************/
void newFile(string name, int size)
{
	File temp; //temporary File object
	int i;
	int j = 1;
	
	//Check if file already exists
	for(int z = 0; z < files.size(); z++)
	{
		//If it does, print message, if not continue
		if(files.at(z).name == name)
		{
			cerr << "Error! File already exists can not create new file: "<<name<< "\n";
			return;
		}
	}

	temp.name = name;
	temp.size = size;

	if(size % BLOCKSIZE > 0)
	{
		i = size / BLOCKSIZE;
		i += 1;
	}
	else
	{
		i = size / BLOCKSIZE;
	}

	while(j <= i)
	{
		for(int k = 0; k < MAXSIZE; k++)
		{     
			if(FAT[k] == 0 && j == i)
			{
				FAT[k] = -1;
				temp.blocks.push_back(k);
				j++;
				break;
			}
			else if(FAT[k] == 0)
			{
				FAT[k] = 1;
				temp.blocks.push_back(k);
				j++;
				break;
			}
		}
	}
	
	files.push_back(temp);
	cout << "Successfuly added a new file, " << name << ", of size " << size << endl;
}
/***************************************************************
searchFile(string name)

Use: 

Parameters: string name


Returns: Nothing
***************************************************************/
int searchFile(string name)
{
	//Counter
	int count = 0;

	//Create iterator
        vector<File>::iterator it

	for(it = files.begin(); it != files.end(); ++it)
	{
		if(it->name == name)
		{
			return count;
		}
		count++;
	}
	return -1;
}
/***************************************************************
copyFile(string name, string name2)

Use: 

Parameters: string name
	    string name2

Returns: Nothing
***************************************************************/
void copyFile(string name, string name2)
{
	int existsCheck = searchFile(name); //int to represent if file is originally in FAT
	int dupeCheck = searchFile(name2); //int to represent if file already exists

	//File does not exist
	if(existsCheck < 0)
	{
		cout << "Error!  The old file name, " << name << ", was not found." << endl;
		return;
	}

	//New file name already exists
	if(dupeCheck > 0(
	{
		cout << "Error!  The new file name, " << name2 << ", already exists." << endl;
		return;
	}

	newFile(name2, files.at(source).size);
	cout << "Successfully copied an existing file, " << name << ", to a new file, "
		<< name2 << endl;

}
/***************************************************************
deleteFile(string name)

Use: 

Parameters: string name

Returns: Nothing
***************************************************************/
void deleteFile(string name)
{
	int existsCheck = searchFile(name); //Int to represent if file exists in FAT
	
	//File doesn't exist
	if(existsCheck < 0)
	{
		cout << "Error!  The file name, " << name << ", was not found." << endl;
		return;
	}

	//File exists
	else
	{
		for(int i = 0; i < files.at(existsCheck).blocks.size(); i++)
		{
			FAT[files.at(existsCheck).blocks.at(i)] = 0;
		}

		files.erase(files.begin() + existsCheck);

		cout << "Successfully deleted a file, " << name << endl;
	}
}
/***************************************************************
modifyFile(string name, int size)

Use: 

Parameters: string name
	    int size

Returns: Nothing
***************************************************************/
void modifyFile(string name, int size)
{
	int existsCheck = searchFile(name); //Int to represent if file exists in FAT

	//File doesn't exist
	if(existsCheck < 0)
	{
		cout << "Error!  The file name, " << name << ", was not found." << endl;
		return;
	}

	//File exists
	else
	{
		for(int i = 0; i < files.at(existsCheck).blocks.size(); i++)
		{
			FAT[files.at(existsCheck).blocks.at(i)] = 0;
		}

		files.erase(files.begin() + existsCheck);

		newFile(name,size);

		cout << "Successfully modified a file, " << name << endl;
	}
}
/***************************************************************
renameFile(string name, string name2)

Use: 

Parameters: string name
            string name2

Returns: Nothing
***************************************************************/
void renameFile(string name, string name2)
{
	int existsCheck = searchFile(name); //Int to represent if file exists in FAT
	int existsCheck2 = searchFile(name2); //Int to represent if desired name already exists in FAT

	//File doesn't exist
	if(existsCheck < 0)
	{
		cout << "Error!  The file name, " << name << ", was not found." << endl;
		return;
	}	
	//Desired name already exists
	if(existsCheck2 > 0)
	{
		cout << "Error!  The file name, " << name << ", is already in use." << endl;
		return;
	}
	//Else, rename
	else
	{
		files.at(existsCheck).name = name2;
		cout << "Successfully changed the file name << name << " to " << name2 << endl;
	}
}
/***************************************************************
print()

Use: Standard print method. Prints the status of the memory
blocks. Information such as; starting address, size, PID, and ID.
Also prints the total number of blocks in use, not in use, and total
sizes of those lists.

Parameters: None

Returns: Nothing
***************************************************************/
void print()
{
	int count = 0; //Counter for total number of files

	//Create block iterator
        vector<File>::iterator it;

	//Print header
	cerr << "Directory Listing" << endl;

	int j=0;
	int totalSize = 0;

	for(int i =0; i < files.size(); i++) 
	{
		cerr<<"File Name: " << setw(20) << left <<files.at(i).name;
		cerr <<"    File Size: "<<files.at(i).size<< "\n";
		count += files.at(i).size;
		cerr<<"Cluster(s) in use:";
		for(int j=0; j < files.at(i).blocks.size(); j++) 
		{
			if(j%12==0&&j!=0)
			{
			cerr<<"\n                  ";
			}
			cerr<< setw(6) << right <<files.at(i).blocks.at(j);
		}
		if( files.at(i).blocks.size() == 0) 
		{
			cerr << "(none)";
		}
		cerr<< "\n\n";
	}

	cerr<<"Files: "<<files.size()<<"\t"<<"Total Size: "<<totalSize<< "\n\n";

	for(int i =0; i < 240; i++)
	{
		if(j % 12 == 0)
		{
			cerr<< "#" <<setw(3)<<setfill('0') << i <<" - "<<setw(3)<<setfill('0')<<i+11<<": "<<"\t"<< setfill(' ');
		}
		cerr<<right<<setw(2)<<FAT.at(i)<<"\t";
		j++;
		if(j % 12 == 0)
		{
			cerr<< "\n";
			j=0;
		}
	}
	cerr<< "\n";
}
