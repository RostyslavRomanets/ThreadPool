#include <iostream>
#include <vector>

#include "SimpleThreadPool.h"

int main() {
    SimpleThreadPool threadPool(2);

    std::vector<int> v { 2, 5, 6, 7, 8, 9 };

    auto taskSum = [&]() {
        int res = 0;
        for (auto i : v)
            res += i;

        return res;
    };

    auto taskMult = [&]() {
        int res = 1;
        for (auto i : v)
            res *= i;

        return res;
    };

    auto sum = threadPool.Post(taskSum);
    auto mult = threadPool.Post(taskMult);

    auto res1 = sum.get();
    auto res2 = mult.get();

    std::cout << res1 <<  " " << res2<< std::endl;
}
