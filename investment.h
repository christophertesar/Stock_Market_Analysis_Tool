#ifndef INVESTMENT_H
#define INVESTMENT_H

#include <list>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <functional>


using namespace std;

class investment{
public:
	investment();
    ~investment();
	
	void setup_investment();
	void read_cv();
	
    void setName(std::string);
	std::string getName();

private:
	std::string name;
	//ofstream histFile;
};

#endif
