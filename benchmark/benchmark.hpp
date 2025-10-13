#pragma once

#include <iostream>
#include <fstream>
#include <chrono>

namespace benchmark {

const char key_request   = 'k';
const char query_request = 'q';

template <typename KeyType>
struct Request {
    char request;
    KeyType first;
    KeyType second;
    Request(char request, KeyType first, KeyType second = 0) :
            request(request), first(first), second(second) {}
};

template <typename KeyType, typename VecType>
int getBenchmarkData(VecType& data, std::ifstream& input_data) {
    char request;
    while (input_data >> request) {
        if (request == key_request) {
            KeyType newKey;
            if (!(input_data >> newKey)) {
                std::cerr << "WRONG GIVEN KEY\n";
                return EXIT_FAILURE;
            }
            data.emplace_back(request, newKey);
        }
        else if (request == query_request) {
            KeyType first, second;
            if (!(input_data >> first >> second)) {
                std::cerr << "WRONG GIVEN BOUNDS\n";
                return EXIT_FAILURE;
            }
            data.emplace_back(request, first, second);
        }
        else {
            std::cerr << "WRONG REQUEST -> " << request << "\n";
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

template <typename SetType, typename KeyType>
size_t set_range_queries(const SetType& set, const KeyType& first, const KeyType& second) {
        if (first > second)
            return 0;

        auto lower = set.lower_bound(first);
        auto upper = set.upper_bound(second);

        return std::distance(lower, upper);
}

template <typename TreeType, typename VecType>
auto runAvl(TreeType& tree, const VecType& data){
    volatile size_t dummy = 0;
    auto result = std::chrono::microseconds::zero();
    for (const auto& req : data) {
        auto begin = std::chrono::high_resolution_clock::now();
        if (req.request == key_request) {
            tree.insert(req.first);
        }
        else if (req.request == query_request) {
            dummy = tree.range_queries(req.first, req.second);
        }
        auto end = std::chrono::high_resolution_clock::now();
        result += std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    }

    return result;
}

template <typename TreeType, typename VecType>
auto runSet(TreeType& tree, const VecType& data){
    volatile size_t dummy = 0;
    auto result = std::chrono::microseconds::zero();
    for (const auto& req : data) {
        auto begin = std::chrono::high_resolution_clock::now();
        if (req.request == key_request) {
            tree.insert(req.first);
        }
        else if (req.request == query_request) {
            dummy = set_range_queries(tree, req.first, req.second);
        }
        auto end = std::chrono::high_resolution_clock::now();
        result += std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    }

    return result;
}
} // namespace benchmark