#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int identifyTheInput(char &ch, int &curState){
	if (ch >= '0' && ch <= '9')
		return 0;
	if (ch == '.')
		return 2;
	if (ch == '@')
		return 4;
	if (ch == '\\' || ch == ' ')
		return 5;
	if (ch == '_')
		return 6;
	if (curState==0 && ((ch >='a' && ch <='z')||(ch >= 'A' && ch <= 'Z')) || (curState!=0 && (((ch >='a' && ch <='d')||(ch >= 'f' && ch <= 'z'))||((ch >= 'A' && ch <= 'D')||(ch>='F' && ch <= 'Z')))))
		return 7;
	if (ch == 'e' || ch == 'E')
		return 1;
}

void maxMunchEscapeSequences(int &i, string &inputStream) {
	while (true) {
		if (inputStream[i]==' ')
			i++;
		else if (inputStream[i]=='\\' && (inputStream[i+1]=='n'||inputStream[i+1]=='r'||inputStream[i+1]=='t'||inputStream[i+1]=='b'))
			i++;
		else break;
	}
}

bool noPath(int i) {
	return (i==-1);
}

bool hasReadFinalState(int lastSeenFinalState){
	return (lastSeenFinalState!=-1);
}

void generateLexeme(string &inputStream, int &lexStart, int &remember) {
	cout << inputStream.substr(lexStart, (remember-lexStart)) << endl;
}

bool reachedAtFinalState(int i) {
	return (i==101||i==102||i==103||i==104||i==105);
}

int main(){

	int NFA[8][8] = { {1,-1,3,0,-1,6,7,7}, {1,4,2,101,-1,-1,-1,-1}, {2,4,-1,102,-1,-1,-1,-1}, {2,-1,-1,0,-1,-1,-1,-1}, {5,-1,-1,0,-1,-1,-1,-1}, {5,-1,-1,103,-1,-1,-1,-1}, {-1,-1,-1,104,-1,6,-1,-1}, {7,7,-1,105,-1,-1,7,7} };

	int lexStart=0, lastSeenFinalState=-1, forward=0, remember=0, curState=0, lineNo=0;
	int input;
	string inputStream;	getline(cin, inputStream);

	while (forward<inputStream.length()) {
		input = identifyTheInput(inputStream[forward], curState);
		forward++;
		curState = NFA[curState][input];
		if (noPath(curState)) {
			if (hasReadFinalState(lastSeenFinalState)) {
				generateLexeme(inputStream, lexStart, remember);
			} else {
				remember++;
				cout << "Error: " << inputStream[forward-1] << endl;
			}
			forward=remember;
			curState=0, lexStart=remember, lastSeenFinalState=-1;
		}
		else if (reachedAtFinalState(NFA[curState][3])) {
			lastSeenFinalState=curState;
			remember=forward;
			if (forward==inputStream.length())
				generateLexeme(inputStream, lexStart, remember);
		} else if (forward==inputStream.length()) {	//there is an lexeme to be generated and the path at the last character is not defined
			forward=remember;
			generateLexeme(inputStream, lexStart, remember);
			curState=0, lexStart=remember, lastSeenFinalState=-1;
		}
	}
	return 0;
}