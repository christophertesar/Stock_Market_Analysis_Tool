#ifndef STOCK_H
#define STOCK_H

#include <vector>
#include "investment.h"

class stock: public investment{
public:
	stock();
    ~stock();
    stock(const stock&);
	void plot_stock();
	void initiate_suggestion_system();
	void read_cv(std::string);
	void setup_investment();
	

private:
	
	void wait_for_key();
	void set_days();
	void calc_change();
	void gen_suggestions();
	void rsi();
	void stochastic();
	void get_trend(int,int);
	void compare_suggestions();
	
	std::vector<double>::iterator openIt,closeIt,volumeIt,changeIt,rsi_valIt,stochIt;
	std::vector<double> open, close, volume, change, rsi_val, stoch;
	
	std::vector<int>::iterator suggestionsIt;
	std::vector<int> suggestions;
	int correct;				//number of correct suggestions
	int wrong; 					//number of incorrect suggestions
	bool overpriced, overbought;
	double trend_prediction;
	double trend_prediction_wHold; //prediction with hold counted as wrong prediction
	double trend_prediction_cHold; //prediction with hold counted as correct prediction
	double trend_prediction_correctWrong; //prediction with ratio over right over wrong
	
	std::vector<int> days;

};




#endif
