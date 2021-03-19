#pragma once
#include <string>
#include <iostream>
#include <vector>

class Censor {
	public:
		std::vector<std::string> foul_language;
		Censor();
		std::string check_censor(std::string s);
		std::string censor_line(std::string s);
		std::string censor_word(std::string s);
};