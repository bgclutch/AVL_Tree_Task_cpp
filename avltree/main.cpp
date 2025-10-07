#include "avl_tree.hpp"
#include <iostream>

int main() {
    avl::avl_tree<int> tree;
    int tmp;

    while (std::cin >> tmp) {
        tree.insert(tmp);
    }

    tree.printTree();

    std::cout << tree.range_queries(3, 9) << std::endl;

    return EXIT_SUCCESS;
}