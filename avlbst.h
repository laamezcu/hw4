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
    AVLNode<Key, Value>* balance(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* node);
    int getBalanceFactor(AVLNode<Key, Value>* node);
    int getHeight(AVLNode<Key, Value>* node);
    AVLNode<Key, Value>* insertRecursive(AVLNode<Key, Value>* node, const std::pair<const Key, Value>& new_item);
    AVLNode<Key, Value>* removeRecursive(AVLNode<Key, Value>* node, const Key& key);
    AVLNode<Key, Value>* findPredecessorAVL(AVLNode<Key, Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    this->root_ = insertRecursive(static_cast<AVLNode<Key, Value>*>(this->root_), new_item);
    if (this->root_) {
      this->root_->setParent(nullptr);
    }
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::insertRecursive(AVLNode<Key, Value>* node, const std::pair<const Key, Value>& new_item) {
  if (node == nullptr) {
    return new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
  }

  if (new_item.first < node->getKey()) {
    AVLNode<Key, Value>* leftChild = insertRecursive(node->getLeft(), new_item);
    node->setLeft(leftChild);
    leftChild->setParent(node);
  } 
  else if (new_item.first > node->getKey()) {
    AVLNode<Key, Value>* rightChild = insertRecursive(node->getRight(), new_item);
    node->setRight(rightChild);
    rightChild->setParent(node);
  } 
  else {
    node->setValue(new_item.second);
    return node;
  }

  return balance(node);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    this->root_ = removeRecursive(static_cast<AVLNode<Key, Value>*>(this->root_), key);
  if (this->root_) {
    this->root_->setParent(nullptr);
  }
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::removeRecursive(AVLNode<Key, Value>* node, const Key& key) {
  if (node == nullptr) {
    return nullptr;
  }

  if (key < node->getKey()) {
    node->setLeft(removeRecursive(node->getLeft(), key));
    if (node->getLeft()) {
      node->getLeft()->setParent(node);
    }
  } 
  else if (key > node->getKey()) {
    node->setRight(removeRecursive(node->getRight(), key));
    if (node->getRight()) {
      node->getRight()->setParent(node);
    }
  } 
  else {
    if (node->getLeft() == nullptr || node->getRight() == nullptr) {
      delete node;
      return nullptr;
    }
    else if (node->getLeft() == nullptr) {
      AVLNode<Key, Value>* rightC = node->getRight();
      rightC->setParent(node->getParent());
      delete node;
      return rightC;
    }
    else if (node->getRight() == nullptr) {
      AVLNode<Key, Value>* leftC = node->getLeft();
      leftC->setParent(node->getParent());
      delete node;
      return leftC;
    }
    else {
      AVLNode<Key, Value>* pred = findPredecessorAVL(node);
      nodeSwap(node, pred);
      node->setLeft(removeRecursive(node->getLeft(), key));
      if (node->getLeft() != nullptr) {
        node->getLeft()->setParent(node);
      }
    }
  }

  return balance(node);
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::findPredecessorAVL(AVLNode<Key, Value>* node) {
  AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(node->getLeft());
  while (current->getRight() != nullptr) {
    current = static_cast<AVLNode<Key, Value>*>(current->getRight());
  }
  return current;
}

template <class Key, class Value>
int AVLTree<Key, Value>::getBalanceFactor(AVLNode<Key, Value>* node) {
  if (node == nullptr) {
    return 0;
  }
  return getHeight(static_cast<AVLNode<Key, Value>*>(node->getLeft())) - getHeight(static_cast<AVLNode<Key, Value>*>(node->getRight()));
}

template <class Key, class Value>
int AVLTree<Key, Value>::getHeight(AVLNode<Key, Value>* node) {
  if (node == nullptr) {
    return -1;
  }
  
  int leftH = getHeight(node->getLeft());
  int rightH = getHeight(node->getRight());
  if (leftH > rightH) {
    return 1 + leftH;
  }
  else {
    return 1 + rightH;
  }
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
  AVLNode<Key, Value>* y = node->getRight();
  AVLNode<Key, Value>* t2 = y->getLeft();

  y->setLeft(node);
  node->setRight(t2);

  if (t2 != nullptr) {
    t2->setParent(node);
  }

  y->setParent(node->getParent());
  node->setParent(y);

  if (y->getParent()) {
    if (y->getParent()->getLeft() == node) {
      y->getParent()->setLeft(y);
    }
    else {
      y->getParent()->setRight(y);
    }
  }
  else {
    this->root_ = y;
  }

  return y;
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
  AVLNode<Key, Value>* y = node->getLeft();
  AVLNode<Key, Value>* t2 = y->getRight();

  y->setRight(node);
  node->setLeft(t2);

  if (t2 != nullptr) {
    t2->setParent(node);
  }

  y->setParent(node->getParent());
  node->setParent(y);

  if (y->getParent()) {
    if (y->getParent()->getLeft() == node) {
      y->getParent()->setLeft(y);
    }
    else {
      y->getParent()->setRight(y);
    }
  }
  else {
    this->root_ = y;
  }

  return y;
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::balance(AVLNode<Key, Value>* node) {
  if (node == nullptr) {
    return node;
  }

  int balanceFactor = getBalanceFactor(node);

  if (balanceFactor > 1) {
    if (getBalanceFactor(static_cast<AVLNode<Key, Value>*>(node->getLeft())) < 0) {
      node->setLeft(rotateLeft(static_cast<AVLNode<Key, Value>*>(node->getLeft())));
    }
    return rotateRight(node);
  }

  if (balanceFactor < -1) {
    if (getBalanceFactor(static_cast<AVLNode<Key, Value>*>(node->getRight())) > 0) {
      node->setRight(rotateRight(static_cast<AVLNode<Key, Value>*>(node->getRight())));
    }
    return rotateLeft(node);
  }

  return node;
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
