#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <regex>
#include <stack>

using namespace std;

#define MAX 30
#define ADDRESS_INFILE  "f:\\input.txt"
#define ADDRESS_OUTFILE "f:\\output.csv"

struct FUNCTIONS
{
	string name;
	string disc;
	string postfix;
};
vector <FUNCTIONS> FUNCTION;
unsigned int N_OPERATORS = 2, N_INPUTS = 0, N_FUNCTIONS = 0;
string  Oper = "()";
int top = -1, i = 0, j = 0, k = 0;
char stk[MAX];
char infix[MAX];
char postfix[MAX];
int** table;
long int row, column;

inline int AND(int a, int b)
{
	if (a == b && a == 1)
		return 1;
	return 0;
}
inline int NAND(int a, int b)
{
	if (a == b && a == 1)
		return 0;
	return 1;
}
inline int OR(int a, int b)
{
	if (a == b && a == 0)
		return 0;
	return 1;
}
inline int NOR(int a, int b)
{
	if (a == b && a == 0)
		return 1;
	return 0;
}
inline int NOT(int a)
{
	if (a == 0)
		return 1;
	return 0;
}
inline int XOR(int a, int b)
{
	if (a == b)
		return 0;
	return 1;
}
inline int XNOR(int a, int b)
{
	if (a == b)
		return 1;
	return 0;
}
struct MyStruct
{
	char Symbol;
	string OP;
	int InStack;
	int InComing;
	int des[4];
} ST[10];
int prec(char c)
{
	for (auto i = 0; i < 10; i++)
		if (c == ST[i].Symbol)
			return ST[i].InStack;
	return -1;
}
int fin(string x, string col[], int n)
{
	for (auto i = 0; i < n; i++)
		if (x == col[i])
			return i;
	return -1;
}
void evaluatePostfix(vector<string> exp, int index, string col[], int column, int ROW)
{
	stack <int> Stack;
	int i, jj, a, b, res,cc;
	Stack.push(-1);
	for (jj = 0; jj < ROW; jj++)
	{
		for (i = 0; i < exp.size() - 1; i++)
		{
			cc = fin(exp[i], col, column);
			if (cc != -1) 
			{
				Stack.push(table[jj][cc]);
			}
			else
			{
				if (exp[i][0] == ST[2].Symbol)
				{
					//XNOR
					b = Stack.top();
					Stack.pop();
					a = Stack.top();
					Stack.pop();
					res = XNOR(a, b);
					Stack.push(res);
					//cout << "XNOR called." << endl;
				}
				else if (exp[i][0] == ST[3].Symbol)
				{
					//XOR
					b = Stack.top();
					Stack.pop();
					a = Stack.top();
					Stack.pop();
					res = XOR(a, b);
					Stack.push(res);
					//cout << "XOR called." << endl;
				}
				else if (exp[i][0] == ST[4].Symbol)
				{
					//NOR
					b = Stack.top();
					Stack.pop();
					a = Stack.top();
					Stack.pop();
					res = NOR(a, b);
					Stack.push(res);
					//cout << "NOR called." << endl;
				}
				else if (exp[i][0] == ST[5].Symbol)
				{
					//OR
					b = Stack.top();
					Stack.pop();
					a = Stack.top();
					Stack.pop();
					res = OR(a, b);
					Stack.push(res);
					//cout << "OR called." << endl;
					//cout << "A : " << a << " & B : " << b << endl;
				}
				else if (exp[i][0] == ST[6].Symbol)
				{
					//NAND
					b = Stack.top();
					Stack.pop();
					a = Stack.top();
					Stack.pop();
					res = NAND(a, b);
					Stack.push(res);
					//cout << "NAND called." << endl;
				}
				else if (exp[i][0] == ST[7].Symbol)
				{
					//AND
					b = Stack.top();
					Stack.pop();
					a = Stack.top();
					Stack.pop();
					res = AND(a, b);
					Stack.push(res);
					//cout << "AND called." << endl;
				}
				else if (exp[i][0] == ST[8].Symbol)
				{
					//NOT
					a = Stack.top();
					Stack.pop();
					res = NOT(a);
					Stack.push(res);
					//cout << "NOT called." << endl;
				}
			}
			//cout << "Stack top : " << Stack.top() << endl;
		}
		table[jj][index + N_INPUTS] = Stack.top();
		//cout << "Table entery : ";
		//cout << table[jj][index + N_INPUTS] << endl;
		Stack.pop();
	}
}
// The main function to convert infix expression to postfix expression 
string infixToPostfix(string s)
{
	stack<char> st;
	st.push('N');
	int l = s.length();
	string ns;
	ns = "";
	for (int i = 0; i < l; i++)
	{
		if (isspace(s[i]))
			continue;
		else if (isalnum(s[i])) 
		{
			ns += s[i];
			if (isalnum(s[i + 1])) 
			{
				ns += s[i + 1];
				i++;
			}
			ns += ' ';
			continue;
		}
		else if (s[i] == '(') 
		{
			st.push('(');
			continue;
		}
		else if (s[i] == ')')
		{
			while (st.top() != 'N' && st.top() != '(')
			{
				char c = st.top();
				st.pop();
				ns += c; //
				ns += ' '; //
			}
			if (st.top() == '(')
			{
				st.pop();
			}
		}
		//If an operator is scanned 
		else 
		{
			while (st.top() != 'N' && prec(s[i]) <= prec(st.top()))
			{
				char c = st.top();
				st.pop();
				ns += c;
				ns += ' ';
			}
			st.push(s[i]);
		}
	}
	// Pop all the remaining elements from the stack 
	while (st.top() != 'N')
	{
		char c = st.top();
		st.pop();
		ns += c;
	}
	ns += " -#";
	return ns;
}

string trim(string& str) 
{
	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

vector<string> tokenize(const string str, const regex re) 
{
	sregex_token_iterator it{ str.begin(), str.end(), re, -1 };
	vector<string> tokenized{ it, {} };
	tokenized.erase(
		remove_if(tokenized.begin(),
			tokenized.end(),
			[](string const& s) {
				return s.size() == 0;
			}),
		tokenized.end());
	return tokenized;
}

string  stringToUpper(string s) 
{
	for (unsigned int l = 0; l < s.length(); l++)
		s[l] = toupper(s[l]);
	return s;
}

int main() 
{
	int i;
	ST[0].Symbol = ')'; ST[0].InStack = -20; ST[0].InComing = -20; ST[0].OP = ")";
	ST[1].Symbol = '?'; ST[1].InStack = 1; ST[1].InComing = 4; ST[1].OP = "?";
	ST[2].Symbol = ' '; ST[2].InStack = 2; ST[2].InComing = 4; ST[2].OP = "XNOR";
	ST[3].Symbol = ' '; ST[3].InStack = 2; ST[3].InComing = 4; ST[3].OP = "XOR";
	ST[4].Symbol = ' '; ST[4].InStack = 2; ST[4].InComing = 4; ST[4].OP = "NOR";
	ST[5].Symbol = ' '; ST[5].InStack = 2; ST[5].InComing = 4; ST[5].OP = "OR";
	ST[6].Symbol = ' '; ST[6].InStack = 3; ST[6].InComing = 2; ST[6].OP = "NAND";
	ST[7].Symbol = ' '; ST[7].InStack = 3; ST[7].InComing = 2; ST[7].OP = "AND";
	ST[8].Symbol = ' '; ST[8].InStack = 4; ST[8].InComing = 1; ST[8].OP = "NOT";
	ST[9].Symbol = '('; ST[9].InStack = 0; ST[9].InComing = 4; ST[9].OP = "(";
	fstream fp(ADDRESS_INFILE, ios::in);
	fstream of(ADDRESS_OUTFILE, ios::out);
	if (!fp) 
	{
		cerr << "File can not open!" << endl;
		exit(1);
	}
	if (!of) 
	{
		cerr << "File can not open!" << endl;
		exit(2);
	}
	string LINE;
	char x;
	string y, yy;
	unsigned int ii;
	getline(fp, LINE);
	while (!fp.eof() && !LINE.find("Operators")) 
		getline(fp, LINE);
	while (!fp.eof() && LINE.find("Primary")) 
	{
		for (ii = 0; ii <= LINE.length(); ii++) 
		{
			if (!isalnum(LINE[ii])) 
			{
				x = LINE[ii];
				break;
			}
		}
		y = LINE.substr(LINE.find(x) + 1);
		y = trim(y);
		y = stringToUpper(y);
		for (ii = 0; ii < 10; ii++) 
		{
			if (y == ST[ii].OP) 
			{
				ST[ii].Symbol = x;
				Oper = Oper + x;
				N_OPERATORS++;
				break;
			}
		}
		getline(fp, LINE);
	}
	for (ii = 0; ii < 10; ii++) 
		cout << "<" << ST[ii].Symbol << " = " << ST[ii].OP << ">\n";
	getline(fp, LINE);
	const regex re(R"([\s|,]+)");
	vector<string> tokenized;
	while (!fp.eof() && LINE.find("Functions")) 
	{
		tokenized = tokenize(LINE, re);
		for (string token : tokenized)
			cout << token << endl;
		N_INPUTS = tokenized.size();
		getline(fp, LINE);
	}
	getline(fp, LINE);
	struct FUNCTIONS tmp;
	string postfix;
	while (!fp.eof()) 
	{
		yy = LINE.substr(0, LINE.find("="));
		yy = trim(yy);
		yy = stringToUpper(yy);
		y = LINE.substr(LINE.find("=") + 1);
		y = trim(y);
		y = stringToUpper(y);
		tmp.name = yy;
		tmp.disc = y;
		postfix = infixToPostfix(y);
		tmp.postfix = postfix;
		FUNCTION.push_back(tmp);
		getline(fp, LINE);
		N_FUNCTIONS++;
	}
	for (unsigned int jj = 0; jj < N_FUNCTIONS; jj++)
		cout << "<" << FUNCTION[jj].name << " = " << FUNCTION[jj].disc << " -> " << FUNCTION[jj].postfix << ">\n";
/*	for (unsigned int jj = 0; jj < tokenized.size(); jj++) {
		cout << tokenized[jj] << "*|";
	}
	for (unsigned int jj = 0; jj < FUNCTION.size(); jj++) {
		cout << FUNCTION[jj].name << "%|";
	}
	cout << "\n";
	*/
	row = (long int)pow(2, N_INPUTS);
	column = N_INPUTS + N_FUNCTIONS;
	table = new int* [row];
	for (int i = 0; i < row; i++) 
		table[i] = new int[column];
	k = 1;
	int l, stat, j;
	string* col = new string[column];
	for (i = 0; i < column; i++)
		col[i] = "";
	for (j = N_INPUTS - 1; j >= 0; j--) 
	{
		stat = 0; l = 0;
		for (i = 0; i < row; i++) 
		{
			table[i][j] = stat;
			l++;
			if (l >= k) 
			{
				l = 0;
				stat = 1 - stat;
			}
		}
		k = k * 2;
	}
	j = 0;
	for (unsigned int jj = 0; jj < tokenized.size(); jj++, j++) 
		col[j] = tokenized[jj];
	for (unsigned int jj = 0; jj < FUNCTION.size(); jj++, j++)
		col[j] = FUNCTION[jj].name;
	for (auto i = 0; i < row; i++)
		for (int j = N_INPUTS; j < column; j++)
			table[i][j] = 0;
	/*cout << "col : ";
	for (int j = 0; j < column; j++)
		cout << col[j] << ' ';
	cout << endl;*/
	string str, sep = "";
	for (i = 0; i < N_FUNCTIONS; i++)
	{
		str = FUNCTION[i].postfix;
		tokenized = tokenize(str, re);
		evaluatePostfix(tokenized, i, col, column, row);
	}
	for (int j = 0; j < column; j++) 
	{
		cout << col[j] << " |";
		of << col[j] << ";";
	}
	cout << "\n";
	of << "\n";
	for (int i = 0; i < row; i++) 
	{
		for (int j = 0; j < column; j++) 
		{
			cout << " " << table[i][j] << " |";
			of << table[i][j] << ";";
		}
		cout << "\n";
		of << "\n";
	}
	fp.close();
	of.close();
	return 0;
}