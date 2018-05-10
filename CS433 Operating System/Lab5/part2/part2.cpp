//============================================================
// CS 433 Operating System
// Assignment 5: Memeory
// Part 2: Simulation of FIFO, LRU and optimal algorithms
// Name: Runyu Xu
// Date: 05/03/2018
// File type: driver file
// Description: This is a simulation that implements the FIFO,
// LRU and optimal page replacement algorithms. You can set the 
// number of pages and number of frames, then the simulation will
// generate a random page reference list and run the algorithm that
// you selected.
//=============================================================

#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

// Purpose: method to generate a random page-reference string
// Parameter: num -- the number of pages
//            reference -- the page reference string list
// Return: reference -- the page reference string list
vector<int> randomReference(int num, vector<int> reference) {
	cout << "The random page-reference string is: ";
	int stringLength = rand() % (30 - num + 1) + num; // generate a random length
	for (int i = 0; i < stringLength; i++) { // add random reference in
		reference.push_back(rand() % num);
		cout << reference[i] << " ";
	}
	cout << endl;
	return reference;
}

// Purpose: method to check if a reference is already in the frame
// Parameter: index -- the index of the reference in the page vector
//            size -- the size of the current frame
//            frame -- the frame list
//            references -- the references list
// Return: return i -- if find this reference
//         return -1 -- if not find 
int searchFrame(int index, vector<int> frame, vector<int> references) {
	for (int i = 0; i < frame.size(); i++) { // for loop to search the frame
		// if find this reference, return i
		if (references[index] == frame[i]) {
			return i;
		}
	}
	return -1; //deafult return 0
}

// Purpose: display the frame list and number of page faults
// Parameter: index -- the index of the reference adding into the frame
//            faultNum -- the number of faults
//            frame -- the frame list
//            references -- the references list
void display(int index, int faultNum, vector<int> frame, vector<int> references) {
	cout << "Adding reference " << references[index] << endl;
	for (int k = 0; k < frame.size(); k++) {
		cout << frame[k] << " ";
	}
	cout << "         " << "total page faults: " << faultNum << endl;
}

// FIFO algorithm
// Purpose: FIFO page replacement method
// Parameter: size -- the size of Frame list
//            reference -- the page reference list
void fifo(int size, vector<int> reference) {
	vector<int> frame;
	int index = 0; // the index in the page reference array
	int faultNum = 0; // the number of page faults
	int i = 0;// the index in the frame
	int firstIndex = 0; // the oldest element in the frame
	cout << "FIFO Page Replacement Algorithm: " << endl;
	while (index < reference.size()) {
		// 1. check if the reference is already in frame
		if (searchFrame(index, frame, reference) != -1) {
			index++;
		}
		else {
			// 2. if the frame is not full, add reference in
			// and record a page fault
			if (i < size) {
				frame.push_back(reference[index]);
				i++;
			}
			// 3. if the frame is full, replace the oldest one with 
			// the newest reference, and record a page fault
			else {
				int temp = reference[index];
				frame[firstIndex] = temp;
				firstIndex = (firstIndex + 1) % size;
			}
			index++;
			faultNum++;
		}
		// print the frame and # of page faults for each reference		
		display(index - 1, faultNum, frame, reference);
	}
	cout << "Finish FIFO, the final number of total faults is: " << faultNum << endl << endl;
}

// LRU algorithm
// Purpose: LRU page replacement method
// Parameter: size -- the size of Frame list
//            reference -- the page reference list
void LRU(int size, vector<int> reference) {
	vector<int> frame;
	vector<int> count; // the vector used to count the time of each index
	int index = 0; // the index in the page reference array
	int faultNum = 0; // the number of page faults
	int i = 0;// the index in the frame
	int least = 0; // the least recently used element in the frame
	// Initalize the count for each index in the frame to -1
	for (int k = 0; k < size; k++) {
		count.push_back(-1);
	}
	cout << "LRU Page Replacement Algorithm: " << endl;
	// input page reference into the frame
	while (index < reference.size()) {
		// 1. check if the reference is already in frame
		if (searchFrame(index, frame, reference) != -1) {
			int position = searchFrame(index, frame, reference);
			//increase the count of each reference in the frame
			for (int k = 0; k < count.size(); k++) {
				count[k]++;
			}
			count[position] = 0;// reset the count of this reference
			index++;
		}
		else {
			// 2. if the frame is not full, add reference in
			// and record a page fault
			if (i < size) {
				//add the reference into the frame
				frame.push_back(reference[index]);
				//increase the count of referencesin the frame
				for (int k = 0; k <= i; k++) {
					count[k]++;
				}
				i++;
				index++;
				faultNum++;
			}
			// 3. if the frame is full, replace the least recently one with 
			// the newest reference, and record a page fault
			else {
				int max = 0; // the max one in count vector
				int temp = reference[index]; // the new reference
				for (int k = 0; k < count.size(); k++) {
					if (count[k] >= max) { // find the max value 
						max = count[k]; //update max 
						least = k; // update least recently used index
					}
					count[k]++; // increase the count of each reference in the frame
				}
				frame[least] = temp; // replace the least recently used reference
				count[least] = 0; // reset its count to 0
				index++;
				faultNum++;
			}
		}
		// print the frame and # of page faults for each reference
		display(index - 1, faultNum, frame, reference);
	}
	cout << "Finish LRU, the final number of total faults is: " << faultNum << endl << endl;
}

// Optimal algorithm
// Purpose: Optimal algorithm method
// Parameter: size -- the size of Frame list
//            reference -- the page reference list
void optimal(int size, vector<int> reference) {
	vector<int> frame;
	int index = 0; // the index in the page reference array
	int faultNum = 0; // the number of page faults
	int i = 0;// the index in the frame
	cout << "Optimal Page Replacement Algorithm: " << endl;
	while (index < reference.size()) {
		// 1. check if the reference is already in frame
		if (searchFrame(index, frame, reference) != -1) {
			index++;
		}
		else {
			// 2. if the frame is not full, add reference in
			// and record a page fault
			if (i < size) {
				frame.push_back(reference[index]);
				i++;
				index++;
				faultNum++;
			}
			// 3. if the frame is full, replace the farthest one with 
			// the newest reference, and record a page fault
			else {
				int farthest = index; // the farthest index in page reference list
				int replaceIndex = 0; // the replace index in the frame
				int temp = reference[index];
				for (int k = 0; k < frame.size(); k++) {
					for (int j = index + 1; j < reference.size(); j++) {
						if (frame[k] == reference[j]) { // if the refernce will be used in future
							if (j > farthest) { // if its index is bigger the farthest, replace
								farthest = j;
								replaceIndex = k;
							}
							break;
						}
						// if there exists a reference that is never used, break
						else if (farthest == reference.size()) {
							break;
						}
						// if a reference is never used again, set it to replace
						else if (j == reference.size() - 1) {
							farthest = j + 1;
							replaceIndex = k;
						}
					}
				}
				frame[replaceIndex] = temp;
				index++;
				faultNum++;
			}
		}
		// print the frame and # of page faults for each reference
		display(index - 1, faultNum, frame, reference);
	}
	cout << "Finish Optimal, the final number of total faults is: " << faultNum << endl << endl;
}



// Main method to run the program
int main() {
	int selection = 1; // selection initial with 1
	int page; // the number of pages entered by user
	int frame; // the number of frames entered by user
	vector<int> page_reference; // the page reference vector
	cout << "---------------------------------------------------------\n";
	cout << "// Name: Runyu Xu\n";
	cout << "// Date : May 3rd, 2018\n";
	cout << "// Course : CS433 - Operating Systems\n";
	cout << "// Assignment 5: Memory \n";
	cout << "// Part2: Simulation of FIFO, LRU and optimal algorithms \n";
	cout << "---------------------------------------------------------\n";

	// Get the number of pages and frames to run the simulation
	cout << "Please enter the number of pages(1-10): ";
	cin >> page;
	cout << endl;
	while (page < 1 || page>10) {
		cout << "Invaild value, please enter again: ";
		cin >> page;
		cout << endl;
	}
	page_reference = randomReference(page, page_reference);
	cout << endl << "Please enter the number of frames(1-7): ";
	cin >> frame;
	cout << endl;
	while (frame < 1 || frame > 7) {
		cout << "Invaild value, please enter again: ";
		cin >> frame;
		cout << endl;
	}
	//Execute the program until user exit 
	while (selection != 0) {
		cout << "=========================Menu==============================" << endl;
		cout << "enter 1-3 to execute different algorithms, enter 0 to exit: " << endl;
		cout << "1. FIFO replacement algorithm;" << endl;
		cout << "2. LRU replacement algorithm;" << endl;
		cout << "3. Optimal replacement algorithm;" << endl;
		cout << "0. exit." << endl;
		cin >> selection;

		switch (selection) {
		case 1:
			fifo(frame, page_reference);
			break;
		case 2:
			LRU(frame, page_reference);
			break;
		case 3:
			optimal(frame, page_reference);
			break;
		case 0:
			exit(0);
		}
	}
}


