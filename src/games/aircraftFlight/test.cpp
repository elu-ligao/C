#include<stdio.h>
#include<stdlib.h>//内存分配函数用到
#include<string.h>//strcmp用到
#define MAXSIZE 10000//定义顺序表大小
#define MAXSIZEB 100//定义顺序表大小
struct administrators //管理员信息
{
	char idd[20];
	char passwordd[20];
}c={"123","123"};//管理员账号密码
typedef struct planeticket//飞机信息
{                                          
	int number;
	char takeof[20];
	char land[20];
	char place[20];
	double money;
	int discount;//折扣
	int inventory;//数量
} planeticket;
struct SqList
{
	planeticket data[MAXSIZE];//定义一个能包含MAXSIZE个information结构体的数组
	int length;
};
typedef struct users
{
	int no;
    char id[11];
    char pwd[20];
	char birthday[20];
	char email[20];
} users;
struct SqListb
{
	users datab[MAXSIZEB];
	int lengthb;
};

int InitList(SqList *&L)	//定义初始化函数
{
	L=(SqList *)malloc(sizeof(SqList));	//申请一个SqList结构大小的内存空间
	L->length=0;//让顺序表长度等于0
	return 1;
}
int InitListb(SqListb *&SL)	//定义初始化函数
{
	SL=(SqListb *)malloc(sizeof(SqListb));	//申请一个SqList结构大小的内存空间
	memset(SL->datab, 0, sizeof(SL->datab));
	SL->lengthb=0;//让顺序表长度等于0
	return 1;
}
int CreateList(SqList *&L)//定义录入航班信息的函数
{
	int i,j;
	
    printf("请输入航班数");
	scanf("%d",&j);
	printf("请依次输入航班相关信息：\n");
	planeticket e;
	
	for(i=L->length;i<j;i++)
	{
		printf("请输入第%d个航班号：\n",i+1);
		scanf("%d",&e.number);
		printf("请输入航班%d起飞时间：\n",e.number);
		scanf("%s", e.takeof);
		printf("请输入航班%d降落时间：\n",e.number);
		scanf("%s", e.land);
		printf("请输入航班%d飞达城市：\n",e.number);
		scanf("%s", e.place);
		printf("请输入航班%d票价：\n",e.number);
		scanf("%lf",&e.money);
		printf("请输入航班%d票价几折：\n",e.number);
		scanf("%d",&e.discount);
		printf("请输入航班%d机票剩余：\n",e.number);
		scanf("%d",&e.inventory);
		L->data[i]=e;//顺序表依次插入

	}
	L->length += j;//顺序表长度等于航班数
	printf("------------ok-----------------\n");
	return 1;
}
int registers(SqListb *&SL,users &g)//注册的函数
{
	users f,e;
	int i,j,k=0;
	printf("请输入账号：\n");
	scanf("%s", e.id);
	for(j=0;j<SL->lengthb;j++)
	{
		f=SL->datab[j];
		if(strcmp(e.id,f.id)==0)
			k=k+1;
	}
	if(k!=0)
		printf("这个账号已有\n");
	else
	{
		i = SL->lengthb;
		SL->lengthb=SL->lengthb+1;	//让表长度增加
		e.no=SL->lengthb;
		printf("请输入%s\n 密码：\n",e.id);
		scanf("%s", e.pwd);
		printf("请输入%s\n 生日：\n",e.id);
		scanf("%s", e.birthday);
		printf("请输入%s\n 邮箱：\n",e.id);
		scanf("%s", e.email);
		e.no=SL->lengthb;
		SL->datab[i]=e;
		g=e;
	}
	return 1;
}
int signin(SqListb *&SL,users &g)//登录的函数
{
	users e,f;
	char x[20];
	int j,k=0;

	while(1)
	{
		printf("请输入账号：\n");
		scanf("%s", e.id);
		
		for(j=0;j<SL->lengthb;j++)
		{
			f=SL->datab[j];
			if(strcmp(e.id,f.id)==0)
			{
				k=k+1;
				e=f;
			}
		}
		if(k<j)
		{
			printf("不存在该帐号请重新输入\n");
			continue;
		}
		else
		{
			printf("请输入%s\n 密码：\n",e.id);
			scanf("%s",x);
			if(strcmp(e.pwd,x)!=0)
				printf("密码错误,登录失败\n");
			else
			{
				printf("登陆成功\n");
				g = e;
				break;
			}
		}
	}
	return 1;
}
void ShowList(SqList *L)//定义输出所有航班信息函数
{
	int i;
	planeticket e;
	for(i=0;i<L->length;i++)
	{
		e=L->data[i];
		printf("航班号名：%d    起飞时间：%s    降落时间： %s  抵达城市：%s  票价；%lf 折扣；%d 剩余票数；%d  ",e.number,e.takeof,e.land,e.place,e.money,e.discount,e.inventory);
		printf("\n");
	}
	printf("----------------------------------------------------\n");
}
void show(SqListb *SL)//定义输出所有会员函数
{
	int i;
	users e;
	for(i=0;i<SL->lengthb;i++)
	{
		e=SL->datab[i];
		printf("id：%s   密码：%s 邮箱：%s    生日： %s    ",e.id,e.pwd,e.email,e.birthday);
		printf("\n");
	}
	printf("----------------------------------------------------\n");
}
void buy(SqList *&L)//购机票
{
	int i,j=-1;
	planeticket r;
	int f;
	while(1)
	{
		printf("输入要买的航班号：\n");
		scanf("%d",&f);
		
		for(i=0;i<L->length;i++)
		{
			r=L->data[i];
			if(r.number==f&&r.inventory>0)
			{
				j=i;
				break;
			}
		}
		if(j!=-1)
		{
			r.inventory=r.inventory-1;
			L->data[j]=r;
			printf("购买成功\n");
			break;
		}
		else
			printf("购买失败，重新输入航班号");
	}
}
void query(users g)
{
	printf("账号%s 密码%s 生日%s 邮箱%s \n",g.id,g.pwd,g.birthday,g.email);
}
void modifypasd(SqListb *&SL,users &g)//修改密码
{
	printf("输入修改后的密码\n");
	scanf("%s", g.pwd);
	SL->datab[g.no-1] = g;
}
int Delete(SqListb *&SL)//删除操作
{
	int i,j=-1;
	users e;
	char x[11] = {0};
	printf("输入要删除的会员编号\n");
	scanf("%s", x);
	for(i=0;i<SL->lengthb;i++)
	{
		e=SL->datab[i];
		if(strcmp(e.id, x) == 0)
		{
			j=i;
			break;
		}
	};
	
	if(j!=-1)
	{
		for(j;j<SL->lengthb;j++)
		{
			SL->datab[j]=SL->datab[j+1];
		}
		SL->lengthb=SL->lengthb-1;
	}
	else
		printf("不存在该会员\n");
	return 1;
}
int main()
{
	SqList *L=NULL;
	SqListb *SL=NULL;
	InitListb(SL);
	InitList(L);
	users g;
	int i,j,k,m=0,n;
	char d[20],e[20];
	do{
		printf("\t\t-------------------------------------------- \n");
		printf("\t\t|         机票购买系统                     |\n");
		printf("\t\t|              主菜单                      |\n");
		printf("\t\t|       (1) 普通用户入口                   |\n");
		printf("\t\t|       (2) 管理员入口                     |\n");
		printf("\t\t|       (0) 退出系统                       |\n");
		printf("\t\t-------------------------------------------  \n");
		printf("请选择<1,2,0>：\n");
		scanf("%d",&i);
		switch(i)
		{
			case 1:
				while(1)
				{
					printf("1会员注册 2会员登录\n");
					scanf("%d",&n);
					if(n==1)
					{
						registers(SL,g);
						break;
					}
					else if(n==2)
					{
						signin(SL,g);
						break;
					}
					else
						printf("输入错误请重新输入\n");
				}
				
				do{
					printf("\t\t* * * * * * * * * * * * * * * * * * * * * * *\n");
					printf("\t\t*           会员菜单                        *\n");
					printf("\t\t*       (1)查询航班                         *\n");
					printf("\t\t*       (2)购买航班                         *\n");
					printf("\t\t*       (3)查询个人信息                     *\n");
					printf("\t\t*       (4)修改密码                         *\n");
					printf("\t\t*       (0)返回主界                         *\n");
					printf("\t\t* * * * * * * * * * * * * * * * * * * * * * *\n");
					printf("请选择<1,2,3,4,0>：\n");
					scanf("%d",&j);
					switch(j)
					{
					case 1:
						m=0;
						if(L->length==0)
							printf("没有航班\n");
						else
							ShowList(L);
						break;
					case 2:
						m=0;
						if(L->length==0)
							printf("没有航班\n");
						else
							buy(L);
						break;
					case 3:
						m=0;
						query(g);
						break;
					case 4:
						m=0;
						modifypasd(SL,g);
						break;
					case 0:
						m=1;
						break;
					default:
						m=0;
						printf("输入有误，请重新输入！\n");
						break;
					}
				}while(m==0);
				m=1;
				break;
			case 2:
				while(1)
				{
					printf("输入管理员账号\n");
					scanf("%s",d);
					if(strcmp(c.idd,d)!=0)
						printf("账号错误再次输入\n");
					else
						break;
				}
				while(1)
				{
					printf("输入密码\n");
					scanf("%s",e);
					if(strcmp(c.passwordd,e))
						printf("密码错误请重新输入\n");
					else
						break;
				}
				do{
					printf("\t\t-------------------------------------------- \n");
					printf("\t\t|             管理员菜单                    |\n");
					printf("\t\t|       (1)录入航班信息                     |\n");
					printf("\t\t|       (2)查询所有会员                     |\n");
					printf("\t\t|       (3)删除某会员                       |\n");
					printf("\t\t|       (0)返回主界                         |\n");
					printf("\t\t-------------------------------------------  \n");
					printf("请选择<1,2,0>：\n");
					scanf("%d",&k);
					switch(k)
					{
					case 1:
						m=0;
						CreateList(L);
						break;
					case 2:
						m=0;
						if(SL->lengthb==0)
							printf("没有会员\n");
						else
							show(SL);
						break;
					case 3:
						m=0;
						if(SL->lengthb==0)
							printf("没有会员\n");
						else
							Delete(SL);
						break;
					case 0:
						m=1;
						break;
					default:
						m=0;
						printf("输入有误，请重新输入！\n");
						break;
					}
				}while(m==0);
				m=1;
				break;
			case 0:
				exit(0);
			default:
				printf("输入有误，请重新输入！\n");
				break;
		}
	}while(1);
	return 1;
}