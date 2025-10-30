#include "avl_tree.hpp"
#include "benchmark.hpp"
#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <chrono>

int main(int argc, char** argv) {
    std::ifstream input_data;

    if (argc > 1) {
        input_data.open(argv[1]);
        std::cout << "benchmark data loaded from " << argv[1] << std::endl;
    }
    else {
        input_data.open("benchmark/default_benchmark.in");
        std::cout << "default benchmark data loaded" << std::endl;
    }

    if (!input_data.is_open()) {
        std::cerr << "Error opening input_data\n";
        return EXIT_FAILURE;
    }

//-------------------------------avl tree benchmark------------------------------------//
    using input_vector = std::vector<benchmark::Request<int>>;
    input_vector data;

    int getBenchDataRes = benchmark::getBenchmarkData<int, input_vector>(data, input_data);
    if (getBenchDataRes != 0)
        return EXIT_FAILURE;

    avl::avl_tree<int> avltree;
    auto avl_result = benchmark::runTree<avl::avl_tree<int>, input_vector>(avltree, data);
    volatile size_t avl_bench = avl_result.count();
//-------------------------------std::set benchmark------------------------------------//
    std::set<int> settree;
    auto set_result = benchmark::runTree<std::set<int>, input_vector>(settree, data);
    volatile size_t set_bench = set_result.count();

    return EXIT_SUCCESS;
}