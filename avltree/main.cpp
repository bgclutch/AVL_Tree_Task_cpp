#include "avl_tree.hpp"
#include <iostream>

int main() {
    avl::avl_tree<int> tree;
    int tmp;

    while (std::cin >> tmp) {
        tree.insert(tmp);
    }

    tree.printTree();

    return EXIT_SUCCESS;
}