
class Polinom {
    std::map<int,int> view;
    
public:
    Polinom(const std::string& str) {
        
        for(auto it = str.begin(); it != str.end();) {
            if (*it == '^') {
                    
            }
        }
    }
    
    std::string getDerivative() {
        return std::accumulate(view.rbegin(), view.rend(), std::string{}, 
            [](const std::string& deriv, auto& p){ 
                int coeff = p.first*p.second, pow = p.first-1;
                if (coeff == 0) return deriv;
                deriv += coeff == 1 ? std::string{} : std::to_string(coeff);
                deriv += pow == 0 ? std::to_string(coeff) : 
                    (pow == 1 ? ("x" : "x^" + std::to_string(pow));
            });
    }
    
};


std::string derivative(std::string polynomial) {
    Polinom polinom(polynomial);
    
    return (std::string)"";
}
