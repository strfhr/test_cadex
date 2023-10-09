#pragma once
#include<iostream>
#include<ctime>

struct Timer {
public:
    Timer(std::string&& print_text)
        : print_text_(print_text) {
            start = std::clock();
        }


    ~Timer() {
        std::cout << print_text_ << " " << std::clock() - start << " processor clock ticks." <<  std::endl;
    }

private:
    std::clock_t start;
    std::string print_text_;
};
