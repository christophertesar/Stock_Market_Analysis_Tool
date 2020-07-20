#include "Stock.h"
#include "gnuplot-cpp/gnuplot_i.hpp" //Gnuplot class handles POSIX-Pipe-communikation with Gnuplot

stock::stock(){

}

stock::~stock(){ //New not called

}

stock::stock(const stock &s){
	
}

void stock::read_cv(std::string filename){
	std::cout << "Reading " << filename << std::endl; 
	std::ifstream file;
	file.open(filename,std::ifstream::in);
	std::string openVar;
	std::string closeVar;
	
	std::string temp;
	std::string line;
	
	if(!file.is_open()){
		std::cout << "Cannot open file." << std::endl;
		return;
	}

	while(std::getline(file, line)){ //Reads entire file
		std::stringstream ss(line);
		std::getline(ss, openVar, ',');
		std::getline(ss, closeVar, ',');

		open.push_back(std::stod(openVar)); //storing variables
		close.push_back(std::stod(closeVar));
	}
	file.close(); //finished
}

void stock::setup_investment(){
	calc_change();
	rsi();
	stochastic();
	set_days();
}

void stock::set_days(){
	for(int i = 0; i <= open.size(); i++){
		days.push_back(i+1);
	}
}

void stock::rsi(){
	
	for(int i = 0; i <= change.size() - 15; i++){
		double negSum = 0; //accumulates pos and neg over 14 day period
		double posSum = 0;
		double posChange = 0; //accumulates number of positive days and neg. days
		double negChange = 0;
		changeIt = change.begin() + i;
		
		for(int j = 0; j <= 13; j++){
			if(*changeIt < 0){
				negSum = negSum + *changeIt;
				negChange++;
				}
			else{
				posSum = posSum + *changeIt;
				posChange++;
				}
			changeIt++;
		}
		double RS = (posSum/posChange)/(-negSum/negChange);
		rsi_val.push_back(100-(100/(1+RS)));
		//std::cout << rsi_val.size() << std::endl;
		
	}
	
	//for(int i=0; i < rsi_val.size(); i++)
	//	std::cout << rsi_val.at(i) << std::endl;
}


void stock::stochastic(){
	double max;				//initialize variables
	double min;
	double stochVal;
	double closeDay;
	
	for(int i = 0; i <= close.size() - 14; i++){                                          //int i for index from close.begin() up to close vector size minus 14
		closeIt = std::max_element(close.begin() + i, close.begin() + i + 14);
		max = *closeIt;
		closeIt = std::min_element(close.begin() + i, close.begin() + i + 14);
		min = *closeIt;
		closeIt = close.begin() + i + 13;
		closeDay = *closeIt;
		stochVal = ((closeDay - min)/(max - min)) * 100 ;
		stoch.push_back(stochVal);
	}
}


void stock::calc_change(){

	change.resize(open.size());
	std::transform(close.begin(),close.end(),open.begin(),change.begin(),std::minus<double>()); //inputs subtract results into vector change

		
}

//Precondition:          Postcondition:Correct results calculated, Buy sell hold suggestions generated, RSI and Stoch. values calculated.
void stock::initiate_suggestion_system(){
	
	gen_suggestions();
	compare_suggestions();
	get_trend(correct,wrong);

}

//Precondition: csv file has been read and has open/close values. //Postcondition: RSI and Stoch. values are calculated.
void stock::gen_suggestions(){

	stochIt = stoch.begin();
	rsi_valIt = rsi_val.begin();
	
	for(int i = 0; i < stoch.size(); i++){
		stochIt = stoch.begin() + i;
		rsi_valIt = rsi_val.begin() + i;
		//std::cout << *stochIt << std::endl;
		//std::cout << *rsi_valIt << std::endl;
		
		if ((*rsi_valIt <= 40 && *stochIt >= 70) ||(*rsi_valIt >= 60 && *stochIt <= 30)){ //check if one is buy and one is sell first to give hold rating.
			suggestions.push_back(0);
		}
		else if(*rsi_valIt <= 45 || *stochIt <= 30){
			suggestions.push_back(1);                 //push back 1 which means buy
		}
		else if(*rsi_valIt >= 55 || *stochIt >= 70){
			suggestions.push_back(-1);                //push back -1 which means sell
		}
		else{
			suggestions.push_back(0);                 //push back 0 which means hold
		}
	}
	//std::cout << suggestions.size() << std::endl;
	//for(int i: suggestions){
	//	std::cout << i << std::endl;
	//}
}

//Precondition: csv file has been read        Postcondition:Buy and Sell suggestions have been compared.
void stock::compare_suggestions(){
	
	suggestionsIt = suggestions.begin(); //starts at day n or day 14
	changeIt = change.begin() + 14; //+14 looks day 14 + 1 = 15 which is day n+1
	
	for(int i = 0; i < suggestions.size(); i++){
		if((*suggestionsIt == -1) && (*changeIt < 0)){
			correct++;
		}
		else if((*suggestionsIt == 1) && (*changeIt > 0)){
			correct++;
		}
		else if((*suggestionsIt == 1) && (*changeIt < 0)){
			wrong++;
		}
		else if((*suggestionsIt == -1) && (*changeIt > 0)){
			wrong++;
		}
		suggestionsIt++;
		changeIt++;
	}
}
//Precondition: suggestions have been compared.   //Postconditions: Percentages have been calculated and output into terminal.
void stock::get_trend(int correct, int wrong){

	double correctD = correct;
	double wrongD = wrong;
	std::cout << "\n" << "Printing Results:\n" << std::endl;
	double hold = suggestions.size() - correctD - wrongD;
	double size = suggestions.size();

	trend_prediction_wHold = (correctD/size) * 100;
	trend_prediction_cHold = ((correctD+hold)/size) * 100;
	trend_prediction_correctWrong = (correctD/wrongD);

	std::cout << "Percent of correct predictions over wrong predictions with hold counted as wrong: ";
	std::cout << trend_prediction_wHold << "%" << "\n" << std::endl;
	
	std::cout << "Trend of correct predictions over wrong predictions with hold counted as correct: ";
	std::cout << trend_prediction_cHold << "%" << "\n" << std::endl;
	
	std::cout << "Ratio of correct predictions vs wrong predictions: ";
	std::cout << trend_prediction_correctWrong << "\n" << std::endl;
	
}

//Precondition: Stochastic and RSI values have been calculated.   Postcondition: Graphs have been output onto screen.
void stock::plot_stock(){
	
	try{
		
		Gnuplot s("Points");
		s.set_title("Stochastic Plot");
		s.set_ylabel("Stochastic (%)");
		s.set_xlabel("Time (Days)");
		
		s.plot_x(stoch,"Points");
		s.set_smooth().plot_x(stoch,"cspline");
		s.unset_smooth();

		
		Gnuplot r("lines");
		r.set_title("RSI Plot");
		r.set_ylabel("RSI (%)");
		r.set_xlabel("Time (Days)");	
		r.plot_x(rsi_val,"RSI Values");
		r.showonscreen();
		
		Gnuplot p("Points");
		p.set_title("Stock Price Plot");
		p.set_ylabel("Price ($USD)");
		p.set_xlabel("Time (Days)");
		p.plot_x(close,"points");
		p.set_smooth().plot_x(close,"cspline");
		
		wait_for_key();
	}
	catch(GnuplotException ge){
		std::cout << ge.what() << std::endl;
	}
	

}

void stock::wait_for_key()
{
    cout << endl << "Press ENTER to continue..." << endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();

    return;
}
























