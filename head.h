/*
#pragma once
#include"calculate.h"
#include"console.h"
*/
#pragma once
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>  // 提供输入输出函数，如printf、scanf、fopen、fprintf、fclose等
#include <time.h>   // 提供时间相关的函数和数据类型，如time、difftime
#include <string.h> // 提供字符串处理函数，如strcmp
#include <windows.h> // 用于 Windows 系统下的高精度时间获取
#include<errno.h>
#include <sys/stat.h>
#include <direct.h>
#include<stdlib.h>
void format_time(time_t timestamp, char* buffer);
void query_package_exceptions(const char* package_id);
#define OVERDUE_TIME_THRESHOLD 86400  // 一天的秒数，可根据实际情况调整
typedef enum PackageExceptionType {
    LOST,           // 丢失
    DAMAGED,        // 损坏
    OVERDUE_STAY,   // 超期滞留
    INFORMATION_ERROR // 信息错误
} PackageExceptionType;
typedef struct PackageException {
    PackageExceptionType type;
    time_t exception_time;
    char package_id[40];
    char description[50]; // 异常描述
} PackageException;

typedef enum UserType {
    STUDENT,    // 学生 0.9
    TEACHER,    // 教师 0.85
    VIP,        // VIP 0.75
    ENTERPRISE, // 企业 0.8
    WORKER      // 务工 1.0
} UserType;
// 包裹类型枚举
typedef enum PackageType {
    LARGE,      // 大件 +20%
    FRAGILE,    // 易碎 +15%
    VALUABLE,   // 贵重 +25%
    DOCUMENT,   // 文件 -10%
    STANDARD    // 普通 0%
} PackageType;
struct Goods {//货物
    char name[50];//名字
    PackageType p_type;
    double weight;//重量，kg
    // struct Logistic* log;//物流
     //char pos[20];//货物位置
};
typedef struct Goods goods;
typedef struct {
    char order_id[20];    // 订单号
    char phone[20];       // 联系电话
    char shelf[6];//货物柜
    time_t create_time;   // 订单创建时间戳
    time_t outbound_time; // 出库时间戳
    int status;           // 状态标记（0=待出库，1=已出库）
    goods* Good;//货物订单
} OutboundOrder;


void log_exception(const PackageException* exception) {
    FILE* log_file = fopen("exception.log", "a"); // 追加模式
    if (log_file != NULL) {
        char time_str[20];
        format_time(exception->exception_time, time_str);

        //const 
        char* type_str;
        switch (exception->type) {
        case LOST:
            type_str = "丢失";
            break;
        case DAMAGED:
            type_str = "损坏";
            break;
        case OVERDUE_STAY:
            type_str = "超期滞留";
            break;
        case INFORMATION_ERROR:
            type_str = "信息错误";
            break;
        default:
            type_str = "未知异常";
            break;
        }

        fprintf(log_file, "[%s] %s - 包裹:%s 描述:%s\n",
            time_str, type_str, exception->package_id, exception->description);
        fclose(log_file);
    }
}




//#include <pthread.h> // 提供线程相关的函数和数据类型，用于创建和管理线程#ifdef _WIN32
   // Windows-specific code
typedef struct  List list;
struct List {
    char mark[20];
    OutboundOrder* data;
    list* pre;//前一项
    list* next;//后一项
};

static void Isolate_list(list* temp, list* tail);
//type 1 第2种链表,head不存内容
void Add_list(list** head, OutboundOrder** ndata, list** tail, int* length)//尾插法
{
    list* temp = (list*)malloc(sizeof(list));
    strcpy(temp->mark, (*ndata)->order_id);
    temp->data = *ndata;
    temp->next = NULL;
    temp->pre = NULL;
    if ((*head)->next == NULL)
        *tail = (*head)->next = temp;
    else {
        (*tail)->next = temp;
        temp->pre = *tail;
        *tail = temp;
    }
    (*length) += 1;;
}
list* Find_list(list* head, int num)//返回第num个链表的位置
{
    //num等于零的时候相当于返回head指针
    list* temp = head;
    for (int i = 0; i < num; i++) temp = temp->next;
    return temp;
}
void Delete_list(list** p, list** head, list** tail, int* length)
{
    if (*p == *head)
        perror("You cannot delete head ptr.");
    if (*length == 1)
        (*head)->next = (*tail) = NULL;
    else
    {
        if ((*p)->next)
        {
            (*p)->next->pre = (*p)->pre;
            (*p)->pre->next = (*p)->next;
        }
        else
        {
            *tail = (*p)->pre;
            (*p)->pre->next = NULL;
        }
    }
    (*length) -= 1;
    free(*p);
}
void Insert_list01(list* prev, list* temp, list* tail)//第一种插入方法
{//prev插入位置前一项的指针  temp是要进行插入操作的指针
    Isolate_list(temp, tail);//隔离temp
    if (prev->next)
    {
        list* prev__next = prev->next;
        prev__next->pre = temp;
        temp->next = prev__next;
    }
    else
    {
        temp->next = NULL;
        tail = temp;
    }
    prev->next = temp;
    temp->pre = prev;

}
void Insert_list02(list* prev, OutboundOrder* ndata, int length, list* tail)//第二种插入方法//可以用它来实现头插法
{//prev插入位置前一项的指针  data是新生成的链表的data域
    list* temp = (list*)malloc(sizeof(list));
    temp->data = ndata;
    if (prev->next)
    {
        list* prev__next = prev->next;
        prev__next->pre = temp;
        temp->next = prev__next;
    }
    else
    {
        temp->next = NULL;
        tail = temp;
    }
    prev->next = temp;
    temp->pre = prev;
    length++;
}
void Clear_list(list* head, int* length)
{
    list* temp = head;
    while (temp != NULL)
    {
        list* temp01 = temp;
        temp = temp->next;
        free(temp01);
    }
    head->next = NULL;
    *length = 0;
}
static void Isolate_list(list* temp, list* tail) {//把temp这个链表的部分隔离出来
    //static  说明只能在这个头文件里面使用
    list* temp__next = temp->next;
    list* temp__pre = temp->pre;
    if (!temp->next)
        tail = temp__pre;
    else temp__next->pre = temp__pre;
    temp__pre->next = temp__next;
    return;
}
void Exchange_list(list* a, list* b, list* tail) {//交换a和b
    //首先确定一点，a和b是否为head->next对各种操作没有影响
    //需要特判的地方有 a/b为尾指针，a/b相连,a/b既有尾指针，有有相连的情况
    if (a == b)return;
    if (a->pre == b || a == tail)//都让a在b的前面
    {
        list* temp = a;
        a = b;
        b = temp;
    }
    list* a_pre = a->pre;
    list* a_next = a->next;
    list* b_pre = b->pre;
    list* b_next = b->next;
    int mark = (b == tail);//标记初始位置b是否为尾指针
    if (a->next != b)
    {
        a_next->pre = b;
        b->next = a_next;
        b_pre->next = a;
        a->pre = b_pre;
    }
    else
    {
        b->next = a;
        a->pre = b;
    }

    b->pre = a_pre;
    a_pre->next = b;
    a->next = b_next;
    if (mark)
        b_next->pre = a;
}
list* Find_char_list(char mark[], list* head)
{
    list* temp = head;
    while (temp != NULL && strcmp(temp->mark, mark) != 0)
        temp = temp->next;
    return temp;
}
int Empty_list(list* head)
{
    return head->next == NULL;
}

//--------------------------------------------------------------------------------------------
struct Logistic;
struct User;
struct Order;
/**
 需要实现的功能
 更新物流的链表
 有新货物的时候，需要对应用户，仓库，物流管理
 接收到货物时，需要更新物流，并且给user发消息
 统计库存地点是否有充足的货物存放
 发货和取货之后都要进行更新
 show 物流轨迹
 超出一定时间，如果用户没有取货，要再发一次消息
 查询货物 需要显示物流的轨迹
 快递大小、保存、扬坏或丢失、误
 领或冒锈、拒收

 */

typedef struct User {//用户
    char tel[12];//还需要手机号和联系方式
    UserType u_type;//不同级别用户
    char name[30];
    char mima[20];
    //struct Order* ord;//用户未取包裹的链头
    //初始化的时候把下面的三个指针初始化一下
    //ord tail 是NULL,head 是给一个malloc 删除的时候也需要释放head分配的内存
    list* head;//用户用来维护包裹链表的两个指针
    list* tail;
    int length;//链表的长度初始化为零
    struct User* next;
}User;
typedef struct HashTable {
    User** buckets;    // 哈希桶数组
    int size;              // 哈希表容量（建议选素数，如1009）
    int count;             // 当前存储用户数量
} HashTable;
// 将手机号转换为哈希索引
unsigned int hash_func(const char* phone, int table_size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *phone++)) {
        hash = ((hash << 5) + hash) + c; // DJB2算法变形
    }
    return hash % table_size;
}
HashTable* hash_init(int size) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->size = size;
    ht->count = 0;
    ht->buckets = (User**)calloc(size, sizeof(User));
    // ht->buckets = (User**)calloc(size, sizeof(User*));
    return ht;
}
HashTable* ht;
void hash_insert(HashTable* ht, const char* phone, const char* name, const char* mima, const UserType  type) {
    unsigned int index = hash_func(phone, ht->size);
    User* node = ht->buckets[index];
    // 检查手机号是否已存在
    while (node) {
        if (strcmp(node->tel, phone) == 0) {
            printf("手机号 %s 已注册！\n", phone);
            return;
        }
        node = node->next;
    }
    // 创建新节点（头插法）
    User* new_node = (User*)malloc(sizeof(User));
    strcpy(new_node->tel, phone);
    strcpy(new_node->name, name);
    strcpy(new_node->mima, mima);
    new_node->u_type = type;
    new_node->head = (list*)malloc(sizeof(list));
    new_node->tail = new_node->head->next = NULL;
    new_node->length = 0;
    new_node->next = NULL;
    if (type == VIP) {
        new_node->next = ht->buckets[index];
        ht->buckets[index] = new_node;
    }
    else {
        if (ht->buckets[index] == NULL) {
            ht->buckets[index] = new_node;
        }
        else {
            User* current = ht->buckets[index];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_node;
            new_node->next = NULL;
        }
    }
    ht->count++;
}
User* hash_search(HashTable* ht, const char* phone) {
    if (!ht)
        return NULL;
    unsigned int index = hash_func(phone, ht->size);
    User* node = ht->buckets[index];
    printf("hello world!");
    while (node) {
        if (strcmp(node->tel, phone) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL; // 未找到
}
int hash_delete(HashTable* ht, const char* phone) {
    unsigned int index = hash_func(phone, ht->size);
    User* prev = NULL;
    User* curr = ht->buckets[index];

    while (curr) {
        if (strcmp(curr->tel, phone) == 0) {
            if (prev) {
                prev->next = curr->next;
            }
            else {
                ht->buckets[index] = curr->next;
            }
            Clear_list(curr->head, &curr->length);
            free(curr->head);
            free(curr);
            ht->count--;
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0; // 删除失败
}
void hash_save(HashTable* ht, const char* filename) {
    if (ht == NULL) return;
    FILE* fp = fopen(filename, "w");
    if (!fp) return;

    for (int i = 0; i < ht->size; i++) {
        User* node = ht->buckets[i];
        while (node) {
            fwrite(node->tel, sizeof(node->tel), 1, fp);
            fwrite(&node->u_type, sizeof(node->u_type), 1, fp);
            fwrite(node->name, sizeof(node->name), 1, fp);
            node = node->next;
        }
    }
    fclose(fp);
}
void hash_load(HashTable* ht, const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) return;

    User temp;
    while (fread(&temp, sizeof(User), 1, fp)) {
        hash_insert(ht, temp.tel, temp.name, temp.mima, temp.u_type);
    }
    fclose(fp);
}

float calculateMoney(User* user, struct Goods* goods, float base_prise)
{
    float userDiscount[5] = { 0.85,0.9,0.75,0.8 };
    float packageDiscount[5] = { 1.2,1.15,1.25,0.9,1.0 };
    float weightDiscount;//权重
    if (goods->weight < 1)
        weightDiscount = 1;
    else if (goods->weight < 5)
        weightDiscount = 1.2;
    else if (goods->weight < 10)
        weightDiscount = 1.5;
    else
        weightDiscount = 2;
    UserType temp_ut = user->u_type;
    PackageType temp_pt = goods->p_type;
    return base_prise * userDiscount[temp_ut] * packageDiscount[temp_pt] * weightDiscount;
}
//---------------------------------------------------------------------------------------------

time_t get_current_timestamp(void) {
    return time(NULL); // 获取当前系统时间（秒级精度）
}
typedef struct {
    time_t timestamp;       // 操作时间戳（精确到秒）
    char operation_type[20];// 操作类型（入库/出库/修改等）
    char package_id[20];    // 关联包裹编号
    char operator_id[20];   // 操作员ID
} OperationLog;

void format_time(time_t timestamp, char* buffer) {
    struct tm* tm_info = localtime(&timestamp);
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", tm_info);
}

int log_operation(const char* operation, const char* package_id, const char* operator_id) {
    FILE* log_file = fopen("operation.log", "a"); // 追加模式
    if (log_file != NULL) {
        time_t now = get_current_timestamp();
        char time_str[20];
        format_time(now, time_str);

        fprintf(log_file, "[%s] %s - 包裹:%s 操作员:%s\n",
            time_str, operation, package_id, operator_id);
        fclose(log_file);
        return 1; // 成功写入日志
    }
    return 0; // 打开文件失败
}


void trace_package(const char* target_id) {
    FILE* log_file = fopen("operation.log", "r");
    if (log_file == NULL) return;

    char line[256];
    while (fgets(line, sizeof(line), log_file)) {
        if (strstr(line, target_id) != NULL) {
            printf("%s", line); // 输出匹配的日志行
        }
    }
    fclose(log_file);
}

//-----------------------------------------------------------把-------------------------
typedef enum { BLACK, RED }Color;
typedef struct RBTreeNode {//红黑树节点
    char key[20];
    Color  color;            // 节点颜色（RED/BLACK）
    struct RBTreeNode* left;  // 左子节点指针
    struct RBTreeNode* right; // 右子节点指针
    struct RBTreeNode* parent;// 父节点指针
    OutboundOrder* ptr;
} RBTreeNode;
typedef struct RBTree {
    RBTreeNode* root;  // 指向根节点的指针
} RBTree;
typedef struct double_black {
    RBTreeNode* point;
}double_black;
RBTree* Root;
RBTree* trace;
//创建红黑树
//创建的是根结点
//下面的四种LL,RR,RK,LR是针对插入新节点的状况的插入策略，和颜色改变策略
void LL(RBTree** Root, RBTreeNode** cur)//对LL情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    father->color = BLACK;
    grandpa->color = RED;
    if (grandpa == (*Root)->root)//处理grandpa之前的情况
        (*Root)->root = father;
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
        (*Root)->root = father;
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
    (*cur)->right = father;
    father->parent = (*cur);
    father->left = NULL;
    if (grandpa == (*Root)->root)
        (*Root)->root = *cur;
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = *cur;
        else grand_fa->left = *cur;
        (*cur)->parent = grand_fa;
    }
    (*cur)->left = grandpa;
    grandpa->parent = *cur;
    grandpa->right = NULL;
}
void LR(RBTree** Root, RBTreeNode** cur)//对LR情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    (*cur)->color = BLACK;
    grandpa->color = RED;
    (*cur)->left = father;
    father->parent = *cur;
    father->right = NULL;
    if (grandpa == (*Root)->root)
        (*Root)->root = *cur;
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = *cur;
        else grand_fa->left = *cur;
        (*cur)->parent = grand_fa;
    }
    (*cur)->right = grandpa;
    grandpa->parent = *cur;
    grandpa->left = NULL;
}
RBTreeNode* newNode(char key[20], RBTree** Root, OutboundOrder** ptr, RBTree** trace )
{
    RBTreeNode* tmp = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    (*Root)->root = tmp;
    (*trace)->root = tmp;
    strcpy(tmp->key, key);
    tmp->parent = tmp->left = tmp->right = NULL;
    tmp->color = BLACK;
    tmp->ptr = *ptr;
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
    if ((*cur)->parent->color == RED)
        //红红有很多种情况
    {
        //先找叔叔和爷爷
        RBTreeNode*uncle;
        RBTreeNode*grandpa;
           grandpa = (*cur)->parent->parent;
        if ((*cur)->parent == grandpa->right)
           uncle =grandpa->left;
        else uncle = grandpa->right;
        //判断叔叔是不是红色
        if (uncle != NULL && uncle->color == RED)
        {
            //在叔叔是红色的情况下，叔父爷变色，且cur指向爷爷,爷爷一定是黑色
            (*cur)->parent->color = uncle->color = BLACK;
            grandpa->color = RED;
            return fixInsert(Root, &grandpa);
        }
        else//叔叔是黑色(包含NULL)
        {
            //一共有四种情况。LL RR RL LR，四种方法有四种不同的转动方法
            if (grandpa->left == uncle) {//R
                if ((*cur)->parent->left == *cur)//RL
                {
                    RL(Root, cur);
                    return *Root;
                }
                else {//RR
                    RR(Root, cur);
                    return *Root;
                }
            }
            else {//L
                if ((*cur)->parent->left == *cur)//LL
                {
                    LL(Root, cur);
                    return *Root;
                }
                else {//LR
                    LR(Root, cur);
                    return *Root;
                }
            }
        }
    }
    return *Root;

}
//加入新节点
RBTreeNode* Add_node(char key[20], RBTree** Root,OutboundOrder** ptr, RBTree** trace )
{
    RBTreeNode* cur = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    (*trace)->root = (*Root)->root;
    strcpy(cur->key, key);
    cur->ptr = *ptr;
    cur->color = RED;
    cur->left = cur->right = NULL;
    int i = 1;
    while (i) {
        if (strcmp(key, (*trace)->root->key) == 0)
            return NULL;
        if (strcmp(key, (*trace)->root->key) > 0) {
            if ((*trace)->root->right != NULL)
                (*trace)->root= (*trace)->root->right;
            else {
                (*trace)->root->right = cur;
                i = 0;
            }
        }
        else {
            if ((*trace)->root->left != NULL)
                (*trace)->root = (*trace)->root->left;
            else {
                (*trace)->root->left = cur;
                i = 0;
            }
        }
    }
    cur->parent = (*trace)->root;
    fixInsert(Root, &cur);//插入之后的调整
    return cur;
}
//三种遍历方法  先序遍历  中序遍历    后序遍历
void  Pre_order(RBTreeNode* root)//先序遍历   根左右
{
    if (!root)
        printf("there is NULL!\n");
    printf(" \n%s\n", root->key);
    if (root->left != NULL)
        Pre_order(root->left);
    if (root->right != NULL)
        Pre_order(root->right);
}
void In_order(RBTreeNode* root)//中序遍历   左根右
{
    if (root->left != NULL)
        In_order(root->left);
    printf(" %s", root->key);
    if (root->right != NULL)
        In_order(root->right);
}
void Post_order(RBTreeNode* root)//后序遍历 左右根
{
    if (root->left != NULL)
        Post_order(root->left);
    if (root->right != NULL)
        Post_order(root->right);
    printf(" %s", root->key);
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
void LL1(RBTree** Root, RBTreeNode** cur)//对LL情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    if (grandpa == (*Root)->root)//处理grandpa之前的情况
        (*Root)->root = father;
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
void RR1(RBTree** Root, RBTreeNode** cur)//对RR情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    if (grandpa == (*Root)->root)//处理grandpa之前的情况
        (*Root)->root = father;
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
void RL1(RBTree** Root, RBTreeNode** cur)//对RL情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    (*cur)->right = father;
    father->parent = *cur;
    if (grandpa == (*Root)->root)
        (*Root)->root = *cur;
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = *cur;
        else grand_fa->left = *cur;
        (*cur)->parent = grand_fa;
    }
    (*cur)->left = grandpa;
    grandpa->parent = *cur;
}
void LR1(RBTree** Root, RBTreeNode** cur)//对LR情况的旋转
{
    RBTreeNode* father = (*cur)->parent;
    RBTreeNode* grandpa = father->parent;
    (*cur)->left = father;
    father->parent = *cur;
    if (grandpa == (*Root)->root)
        (*Root)->root = *cur;
    else
    {
        RBTreeNode* grand_fa = grandpa->parent;
        if (grand_fa->right == grandpa)
            grand_fa->right = cur;
        else grand_fa->left = cur;
        (*cur)->parent = grand_fa;
    }
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
    if (!siblings&&siblings->color == RED)
    {
        //兄父变色
        siblings->color = BLACK;
        father->color = RED;
        //朝着双黑方向旋转调整
        //一共只有两种情况
        //准备工作
        if ((*Root)->root == father)
            (*Root)->root = siblings;
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
            free(*Point);
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
                    LL1(&Root, &Red);
                }
                else
                {//LR
                    Red = siblings->right;
                    Red->color = father->color;
                    father->color = BLACK;
                    LR1(&Root, &Red);
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
                    RR1(&Root, &Red);
                }
                else {
                    //RL
                    Red = siblings->left;
                    Red->color = father->color;
                    father->color = BLACK;
                    RL1(&Root, &Red);
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
RBTreeNode* Find_key(char* key, RBTree* Root)//查找key值所对应的节点坐标
//返回NULL就代表这个红黑树里面没有这个内容
{
    RBTreeNode* cur = Root->root;
    while (cur != NULL && !strcmp(cur->key, key))
    {
        if (strcmp(cur->key, key))
            cur = cur->left;
        else cur = cur->right;
    }
    return cur;
}
RBTree* Delete(RBTreeNode** cur, RBTree** Root) {
    if (*cur == NULL)
        return Root;
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
            // 不再需要释放cur->key，因为它是数组
            // 直接使用strcpy复制字符串
            strcpy((*cur)->key, alter->key);
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
            Resolve_double_black(&Root, &db, 0);
        }
        else {
            child->color = BLACK;
        }
        free(cur);
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
            Resolve_double_black(&Root, &db, 0);
        }
        else {
            child->color = BLACK;
        }
        free((*cur));
        return (*Root);
    }
    else if ((*cur)->right == NULL && (*cur)->left == NULL) {
        if ((*cur)->color == RED) {
            if (cur == father->left)
                father->left = NULL;
            else
                father->right = NULL;
            free((*cur));
            return (*Root);
        }
        else {
            double_black* This_Point = (double_black*)malloc(sizeof(double_black));
            This_Point->point = cur;
            Resolve_double_black(Root, This_Point, 1);
            return (*Root);
        }
    }
    else {
        RBTreeNode* alter = (*cur)->left;
        while (alter->right != NULL)
            alter = alter->right;
        // 不再需要释放 cur->key，因为它是数组
        // 直接使用 strcpy 复制字符串
        strcpy((*cur)->key, alter->key);
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
//-----------------------------------------------------------
typedef struct Goods goods;
typedef long long ll;//LL
static int sequence = 0;
int generateRandomCode(void) {
    return rand() % 1000; // 生成 0 - 999 的随机数
}
RBTree* Root;
// 生成唯一订单号
void generateOrderNumber(char* orderNumber) {
    // char* orderNumber = *orderNumber1;
     // 获取当前时间
    time_t currentTime = time(NULL);
    // 生成随机数
    int randomCode = generateRandomCode();
    // 自增序列
    sequence = (sequence + 1) % 1000;
    // 格式化订单号
    sprintf(orderNumber, "%ld%03d%03d", currentTime, randomCode, sequence);
}
// 生成四位数取件码，使用毫秒级时间
/*void generatePickupCode(char* pickupCode, char shelfArea, int layer) {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    ULARGE_INTEGER ui;
    ui.LowPart = ft.dwLowDateTime;
    ui.HighPart = ft.dwHighDateTime;
    long long milliseconds = (ui.QuadPart - 116444736000000000LL) / 10000;
    int msPart = milliseconds % 100;
    sprintf(pickupCode, "%c%d%02d", shelfArea, layer, msPart);
}
*/
// 出库订单结构体
HashTable* ht;


//------------------------------------------------------------------

typedef struct Shelf {
    char shelf_name;
    RBTree* tree;
    int capacity;
    int current_count;
} Shelf;
RBTree* shelf_trace[6];
// 全局货架数组
#define SHELF_COUNT 6
Shelf shelves[SHELF_COUNT];

// 初始化货架
void init_shelves(void) {
    char shelf_names[7] = { 'A','B','C','D','E','F','\0' };
    for (int i = 0; i < SHELF_COUNT; i++) {
        shelves[i].shelf_name = shelf_names[i];
        shelves[i].tree = (RBTree*)malloc(sizeof(RBTree));
        shelf_trace[i] = (RBTree*)malloc(sizeof(RBTree));
        shelves[i].tree->root = NULL;
        shelves[i].capacity = 600;
        shelves[i].current_count = 0;
    }
}
//----------------------------------------------------------------
int confirm_outbound(OutboundOrder* order) {
    // 文字提示+信息显示
    printf("\n====== 出库二次确认 ======\n");
    printf("订单号: %s\n电话: %s\n", order->order_id, order->phone);

    printf("输入Y(y)确定出库\n");
    // 确认逻辑
    char confirm;

    scanf_s(" %c", &confirm);

    if (confirm == 'Y' || confirm == 'y') {
        order->outbound_time = time(NULL); // 记录出库时间
        order->status = 1;
        printf("出库成功"); //
        return 0;
    }
    else {
        printf("出库取消");
        return 1;
    }
}
// 时间差计算（单位：小时）
double get_time_diff(time_t start, time_t end) {
    return difftime(end, start) / 3600.0;
}
// 包裹生命周期状态判断
void Intelligent_schedule(OutboundOrder* order);//智能调度
void check_expiration(OutboundOrder* order) {
    time_t now = time(NULL);
    if (order->status == 0) { // 待出库订单
        double warehouse_hours = get_time_diff(order->create_time, now);
        if (warehouse_hours > 48) { // 超过48小时未出库
            printf("仓库滞留超时");
        }
    }
    else { // 已出库订单
        double transit_hours = get_time_diff(order->outbound_time, now);
        if (transit_hours > 72) { // 超过72小时未送达
            printf("运输超时");
        }
    }
}
void GenerateOrder(RBTree** Root)
{
    OutboundOrder* ptr = (OutboundOrder*)malloc(sizeof(OutboundOrder));
    if (!ptr)
    {
        printf("PTR hasnot enough memory");
    }
    goods* Goods = (goods*)malloc(sizeof(goods));
    if (!Goods)
    {
        printf("GOODS hasnot enough memory");
    }
    ptr->Good = Goods;
    printf("请输入货物的名字和属性和重量");
    //scanf_s("%50s %d %lf", ptr->Good->name, &(ptr->Good->p_type), &(ptr->Good->weight));
    scanf_s("%s", ptr->Good->name, sizeof(ptr->Good->name));
    scanf_s("%d", &(ptr->Good->p_type));
    scanf_s("%lf", &(ptr->Good->weight));
    generateOrderNumber(ptr->order_id);
    RBTreeNode* tmp = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    ptr->create_time = time(NULL);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("请输入联系电话: ");
    scanf_s("%s", ptr->phone, 20);
    if (strlen(ptr->phone) != 11 || strlen(ptr->order_id) == 0) {
        PackageException exception = {
             INFORMATION_ERROR,
             time(NULL),
             ptr->order_id,
            "订单信息不完整或手机号格式错误"
        };
        log_exception(&exception);
        printf("订单信息存在错误，已记录异常！\n");
        return;
    }

    //把订单号放到user里面
    printf("/n%d/n", ht->count);
    User* user = hash_search(ht, ptr->phone);
    if (user) {
        printf("找到用户：%s，手机号：%s\n", user->name, user->tel);
    }
    else {
        printf("未找到该用户\n");
    }
    Intelligent_schedule(&ptr);//智能调度
    Add_list(&user->head, &ptr, &user->tail, &user->length);
    printf("\n%s\n%s\n", user->tail->mark, user->tail->data->shelf);
    ptr->status = 0;//创建时未出库
    log_operation("入库", ptr->order_id, "管理员");
    FILE* fp = fopen(ptr->phone, "a");
    if (!fp)
    {
        perror("ERROR!");
    }
    fprintf(fp,
        "手机号为%s\t"
        "订单号为%s\t "
        "订单创建时间为%I64d\t "
        "货物的名字为 % s\t"
        "属性为 % d\t"
        "货物柜为 % s\t"
        "货物重量为 % lf\n",
        ptr->phone, ptr->order_id, ptr->create_time, ptr->Good->name, ptr->Good->p_type, ptr->shelf, ptr->Good->weight);
    fclose(fp);
    printf("%s %d %s \n", user->tail->data->order_id, user->length, user->tail->data->shelf);


    if (!(*Root)->root )
        tmp = newNode(ptr->order_id, Root, ptr,&trace);
    else
        tmp = Add_node(ptr->order_id, Root,ptr,&trace);
}
void Intelligent_schedule(OutboundOrder** order)//智能调度
{
    char ch;
    int k = (*order)->Good->p_type;
    if (k != 4)
        ch = shelves[k].shelf_name;
    else
    {
        int i = rand() % 2;
        k += i;
        ch = shelves[k].shelf_name;
    }
    int n = ch - 'A';
    if (shelves[k].current_count == shelves[k].capacity)
    {
        printf("%c货架已满", ch);
        return;
    }

    int i =rand()%600;
    (*order)->shelf[0] = ch;
    while (1) {
        (*order)->shelf[1] = '0' + i / 1000;
        i %= 1000;
        (*order)->shelf[2] = '0' + i / 100;
        i %= 100;
        (*order)->shelf[3] = '0' + i / 10;
        i %= 10;
        (*order)->shelf[4] = '0' + i;
        (*order)->shelf[5] = '\0';
        RBTreeNode* tmp;
        if (shelves[k].tree->root == NULL)
            tmp = newNode((*order)->shelf, &(shelves[k].tree), &order, &(shelf_trace[k]));
        else
            tmp = Add_node((*order)->shelf, &(shelves[k].tree), &order, &(shelf_trace[k]));
        if (!tmp)
        {
            printf("智能调度成功\n");
            break;
        }
        i = rand() % 600;
    }
    shelves[(*order)->Good->p_type].current_count += 1;
    if (shelves[(*order)->Good->p_type].current_count > 500)
    {
        printf("货架%c已达阈值\n", ch);
    }
    return;
}
void Pickup(RBTree** Root, int i)//, OutboundOrder** order)
{
    char tel[20], mima[20];
    if (!i) {
        printf("输入你的手机号和密码\n");
        scanf_s("%s", tel, sizeof(tel));
        scanf_s("%s", mima, sizeof(mima));
    }
    else {
        printf("输入取件人的手机号\n");
        scanf_s("%s", tel, sizeof(tel));
    }

    User* user = hash_search(ht, tel);
    if (user) {
        printf("找到用户：%s，手机号：%s\n", user->name, user->tel);
    }
    else {
        printf("未找到该用户\n");
    }
    if (!user->length)
    {
        printf("没有可取包裹\n");
        return;
    }
    OutboundOrder* order = user->head->next->data;
    //    printf("%s", tel);
      //  printf("%s", order->phone);
    if (!i) {
        if ((!strcmp(user->tel, order->phone)) && (!strcmp(mima, user->mima))) {
            int k = confirm_outbound(order);
            if (k)
                return;
        }
    }
    else {
        if ((!strcmp(user->tel, order->phone))) {
            int k = confirm_outbound(order);
            if (k)
                return;
        }
        log_operation("出库", order->order_id, order->phone);
        OutboundOrder* ptr = order;
        FILE* fp = fopen(ptr->phone, "a");
        if (!fp)
        {
            perror("ERROR!");
        }
        fprintf(fp, "手机号为%s\t 订单号为%s\t 出库时间为%ld\t 货物的名字为%s\t 属性为%d\n", ptr->phone, ptr->order_id, ptr->outbound_time, ptr->Good->name, ptr->Good->p_type);
        fclose(fp);

        //删除订单
        shelves[ptr->status - 'A'].current_count -= 1;
        list* tmp = Find_char_list(ptr->order_id, user->head);
        free(order->Good);
        Delete_list(&tmp, &user->head, &user->tail, &user->length);
        RBTreeNode* ntmp = Find_key(ptr->phone, *Root);
        RBTreeNode* ntmp1 = Find_key(ptr->shelf, shelves[ptr->shelf[0] - 'A'].tree);
        Delete(&ntmp, Root);
        Delete(&ntmp1, &shelves[ptr->shelf[0] - 'A'].tree);
        free(ptr);
    }
}
void Inquiry_order(RBTree* Root, char* order_id) {
    RBTreeNode* tmp = Find_key(order_id, Root);
    if (!tmp)
    {
        printf("The outboundorder is deleted!\n");
        return;
    }
    OutboundOrder* ptr1 = (OutboundOrder*)tmp->ptr;
    printf("手机号为%s", ptr1->phone);
    if (ptr1->status)
        printf("货物已出库");
    else
        printf("货物未出库");
}


void check_all_orders_for_overdue(HashTable* ht) {
    if (ht == NULL) {
        return;
    }

    time_t current_time = time(NULL);

    // 遍历哈希表的每个桶
    for (int i = 0; i < ht->size; i++) {
        User* user = ht->buckets[i];
        while (user != NULL) {
            // 遍历用户的订单链表
            list* order_node = user->head->next;
            while (order_node != NULL) {
                OutboundOrder* order = order_node->data;
                if (order->status == 0) {  // 待出库状态
                    double elapsed_time = difftime(current_time, order->create_time);
                    if (elapsed_time > OVERDUE_TIME_THRESHOLD) {
                        // 订单超期，记录异常信息
                        PackageException exception;
                        exception.type = OVERDUE_STAY;
                        exception.exception_time = current_time;
                        strcpy(exception.package_id, order->order_id);
                        sprintf(exception.description, "用户%s的订单 %s超期未取", user->name, order->order_id);
                        log_exception(&exception);
                        // 这里可以添加给用户发送消息的代码
                        printf("给用户 %s (%s) 发送超期未取通知\n", user->name, user->tel);
                    }
                }
                order_node = order_node->next;
            }
            user = user->next;
        }
    }
}


//-------------------------------------------------------------
typedef struct {
    char admin_password[50];  // 存储密码字符串
    int capacity_threshold;   // 存储整数型阈值
    char staff_pass[50];
} SystemConfig;
int load_config(SystemConfig* config) {
    FILE* fp = fopen("config.txt", "r");
    if (!fp) return 0; // 文件不存在返回0

    char line[256];
    while (fgets(line, sizeof(line), fp)) { // 逐行读取
        // 跳过注释行和空行
        if (line[0] == '#' || line[0] == '\n') continue;

        // 分割键值对
        char* key = strtok(line, "=");
        char* value = strtok(NULL, "\n");

        // 处理不同参数
        if (strcmp(key, "admin_password") == 0) {
            strcpy(config->admin_password, value);
        }
        else if (strcmp(key, "capacity_threshold") == 0) {
            config->capacity_threshold = atoi(value); // 字符串转整数
        }
        else if (strcmp(key, "staff_pass") == 0) {
            strcpy(config->staff_pass, value);
        }
    }
    fclose(fp);
    return 1;
}


void save_config(SystemConfig* config) {
    FILE* fp = fopen("config.txt", "w");
    if (!fp) return;

    fprintf(fp, "admin_password=%s\n", config->admin_password);
    fprintf(fp, "capacity_threshold=%d\n", config->capacity_threshold);
    fprintf(fp, "staff_pass=%s\n", config->staff_pass);
    fclose(fp);
}

void init_default_config() {
    SystemConfig default_config = {
       "admin12356",
         500,
         "1"
    };
    save_config(&default_config);
}

void read_from_file(FILE* load, RBTree** Root) {
    while (1) {
        OutboundOrder* ptr = (OutboundOrder*)malloc(sizeof(OutboundOrder));
        if (!ptr) {
            printf("PTR hasnot enough memory");
            break;
        }
        goods* Goods = (goods*)malloc(sizeof(goods));
        if (!Goods) {
            printf("GOODS hasnot enough memory");
            free(ptr);
            break;
        }
        ptr->Good = Goods;
        if (fscanf(load, "%s", ptr->Good->name) != 1) {
            free(ptr->Good);
            free(ptr);
            break;
        }
        if (fscanf(load, "%d", &(ptr->Good->p_type)) != 1) {
            free(ptr->Good);
            free(ptr);
            break;
        }

        if (fscanf(load, "%lf", &(ptr->Good->weight)) != 1) {
            free(ptr->Good);
            free(ptr);
            break;
        }

        generateOrderNumber(ptr->order_id);
        ptr->create_time = time(NULL);

        if (fscanf(load, "%s", ptr->phone) != 1) {
            free(ptr->Good);
            free(ptr);
            break;
        }
        // 把订单号放到user里面
        User* user = hash_search(ht, ptr->phone); 
        if (user) {
            printf("找到用户：%s，手机号：%s\n", user->name, user->tel);
        }
        else {
            printf("未找到该用户\n");
            free(ptr->Good);
            free(ptr);
            continue;
        }
        Intelligent_schedule(&ptr);  // 智能调度
        Add_list(&user->head, &ptr, &user->tail, &user->length);
        printf("\n%s\n%s\n", user->tail->mark, user->tail->data->shelf);
        ptr->status = 0;  // 创建时未出库
        log_operation("入库", ptr->order_id, "管理员");
        FILE* fp = fopen(ptr->phone, "a");
        if (!fp) {
            perror("ERROR! 订单录入失败");
        }
        else {
            fprintf(fp,
                "手机号为%s\t"
                "订单号为%s\t "
                "订单创建时间为%I64d\t "
                "货物的名字为 %s\t"
                "属性为 %d\t"
                "货物柜为 %s\t"
                "货物重量为 %lf\n",
                ptr->phone,
                ptr->order_id,
                ptr->create_time,
                ptr->Good->name,
                ptr->Good->p_type,
                ptr->shelf,
                ptr->Good->weight
            );
            fclose(fp);
        }
        RBTreeNode* tmp;
        printf("%s %d %s \n", user->tail->data->order_id, user->length, user->tail->data->shelf);
        RBTree* tmp1 = *Root;
        if (!tmp1->root)
            tmp = newNode(ptr->order_id, Root, &ptr,&trace);
        else
            tmp = Add_node(ptr->order_id, Root, &ptr,&trace);
    }
    Pre_order((*Root)->root);
}


void displayMenu(void);
void handleUserInput(RBTree** Root, HashTable* ht, SystemConfig* sys_config, int* x);
void adminInterface(RBTree* Root, SystemConfig* sys_config);
void printAllOrders(RBTree* Root);
void registerUser(HashTable* ht);

// 显示菜单
void displayMenu(void) {
    printf("\n快递管理系统菜单:\n");
    printf("1. 员工登陆(创建新订单)(取件（不需要输入密码）)(汇报货物异常情况)\n");
    printf("2. 用户取件   (如有多件待取物品请多次操作)\n");
    printf("3. 查找用户\n");
    printf("4. 管理员模式(改变员工/管理员密码)(查看货架属性)(遍历货物链表，货架链表)(查看货物情况)\n");
    printf("5. 注册用户\n");
    printf("6. 用户登录(查询包裹异常信息)(取件（需要输入密码）)\n");
    printf("7. 退出\n");
    printf("请输入你的选择: \n");
}


// 处理用户输入
void handleUserInput(RBTree** Root, HashTable* ht, SystemConfig* sys_config, int* x) {
    int choice;
    scanf("%d", &choice);
    getchar(); // 消耗掉换行符

    switch (choice) {
    case 1:
        printf("请输入员工密码: ");
        char password[50];
        scanf_s("%s", password, sizeof(password));
        getchar(); // 消耗掉换行符

        if (strcmp(password, sys_config->staff_pass) == 0) {
            printf("密码正确，进入员工模式\n");
            printf("1. 创建新订单\n");
            printf("2. 取件\n");
            printf("3. 货物异常情况登记\n");
            printf("请输入你的选择\n");
            int n;
            scanf_s("%d", &n);
            getchar();
            switch (n) {
            case 1:
                GenerateOrder(Root);
                break;
            case 2:
                Pickup(Root, 1);
                break;
            case 3:
                printf("请输入异常包裹的订单号\n");
                char order_id[20];
                scanf_s("%s", order_id, sizeof(order_id));
                getchar();
                printf("\n设置包裹异常属性:\n");
                printf("1. 丢失\n");
                printf("2. 损坏\n");
                printf("3. 超期滞留\n");
                printf("4. 信息错误\n");
                printf("输入你的选择\n");
                int n;
                scanf_s("%d", &n);
                getchar();
                const char* type_str;
                switch (n - 1) {
                case LOST:
                    type_str = "丢失";
                    break;
                case DAMAGED:
                    type_str = "损坏";
                    break;
                case OVERDUE_STAY:
                    type_str = "超期滞留";
                    break;
                case INFORMATION_ERROR:
                    type_str = "信息错误";
                    break;
                default:
                    type_str = "未知异常";
                    break;
                }
                PackageException exception = {
                n - 1,
                time(NULL),
                order_id
                };
                printf("请输出异常描述\n");
                scanf_s("%s", exception.description, sizeof(exception.description));
                getchar();
                log_exception(&exception);
                printf("包裹异常属性设置完成\n");
            break;            default:
                printf("输入不合法\n");
                break;
            }
            break;
    case 2: {
        Pickup(Root, 0);
        break;
    }
    case 3: {
        char phone[20];
        printf("请输入要查找的手机号: ");
        scanf("%s", phone);
        User* user = hash_search(ht, phone);
        if (user) {
            printf("找到用户：%s，手机号：%s\n", user->name, user->tel);
        }
        else {
            printf("未找到该用户\n");
        }
        break;
    }
    case 4:
        adminInterface(*Root, sys_config);
        break;
    case 7:
        printf("退出系统\n");
        (*x) = 0;
        break;
    case 5:
        registerUser(ht);
        break;
    case 6: {
        char phone[20];
        char mima[20];
        char package_id[40];
        printf("请输入用户的手机号: ");
        scanf("%s", phone);
        User* user = hash_search(ht, phone);
        if (user) {
            printf("找到用户：%s，手机号：%s\n", user->name, user->tel);
        }
        else {
            printf("输入错误或没有进行注册\n");
            break;
        }
        printf("请输入密码\n");
        scanf_s("%s", mima, sizeof(mima));
        if (!strcmp(mima, user->mima))
        {
            printf("\n登陆成功\n");
        }
        else {
            printf("密码输入错误\n");
            break;
        }
        printf("1. 查询包裹异常信息\n");
        printf("2. 取件\n");
        printf("请输入你的选择\n");
        int k;
        scanf_s("%d", &k);
        switch (k)
        {
            list* temp;
        case 1:
            temp = user->head->next;
            if (!temp)
            {
                printf("没有货物\n");
                break;
            }
            while (temp)
            {
                printf("订单号为%s\n", temp->data->order_id);
                printf("货物位置为%s\n", temp->data->shelf);
                printf("--------------------------------\n");
                temp = temp->next;
            }
            printf("请输入要查询的订单编号: ");
            scanf_s("%s", package_id, sizeof(package_id));
            query_package_exceptions(package_id);
            break;
        case 2:
            Pickup(Root, 0);
            break;
        }

        break;
    }

    default:
        printf("无效的选择，请重新输入\n");
        }
        else printf("密码错误");
    }
}



// 管理员接口
// 查看各货架属性
void viewShelfAttributes() {
    for (int i = 0; i < SHELF_COUNT; i++) {
        printf("货架 %c 属性：\n", shelves[i].shelf_name);
        printf("  容量：%d\n", shelves[i].capacity);
        printf("  当前数量：%d\n", shelves[i].current_count);
    }
}

// 遍历特定货架的树
void traverseShelfTree(char shelf_name) {
    int index = shelf_name - 'A';
    if (index >= 0 && index < SHELF_COUNT) {
        if (shelves[index].tree->root == NULL) {
            printf("货架 %c 为空\n", shelf_name);
        }
        else {
            printf("货架 %c 的订单信息：\n", shelf_name);
            In_order(shelves[index].tree->root);
            printf("\n");
        }
    }
    else {
        printf("无效的货架名称\n");
    }
}

// 管理员接口
void adminInterface(RBTree* Root, SystemConfig* sys_config) {
    char password[50];
    printf("请输入管理员密码: ");
    scanf_s("%s", password, sizeof(password));
    getchar(); // 消耗掉换行符

    if (strcmp(password, sys_config->admin_password) == 0) {
        printf("密码正确，进入管理员模式\n");
        int choice;
        while (1) {
            printf("\n管理员菜单:\n");
            printf("1. 查看所有订单\n");
            printf("2. 查看各货架属性\n");
            printf("3. 遍历特定货架的树\n");
            printf("4. 重新设置管理员密码\n");
            printf("5. 设置包裹异常属性\n");
            printf("6. 重新设置员工密码");
            printf("7. 查看日志\n");
            printf("8. 退出管理员模式\n");
            printf("请输入你的选择: \n");
            scanf("%d", &choice);
            getchar(); // 消耗掉换行符

            switch (choice) {
            case 1:
                printAllOrders(Root);
                break;
            case 2:
                viewShelfAttributes();
                break;
            case 3: {
                char shelf_name;
                printf("请输入要遍历的货架名称 (A-F): ");
                scanf("%c", &shelf_name);
                getchar(); // 消耗掉换行符
                traverseShelfTree(shelf_name);
                break;
            }
            case 4:
                printf("请输入新密码");
                char newpassword[50];

                scanf_s("%s", newpassword, sizeof(newpassword));
                strncpy((*sys_config).admin_password, newpassword, sizeof((*sys_config).admin_password) - 1);
                (*sys_config).admin_password[sizeof((*sys_config).admin_password) - 1] = '\0';

                save_config(sys_config);

                break;
            case 5:
                printf("请输入异常包裹的订单号\n");
                char order_id[20];
                scanf_s("%s", order_id, sizeof(order_id));
                getchar();
                printf("\n设置包裹异常属性:\n");
                printf("1. 丢失\n");
                printf("2. 损坏\n");
                printf("3. 超期滞留\n");
                printf("4. 信息错误\n");
                printf("5. 未知异常\n");
                printf("输入你的选择\n");
                int n;
                scanf_s("%d", &n);
                getchar();
                const char* type_str;
                switch (n - 1) {
                case LOST:
                    type_str = "丢失";
                    break;
                case DAMAGED:
                    type_str = "损坏";
                    break;
                case OVERDUE_STAY:
                    type_str = "超期滞留";
                    break;
                case INFORMATION_ERROR:
                    type_str = "信息错误";
                    break;
                default:
                    type_str = "未知异常";
                    break;
                }
                PackageException exception = {
                (n - 1),
                time(NULL),
                order_id
                };
                printf("请输出异常描述\n");
                scanf_s("%s", exception.description, sizeof(exception.description));
                getchar();
                log_exception(&exception);
                printf("包裹异常属性设置完成\n");
                break;
            case 6:
                printf("请输入新密码");
                char newpassword1[50];

                scanf_s("%s", newpassword1, sizeof(newpassword1));
                strncpy((*sys_config).staff_pass, newpassword1, sizeof((*sys_config).staff_pass) - 1);
                (*sys_config).staff_pass[sizeof((*sys_config).staff_pass) - 1] = '\0';

                save_config(sys_config);
            case 7:
                /*
                    查看日志
                */
                break;
            case 8:
                printf("退出管理员模式\n");
                return;
            default:
                printf("无效的选择，请重新输入\n");
            }
        }
    }
    else {
        printf("密码错误，无法进入管理员模式\n");
    }
}

// 打印所有订单
void printAllOrders(RBTree* Root) {
    if (Root->root == NULL) {
        printf("没有订单记录\n");
        return;
    }
    printf("\n所有订单信息:\n");
    In_order(Root->root);
    printf("\n");
}

// 注册用户
void registerUser(HashTable* ht) {
    char phone[20];
    char name[30];
    char mima[20];
    char check_mima[20];
    int userType;

    printf("请输入手机号: ");
    scanf_s("%s", phone, sizeof(phone));
    getchar(); // 消耗掉换行符
    User* user = hash_search(ht, phone);
    if (user) {
        printf("用户已注册");
        return;
    }
    printf("请输入姓名: ");
    scanf_s("%s", name, sizeof(name));
    getchar(); // 消耗掉换行符

    while (1) {
        printf("请设置密码:  \t请不要超过15位\n");
        scanf_s("%s", mima, sizeof(mima));
        getchar();

        printf("请再次输入密码: \n");
        scanf_s("%s", check_mima, sizeof(check_mima));
        getchar();
        if (!strcmp(mima, check_mima))
            break;
        printf("两次密码内容不同，请重新设置密码\n");
    }
    printf("请选择用户类型 (0: 学生, 1: 教师, 2: VIP, 3: 企业, 4: 务工): ");
    scanf("%d", &userType);
    getchar(); // 消耗掉换行符

    hash_insert(ht, phone, name, mima, (UserType)userType);
    printf("用户注册成功\n");
}

void query_package_exceptions(const char* package_id) {
    FILE* log_file = fopen("exception.log", "r");
    if (log_file == NULL)
    {
        printf("该订单不存在\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), log_file)) {
        if (strstr(line, package_id) != NULL) {
            printf("%s", line); // 输出匹配的日志行
        }
    }
    fclose(log_file);
}
void read_registered_users_from_file(FILE* load, HashTable* ht) {
    while (1) {
        char phone[20];
        char name[30];
        char mima[20];
        int userType;

        if (fscanf(load, "%19s", phone) != 1) {
            break;
        }
        User* user = hash_search(ht, phone);
        if (user) {
            printf("用户 %s 已注册\n", phone);
            continue;
        }

        if (fscanf(load, "%29s", name) != 1) {
            break;
        }

        if (fscanf(load, "%19s", mima) != 1) {
            break;
        }

        if (fscanf(load, "%d", &userType) != 1) {
            break;
        }

        hash_insert(ht, phone, name, mima, (UserType)userType);
        printf("用户 %s 注册成功\n", phone);
    }
}
//void copy_file(const char* src, const char* dest_dir) {
//    char dest_path[100];
//    sprintf(dest_path, "%s/%s", dest_dir, src);
//
//    FILE* src_file = fopen(src, "rb");
//    FILE* dest_file = fopen(dest_path, "wb");
//
//    char buffer[1024];
//    size_t bytes;
//    while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
//        fwrite(buffer, 1, bytes, dest_file);
//    }
//
//    fclose(src_file);
//    fclose(dest_file);
//}
//void full_backup() {
//    // 创建备份目录
//    time_t now = time(NULL);
//    struct tm* tm_info = localtime(&now);
//    char dir_name[30];
//    strftime(dir_name, 30, "backup/%Y-%m-%d_%H%M%S", tm_info);
//    _mkdir(dir_name); // 创建带时间戳的目录
//
//    // 备份用户表
//    copy_file("user.txt", dir_name);
//    // 备份包裹表
//    copy_file("package.txt", dir_name);
//    // 备份财务表
//    copy_file("finance.txt", dir_name);
//}
