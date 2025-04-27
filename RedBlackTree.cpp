#include <iostream>
#include <cassert>
#include <random>
#include "RedBlackTree.h"

using namespace std;

RedBlackTree::RedBlackTree(){
    root = nullptr;
    numItems = 0;
}

RedBlackTree::RedBlackTree(int newData){
    Insert(newData);
}

RedBlackTree::RedBlackTree(const RedBlackTree &rbt){
    root = CopyOf(rbt.root);
	numItems = rbt.numItems;
}

void RedBlackTree::Insert(int newData){
    RBTNode* newNode = new RBTNode();
    newNode->data = newData;
    newNode->color = COLOR_RED;
	BasicInsert(newNode);
	InsertFixUp(newNode);
	numItems++;
}

bool RedBlackTree::Contains(int data) const{
    return Get(data) != nullptr;
}


int RedBlackTree::GetMin() const {
	RBTNode* current = root;
	if (!current) {
        throw runtime_error("Tree is empty.");
    }
	while (current->left) {
        current = current->left;
    }
	return current->data;
}


int RedBlackTree::GetMax() const {
	RBTNode* current = root;
	if (!current) throw runtime_error("Tree is empty.");
	while (current->right) current = current->right;
	return current->data;
}

string RedBlackTree::ToInfixString(const RBTNode *n) {

	if (!n) {
        return "";
    }

	return ToInfixString(n->left) + GetNodeString(n) + ToInfixString(n->right);
}

string RedBlackTree::ToPrefixString(const RBTNode *n) {
	if (!n) {
        return "";
    }
	return GetNodeString(n) + ToPrefixString(n->left) + ToPrefixString(n->right);
}

string RedBlackTree::ToPostfixString(const RBTNode *n) {
	if (!n) {
        return "";
    }
	return ToPostfixString(n->left) + ToPostfixString(n->right) + GetNodeString(n);
}


string RedBlackTree::GetColorString(const RBTNode *n) {
	if (!n) {
        return "";
    }
	if (n->color == COLOR_RED) {
        return "R";
    }
	if (n->color == COLOR_BLACK) {
        return "B";
    }
	return ""; 
}

string RedBlackTree::GetNodeString(const RBTNode *n){
    if (!n){
        return "";
    }
    return " " + GetColorString(n) + to_string(n->data) + " ";
}

void RedBlackTree::BasicInsert(RBTNode *node){
    if (!root){
        root = node;
        root->color = COLOR_BLACK;
        return;
    }
    RBTNode *current = root, *parent = nullptr;
    //go through current tree and put it where it should be
    while(current){
        parent = current;
        if (node->data < current->data){
            current = current->left;
        }
        else{
            current = current->right;
        }
    }
    node->parent = parent;
    if (node->data < parent->data){
        parent->left = node;
    } else {
        parent->right = node;
    }
}

void RedBlackTree::InsertFixUp(RBTNode *node){
    while(node != root && node->parent->color == COLOR_RED){
        RBTNode* parent = node->parent;
        RBTNode* grandparent = parent->parent;
        RBTNode* uncle = GetUncle(node);

        //recolor
        if (uncle && uncle->color == COLOR_RED){ 
            parent->color = COLOR_BLACK;
            uncle->color = COLOR_BLACK;
            grandparent->color = COLOR_RED;
            node = grandparent;
        }
        //rearrange
        else{
            if(IsLeftChild(parent)){
                if(IsRightChild(node)){
                    node = parent;
                    LeftRotate(node);
                }
                node->parent->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
                RightRotate(grandparent);
            } else {
                if (IsLeftChild(node)){
                    node = parent;
                    RightRotate(node);
                }
                node->parent->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
                LeftRotate(grandparent);
            }
        }
    }
    root->color = COLOR_BLACK;
}

RBTNode *RedBlackTree::GetUncle(RBTNode *node) const{
    if(!node || !node->parent || !node->parent->parent){
        return nullptr;
    }
    if(IsLeftChild(node->parent)){
        return node->parent->parent->right;
    } else{
        return node->parent->parent->left;
    }
}

bool RedBlackTree::IsLeftChild(RBTNode *node) const {
    return node && node->parent && node->parent->left == node;
}

bool RedBlackTree::IsRightChild(RBTNode *node) const{
    return node && node->parent && node->parent->right == node;
}

void RedBlackTree::LeftRotate(RBTNode *x){
    RBTNode *y = x->right;
    x->right = y->left;
    
    if(y->left){
        y->left->parent = x;
    }
    
    if (!x->parent){
        root = y;
    } else if (x == x->parent->left){
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void RedBlackTree::RightRotate(RBTNode *x){
    RBTNode *y = x->left;
    x->left = y->right;

    if (y->right){
        y->right->parent = x;
    }

    y->parent = x->parent;

    if(!x->parent){
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
}

RBTNode *RedBlackTree::CopyOf(const RBTNode *node) {

    if (!node){
        return nullptr;
    }

    RBTNode* copy = new RBTNode();
    copy->data = node->data;
    copy->color = node->color;
    copy->IsNullNode = node->IsNullNode;

    if(node->left){
        copy->left = CopyOf(node->left);
        if(copy->left){
            copy->left->parent = copy;
        }
    }
    if (node->right){
        copy->right = CopyOf(node->right);
        if(copy->right){
            copy->right->parent = copy;
        }
    }

    return copy;
}

RBTNode *RedBlackTree::Get(int data) const{
    RBTNode *current = root;
    while(current){
        if (data == current->data){
            return current;
        }
        current = (data < current->data) ? current->left : current->right;
    }

    return nullptr;
}