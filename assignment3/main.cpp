#include <bits/stdc++.h>
#include <bitset>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

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

void writeBitsBuckets(int numBits, int numBuckets) {
	ofstream outputFile ("BitsandBuckets.txt");

	if (outputFile.is_open()) {
		outputFile << numBits << endl;
		outputFile << numBuckets << endl;

		outputFile.close();
	}
}

void writeBucketArray(vector<long> bucketArray) {
	ofstream outputFile ("BucketArray.txt");

	if (outputFile.is_open()) {
		for (int i = 0; i < bucketArray.size(); i++) {
			outputFile << bucketArray.at(i) << endl;
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

	// Convert to decimal
	int key = bitset<32>(keyBin).to_ulong();
  //cout << key << endl;

	return key;
}

vector<long> updateBucketArray(vector<long> bucketArray, int numBuckets) {
	fstream myFile("EmployeeIndex.txt");
	string dummy;
	int runningLength = 0;

	myFile.seekg(0, ios::beg);
	for (int i = 1; i < numBuckets; i++) {
		getline(myFile, dummy);
		//cout << bucketArray.at(i) << endl;
		//cout << "Dummy:" << dummy << endl;
		//runningLength += (dummy.length() + 2);
		bucketArray.at(i) = myFile.tellg();
		//cout << myFile.tellg() << endl;
		//cout << runningLength << endl;
	}

	myFile.close();

	return bucketArray;
}

int bitFlip(int key) {
	string binary = bitset<32>(key).to_string();
	for (int i = 0; i < binary.length(); i++) {
		if (binary[i] == '1') {
			binary[i] = '0';
			break;
		}
	}
	key = bitset<32>(binary).to_ulong();

	return key;
}

vector<long> store(int key, int numBuckets, vector<string> result, vector<long> bucketArray) {
	string bucket = "", currLine;
	fstream myFile("EmployeeIndex.txt");
	long pos;

	// Check if bit flip is needed
	cout << "Key before flip: " << key << endl;
	if (key+1 > numBuckets) {
		key = bitFlip(key);
	}
	cout << "Key after flip: " << key << endl;


	// Get position of bucket in file
	pos = bucketArray.at(key);
	//cout << pos << endl;

	if (myFile.is_open()) {
		// Read in bucket
		myFile.seekg(pos);
		getline(myFile, bucket);
		cout << bucket << endl;

		// Append result to end of bucket
		for (int i = 0; i < 4; i++) {
			bucket.append(result.at(i));
			bucket.append(" ");
		}
		bucket.append("; ");
		cout << bucket << endl;

		//cout << bucket << endl;

		// Write bucket back to where you pulled it from
		ofstream makeFile("temp.txt");
		makeFile.close();
		fstream tempFile("temp.txt");

		if (tempFile.is_open()) {
			int lineNum = 0;
			myFile.seekg(0, ios::beg);

			// Copy unchanged buckets over
			for (int i = 0; i < key; i++) {
				getline(myFile, currLine);
				cout << "Unchanged buckets:" << currLine << endl;
				tempFile << currLine << endl;
				lineNum++;
			}

			// Copy in changed bucket
			cout << "changed bucket:" << bucket << endl;
			tempFile << bucket;
			// If not putting in last bucket, tack endl back on
			if (key + 1 != numBuckets) {
				tempFile << endl;
			}
			lineNum++;

			// Throw out old version of changed bucket
			getline(myFile, currLine);

			// Copy in rest of file
			while (getline(myFile, currLine)) {
				cout << "rest of file:" << currLine << endl;
				tempFile << currLine;
				lineNum++;
				if (lineNum != numBuckets) {
					cout << "NEW LKINE" << endl;
					tempFile << endl;
				}
			}

			tempFile.close();
		}
		myFile.close();

		// Remove old file
		remove("EmployeeIndex.txt");

		// Rename new file
		rename("temp.txt", "EmployeeIndex.txt");

		// Update bucket array
		bucketArray = updateBucketArray(bucketArray, numBuckets);
	}

	return bucketArray;
}

vector<long> initIndex(vector<long> bucketArray) {
	// Make file
	ofstream makeFile("EmployeeIndex.txt");
	makeFile.close();

	fstream myFile("EmployeeIndex.txt");

	if (myFile.is_open()) {
		myFile.seekg(0, ios::beg);
		bucketArray.push_back(myFile.tellg());
		myFile << " ";

		myFile.close();
	}

	return bucketArray;
}

vector<long> resort(int numBits, int numBuckets, vector<long> bucketArray) {
	string bucket = "", currRecord, currLine;
	fstream myFile("EmployeeIndex.txt");
	long pos;
	int bitFlipBucketKey, newBucketKey, currId, currKey;
	vector<string> records, moveRecords, keepRecords;

	// Figure out which bucket we need to check for possible movable records
	newBucketKey = numBuckets-1;
	bitFlipBucketKey = bitFlip(newBucketKey);

	cout << "bucket key to check: " <<bitFlipBucketKey << endl;

	// Get position of bucket in file
	pos = bucketArray.at(bitFlipBucketKey);
	//cout << pos << endl;

	if (myFile.is_open()) {
		// Read in bucket
		myFile.seekg(pos);
		getline(myFile, bucket);

		cout << bucket << endl;

		// Separate records
		while (bucket != " ") {
			records.push_back(bucket.substr(0, bucket.find(';')));
			bucket.erase(0, (bucket.find(';')+1));
		}


		// Go through all the records and seperate the ones that need to be moved from the ones that don't
		for (int i = 0; i < records.size(); i++) {
			// Extract id
			currRecord = records.at(i);
			currId = stoi(currRecord.substr(1, currRecord.find(" ", 1)-1));

			// Get the key for this record
			currKey = hashAndGetBits(currId, numBits);

			// Check if record should be moved
			if (currKey == (numBuckets - 1)) {
				moveRecords.push_back(currRecord);
			}
			else {
				keepRecords.push_back(currRecord);
			}
		}

		// Rewrite time
		ofstream makeFile("temp.txt");
		makeFile.close();
		fstream tempFile("temp.txt");

		if (tempFile.is_open()) {
			int i = 0;
			myFile.seekg(0, ios::beg);

			// Copy unchanged buckets over
			for (i; i < bitFlipBucketKey; i++) {
				getline(myFile, currLine);
				cout << "Unchanged buckets:" << currLine << endl;
				tempFile << currLine << endl;
			}

			// Rebuild the bucket for the records that are being kept
			bucket = "";
			for (int j = 0; j < keepRecords.size(); j++) {
				bucket.append(keepRecords.at(j));
				bucket.append(";");
			}
			bucket.append(" ");

			// Copy in changed bucket with kept files
			cout << "changed bucket:" << bucket << endl;
			tempFile << bucket <<endl;
			i++;

			// Throw out old version of changed bucket
			getline(myFile, currLine);

			// Copy unchanged buckets over
			for (i; i < newBucketKey; i++) {
				getline(myFile, currLine);
				cout << "Unchanged buckets:" << currLine << endl;
				tempFile << currLine << endl;
			}

			// Rebuild the bucket for the records that are being moved
			bucket = "";
			for (int j = 0; j < moveRecords.size(); j++) {
				bucket.append(moveRecords.at(j));
				bucket.append(";");
			}
			bucket.append(" ");

			// Copy in changed bucket with moved files
			cout << "changed bucket:" << bucket << endl;
			tempFile << bucket;

			tempFile.close();
		}

		myFile.close();

		// Remove old file
		remove("EmployeeIndex.txt");

		// Rename new file
		rename("temp.txt", "EmployeeIndex.txt");

		// Update bucket array
		bucketArray = updateBucketArray(bucketArray, numBuckets);
	}

	return bucketArray;
}

vector<long> checkForCapacity(int* numBuckets, int numRecords, int* numBits, vector<long> bucketArray) {
	fstream myFile("EmployeeIndex.txt");
	float currCap;

	// Add a bucket if above 80% capacity
	currCap =  ((float) numRecords) / ((*numBuckets)*4);
	cout << currCap << endl;
	if (currCap > 0.8) {
		(*numBuckets)++;

		if ( (*numBuckets) > pow(2, (*numBits)) ) {
			(*numBits)++;
		}

		// Allocate new space
		myFile.seekg(0, ios::end);
		myFile << endl;
		bucketArray.push_back(myFile.tellg());
		myFile << " ";

		myFile.close();

		// Move flip bits
		bucketArray = resort(*numBits, *numBuckets, bucketArray);
	}

	return bucketArray;
}

void getNextLineAndHash(istream& str)
{
	string cell, line;
	vector<long> bucketArray;
	int id, key, numBits = 1, numBuckets = 1, numRecords = 0;

	bucketArray = initIndex(bucketArray);

	// This iterates through the whole file line by line
	while(getline(str, line)){
		vector<string> result;
		stringstream lineStream(line);

		// Get the data for current line
		while(getline(lineStream,cell, ','))
		{
			result.push_back(cell);
		}

		// Use the id to get the key for which bucket it should be placed in
		id = stoi(result.at(0));
		key = hashAndGetBits(id, numBits);

		bucketArray = store(key, numBuckets, result, bucketArray);
		numRecords++;

		bucketArray = checkForCapacity(&numBuckets, numRecords, &numBits, bucketArray);
		cout << numBuckets << endl;
		cout << numBits << endl;
		cout << endl;
	}
  // Write numBits and numBuckets to a file for use with lookup
	writeBitsBuckets(numBits, numBuckets);

	// Write bucketArray to file for use with lookup
	writeBucketArray(bucketArray);
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

void importBitsAndBuckets(int* numBits, int* numBuckets) {
	fstream myFile("BitsandBuckets.txt");
	string temp;

	getline(myFile, temp);
	*numBits = stoi(temp);

	getline(myFile, temp);
	*numBuckets = stoi(temp);
}

vector<long> importBucketArray() {
	fstream myFile("BucketArray.txt");
	string temp;
	vector<long> bucketArray;

	while (getline(myFile, temp)) {
			bucketArray.push_back(stoi(temp));
	}

	return bucketArray;
}

void getHashIndex(string lookupId)
{
	int key, numBits, numBuckets, currId;
	vector<long> bucketArray;
	vector<string> records;
	long pos;
	ifstream myFile ("EmployeeIndex.txt");
	string bucket, currRecord, temp;

	importBitsAndBuckets(&numBits, &numBuckets);
	bucketArray = importBucketArray();

	// Hash id to get array index
	key = hashAndGetBits(stoi(lookupId), numBits);

	// Bit flip if necessary
	if (key+1 > numBuckets) {
		key = bitFlip(key);
	}

	// Get position of bucket in file
	pos = bucketArray.at(key);

	if (myFile.is_open()) {
		// Read in bucket
		myFile.seekg(pos);
		getline(myFile, bucket);

		// Separate records
		while (bucket != " ") {
			records.push_back(bucket.substr(0, bucket.find(';')));
			bucket.erase(0, (bucket.find(';')+1));
		}

		// Go through all the records and find the one we are looking for
		for (int i = 0; i < records.size(); i++) {
			// Extract id
			currRecord = records.at(i);
			currId = stoi(currRecord.substr(1, currRecord.find(" ", 1)-1));

			// Check to see if this is the correct record
			if (currId == stoi(lookupId)) {
				cout << "Here is the info corresponding to that ID:" << endl;

				temp = currRecord.substr(0, currRecord.find(" ", 1));
				currRecord.erase(0, (currRecord.find(" ", 1)));
				cout << "ID:" << temp << endl;

				temp = currRecord.substr(0, currRecord.find(" ", 1));
				currRecord.erase(0, (currRecord.find(" ", 1)));
				cout << "First Name:" << temp << endl;

				temp = currRecord.substr(0, currRecord.find(" ", 1));
				currRecord.erase(0, (currRecord.find(" ", 1)));
				cout << "Last Name:" << temp << endl;

				temp = currRecord.substr(0, currRecord.length()-10);
				currRecord.erase(0, currRecord.length()-10);
				cout << "Bio:" << temp << endl;

				temp = currRecord.substr(0, currRecord.find(" ", 1));
				currRecord.erase(0, (currRecord.find(" ", 1)));
				cout << "Manager ID:" << temp << endl;

				break;
			}

			if (i+1 == records.size()) {
				cout << "No information corresponding to that ID could be found." << endl;
			}
		}

		myFile.close();
	}
}

//Driver method to test map class
int main(int argc, char *argv[])
{
	cout << argv[1] << endl;
	if (string(argv[1]) == "-C") {
		cout << "Creating Hash Index" << endl;
		createHashIndex();
	}
	else if (string(argv[1]) == "-L") {
		cout << "Looking up " << argv[2] << endl;
		getHashIndex(string(argv[2]));
	}
	else {
		cout << "Incorrect Inputs" << endl;
	}
	return 0;
}
