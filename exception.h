#pragma once
#include"struct_def.h"
/*void trace_package(const char* target_id) {
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
    return;
}
void query_package_exceptions(char package_id[20])
{
    FILE* log_file = fopen("exception.log", "r");
    if (log_file == NULL)
    {
        printf("不存在异常包裹\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), log_file)) {
        if (strstr(line, package_id) != NULL) {
            {
                printf("%s", line); // 输出匹配的日志行
                fclose(log_file);
            }
        }
    }

        printf("该订单包裹无异常情况\n");
    fclose(log_file);
}
*/
void log_exception(const PackageException* exception) {
    FILE* log_file = fopen("exception.txt", "a");
    if (log_file == NULL) {
        perror("无法打开日志文件");
        return;
    }

    char time_str[20];
    format_time(exception->exception_time, time_str);

    const char* type_str;
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
    return;
}

void query_package_exceptions(char package_id[20]) {
    FILE* log_file = fopen("exception.txt", "r");
    if (log_file == NULL) {
        printf("不存在异常包裹\n");
        return;
    }

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), log_file)) {
        if (strstr(line, package_id) != NULL) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf("该订单包裹无异常情况\n");
    }

    fclose(log_file);
}
