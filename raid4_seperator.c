#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h> 
#include <errno.h> 

key_t key1,key2;
int msgqid[4],msgrid;
int BLOCK_S = 256;

typedef struct threadInfo
{
	int queue_id;
	int starter;
	char path[256]; 
} tInfo;

typedef struct mymsg {
    long   mtype;       /* Message type. */
    char   mtext[256];    /* Message text. */
} msg;

int init_queue(void)
{
    int queue_id;
    /* attempt to create or open message queue */
    if ( (queue_id = msgget((key_t)0105, 0660 | IPC_CREAT) == -1))
            perror("msgget failed");

    return (queue_id);
}


void *wThread(void * info){

	//printf("Thread is created\n");
	sleep(1);
	tInfo *ti = (tInfo *)info;


	msg *m1 = malloc(sizeof(msg));
	int count = ti->starter;
	
	int j,t=0;
	FILE *fp=NULL;
	struct msqid_ds buf;
	char filename[256];
	int i = 0;
	printf("ti.starter v= %d\n",ti->starter);
	
	// do{
	// 	j = msgctl(ti->queue_id, IPC_STAT, &buf);
		
	// 	printf("buff %ld  %d\n", buf.msg_qnum,j);
	// 	if(buf.msg_qnum <= 0){
	// 		break;
	// 	}
		//else{
			//printf("asdf%d\n",j);
		//printf("%d\n", );
	msgctl(ti->queue_id, IPC_STAT, &buf);
	while(msgrid >= 0){
	
	
		msgctl(ti->queue_id, IPC_STAT, &buf);
		while(buf.msg_qnum == 0) {
			msgctl(ti->queue_id, IPC_STAT, &buf);
			continue;
			//printf("buff %ld\n", buf.msg_qnum);
			//printf("asdfg\n");
		}

		j = msgrcv(ti->queue_id, m1, sizeof(m1->mtext), count, IPC_NOWAIT);
		//while((j = msgrcv(ti->queue_id, m1, sizeof(m1->mtext), count, IPC_NOWAIT))<=-1){}
		//
		//printf("%d\n",j );
		msgsnd(msgrid, m1 , sizeof(m1->mtext),  IPC_NOWAIT);
		
		m1->mtext[j] = '\0';
		char str[j];
		strcpy(str,m1->mtext);
		sprintf(filename, "%s%d/%d.txt", ti->path ,ti->starter,i);
		fp = fopen(filename , "wb");
		if(fp == NULL)
		{
			printf("\nUnable to creat the file");
		}
		printf("%s\n",filename );
		fwrite(&str , j , sizeof(char),fp);
		
		//printf("\n%dheyyyyyyy   %s\n", count,m1->mtext);
		
		i++;
		count+=4;
		fclose(fp);
	}
		//t++;
		//}

	//}while(1==1);
	//printf("T = %d\n", t);
}

/*void *qThread(void * asd){

	printf("Thread is created\n");

	 //Message pmsg;
	 msg *m2 = malloc(sizeof(msg));
	 //printf("%d\n",msgqid1);
	//while (1==1) {
		//msgrcv (first,pmsg);
	int r_qid;
	int j;
	//if((r_qid = init_queue()) == -1)
	//	return (NULL);

	//printf("%d\n", r_qid);
	struct msqid_ds buf;
	int i =0;
	do{
		j = msgctl(msgqid2, IPC_STAT, &buf);
		printf("%ld\n", buf.msg_qnum);
		if(buf.msg_qnum<=0){break;}
		while((msgrcv(msgqid2, m2, sizeof(m2->mtext), 2, IPC_NOWAIT))==-1){
			//printf("asdf\n");
		}
		m2->mtext[256] = '\0';
		printf("\n%dheyyyyyyy   %s\n", j,m2->mtext);
		i++;
	}while(1==1);
	
		//send (second,"1");
	//}
}*/


int main() 
{
	tInfo ti[4];
	msg m[4];
	msg *m1 = malloc(sizeof(msg));
	pthread_t threadid[4];
	int i;
	for (i = 0; i < 4; ++i)
	{
		strcpy(ti[i].path,"/home/raj_dhamsaniya/study3/");
		ti[i].starter = i+1;
		//m[i].mtype = i+5;
	}

	size_t size = sizeof(m[0].mtext);


	msgrid = msgget( (key_t)0131, 0660 | IPC_CREAT);
	
	//key1 = ftok(".",1);
	msgqid[0] =msgget( (key_t)0106, 0660 | IPC_CREAT);
	ti[0].queue_id = msgqid[0];
	printf("%d\n", msgqid[0]);

	msgqid[1] =msgget( (key_t)0126, 0660 | IPC_CREAT);
	ti[1].queue_id = msgqid[1];
	printf("%d\n", msgqid[1]);

	msgqid[2] =msgget( (key_t)0116, 0660 | IPC_CREAT);
	ti[2].queue_id = msgqid[2];
	printf("%d\n", msgqid[2]);

	msgqid[3] =msgget( (key_t)0121, 0660 | IPC_CREAT);
	ti[3].queue_id = msgqid[3];
	printf("%d\n", msgqid[3]);

	
	for (i = 0; i < 4; ++i)
	{
		pthread_create(&threadid[i],NULL,wThread,(void *)&ti[i]);
	}
	
	// strcpy(m[3].mtext , "ofsjvkndkjghdsvjsdlfjsdklfvdhidsihfsdkvndnnlksfjiewojfeshfdjkdnvdkdsvjdshfesufsdkjdnvkjvnkjdhskhnvdkjhasdfghjklasdfghjdassdsdsddsddsddhfdsfjkjdfkdjbvjbjkdvdkjskjfkdsjndskjhdkjfjdskkkkkksjdhvkjvndknckdsklsdvnkdjlkvkldsnndsjdksjkfdsjkfhjdshkjnvcmgnjkfdghdshi");
	// printf("%d\n",msgsnd(msgqid[3], (char *)&m[3] , size,  IPC_NOWAIT));
	// m[3].mtype += 3;
	// printf("%d\n",msgsnd(msgqid[3], (char *)&m[3] , size,  IPC_NOWAIT));
	
	// strcpy(m[2].mtext , "kkkkkksjdhvkjvndknckdsklsdvnkdjlkvkldsnndsjdksjkfdsjkfhjdshkjnvcm,njkfdghdshiofsjvkndkjghdsvjsdlfjsdklfvdhidsihfsdkvndnnlksfjiewojfeshfdjkdnvdkdsvjdshfesufsdkjdnvkjvnkjdhskhnvdkjhasdfghjklasdfghjdassdsdsddsddsddhfdsfjkjdfkdjbvjbjkdvdkjskjfkdsjndskjhdkjfjds");
	// printf("%d\n",msgsnd(msgqid[2], (char *)&m[2] , size,  IPC_NOWAIT));
	
	// strcpy(m[1].mtext , "assdsdsddsddsddhfdsfjkjdfkdjbvjbjkdvdkjskjfkdsjndskjhdkjfjdskkkkkksjdhvkjvndknckdsklsdvnkdjlkvkldsnndsjdksjkfdsjkfhjdshkjnvcmgnjkfdghdshiofsjvkndkjghdsvjsdlfjsdklfvdhidsihfsdkvndnnlksfjiewojfeshfdjkdnvdkdsvjdshfesufsdkjdnvkjvnkjdhskhnvdkjhasdfghjklasdfghjd");
	// printf("%d\n",msgsnd(msgqid[1], (char *)&m[1] , size,  IPC_NOWAIT));
	
	// strcpy(m[0].mtext , "sddsddhfdsfjkjdfkdjbvjbjkdvdkjskjfkdsjndskjhdkjfjdskkkkkksjdhvkjvndknckdsklsdvnkdjlkvkldsnndsjdksjkfdsjkfhjdshkjnvcmgnjkfdghdshiofsjvkndkjghdsvjsdlfjsdklfvdhidsihfsdkvndnnlksfjiewojfeshfdjkdnvdkdsvjdshfesufsdkjdnvkjvnkjdhskhnvdkjhasdfghjklasdfghjdassdsdsdd");
	// printf("%d\n",msgsnd(msgqid[0], (char *)&m[0] , size,  IPC_NOWAIT));

	char filename1[255] = "projectapproval.docx";
	unsigned int count=1,mod;
	struct msqid_ds buf;
	char a[BLOCK_S],b[BLOCK_S],c[BLOCK_S],d[BLOCK_S];
	FILE *fp = fopen(filename1, "rb");
	fseek(fp, 0L, SEEK_END);
	int sz = ftell(fp);
	rewind(fp);

	if ( !feof(fp) )
	{
		do
		{
			if((sz - ftell(fp)) < BLOCK_S ){
				// printf("sz =%d\n",sz);
				// printf("ftell =%d\n",ftell(fp));
				// printf("BS =%d\n",BLOCK_S);
				break;
			}

			// sprintf(filename, "/home/raj_dhamsaniya/study2/%d", count);
			// fp2 = fopen(filename , "wb");
			mod = count % 4;
			if(mod == 1){
				fread(&a, BLOCK_S,sizeof(char) , fp); // read one character
				m[0].mtype = count;
				strcpy(m[0].mtext , a);
				printf("%d\n",msgsnd(msgqid[0], (char *)&m[0] , size,  IPC_NOWAIT));
				//printf("%s\n",filename );
			}
			else if(mod == 2){
				fread(&b, BLOCK_S,sizeof(char) , fp);
				m[1].mtype = count;
				strcpy(m[1].mtext , b);
				printf("%d\n",msgsnd(msgqid[1], (char *)&m[1] , size,  IPC_NOWAIT));
			}
			else if(mod == 3){
				fread(&c, BLOCK_S,sizeof(char) , fp);
				m[2].mtype = count;
				strcpy(m[2].mtext , c);
				printf("%d\n",msgsnd(msgqid[2], (char *)&m[2] , size,  IPC_NOWAIT));

				
		
				//printf("buff %ld  %d\n", buf.msg_qnum);
				msgctl(msgrid, IPC_STAT, &buf);
				while(buf.msg_qnum == 0) {
					msgctl(msgrid, IPC_STAT, &buf);
					//printf("ghjk\n");
				}
				msgrcv(msgrid, m1, sizeof(m1->mtext), count, IPC_NOWAIT);

				count++;
				for(i=0 ; i< strlen(a) ; i++){
					d[i] = (a[i] ^ b[i]) ^ c[i] ; 
				}
					
				m[3].mtype = count;
				strcpy(m[3].mtext , d);
				printf("%d\n",msgsnd(msgqid[3], (char *)&m[3] , size,  IPC_NOWAIT));
			}
			msgctl(msgrid, IPC_STAT, &buf);
		
			//printf("buff %ld  %d\n", buf.msg_qnum);
			while(buf.msg_qnum == 0) {
				msgctl(msgrid, IPC_STAT, &buf);
				//printf("qwerty\n");
			}
			msgrcv(msgrid, m1, sizeof(m1->mtext), count, IPC_NOWAIT);

			count++;
		}while ( !feof(fp) );

		if(ftell(fp) != sz){
			i = (sz - ftell(fp));
			mod = count % 4;
			char a[i];

			//printf("%dfhdhfadhf\n%d\n",i ,count);
			fread(&c, i,sizeof(char) , fp);
			m[mod-1].mtype = count;
			strcpy(m[mod-1].mtext , c);
			printf("%d\n",msgsnd(msgqid[mod-1], (char *)&m[mod-1] , i,  IPC_NOWAIT));
			//printf("asdfghjkl\n");
			while(buf.msg_qnum == 0) {
				msgctl(msgrid, IPC_STAT, &buf);
				//printf("qwerty\n");
			}
			msgrcv(msgrid, m1, sizeof(m1->mtext), count, IPC_NOWAIT);

			count++;
		}
		fclose(fp);
	}

	for (i = 0; i < 4; ++i)
	{
		msgctl(msgqid[i],IPC_RMID,NULL);
	}
	msgctl(msgrid,IPC_RMID,NULL);

	for (i = 0; i < 4; ++i)
	{
		msgqid[i] = -1;
	}
	msgrid = -1;
	for (i = 0; i < 4; ++i)
	{
		pthread_join(threadid[i],NULL);
	}

	for (i = 0; i < 4; ++i)
	{
		msgctl(msgqid[i],IPC_RMID,NULL);
	}

}