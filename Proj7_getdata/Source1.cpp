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
		datafile <<setw(20)<<"fileName"<< setw(20) <<"code"<< setw(20) <<"comments"<<"\n";
		datafile <<setw(20)<< inFileName << setw(20) << codeLines << setw(20) << commentsLines << "\n";
		datafile.close();
	}
	else cout << "Unable to open file";



	return 0;
}