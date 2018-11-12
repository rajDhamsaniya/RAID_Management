#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h> 
#include <errno.h> 
#include <dirent.h> 
#include <semaphore.h>

int msgqid[4];
int msgrid[4];
int const BLOCK_S = 256;

sem_t s[4];


typedef struct threadInfo
{
	int sendCh;
	int recvCh;
	int partition;
	int maxFiles;
	char path[256]; 
} tInfo;

typedef struct mymsg {
    long   mtype;       /* Message type. */
    char   mtext[256];    /* Message text. */
} msg;

void * rthread(void * inf){

	sleep(1);
	tInfo * ti = (tInfo *)inf;
	int j,i=0,k;
	msg *m1 = malloc(sizeof(msg));
	msg m2;
	char a[BLOCK_S];
	FILE *fp;
	int sz;
	
	
	char filename[256];
	int file_count = 0;
	DIR * dirp;
	struct dirent * entry;
	printf("Thread created %d\n", ti->partition);
	dirp = opendir(ti->path); 
	while ((entry = readdir(dirp)) != NULL) {
	    if (entry->d_type == DT_REG) { 
	         file_count++;
	    }
	}
	closedir(dirp);
	struct msqid_ds buf;

	printf("%d\n", file_count);
	k = file_count;
	while(k--){
		
		sem_wait(&s[(ti->partition -1)]);
		sprintf(filename , "%s/%d.txt" ,ti->path,i);
		fp = fopen(filename , "rb");
		printf("%s\n",filename );
		fseek(fp, 0L, SEEK_END);
		sz = ftell(fp);
		rewind(fp);
		if(fp == NULL){
			printf("sdfhjkl\n");
		}
		fread(&a,BLOCK_S,sizeof(char),fp);
		//strcpy(m1->mtext , a);
		sprintf(m2.mtext,"%s",a);
		m2.mtype = ti->partition;
		
		msgsnd(ti->sendCh , (char *)&m2 , sz ,MSG_NOERROR);
		
		fclose(fp);
		
		i++;
	}

	if((ti->maxFiles) > file_count){
		sprintf(m2.mtext,"%c",'\0');
		m2.mtype = ti->partition;
		printf("last %d",msgsnd(ti->sendCh , (char *)&m2 , 0 ,MSG_NOERROR));
	}
	else{
		return NULL;
	}
}

void main(){


	tInfo ti[4];
	int i;
	msg am[4];
	msg rm[4];
	size_t size = sizeof(am[1].mtext);
	
	pthread_t threadId[4];
	FILE * fp;
	int len[4];

	int file_count = 0;
	DIR * dirp;
	struct dirent * entry;

	dirp = opendir("/home/raj_dhamsaniya/study3/1/"); 
	while ((entry = readdir(dirp)) != NULL) {
	    if (entry->d_type == DT_REG) {
	         file_count++;
	    }
	}
	closedir(dirp);

	printf("%d\n",file_count );

	msgqid[0] =msgget( (key_t)0106, 0660 | IPC_CREAT);
	msgqid[1] =msgget( (key_t)0126, 0660 | IPC_CREAT);
	msgqid[2] =msgget( (key_t)0116, 0660 | IPC_CREAT);
	msgqid[3] =msgget( (key_t)0121, 0660 | IPC_CREAT);

	msgrid[0] = msgget( (key_t)0131, 0660 | IPC_CREAT);
	msgrid[1] = msgget( (key_t)0136, 0660 | IPC_CREAT);
	msgrid[2] = msgget( (key_t)0141, 0660 | IPC_CREAT);
	msgrid[3] = msgget( (key_t)0146, 0660 | IPC_CREAT);

	for (i = 0; i < 4; ++i)
	{
		sem_init(&s[i], 0, 0);
	}

	fp = fopen("./project/hello.docx","wb");

	for (i = 0; i < 4; ++i)
	{
		ti[i].sendCh = msgrid[i];
		ti[i].recvCh = msgqid[i];
		ti[i].partition = i+1;
		ti[i].maxFiles = file_count;
		sprintf(ti[i].path , "/home/raj_dhamsaniya/study3/%d",i+1);
		strcpy(am[i].mtext,"Hello,WOrld");
		am[i].mtype = i+1;
	}
	size = strlen(am[0].mtext);

	for (i = 0; i < 4; ++i)
	{
		pthread_create(&threadId[i] , NULL , rthread , &ti[i]);
	}

	int j=0;

	while(file_count--){
		
		printf("%d\n", file_count);
		for (i = 0; i < 4; ++i)
		{
			sem_post(&s[i]);
			struct msqid_ds buf;

			msgctl(msgrid[i], IPC_STAT, &buf);
			while(buf.msg_qnum == 0) {
				msgctl(msgrid[i], IPC_STAT, &buf);
			}
			len[i] = msgrcv(msgrid[i] , &rm[i] , sizeof(rm[i].mtext) , i+1 , MSG_NOERROR);
		}

		for (i = 0; i < 3; ++i)
		{
			fwrite(&rm[i].mtext,len[i],sizeof(char) , fp);
			if(len[i] < 256){
				printf("length = %d\n",len[i] );
			}
			
		}
		j++;
	}

	for (i = 0; i < 4; ++i)
	{
		pthread_join(threadId[i],NULL);
	}
}