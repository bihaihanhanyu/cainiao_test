#pragma once
#include"struct_def.h"
#include"list.h"
#include"redblacktree.h"
#include"log.h"
#include"hash.h"
#include"exception.h"
#include"calculate.h"
//#include"console.h"
void init_shelves(void) {
    char shelf_names[7] = { 'A','B','C','D','E','F','\0' };
    for (int i = 0; i < SHELF_COUNT; i++) {
        shelves[i].shelf_name = shelf_names[i];
        shelves[i].tree = (RBTree*)malloc(sizeof(RBTree));

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
    int count = 0;
    getchar();
   confirm = getchar();
   // scanf_s("%c",&confirm);
    // 读取字符并指定长度
   // scanf_s("%c",&confirm);
    //getchar();
    if (confirm == 'Y' || confirm == 'y') {
        order->outbound_time = time(NULL); // 记录出库时间
        order->status = 1;
        printf("出库成功\n"); //
       
        return 0;
    }
    else {
        printf("出库取消\n");
        return 1;
    }
}
// 时间差计算（单位：小时）
double get_time_diff(time_t start, time_t end) {
    return difftime(end, start) / 3600.0;
}
// 包裹生命周期状态判断
void Intelligent_schedule(OutboundOrder** order);//智能调度
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
    printf("请输入货物的名字和属性和重量(kg)以及基础运费\n");
    printf("(货物属性栏输入数字0-4)\n");
    printf("0.  LARGE \n");
    printf("1.  FRAGILE\n");
    printf("2.  VALUABLE\n");
    printf("3.  DOCUMENT\n");
    printf("4.  STANDARD\n");
    
    //scanf_s("%50s %d %lf", ptr->Good->name, &(ptr->Good->p_type), &(ptr->Good->weight));
    while (1) {
        scanf_s("%s", ptr->Good->name, sizeof(ptr->Good->name));
        scanf_s("%d", &(ptr->Good->p_type));
        scanf_s("%lf", &(ptr->Good->weight));
        scanf_s("%f", &(ptr->Good->price));
        if (ptr->Good->p_type < 0 || ptr->Good->p_type>4)
            printf("操作违法，请重新输入\n");
        break;
    }
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
            "订单信息不完整或手机号格式错误\n"
        };
        log_exception(&exception);
        printf("订单信息存在错误，已记录异常！\n");
        return;
    }

    //把订单号放到user里面
    User* user = hash_search(ht, ptr->phone);
    if (user) {
        printf("找到用户：%s，手机号：%s\n", user->name, user->tel);
    }
    else {
        printf("未找到该用户\n");
        return;
    }
    Intelligent_schedule(&ptr);//智能调度
    Add_list(&user->head, &ptr, &user->tail, &user->length);
    user->length++;
    ptr->status = 0;//创建时未出库
    log_operation("入库", ptr->order_id, "管理员");
    FILE* fp = fopen(ptr->phone, "a");
    if (!fp)
    {
        perror("ERROR!");
    }
    char time[50];
    format_time(ptr->create_time, time);
    fprintf(fp,
        "手机号为%s\t"
        "订单号为%s\t "
       
        "订单创建时间为 %s\t "
        "货物的名字为 % s\t"
        "属性为 %d\t"
        "货物柜为 %s\t"
        "货物重量为 %lf\t",
       
        ptr->phone, ptr->order_id,
        //ptr->create_time,
        time,
        ptr->Good->name,
        ptr->Good->p_type,
        ptr->shelf, 
        ptr->Good->weight
       
    );
    fclose(fp);
    printf("%s\n",ptr->order_id);
    if (!(*Root)->root)
        tmp = newNode(ptr->order_id, Root, &ptr);
    else
        tmp = Add_node(ptr->order_id, Root, &ptr);
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
    int i = rand() % 600;
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
            tmp = newNode((*order)->shelf, &(shelves[k].tree), &order);
        else
            tmp = Add_node((*order)->shelf, &(shelves[k].tree), &order);
        if (tmp)
        {
            printf("智能调度成功\n");
            break;
        }
        i = rand() % 600;
    }
    shelves[k].current_count += 1;
    if (shelves[(*order)->Good->p_type].current_count >200 )
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
        return;
    }
    if (user->length <= 0)
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
        else {
            printf("密码输入错误\n");
            return;
        }
    }
    else {
        if ((!strcmp(user->tel, order->phone))) {
            int k = confirm_outbound(order);
            if (k)
                return;
        }
    }

    log_operation("出库", order->order_id, order->phone);
    OutboundOrder* ptr = order;
    char time[50];
    format_time(ptr->outbound_time, time);
    FILE* fp = fopen(ptr->phone, "a");
    if (!fp)
    {
        perror("ERROR!");
    }
    fprintf(fp, "手机号为%s\t 订单号为%s\t 出库时间为%s\t 货物的名字为%s\t 属性为%d\n", ptr->phone, ptr->order_id,
        //  ptr->outbound_time,
        time,
        ptr->Good->name, ptr->Good->p_type);
    fclose(fp);
    //删除订单
    float money = calculateMoney(user,order->Good, order->Good->price);
    printf("您应付运费%.2f\n",money);
    shelves[order->shelf[0] - 'A'].current_count--;
    list* tmp = Find_char_list(ptr->order_id, user->head);
    free(order->Good);
    user->length--;
    Delete_list(&tmp, &user->head, &user->tail, &user->length);
    RBTreeNode* ntmp = Find_key(ptr->order_id, *Root);
    RBTreeNode* ntmp1 = Find_key(ptr->shelf, shelves[ptr->shelf[0] - 'A'].tree);
    Delete(&ntmp, Root);
    Delete(&ntmp1, &shelves[ptr->shelf[0] - 'A'].tree);
    free(order);
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
                        printf("用户 %s (%s) 发送超期未取通知\n", user->name, user->tel);
                    }
                }
                order_node = order_node->next;
            }
            user = user->next;
        }
    }
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
        if (fscanf(load, "%f", &(ptr->Good->price)) != 1) {
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
        user->length++;
        Add_list(&user->head, &ptr, &user->tail, &user->length);
        // printf("\n%s\n%s\n", user->tail->mark, user->tail->data->shelf);
        ptr->status = 0;  // 创建时未出库
        log_operation("入库", ptr->order_id, "管理员");
        FILE* fp = fopen(ptr->phone, "a");
        if (!fp) {
            perror("ERROR! 订单录入失败");
        }
        else {
            char time[50];
            format_time(ptr->create_time, time);
            fprintf(fp,
                "手机号为%s\t"
                "订单号为%s\t "
                "订单创建时间为%s\t "
                "货物的名字为 %s\t"
                "属性为 %d\t"
                "货物柜为 %s\t"
                "货物重量为 %lf\n",
               
                ptr->phone,
                ptr->order_id,
                // ptr->create_time,
                time,
                ptr->Good->name,
                ptr->Good->p_type,
                ptr->shelf,
                ptr->Good->weight
                
            );
            fclose(fp);
        }
        RBTreeNode* tmp;
        //      printf("%s %d %s \n", user->tail->data->order_id, user->length, user->tail->data->shelf);
        RBTree* tmp1 = *Root;
        if (!tmp1->root)
            tmp = newNode(ptr->order_id, Root, &ptr);
        else
            tmp = Add_node(ptr->order_id, Root, &ptr);
    }
}
