#pragma once
//#include"test.h"
#ifndef test_h
#define test_h
#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include"list.h"
struct Logistic;
struct User;
struct Order;
struct Depository;
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
struct Logistic {//物流管理
    int num;//初始化，经过的地点
    double stage_time[20];//阶段性的时间
    char* stage_pos[20];//阶段性的地点
};
static struct Logistic* Add_Logistic(struct Logistic* log)//链表实现物流的进度增加
{
    //...实现
    struct Logistic* nlog = log;
    return nlog;
}
void Show_Logistic(struct Logistic* log)//输出每一项的地点和对应的时间
{
    for (int i = 0; i < log->num; i++) {
        printf("%s", log->stage_pos[i]);
        printf("(");
        printf("%.2f", log->stage_time[i]);
        printf(")");
        if (i != log->num - 1)
            printf("->");
    }

}
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
typedef struct User {//用户
    char tel[12];//还需要手机号和联系方式
    UserType u_type;//不同级别用户
    char name[30];
    //struct Order* ord;//用户未取包裹的链头
    
    
    
    //初始化的时候把下面的三个指针初始化一下
    //ord tail 是NULL,head 是给一个malloc 删除的时候也需要释放head分配的内存
    list* ord;//用户未取包裹的链头
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
    return ht;
}
void hash_insert(HashTable* ht, const char* phone, const char* name, const UserType  type) {
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
    new_node->u_type = type;
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
    unsigned int index = hash_func(phone, ht->size);
    User* node = ht->buckets[index];

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
    FILE* fp = fopen(filename, "wb");
    if (!fp) return;

    for (int i = 0; i < ht->size; i++) {
        User* node = ht->buckets[i];
        while (node) {
            fwrite(node, sizeof(User), 1, fp);
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
        hash_insert(ht, temp.tel, temp.name, temp.u_type);
    }
    fclose(fp);
}
/*struct Order {//订单
    int i;
    struct Goods* goods;
};*/
struct Depository {//仓库
    struct Goods* store;
};
struct Goods {//货物
   // char withdraw_code[20];//取货码
   // char ref[20];//编码
    char name[50];//名字
    PackageType p_type;
    double weight;//重量，kg
   // struct Logistic* log;//物流
    //char pos[20];//货物位置
};
float calculateMoney(User* user,struct Goods* goods, float base_prise)
{
    float userDiscount[5] = { 0.9,0.85,0.75,0.8 };
    float packageDiscount[5] = { 1.2,1.15,1.25,0.9,1.0 };
    float weightDiscount;//权重
    if (goods->weight < 1) 
        weightDiscount = 1;
    else if (goods->weight < 5) 
        weightDiscount = 0.95;
    else if (goods->weight < 10)
        weightDiscount = 0.9;
    else 
        weightDiscount = 0.8;
    UserType temp_ut = user->u_type;
    PackageType temp_pt = goods->p_type;
    return base_prise * userDiscount[temp_ut] * packageDiscount[temp_pt];
}
struct Flash_Bird {
    //用户管理，包裹相关的出库，入库，查询，异常的管理
    //驿站的库存盘点，预警，货架处理（包裹存放位置和对应的取件码）
    //取件通知和寄件提醒的管理，实现包裹的收，存，发的全流程管理




};
typedef  struct Flash_Bird fbird;
#endif
