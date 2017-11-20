/*CA HW1 task1 bintree.c
 *Design By Melico*/
#include <stdlib.h>
#include "bintree.h"

int compareInt(const void* a, const void* b)
{
    if (*(int*)a < *(int*)b)
    {
        return -1;
    }
    else if (*(int*)a > *(int*)b)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void* allocInt(const void * a)
{
    int* tmp = (int*) malloc (sizeof(int));
    *tmp = *(int*)a;
    return tmp;
}

void deleteInt(const void * a)
{
    free((int*)a);
}

void freeTree(Node* root, void (*del)(const void *))
{
    if (root == NULL)
    {
        return;
    }
    else
    {
        freeTree(root->right, (*del));
        freeTree(root->left, (*del));
        del(root->val);
        free(root);
    }
}

Node* insertNode(Node* root, void* val, int (*cmp)(const void* a, const void* b), void *(*alloc)(const void*))
{
    Node* parent = NULL;
    Node* cur = root;
    if (val == NULL)
    {
        return root;
    }
    while (cur != NULL)
    {
        switch (cmp(val, cur->val))
        {
            case 0:
                return cur;
            case 1:
                parent = cur;
                cur = cur->right;
                break;
            case -1:
                parent = cur;
                cur = cur->left;
                break;
        }
    }
    if(parent == NULL)
    {
        Node* new_tree = (Node*) malloc (sizeof(Node));
        new_tree->left = NULL;
        new_tree->right = NULL;
        new_tree->val = alloc(val);
        new_tree->parent = NULL;
        return new_tree;
    }
    else
    {
        cur = (Node*)malloc(sizeof(Node));
        cur->left = NULL;
        cur->right = NULL;
        cur->val = alloc(val);
        if (parent == NULL)
        {
            cur->parent = cur;
        }
        else
        {
            cur->parent = parent;
        }
        if (cmp(val, parent->val) == 1)
        {
            parent->right = cur;
        }
        else
        {
            parent->left = cur;
        }
    }
    return cur;
}

Node* searchNode(Node* root, void* val, int (*cmp)(const void* a, const void* b))
{
    Node* cur = root;
    if (val==NULL)
    {
        return NULL;
    }
    while (cur != NULL)
    {
        if (cmp(val, cur->val) == 1)
        {
            cur = cur->right;
        }
        else if(cmp(val, cur->val) == -1)
        {
            cur = cur->left;
        }
        else if(cmp(val,cur->val) == 0)
        {
            return cur;
        }
        else
        {
            break;
        }
    }
    return NULL;
}

Node* removeNode(Node* root, void* val, int (*cmp)(const void* a, const void* b), void (*del)(const void*))
{
    Node* node_root = root;
    Node* cur = root;
    if (val == NULL)
    {
        return root;
    }
    while (cur != NULL)
    {
        if (cmp(val, cur->val) == 1)
        {
            cur = cur->right;
        }
        else if (cmp(val, cur->val) == -1)
        {
            cur = cur->left;
        }
        else
        {
            break;
        }
    }
    if (cur == NULL)
    {
        return root;
    }
    if (cur->right==NULL && cur->left==NULL)
    {
        if (cur->parent == NULL)
        {
            node_root = NULL;
        }
        else
        {
            if (cmp(val, cur->parent->val) == 1)
            {
                cur->parent->right = NULL;
            }
            else if (cmp(val, cur->parent->val) == -1)
            {
                cur->parent->left = NULL;
            }
            else
            {
                node_root = NULL;
            }
        }
        cur->parent = NULL;
        cur->right = NULL;
        cur->left = NULL;
        del(cur->val);
        free(cur);
    }
    else if (cur->right!=NULL && cur->left==NULL)
    {
        if (cur->parent == NULL)
        {
            cur->right->parent = NULL;
            node_root = cur->right;
        }
        else
        {
            if (cmp(val, cur->parent->val) == 1)
            {
                cur->right->parent = cur->parent;
                cur->parent->right = cur->right;
            }
            else
            {
                cur->right->parent = cur->parent;
                cur->parent->left = cur->right;
            }
        }
        cur->parent = NULL;
        cur->right = NULL;
        cur->left = NULL;
        del(cur->val);
        free(cur);
    }
    else if (cur->right==NULL && cur->left!=NULL)
    {
        if (cur->parent == NULL)
        {
            cur->left->parent = cur->left;
            node_root = cur->left;
        }
        else
        {
            if (cmp(val, cur->parent->val) == 1)
            {
                cur->left->parent = cur->parent;
                cur->parent->right = cur->left;
            }
            else
            {
                cur->left->parent = cur->parent;
                cur->parent->left = cur->left;
            }
        }
        cur->parent = NULL;
        cur->right = NULL;
        cur->left = NULL;
        del(cur->val);
        free(cur);
    }
    else
    {
        int flag = 0;
        Node* tmp;
        Node* leftmost;
        tmp = cur;
        leftmost = cur->right;
        while (leftmost->left != NULL)
        {
            tmp = leftmost;
            leftmost = leftmost->left;
            flag = 1;
        }
        *(int*)cur->val = *(int*)leftmost->val;
        if (leftmost->right != NULL)
        {
            leftmost->right->parent = tmp;
        }
        if (flag == 1)
        {
            tmp->left = leftmost->right;
        }
        else
        {
            tmp->right = leftmost->right;
        }
        leftmost->parent = NULL;
        leftmost->left = NULL;
        leftmost->right = NULL;
        del(leftmost->val);
        free(leftmost);
    }
    return node_root;
}

