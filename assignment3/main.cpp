#include<bits/stdc++.h> 
using namespace std; 

void writeToFile(vector<string> result)
{
	ofstream outputFile ("EmployeeIndex.txt");
	int i = 0;
	if (outputFile.is_open()){
		while(i != result.size()){
			outputFile << result.at(i) << ", ";
			outputFile << result.at(i+1) << ", ";
			outputFile << result.at(i+2) << ", ";
			outputFile << result.at(i+3) << endl;

			i += 4;
		}
		outputFile.close();
	}
}

void getNextLineAndHash(istream& str)
{
	string cell, line;
	vector<string> result;

	// This iterates through the whole file
	while(getline(str, line)){
		stringstream lineStream(line);

		while(getline(lineStream,cell, ','))
		{
			result.push_back(cell);
		}
	}
	// **Still need to actually hash and make the bucket array** //

	writeToFile(result);
}

// This will read from CSV file and create hash
void createHashIndex()
{
	vector<string> result;
	ifstream file ("Employees.csv");
	if (file.is_open()){
		getNextLineAndHash(file);

		file.close();
	}
}

void getHashIndex(string lookupId)
{
	// Hash id to get array index

	// This reads the output file and finds the info from the id
	ifstream readFile ("EmployeeIndex.txt");
	string data;

	while(!readFile.eof()){
		getline(readFile, data);
		if(data.rfind(lookupId, 8) != string::npos)
			cout << data << endl;
	}
}

//Driver method to test map class 
int main() 
{ 
	//User input for Creation or Lookup
	while(1){
		string input, split;
		cout << "Enter a command (C or L) with the id if necessary: ";
		getline(cin, input);
		stringstream splitter(input);
		while (getline(splitter, split, ' ')){
			if (split == "C"){
				cout << "Creating Hash Index" << endl;
				createHashIndex();
				break;
				}
			else if (split == "L"){
				getline(splitter, split, ' ');
				cout << "Looking up " << split << endl;
				getHashIndex(split);
			}
			else {
				cout << "Incorrect Inputs" << endl;
			}
		}
	}
	return 0; 
} 
