#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <stdlib.h>  
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

vector<string> removeComments(vector<string>& source) {
	vector<string> ans;
	bool inBlock = false;
	string sf;
	for (auto &t : source) {
		for (int i = 0; i < t.size();) {
			if (!inBlock) {
				if (i + 1 == t.size()) sf += t[i++];
				else {
					string m = t.substr(i, 2);
					if (m == "/*") inBlock = true, i += 2;
					else if (m == "//") break;
					else sf += t[i++];
				}
			}
			else {
				if (i + 1 == t.size()) i++;
				else {
					string m = t.substr(i, 2);
					if (m == "*/") inBlock = 0, i += 2;
					else i++;
				}
			}
		}
		if (sf.size() && !inBlock) ans.push_back(sf), sf = "";
	}
	return ans;
}

struct PreprocessResult
{
	int num_includes = 0;
	int num_defines = 0;
	int num_pragmas = 0;
	int num_def_conditionals = 0;
	int num_conditionals = 0;
};

void analyzePreprocess(vector<string> const& aInput, PreprocessResult& aResult) {
	for (auto line : aInput)
	{
		//find first character that isn't a space or tab
		for (int i = 0; i < line.length(); ++i) {
			if (line[i] != ' ' || line[i] != '\t') {
				//check if this is a preprocessor directive
				if (line[i] == '#') {
					//identify directive
					string substring = line.substr(i + 1, i + 8);
					//include
					if (substring.find("include") != string::npos) {
						++aResult.num_includes;
					}
					//define
					else if (substring.find("define") != string::npos) {
						++aResult.num_defines;
					}
					//pragma
					else if (substring.find("pragma") != string::npos) {
						++aResult.num_pragmas;
					}
					//ifdef
					//ifndef
					//undef
					else if (substring.find("ifdef") != string::npos ||
							 substring.find("ifndef") != string::npos ||
							 substring.find("undef") != string::npos) {
						++aResult.num_def_conditionals;
					}
					//else
					//elif
					//endif
					//if
					else if (substring.find("else") != string::npos ||
							 substring.find("elif") != string::npos ||
							 substring.find("endif") != string::npos ||
							 substring.find("if") != string::npos) {
						++aResult.num_conditionals;
					}
					//unrecognized preprocessor command
					else {
						continue;
					}
				}
				//non-preprocessor line
				else {
					continue;
				}
			}
		}
	}
}

struct KeywordsResult {
	int type_num = 0;
	int conditional_num = 0;
	int loop_num = 0;
	int structure_num = 0;
	int member_num = 0;
	int namespace_num = 0;
	int cast_num = 0;
	int memory_num = 0;
	int exception_num = 0;
	int misc_num = 0;
	int asm_num = 0;
	int goto_num = 0;
	int mutable_num = 0;
	int union_num = 0;
};

void identifyKeyword(string const& aKeyword, KeywordsResult& aResult) {
	if (
		aKeyword == "auto"		||
		aKeyword == "bool"		||
		aKeyword == "char"		||
		aKeyword == "char8_t"	||
		aKeyword == "char16_t"	||
		aKeyword == "char32_t"	||
		aKeyword == "const"		||
		aKeyword == "double"	||
		aKeyword == "extern"	||
		aKeyword == "float"		||
		aKeyword == "inline"	||
		aKeyword == "int"		||
		aKeyword == "long"		||
		aKeyword == "noexcept"	||
		aKeyword == "short"		||
		aKeyword == "static"	||
		aKeyword == "unsigned"	||
		aKeyword == "volatile"	||
		aKeyword == "wchar_t"	) {
		++aResult.type_num;
	}
	else if (
		aKeyword == "case"		||
		aKeyword == "default"	||
		aKeyword == "if"		||
		aKeyword == "else"		||
		aKeyword == "switch"	) {
		++aResult.conditional_num;
	}
	else if (
		aKeyword == "do"		||
		aKeyword == "for"		||
		aKeyword == "while"		) {
		++aResult.loop_num;
	}
	else if (
		aKeyword == "class"		||
		aKeyword == "enum"		||
		aKeyword == "struct"	) {
		++aResult.structure_num;
	}
	else if (
		aKeyword == "explicit"	||
		aKeyword == "friend"	||
		aKeyword == "operator"	||
		aKeyword == "private"	||
		aKeyword == "protected" ||
		aKeyword == "public"	) {
		++aResult.member_num;
	}
	else if (
		aKeyword == "namespace"	||
		aKeyword == "using"		) {
		++aResult.namespace_num;
	}
	else if (
		aKeyword == "const_cast"		||
		aKeyword == "dynamic_cast"		||
		aKeyword == "reinterpret_cast"	||
		aKeyword == "static_cast"		) {
		++aResult.cast_num;
	}
	else if (
		aKeyword == "delete"	||
		aKeyword == "new"		) {
		++aResult.memory_num;
	}
	else if (
		aKeyword == "catch"		||
		aKeyword == "throw"		||
		aKeyword == "try"		) {
		++aResult.exception_num;
	}
	else if (
		aKeyword == "constexpr"	||
		aKeyword == "return"	||
		aKeyword == "sizeof"	||
		aKeyword == "template"	) {
		++aResult.misc_num;
	}
	//special cases
	else if (aKeyword == "asm") {
		++aResult.misc_num;
		++aResult.asm_num;
	}
	else if (aKeyword == "goto") {
		++aResult.conditional_num;
		++aResult.goto_num;
	}
	else if (aKeyword == "mutable") {
		++aResult.type_num;
		++aResult.mutable_num;
	}
	else if (aKeyword == "union") {
		++aResult.structure_num;
		++aResult.union_num;
	}
}

void analyzeKeywords(vector<string> const& aInput, KeywordsResult& aResult) {
	for (auto line : aInput) {
		//tokenize line
		char cline[1024] = { 0 };
		strcpy(cline, line.c_str());
		char const* delimiters = " \t\r(){}<>;=-+*/!%^&|:,.?~";
		vector<string> tokens;
		char * token = strtok(cline, delimiters);
		while(token != nullptr)
		{
			tokens.push_back(token);
			token = strtok(nullptr, delimiters);
		}
		//identify keywords in tokens
		for(auto const word : tokens)
		{
			identifyKeyword(word, aResult);
		}
	}
}

int main()
{
	//read a file
	string line;
	vector<string> source;
	vector<string> destination;
	string inFileName = "driver1.cpp";
	ifstream myfile(inFileName);
	if (!myfile.is_open()) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}
	while (getline(myfile, line))
	{
		source.push_back(line);
	}
	myfile.close();

	//remove comments
	destination = removeComments(source);
	int commentsLines = source.size() - destination.size();
	int codeLines = destination.size();

	//analyze preprocessor commands
	PreprocessResult preprocess_result;
	analyzePreprocess(destination, preprocess_result);

	//analyze keywords
	KeywordsResult keywords_result;
	analyzeKeywords(destination, keywords_result);

	//write code to "out.txt"
	ofstream outfile("output.txt");
	if (outfile.is_open())
	{
		for(auto line:destination)
			outfile << line<<"\n";
		outfile.close();
	}
	else cout << "Unable to open file";

	//write statistic data to "data.txt"
	ofstream datafile("data.txt");
	if (datafile.is_open())
	{
		datafile <<setw(30)<<"fileName"<< setw(30) <<"code"<< setw(30) <<"comments"<<
			setw(30) << "includes" << 
			setw(30) << "defines" << 
			setw(30) << "pragmas" << 
			setw(30) << "define_conditionals" << 
			setw(30) << "preprocess_conditionals" <<
			setw(30) << "types" << 
			setw(30) << "conditionals" << 
			setw(30) << "loops" << 
			setw(30) << "structures" << 
			setw(30) << "members" << 
			setw(30) << "namespaces" << 
			setw(30) << "casts" << 
			setw(30) << "memorys" << 
			setw(30) << "exceptions" << 
			setw(30) << "miscs" << 
			setw(30) << "asms" << 
			setw(30) << "gotos" << 
			setw(30) << "mutables" << 
			setw(30) << "unions" <<
			"\n";
		datafile << setw(30)<< inFileName << setw(30) << codeLines << setw(30) << commentsLines <<
			setw(30) << preprocess_result.num_includes				<< 
			setw(30) << preprocess_result.num_defines				<< 
			setw(30) << preprocess_result.num_pragmas				<< 
			setw(30) << preprocess_result.num_def_conditionals		<<
			setw(30) << preprocess_result.num_conditionals			<<
			setw(30) << keywords_result.type_num					<<
			setw(30) << keywords_result.conditional_num				<<
			setw(30) << keywords_result.loop_num					<<
			setw(30) << keywords_result.structure_num				<<
			setw(30) << keywords_result.member_num					<<
			setw(30) << keywords_result.namespace_num				<<
			setw(30) << keywords_result.cast_num					<<
			setw(30) << keywords_result.memory_num					<<
			setw(30) << keywords_result.exception_num				<<
			setw(30) << keywords_result.misc_num					<<
			setw(30) << keywords_result.asm_num						<<
			setw(30) << keywords_result.goto_num					<<
			setw(30) << keywords_result.mutable_num					<<
			setw(30) << keywords_result.union_num					<<
			"\n";
		datafile.close();
	}
	else cout << "Unable to open file";

	//write statistic data to "data.csv"
	ofstream datafile_csv("data.csv");
	if (datafile_csv.is_open())
	{
		datafile_csv << "fileName" << "," << "code" << "," << "comments" <<
			"," << "includes" <<
			"," << "defines" <<
			"," << "pragmas" <<
			"," << "define_conditionals" << 
			"," << "preprocess_conditionals" <<
			"," << "types" << 
			"," << "conditionals" << 
			"," << "loops" << 
			"," << "structures" << 
			"," << "members" << 
			"," << "namespaces" << 
			"," << "casts" << 
			"," << "memorys" << 
			"," << "exceptions" << 
			"," << "miscs" << 
			"," << "asms" <<
			"," << "gotos" << 
			"," << "mutables" << 
			"," << "unions" <<
			"\n";
		datafile_csv << inFileName << "," << codeLines << "," << commentsLines <<
			"," << preprocess_result.num_includes <<
			"," << preprocess_result.num_defines <<
			"," << preprocess_result.num_pragmas <<
			"," << preprocess_result.num_def_conditionals <<
			"," << preprocess_result.num_conditionals <<
			"," << keywords_result.type_num <<
			"," << keywords_result.conditional_num <<
			"," << keywords_result.loop_num <<
			"," << keywords_result.structure_num <<
			"," << keywords_result.member_num <<
			"," << keywords_result.namespace_num <<
			"," << keywords_result.cast_num <<
			"," << keywords_result.memory_num <<
			"," << keywords_result.exception_num <<
			"," << keywords_result.misc_num <<
			"," << keywords_result.asm_num <<
			"," << keywords_result.goto_num <<
			"," << keywords_result.mutable_num <<
			"," << keywords_result.union_num <<
			"\n";
			datafile_csv.close();
	}
	else cout << "Unable to open file";

	return 0;
}