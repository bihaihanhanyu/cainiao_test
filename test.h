
#include"head.h"
#define _CRT_SECURE_NO_WARNINGS
int main()
{

    for (int i = 0; i < 6; i++)
        NUM.num[i] = 0;
    SystemConfig sys_config;

    // 尝试加载配置
    if (!load_config(&sys_config)) {
        printf("配置文件不存在，创建默认配置...\n");
        init_default_config();
        load_config(&sys_config); // 重新加载
    }
    // 使用配置参数
    printf("当前管理员密码：%s\n", sys_config.admin_password);
    printf("当前容量阈值：%d\n", sys_config.capacity_threshold);//加载系统参数

    //hash init

    srand((int)time(NULL)); // 只设置一次随机数种子
    Root = (RBTree*)malloc(sizeof(RBTree));
    Root->root = NULL;
    ht = hash_init(1009);
    hash_insert(ht, "15539796296", "xuyuxuan", STUDENT);
    hash_insert(ht, "13071709996", "yuj", VIP);
    hash_insert(ht, "18637587189", "xuyongj", WORKER);

    hash_search(ht, "15539796296");
    //hash_save(ht, "user.txt");

    GenerateOrder(&Root);
    User* user = hash_search(ht, "15539796296");
    printf("%s %d %s \n", user->tail->data->order_id,user->length,user->tail->data->shelf);


  //  Pre_order(Root);
    RBTreeNode* first = Root->root;
    OutboundOrder* ptr = first->ptr;
  //  printf("%s \n",ptr->phone);
    printf("%s \n",ptr->order_id);
    printf("%s \n",ptr->phone);
    printf("%s \n",ptr->shelf);
    printf("%s \n", (char*)ptr,20);
    printf("%s \n", (char*)ptr+20, 20);
    printf("%s \n", (char*)ptr + 40, 6);
    Pickup(Root, ptr);
    Inquiry_order(Root, ptr->order_id);
    Clear_RBTREE(Root->root);
    free(Root);


    getchar();
    return 0;
}
