#include "avl_tree.hpp"
#include <iostream>

int main() {
    avl::avl_tree<int> tree;
    char request;

    while (std::cin >> request) {
        if (request == avl::key_request) {
            int newKey;
            if (!(std::cin >> newKey)) {
                std::cerr << "WRONG GIVEN KEY\n";
                return EXIT_FAILURE;
            }
            tree.insert(newKey);
        }
        else if (request == avl::query_request) {
            int first, second;
            if (!(std::cin >> first >> second)) {
                std::cerr << "WRONG GIVEN BOUNDS\n";
                return EXIT_FAILURE;
            }
            std::cout << tree.range_queries(first, second) << " ";
        }
        else {
            std::cerr << "WRONG REQUEST -> " << request << "\n";
            return EXIT_FAILURE;
        }
    }

    std::cout << std::endl;

    return EXIT_SUCCESS;
}
