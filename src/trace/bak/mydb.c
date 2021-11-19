#include "mysql.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

MYSQL mysql;

void printMySqlInfo()
{
    const char *stat = mysql_stat(&mysql);
    const char *server_info = mysql_get_server_info(&mysql);
    const char *clientInfo = mysql_get_client_info();
    unsigned long version = mysql_get_client_version();
    const char *hostinfo =  mysql_get_host_info(&mysql);
    unsigned long serverversion = mysql_get_server_version(&mysql);
    unsigned int protoinfo = mysql_get_proto_info(&mysql);
     
    printf("stat：%s\n",stat);
    printf("server_info：%s\n",server_info);
    printf("clientInfo：%s\n",clientInfo);
    printf("version：%ld\n",version);
    printf("hostinfo：%s\n",hostinfo);
    printf("serverversion：%ld\n",serverversion);
    printf("protoinfo：%d\n",protoinfo);
     
    const char *charactername = mysql_character_set_name(&mysql);
    printf("client character set：%s\n",charactername);
#if 1
	if (!mysql_set_character_set(&mysql, "utf8"))
    {
        printf("New client character set: %s\n",
               mysql_character_set_name(&mysql));
    }
#endif
}

typedef struct st_zdxxb
{
	char jqbh	[ 12 + 1 ];
	char shm	[ 20 + 1 ];
	char shh	[ 15 + 1 ];
	char zdh	[ 8  + 1 ];
	char sheng	[ 10 + 1 ];
	char shi	[ 10 + 1 ];
	char xian	[ 10 + 1 ];
	char xz		[ 10 + 1 ];
	char jd		[ 40 + 1 ];
	char yyzt	[ 2  + 1 ];
}st_zdxxb_def;

void switch_getval(st_zdxxb_def *pst_tbl_zdxxb, int i, void *val)
{
	switch(i)
	{
		case 0:
			memcpy(pst_tbl_zdxxb->jqbh, (char *)val, 12);
			break;
		case 1:
			memcpy(pst_tbl_zdxxb->shm, (char *)val, 20);
			break;
		case 2:
			memcpy(pst_tbl_zdxxb->shh, (char *)val, 15);
			break;
		case 3:
			memcpy(pst_tbl_zdxxb->zdh, (char *)val, 8);
			break;
		case 4:
			memcpy(pst_tbl_zdxxb->sheng, (char *)val, 10);
			break;
		case 5:
			memcpy(pst_tbl_zdxxb->shi, (char *)val, 10);
			break;
		case 6:
			memcpy(pst_tbl_zdxxb->xian, (char *)val, 10);
			break;
		case 7:
			memcpy(pst_tbl_zdxxb->xz, (char *)val, 10);
			break;
		case 8:
			memcpy(pst_tbl_zdxxb->jd, (char *)val, 40);
			break;
		case 9:
			memcpy(pst_tbl_zdxxb->yyzt, (char *)val, 2);
			break;
		default:
			break;
	}
}

int insert(st_zdxxb_def *pst_tbl_zdxxb)
{
	int flag;
	char sql[512] = {0};
	
	sprintf(sql, "insert into zdxxb (jqbh, shm, shh, zdh, sheng, shi, xian, xz, jd, yyzt) values ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')", 
			pst_tbl_zdxxb->jqbh, 
			pst_tbl_zdxxb->shm, 
			pst_tbl_zdxxb->shh, 
			pst_tbl_zdxxb->zdh, 
			pst_tbl_zdxxb->sheng, 
			pst_tbl_zdxxb->shi, 
			pst_tbl_zdxxb->xian, 
			pst_tbl_zdxxb->xz, 
			pst_tbl_zdxxb->jd,  
			pst_tbl_zdxxb->yyzt);

	printf("[%s]\n", sql);
	flag = mysql_real_query(&mysql, sql, strlen(sql));
	if(flag)
	{
		printf("insert error, [%d]-[%s]\n", mysql_errno(&mysql),mysql_error(&mysql));
		return -1;
	}

	return 0;
}

int query(st_zdxxb_def *pst_tbl_zdxxb)
{
	st_zdxxb_def st_tbl_zdxxb = {0};
    int flag, i;
    char sql[128] = {0};
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    MYSQL_FIELD *fields = NULL;
    sprintf(sql, "select * from zdxxb where jqbh = %s", pst_tbl_zdxxb->jqbh);
    flag = mysql_real_query(&mysql, sql, (unsigned int)strlen(sql));
    if (flag) {
        printf("query error:%d from %s\n",mysql_errno(&mysql),mysql_error(&mysql));
        return -1;
    }
     
     // 将查询结果读取到内存当中，如果数据很多的情况会比较耗内存
    res = mysql_store_result(&mysql);
    // res = mysql_use_result(&mysql); // 需要用到的时候，每次从服务器中读取一行
     
    // 字段数量
    unsigned int field_count = mysql_field_count(&mysql);
    printf("field_cout:%d\n",field_count);
     
    // 查询总数
    my_ulonglong rows = mysql_num_rows(res);
    printf("%lld\n",rows);
     
    // 获取所有字段
    fields = mysql_fetch_fields(res);
    for (i = 0; i < mysql_num_fields(res); i++) {
         printf("%s\t", fields[i].name);
    }
     
    printf("\n");
    // 遍历结果集
    while((row = mysql_fetch_row(res)))
    {
        for (i = 0; i < mysql_num_fields(res); i++)
        {
            printf("%s\t",row[i]);
			switch_getval(&st_tbl_zdxxb, i, row[i]);
        }
         
        printf("\n");
    }
	memcpy(pst_tbl_zdxxb, &st_tbl_zdxxb, sizeof(st_tbl_zdxxb));
    printf("jqbh[%s], shh[%s], yyzt[%s]\n", st_tbl_zdxxb.jqbh, st_tbl_zdxxb.shh, st_tbl_zdxxb.yyzt);
    printf("sheng[%s], shi[%s], xian[%s], sz[%s], jd[%s]\n", st_tbl_zdxxb.sheng, st_tbl_zdxxb.shi, st_tbl_zdxxb.xian, st_tbl_zdxxb.xz, st_tbl_zdxxb.jd);
    // 释放结果集
    mysql_free_result(res);
     
    return 0;
}



int  main(int argc, char *argv[])
{

	st_zdxxb_def st_tbl_zdxxb = {0};
	memcpy(st_tbl_zdxxb.jqbh, "0002", 4);

    mysql_init(&mysql);

    /*使用mysql_real_connect连接服务器,其参数依次为MYSQL句柄，服务器IP地址，
      登录mysql的用户名，密码，要连接的数据库等*/
    if(!mysql_real_connect(&mysql, "localhost", "elu", "elu123", "amz", 0, NULL, 0)) {
        printf("connecting to Mysql error:%d from %s\n",mysql_errno(&mysql), mysql_error(&mysql));
        return -1;
    }else {
        printf("Connected Mysql successful!\n");
    }
     
#if 0
	// 设置手动提交事务
	mysql_autocommit(&mysql, 0);
     
//    printMySqlInfo();
     
    // 设置编码
    mysql_query(&mysql, "set names utf8");

	memcpy(st_tbl_zdxxb.shm, "测试商户01", 20);
	memcpy(st_tbl_zdxxb.shh, "123456789012345", 15);
	memcpy(st_tbl_zdxxb.zdh, "12345678", 8);
	memcpy(st_tbl_zdxxb.sheng, "广东", 10);
	memcpy(st_tbl_zdxxb.shi, "深圳", 10);
	memcpy(st_tbl_zdxxb.xian, "南山", 10);
	memcpy(st_tbl_zdxxb.xz, "蛇口", 10);
	memcpy(st_tbl_zdxxb.jd, "赤湾", 40);
	memcpy(st_tbl_zdxxb.yyzt, "1", 1);

	insert(&st_tbl_zdxxb);

	query(&st_tbl_zdxxb);

	memcpy(st_tbl_zdxxb.jqbh, "0004", 4);
	memcpy(st_tbl_zdxxb.yyzt, "0", 1);
	
	insert(&st_tbl_zdxxb);

	query(&st_tbl_zdxxb);

	//	事务回滚（在提交事务前）执行
//	mysql_rollback(&mysql);

	// 提交事务
	mysql_commit(&mysql);

#endif
	mysql_close(&mysql);
	return 0;
}


