#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>

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
    fout << dept.did << ',' << dept.dname << ','
        << dept.budget << ',' << emp.eid << ',' << emp.ename << ',' << emp.age << ','
        << emp.salary << "\n";
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

int getSmallestEmp(vector<EmpBlock> emps) {
  priority_queue<int, vector<int>, greater<int>> sortedEids;
  int smallest;

  for (int i = 0; i < emps.size(); i++) {
    sortedEids.push(emps[i].eid);
  }

  while (sortedEids.empty() == false && sortedEids.top() == -1) {
    sortedEids.pop();
  }

  if (sortedEids.empty() == true) {
    return -1;
  }

  for (int i = 0; i < emps.size(); i++) {
    if (emps[i].eid == sortedEids.top()) {
      return i;
    }
  }
}

int getSmallestDept(vector<DeptBlock> depts) {
  priority_queue<int, vector<int>, greater<int>> sortedManagerIds;
  int smallest;

  for (int i = 0; i < depts.size(); i++) {
    sortedManagerIds.push(depts[i].managerid);
  }

  while (sortedManagerIds.empty() == false && sortedManagerIds.top() == -1) {
    sortedManagerIds.pop();
  }

  if (sortedManagerIds.empty() == true) {
    return -1;
  }

  for (int i = 0; i < depts.size(); i++) {
    if (depts[i].managerid == sortedManagerIds.top()) {
      return i;
    }
  }
}

EmpBlock getNextEmp(int runNum, vector<int> &empRunsFp) {
  string filename;
  fstream run;
  string line, word;
  EmpBlock emp;

  filename = "empRun" + to_string(runNum);
  run.open(filename, ios::in);
  run.seekg(empRunsFp[runNum], ios_base::beg);

  // Get the employee
  if (getline(run, line, '\n')) {
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
  } else {
      emp.eid = -1;
  }

  empRunsFp[runNum] = run.tellg();

  run.close();

  return emp;
}

DeptBlock getNextDept(int runNum, vector<int> &deptRunsFp) {
  string filename;
  fstream run;
  string line, word;
  DeptBlock dept;

  filename = "deptRun" + to_string(runNum);
  run.open(filename, ios::in);
  run.seekg(deptRunsFp[runNum], ios_base::beg);

  // Get the dept
  if (getline(run, line, '\n')) {
      stringstream s(line);
      getline(s, word,',');
      dept.did = stoi(word);
      getline(s, word, ',');
      dept.dname = word;
      getline(s, word, ',');
      dept.budget = stod(word);
      getline(s, word, ',');
      dept.managerid = stoi(word);
  } else {
      dept.did = -1;
  }

  deptRunsFp[runNum] = run.tellg();

  run.close();

  return dept;
}

void addEmpBlocks(vector<EmpBlock> &empBlocks, vector<EmpBlock> &nextEmps, vector<int> &empRunsFp, int numBlocks) {
  int smallest;
  EmpBlock emp;

  // Get the run that has the next smallest emp eid
  smallest = getSmallestEmp(nextEmps);

  // If there are no more next employees left, return -1
  if (smallest == -1) {
    emp.eid = -1;
    empBlocks.push_back(emp);
  }
  else {
    empBlocks.push_back(nextEmps[smallest]);

    // Update the nextEmps
    emp = getNextEmp(smallest, empRunsFp);
    nextEmps[smallest] = emp;
  }

  mergeSortBlockEmp(empBlocks, 0, empBlocks.size()-1);
}

void addDeptBlocks(vector<DeptBlock> &deptBlocks, vector<DeptBlock> &nextDepts, vector<int> &deptRunsFp, int numBlocks) {
  int smallest;
  DeptBlock dept;

  // Get the run that has the next smallest dept managerid
  smallest = getSmallestDept(nextDepts);

  // If there are no more next depts left, return -1
  if (smallest == -1) {
    dept.managerid = -1;
    deptBlocks.push_back(dept);
  }
  else {
    deptBlocks.push_back(nextDepts[smallest]);

    // Update the nextDepts
    dept = getNextDept(smallest, deptRunsFp);
    nextDepts[smallest] = dept;
  }

  mergeSortBlockDept(deptBlocks, 0, deptBlocks.size()-1);
}

void initNextEmps(vector<EmpBlock> &nextEmps, vector<int> &empRunsFp, int numEmpRuns) {
  string filename, line, word;
  EmpBlock emp;
  fstream run;

  // Get first line from all emp runs
  for (int i = 0; i < numEmpRuns; i++) {
    filename = "empRun" + to_string(i);
    run.open(filename, ios::in);

    // Get the employee
    if (getline(run, line, '\n')) {
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
    } else {
        emp.eid = -1;
    }

    nextEmps.push_back(emp);

    empRunsFp.push_back(run.tellg());

    run.close();
  }
}

void initNextDepts(vector<DeptBlock> &nextDepts, vector<int> &deptRunsFp, int numDeptRuns) {
  string filename, line, word;
  DeptBlock dept;
  fstream run;

  // Get first line from all dept runs
  for (int i = 0; i < numDeptRuns; i++) {
    filename = "deptRun" + to_string(i);
    run.open(filename, ios::in);

    // Get the dept
    if (getline(run, line, '\n')) {
        stringstream s(line);
        getline(s, word,',');
        dept.did = stoi(word);
        getline(s, word, ',');
        dept.dname = word;
        getline(s, word, ',');
        dept.budget = stod(word);
        getline(s, word, ',');
        dept.managerid = stoi(word);
    } else {
        dept.did = -1;
    }

    nextDepts.push_back(dept);

    deptRunsFp.push_back(run.tellg());

    run.close();
  }
}

void fillEmpBlocks(vector<EmpBlock> &empBlocks, vector<EmpBlock> &nextEmps, vector<int> &empRunsFp, int numBlocks) {
  int smallest;
  EmpBlock emp;

  // Fill half the blocks with the smallest employees
  for (int i = 0; i < numBlocks/2; i++) {
    // Get the run that has the next smallest emp eid
    smallest = getSmallestEmp(nextEmps);
    empBlocks.push_back(nextEmps[smallest]);

    // Update the nextEmps
    emp = getNextEmp(smallest, empRunsFp);
    nextEmps[smallest] = emp;
  }

  // Sort the current employees
  mergeSortBlockEmp(empBlocks, 0, empBlocks.size()-1);
}

void fillDeptBlocks(vector<DeptBlock> &deptBlocks, vector<DeptBlock> &nextDepts, vector<int> &deptRunsFp, int numBlocks) {
  int smallest;
  DeptBlock dept;

  // Fill half the blocks with the smallest employees
  for (int i = 0; i < numBlocks/2; i++) {
    // Get the run that has the next smallest emp eid
    smallest = getSmallestDept(nextDepts);
    deptBlocks.push_back(nextDepts[smallest]);

    // Update the nextEmps
    dept = getNextDept(smallest, deptRunsFp);
    nextDepts[smallest] = dept;
  }

  // Sort the current employees
  mergeSortBlockDept(deptBlocks, 0, deptBlocks.size()-1);
}

void mergeJoin(int numBlocks, int numEmpRuns, int numDeptRuns, fstream &joinout) {
  vector<int> empRunsFp;
  vector<int> deptRunsFp;
  vector<EmpBlock> empBlocks;
  vector<DeptBlock> deptBlocks;
  vector<EmpBlock> nextEmps;
  vector<DeptBlock> nextDepts;
  EmpBlock currEmp;
  DeptBlock currDept;
  int empIndex, deptIndex;
  string filename;

  // Initialize the nextEmps
  initNextEmps(nextEmps, empRunsFp, numEmpRuns);

  // Fill the empBlocks
  fillEmpBlocks(empBlocks, nextEmps, empRunsFp, numBlocks);

  // Initialize the nextDepts
  initNextDepts(nextDepts, deptRunsFp, numDeptRuns);

  // Fill the deptBlocks
  fillDeptBlocks(deptBlocks, nextDepts, deptRunsFp, numBlocks);

  // Continue to join until all possibilities have been checked
  while (true) {
    // Check if all of the indexes of empBlocks or deptBlocks contain -1, if yes, we are done. Time to exit
    for (int i = 0; empIndex < empBlocks.size(); empIndex++) {
      if (empBlocks[empIndex].eid != -1) {
        currEmp = empBlocks[empIndex];
        break;
      }
      else if (empIndex == empBlocks.size() - 1) {
        // Remove run files before exiting
        for (int i = 0; i < numEmpRuns; i++) {
          filename = "empRun" + to_string(i);
          remove(filename.c_str());
        }
        for (int i = 0; i < numDeptRuns; i++) {
          filename = "deptRun" + to_string(i);
          remove(filename.c_str());
        }

        exit(0);
      }
    }
    for (deptIndex = 0; deptIndex < deptBlocks.size(); deptIndex++) {
      if (deptBlocks[deptIndex].managerid != -1) {
        currDept = deptBlocks[deptIndex];

        break;
      }
      else if (deptIndex == deptBlocks.size() - 1) {
        // Remove run files before exiting
        for (int i = 0; i < numEmpRuns; i++) {
          filename = "empRun" + to_string(i);
          remove(filename.c_str());
        }
        for (int i = 0; i < numDeptRuns; i++) {
          filename = "deptRun" + to_string(i);
          remove(filename.c_str());
        }

        exit(0);
      }
    }

    // Otherwise, compare the two found values
    // if match join. Delete deptBlocks and grab next
    if (currEmp.eid == currDept.managerid) {
      joinout.open("Join.csv", ios::out | ios::app);
      printJoin(currEmp, currDept, joinout);
      joinout.close();
      deptBlocks.erase(deptBlocks.begin()+deptIndex);
      addDeptBlocks(deptBlocks, nextDepts, deptRunsFp, numBlocks);
    }
    // If empBlocks < deptBlocks, delete empBlocks, grab next smallest emp.
    else if (currEmp.eid < currDept.managerid) {
      empBlocks.erase(empBlocks.begin()+empIndex);
      addEmpBlocks(empBlocks, nextEmps, empRunsFp, numBlocks);
    }
    // If empBlocks > deptBlocks, delete deptBlocks, grab next smallest dept.
    else if (currEmp.eid > currDept.managerid) {
      deptBlocks.erase(deptBlocks.begin()+deptIndex);
      addDeptBlocks(deptBlocks, nextDepts, deptRunsFp, numBlocks);
    }
  }

  //MOVE UP WHEN JOIN ONLY ON Dept
  //WHEN NO MATCH, MOVE UP ON SMALLER ONE
}

int main() {
  // open file streams to read and write
  fstream empin;
  fstream deptin;
  fstream joinout;
  empin.open("Emp.csv", ios::in);
  deptin.open("Dept.csv", ios::in);
  joinout.open("Join.csv", ios::out | ios::trunc);
  joinout.close();
  int numBlocks = 22;
  int numEmpRuns, numDeptRuns;

  // Make records into run files
  numEmpRuns = makeEmpRuns(numBlocks, empin);
  numDeptRuns = makeDeptRuns(numBlocks, deptin);

  // Merge and join
  mergeJoin(numBlocks, numEmpRuns, numDeptRuns, joinout);

  return 0;
}
