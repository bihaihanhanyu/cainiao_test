#pragma once
#include"struct_def.h"
void format_time(time_t timestamp, char* buffer);
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
//HashTable* ht;
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
