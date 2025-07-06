#pragma once
#include"struct_def.h"
static void Isolate_list(list* temp, list* tail);
//type 1 第2种链表,head不存内容
void Add_list(list** head, OutboundOrder** ndata, list** tail, int* length)//尾插法
{
    list* temp = (list*)malloc(sizeof(list));
    strcpy(temp->mark, (*ndata)->order_id);
    temp->data = *ndata;
    temp->next = NULL;
    temp->pre = NULL;
    if ((*head)->next == NULL) {
        *tail = (*head)->next = temp;
        temp->pre = (*head);
    }
    else {
        (*tail)->next = temp;
        temp->pre = *tail;
        *tail = temp;
    }
    (*length) += 1;;
}
list* Find_list(list* head, int num)//返回第num个链表的位置
{
    //num等于零的时候相当于返回head指针
    list* temp = head;
    for (int i = 0; i < num; i++) temp = temp->next;
    return temp;
}
void Delete_list(list** p, list** head, list** tail, int* length)
{
    if (*p == *head) {
        perror("You cannot delete head ptr.");
        return;
    }
    if (*length == 1)
        (*head)->next = (*tail) = NULL;
    else
    {
        if ((*p)->next)
        {
            (*p)->next->pre = (*p)->pre;
            (*p)->pre->next = (*p)->next;
        }
        else
        {
            *tail = (*p)->pre;
            (*p)->pre->next = NULL;
        }
    }
    (*length) -= 1;
    free(*p);
}
void Insert_list01(list* prev, list* temp, list* tail)//第一种插入方法
{//prev插入位置前一项的指针  temp是要进行插入操作的指针
    Isolate_list(temp, tail);//隔离temp
    if (prev->next)
    {
        list* prev__next = prev->next;
        prev__next->pre = temp;
        temp->next = prev__next;
    }
    else
    {
        temp->next = NULL;
        tail = temp;
    }
    prev->next = temp;
    temp->pre = prev;

}
void Insert_list02(list* prev, OutboundOrder* ndata, int length, list* tail)//第二种插入方法//可以用它来实现头插法
{//prev插入位置前一项的指针  data是新生成的链表的data域
    list* temp = (list*)malloc(sizeof(list));
    temp->data = ndata;
    if (prev->next)
    {
        list* prev__next = prev->next;
        prev__next->pre = temp;
        temp->next = prev__next;
    }
    else
    {
        temp->next = NULL;
        tail = temp;
    }
    prev->next = temp;
    temp->pre = prev;
    length++;
}
void Clear_list(list* head, int* length)
{
    list* temp = head;
    while (temp != NULL)
    {
        list* temp01 = temp;
        temp = temp->next;
        free(temp01);
    }
    head->next = NULL;
    *length = 0;
}
static void Isolate_list(list* temp, list* tail) {//把temp这个链表的部分隔离出来
    //static  说明只能在这个头文件里面使用
    list* temp__next = temp->next;
    list* temp__pre = temp->pre;
    if (!temp->next)
        tail = temp__pre;
    else temp__next->pre = temp__pre;
    temp__pre->next = temp__next;
    return;
}
void Exchange_list(list* a, list* b, list* tail) {//交换a和b
    //首先确定一点，a和b是否为head->next对各种操作没有影响
    //需要特判的地方有 a/b为尾指针，a/b相连,a/b既有尾指针，有有相连的情况
    if (a == b)return;
    if (a->pre == b || a == tail)//都让a在b的前面
    {
        list* temp = a;
        a = b;
        b = temp;
    }
    list* a_pre = a->pre;
    list* a_next = a->next;
    list* b_pre = b->pre;
    list* b_next = b->next;
    int mark = (b == tail);//标记初始位置b是否为尾指针
    if (a->next != b)
    {
        a_next->pre = b;
        b->next = a_next;
        b_pre->next = a;
        a->pre = b_pre;
    }
    else
    {
        b->next = a;
        a->pre = b;
    }

    b->pre = a_pre;
    a_pre->next = b;
    a->next = b_next;
    if (mark)
        b_next->pre = a;
}
list* Find_char_list(char mark[], list* head)
{
    list* temp = head;
    while (temp != NULL && strcmp(temp->mark, mark) != 0)
        temp = temp->next;
    return temp;
}
int Empty_list(list* head)
{
    return head->next == NULL;
}
