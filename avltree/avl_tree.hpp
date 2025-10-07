#pragma once

#include <iostream>
#include <stack>
#include <algorithm>
#include <cassert>


namespace avl {

constexpr int MIN_BALANCE = -1;
constexpr int MAX_BALANCE =  1;

enum class Queries {
    k,
    q
};

enum class FindFlags {
    left,
    right,
    exists
};

template <typename KeyType>
class avl_tree {
 private:
    class avl_node {
     public:
        KeyType key_;
        avl_node* parent_;
        avl_node* left_;
        avl_node* right_;
        int height_;
        size_t subtree_size_;

     public:
        avl_node(KeyType key, avl_node* parent, avl_node* left = nullptr
                , avl_node* right = nullptr, int height = 1, size_t subtree_size = 1):
        key_(key),
        parent_(parent),
        left_(left),
        right_(right),
        height_(height),
        subtree_size_(subtree_size) {};

        void updateNodeHeight() {
            height_ = 1 + std::max(getHeight(left_), getHeight(right_));
        }

        int getHeight(avl_node* node) const {
            return node ? node->height_ : 0;
        }

        int getBalanceFactor() const {
            return getHeight(left_) - getHeight(right_);
        }
    };

    avl_node* root = nullptr;

    using find_flag = avl::FindFlags;
    using find_res  = std::pair<avl_node*, find_flag>;

 public:
    ~avl_tree() {
        deleteTree();
    }

    void insert(const KeyType& key_to_insert) {
        if (!root) {
            root = new avl_node(key_to_insert, nullptr);
            return;
        }

        auto [parent, where_to_insert] = find(key_to_insert);

        if (where_to_insert == find_flag::exists)
            return;

        avl_node* new_node = new avl_node(key_to_insert, parent);

        if (where_to_insert == find_flag::right)
            parent->right_ = new_node;
        else
            parent->left_ = new_node;

        updateHeights(new_node);
    }

    find_res find (const KeyType& key_to_find) const {
        avl_node* current = root;
        avl_node* parent = nullptr;
        find_flag where_found = find_flag::exists;

        while (current) {
            if (key_to_find == current->key_)
                return {current, find_flag::exists};

            parent = current;

            if (key_to_find > current->key_) {
                current = current->right_;
                where_found = find_flag::right;
            }
            else {
                current = current->left_;
                where_found = find_flag::left;
            }
        }

        return {parent, where_found};
    }

 private:
    void deleteTree() {
        if (!root)
            return;

        std::stack<avl_node*> deletions;
        avl_node* current = root;
        deletions.push(current);

        while (!deletions.empty()) {
            current = deletions.top();

            if (!current->left_ && !current->right_) {
                deletions.pop();
                avl_node* parent = current->parent_;

                if (parent) {
                    if (current == parent->left_)
                        parent->left_ = nullptr;
                    else
                        parent->right_ = nullptr;
                }
                delete current;
                continue;
            }

            if (current->right_)
                deletions.push(current->right_);

            if (current->left_)
                deletions.push(current->left_);
        }
    }

    avl_node* leftRotate(avl_node* disbalancedNode) {
        avl_node* newRoot = disbalancedNode->right_;
        avl_node* newRightSubtree = newRoot->left_;

        newRoot->parent_ = disbalancedNode->parent_;
        newRoot->left_ = disbalancedNode;

        disbalancedNode->parent_ = newRoot;
        disbalancedNode->right_ = newRightSubtree;

        if (newRightSubtree)
            newRightSubtree->parent_ = disbalancedNode;

        disbalancedNode->updateNodeHeight();
        setSubtreeSize(disbalancedNode);

        newRoot->updateNodeHeight();
        setSubtreeSize(newRoot);

        return newRoot;
    }

    avl_node* rightRotate(avl_node* disbalancedNode) {
        avl_node* newRoot = disbalancedNode->left_;
        avl_node* newLeftSubtree = newRoot->right_;

        newRoot->parent_ = disbalancedNode->parent_;
        newRoot->right_ = disbalancedNode;

        disbalancedNode->parent_ = newRoot;
        disbalancedNode->left_ = newLeftSubtree;

        if (newLeftSubtree)
            newLeftSubtree->parent_ = disbalancedNode;

        disbalancedNode->updateNodeHeight();
        setSubtreeSize(disbalancedNode);

        newRoot->updateNodeHeight();
        setSubtreeSize(newRoot);

        return newRoot;
    }

    avl_node* rebalance(avl_node* disbalancedNode) {
        int balanceFactor = disbalancedNode->getBalanceFactor();
        if (MIN_BALANCE <= balanceFactor && balanceFactor <= MAX_BALANCE)
            return disbalancedNode;

        avl_node* newRoot = nullptr;

        if (balanceFactor > MAX_BALANCE) { // left disbalancedNode
            avl_node* leftDisbChild = disbalancedNode->left_;

            if (leftDisbChild->getBalanceFactor() < 0)
               disbalancedNode->left_ = leftRotate(leftDisbChild);

            newRoot = rightRotate(disbalancedNode);
        }
        else {                            // right disbalancedNode
            avl_node* rightDisbChild = disbalancedNode->right_;

            if (rightDisbChild->getBalanceFactor() > 0)
               disbalancedNode->right_ = rightRotate(rightDisbChild);

            newRoot = leftRotate(disbalancedNode);
        }

        assert(newRoot);
        setChildDependency(newRoot, disbalancedNode);

        return newRoot;
    }

    void setChildDependency(avl_node* newRoot, const avl_node* disbalancedNode) {
        avl_node* parent = newRoot->parent_;
        if (!parent) {
            root = newRoot;
            return;
        }

        if (parent->left_ == disbalancedNode)
            parent->left_ = newRoot;
        else
            parent->right_ = newRoot;
    }

    void updateHeights(avl_node* node) {
        while (node) {
            node->updateNodeHeight();
            setSubtreeSize(node);
            int balanceFactor = node->getBalanceFactor();

            if (MIN_BALANCE > balanceFactor || balanceFactor > MAX_BALANCE) {
                avl_node* disbalancedNode = node;
                node = rebalance(disbalancedNode);
            }
            else
                node = node->parent_;
        }
    }

    size_t getSubtreeSize(avl_node* node) const{
        return node ? node->subtree_size_ : 0;
    }

    void setSubtreeSize(avl_node* node) const{
        node->subtree_size_ = 1 + getSubtreeSize(node->left_) + getSubtreeSize(node->right_);
    }

 public:
    avl_node* lower_bound(const KeyType& key) const {
        auto [node, where_found] = find(key);

        if (where_found == find_flag::exists)
            return node;

        if (where_found == find_flag::left) {
            return node;
        }
        else {
            avl_node* parent = node->parent_;
            avl_node* curNode = node;
            while (parent && parent->right_ == curNode) {
                curNode = parent;
                parent = parent->parent_;
            }
            return parent;
        }
    }

    avl_node* upper_bound(const KeyType& key) const {
        auto [node, where_found] = find(key);

        if (where_found == find_flag::left) {
            return node;
        }

        if (where_found == find_flag::exists) {
            if (node->right_) {
                node = node->right_;
                while (node->left_)
                    node = node->left_;
                return node;
            }

            avl_node* parent = node->parent_;
            avl_node* curNode = node;

            while (parent && parent->right_ == curNode) {
                curNode = parent;
                parent = parent->parent_;
            }
            return parent;
        }
        else {
            avl_node* parent = node->parent_;
            avl_node* curNode = node;
            while (parent && parent->right_ == curNode) {
                curNode = parent;
                parent = parent->parent_;
            }
            return parent;
        }
    }

    size_t distance(avl_node* lower, avl_node* upper) const {
        if (!lower || !upper || lower->key_ > upper->key_)
            return 0;
        return getSmallerKeysCount(upper) - getSmallerKeysCount(lower);
    }

 private:
    size_t getSmallerKeysCount(const avl_node* node) const {
        size_t result = 0;
        avl_node* current = root;

        while (current) {
            if (node->key_ < current->key_) {
                current = current->left_;
            }
            else {
                result += 1 + getSubtreeSize(current->left_);
                if (node->key_ == current->key_)
                    break;
                current = current->right_;
            }
        }

        return result;
    }

 public:
    size_t range_queries(const KeyType& first, const KeyType& second) const{
        if (first > second)
            return 0;

        avl_node* lower = upper_bound(first);
        avl_node* upper = lower_bound(second);

        return distance(lower, upper);
    }

    #if 1
    void printTree() const{
        if (!root)
            return;

        std::stack<avl_node*> stack;
        avl_node* node = root;

        while (!stack.empty() || node) {
            while (node) {
                stack.push(node);
                node = node->left_;
            }

            node = stack.top();
            stack.pop();

            if (node == root)
                std::cout << "ROOT->";

            std::cout << node->key_ << "(" << node->subtree_size_ << ") ";
            node = node->right_;
        }
        std::cout << std::endl;
    }
    #endif
};

} // namespace avl