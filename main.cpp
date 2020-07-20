#include "Stock.h"

int main(){
	std::string tesla = "tesla.csv";
	std::string nvidia = "nvda.csv";
	std::string amds = "amd.csv";
	
	stock nvda, amd, tsla;               		//Create stock objects
	std::list<stock> stockList;			 		//Create list of stocks
	
	stockList.push_back(nvda);           //Insert stock objects into list (tsla,amd,nvda)
	stockList.push_back(amd);			 //Note: push_back copied objects
	stockList.push_back(tsla);			 
	std::list<stock>::iterator it = stockList.begin(); //Instantiate stockList iterator
	
	it->read_cv(tesla);
	it->setup_investment();
	it->initiate_suggestion_system();
	it->plot_stock();
	
	it++;
	
	it->read_cv(amds);
	it->setup_investment();
	it->initiate_suggestion_system();
	it->plot_stock();
	
	it++;
	
	it->read_cv(nvidia);
	it->setup_investment();
	it->initiate_suggestion_system();
	it->plot_stock();
	
	
return 0;
}
