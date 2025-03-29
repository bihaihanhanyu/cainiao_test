#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define GROUP_COUNT 1000
#define MAX_WORD_LENGTH 20

// 随机生成小于 20 个字符的英文单词
void generate_random_word(char* word) {
    int length = rand() % 19 + 1;
    for (int i = 0; i < length; i++) {
        word[i] = 'a' + rand() % 26;
    }
    word[length] = '\0';
}

// 生成 11 位手机号
void generate_phone_number(char* phone) {
    phone[0] = '1';
    for (int i = 1; i < 11; i++) {
        phone[i] = '0' + rand() % 10;
    }
    phone[11] = '\0';
}

// 生成随机密码
void generate_password(char* password) {
    int length = rand() % 15 + 1;
    for (int i = 0; i < length; i++) {
        password[i] = 'a' + rand() % 26;
    }
    password[length] = '\0';
}

// 生成用户注册数据
void generate_user_registration_data() {
    FILE* fp = fopen("user_registration_data.txt", "w");
    if (fp == NULL) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < GROUP_COUNT; i++) {
        char phone[20];
        char name[30];
        char mima[20];
        char check_mima[20];
        int userType = rand() % 5;

        generate_phone_number(phone);
        generate_random_word(name);
        generate_password(mima);
        strcpy(check_mima, mima);

        fprintf(fp, "%s %s %s %s %d\n", phone, name, mima, check_mima, userType);
    }

    fclose(fp);
}

// 生成 read_from_file 需要的数据
void generate_read_from_file_data() {
    FILE* fp = fopen("read_from_file_data.txt", "w");
    if (fp == NULL) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < GROUP_COUNT; i++) {
        char goods_name[MAX_WORD_LENGTH];
        int goods_type = rand() % 5;
        float goods_weight = (float)(rand() % 1000) / 10.0; // 0 - 100.0
        char phone[20];

        generate_random_word(goods_name);
        generate_phone_number(phone);

        fprintf(fp, "%s %d %.1f %s\n", goods_name, goods_type, goods_weight, phone);
    }

    fclose(fp);
}

int main() {
    srand(time(NULL));

    generate_user_registration_data();
    generate_read_from_file_data();

    printf("Test data generated successfully.\n");

    return 0;
}
