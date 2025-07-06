#pragma once
#include"struct_def.h"
void LL(RBTree** Root, RBTreeNode** cur)//对LL情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    father->color = BLACK;
    grandpa->color = RED;
    if (grandpa == (*Root)->root)//处理grandpa之前的情况
    {
        (*Root)->root = father;
        father->parent = NULL;
    }
    else {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = father;
        else grand_fa->left = father;
        father->parent = grand_fa;
    }
    //开始处理旋转
    grandpa->left = father->right;
    if (father->right != NULL) {
        father->right->parent = grandpa;
    }
    father->right = grandpa;
    grandpa->parent = father;
}
void RR(RBTree** Root, RBTreeNode** cur)//对RR情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    father->color = BLACK;
    grandpa->color = RED;
    if (grandpa == (*Root)->root)//处理grandpa之前的情况
    {
        (*Root)->root = father;
        father->parent = NULL;
    }
    else {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = father;
        else grand_fa->left = father;
        father->parent = grand_fa;
    }
    //开始处理旋转
    grandpa->right = father->left;
    if (father->left != NULL) {
        father->left->parent = grandpa;
    }
    father->left = grandpa;
    grandpa->parent = father;
}
void RL(RBTree** Root, RBTreeNode** cur)//对RL情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    (*cur)->color = BLACK;
    grandpa->color = RED;
    father->left = (*cur)->right;
    if ((*cur)->right)
        (*cur)->right->parent = father;
    (*cur)->right = father;
    father->parent = (*cur);
    if (grandpa == (*Root)->root)
    {
        (*Root)->root = *cur;
        (*cur)->parent = NULL;
    }
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = *cur;
        else grand_fa->left = *cur;
        (*cur)->parent = grand_fa;
    }
    grandpa->right = (*cur)->left;
    if ((*cur)->left)
        (*cur)->left->parent = grandpa;
    (*cur)->left = grandpa;
    grandpa->parent = *cur;
}
void LR(RBTree** Root, RBTreeNode** cur)//对LR情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    (*cur)->color = BLACK;
    grandpa->color = RED;
    father->right = (*cur)->left;
    if ((*cur)->left)
        (*cur)->left->parent = father;
    (*cur)->left = father;
    father->parent = *cur;
    if (grandpa == (*Root)->root)
    {
        (*Root)->root = *cur;
        (*cur)->parent = NULL;
    }
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = *cur;
        else grand_fa->left = *cur;
        (*cur)->parent = grand_fa;
    }
    grandpa->left = (*cur)->right;
    if ((*cur)->right)
        (*cur)->right->parent = grandpa;
    (*cur)->right = grandpa;
    grandpa->parent = *cur;
}
RBTreeNode* newNode(char key[20], RBTree** Root, OutboundOrder** ptr)
{
    RBTreeNode* tmp = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    (*Root)->root = tmp;
    strcpy(tmp->key, key);
    tmp->parent = tmp->left = tmp->right = NULL;
    tmp->color = BLACK;
    tmp->ptr = *ptr;
    RBTree* tmp01 = *Root;
    return tmp;
}
RBTree* fixInsert(RBTree** Root, RBTreeNode** cur)//Root->root是根结点
{
    //插入新节点默认为红节点，如果违反问题一定只有根黑，和红红两种情况
    if ((*cur) == (*Root)->root)//根黑
    {
        (*cur)->color = BLACK;
        return *Root;
    }
    //红红
    while ((*cur)->parent && (*cur)->parent->color == RED)
        //if ((*cur)->parent && (*cur)->parent->color == RED)
            //红红有很多种情况
    {
        //先找叔叔和爷爷
        RBTreeNode* uncle;
        RBTreeNode* grandpa;
        grandpa = (*cur)->parent->parent;
        if (!grandpa)
        {
            (*cur)->parent->color = BLACK;
            break;
        }
        if ((*cur)->parent == grandpa->right)
            uncle = grandpa->left;
        else uncle = grandpa->right;
        //判断叔叔是不是红色
        if (uncle != NULL && uncle->color == RED)
        {
            //在叔叔是红色的情况下，叔父爷变色，且cur指向爷爷,爷爷一定是黑色
            (*cur)->parent->color = uncle->color = BLACK;
            grandpa->color = RED;
            //*cur=grandpa;
            return fixInsert(Root, &grandpa);
        }
        else//叔叔是黑色(包含NULL)
        {
            //一共有四种情况。LL RR RL LR，四种方法有四种不同的转动方法
            if (grandpa->left == uncle) {//R
                if ((*cur)->parent->left == *cur)//RL
                {
                    RL(Root, cur);
                    break;
                }
                else {//RR
                    RR(Root, cur);
                    break;
                }
            }
            else {//L
                if ((*cur)->parent->left == *cur)//LL
                {
                    LL(Root, cur);
                    break;
                }
                else {//LR
                    LR(Root, cur);
                    break;
                }
            }
        }
    }
    (*Root)->root->color = BLACK;
    return *Root;

}
//加入新节点
RBTreeNode* Add_node(char key[20], RBTree** Root, OutboundOrder** ptr)
{
    RBTreeNode* cur = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    RBTreeNode* trace = (*Root)->root;
    strcpy(cur->key, key);
    cur->ptr = *ptr;
    cur->color = RED;
    cur->left = cur->right = NULL;
    int i = 1;
    while (i) {
        if (strcmp(key, trace->key) == 0)
            return NULL;
        if (strcmp(key, trace->key) > 0) {
            if (trace->right != NULL)
                trace = trace->right;
            else {
                trace->right = cur;
                i = 0;
            }
        }
        else {
            if (trace->left != NULL)
                trace = trace->left;
            else {
                trace->left = cur;
                i = 0;
            }
        }
    }
    cur->parent = trace;
    fixInsert(Root, &cur);//插入之后的调整
    return cur;
}
//三种遍历方法  先序遍历  中序遍历    后序遍历
void  Pre_order(RBTreeNode* root)//先序遍历   根左右
{
    if (!root)
        // printf("there is NULL!\n");
        return;
    printf(" \n%s\n", root->key);
    if (root->left != NULL)
        Pre_order(root->left);
    if (root->right != NULL)
        Pre_order(root->right);
}
void In_order(RBTreeNode* root)//中序遍历   左根右
{
    if (!root)
        // printf("there is NULL!\n");
        return;
    if (root->left != NULL)
        In_order(root->left);
    printf(" %s\n", root->key);
    // printf("%s\n", root->ptr->shelf);
    // printf("%s\n", root->ptr->order_id);
    if (root->right != NULL)
        In_order(root->right);
}
void Post_order(RBTreeNode* root)//后序遍历 左右根
{
    if (!root)
        // printf("there is NULL!\n");
        return;
    if (root->left != NULL)
        Post_order(root->left);
    if (root->right != NULL)
        Post_order(root->right);
    printf(" %s\n", root->key);
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
RBTreeNode* Find_key(char* key, RBTree* Root)//查找key值所对应的节点坐标
//返回NULL就代表这个红黑树里面没有这个内容
{
    RBTreeNode* cur = Root->root;
    while (cur != NULL && strcmp(cur->key, key) != 0)
    {
        if (strcmp(cur->key, key) > 0)
            cur = cur->left;
        else if (strcmp(cur->key, key) < 0)
            cur = cur->right;
        else break;
    }
    return cur;
}
void LL1(RBTree** Root, RBTreeNode** cur)//对LL情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    if (grandpa == (*Root)->root)//处理grandpa之前的情况
    {
        (*Root)->root = father;
        father->parent = NULL;
    }
    else {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = father;
        else grand_fa->left = father;
        father->parent = grand_fa;
    }
    //开始处理旋转
    grandpa->left = father->right;
    if (father->right != NULL) {
        father->right->parent = grandpa;
    }
    father->right = grandpa;
    grandpa->parent = father;
}
void RR1(RBTree** Root, RBTreeNode** cur)//对RR情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    if (grandpa == (*Root)->root)//处理grandpa之前的情况
    {
        (*Root)->root = father;
        father->parent = NULL;
    }
    else {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = father;
        else grand_fa->left = father;
        father->parent = grand_fa;
    }
    //开始处理旋转
    grandpa->right = father->left;
    if (father->left != NULL) {
        father->left->parent = grandpa;
    }
    father->left = grandpa;
    grandpa->parent = father;
}
void RL1(RBTree** Root, RBTreeNode** cur)//对RL情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    if ((*cur)->right)
        (*cur)->right->parent = father;
    (*cur)->right = father;
    father->parent = (*cur);
    if (grandpa == (*Root)->root)
    {
        (*Root)->root = *cur;
        (*cur)->parent = NULL;
    }
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = *cur;
        else grand_fa->left = *cur;
        (*cur)->parent = grand_fa;
    }
    grandpa->right = (*cur)->left;
    if ((*cur)->left)
        (*cur)->left->parent = grandpa;
    (*cur)->left = grandpa;
    grandpa->parent = *cur;
}
void LR1(RBTree** Root, RBTreeNode** cur)//对LR情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    father->right = (*cur)->left;
    if ((*cur)->left)
        (*cur)->left->parent = father;
    (*cur)->left = father;
    father->parent = *cur;
    if (grandpa == (*Root)->root)
    {
        (*Root)->root = *cur;
        (*cur)->parent = NULL;
    }
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = *cur;
        else grand_fa->left = *cur;
        (*cur)->parent = grand_fa;
    }
    grandpa->left = (*cur)->right;
    if ((*cur)->right)
        (*cur)->right->parent = grandpa;
    (*cur)->right = grandpa;
    grandpa->parent = *cur;
}
//处理双黑节点的函数
void Resolve_double_black(RBTree** Root, double_black** Point, int mark)
//mark==1 代表这个节点最后是要被删掉的
//mark==0 代表这个节点是从下面传上来的
{
    //cur是目标删除节点
    RBTreeNode* cur = (*Point)->point;
    if (cur == (*Root)->root || cur->color == RED)
    {
        free(*Point);
        cur->color = BLACK;
        return;
    }
    //获得兄弟节点和父亲节点
    RBTreeNode* father = cur->parent;
    RBTreeNode* siblings = (father->right == cur) ? father->left : father->right;
    //如果兄弟是红色节点-->父亲节点一定是黑色
    if (siblings && siblings->color == RED)
    {
        //兄父变色
        siblings->color = BLACK;
        father->color = RED;
        //朝着双黑方向旋转调整
        //一共只有两种情况
        //准备工作
        if ((*Root)->root == father) {
            (*Root)->root = siblings;
            siblings->parent = NULL;
        }
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
            if (siblings->right)
                siblings->right->parent = father;
            siblings->right = father;
            father->parent = siblings;
        }
        else//cur在左子树
        {
            father->right = siblings->left;
            if (siblings->left)
                siblings->left->parent = father;
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

            //用 父亲，兄弟和他的红孩子判断是RR LL RL LR的哪一种
            //LL/RR型：r变s，s变p，p变黑 + 旋转（双黑变单黑)
            //LR/RL型：r变p，p变黑 + 旋转
            if (father->left == siblings)
            {//L
                if(mark)
                father->right = NULL;//可能有问题
                if (siblings->left != NULL && siblings->left->color == RED)
                {//LL
                    Red = siblings->left;
                    Red->color = siblings->color;
                    siblings->color = father->color;
                    father->color = BLACK;
                    LL1(Root, &Red);
                }
                else
                {//LR
                    Red = siblings->right;
                    Red->color = father->color;
                    father->color = BLACK;
                    LR1(Root, &Red);
                }
            }
            else {//R
                if(mark)
                father->left = NULL;//可能有问题
                if (siblings->right != NULL && siblings->right->color == RED)
                {//RR
                    Red = siblings->right;
                    Red->color = siblings->color;
                    siblings->color = father->color;
                    father->color = BLACK;
                    RR1(Root, &Red);
                }
                else {
                    //RL
                    Red = siblings->left;
                    Red->color = father->color;
                    father->color = BLACK;
                    RL1(Root, &Red);
                }
            }
            if (mark == 1) {
                if (cur == father->left)
                    father->left = NULL;
                else father->right = NULL;

                free(cur);
            }
            free(*Point);
            return;
        }
        else {//兄弟是黑节点，但是兄弟没有红孩子->如果只有一个非NULL的黑子节点不可能，结果就是两个NULL
            // ➔ 兄弟变红，双黑上移父节点————》遇红或根变单黑
            siblings->color = RED;
            (*Point)->point = father;
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
RBTree* Delete(RBTreeNode** cur, RBTree** Root) {
    RBTreeNode* tmp = *cur;
    if (*cur == NULL)
        return *Root;
    RBTreeNode* father = (*cur)->parent;
    if (*cur == (*Root)->root) {
        if (!((*cur)->left || (*cur)->right)) {
            free((*cur));
            (*Root)->root = NULL;
            return (*Root);
        }
        else if ((*cur)->left && !(*cur)->right) {
            RBTreeNode* child = (*cur)->left;
            (*Root)->root = child;
            child->parent = NULL;
            child->color = BLACK;
            free(*cur);
            return *Root;
        }
        else if ((*cur)->right && !(*cur)->left) {
            RBTreeNode* child = (*cur)->right;
            (*Root)->root = child;
            child->parent = NULL;
            child->color = BLACK;
            free((*cur));
            return (*Root);
        }
        else {
            RBTreeNode* alter = (*cur)->left;
            while (alter->right != NULL)
                alter = alter->right;
            strcpy((*cur)->key, alter->key);
            (*cur)->ptr = alter->ptr;
            return Delete(&alter, Root);
        }
    }
    if ((*cur)->left == NULL && (*cur)->right != NULL) {
        RBTreeNode* child = (*cur)->right;
        if (father->left == *cur)
            father->left = child;
        else
            father->right = child;
        child->parent = father;
        if ((*cur)->color == BLACK && child->color == BLACK) {
            double_black* db = (double_black*)malloc(sizeof(double_black));
            db->point = child;
            Resolve_double_black(Root, &db, 0);
        }
        else {
            child->color = BLACK;
        }
        free(*cur);
        return (*Root);
    }
    else if ((*cur)->right == NULL && (*cur)->left != NULL) {
        RBTreeNode* child = (*cur)->left;
        if (father->left == *cur)
            father->left = child;
        else
            father->right = child;
        child->parent = father;
        if ((*cur)->color == BLACK && child->color == BLACK) {
            double_black* db = (double_black*)malloc(sizeof(double_black));
            db->point = child;
            Resolve_double_black(Root, &db, 0);
        }
        else {
            child->color = BLACK;
        }
        free((*cur));
        return (*Root);
    }
    else if ((*cur)->right == NULL && (*cur)->left == NULL) {
        if ((*cur)->color == RED) {
            if (*cur == father->left)
                father->left = NULL;
            else
                father->right = NULL;
            free((*cur));
            return (*Root);
        }
        else {
            double_black* This_Point = (double_black*)malloc(sizeof(double_black));
            This_Point->point = *cur;
            Resolve_double_black(Root, &This_Point, 1);
            return (*Root);
        }
    }
    else {
        RBTreeNode* alter = (*cur)->left;
        while (alter->right != NULL)
            alter = alter->right;
        strcpy((*cur)->key, alter->key);
        (*cur)->ptr = alter->ptr;
        return Delete(&alter, Root);
    }
    return (*Root);
}
void Clear_RBTREE(RBTreeNode* root)
{
    if (root == NULL)
        return;
    if (root->left != NULL)
        Clear_RBTREE(root->left);
    if (root->right != NULL)
        Clear_RBTREE(root->right);
    free(root);
}
