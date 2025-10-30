#include "avl_tree.hpp"
#include <iostream>
#include <limits>

void clearInput();

int main() {
    avl::avl_tree<int> tree;
    char request;

    while (std::cin >> request) {
        if (request == avl::key_request) {
            int newKey;
            while (!(std::cin >> newKey)) {
                std::cerr << "WRONG GIVEN KEY -> " << newKey << "\n";
                clearInput();

            }
            tree.insert(newKey);
        }
        else if (request == avl::query_request) {
            int first, second;
            while (!(std::cin >> first >> second)) {
                std::cerr << "WRONG GIVEN BOUNDS-> (" << first << ", " << second << "]\n";
                clearInput();
            }
            std::cout << tree.range_queries(first, second) << " ";
        }
        else {
            std::cerr << "WRONG REQUEST -> " << request << "\n";
            clearInput();
        }
    }

    std::cout << std::endl;

    return EXIT_SUCCESS;
}

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
