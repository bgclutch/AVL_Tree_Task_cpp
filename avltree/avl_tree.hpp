#pragma once

#include <stack>
#include <algorithm>
#include <cassert>
#include <memory>

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

template <typename KeyType>
class avl_tree final {
 private:
    class avl_node final {
     public:
        KeyType key_;
        size_t height_;
        size_t subtree_size_;
        avl_node* parent_;
        std::unique_ptr<avl_node> left_;
        std::unique_ptr<avl_node> right_;

     public:
        avl_node(KeyType key, size_t height = 1, size_t subtree_size = 1,
                 avl_node* parent = nullptr, std::unique_ptr<avl_node> left = nullptr,
                 std::unique_ptr<avl_node> right = nullptr):
        key_(key),
        height_(height),
        subtree_size_(subtree_size),
        parent_(parent),
        left_(std::move(left)),
        right_(std::move(right)) {};

        void updateNodeHeight() {
            auto [left, right] = getChildsHeight();
            height_ = 1 + std::max(left, right);
        }

        size_t getHeight() const {
            return height_;
        }

     private:
        auto getChildsHeight() const {
            size_t left = 0;
            if (left_)
                left = left_->getHeight();

            size_t right = 0;
            if (right_)
                right = right_->getHeight();

            return std::pair(left, right);
        }

     public:
        int getBalanceFactor() const {
            auto [left, right] = getChildsHeight();
            return left - right;
        }

        size_t getSubtreeSize() const {
            return subtree_size_;
        }

        void updateSubtreeSize() noexcept {
            subtree_size_ = 1;
            subtree_size_ += left_.get()  ? left_->getSubtreeSize() : 0;
            subtree_size_ += right_.get() ? right_->getSubtreeSize() : 0;
        }

        size_t getSmallerKeysCount() const {
            size_t result = 0;
            const avl_node* current = ascentToRoot();
            const KeyType& target = key_;

            while (current) {
                if (target <= current->key_) {
                    current = current->left_.get();
                }
                else {
                    result += 1;
                    result += current->left_.get() ? current->left_->getSubtreeSize() : 0;
                    current = current->right_.get();
                }
            }
            return result;
        }

     private:
        const avl_node* ascentToRoot() const noexcept {
            const avl_node* node = this;

            while (node->parent_)
                node = node->parent_;

            return node;
        }
    };

 private:
    static avl_node* findMin(avl_node* node) {
        if (!node)
            return nullptr;

        while (node->left_)
            node = node->left_.get();

        return node;
    }

 public:
    class avl_iterator final {
     public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = avl_node;
        using difference_type   = std::ptrdiff_t;
        using reference         = const avl_node&;
        using pointer           = const avl_node* const&;

    private:
        avl_node* node_;
    public:
        explicit avl_iterator(avl_node* node = nullptr) : node_(node) {}

        explicit operator bool() const noexcept {
            return node_ != nullptr;
        }

        avl_iterator& operator++() {
            if (!node_)
                return *this;

            if (node_->right_) {
                node_ = node_->right_.get();
                node_ = findMin(node_);
            }
            else {
                auto parent = node_->parent_;
                while (parent && node_ == parent->right_.get()) {
                    node_  = parent;
                    parent = parent->parent_;
                }
                node_ = parent;
            }

            return *this;
        }

        bool operator==(const avl_iterator& other) const noexcept {
            return node_ == other.node_;
        }

        bool operator!=(const avl_iterator& other) const noexcept {
            return !(*this == other);
        }

        const avl_node& operator*() const noexcept {
            return *node_;
        }

        const avl_node* const& operator->() const noexcept {
            return node_;
        }
    };

        using rotate_direction = avl::RotationDirection;
        using find_flag = avl::FindFlags;
        using find_res  = std::pair<avl_node*, find_flag>;
        using iterator  = avl_iterator;

        std::unique_ptr<avl_node> root = nullptr;

    public:
        avl_tree() = default; // constructor

        avl_tree(const avl_tree<KeyType>& other) { // copy constructor
            root = deep_copy(other);
        }

        avl_tree(avl_tree<KeyType>&& other) noexcept : root{std::move(other.root)} {} // move constructor

        avl_tree& operator=(const avl_tree<KeyType>& other) { // copy assignment
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

        iterator begin() noexcept {
            avl_iterator iterator(findMin(root.get()));
            return iterator;
        }

        iterator end() noexcept {
            avl_iterator iterator(nullptr);
            return iterator;
        }

        iterator begin() const noexcept {
            avl_iterator iterator(findMin(root.get()));
            return iterator;
        }

        iterator end() const noexcept {
            avl_iterator iterator(nullptr);
            return iterator;
        }

        iterator cbegin() const noexcept {
            return this->begin();
        }

        iterator cend() const noexcept {
            return this->end();
        }

 private:
    std::unique_ptr<avl_node> deep_copy(const avl_tree& other) {
        if (!other.root)
            return nullptr;

        std::stack<std::pair<const avl_node*, avl_node*>> stack;
        const avl_node* node = other.root.get();

        auto newRoot = std::make_unique<avl_node>(node->key_, node->height_, node->subtree_size_, node->parent_);

        stack.push({node, newRoot.get()});

        while (!stack.empty()) {
            auto [old_node, new_node] = stack.top();
            stack.pop();

            if (old_node->left_) {
                new_node->left_ = std::make_unique<avl_node>(old_node->left_->key_, old_node->left_->height_,
                                                             old_node->left_->subtree_size_, new_node);

                stack.push({old_node->left_.get(), new_node->left_.get()});
            }

            if (old_node->right_) {
                new_node->right_ = std::make_unique<avl_node>(old_node->right_->key_, old_node->right_->height_,
                                                              old_node->right_->subtree_size_, new_node);

                stack.push({old_node->right_.get(), new_node->right_.get()});
            }
        }

        return newRoot;
    }

 public:
    void insert(const KeyType& key_to_insert) {
        if (!root) {
            root = std::make_unique<avl_node>(key_to_insert);
            return;
        }

        auto [parent, where_to_insert] = find(key_to_insert);
        if (where_to_insert == find_flag::exists)
            return;

        auto new_node = std::make_unique<avl_node>(key_to_insert);
        new_node->parent_ = parent;

        if (where_to_insert == find_flag::right)
            parent->right_ = std::move(new_node);
        else if (where_to_insert == find_flag::left)
            parent->left_ = std::move(new_node);

        updateHeights(parent);
    }

    find_res find(const KeyType& key_to_find) const {
        avl_node* current = root.get();
        avl_node* parent = nullptr;
        find_flag where_found = find_flag::exists;

        while (current) {
            if (key_to_find == current->key_)
                return {current, find_flag::exists};

            parent = current;

            if (key_to_find > current->key_) {
                current = current->right_.get();
                where_found = find_flag::right;
            }
            else {
                current = current->left_.get();
                where_found = find_flag::left;
            }
        }

        return {parent, where_found};
    }

 private:
    std::unique_ptr<avl_node> rotateLeft(std::unique_ptr<avl_node> disbalancedNode) {
        std::unique_ptr<avl_node> newRoot;
        std::unique_ptr<avl_node> newSubtree;

        newRoot = std::move(disbalancedNode->right_);
        newSubtree = std::move(newRoot->left_);

        newRoot->left_ = std::move(disbalancedNode);
        newRoot->parent_ = newRoot->left_->parent_;
        newRoot->left_->parent_ = newRoot.get();

        if (newSubtree)
            newSubtree->parent_ = newRoot->left_.get();
        newRoot->left_->right_ = std::move(newSubtree);

        newRoot->left_->updateNodeHeight();
        newRoot->left_->updateSubtreeSize();

        newRoot->updateNodeHeight();
        newRoot->updateSubtreeSize();

        return newRoot;
    }

    std::unique_ptr<avl_node> rotateRight(std::unique_ptr<avl_node> disbalancedNode) {
        std::unique_ptr<avl_node> newRoot;
        std::unique_ptr<avl_node> newSubtree;

        newRoot = std::move(disbalancedNode->left_);
        newSubtree = std::move(newRoot->right_);

        newRoot->right_ = std::move(disbalancedNode);
        newRoot->parent_ = newRoot->right_->parent_;
        newRoot->right_->parent_ = newRoot.get();

        if (newSubtree)
            newSubtree->parent_ = newRoot->right_.get();
        newRoot->right_->left_ = std::move(newSubtree);

        newRoot->right_->updateNodeHeight();
        newRoot->right_->updateSubtreeSize();

        newRoot->updateNodeHeight();
        newRoot->updateSubtreeSize();

        return newRoot;
    }

    std::unique_ptr<avl_node> rebalance(std::unique_ptr<avl_node> disbalancedNode) {
        int balanceFactor = disbalancedNode->getBalanceFactor();
        if (MIN_BALANCE <= balanceFactor && balanceFactor <= MAX_BALANCE)
            return disbalancedNode;

        std::unique_ptr<avl_node> newRoot = nullptr;

        if (balanceFactor > MAX_BALANCE) { // left disbalancedNode
            if (disbalancedNode->left_->getBalanceFactor() < 0) {
                disbalancedNode->left_ = rotateLeft(std::move(disbalancedNode->left_));
                disbalancedNode->left_->parent_ = disbalancedNode.get();
            }

            newRoot = rotateRight(std::move(disbalancedNode));
        }
        else {                             // right disbalancedNode
            if (disbalancedNode->right_->getBalanceFactor() > 0) {
                disbalancedNode->right_ = rotateRight(std::move(disbalancedNode->right_));
                disbalancedNode->right_->parent_ = disbalancedNode.get();
            }
            newRoot = rotateLeft(std::move(disbalancedNode));
        }

        return newRoot;
    }

    void updateHeights(avl_node* node) {
        while (node) {
            node->updateNodeHeight();
            node->updateSubtreeSize();

            int balanceFactor = node->getBalanceFactor();

            if (balanceFactor < MIN_BALANCE || balanceFactor > MAX_BALANCE) {
                avl_node* parent = node->parent_;

                if (parent) {
                    if (parent->left_.get() == node)
                        parent->left_ = rebalance(std::move(parent->left_));
                    else
                        parent->right_ = rebalance(std::move(parent->right_));
                }
                else {
                    root = rebalance(std::move(root));
                }

                node = parent;
            }
            else {
                node = node->parent_;
            }
        }
    }

 public:
    iterator lower_bound(const KeyType& key) const {
        auto [node, where_found] = find(key);

        if (where_found == find_flag::exists)
            return avl_iterator(node);


        if (where_found == find_flag::left) {
            return avl_iterator(node);
        }

        return ascent(node);
    }

    iterator upper_bound(const KeyType& key) const {
        auto [node, where_found] = find(key);

        if (where_found == find_flag::left) {
            return avl_iterator(node);
        }

        if (where_found == find_flag::exists) {
            if (node && node->right_) {
                node = node->right_.get();
                while (node && node->left_)
                    node = node->left_.get();
                return avl_iterator(node);
            }
        }

        return ascent(node);
    }

 private:
    iterator ascent(avl_node* node) const {
        if (!node)
            return avl_iterator(node);


        avl_node* parent = node->parent_;
        avl_node* curNode = node;

        while (parent && parent->right_.get() == curNode) {
            curNode = parent;
            parent = parent->parent_;
        }

        return avl_iterator(parent);
    }

 public:
    size_t range_queries(const KeyType& first, const KeyType& second) const {
        if (first > second)
            return 0;

        iterator lower = lower_bound(first);
        iterator upper = upper_bound(second);

        if (!lower)
            return 0;
        if (!upper)
            return root->getSubtreeSize() - lower->getSmallerKeysCount();

        return distance(lower, upper);
    }
};

template <typename Iterator>
size_t distance(Iterator lower, Iterator upper) {
    assert (lower);
    assert (upper);

    return upper->getSmallerKeysCount() - lower->getSmallerKeysCount();
}

} // namespace avl