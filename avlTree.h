#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include "library1.h"

using namespace std;

/***************************************************************
 * Compare classes
 ***************************************************************/


/* Compares two elements of the same type */
template<class Data> class IsLess {
public:
    virtual bool operator()(Data*, Data*) const = 0;
    virtual ~IsLess() {}
};

/* Compares a value and an element of the same type */
template<class Data, class Value> class IsLessThanKey {
public:
    virtual bool operator()(Value, Data*) const = 0;
    virtual bool operator()(Data*, Value) const = 0;
    virtual ~IsLessThanKey() {}
};

/***************************************************************
 * A class Represents a node data the AVLTree
 ***************************************************************/

template<class Data> class AVLNode {
private:
    
    Data* data;
    int height;
    int balanceFactor;
    
public:
    
    AVLNode(Data* data) :
    data(data), height(0), leftSon(0), rightSon(0), balanceFactor(0) {
    }
    
    Data* getData() const {
        return data;
    }
    
    void setData(Data* newData) {
        data = newData;
    }
    
    int getHeight() const {
        return height;
    }
    
    int getBalanceFactor() const {
        return balanceFactor;
    }
    
    void HeightUpdate();
    void BfUpdate();
    
    AVLNode* leftSon;
    AVLNode* rightSon;
};

/*get the height of AVLNode pointer*/
template<class Data>
static int nodeHeight(AVLNode<Data>* &node) {
    if (node == 0){
        return -1;
    }
    return node->getHeight();
}

//get BF of AVLNode pointer
template<class Data>
static int nodeBF(AVLNode<Data>* &node) {
    if (node == 0){
        return 0;
    }
    return node->getBalanceFactor();
}

//update node height using sons' heights
template<class Data>
void AVLNode<Data>::HeightUpdate() {
    height = max(nodeHeight(leftSon), nodeHeight(rightSon)) + 1;
}

//update node's BF using sons' heights
template<class Data>
void AVLNode<Data>::BfUpdate() {
    balanceFactor = nodeHeight(leftSon) - nodeHeight(rightSon);
}


//AVLtree's class
template<class Data, class Value> class AVLtree {
public:
    AVLtree() :
    root(0) {}
    ~AVLtree() {
        removeAllNodes(root);
    }
    bool isEmpty();
    StatusType Insert(Data* element, const IsLess<Data>&);
    StatusType Remove(Data* element, const IsLess<Data>&);
    void removeAllData() {
        removeAllData(root);
    }
    Data* searchElement(Value key, const IsLessThanKey<Data, Value>&) const;
    Data* min() const;
    Data* max() const;
    void printInOrder() {
        printInOrder(root);
    }
    void printPreOrder() {
        printPreOrder(root);
    }
    void printTree() {
        printTree(root);
    }
    StatusType InorderArray(Data** &array, int* size) const;
    
    AVLNode<Data>* getRoot(){
        return root;
    }
    AVLtree(Data** elements , int size);
private:
    AVLNode<Data>* root; //pointer to the tree's root (0 if empty)
    void removeAllNodes(AVLNode<Data>* current);
    StatusType addNode(AVLNode<Data>* node, AVLNode<Data>* &current, const IsLess<Data>&);
    StatusType removeNode(Data* element, AVLNode<Data>* &current, const IsLess<Data>&);
    void removeAllData(AVLNode<Data>*);
    void ArraySize(AVLNode<Data>* current, int* size) const;
    StatusType AssembleArray(AVLNode<Data>* current, Data** array, int size, int &index) const;
    Data* searchElementRec(AVLNode<Data>* const &current, Value key,
                           const IsLessThanKey<Data, Value>& less) const;
    void rotate(AVLNode<Data>* &current);
    void update(AVLNode<Data>* &current);
    void printInOrder(AVLNode<Data>*) const;
    void printPreOrder(AVLNode<Data>*) const;
    void printTree(AVLNode<Data>*) const;
    AVLNode<Data>* next(AVLNode<Data>* &current) const;
    AVLNode<Data>* sortedArrToAVL(Data** elements, int left, int right);
};

//constructor from sorted array to balanced AVL tree ,
template<class Data, class Value> AVLtree<Data, Value>::AVLtree(Data **elements , int size) {
    root = sortedArrToAVL(elements , 0 , size-1);
}

template<class Data, class Value> AVLNode<Data>* AVLtree<Data, Value>::sortedArrToAVL(Data** elements, int left, int right)
{
    if (left > right)
        return NULL;

    int mid = (left + right)/2;
    AVLNode<Data> *root = new AVLNode<Data>(elements[mid]);
    root->leftSon =  sortedArrToAVL(elements, left, mid-1);
    root->rightSon = sortedArrToAVL(elements, mid+1, right);

    return root;
}

//checks if the tree is empty
template<class Data, class Value> bool AVLtree<Data, Value>::isEmpty() {
    if (root == 0) {
        return true;
    }
    return false;
}

//deletes all of the tree's nodes recursively
template<class Data, class Value> void AVLtree<Data, Value>::removeAllNodes(AVLNode<Data>* current) {
    if (current == 0)
        return;
    removeAllNodes(current->leftSon);
    removeAllNodes(current->rightSon);
    delete current;
}

//creates node and calls "addNode"
template<class Data, class Value> StatusType AVLtree<Data, Value>::Insert(Data* element,
                                                                          const IsLess<Data>& less) {
    AVLNode<Data>* node = new AVLNode<Data> (element);
    if (node == 0)
        return ALLOCATION_ERROR;
    return addNode(node, root, less);
}

//adds a node to the tree recursively
template<class Data, class Value> StatusType AVLtree<Data, Value>::addNode(AVLNode<Data>* node,
                                                                           AVLNode<Data>* &current, const IsLess<Data>& less) {
    StatusType control = SUCCESS;
    if (current == 0)
        current = node;
    else if (less(node->getData(), current->getData()))
        control = addNode(node, current->leftSon, less);
    else if (less(current->getData(), node->getData()))
        control = addNode(node, current->rightSon, less);
    else { //the node all ready exists data the tree
        delete node;
        return FAILURE;
    }
    
    //update BFs and preform rotations if necessary
    update(current);
    rotate(current);
    update(current);
    
    return control;
}

//calls recursive function "removeNode"
template<class Data, class Value> StatusType AVLtree<Data, Value>::Remove(
                                                                          Data* element, const IsLess<Data>& less) {
    return removeNode(element, root, less);
}

//removes a node from the tree recusively
template<class Data, class Value> StatusType AVLtree<Data, Value>::removeNode(Data* element,
                                                                              AVLNode<Data>* &current, const IsLess<Data>& less) {
    if (current == 0)
        return FAILURE;
    if (less(element, current->getData()))
        removeNode(element, current->leftSon, less);
    else if (less(current->getData(), element))
        removeNode(element, current->rightSon, less);
    else { //we found the element wanted for removal
        if (current->leftSon == 0) { //if the leftSon son is empty
            AVLNode<Data>* temp = current;
            current = current->rightSon;
            delete temp;
        } else if (current->rightSon == 0) { //if the rightSon son is empty
            AVLNode<Data>* temp = current;
            current = current->leftSon;
            delete temp;
        } else { //if the node has both sons
            AVLNode<Data>* temp = next(current); //get the next node Inorder
            current->setData(temp->getData()); //copy it's data to the current node
            removeNode(temp->getData(), current->rightSon, less); //remove the next node Inorder
        }
    }
    
    //update BFs and preform rotations if necessary
    update(current);
    rotate(current);
    update(current);
    
    return SUCCESS;
}

//remove all the data the tree points to recursively
template<class Data, class Value> void AVLtree<Data, Value>::removeAllData(AVLNode<Data>* current) {
    if (current == 0)
        return;
    removeAllData(current->leftSon);
    removeAllData(current->rightSon);
    delete current->getData();
}

//calls recursive function "searchElementRec"
template<class Data, class Value> Data* AVLtree<Data, Value>::searchElement(Value key,const IsLessThanKey<Data, Value>& less) const {
    return searchElementRec(root, key, less);
}

//search for a node by data recursively
template<class Data, class Value> Data* AVLtree<Data, Value>::searchElementRec(AVLNode<Data>* const &current, Value key, const IsLessThanKey<Data, Value>& less) const {
    if (current == 0)
        return NULL;
    Data* data = current->getData();
    if (less(data, key))
        return searchElementRec(current->rightSon, key, less);
    if (less(key, data))
        return searchElementRec(current->leftSon, key, less);
    //we reached the wanted element
    return data;
}

//get the smallest node data the tree
template<class Data, class Value> Data* AVLtree<Data, Value>::min() const {
    AVLNode<Data>* current = root;
    if (current == 0)
        return 0;
    while (current->leftSon != 0)
        //go leftSon until NULL
        current = current->leftSon;
    return current->getData();
}

//get the biggest node in the tree
template<class Data, class Value> Data* AVLtree<Data, Value>::max() const {
    AVLNode<Data>* current = root;
    if (current == 0)
        return 0;
    while (current->rightSon != 0){
        //go leftSon until NULL
        current = current->rightSon;
    }
    return current->getData();
}

//update sons' and current node's height and BF
template<class Data, class Value> void AVLtree<Data, Value>::update(AVLNode<Data>* &current) {
    if (current == 0)
        return;
    if (current->leftSon != 0) { //update leftSon son
        current->leftSon->HeightUpdate();
        current->leftSon->BfUpdate();
    }
    if (current->rightSon != 0) { //update rightSon son
        current->rightSon->HeightUpdate();
        current->rightSon->BfUpdate();
    }
    current->HeightUpdate(); //update current node
    current->BfUpdate();
}

//preform rotations if necessary according to BFs
template<class Data, class Value> void AVLtree<Data, Value>::rotate(AVLNode<Data>* &current) {
    if (current == 0)
        return;
    int balanceFactor = nodeBF(current);
    int bf_l = nodeBF(current->leftSon);
    int bf_r = nodeBF(current->rightSon);
    
    //LL rotation
    if (balanceFactor == 2 && bf_l >= 0) {
        AVLNode<Data>* temp = current;
        current = current->leftSon;
        temp->leftSon = current->rightSon;
        current->rightSon = temp;
    }
    
    //LR rotation
    else if (balanceFactor == 2 && bf_l == -1) {
        AVLNode<Data>* temp1 = current;
        AVLNode<Data>* temp2 = current->leftSon;
        current = current->leftSon->rightSon;
        temp1->leftSon = current->rightSon;
        temp2->rightSon = current->leftSon;
        current->rightSon = temp1;
        current->leftSon = temp2;
    }
    
    //RR rotation
    else if (balanceFactor == -2 && bf_r <= 0) {
        AVLNode<Data>* temp = current;
        current = current->rightSon;
        temp->rightSon = current->leftSon;
        current->leftSon = temp;
    }
    
    //RL rotation
    else if (balanceFactor == -2 && bf_r == 1) {
        AVLNode<Data>* temp1 = current;
        AVLNode<Data>* temp2 = current->rightSon;
        current = current->rightSon->leftSon;
        temp1->rightSon = current->leftSon;
        temp2->leftSon = current->rightSon;
        current->leftSon = temp1;
        current->rightSon = temp2;
    }
}

//get the next node Inorder
template<class Data, class Value> AVLNode<Data>* AVLtree<Data, Value>::next(AVLNode<Data>* &current) const {
    AVLNode<Data>* temp = current->rightSon; //go rightSon once
    while (temp->leftSon != 0) { //then leftSon until NULL
        temp = temp->leftSon;
    }
    return temp;
}

//returns an array of Data pointers Inorder
template<class Data, class Value> StatusType AVLtree<Data, Value>::InorderArray(Data** &array, int* size) const {
    *size = 0;
    ArraySize(root, size); //get the array size
    if (*size > 0) {
        array = new Data*[*size];
        int index = 0;
        return AssembleArray(root, array, *size, index);
        //add item to the array
    }
    return SUCCESS;
}

//gets the intended array size
template<class Data, class Value> void AVLtree<Data, Value>::ArraySize(AVLNode<Data>* current, int* size) const {
    if (current == 0)
        return;
    ArraySize(current->leftSon, size);
    ArraySize(current->rightSon, size);
    *size = *size + 1;
}

//inserts the Data pointers into the array Inorder
template<class Data, class Value> StatusType AVLtree<Data, Value>::AssembleArray(AVLNode<Data>* current, Data** array, int size, int &index) const {
    if (current == 0)
        return SUCCESS;
    AssembleArray(current->leftSon, array, size, index);
    *(array + index) = current->getData();
    index++;
    AssembleArray(current->rightSon, array, size, index);
    return SUCCESS;
}

#endif /* AVLTREE_H */
