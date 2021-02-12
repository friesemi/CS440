#include <bits/stdc++.h>
#include <bitset>

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

int hashAndGetBits(int id, int numBits)
{
	// Convert to binary
	//cout << id << endl;
	string binary = bitset<32>(id).to_string();
	//cout << binary << endl;

	// Grab n least significant bits
	string keyBin = binary.substr(binary.length() - numBits);
	//cout << keyBin << endl;

	int key = bitset<32>(keyBin).to_ulong();
  //cout << key << endl;

	return key;
}

void getNextLineAndHash(istream& str)
{
	string cell, line;
	vector<string> result;
	int id, key, i = 0, numBits = 1, numBuckets = 1;

	//Write initial numBuckets and numBits to beginning of output file

	// This iterates through the whole file line by line
	while(getline(str, line)){
		stringstream lineStream(line);

		// Get the data for current line
		while(getline(lineStream,cell, ','))
		{
			result.push_back(cell);
		}

		// Use the id to get the key for which bucket it should be placed in
		id = stoi(result.at(i));
		key = hashAndGetBits(id, numBits);

		// Find memory address with key from bucket array (DO BUCKET ARRAY AT END)
		// getMemAddr(key, bucketArray)

		store(key, numBuckets, result);

		//checkForCapacity()
			//increase capacity if needed (increase numBuckets)
			//increase numBits if needed
			//write the numBuckets and numBits to beginning of output file
			//allocate new bucket
			//Check bucket that is the bit flip of new bucket. Check all records and move if needed

		i += 4;
	}
	//writeToFile(result);
}

void store(int key, int numBuckets, vector<string> result) {
	// Check if bit flip is needed
	if (key > numBuckets) {
		//bitFlip(key)
	}

	// Pull line *key + 2* from file

	// Append result to end of line

	// Write line back to where you pulled it from
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
