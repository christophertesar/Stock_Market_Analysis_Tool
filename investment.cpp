#include "investment.h"

investment::investment(){

}

investment::~investment(){

}

void investment::setName(std::string str){
	name = str;
}

std::string investment::getName(){
	return name;
}

//void investment::read_cv(){};
