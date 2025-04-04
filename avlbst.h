#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;
};

template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0) {}

template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode() {}

template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const {
    return balance_;
}

template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance) {
    balance_ = balance;
}

template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff) {
    balance_ += diff;
}

template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const {
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const {
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const {
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item) override;
    virtual void remove(const Key& key) override;
protected:
    virtual void nodeSwap(Node<Key, Value>* n1, Node<Key, Value>* n2) override;

private:
    AVLNode<Key, Value>* makeAVLNode(const std::pair<const Key, Value>& new_item, AVLNode<Key, Value>* parent);
    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* node);
    void balance(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* findAVLNode(const Key& key) const;
    AVLNode<Key, Value>* getSmallestAVLNode() const;
    AVLNode<Key, Value>* predecessorAVLNode(AVLNode<Key, Value>* current);
    AVLNode<Key, Value>* successorAVLNode(AVLNode<Key, Value>* node);
};

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::makeAVLNode(const std::pair<const Key, Value>& new_item, AVLNode<Key, Value>* parent) {
    return new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* right = node->getRight();
    AVLNode<Key, Value>* rightLeft = right->getLeft();

    right->setLeft(node);
    node->setRight(rightLeft);

    if (rightLeft != nullptr) {
        rightLeft->setParent(node);
    }
    right->setParent(node->getParent());
    node->setParent(right);

    if (right->getParent() != nullptr) {
        if (node == right->getParent()->getLeft()) {
            right->getParent()->setLeft(right);
        } else {
            right->getParent()->setRight(right);
        }
    } else {
        this->root_ = right;
    }
    return right;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* left = node->getLeft();
    AVLNode<Key, Value>* leftRight = left->getRight();

    left->setRight(node);
    node->setLeft(leftRight);

    if (leftRight != nullptr) {
        leftRight->setParent(node);
    }
    left->setParent(node->getParent());
    node->setParent(left);

    if (left->getParent() != nullptr) {
        if (node == left->getParent()->getLeft()) {
            left->getParent()->setLeft(left);
        } else {
            left->getParent()->setRight(left);
        }
    } else {
        this->root_ = left;
    }
    return left;
}

template<class Key, class Value>
void AVLTree<Key, Value>::balance(AVLNode<Key, Value>* node) {
    while (node != nullptr) {
        if (node->getBalance() > 1) {
            if (node->getRight()->getBalance() < 0) {
                rotateRight(node->getRight());
            }
            rotateLeft(node);
        } else if (node->getBalance() < -1) {
            if (node->getLeft()->getBalance() > 0) {
                rotateLeft(node->getLeft());
            }
            rotateRight(node);
        }
        node = node->getParent();
    }
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::findAVLNode(const Key& key) const {
    return static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getSmallestAVLNode() const {
    return static_cast<AVLNode<Key, Value>*>(this->getSmallestNode());
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::predecessorAVLNode(AVLNode<Key, Value>* current) {
    return static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(current));
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::successorAVLNode(AVLNode<Key, Value>* node) {
    return static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::successor(node));
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item) {
    if (this->root_ == nullptr) {
        this->root_ = makeAVLNode(new_item, nullptr);
        return;
    }

    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while (current != nullptr) {
        parent = current;
        if (new_item.first < current->getKey()) {
            current = current->getLeft();
        } else if (new_item.first > current->getKey()) {
            current = current->getRight();
        } else {
            current->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key, Value>* newNode = makeAVLNode(new_item, parent);
    if (new_item.first < parent->getKey()) {
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }
    balance(parent);
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key) {
    AVLNode<Key, Value>* nodeToRemove = findAVLNode(key);
    if (nodeToRemove == nullptr) {
        return;
    }
    if (nodeToRemove->getLeft() != nullptr && nodeToRemove->getRight() != nullptr) {
        AVLNode<Key, Value>* predecessorNode = predecessorAVLNode(nodeToRemove);
        nodeSwap(nodeToRemove, predecessorNode);
        nodeToRemove = predecessorNode;
    }

    AVLNode<Key, Value>* child = (nodeToRemove->getLeft() != nullptr) ? nodeToRemove->getLeft() : nodeToRemove->getRight();

    if (nodeToRemove->getParent() == nullptr) {
        this->root_ = child;
    } else if (nodeToRemove == nodeToRemove->getParent()->getLeft()) {
        nodeToRemove->getParent()->setLeft(child);
    } else {
        nodeToRemove->getParent()->setRight(child);
    }

    if (child != nullptr) {
        child->setParent(nodeToRemove->getParent());
    }
    balance(nodeToRemove->getParent());
    delete nodeToRemove;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(Node<Key, Value>* n1, Node<Key, Value>* n2) {
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    AVLNode<Key, Value>* avlNode1 = static_cast<AVLNode<Key, Value>*>(n1);
    AVLNode<Key, Value>* avlNode2 = static_cast<AVLNode<Key, Value>*>(n2);
    int8_t tempB = avlNode1->getBalance();
    avlNode1->setBalance(avlNode2->getBalance());
    avlNode2->setBalance(tempB);
}

#endif