/**********************************************
  > File Name		: AVL.cpp
  > Author		    : lunar
  > Email			: lunar_ubuntu@qq.com
  > Created Time	: Mon 05 Oct 2020 08:10:56 PM CST
 **********************************************/

#include "AVLTree.h"

template <typename T>
AVLTree<T>::AVLTree() {
    root = NULL;
}

template <typename T>
void AVLTree<T>::insert(T val) {
    root = _insert(root, val);
}

template <typename T>
void AVLTree<T>::remove(T val) {
    root = _remove(root, val);
}

template <typename T>
TreeNode<T>* AVLTree<T>::rotate_left(TreeNode<T>* node) {
    TreeNode<T>* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    update_node_height(node);
    update_node_height(temp);
    return temp;
}

template <typename T>
TreeNode<T>* AVLTree<T>::rotate_right(TreeNode<T>* node) {
    TreeNode<T>* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    update_node_height(node);
    update_node_height(temp);
    return temp;
}

template <typename T>
void AVLTree<T>::update_node_height(TreeNode<T>* node) {
    int max = 0;
    if (node->left != NULL && node->left->height > max) {
        max = node->left->height;
    }
    if (node->right != NULL && node->right->height > max) {
        max = node->right->height;
    }
    node->height = max + 1;
}

template <typename T>
int AVLTree<T>::get_node_height(TreeNode<T>* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

template <typename T>
TreeNode<T>* AVLTree<T>::_insert(TreeNode<T>* node, T val) {
    if (node == NULL) {
        node = new TreeNode<T>(val);
        assert(node != NULL);
        return node;
    }
    // This asks T has to support substraction operation and return an integer.
    int diff = static_cast<int>(val - node->val);
    if (diff < 0) {
        node->left = _insert(node->left, val);
        // If the difference between two child trees exceed 1
        // node height update is needed.
        if (get_node_height(node->left) - get_node_height(node->right) > 1) {
            // This asks T supports compare operation.
            if (val > node->left->val) {
                node->left = rotate_left(node->left);
            }
            node = rotate_right(node);
        }
    } else if (diff >= 0) {
        node->right = _insert(node->right, val);
        if (get_node_height(node->right) - get_node_height(node->left) > 1) {
            node->right = rotate_right(node->right);
        }
        node = rotate_left(node);
    }
    update_node_height(node);
    return node;
}

template <typename T>
TreeNode<T>* AVLTree<T>::_remove(TreeNode<T>* node, T val) {
    if (node == NULL) {
        return node;
    }
    if (val < node->val) {
        node->left = _remove(node->left, val);
        if (get_node_height(node->right) - get_node_height(node->left) > 1) {
            if (get_node_height(node->right->left) > get_node_height(node->right->right)) {
                node->right = rotate_right(node->right);
            }
            node = rotate_left(node);
        }
    } else if (val > node->val) {
        node->right = _remove(node->right, val);
        if (get_node_height(node->left) - get_node_height(node->right) > 1) {
            if (get_node_height(node->left->right) > get_node_height(node->left->left)) {
                node->left = rotate_left(node->left);
            }
            node = rotate_right(node);
        } 
    } else {
        if (node->left == NULL && node->right == NULL) {
            delete node;
            return NULL;
        } else if (node->left == NULL) {
            TreeNode<T>* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == NULL) {
            TreeNode<T>* temp = node->left;
            delete node;
            return temp;
        } else {
            if (get_node_height(node->right) >= get_node_height(node->left)) {
                node->val = node->right->val;
                node->right = _remove(node->right, node->val);
            } else {
                node->val = node->left->val;
                node->left = _remove(node->left, node->val);
            }
            update_node_height(node);
            return node;
        }
    }
    update_node_height(node);
    return node;
}
