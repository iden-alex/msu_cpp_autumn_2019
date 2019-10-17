#include <iostream>
#include <string.h>

bool is_oper(char c) {
    switch (c){
        case '+':
        case '-':
        case '/':
        case '*':
            return true;
        default:
            return false;
    }
}
int number(std::string &str) {
    int a = 0;
    int decade = 1;
    while(!str.empty()) {
        char c = str.back();
        if (c >= '0' && c <= '9') {
            a += (c - '0')*decade;
            decade *= 10;
            str.pop_back();
        } else if (c == '-' &&(str.size() == 1 || is_oper(str[str.size()-2]))) {
            a = -a;
            str.pop_back();
        } else if(is_oper(c)){
            break;
        } else {
            throw "error";
        }
    }
    return a;
}

int mul(std::string &str) {
    int a = number(str);
    if(!str.empty()) {
        switch(str.back()) {
            case '*':
            {
                str.pop_back();
                a *= mul(str);
                break;
            }
            case '/':
            {
                str.pop_back();
                if (a == 0) {
                    throw "error";
                }
                a = mul(str) / a;
                break;
            }
            default:
            {
                return a;
            }
        }
    }
    return a;
}

int add(std::string &str) {
    int a = mul(str);
    if(!str.empty()) {
        switch(str.back()) {
            case '+': 
            {
                str.pop_back();
                a += add(str);
                break;
            }
            case '-': 
            {
                str.pop_back();
                a = add(str) - a;
                break;
            }
            default: 
            {
                return a;    
            }
        }
    }
    return a;
}

int main(int argc, char **argv) {
    char *row_str = argv[1];
    if (argc != 2 || row_str == 0) {
        std::cerr << "error" << std::endl;
        return 1;
    }
    std::string str = "";
    int i = 0;
    while(row_str[i] != '\0') {
        if (!isspace(row_str[i])) {
            str.push_back(row_str[i]);
        }
        i++;
    }
    if (str.empty()) {
        std::cerr << "error" << std::endl;
        return 1;
    }
    try {
        std::cout << add(str) << std::endl;
    } catch (...) {
        std::cerr << "error" << std::endl;
        return 1;
    }
    return 0;
}
