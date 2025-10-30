#pragma once

#include <stack>
#include <algorithm>
#include <cassert>

namespace avl {

static constexpr int MIN_BALANCE = -1;
static constexpr int MAX_BALANCE =  1;

static constexpr char key_request   = 'k';
static constexpr char query_request = 'q';

enum class FindFlags {
    left,
    right,
    exists
};

enum class RotationDirection {
    left,
    right
};

/*
Iterator class/struct
const ref semantics (const Node&) access w/o changing
incr/decr, std::next && std::prev already done for classes with ++/--
*/
template <typename KeyType>
class avl_tree {
 private:
    class avl_node {
     public:
        KeyType key_;
        avl_node* parent_;
        avl_node* left_;
        avl_node* right_;
        size_t height_;
        size_t subtree_size_;

     public:
        avl_node(KeyType key, avl_node* parent, avl_node* left = nullptr
                , avl_node* right = nullptr, size_t height = 1, size_t subtree_size = 1):
        key_(key),
        parent_(parent),
        left_(left),
        right_(right),
        height_(height),
        subtree_size_(subtree_size) {};

        void updateNodeHeight() {
            height_ = 1 + std::max(getHeight(left_), getHeight(right_));
        }

        static size_t getHeight(const avl_node* node) {
            return node ? node->height_ : 0;
        }

        int getBalanceFactor() const {
            return getHeight(left_) - getHeight(right_);
        }

        static size_t getSubtreeSize(const avl_node* node) {
            return node ? node->subtree_size_ : 0;
        }

        void updateSubtreeSize() {
            subtree_size_ = 1 + getSubtreeSize(left_) + getSubtreeSize(right_);
        }
    };
    #if 0
    struct Iterator {
        const avl_node& Node;
        avl_node& operator++() const {}
        avl_node& operator--() const {}
    };
    #endif

    avl_node* root = nullptr;

    using rotate_direction = avl::RotationDirection;
    using find_flag = avl::FindFlags;
    using find_res  = std::pair<avl_node*, find_flag>;

 public:
    avl_tree() = default; // constructor
    ~avl_tree() {         // destructor
        deleteTree();
    }

    avl_tree(const avl_tree<KeyType>& other) { // copy constructor
        root = deep_copy(other);
    }

    avl_tree(avl_tree<KeyType>&& other) noexcept : root{std::exchange(other.root, nullptr)} {} // move constructor

    avl_tree& operator=(const avl_tree<KeyType>& other) { // copy assignment
        if (this == &other)
            return *this;

        auto tmp = deep_copy(other);
        std::swap(root, tmp);
        return *this;
    }

    avl_tree& operator=(avl_tree<KeyType>&& other) noexcept { // move assignment
        if (this == &other)
            return *this;

        std::swap(root, other.root);
        return *this;
    }

 private:
    static avl_node* deep_copy(const avl_tree& other) {
        if (!other.root)
            return nullptr;

        std::stack<std::pair<const avl_node*, avl_node*>> stack;
        avl_node* node = other.root;

        avl_node* newRoot = new avl_node(node->key_, node->parent_, node->left_, node->right_
                                       , node->height_, node->subtree_size_);

        stack.push({node, newRoot});

        while (!stack.empty()) {
            auto [old_node, new_node] = stack.top();
            stack.pop();

            if (old_node->left_) {
                new_node->left_ = new avl_node(old_node->left_->key_, new_node, old_node->left_->left_
                                             , old_node->left_->right_, old_node->left_->height_
                                             , old_node->left_->subtree_size_);

                stack.push({old_node->left_, new_node->left_});
            }

            if (old_node->right_) {
                new_node->right_ = new avl_node(old_node->right_->key_, new_node, old_node->right_->left_
                                              , old_node->right_->right_, old_node->right_->height_
                                              , old_node->right_->subtree_size_);

                stack.push({old_node->right_, new_node->right_});
            }
        }

        return newRoot;
    }

 public:
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

    find_res find(const KeyType& key_to_find) const {
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

    avl_node* rotation(avl_node* disbalancedNode, const rotate_direction direction) {
        avl_node* newRoot;
        avl_node* newSubtree;

        if (direction == rotate_direction::left) {
            newRoot = disbalancedNode->right_;
            newSubtree = newRoot->left_;
            newRoot->left_ = disbalancedNode;
            disbalancedNode->right_ = newSubtree;
        }
        else {
            newRoot = disbalancedNode->left_;
            newSubtree = newRoot->right_;
            newRoot->right_ = disbalancedNode;
            disbalancedNode->left_ = newSubtree;
        }

        newRoot->parent_ = disbalancedNode->parent_;
        disbalancedNode->parent_ = newRoot;

        if (newSubtree)
            newSubtree->parent_ = disbalancedNode;

        disbalancedNode->updateNodeHeight();
        disbalancedNode->updateSubtreeSize();

        newRoot->updateNodeHeight();
        newRoot->updateSubtreeSize();

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
               disbalancedNode->left_ = rotation(leftDisbChild, rotate_direction::left);

            newRoot = rotation(disbalancedNode, rotate_direction::right);
        }
        else {                            // right disbalancedNode
            avl_node* rightDisbChild = disbalancedNode->right_;

            if (rightDisbChild->getBalanceFactor() > 0)
                disbalancedNode->right_ = rotation(rightDisbChild, rotate_direction::right);

            newRoot = rotation(disbalancedNode, rotate_direction::left);
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
            node->updateSubtreeSize();
            int balanceFactor = node->getBalanceFactor();

            if (MIN_BALANCE > balanceFactor || balanceFactor > MAX_BALANCE) {
                avl_node* disbalancedNode = node;
                node = rebalance(disbalancedNode);
            }
            else
                node = node->parent_;
        }
    }

 public:
    avl_node* lower_bound(const KeyType& key) const { // FIXME return iterator
        auto [node, where_found] = find(key);

        if (where_found == find_flag::exists)
            return node;

        if (where_found == find_flag::left) {
            return node;
        }

        return ascent(node);
    }

    avl_node* upper_bound(const KeyType& key) const { // FIXME return iterator
        auto [node, where_found] = find(key);

        if (where_found == find_flag::left) {
            return node;
        }

        if (where_found == find_flag::exists) {
            if (node && node->right_) {
                node = node->right_;
                while (node && node->left_)
                    node = node->left_;
                return node;
            }
        }

        return ascent(node);
    }

 private:
    avl_node* ascent(avl_node* node) const {
        avl_node* parent = node->parent_;
        avl_node* curNode = node;

        while (parent && parent->right_ == curNode) {
            curNode = parent;
            parent = parent->parent_;
        }

        return parent;
    }

 public:
    size_t distance(const avl_node* lower, const avl_node* upper) const { // FIXME iterators
        if (!root) {
            return 0;
        }
        if (!lower) {
            return 0;
        }
        if (!upper) {
            auto res = avl_node::getSubtreeSize(root) - getSmallerKeysCount(lower);
            return res;
        }
        if (lower->key_ > upper->key_) {
            return 0;
        }
        return getSmallerKeysCount(upper) - getSmallerKeysCount(lower);
    }

 private:
    size_t getSmallerKeysCount(const avl_node* node) const {
        size_t result = 0;
        avl_node* current = root;

        while (current) {
            if (node->key_ <= current->key_) {
                current = current->left_;
            }
            else {
                result += 1 + avl_node::getSubtreeSize(current->left_);
                current = current->right_;
            }
        }
        return result;
    }

 public:
    size_t range_queries(const KeyType& first, const KeyType& second) const {
        if (first > second)
            return 0;

        avl_node* lower = lower_bound(first);
        avl_node* upper = upper_bound(second);

        return distance(lower, upper);
    }
};

} // namespace avl