#pragma once

#include <iostream>
#include <stack>
#include <cassert>


namespace avl {

constexpr int MIN_BALANCE = -1;
constexpr int MAX_BALANCE =  1;

enum class Queries {
    k = 0,
    q = 1
};

enum class FindFlags {
    left,
    right,
    exists
};

#if 0
template <typename KeyType>
class avl_node { // maybe make it as module?
 public:
    KeyType key_;
    avl_node* parent_;
    avl_node* left_;
    avl_node* right_;
 private:
    size_t height_;
 public:
    avl_node(KeyType key, avl_node* parent, avl_node* left = nullptr
            , avl_node* right = nullptr, size_t height = 0):
    key_(key),
    parent_(parent),
    left_(left),
    right_(right),
    height_(height) {};
};
#endif

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
        avl_node(KeyType key, avl_node* parent, avl_node* left = nullptr
                ,avl_node* right = nullptr, int height = 1):
        key_(key),
        parent_(parent),
        left_(left),
        right_(right),
        height_(height) {};
    };

    avl_node* root = nullptr;

    using find_flag = avl::FindFlags;
    using find_res  = std::pair<avl_node*, find_flag>;

 public:
    avl_tree() {};
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

        avl_node* disbalancedNode = updateHeights(new_node->parent_);

        rebalance(disbalancedNode);
        updateHeights(new_node);
    }

    find_res find (const KeyType& key_to_find) {
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

    avl_node* leftRotate(avl_node* disbalanced) {
        avl_node* newRoot = disbalanced->right_;
        avl_node* newRightSubtree = newRoot->left_;

        newRoot->parent_ = disbalanced->parent_;
        newRoot->left_ = disbalanced;

        disbalanced->parent_ = newRoot;
        disbalanced->right_ = newRightSubtree;

        if (newRightSubtree)
            newRightSubtree->parent_ = disbalanced;

        updateNodeHeight(disbalanced);
        updateNodeHeight(newRoot);

        return newRoot;
    }

    avl_node* rightRotate(avl_node* disbalanced) {
        avl_node* newRoot = disbalanced->left_;
        avl_node* newLeftSubtree = newRoot->right_;

        newRoot->parent_ = disbalanced->parent_;
        newRoot->right_ = disbalanced;

        disbalanced->parent_ = newRoot;
        disbalanced->left_ = newLeftSubtree;

        if (newLeftSubtree)
            newLeftSubtree->parent_ = disbalanced;

        updateNodeHeight(disbalanced);
        updateNodeHeight(newRoot);

        return newRoot;
    }

    void rebalance (avl_node* disbalanced) {
        int balanceFactor = getBalanceFactor(disbalanced);
        if (MIN_BALANCE <= balanceFactor && balanceFactor <= MAX_BALANCE)
            return;

        avl_node* rotationRes = nullptr;

        if (balanceFactor > MAX_BALANCE) { // left disbalanced
            avl_node* leftDisbChild = disbalanced->left_;

            if (getBalanceFactor(leftDisbChild) < 0) {
               disbalanced->left_ = leftRotate(leftDisbChild);
            }

            rotationRes = rightRotate(disbalanced);
        }
        else { // right disbalanced
            avl_node* rightDisbChild = disbalanced->right_;

            if (getBalanceFactor(rightDisbChild) > 0) {
               disbalanced->right_ = rightRotate(rightDisbChild);
            }

            rotationRes = leftRotate(disbalanced);
        }

        assert(rotationRes);
        setChildDependency(rotationRes, disbalanced);
        updateHeights(disbalanced);
    }

    void setChildDependency(avl_node* rotationRes, const avl_node* disbalanced) {
        avl_node* parent = rotationRes->parent_;
        if (!parent) {
            root = rotationRes;
            return;
        }

        if (parent->left_ == disbalanced)
            parent->left_ = rotationRes;
        else
            parent->right_ = rotationRes;
    }

    avl_node* updateHeights(avl_node* node) {
        while (node) {
            updateNodeHeight(node);
            int balanceFactor = getBalanceFactor(node);

            if (MIN_BALANCE > balanceFactor || balanceFactor > MAX_BALANCE)
                return node;

            node = node->parent_;
        }
        return node;
    }

    void updateNodeHeight(avl_node* node) {
        if (node)
            node->height_ = 1 + std::max(getHeight(node->left_), getHeight(node->right_));
    }

    int getHeight(avl_node* node) const{
        return node ? node->height_ : 0;
    }

    int getBalanceFactor(avl_node* node) const{
        return node ? getHeight(node->left_) - getHeight(node->right_) : 0;
    }

    avl_node* lower_bound() {
        return nullptr;
    }

    avl_node* upper_bound() {
        return nullptr;
    }

 public:
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

            std::cout << node->key_ << "(" << node->height_ << ") ";
            node = node->right_;
        }
        std::cout << std::endl;
    }
    #endif

    // upper/lower bound
    // distance
};

} // namespace avl