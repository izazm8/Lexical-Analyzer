#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

bool noPath(int i) {
	return (i == -1);
}

bool hasReadFinalState(int lastSeenFinalState){
	return (lastSeenFinalState != -1);
}

bool isEscapeSequence(char ch) {
	return (ch == '\n' || ch == '\a' || ch == '\t' || ch == '\b' || ch == '\r' || ch == '\f' || ch == '\v' || ch == ' ');
}

void printEscapeSequence(char ch){
	if (ch == '\n')
		cout << "\\n";
	else if (ch == '\a')
		cout << "\\a";
	else if (ch == '\t')
		cout << "\\t";
	else if (ch == '\r')
		cout << "\\r";
	else if (ch == '\b')
		cout << "\\b";
	else if (ch == '\f')
		cout << "\\f";
	else if (ch == '\v')
		cout << "\\v";
}

void generateLexeme(string &inputStream, int &lexStart, int &remember, int &lastSeenFinalState, int &lineNo) {
	for (int i = lexStart; i < remember; i++) {
		if (inputStream[i] == '\\') {
			if (i == remember - 2 || i == inputStream[i + 1] == '\\') {
				cout << '\\';
			}
			else if (inputStream[i + 1] == 'n') {
				cout << "\\n"; i++;
			}
			else if (inputStream[i + 1] == 'a'){
				cout << "\\a"; i++;
			}
			else if (inputStream[i + 1] == 't'){
				cout << "\\t"; i++;
			}
			else if (inputStream[i + 1] == 'r') {
				cout << "\\r"; i++;
			}
			else if (inputStream[i + 1] == 'b') {
				cout << "\\b"; i++;
			}
			else if (inputStream[i + 1] == 'f') {
				cout << "\\f"; i++;
			}
			else if (inputStream[i + 1] == 'v') {
				cout << "\\v"; i++;
			}
			else {
				cout << '\\';
			}
		}
		else if (isEscapeSequence(inputStream[i])){
			printEscapeSequence(inputStream[i]);
		}
		else {
			cout << inputStream[i];
		}
	}
	cout << '\t' << lastSeenFinalState << endl;
}

bool reachedAtFinalState(int i, vector <vector<int> > &compressed) {
	for (int j = 0; j < compressed.size(); j++){
		if (i != 0 && i == compressed[j][compressed[j].size() - 1])
			return true;
	}
	return false;
}

int getNoOfStates(){
	ifstream in("input.txt");
	string str; int count = 0;
	while (!in.eof()) {
		getline(in, str);
		if (str.length() > 0 )
			count++;
	}
	in.close();
	return count - 1;
}

void print2dArray(int **arr, int rows, int cols) {
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++)
			cout << arr[i][j] << '\t';
		cout << endl;
	}
}

void printCol(int **arr, int rows, int col) {
	for (int i = 0; i < rows; i++) {
		cout << arr[i][col] << ' ';
	}
	cout << endl;
}

bool isEqual(int **arr, int col1, int col2, int rows) {
	for (int i = 0; i < rows; i++) {
		if (arr[i][col1] != arr[i][col2])
			return false;
	}
	return true;
}

void print2dVector(vector<vector<int> > &compressed) {
	for (int i = 0; i < compressed.size(); i++) {
		for (int j = 0; j < compressed[i].size(); j++)
			cout << compressed[i][j] << '\t';
		cout << endl;
	}
}


void addColToCompressedTable(vector<vector<int> > &compressed, int **arr, int rows, int col){
	for (int i = 0; i < rows; i++)
		compressed[i].push_back(arr[i][col]);
}

int existsInCompressed(vector<vector<int> > &compressed, int **NFA, int rows, int col) {
	for (int j = 0; j < compressed[0].size(); j++){
		bool check = true;
		for (int i = 0; i < rows; i++) {
			if (NFA[i][col] != compressed[i][j]) {
				check = false;
				break;
			}
		}
		if (check) {
			return j;
		}
	}
	return -1;
}

void printMapper(int mapper[]) {
	for (int i = 0; i < 129; i++)
		cout << mapper[i] << ' ';
	cout << endl;
}

int main(){

	int cols = 129;
	int noOfStates = getNoOfStates(); 	//rows
	cout << noOfStates << endl;
	vector<int> states;

	int **NFA = new int*[noOfStates];
	for (int i = 0; i < noOfStates; i++)
		NFA[i] = new int[cols];

	for (int i = 0; i < noOfStates; i++)
		for (int j = 0; j < cols; j++)
			NFA[i][j] = -1;

	ifstream in("input.txt");
	while (true) {
		int temp;
		in >> temp;
		if (temp >= 128) {
			states.push_back(128);
			break;
		}
		states.push_back(temp);
	}

	cout << states.size() << endl;

	for (int i = 0; i < noOfStates; i++) {
		int temp;
		for (int j = 0; j < states.size(); j++){
			in >> temp;
			NFA[i][states[j]] = temp;
		}
	}
	vector< vector<int> > compressed(noOfStates);

	for (int i = 0; i < 129; i++) {
		int j;
		for (j = 0; j < i; j++)
			if (isEqual(NFA, i, j, noOfStates))
				break;
		if (j == i)
			addColToCompressedTable(compressed, NFA, noOfStates, i);
	}

	print2dVector(compressed);

	int mapper[129];
	for (int i = 0; i < 129; i++) {
		int isUnique = existsInCompressed(compressed, NFA, noOfStates, i);
		if (isUnique != -1)
			mapper[i] = isUnique;
	}
	cout << endl;

	//printMapper(mapper);
	in.close();
	int lexStart = 0, lastSeenFinalState = -1, forward = 0, remember = 0, curState = 0, lineNo = 0;
	int input;
	ifstream in1("i.txt");
	string inputStream;	getline(in1, inputStream, '\0');
	in1.close();
	while (forward<inputStream.length()) {
		int escapeSequence;
		if (inputStream[forward] == '\\') {
			if (forward == inputStream.length() - 1 || forward == inputStream[forward + 1] == '\\') {
				escapeSequence = (int)'\\';
			}
			else if (inputStream[forward + 1] == 'n') {
				escapeSequence = int('\n');
				forward++; 				lineNo++;
			}
			else if (inputStream[forward + 1] == 'a'){
				escapeSequence = int('\a');
				forward++;
			}
			else if (inputStream[forward + 1] == 't'){
				escapeSequence = int('\t');
				forward++;
			}
			else if (inputStream[forward + 1] == 'r') {
				escapeSequence = int('\r');
				forward++;
			}
			else if (inputStream[forward + 1] == 'b') {
				escapeSequence = int('\b');
				forward++;
			}
			else if (inputStream[forward + 1] == 'f') {
				escapeSequence = int('\f');
				forward++;
			}
			else if (inputStream[forward + 1] == 'v') {
				escapeSequence = int('\v');
				forward++;
			}
			else {
				escapeSequence = (int)'\\';
			}
			curState = compressed[curState][mapper[escapeSequence]];
		}
		else if (isEscapeSequence(inputStream[forward])) {
			if (inputStream[forward] == '\n') {
				escapeSequence = (int)'\n'; lineNo++;
			}
			else if (inputStream[forward] == '\a') {
				escapeSequence = (int)'\a';
			}
			else if (inputStream[forward] == '\t') {
				escapeSequence = (int)'\t';
			}
			else if (inputStream[forward] == '\r') {
				escapeSequence = (int)'\r';
			}
			else if (inputStream[forward] == '\b') {
				escapeSequence = (int)'\b';
			}
			else if (inputStream[forward] == '\f') {
				escapeSequence = (int)'\f';
			}
			else if (inputStream[forward] == '\v') {
				escapeSequence = (int)'\v';
			}
			else if (inputStream[forward] == ' ') {
				escapeSequence = (int)' ';
			}
			curState = compressed[curState][mapper[escapeSequence]];
		}
		else {
			curState = compressed[curState][mapper[(int)inputStream[forward]]];
		}
		forward++;
		if (noPath(curState)) {
			if (hasReadFinalState(lastSeenFinalState)) {
				generateLexeme(inputStream, lexStart, remember, lastSeenFinalState, lineNo);
			}
			else {
				remember++;
				cout << "Error: ";
				if (isEscapeSequence(inputStream[remember - 1]))
					printEscapeSequence(inputStream[forward]);
				else
					cout << inputStream[remember - 1];
				cout << '\t' << "Line No.: " << lineNo << endl;
			}
			forward = remember;
			curState = 0, lexStart = remember, lastSeenFinalState = -1;
		}
		else if (reachedAtFinalState(compressed[curState][compressed[curState].size() - 1], compressed)) {
			lastSeenFinalState = compressed[curState][compressed[curState].size() - 1];
			remember = forward;
			if (forward == inputStream.length()){
				generateLexeme(inputStream, lexStart, remember, lastSeenFinalState, lineNo);
			}
		}
		else if (forward == inputStream.length()) {	//there is an lexeme to be generated and the path at the last character is not defined
			if (lastSeenFinalState == -1) {
				cout << "Error: ";
				if (isEscapeSequence(inputStream[remember - 1]))
					printEscapeSequence(inputStream[forward]);
				else
					cout << inputStream[remember - 1];
				cout << '\t' << "Line No.: " << lineNo << endl;
				break;
			}
			generateLexeme(inputStream, lexStart, remember, lastSeenFinalState, lineNo);
			forward = remember;
			curState = 0, lexStart = remember, lastSeenFinalState = -1;
		}
	}

	for (int i = 0; i < noOfStates; ++i) {
		delete[] NFA[i];
	}
	delete[] NFA;
	return 0;
}