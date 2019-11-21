#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

template <class T>
std::string my_to_string(const T &a) {
    std::stringstream tmp;
    tmp << a;
    std::string res;
    tmp >> res;
    return res;
}

template <class... ArgsType>
std::string format(const std::string &begin_str, ArgsType ... args) {
    std::vector <std::string> samples = {my_to_string(std::forward<ArgsType> (args))...};
    std::string res;
    size_t i = 0;
    while(i < begin_str.length()) {
        if (begin_str[i] == '{') {
            i++;
            if (begin_str[i] == '}' || begin_str[i] == '{') {
                throw std::runtime_error("Invalid string");
            }
            std::string sample_num;
            while (begin_str[i] != '}') {
                sample_num += begin_str[i];
                i++;
                if (i >= begin_str.length()) {
                    throw std::runtime_error("Invalid index");
                }
            }
            for (auto c: sample_num) {
                if (!isdigit(c)) {
                    throw std::runtime_error("Invalid index");
                }
            }
            size_t j = 0;
            try {
                j = std::stoul(sample_num);
            } catch(std::exception &ex) {
                throw std::runtime_error("Invalid index");
            }
            if (j < samples.size()) {
                res += samples[j];
                sample_num.clear();
            } else {
                throw std::runtime_error("Invalid index");
            }
        } else if (begin_str[i] == '}') {
            throw std::runtime_error("Invalid string");
        } else {
            res += begin_str[i];
        }
        i++;
    }
    return res;
}
 
