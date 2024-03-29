#include <iostream>
#include <vector>

#include "SimpleThreadPool.h"

int main() {
    std::vector<int> v { 2, 5, 6, 7, 8, 9 };

    auto taskSum = [&]() {
        int res = 0;
        for (auto i : v)
            res += i;

        std::this_thread::sleep_for(std::chrono::seconds(1));
        return res;
    };

    auto taskMult = [&]() {
        int res = 1;
        for (auto i : v)
            res *= i;

        std::this_thread::sleep_for(std::chrono::seconds(1));
        return res;
    };

    try
    {
        SimpleThreadPool threadPool(2);

        auto sum = threadPool.Post(taskSum);
        auto mult = threadPool.Post(taskMult);
        auto sum2 = threadPool.Post(taskSum);

        auto res1 = sum.get();
        auto res2 = mult.get();
        std::cout << res1 << " " << res2 << std::endl;

        auto res3 = sum2.get();
        std::cout << res3 << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << "Exception: " << ex.what();
    }
}
