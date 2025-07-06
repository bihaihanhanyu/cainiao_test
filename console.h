#pragma once
#include"order.h"
void displayMenu(void);
void handleUserInput(RBTree** Root, HashTable* ht, SystemConfig* sys_config, int* x);
void adminInterface(RBTree* Root, SystemConfig* sys_config);
void printAllOrders(RBTree* Root);
void registerUser(HashTable* ht);
// 显示菜单
void displayMenu(void) {
    printf("\n快递管理系统菜单:\n");
    printf("1. 员工登陆(创建新订单)(取件)(汇报货物异常情况)\n");
    //printf("2. 用户取件   (如有多件待取物品请多次操作)\n");
    //printf("3. 查找用户\n");
    printf("2. 管理员模式(改变员工/管理员密码)(查看货架属性)(遍历货物链表，货架链表)(查看货物情况)\n");
    printf("3. 注册用户\n");
    printf("4. 用户登录(查询包裹异常信息)(取件)\n");
    printf("5. 退出\n");
    printf("请输入你的选择: \n");
}
// 处理用户输入
void handleUserInput(RBTree** Root, HashTable* ht, SystemConfig* sys_config, int* x) {
    int choice;
    int kongzhixunhuan;
    scanf("%d", &choice);
    getchar(); // 消耗掉换行符
    char phone[20];
    char mima[20];
    char package_id[40];
    switch (choice) {
    case 1:
        printf("请输入员工密码: \n");
        char password[50];
        scanf_s("%s", password, sizeof(password));
        getchar(); // 消耗掉换行符
        if (strcmp(password, sys_config->staff_pass) == 0) {
            int num = 1;
            printf("密码正确，进入员工模式\n");
            while (num) {
                printf("员工菜单\n");
                printf("1. 创建新订单\n");
                printf("2. 取件\n");
                printf("3. 货物异常情况登记\n");
                printf("4. 退出员工界面\n");
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
                    kongzhixunhuan = 1;
                    while (kongzhixunhuan) {
                        printf("\n设置包裹异常属性:\n");
                        printf("1. 丢失\n");
                        printf("2. 损坏\n");
                        printf("3. 超期滞留\n");
                        printf("4. 信息错误\n");
                        printf("5. 未知异常\n");
                        printf("输入你的选择\n");

                        scanf_s("%d", &n);
                        getchar();
                        const char* type_str;
                        switch (n - 1) {
                        case LOST:
                            type_str = "丢失";
                            kongzhixunhuan = 0;
                            break;
                        case DAMAGED:
                            type_str = "损坏";
                            kongzhixunhuan = 0;
                            break;
                        case OVERDUE_STAY:
                            type_str = "超期滞留";
                            kongzhixunhuan = 0;
                            break;
                        case INFORMATION_ERROR:
                            type_str = "信息错误";
                            kongzhixunhuan = 0;
                            break;
                        case 4:
                            type_str = "未知异常";
                            kongzhixunhuan = 0;
                            break;
                        default:
                            printf("输入无效，请重新输入错误类型\n");
                            kongzhixunhuan = 1;
                            break;
                        }
                    }
                    PackageException exception = {
                    (n - 1),
                    time(NULL)
                    };
                    strcpy(exception.package_id, order_id);
                    printf("请输出异常描述\n");
                    scanf_s("%s", exception.description, sizeof(exception.description));
                    getchar();
                    log_exception(&exception);
                    printf("包裹异常属性设置完成\n");
                    break;
                case 4:
                    printf("已退出员工界面\n");
                    num = 0;
                    break;
                default:
                    printf("输入不合法,请重新输入\n");
                }
            }
        }
        else
        {
            printf("密码错误\n");
        }
        break;
    case 2:
        adminInterface(*Root, sys_config);
        break;
    case 5:
        printf("退出系统\n");
        (*x) = 0;
        break;
    case 3:
        registerUser(ht);
        break;
    case 4:
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
        int guanli = 1;
        while (guanli) {
            printf("用户界面\n");
            printf("1. 查询包裹异常信息\n");
            printf("2. 取件\n");
            printf("3. 退出用户界面\n");
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
                printf("请输入要查询的订单编号: \n");
                while (1) {
                    scanf_s("%s", package_id, sizeof(package_id));
                    getchar();
                    if (strlen(package_id) == 16)
                        break;
                    printf("订单号格式不正确，请重新输入\n");
                }
                query_package_exceptions(package_id);
                break;
            case 2:
                Pickup(Root, 0);
                break;
            case 3:
                printf("已经退出用户登录界面\n");
                guanli = 0;
                break;
            default:
                printf("无效的选择，请重新输入\n");
                break;
            }
        }
        break;

    default:
        printf("无效的选择，请重新输入\n");
        break;
    }
}
//管理员接口
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
    int xuanzede;
    int xuanze;
    int kongzhixunhuan;
    int n;
    printf("请输入管理员密码: \n");
    scanf_s("%s", password, sizeof(password));
    getchar(); // 消耗掉换行符
    char package_id[20];
    if (strcmp(password, sys_config->admin_password) == 0) {
        printf("密码正确，进入管理员模式\n");
        int choice;
        while (1) {
            printf("\n管理员菜单:\n");
            printf("1. 查看所有订单\n");
            printf("2. 查看各货架属性\n");
            printf("3. 遍历特定货架的树\n");
            printf("4. 重新设置管理员密码\n");
            printf("5. 重新设置员工密码\n");
            printf("6. 设置包裹异常属性\n");
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
                printf("请输入新密码:\n");
                char newpassword[50];

                scanf_s("%s", newpassword, sizeof(newpassword));
                strncpy((*sys_config).admin_password, newpassword, sizeof((*sys_config).admin_password) - 1);
                (*sys_config).admin_password[sizeof((*sys_config).admin_password) - 1] = '\0';

                save_config(sys_config);

                break;
            case 6:
                printf("请输入异常包裹的订单号:\n");
                char order_id[20];
                scanf_s("%s", order_id, sizeof(order_id));
                getchar();
                kongzhixunhuan = 1;
                while (kongzhixunhuan) {
                    printf("\n设置包裹异常属性:\n");
                    printf("1. 丢失\n");
                    printf("2. 损坏\n");
                    printf("3. 超期滞留\n");
                    printf("4. 信息错误\n");
                    printf("5. 未知异常\n");
                    printf("输入你的选择\n");
                   
                    scanf_s("%d", &n);
                    getchar();
                    const char* type_str;
                    switch (n - 1) {
                    case LOST:
                        type_str = "丢失";
                        kongzhixunhuan = 0;
                        break;
                    case DAMAGED:
                        type_str = "损坏";
                        kongzhixunhuan = 0;
                        break;
                    case OVERDUE_STAY:
                        type_str = "超期滞留";
                        kongzhixunhuan = 0;
                        break;
                    case INFORMATION_ERROR:
                        type_str = "信息错误";
                        kongzhixunhuan = 0;
                        break;
                    case 4:
                        type_str = "未知异常";
                        kongzhixunhuan = 0;
                        break;
                    default:
                        printf("输入无效，请重新输入错误类型\n");
                        kongzhixunhuan = 1;
                        break;
                    }
                }
                PackageException exception = {
                (n - 1),
                time(NULL)
                };
                strcpy(exception.package_id, order_id);
                printf("请输出异常描述:\n");
                scanf_s("%s", exception.description, sizeof(exception.description));
                getchar();
                log_exception(&exception);
                printf("包裹异常属性设置完成\n");
                break;
            case 5:
                printf("请输入新密码:");
                char newpassword1[50];

                scanf_s("%s", newpassword1, sizeof(newpassword1));
                strncpy((*sys_config).staff_pass, newpassword1, sizeof((*sys_config).staff_pass) - 1);
                (*sys_config).staff_pass[sizeof((*sys_config).staff_pass) - 1] = '\0';

                save_config(sys_config);
                break;
            case 7:
                xuanzede = 1;
                while (xuanzede) {
                    printf("日志查询界面\n");
                    printf("1. 输入手机号查询对应货物操作日志\n");
                    printf("2. 输入订单号查询包裹异常记录\n");
                    printf("3. 退出日志查询界面\n");
                    printf("请输入你的选择\n");
                    scanf_s("%d", &xuanze);
                    getchar();
                    switch (xuanze)
                    {
                    case 1: {
                        printf("请输入手机号: \n");
                        scanf_s("%s", package_id, sizeof(package_id));
                        getchar();
                        FILE* fp3 = fopen(package_id, "r");
                        if (fp3 == NULL) {
                            printf("该手机号用户的货物不存在操作记录\n");
                            break;
                        }
                            printf("货物属性栏数字的含义\n");
                            printf("LARGE     大件\n");
                            printf("FRAGILE   易碎\n");
                            printf("VALUABLE  贵重\n");
                            printf("DOCUMENT  文件\n");
                            printf("STANDARD  普通\n");
                        char ch;
                        // 从文件中读取字符并输出到屏幕
                        while ((ch = fgetc(fp3)) != EOF) {
                            putchar(ch);
                        }
                        fclose(fp3);
                    }
                          break;
                    case 2:
                        printf("请输入要查询的订单编号: \n");
                        while (1) {
                            scanf_s("%s", package_id, sizeof(package_id));
                            getchar();
                            if(strlen(package_id)==16)
                            break;
                            printf("订单号格式不正确，请重新输入\n");
                        }
                        query_package_exceptions(package_id);
                        break;
                    case 3:
                        printf("退出日志查询界面\n");
                        xuanzede = 0;
                        break;
                    default:
                        printf("输入不合法，请重新输入\n");
                        break;
                    }
                }
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
    if (strlen(phone) != 11)
    {
        printf("手机号格式输入错误\n");
        return;
    }
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
        if (strlen(mima) > 15)
        {
            printf("密码超过15位，请重新输入\n");
            continue;
        }
        printf("请再次输入密码: \n");
        scanf_s("%s", check_mima, sizeof(check_mima));
        getchar();
        if (strlen(check_mima) > 15)
        {
            printf("密码超过15位，请重新输入\n");
            continue;
        }
        if (!strcmp(mima, check_mima))
            break;
        printf("两次密码内容不同，请重新设置密码\n");
    }
    while (1) {
    printf("请选择用户类型 (0: 学生, 1: 教师, 2: VIP, 3: 企业, 4: 务工): ");
    scanf("%d", &userType);
    getchar(); // 消耗掉换行符
    if (userType < 0 || userType>4) {
        printf("用户类型选择无效，请重新选择\n");
        continue;
    }
    else
        break;
}
    hash_insert(ht, phone, name, mima, (UserType)userType);
    printf("用户注册成功\n");
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
