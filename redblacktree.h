#pragma once
#include<stdio.h>
#include<stdlib.h>
typedef enum { BLACK, RED }Color;
typedef struct RBTreeNode {//红黑树节点
    int key;
    Color color;            // 节点颜色（RED/BLACK）
    struct RBTreeNode* left;  // 左子节点指针
    struct RBTreeNode* right; // 右子节点指针
    struct RBTreeNode* parent;// 父节点指针
    void* ptr;
} RBTreeNode;
typedef struct RBTree {
    RBTreeNode* root;  // 指向根节点的指针
} RBTree;
typedef struct double_black {
    RBTreeNode* point;
}double_black;
//创建红黑树
//创建的是根结点
//下面的四种LL,RR,RK,LR是针对插入新节点的状况的插入策略，和颜色改变策略
void LL(RBTree* Root, RBTreeNode* cur)//对LL情况的旋转
{
    RBTreeNode* father = cur->parent;
    RBTreeNode* grandpa = father->parent;
    father->color = BLACK;
    grandpa->color = RED;
    if (grandpa == Root->root)//处理grandpa之前的情况
        Root->root = father;
    else {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = father;
        else grand_fa->left = father;
        father->parent = grand_fa;
    }
    //开始处理旋转
    if (father->right != NULL) {
        grandpa->left = father->right;
        father->right->parent = grandpa;
    }
    father->right = grandpa;
    grandpa->parent = father;
}
void RR(RBTree* Root, RBTreeNode* cur)//对RR情况的旋转
{
    RBTreeNode* father = cur->parent;
    RBTreeNode* grandpa = father->parent;
    father->color = BLACK;
    grandpa->color = RED;
    if (grandpa == Root->root)//处理grandpa之前的情况
        Root->root = father;
    else {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = father;
        else grand_fa->left = father;
        father->parent = grand_fa;
    }
    //开始处理旋转
    if (father->left != NULL) {
        grandpa->right = father->left;
        father->left->parent = grandpa;
    }
    father->left = grandpa;
    grandpa->parent = father;
}
void RL(RBTree* Root, RBTreeNode* cur)//对RL情况的旋转
{
    RBTreeNode* father = cur->parent;
    RBTreeNode* grandpa = father->parent;
    cur->color = BLACK;
    grandpa->color = RED;
    cur->right = father;
    father->parent = cur;
    if (grandpa == Root->root)
        Root->root = cur;
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = cur;
        else grand_fa->left = cur;
        cur->parent = grand_fa;
    }
    cur->left = grandpa;
    grandpa->parent = cur;
}
void LR(RBTree* Root, RBTreeNode* cur)//对LR情况的旋转
{
    RBTreeNode* father = cur->parent;
    RBTreeNode* grandpa = father->parent;
    cur->color = BLACK;
    grandpa->color = RED;
    cur->left = father;
    father->parent = cur;
    if (grandpa == Root->root)
        Root->root = cur;
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = cur;
        else grand_fa->left = cur;
        cur->parent = grand_fa;
    }
    cur->right = grandpa;
    grandpa->parent = cur;
}
RBTreeNode* newNode(int key,RBTree*Root)
{
    RBTreeNode* tmp = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    if (tmp == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    Root->root = tmp;
    tmp->key = key;
    tmp->parent = tmp->left = tmp->right = NULL;
    tmp->color = BLACK;
    return tmp;
}
RBTree* fixInsert(RBTree* Root, RBTreeNode* cur)//Root->root是根结点
{
    //插入新节点默认为红节点，如果违反问题一定只有根黑，和红红两种情况
    if (cur == Root->root)//根黑
    {
        cur->color = BLACK;
        return Root;
    }
    //红红
    if (cur->parent->color == RED)
        //红红有很多种情况
    {
        //先找叔叔和爷爷
        RBTreeNode* uncle;
        RBTreeNode* grandpa = cur->parent->parent;
        if (cur->parent == grandpa->right)
            uncle = grandpa->left;
        else uncle = grandpa->right;
        //判断叔叔是不是红色
        if (uncle != NULL && uncle->color == RED)
        {
            //在叔叔是红色的情况下，叔父爷变色，且cur指向爷爷,爷爷一定是黑色
            cur->parent->color = uncle->color = BLACK;
            if (Root->root != grandpa)
                grandpa->color = RED;
            return fixInsert(Root, grandpa);
        }
        else//叔叔是黑色(包含NULL)
        {
            //一共有四种情况。LL RR RL LR，四种方法有四种不同的转动方法
            if (grandpa->left == uncle) {//R
                if (cur->parent->left == cur)//RL
                {
                    RL(Root, cur);
                    return Root;
                }
                else {//RR
                    RR(Root, cur);
                    return Root;
                }
            }
            else {//L
                if (cur->parent->left == cur)//LL
                {
                    LL(Root, cur);
                    return Root;
                }
                else {//LR
                    LR(Root, cur);
                    return Root;
                }
            }
        }
    }
    return Root;

}
//加入新节点
RBTreeNode* Add_node(int key, RBTree* Root)
{
    RBTreeNode* head = Root->root;//红黑树的根节点
    RBTreeNode* trace = head;
    RBTreeNode* cur = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    if (cur == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    cur->key = key;
    cur->color = RED;
    cur->left = cur->right = NULL;
    while (1) {
        if (key > trace->key) {
            if (trace->right != NULL)
                trace = trace->right;
            else {
                trace->right = cur;
                break;
            }
        }
        else {
            if (trace->left != NULL)
                trace = trace->left;
            else {
                trace->left = cur;
                break;
            }
        }
    }
    cur->parent = trace;
    fixInsert(Root, cur);//插入之后的调整
    return cur;
}
//三种遍历方法  先序遍历  中序遍历    后序遍历
void  Pre_order(RBTreeNode* root)//先序遍历   根左右
{
    printf(" %d", root->key);
    if (root->left != NULL)
        Pre_order(root->left);
    if (root->right != NULL)
        Pre_order(root->right);
}
void In_order(RBTreeNode* root)//中序遍历   左根右
{
    if (root->left != NULL)
        In_order(root->left);
    printf(" %d", root->key);
    if (root->right != NULL)
        In_order(root->right);
}
void Post_order(RBTreeNode* root)//后序遍历 左右根
{
    if (root->left != NULL)
        Post_order(root->left);
    if (root->right != NULL)
        Post_order(root->right);
    printf(" %d", root->key);
}
//红黑树的删除源于二叉搜索树的删除
//左右子树都有，用他左子树中的最大值或右子树的最小值代替他
//只有左子树，只有右子树   --》代替后变黑  这种情况只有father是黑的，子树cur是红的
//没有孩子
//红节点   删除后无需任何改变
//黑节点。删除变双黑
//当兄弟是黑色：
//  ├─ 子情况A：兄弟至少有一个红孩子
//  │   ├─ LL/RR型：r变s，s变p，p变黑 + 旋转（双黑变单黑）
//  │   └─ LR/RL型：r变p，p变黑 + 旋转
//  │
//  └─ 子情况B：兄弟孩子全黑
//      ➔ 兄弟变红，双黑上移父节点————》遇红或根变单黑
//当兄弟是红色：
//  ➔ 兄父变色，朝双黑方向旋转
//  ➔ 保持双黑继续调整
//下面的四个RR1，LL1，RL1，LR1是针对处理双黑节点的旋转方法
//因为我们在处理双黑节点的部分和处理插入新节点的颜色的改变原理不同
void LL1(RBTree* Root, RBTreeNode* cur)//对LL情况的旋转
{
    RBTreeNode* father = cur->parent;
    RBTreeNode* grandpa = father->parent;
    if (grandpa == Root->root)//处理grandpa之前的情况
        Root->root = father;
    else {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = father;
        else grand_fa->left = father;
        father->parent = grand_fa;
    }
    //开始处理旋转
    if (father->right != NULL) {
        grandpa->left = father->right;
        father->right->parent = grandpa;
    }
    father->right = grandpa;
    grandpa->parent = father;
}
void RR1(RBTree* Root, RBTreeNode* cur)//对RR情况的旋转
{
    RBTreeNode* father = cur->parent;
    RBTreeNode* grandpa = father->parent;
    if (grandpa == Root->root)//处理grandpa之前的情况
        Root->root = father;
    else {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = father;
        else grand_fa->left = father;
        father->parent = grand_fa;
    }
    //开始处理旋转
    if (father->left != NULL) {
        grandpa->right = father->left;
        father->left->parent = grandpa;
    }
    father->left = grandpa;
    grandpa->parent = father;
}
void RL1(RBTree* Root, RBTreeNode* cur)//对RL情况的旋转
{
    RBTreeNode* father = cur->parent;
    RBTreeNode* grandpa = father->parent;
    cur->right = father;
    father->parent = cur;
    if (grandpa == Root->root)
        Root->root = cur;
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = cur;
        else grand_fa->left = cur;
        cur->parent = grand_fa;
    }
    cur->left = grandpa;
    grandpa->parent = cur;
}
void LR1(RBTree* Root, RBTreeNode* cur)//对LR情况的旋转
{
    RBTreeNode* father = cur->parent;
    RBTreeNode* grandpa = father->parent;
    cur->left = father;
    father->parent = cur;
    if (grandpa == Root->root)
        Root->root = cur;
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = cur;
        else grand_fa->left = cur;
        cur->parent = grand_fa;
    }
    cur->right = grandpa;
    grandpa->parent = cur;
}
//处理双黑节点的函数
void Resolve_double_black(RBTree* Root, double_black* Point, int mark)
//mark==1 代表这个节点最后是要被删掉的
//mark==0 代表这个节点是从下面传上来的
{
    //cur是目标删除节点
    RBTreeNode* cur = Point->point;
    if (cur == Root->root || cur->color == RED)
    {
        free(Point);
        cur->color = BLACK;
        return;
    }
    //获得兄弟节点和父亲节点
    RBTreeNode* father = cur->parent;
    RBTreeNode* siblings = (father->right == cur) ? father->left : father->right;
    //如果兄弟是红色节点-->父亲节点一定是黑色
    if (siblings->color == RED)
    {
        //兄父变色
        siblings->color = BLACK;
        father->color = RED;
        //朝着双黑方向旋转调整
        //一共只有两种情况
        //准备工作
        if (Root->root == father)
            Root->root = siblings;
        else {
            RBTreeNode* grandpa = father->parent;
            if (grandpa->left == father)
                grandpa->left = siblings;
            else grandpa->right = siblings;
            siblings->parent = grandpa;
        }
        if (cur == father->right)//如果在右子树
        {
            //开始旋转
            father->left = siblings->right;
            siblings->right = father;
            father->parent = siblings;

        }
        else//cur在左子树
        {
            father->right = siblings->left;
            siblings->left = father;
            father->parent = siblings;
        }
        Resolve_double_black(Root, Point, mark);
        return;
    }
    //如果兄弟是黑色节点
    else {
        //如果他有至少有一个红孩子
        RBTreeNode* Red;//红孩子
        if ((siblings->left != NULL && siblings->left->color == RED) || (siblings->right != NULL && siblings->right->color == RED))
        {
            free(Point);
            //用 父亲，兄弟和他的红孩子判断是RR LL RL LR的哪一种
            //LL/RR型：r变s，s变p，p变黑 + 旋转（双黑变单黑)
            //LR/RL型：r变p，p变黑 + 旋转
            if (father->left == siblings)
            {//L
                father->right = NULL;
                if (siblings->left != NULL && siblings->left->color == RED)
                {//LL
                    Red = siblings->left;
                    Red->color = BLACK;
                    siblings->color = father->color;
                    father->color = BLACK;
                    LL1(Root, Red);
                }
                else
                {//LR
                    Red = siblings->right;
                    Red->color = father->color;
                    father->color = BLACK;
                    LR1(Root, Red);
                }
            }
            else {//R
                father->left = NULL;
                if (siblings->right != NULL && siblings->right->color == RED)
                {//RR
                    Red = siblings->right;
                    Red->color = BLACK;
                    siblings->color = father->color;
                    father->color = BLACK;
                    RR1(Root, Red);
                }
                else {
                    //RL
                    Red = siblings->left;
                    Red->color = father->color;
                    father->color = BLACK;
                    RL1(Root, Red);
                }
            }
            if (mark == 1) {
                if (cur == father->left)
                    father->left = NULL;
                else father->right = NULL;
                free(cur);
            }
            return;
        }
        else {//兄弟是黑节点，但是兄弟没有红孩子->如果只有一个非NULL的黑子节点不可能，结果就是两个NULL
            // ➔ 兄弟变红，双黑上移父节点————》遇红或根变单黑
            siblings->color = RED;
            Point->point = father;
            if (father->left == cur)
                father->left = NULL;
            else father->right = NULL;
            if (mark == 1) {
                if (cur == father->left)
                    father->left = NULL;
                else father->right = NULL;
                free(cur);
            }
            Resolve_double_black(Root, Point, 0);
            return;
        }
    }
}
RBTreeNode* Find_key(int key, RBTree* Root)//查找key值所对应的节点坐标
//返回NULL就代表这个红黑树里面没有这个内容
{
    RBTreeNode* cur = Root->root;
    while (cur != NULL && cur->key != key)
    {
        if (cur->key > key)
            cur = cur->left;
        else cur = cur->right;
    }
    return cur;
}
RBTree* Delete(RBTreeNode* cur, RBTree* Root)//红黑树的删除
{
    if (cur == NULL)
        return NULL;
    //return Root;
    //cur是目标删除节点
  //  RBTreeNode*cur1=cur;//cur的副本
    //特殊处理根节点
    if (cur == Root->root)
    {
        if (!(cur->left || cur->right))//单个节点
        {
            free(cur);
            Root->root = NULL;
            return Root;
        }
        else {//有子树
            if (cur->left && !cur->right)//只有左子树
            {
                cur->left->color = BLACK;
                Root->root = cur->left;
                free(cur);
                return Root;
            }
            else if (cur->right && !cur->left)//只有右子树
            {
                cur->right->color = BLACK;
                Root->root = cur->right;
                free(cur);
                return Root;
            }
            else {//左右子树都有
                RBTreeNode* alter = cur;//代替
                alter = alter->left;
                while (alter->right != NULL)
                    alter = alter->right;
                cur->key = alter->key;
                //删除alter节点
                Delete(alter, Root);
                return  Root;
            }
        }
    }
    RBTreeNode* father = cur->parent;
    //RBTreeNode*siblings=(father->right==cur)?father->left:father->right;
    //获得兄弟节点和父亲节点和爷爷节点
    if (cur->left == NULL && cur->right != NULL)//只有右子树
    {
        if (father->left == cur)
            father->left = cur->right;
        else father->right = cur->right;
        cur->right->parent = father;
        free(cur);
        return Root;
    }
    else if (cur->right == NULL && cur->left != NULL)//只有左子树
    {
        if (father->left == cur)
            father->left = cur->left;
        else father->right = cur->left;
        cur->left->parent = father;
        free(cur);
        return Root;
    }
    else if (cur->right == NULL && cur->left == NULL)//没有子节点
    {
        if (cur->color == RED) {//红节点
            if (cur == father->left)
                father->left = NULL;
            else father->right = NULL;
            free(cur);
            return Root;
        }
        else {//黑节点
            //删除黑节点会出现双黑标记
            double_black* This_Point = (double_black*)malloc(sizeof(double_black));
            This_Point->point = cur;
            Resolve_double_black(Root, This_Point, 1);//处理双黑节点,也需要负责删除cur节点
            return Root;
        }
    }
    else//左右子节点都有
    {
        //可以选择找左子树中最大的
        RBTreeNode* alter = cur;//代替
        alter = alter->left;
        while (alter->right != NULL)
            alter = alter->right;
        cur->key = alter->key;
        //删除alter节点
        Delete(alter, Root);
        return Root;
    }
    return Root;
}
void Clear_RBTREE(RBTreeNode* root)
{
    if (root == NULL)
        return;
    if (root->left != NULL)
        Clear_RBTREE(root->left);
    if (root->right != NULL)
        Clear_RBTREE(root->right);
    free(root->ptr);
    free(root);
}
