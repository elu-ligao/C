#include "msgs.h"
#include "ipc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/msg.h>

EXEC SQL include "DbsLen.h";
EXEC SQL include "DbsTbl.h";


void do_deal(int);

int main(int argc, char *argv[])
{
	int msgid = -1;
	int iRet = -1;
	int iCnt = 0;

	msgid = msgget((key_t)KEY_DEAL, 0666 | IPC_CREAT);
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	do
	{
		iRet = DbsConnect();
		if(iRet == 0)
			break;
		sleep(30);
	}while(iCnt++ < 5);

	if(iCnt == 5)
	{
		printf("DbsConnect error, iRet[%d]\n", iRet);
		return -1;
	}

	do_deal(msgid);

	if(msgctl(msgid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}


void do_deal(int msgid)
{
	long  msgtype = 1;
	st_ipc_def	stInIpc;
	struct st_msg data, msgs;
	int msgid2 = -1;

	msgid2 = msgget(KEY_COMMCUP, IPC_EXCL);
	if(msgid2 < 0)
	{
		printf("msq not existed! errno=%d [%s]\n",errno,strerror(errno));
		return;
	}
	while(1)
	{
		usleep(1000);
		if(msgrcv(msgid, (void*)&data, BUFSIZ, msgtype, MSG_NOERROR))
		{
			printf("msgrcv success\n");
			memset(&stInIpc, 0, sizeof(stInIpc));
			memcpy(&stInIpc, data.msg_data, sizeof(stInIpc));
			/* 收到一条请求 */
			if(0 == memcpy(stInIpc.xxlx, "0200", 4) && 0 == memcpy(stInIpc.field_03, "000000", 6))
			{
				iTxn_0200(&stInIpc);
			}

			/* 返回给Bridge */
			memset(&msgs, 0, sizeof(msgs));
			msgs.msg_type = data.msg_type;
			msgs.sock = data.sock;
			memcpy(msgs.msg_data, data.msg_data, strlen(data.msg_data));
			while(1)
			{
				if(msgsnd(msgid2,&msgs, sizeof(msgs)-sizeof(long),IPC_NOWAIT) < 0)
				{
					if(errno != EAGAIN)   
					{
						printf("msgsnd() write msg failed,errno=%d[%s]\n",errno,strerror(errno));
					}
					sleep(1);
					continue;
				}
				else
					break;
			}
		}
	}
	return;
}
