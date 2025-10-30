#pragma once

#include <iostream>
#include <fstream>
#include <set>
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

#if 0 // maybe this option would be better?
template <typename TreeType, typename KeyType>
size_t set_range_queries(const TreeType& tree, const KeyType& first, const KeyType& second) {
    if (first > second)
        return 0;

    if constexpr (std::is_same_v<TreeType, avl::avl_tree<KeyType>>) {
        return tree.range_queries(first, second);
    }
    else {
        auto lower = tree.lower_bound(first);
        auto upper = tree.upper_bound(second);
        return std::distance(lower, upper);
    }
}
#endif

template <typename KeyType>
size_t set_range_queries(const avl::avl_tree<KeyType>& tree, const KeyType& first, const KeyType& second) {
    if (first > second)
        return 0;

    return tree.range_queries(first, second);

}

template <typename KeyType>
size_t set_range_queries(const std::set<KeyType>& tree, const KeyType& first, const KeyType& second) {
    if (first > second)
        return 0;

    auto lower = tree.lower_bound(first);
    auto upper = tree.upper_bound(second);
    return std::distance(lower, upper);
}

template <typename TreeType, typename VecType>
auto runTree(TreeType& tree, const VecType& data) {
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