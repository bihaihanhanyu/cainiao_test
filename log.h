#pragma once
#include"struct_def.h"

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
