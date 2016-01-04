#include <iostream>


class StringPointer {
    std::string def;
    std::string* p;
    
public:
    StringPointer(std::string* p): p(p) {
    }
    
    const std::string* operator->() const {
        return !p ? &def : p;
    }
    
    const std::string& operator* () const {
        return !p ? def : *p;
    }
};

int main(int argc, char** argv) {
	std::string s1 = "Hello, world!";

    StringPointer sp1(&s1);
    StringPointer sp2(NULL);

    std::cout << sp1->length() << std::endl;
    std::cout << *sp1 << std::endl;
    std::cout << sp2->length() << std::endl;
    std::cout << *sp2 << std::endl;
    
    return 0;
}
