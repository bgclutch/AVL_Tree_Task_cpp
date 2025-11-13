#include <gtest/gtest.h>

#include "avl_tree.hpp"

TEST(AVL_TREE_FUNCTIONS, range_query_1) {
    avl::avl_tree<int> tree;

    tree.insert(8);
    tree.insert(10);
    tree.insert(1);

    size_t count = tree.range_queries(0, 9);
    ASSERT_EQ(count, 2);
}

TEST(AVL_TREE_FUNCTIONS, range_query_2) {
    avl::avl_tree<int> tree;

    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);

    size_t count = tree.range_queries(-2, 9);
    ASSERT_EQ(count, 2);
}

TEST(AVL_TREE_FUNCTIONS, range_query_3) {
    avl::avl_tree<int> tree;

    tree.insert(-1);
    tree.insert(10);
    tree.insert(2);
    tree.insert(4);

    size_t count = tree.range_queries(-2, 5);
    ASSERT_EQ(count, 3);
}

TEST(AVL_TREE_FUNCTIONS, range_query_4) {
    avl::avl_tree<int> tree;

    tree.insert(-5);
    tree.insert(0);
    tree.insert(-1);
    tree.insert(11);
    tree.insert(2);
    tree.insert(8);

    size_t count = tree.range_queries(-2, 10);
    ASSERT_EQ(count, 4);
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

    size_t count = tree.range_queries(2, 10);
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

    size_t count = copy_tree.range_queries(2, 10);
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

    size_t count = copy_tree.range_queries(2, 10);
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
    size_t count = copy_tree.range_queries(2, 10);
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
    size_t count = copy_tree.range_queries(2, 10);
    ASSERT_EQ(count, 4);
}

TEST(AVL_ITERATOR, begin) {
    avl::avl_tree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(-100);

    ASSERT_EQ((*tree.begin()).key_, -100);
}

TEST(AVL_ITERATOR, end) {
    avl::avl_tree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(-100);

    ASSERT_TRUE(tree.end().isNull());
}

TEST(AVL_ITERATOR, cbegin) {
    avl::avl_tree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(-100);

    ASSERT_EQ((*tree.cbegin()).key_, -100);
}

TEST(AVL_ITERATOR, cend) {
    avl::avl_tree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(-100);

    ASSERT_TRUE(tree.cend().isNull());
}

TEST(AVL_ITERATOR, isNull_method_first) {
    avl::avl_tree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(-100);

    ASSERT_EQ(tree.end().isNull(), true);
}

TEST(AVL_ITERATOR, isNull_method_second) {
    avl::avl_tree<int> tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(10);
    tree.insert(-100);

    ASSERT_EQ(tree.begin().isNull(), false);
}

TEST(AVL_ITERATOR, operator_not_equal) {
    avl::avl_tree<int> tree;
    tree.insert(1);
    tree.insert(2);

    ASSERT_TRUE(tree.begin() != tree.end());
}

TEST(AVL_ITERATOR, operator_equal) {
    avl::avl_tree<int> tree;
    tree.insert(1);
    tree.insert(2);

    ASSERT_FALSE(tree.begin() == tree.end());
}

TEST(AVL_ITERATOR, operator_plus_plus_first) {
    avl::avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    auto tree_it = tree.begin();
    ++tree_it;
    avl::avl_tree<int>::iterator root_it(tree.root.get());

    ASSERT_EQ(tree_it, root_it);
}

TEST(AVL_ITERATOR, operator_plus_plus_second) {
    avl::avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(11);
    tree.insert(19);

    auto tree_it = tree.begin();
    ++tree_it;
    ++tree_it;

    ASSERT_EQ((*tree_it).key_, 19);
}

TEST(AVL_ITERATOR, operator_minus_minus_first) {
    avl::avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    auto tree_it = tree.end();
    --tree_it;

    ASSERT_TRUE(tree_it.isNull());
}

TEST(AVL_ITERATOR, operator_minus_minus_second) {
    avl::avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(11);
    tree.insert(19);

    auto tree_it = tree.begin();
    for (int i = 0; i != 4 ; ++tree_it, ++i) {}

    ASSERT_EQ((*tree_it).key_, 30);
}

TEST(AVL_ITERATOR, operator_dereference) {
    avl::avl_tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    ASSERT_EQ((*tree.begin()).key_, 10);
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}