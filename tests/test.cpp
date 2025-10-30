#include <gtest/gtest.h>

#include "avl_tree.hpp"

TEST(AVL_TREE_FUNCTIONS, range_query_1) {
    avl::avl_tree<int> tree;

    tree.insert(8);
    tree.insert(10);
    tree.insert(1);

    size_t count = avl::distance(tree.lower_bound(0), tree.upper_bound(9));
    ASSERT_EQ(count, 2);
}

TEST(AVL_TREE_FUNCTIONS, range_query_2) {
    avl::avl_tree<int> tree;

    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);

    size_t count = avl::distance(tree.lower_bound(-2), tree.upper_bound(10));
    ASSERT_EQ(count, 3);
}

TEST(AVL_TREE_FUNCTIONS, range_query_3) {
    avl::avl_tree<int> tree;

    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);

    size_t count = avl::distance(tree.lower_bound(-2), tree.upper_bound(10));
    ASSERT_EQ(count, 3);
}

TEST(AVL_TREE_FUNCTIONS, range_query_4) {
    avl::avl_tree<int> tree;

    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);

    size_t count = avl::distance(tree.lower_bound(-2), tree.upper_bound(10));
    ASSERT_EQ(count, 5);
}

TEST(AVL_TREE_FUNCTIONS, range_query_5) {
    avl::avl_tree<int> tree;

    tree.insert(9);
    tree.insert(-3);
    tree.insert(79);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);

    size_t count = avl::distance(tree.lower_bound(2), tree.upper_bound(10));
    ASSERT_EQ(count, 4);
}

TEST(AVL_TREE_FUNCTIONS, copy_ctor) {
    avl::avl_tree<int> tree;

    tree.insert(9);
    tree.insert(-3);
    tree.insert(79);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);

    avl::avl_tree<int> copy_tree{tree};

    size_t count = avl::distance(copy_tree.lower_bound(2), copy_tree.upper_bound(10));
    ASSERT_EQ(count, 4);
}

TEST(AVL_TREE_FUNCTIONS, move_ctor) {
    avl::avl_tree<int> tree;

    tree.insert(9);
    tree.insert(-3);
    tree.insert(79);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);

    avl::avl_tree<int> copy_tree{std::move(tree)};

    size_t count = avl::distance(copy_tree.lower_bound(2), copy_tree.upper_bound(10));
    ASSERT_EQ(count, 4);
}

TEST(AVL_TREE_FUNCTIONS, copy_assigment) {
    avl::avl_tree<int> tree;

    tree.insert(9);
    tree.insert(-3);
    tree.insert(79);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);

    avl::avl_tree<int> copy_tree;

    copy_tree = tree;
    size_t count = avl::distance(copy_tree.lower_bound(2), copy_tree.upper_bound(10));
    ASSERT_EQ(count, 4);
}

TEST(AVL_TREE_FUNCTIONS, move_assigment) {
    avl::avl_tree<int> tree;

    tree.insert(9);
    tree.insert(-3);
    tree.insert(79);
    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(8);

    avl::avl_tree<int> copy_tree;

    copy_tree = std::move(tree);
    size_t count = avl::distance(copy_tree.lower_bound(2), copy_tree.upper_bound(10));
    ASSERT_EQ(count, 4);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}