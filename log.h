#pragma once
#include<stdio.h>
#include <time.h>
time_t get_current_timestamp() {
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

void log_operation(const char* operation, const char* package_id, const char* operator_id) {
    FILE* log_file = fopen("operation.log", "a"); // 追加模式 
    if (log_file != NULL) {
        time_t now = get_current_timestamp();
        char time_str[20];
        format_time(now, time_str);

        fprintf(log_file, "[%s] %s - 包裹:%s 操作员:%s\n",
            time_str, operation, package_id, operator_id);
        fclose(log_file);
    }
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


    log_operation("入库", pkg->package_id, "管理员");



    log_operation("出库", package_id, operator_id);
