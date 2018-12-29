#include<iostream>
#include<stdlib.h>
#include<fstream>
//#include"stack.h"  // ** can be removed
#include<vector>
#include<string>
using namespace std;

void printStack(vector<char> s);

//---------------------------------------
// CS421 File ll1.cpp for HW3A
// Your name: Jiali
//---------------------------------------

vector<char> M[3][5];  // the table of rules

void readrules()
{
    ifstream fin ("rules", ios::in);
    char ch;
    int row = 0; //index of table's row
    int col = 3; //index of table's column
    while(fin.get(ch)){ //read the rule file
        if(ch != '$' && ch!= ' ' && ch!='\n'){
            M[row][col].push_back(ch);
        }
        else if ( ch == '$'){
            col++;
        }
        if(col > 4 ){
            row++;
            col = 3;
        }
    }
    fin.close();
    
    char rowChar[] = {'S', 'A', 'B'};
    char colChar[] = {'S', 'A', 'B', '0', '1'};
    
    //loop the table and delete the first element of the M[i][j] -'A' , 'B', 'S'
    for(int i = 0; i <= 2; i++){
        for(int j = 0; j<= 4; j++){
            if(M[i][j].size() > 0){
                M[i][j].erase(M[i][j].begin());
            }
            
            //print out the table;
            cout<<"Row "<<rowChar[i]<<" col "<<colChar[j]<<" :";
            
            for(vector<char>::iterator it = M[i][j].begin(); it!= M[i][j].end(); ++it){
                cout<<" "<<*it;
            }
            cout<<endl;
        }
    }
}


void addtostack(vector<char> V, vector<char> &S)
{
    cout << "Adding rhs of a rule to the stack" << endl;

    //add M[i][j]'s content into the stack;
    for(vector<char>::iterator it = V.begin(); it != V.end(); it++){
        S.push_back(*it);
		cout << *it << " ";
    }
	cout << endl;
    //printout the current stack;
    cout << "Stack"<<endl;
    printStack(S);
}

int main()
{
    //using vector<char> to implement the stack;
    vector<char> stack;
    readrules();
    
    //read user's input;
    string ss;
    cout << "Enter a string made of 0's and/or 1's: ";
    cin >> ss;
    
    // push 'S' onto the stack to start
    stack.push_back('S');
    cout << "Stack"<<endl;
    printStack(stack);

    int i = 0;      //index of the char in the string(input)
    int col = 0;    //index of the table's column
    int row = 0;    //index of the table's row
    
    char stackElem; //the element poped from the stack (the top element of the stack)
    
    bool error = false; //flag to record if there is an error
    
    while (ss[i] != '\0')  // for each char of ss
    {
        cout<<"-----------------------"<<endl;
        cout<<"current char is "<<ss[i]<<endl;
        
        //no match in the table, show error and set the error flag.
        if(ss[i] != '0' && ss[i] != '1'){
            cout<<"Error no rule !"<<endl;
            error = true;
            break;
        }
        
        if(stack.size() == 0){
            cout<<"Error -- Empty stack"<<endl;
            error = true;
            break;
        }
        
        stackElem = stack[stack.size()-1];  //the last element in the stack
        
        //case: stack pop A B S
        if(stackElem == 'A' || stackElem == 'B' || stackElem == 'S'){
            if(stackElem == 'A') { row = 1; }
            else if(stackElem == 'B'){ row = 2; }
            else if(stackElem == 'S'){ row = 0;}
            
            if(ss[i] == '0') { col = 3;}
            else if(ss[i] == '1') { col = 4;}
            
            if(M[row][col].size() != 0){
                stack.erase(stack.end()-1);
                addtostack(M[row][col], stack);
				printStack(stack);
            }
            else
            {
                cout<<"Error no rule !"<<endl;
                error = true;
                break; //return 0;
            }
            
            stackElem = stack[stack.size()-1];
            
            cout<<"-----------------------"<<endl;
            cout<<"current char is "<<ss[i]<<endl;
            
            if(stackElem == ss[i]){ //if matched, delete that element in the stack
                cout<<"matched!"<<endl;
                stack.erase(stack.end()-1);
                
                cout<<endl;
                cout << "Stack"<<endl;
                printStack(stack);
            }
            else{
                cout<<"Error -- Mismatch"<<endl;
                error = true;
                break;
            }
        }
        
        //case 2: stack pop  0 1
        else if(stackElem == ss[i]){
            cout<<"matched!"<<endl;
            stack.erase(stack.end()-1);
            
            cout<<endl;
            cout << "Stack"<<endl;
            printStack(stack);
            //continue;
        }
    
        i++;
        
    }
    
    string result = (error == false ? "Accept!": "Reject!");
    cout<<"-----------------------"<<endl;
    cout<<result<<endl;
    return 0;
    
}

void printStack(vector<char> s){
    if(s.size() == 0){
        cout<<"[ Empty ]"<<endl;
    }
    else{
        for(int i = (int)s.size()-1; i >= 0 ; i--){
            cout<<s[i]<<endl;
        }
    }
}
