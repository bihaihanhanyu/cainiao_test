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
#define OVERDUE_TIME_THRESHOLD 86400
//86400  // 一天的秒数，可根据实际情况调整
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
    float price;
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
typedef struct  List list;
struct List {
    char mark[20];
    OutboundOrder* data;
    list* pre;//前一项
    list* next;//后一项
};
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
typedef struct {
    time_t timestamp;       // 操作时间戳（精确到秒）
    char operation_type[20];// 操作类型（入库/出库/修改等）
    char package_id[20];    // 关联包裹编号
    char operator_id[20];   // 操作员ID
} OperationLog;
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
typedef struct Goods goods;
typedef long long ll;//LL
static int sequence = 0;
HashTable* ht;
typedef struct Shelf {
    char shelf_name;
    RBTree* tree;
    int capacity;
    int current_count;
} Shelf;
// 全局货架数组
#define SHELF_COUNT 6
Shelf shelves[SHELF_COUNT];
typedef struct {
    char admin_password[50];  // 存储密码字符串
    int capacity_threshold;   // 存储整数型阈值
    char staff_pass[50];
} SystemConfig;
time_t get_current_timestamp(void) {
    return time(NULL); // 获取当前系统时间（秒级精度）
}
void format_time(time_t timestamp, char* buffer) {
    struct tm* tm_info = localtime(&timestamp);
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", tm_info);
}
int generateRandomCode(void) {
    return rand() % 1000; // 生成 0 - 999 的随机数
}
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

