#pragma once

#include <iostream>
#include <stack>
#include <algorithm>
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
        size_t smaller_elems_count_;
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

        updateNodeHeight(disbalancedNode);
        updateNodeHeight(newRoot);

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

        updateNodeHeight(disbalancedNode);
        updateNodeHeight(newRoot);

        return newRoot;
    }

    avl_node* rebalance (avl_node* disbalancedNode) {
        int balanceFactor = getBalanceFactor(disbalancedNode);
        if (MIN_BALANCE <= balanceFactor && balanceFactor <= MAX_BALANCE)
            return disbalancedNode;

        avl_node* newRoot = nullptr;

        if (balanceFactor > MAX_BALANCE) { // left disbalancedNode
            avl_node* leftDisbChild = disbalancedNode->left_;

            if (getBalanceFactor(leftDisbChild) < 0)
               disbalancedNode->left_ = leftRotate(leftDisbChild);

            newRoot = rightRotate(disbalancedNode);
        }
        else {                            // right disbalancedNode
            avl_node* rightDisbChild = disbalancedNode->right_;

            if (getBalanceFactor(rightDisbChild) > 0)
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
            updateNodeHeight(node);
            int balanceFactor = getBalanceFactor(node);

            if (MIN_BALANCE > balanceFactor || balanceFactor > MAX_BALANCE) {
                avl_node* disbalancedNode = node;
                node = rebalance(disbalancedNode);
            }
            else
                node = node->parent_;
        }
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

    size_t distance() const {
        return 0;
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
};

} // namespace avl