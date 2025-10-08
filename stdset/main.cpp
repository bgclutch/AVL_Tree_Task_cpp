#include <iostream>
#include <set>

namespace {
    const char key_request   = 'k';
    const char query_request = 'q';
} // anonymous namespace


template <typename SetType, typename KeyType>
size_t range_queries(const SetType& set, const KeyType& first, const KeyType& second) {
        if (first > second)
            return 0;

        auto lower = set.lower_bound(first);
        auto upper = set.upper_bound(second);

        return std::distance(lower, upper);
    }

int main() {
    std::set<int> tree;
    char request;

    while (std::cin >> request) {
        if (request == key_request) {
            int newKey;
            if (!(std::cin >> newKey)) {
                std::cerr << "WRONG GIVEN KEY\n";
                return EXIT_FAILURE;
            }
            tree.insert(newKey);
        }
        else if (request == query_request) {
            int first, second;
            if (!(std::cin >> first >> second)) {
                std::cerr << "WRONG GIVEN BOUNDS\n";
                return EXIT_FAILURE;
            }
           std::cout << range_queries(tree, first, second) << " ";
        }
        else {
            std::cerr << "WRONG REQUEST -> " << request << "\n";
            return EXIT_FAILURE;
        }
    }

    std::cout << std::endl;

    return EXIT_SUCCESS;
}