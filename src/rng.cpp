#include "rng.hpp"
namespace snek {
    int generate_random_number(int start, int end) {
        static std::random_device dev;
        static std::mt19937 rng(dev());
        std::uniform_int_distribution<int> dist(start, end);
        return dist(rng);
    }
}