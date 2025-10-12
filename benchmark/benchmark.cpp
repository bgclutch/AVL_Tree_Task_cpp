#include "avl_tree.hpp"
#include <iostream>
#include <set>
#include <chrono>
#include <fstream>

namespace {
    const char key_request   = 'k';
    const char query_request = 'q';
} // anonymous namespace


template <typename SetType, typename KeyType>
size_t set_range_queries(const SetType& set, const KeyType& first, const KeyType& second) {
        if (first > second)
            return 0;

        auto lower = set.lower_bound(first);
        auto upper = set.upper_bound(second);

        return std::distance(lower, upper);
}

int main() { // correct input data guarantee
//-------------------------------avl tree benchmark------------------------------------//
    avl::avl_tree<int> avltree;
    char request;
    std::ifstream avl_input("tests/EndToEnd/input_files/test_01.in");
    std::ofstream avl_output("benchmark/avl_benchmark.out");

    if (!avl_input.is_open() || !avl_output.is_open()) {
        std::cerr << "Error opening avl input/output files\n";
        return EXIT_FAILURE;
    }

    auto avl_begin = std::chrono::high_resolution_clock::now();
    auto avl_end = avl_begin;
    auto avl_result = std::chrono::microseconds::zero();

    while (avl_input >> request) {
        if (request == avl::key_request) {
            int newKey;
            if (!(avl_input >> newKey)) {
                std::cerr << "WRONG GIVEN KEY\n";
                return EXIT_FAILURE;
            }
            avl_begin = std::chrono::high_resolution_clock::now();
            avltree.insert(newKey);
            avl_end = std::chrono::high_resolution_clock::now();
            avl_result += std::chrono::duration_cast<std::chrono::microseconds>(avl_end - avl_begin);
        }
        else if (request == avl::query_request) {
            int first, second;
            if (!(avl_input >> first >> second)) {
                std::cerr << "WRONG GIVEN BOUNDS\n";
                return EXIT_FAILURE;
            }
            avl_begin = std::chrono::high_resolution_clock::now();
            avl_output << avltree.range_queries(first, second) << " ";
            avl_end = std::chrono::high_resolution_clock::now();
            avl_result += std::chrono::duration_cast<std::chrono::microseconds>(avl_end - avl_begin);
        }
        else {
            std::cerr << "WRONG REQUEST -> " << request << "\n";
            return EXIT_FAILURE;
        }
    }

    std::cout << "avl result:" << avl_result.count() << std::endl;

//-------------------------------std::set benchmark------------------------------------//

    std::set<int> settree;
    std::ifstream set_input("tests/EndToEnd/input_files/test_01.in");
    std::ofstream set_output("benchmark/set_benchmark.out");
    if (!set_input.is_open() || !set_output.is_open()) {
        std::cerr << "Error opening set input/output files.\n";
        return EXIT_FAILURE;
    }

    auto set_begin = std::chrono::high_resolution_clock::now();
    auto set_end = set_begin;
    auto set_result = std::chrono::microseconds::zero();

    while (set_input >> request) {
        if (request == key_request) {
            int newKey;
            if (!(set_input >> newKey)) {
                std::cerr << "WRONG GIVEN KEY\n";
                return EXIT_FAILURE;
            }
            set_begin = std::chrono::high_resolution_clock::now();
            settree.insert(newKey);
            set_end = std::chrono::high_resolution_clock::now();
            set_result += std::chrono::duration_cast<std::chrono::microseconds>(set_end - set_begin);
        }
        else if (request == query_request) {
            int first, second;
            if (!(set_input >> first >> second)) {
                std::cerr << "WRONG GIVEN BOUNDS\n";
                return EXIT_FAILURE;
            }
            set_begin = std::chrono::high_resolution_clock::now();
            set_output << set_range_queries(settree, first, second) << " ";
            set_end = std::chrono::high_resolution_clock::now();
            set_result += std::chrono::duration_cast<std::chrono::microseconds>(set_end - set_begin);
        }
        else {
            std::cerr << "WRONG REQUEST -> " << request << "\n";
            return EXIT_FAILURE;
        }
    }

    std::cout << "set result:" << set_result.count() << std::endl;

    return EXIT_SUCCESS;
}