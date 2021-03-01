#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct EmpBlock {
    int eid;
    string ename;
    int age;
    double salary;
};

struct DeptBlock {
    int did;
    string dname;
    double budget;
    int managerid;
};


// Grab a single block from the emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpBlock grabEmp(fstream &empin) {
    string line, word;
    EmpBlock emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        emp.eid = stoi(word);
        getline(s, word, ',');
        emp.ename = word;
        getline(s, word, ',');
        emp.age = stoi(word);
        getline(s, word, ',');
        emp.salary = stod(word);
        return emp;
    } else {
        emp.eid = -1;
        return emp;
    }
}

// Grab a single block from the dept.csv file, in theory if a block was larger than
// one tuple, this function would return an array of DeptBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
DeptBlock grabDept(fstream &deptin) {
    string line, word;
    DeptBlock dept;
    if (getline(deptin, line, '\n')) {
        stringstream s(line);
        getline(s, word,',');
        dept.did = stoi(word);
        getline(s, word, ',');
        dept.dname = word;
        getline(s, word, ',');
        dept.budget = stod(word);
        getline(s, word, ',');
        dept.managerid = stoi(word);
        return dept;
    } else {
        dept.did = -1;
        return dept;
    }
}

//Print out the attributes from emp and dept when a join condition is met
void printJoin(EmpBlock emp, DeptBlock dept, fstream &fout) {
    fout << emp.eid << ',' << emp.ename << ',' << emp.age << ','
        << emp.salary << ',' << dept.did << ',' << dept.dname << ','
        << dept.budget << "\n";
}

//Print out the attributes from emp
void printEmp(EmpBlock emp, fstream &fout) {
    fout << emp.eid << ',' << emp.ename << ',' << emp.age << ','
        << emp.salary << "\n";
}

//Print out the attributes from dept
void printDept(DeptBlock dept, fstream &fout) {
    fout << dept.did << ',' << dept.dname << ','
        << dept.budget << ',' << dept.managerid << "\n";
}

// Classic merge sort with eid
void mergeEmp(vector<EmpBlock> &empBlocks, int left, int mid, int right) {
	int i = 0, j = 0, k = left;
	int n1 = mid-left+1;
	int n2 = right-mid;

	// copy data to temp vectors
	vector<EmpBlock> L, R;
	for (int x = 0; x < n1; x++){
		L.push_back(empBlocks[left+x]);
	}
	for (int y = 0; y < n2; y++){
		R.push_back(empBlocks[mid+1+y]);
	}

	// check for need to swap
	while (i < n1 && j < n2) {
		if ((L[i].eid) <= (R[j].eid)) {
			empBlocks[k] = L[i];
			i++;
		}
		else {
			empBlocks[k] = R[j];
			j++;
		}
		k++;
	}

	// copy any remaining values over to empBlocks
	while (i < n1) {
		empBlocks[k] = L[i];
		i++;
		k++;
	}
	while (j < n2) {
		empBlocks[k] = R[j];
		j++;
		k++;
	}
}

void mergeSortBlockEmp(vector<EmpBlock> &empBlocks, int left, int right) {
	// Recursive base case
	if (left >= right) { return; }

	int mid = left+(right-left)/2;
	mergeSortBlockEmp(empBlocks, left, mid);
	mergeSortBlockEmp(empBlocks, mid+1, right);
	mergeEmp(empBlocks, left, mid, right);
}

// Classic merge sort with managerid
void mergeDept(vector<DeptBlock> &deptBlocks, int left, int mid, int right) {
	int i = 0, j = 0, k = left;
	int n1 = mid-left+1;
	int n2 = right-mid;

	// copy data to temp vectors
	vector<DeptBlock> L, R;
	for (int x = 0; x < n1; x++){
		L.push_back(deptBlocks[left+x]);
	}
	for (int y = 0; y < n2; y++){
		R.push_back(deptBlocks[mid+1+y]);
	}

	// check for need to swap
	while (i < n1 && j < n2) {
		if ((L[i].managerid) <= (R[j].managerid)) {
			deptBlocks[k] = L[i];
			i++;
		}
		else {
			deptBlocks[k] = R[j];
			j++;
		}
		k++;
	}

	// copy any remaining values over to deptBlocks
	while (i < n1) {
		deptBlocks[k] = L[i];
		i++;
		k++;
	}
	while (j < n2) {
		deptBlocks[k] = R[j];
		j++;
		k++;
	}
}

void mergeSortBlockDept(vector<DeptBlock> &deptBlocks, int left, int right) {
	// Recursive base case
	if (left >= right) { return; }

	int mid = left+(right-left)/2;
	mergeSortBlockDept(deptBlocks, left, mid);
	mergeSortBlockDept(deptBlocks, mid+1, right);
	mergeDept(deptBlocks, left, mid, right);
}

int makeEmpRuns(int numBlocks, fstream &empin) {
  int numRuns = 0;
  bool flag = true;
  string runName;
  fstream empout;

  // sort Emp.csv into runs
  while (flag) {
    vector<EmpBlock> empBlocks;

    // Grab 22 employees
    for (int i = 0; i < numBlocks; i++) {
      EmpBlock currEmp = grabEmp(empin);
      // checks if filestream is empty
      if (currEmp.eid == -1) {
          flag = false;
          break;
      }

      // Add to current blocks
      empBlocks.push_back(currEmp);
    }

    // Sort current 22 employees into a run
    mergeSortBlockEmp(empBlocks, 0, empBlocks.size()-1);

    // Print current run to file
    runName = "empRun" + to_string(numRuns);
    empout.open(runName, ios::out | ios::trunc);
    for (int i = 0; i < empBlocks.size(); i++) {
      printEmp(empBlocks[i], empout);
    }
    empout.close();

    numRuns++;
  }

  return numRuns;
}

int makeDeptRuns(int numBlocks, fstream &deptin) {
  int numRuns = 0;
  bool flag = true;
  string runName;
  fstream deptout;

  // sort Emp.csv into runs
  while (flag) {
    vector<DeptBlock> deptBlocks;

    // Grab 22 employees
    for (int i = 0; i < numBlocks; i++) {
      DeptBlock currDept = grabDept(deptin);
      // checks if filestream is empty
      if (currDept.did == -1) {
          flag = false;
          break;
      }

      // Add to current blocks
      deptBlocks.push_back(currDept);
    }

    // Sort current 22 employees into a run
    mergeSortBlockDept(deptBlocks, 0, deptBlocks.size()-1);

    // Print current run to file
    runName = "deptRun" + to_string(numRuns);
    deptout.open(runName, ios::out | ios::trunc);
    for (int i = 0; i < deptBlocks.size(); i++) {
      printDept(deptBlocks[i], deptout);
    }
    deptout.close();

    numRuns++;
  }

  return numRuns;
}

int main() {
  // open file streams to read and write
  fstream empin;
  fstream deptin;
  fstream joinout;
  empin.open("Emp.csv", ios::in);
  deptin.open("Dept.csv", ios::in);
  joinout.open("Join.csv", ios::out | ios::app);
  int numBlocks = 22;
  int numEmpRuns, numDeptRuns;

  // Make records into run files
  numEmpRuns = makeEmpRuns(numBlocks, empin);
  numDeptRuns = makeDeptRuns(numBlocks, deptin);

  // Merge and join


/*
  // opens new filestream for dept relation (needs to read in a new one each time a new emp block is seen)


  flag = true
  while (flag) {
      // FOR BLOCK IN RELATION DEPT
      DeptBlock deptBlock = grabDept(deptin);

      // in theory these would iterate through the two blocks: empBlock and deptBlock
      // but since both only contain one tuple, no iteration is needed
      if (deptBlock.did == -1) {
          flag = false;
      } else {
          // check join condition and print join to output file
          if (deptBlock.managerid == empBlock.eid) {
              printJoin(empBlock, deptBlock, joinout);
          }
      }
  }
  */
  return 0;
}
