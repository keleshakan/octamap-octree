#include <iostream>
#include <iomanip>
#include <map>
#include <random>
int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // if an event occurs 4 times a minute on average
    // how often is it that it occurs n times in one minute?
    std::poisson_distribution<> d(50);

    std::map<int, int> hist;
    for(int n=0; n<4; ++n) {
        ++hist[d(gen)];
    }

    static double total_x = 0;
    double some_number ;

    for (auto p : hist) {
        total_x += p.first * p.second;
    }
    some_number = total_x / 4;

    std::cout << some_number << "\n\n";

    std::cout << hist.size() << "\n\n";

    for(auto p : hist) {
        std::cout << p.first <<
                  ' ' << p.second << '\n';
    }
}
