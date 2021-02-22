#include <bits/stdc++.h>

using namespace std;

vector<string> getEmployeesFromFile() {
	vector<string> employees;
	string cell, line;

	ifstream file ("Emp.csv");
	if (file.is_open()){
		while (getline(file, line)){
			employees.push_back(line);
		}
		file.close();
	}
	return employees;
}

// Fill a block with 22 employees
vector<string> getEmployeeBlock(const vector<string> &emps, int initIndex) {

	vector<string> empBlock;
	int j = 0, incVal = 22;

	if(initIndex + incVal > emps.size()){
		incVal = 18;
	}

	for (int i = initIndex; i < initIndex + incVal; i++){
		empBlock.push_back(emps.at(i));
		j++;
	}
	return empBlock;
}

// get the integer value of empid
int getEmpId (vector<string> &empBlock, int index) {
	string s = empBlock[index];
	int pos = s.find(',');
	return stoi(s.substr(0, pos));
}

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

void sort() {
	vector<string> emps, empBlock;
	// Index to find next block of employees (inc by 22)
	int initIndex = 0, i = 0, j = 1;
	emps = getEmployeesFromFile();

	// Get block of 22 employees
	while (initIndex <= emps.size()) {
		empBlock = getEmployeeBlock(emps, initIndex);

		mergeSortBlock(empBlock, 0, empBlock.size()-1);

		cout << "Sorted Block " << j << ": " << endl;
		for (i = 0; i < empBlock.size(); i++){
			cout << getEmpId(empBlock, i) << endl;
		}
		j++;

		// Starting index of next block
		initIndex += 22;
	}



}

int main() {
	sort();

	return 0;
}