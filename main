#include<stdio.h>


int main()
{
	printf("------ 菜鸟驿站系统 ------");
	printf("1. 用户登录");
	printf("2. 管理员登录");
	printf("3. 退出");
	printf("--------------------------");
	char x;
	x = getchar();
	while (x != '3')
	{
		x = getchar();
		if (x == '1')
		{
			//用户的取件，寄件，查询
			printf("请输入您的手机号码");
			int ttel[12];
			scanf("%d", ttel);
			if (finduser(ttel))//查找用户，若找到则返回其结构体指针
			{
				user* t_user = finduser(ttel);
				printf("1.取件    2.寄件    3.查询");
				char opera=getchar();

			}
			else
			{
				printf("用户不存在！");
					break;
			}


		}


	}

	return 0;
}
