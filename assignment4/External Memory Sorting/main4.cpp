#include <bits/stdc++.h>

using namespace std;

// Used to track number of runs and increment block sizes
int runNum = 1;

// Fill a vector with employees from the given file
vector<string> getEmployeesFromFile(string fileName) {
	vector<string> employees;
	string cell, line;

	ifstream file (fileName);
	if (file.is_open()){
		while (getline(file, line)){
			employees.push_back(line);
		}
		file.close();
	}
	return employees;
}

// Fill a block with employees (22*runNum)
vector<string> getEmployeeBlock(const vector<string> &emps, int initIndex, int incVal) {

	vector<string> empBlock;

	// Fill block with remaining employees if they don't fill entire block
	if(initIndex + incVal > emps.size()){
		incVal = emps.size() - initIndex;
	}

	for (int i = initIndex; i < initIndex + incVal; i++){
		empBlock.push_back(emps.at(i));
	}
	return empBlock;
}

// Get the integer value of empid
int getEmpId (vector<string> &empBlock, int index) {
	string s = empBlock[index];
	int pos = s.find(',');
	return stoi(s.substr(0, pos));
}

// Classic merge sort with eid
void merge(vector<string> &empBlock, int left, int mid, int right) {
	int i = 0, j = 0, k = left;
	int n1 = mid-left+1;
	int n2 = right-mid;

	// copy data to temp vectors
	vector<string> L, R;
	for (int x = 0; x < n1; x++){
		L.push_back(empBlock[left+x]);
	}
	for (int y = 0; y < n2; y++){
		R.push_back(empBlock[mid+1+y]);
	}

	// check for need to swap
	while (i < n1 && j < n2) {
		if (getEmpId(L, i) <= getEmpId(R, j)) {
			empBlock[k] = L[i];
			i++;
		}
		else {
			empBlock[k] = R[j];
			j++;
		}
		k++;
	}

	// copy any remaining values over to empblock
	while (i < n1) {
		empBlock[k] = L[i];
		i++;
		k++;
	}
	while (j < n2) {
		empBlock[k] = R[j];
		j++;
		k++;
	}
}

void mergeSortBlock(vector<string> &empBlock, int left, int right) {
	// Recursive base case
	if (left >= right) { return; }

	int mid = left+(right-left)/2;
	mergeSortBlock(empBlock, left, mid);
	mergeSortBlock(empBlock, mid+1, right);
	merge(empBlock, left, mid, right);
}

// Get blocks of employees and output to the file
void getBlocksAndOutput(vector<string> &emps) {
	vector<string> empBlock;
	// Index to find next block of employees (inc by 22*runNum)
	int initIndex = 0, i = 0;

	// Loop for one run to sort blocks
	while (initIndex <= emps.size()) {
		empBlock = getEmployeeBlock(emps, initIndex, 22*runNum);

		mergeSortBlock(empBlock, 0, empBlock.size()-1);

		//Write each newly sorted block to file
		ofstream outputFile;
		outputFile.open("EmpSorted.csv", ios_base::app);

		for (i = 0; i < empBlock.size(); i++){
			outputFile << empBlock[i] << endl;
		}
		outputFile.close();

		// Starting index of next block
		initIndex += 22*runNum;
	}
}

// Do multiple runs increasing the block size to sort previously sorted blocks
void sort() {
	vector<string> emps;
	if (runNum == 1){
		emps = getEmployeesFromFile("Emp.csv");
	}

	while (22*runNum <= emps.size()) {
		if (runNum > 1) {
			emps = getEmployeesFromFile("EmpSorted.csv");
			remove("EmpSorted.csv");
		}

		getBlocksAndOutput(emps);

		runNum++;
	}
}

int main() {
	// Remove leftover results
	remove("EmpSorted.csv");
	sort();

	return 0;
}