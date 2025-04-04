#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item) override; // TODO
    virtual void remove(const Key& key) override;  // TODO
protected:
    //virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2) override;
    virtual void nodeSwap(Node<Key,Value>* n1, Node<Key,Value>* n2) override;

    // Add helper functions here
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
AVLNode<Key, Value>* AVLTree<Key, Value>::makeAVLNode(const std::pair<const Key, Value>& new_item, AVLNode<Key, Value>* parent)
{
    return new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* right = node->getRight();
    AVLNode<Key, Value>* rightLeft = right->getLeft();

    right->setLeft(node);
    node->setRight(rightLeft);

    if (right->getParent() != nullptr) {
        if (node == right->getParent()->getLeft()) {
            right->getParent()->setLeft(right);
        } else {
            right->getParent()->setRight(right);
        }
    } else {
        this->root_ = right;
    }
    node->setBalance(node->getBalance() - 1 - std::max(right->getBalance(), static_cast<int8_t>(0)));
    right->setBalance(right->getBalance() - 1 + std::min(node->getBalance(), static_cast<int8_t>(0)));
    return right;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* left = node->getLeft();
    AVLNode<Key, Value>* leftRight = left->getRight();

    left->setRight(node);
    node->setLeft(leftRight);

    if (left->getParent() != nullptr) {
        if (node == left->getParent()->getLeft()) {
            left->getParent()->setLeft(left);
        } else {
            left->getParent()->setRight(left);
        }
    } else {
        this->root_ = left;
    }
    node->setBalance(node->getBalance() + 1 - std::min(left->getBalance(), static_cast<int8_t>(0)));
    left->setBalance(left->getBalance() + 1 + std::max(node->getBalance(), static_cast<int8_t>(0)));
    return left;
}

template<class Key, class Value>
void AVLTree<Key, Value>::balance(AVLNode<Key, Value>* node)
{
    while (node != nullptr) {
        if (node->getBalance() > 1) {
            if(node->getRight()->getBalance() < 0) {
                rotateRight(node->getRight());
            }
            node = rotateLeft(node);
        } else if (node->getBalance() < -1) {
            if (node->getLeft()->getBalance() > 0) {
                rotateLeft(node->getLeft());
            }
            node = rotateRight(node);
        }
        node = node->getParent();
    }
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::findAVLNode(const Key& key) const
{
    return static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getSmallestAVLNode() const
{
    return static_cast<AVLNode<Key, Value>*>(this->getSmallest());
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::predecessorAVLNode(AVLNode<Key, Value>* current)
{
    return static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(current));
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::successorAVLNode(AVLNode<Key, Value>* node)
{
    return static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::successor(node));
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
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

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* nodeToRemove = findAVLNode(key);
    if (nodeToRemove == nullptr) {
        return;
    }

    if (nodeToRemove->getLeft() != nullptr && nodeToRemove->getRight() != nullptr) {
        AVLNode<Key, Value>* predecessorNode = predecessorAVLNode(nodeToRemove);
        nodeSwap(nodeToRemove, predecessorNode);
        nodeToRemove = predecessorNode;
    }

    AVLNode<Key, Value>* child;
    if (nodeToRemove->getLeft() != nullptr) {
        child = nodeToRemove->getLeft();
    } else {
        child = nodeToRemove->getRight();
    }

    AVLNode<Key, Value>* parent = nodeToRemove->getParent();

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
    
    delete nodeToRemove;

    while (parent != nullptr) {
        if (child != nullptr && child->getKey() < parent->getKey()) {
            parent->updateBalance(1);
        }
        else {
            parent->updateBalance(-1);
        }

        if (std::abs(parent->getBalance()) > 1) {
            balance(parent);
        }
        child = parent;
        parent = parent->getParent();
    }
}

template<class Key, class Value>
//void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
void AVLTree<Key, Value>::nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    AVLNode<Key, Value>* avlN1 = static_cast<AVLNode<Key, Value>*>(n1);
    AVLNode<Key, Value>* avlN2 = static_cast<AVLNode<Key, Value>*>(n2);
    int8_t tempB = avlN1->getBalance();
    avlN1->setBalance(avlN2->getBalance());
    avlN2->setBalance(tempB);
    /*
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
    */
}


#endif
