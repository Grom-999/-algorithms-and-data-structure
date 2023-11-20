#include <string>
#include <sstream>
#include <regex>
#include <iostream>

std::string split(std::string& str) {
    long long int sum_temp = 0;
    static const std::regex regular("[^-?\\d+]");
 
    str = std::regex_replace(str, regular, "");

    return str;
}

int main() {
    long long int a = 0;
    std::string line;

    while (std::getline(std::cin, line)) {

       line =  split(line);

    }

    std::cout << a;

    return 0;
}