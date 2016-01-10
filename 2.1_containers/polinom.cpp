#include <string>
#include <regex>
#include <map>
#include <iostream>
#include <assert.h>

class Polinom {
	std::map<int,int> view;

    	void ParseExpr(const std::string& expr) {
		try {
			std::regex re("(-|\\+)?(\\d*)\\*?(x)?\\^?(\\d*)");
			auto it  = std::sregex_token_iterator(expr.begin(), expr.end(), re, {1, 2, 3, 4});
			auto it_end = std::sregex_token_iterator();
			for (; it != it_end; ) {
				int sign = it->str() == "-" ? -1 : 1; ++it;
				std::string coeff = it->str(); ++it;
				std::string var   = it->str(); ++it;
				std::string pow   = it->str(); ++it;
				if (coeff.empty() && var.empty())
					break;
				
				int c = sign * (coeff.empty() ? 1 : std::stoi(coeff));
				int p = (pow.empty() ? (var.empty() ? 0 : 1) : std::stoi(pow));
				
				view[p] += c;
			}
		} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		//for (auto it = view.begin(); it != view.end(); ++it) {
		//	std::cout << it->first << "; " << it->second << std::endl;
		//}
    	}

public:
	std::string getDerivative() {
        	std::string derivative;
		for(auto it = view.rbegin(); it != view.rend(); ++it) {
			int new_coeff = it->first * it->second;
			if (new_coeff == 0) 
				continue;
			int new_pow = it->first > 0 ? it->first - 1 : 0;
			derivative += new_coeff > 0 && !derivative.empty() ? "+" : "";
			derivative += new_coeff == 1 ? (new_pow == 0 ? "1" : "") : 
				                       (new_pow > 0 ? std::to_string(new_coeff) + "*" : std::to_string(new_coeff));
			derivative += new_pow == 0 ? "" : (new_pow == 1 ? "x" : std::string("x^") + std::to_string(new_pow));
		}
		//std::cout << derivative << std::endl;
		return derivative;
	}

    
    	Polinom(const std::string& expr) {
		ParseExpr(expr);
    	}

};


std::string derivative(std::string expr) {
    Polinom polinom(expr);
    return polinom.getDerivative();
}

int main(int argc, char** argv) {
	assert(derivative("2*x^100+100*x^2") == "200*x^99+200*x");
	assert(derivative("-3*x^2-2*x") == "-6*x-2");
	assert(derivative("x+x+x+x+x+x+x+x+x+x") == "10");
	assert(derivative("2*x+1") == "2");
	assert(derivative("x^2+x") == "2*x+1");
	assert(derivative("x^10000+x+1") == "10000*x^9999+1");
	assert(derivative("-x^2-x^3") == "-3*x^2-2*x");

	
	return 0;
}
