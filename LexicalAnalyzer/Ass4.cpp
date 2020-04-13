#include <iostream>
#include <string>
#include <fstream>
using namespace std;

bool isDigit(char x) {
	return ((x>='0'&&x<='9')||x=='.');
}

bool isCharacter(char x){
	return (x=='_'||(x>='A'&&x<='Z')||(x>='a'&&x<='z'));
}

bool isError(char x) {
	return x=='@';
	//return !(isCharacter(x)||isDigit(x));
}

bool isSpaces(char x){
	return (x=='\t'||x=='\n'||x=='\b'||x==' '||x=='\r');
}

void numbers(int numbersDFA[][8], string &str, unsigned &index, ofstream &out){//, ofstream &out) {
	int curState=0;
	string str1 = "";
	bool dotCheck = true, eCheck = true;
	while (index<str.length()&&((str[index]>='0'&& str[index]<='9')||str[index]=='.'||str[index]=='e'||str[index]=='E')){

		if (isDigit(str[index])) {
			if (str[index]=='.') {
				if (!dotCheck||!eCheck)
					break;
				dotCheck = false;
				curState=numbersDFA[curState][2];
			} else {
				curState=numbersDFA[curState][0];
			}
		} 
		else if (str[index]=='e'||str[index]=='E'){
			if (!eCheck) 
				break;
			eCheck = false;
			curState=numbersDFA[curState][1];
		}
		str1+=str[index];
		index++;
	}
	if (curState==4||curState==3) {
		out << "Incomplete Number: " << str1 << endl;
		index--;
		return;
	}
	out << "Numbers: " << str1 << endl;
	index--;
}

void identifiers(int dfa[][8], string &str, unsigned &index, ofstream &out) {
	out << "Identifiers: ";
	int curState = 0;
	while (index<str.length() && (str[index]=='_' || (str[index]>='a' && str[index] <='z') || (str[index] >='A' && str[index]<='Z') || (str[index]>='0'&& str[index]<='9'))) {
		out << str[index];
		curState=dfa[curState][7];
		index++;
	}
	index--;
	out << endl;
}

void escapeSequences(int nfa[][8], string &str, unsigned &index, ofstream &out) {
	while (isSpaces(str[index])) {
		if (index+1<str.length()&&str[index]=='\\'&&(str[index+1]=='r'||str[index+1]=='n'||str[index+1]=='t'||str[index+1]=='b'))
			index++;
		index++;
	}
	index--;
	out << "Escape Sequences: " << endl;
}

int main(){

	ofstream out("output.txt");

	int NFA[8][8] = { {1,-1,3,0,-1,6,7,7}, {1,4,2,1,-1,-1,-1,-1}, {2,4,-1,1,-1,-1,-1,-1}, {2,-1,-1,0,-1,-1,-1,-1}, {5,-1,-1,0,-1,-1,-1,-1}, {5,-1,-1,1,-1,-1,-1,-1}, {-1,-1,-1,1,-1,6,-1,-1}, {7,7,-1,1,-1,-1,7,7} };
	
	string str; //= "23....4ea213eb216\n\t\b...32ea1";
	cout << "Enter String: ";
	getline(cin,str);

	//str = "asdf\b\tasdf\tasdf\n123.e";
	
	for (unsigned i = 0; i < str.length(); i++) {
		if (isDigit(str[i])){
			numbers(NFA,str,i,out);
		} else if (isCharacter(str[i])) {		//also include underScore(_)
			identifiers(NFA, str, i,out);			
		} else if (isError(str[i])) {			//@, $ etc..
			out << "Error: " << str[i] << endl;
		} else if (isSpaces(str[i])) {			//\b, \n, \t. ' ' and other spaces	
			escapeSequences(NFA, str, i,out);
		}
	}

	out.close();

	return 0;
}