#include <iostream>
#include <string>
#include <fstream>

using namespace std;

bool isDigit(int x){
	for (int i = 0; i < 10; i++) {
		if (x==i)
			return true;
	}
	return false;
}

bool isOtherCharacter(char ch){
	for (int i = 0; i < 10; i++) {
		if ((ch-'0')==i)
			return false;
	}
	return !(ch=='e'||ch=='E'||ch=='.');
}

void identifiers(string &str, unsigned &index, ofstream &out) {
	int dfa[2][1] = { {1}, {1} };
	out << "Identifiers: ";
	int curState = 0;
	while (index<str.length() && (str[index]=='_' || (str[index]>='a' && str[index] <='z') || (str[index] >='A' && str[index]<='Z') || (str[index]>='0'&& str[index]<='9'))) {
		out << str[index];
		curState=dfa[curState][0];
		index++;
	}
	index--;
	out << endl;
}

void numbers(string &str, unsigned &index, ofstream &out) {
	int numbersDFA[7][3] = { {1,3,-1}, {1,2,5}, {2,-1,5}, {4,-1,-1}, {4,-1,5}, {6,-1,-1}, {6,-1,-1} };
	int curState=0;
	string str1 = "";
	bool dotCheck = true, eCheck = true;
	while (index<str.length()&&((str[index]>='0'&& str[index]<='9')||str[index]=='.'||str[index]=='e'||str[index]=='E')){

		// if (isOtherCharacter(str[index]))
		// 	break;

		if (isDigit(str[index]-'0')) {
			curState=numbersDFA[curState][0];
		} else if (str[index]=='.') {
			if (!dotCheck||!eCheck)
				break;
			dotCheck = false;
			curState=numbersDFA[curState][1];
		} else if (str[index]=='e'||str[index]=='E'){
			if (!eCheck) 
				break;
			eCheck = false;
			curState=numbersDFA[curState][2];
		}
		str1+=str[index];
		index++;
	}

	if (curState==5||curState==3) {
		out << "Error: " << str1 << endl;
		index--;
		return;
	}
	out << "Numbers: " << str1 << endl;
	index--;
}


int main(){

	ofstream out("output.txt");

	cout << "Enter String: ";
	string str;
	getline(cin, str);

	for (unsigned i = 0; i < str.length(); i++) {
		if (str[i]=='$'||str[i]=='@') {
			out << "Error: " << str[i] << endl;
		} else if ((str[i]>='a'&&str[i]<='z') || (str[i]>='A'&&str[i]<='Z')||str[i]=='_') {
			identifiers(str, i, out);
		} else if ((str[i]>='0'&&str[i]<='9')||str[i]=='.') {
			numbers(str, i, out);
		} else {
			cout << "Invalid Character...!\n";
			remove("output.txt");
			break;
		}
	}
	out.close();

	return 0;
}