#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
typedef struct file{
	char name[1030];
	char type[1030];
	char md5[1030];
	int size;
	char time[1030];
}filest;

void readstr(int connfd,int serv_socket,char data[]);
char conn_type[1030];
char cmd[1030];
char parse_cmd[102][102];
int cmd_cnt;
char rec_cmd[1030];
char parse_rec_cmd[1030][1030];
int rec_cmd_cnt;
char send_serv[1030];
char rec[1030];
char temp_time[1030];
filest filestruct[1030];
int serv_cnt;
char *shared_folder="./";
#define compare_time(time1,time2) strcmp(time1,time2)
int up_permission=1;
int down_permission=1;
int decide(char conn_type[],int sock)
{
	if(strcmp(conn_type,"tcp"))
	{
		if (((sock) = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		{
			perror("Socket Error");
			return 1;
		}
	}
	else
	{
		if (((sock) = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		{
			perror("Socket Error");
			return 1;
		}
	}
	return sock;
}
void parse_s(char s[])
{
	if(strcmp(s,"udp")==0)
	{
		strcpy(s,"tcp");
	}
}
void time_parse(char ltime[])
{
	int i;
	//	printf("time :::  %s\n",ltime);
	temp_time[0]=ltime[20];
	temp_time[1]=ltime[21];
	temp_time[2]=ltime[22];
	temp_time[3]=ltime[23];
	temp_time[19]='\0';
	temp_time[7]='-';
	temp_time[10]='-';
	temp_time[4]='-';
	for(i=11;i<19;i++)
	{
		temp_time[i]=ltime[i];
	}
	//	switch(ltime)
	//	{
	if( strstr(ltime,"Jan"))
	{
		temp_time[5]='0';
		temp_time[6]='1';
		//	break;
	}
	else if( strstr(ltime,"Feb"))
	{
		temp_time[5]='0';
		temp_time[6]='2';
		//	break;
	}
	else if( strstr(ltime,"Mar"))
	{
		temp_time[5]='0';
		temp_time[6]='3';
		//break;
	}
	else if( strstr(ltime,"Apr"))
	{
		temp_time[5]='0';
		temp_time[6]='4';
		//	break;
	}
	else if( strstr(ltime,"May"))
	{
		temp_time[5]='0';
		temp_time[6]='5';
		//	break;
	}
	else if( strstr(ltime,"Jun"))
	{
		temp_time[5]='0';
		temp_time[6]='6';
		//	break;
	}
	else if( strstr(ltime,"Jul"))
	{
		temp_time[5]='0';
		temp_time[6]='7';
		//	break;
	}
	else if( strstr(ltime,"Aug"))
	{
		temp_time[5]='0';
		temp_time[6]='8';
		//	break;
	}
	else if( strstr(ltime,"Sep"))
	{
		temp_time[5]='0';
		temp_time[6]='9';
		//	break;
	}
	else if( strstr(ltime,"Oct"))
	{
		temp_time[5]='1';
		temp_time[6]='0';
		//	break;
	}
	else if( strstr(ltime,"Nov"))
	{
		temp_time[5]='1';
		temp_time[6]='1';
		//	break;
	}
	else if( strstr(ltime,"Dec"))
	{
		temp_time[5]='1';
		temp_time[6]='2';
		//	break;
	}
	//	}
	//	switch(ltime[8])
	//	{
	//	printf("ltime[8]=== %c  ltime[9]== %c\n",ltime[8],ltime[9]);
	if(ltime[8]==' ')
	{
		//		printf("entered wt space\n");
		temp_time[8]='0';
		temp_time[9]=ltime[9];
		//	break;
	}
	else 
	{
		//		printf("entered w/o space\n");
		temp_time[8]=ltime[8];
		temp_time[9]=ltime[9];
		//	break;
	}
	//	}

}
void parse_imptime(char ltime[])
{
	char temp[100];
	strcpy(temp,ltime);
	char temp2[100];
	temp2[0]=temp[6];
	temp2[1]=temp[7];
	temp2[2]=temp[8];
	temp2[3]=temp[9];
	temp2[4]='-';
	int i;
	for(i=0;i<6;i++)
	{
		temp2[i+5]=temp[i];
	}
	for(i=11;i<19;i++)
	{
		temp2[i]=temp[i];
	}
	temp2[19]='\0';
	strcpy(ltime,temp2);
}
int cal_md5(char file_name[], char md5_sum[])
{
	char temp[200],c;
	strcpy(temp,"md5sum  ");
	strcat(temp,file_name);
	strcat(temp," >md5sum &");
	system(temp);
	FILE *fp;
	fp=fopen("md5sum","r");
	int i=0;
	while( ( c = fgetc(fp) ) != EOF )
	{
		if(c==' ')
			break;
		md5_sum[i++]=c;
	}
	md5_sum[i]='\0';
	fclose(fp);
//	printf("md5:  %s\n",md5_sum);
	return 32;
}
char *concatenate(char ch1[],char ch2[])
{
	char temp[1030];
	strcpy(temp,ch1);
	strcat(temp," ");
	strcat(temp,ch2);
	char *ret=temp;
	return ret;
}
void serv_parse()
{
	int i;
	for(i=0;rec[i]!='\0';i++)
	{
		rec_cmd[i]=rec[i];
	}
	rec_cmd[i]=rec[i];
	int fl=1,j=0,k=0;
	for(i=0;rec[i]!='\0';i++)
	{
		if(rec[i]!=' ')
		{
			fl=0;
			parse_rec_cmd[j][k++]=rec[i];
		}
		else
		{
			if(fl==0)
			{
				parse_rec_cmd[j][k]='\0';
				k=0;
				fl=1;
				j++;
			}
		}
	}
	//printf("SERVER :: %d %s %s\n",j,rec,parse_rec_cmd[0]);
	parse_rec_cmd[j][k]='\0';
	rec_cmd_cnt=j+1;
}
void scan()
{
	char c;
	scanf("%[^\n]",cmd);
	c=getchar();
	int fl=1,j=0,k=0,i;
	cmd_cnt=0;
	for(i=0;cmd[i]!='\0';i++)
	{
		if(cmd[i]!=' ')
		{
			fl=0;
			parse_cmd[j][k++]=cmd[i];
		}
		else
		{
			if(fl==0)
			{
				parse_cmd[j][k]='\0';
				k=0;
				fl=1;
				j++;
			}
		}
	}
	//printf("%d %s %s\n",j,cmd,parse_cmd[0]);
	parse_cmd[j][k]='\0';
	cmd_cnt=j+1;
}
void get_file_st()
{
	serv_cnt=0;
	int i=0;
	struct stat sr;
	struct dirent *dir_read;
	DIR *dir=opendir(shared_folder);
	if(dir==NULL)
	{
		printf("Failed To Open The Given Directory\n");
	}
	else
	{
		FILE *fp;
		char temp[1030],c;
		dir_read=readdir(dir);
		i=0;
		int j=0;
		while((dir_read) && (dir_read!=NULL))
		{
			//printf("ENETERD FILE\n");
			strcpy(filestruct[i].name,dir_read->d_name);
			stat(dir_read->d_name,&sr);
			strcpy(temp,"file ");
			strcat(temp,dir_read->d_name);
			strcat(temp," >temp &");
			system(temp);
			filestruct[i].size=sr.st_size;
			//sprintf(filestruct[i].time,"%d",ctime(&sr.st_mtime));
			strcpy(filestruct[i].time,ctime(&sr.st_mtime));
			cal_md5(filestruct[i].name,filestruct[i].md5);
			//printf("name %s\n", dir_read->d_name);
			fp=fopen("temp","r");
			j=0;
			//			printf("READINg %s\n",temp);
			while( ( c = fgetc(fp) ) != EOF )
			{
				filestruct[i].type[j++]=c;
				//				fp++;
				//				printf("%c \n",c);
			}
			filestruct[i].type[j]='\0';
			i++;
			dir_read=readdir(dir);
		}
		serv_cnt=i;
	}
}
int connection(int serv_connect,char conn_type[])
{
	if(strcmp(conn_type,"tcp")==0)
	{
		//printf("%d %s\n",serv_connect,conn_type);
		if (listen(serv_connect, 10) == -1) {
			//printf("ecn\n");
			perror("Listen");
			return -1;
		}
	}
	//	if(strcmp(conn_type,"tcp")==0)
	//		printf("TCPServer Waiting for client on port %d\n ",server_port_no);
	//	if(strcmp(conn_type,"udp")==0)
	//		printf("UDPServer Waiting for client on port %d\n ",server_port_no);
}
void recieve(int connfd,int serv_socket,struct sockaddr_in client_addr,socklen_t * local,char conn_type[])
{
	//printf("SER\n");
	int i=0;
	if(strcmp(conn_type,"tcp")==0)
	{
		i=recv(connfd,rec,1024,0);
	}
	if(strcmp(conn_type,"udp")==0)
	{
		i=recvfrom(serv_socket,rec,1024,0,(struct sockaddr *)&client_addr, local);
	}
	rec[i]='\0';
	//	readstr(connfd,serv_socket,rec);
	serv_parse();
	//printf("HEP\n");
}
void writestr(int connfd,int serv_socket,char data[])
{
	if(strcmp(conn_type,"udp")==0)
		connfd=serv_socket;
	int g=strlen(data);
	int arr[1];
	arr[0]=g;
	write(connfd,arr,sizeof(arr));
	write(connfd,data,g);
}
void readstr(int connfd,int serv_socket,char data[])
{
	int arr[1];
	if(strcmp(conn_type,"udp")==0)
		connfd=serv_socket;
	read(connfd,arr,sizeof(arr));
	read(connfd,data,arr[0]);
	//printf("%s %d\n",data,arr[0]);
	data[arr[0]]='\0';
}
void readint(int connfd,int serv_socket,int data[])
{
	if(strcmp(conn_type,"udp")==0)
		connfd=serv_socket;
	read(connfd,data,sizeof(data));
}
void writeint(int connfd,int serv_socket,int data[])
{
	if(strcmp(conn_type,"udp")==0)
		connfd=serv_socket;
	write(connfd,data,sizeof(data));
}
void sendstr(int connfd,int serv_socket,struct sockaddr_in client_addr,char conn_type[],char data[],int size)
{
	if(strcmp(conn_type,"tcp")==0)
		send(connfd,data,size,0);
	else
		sendto(serv_socket,data,size,0,(struct sockaddr *)&client_addr, sizeof(struct sockaddr));

}
void sendint(int connfd,int serv_socket,struct sockaddr_in client_addr,char conn_type[],int *data,int size)
{
	if(strcmp(conn_type,"tcp")==0)
		send(connfd,data,sizeof(int),0);
	else 
		sendto(serv_socket,data,sizeof(int),0,(struct sockaddr *)&client_addr, sizeof(struct sockaddr));
}
void recstr(int connfd,int serv_socket,struct sockaddr_in client_addr,char conn_type[],socklen_t * local,char rec[],int size)
{
	int i=0;
	if(strcmp(conn_type,"tcp")==0)
		i=recv(connfd,rec,size,0);
	if(strcmp(conn_type,"udp")==0)
		i=recvfrom(serv_socket,rec,size,0,(struct sockaddr *)&client_addr, local);
	rec[i]='\0';
}
void recint(int connfd,int serv_socket,struct sockaddr_in client_addr,char conn_type[],socklen_t * lol,int *data,int size)
{
	if(strcmp(conn_type,"tcp")==0)
		recv(connfd,data,size,0);
	if(strcmp(conn_type,"udp")==0)
		recvfrom(serv_socket,data,size,0,(struct sockaddr *)&client_addr,lol);
	//	printf("%d\n",recv);
}
void help_cmd()
{
}


int compute_length(char data[])
{
	return strlen(data);
}
int client_code(int client_port_no)
{
	//	strcpy(conn_type,conn_type);
	//	printf("Client1\n");
	//	printf("Client1\n");
	//	printf("Client1\n");
	//	printf("Client1\n");
	//	printf("Client1\n");
	//printf("Client1\n");
	filest FileDescription[1030];
	parse_s(conn_type);
	//printf("ENT\n");
	char md5_array[33];
	char md5_recv[33];
	int sock,num_bytes_recieved;
	char send_data[1030],recv_data[1030];
	int recv_data_int;
	struct hostent *host;
	//printf("ENT\n");
	struct sockaddr_in server_addr;                // server_addr
	host = gethostbyname("127.0.0.1");
	//int f=decide(conn_type,sock),i;
	int f,i;
	//	sock=f;
	if(strcmp(conn_type,"tcp")==0){

		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("Socket");
			return 1;
		}    
	}    
	if(strcmp(conn_type,"udp")==0){

		if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
			perror("Socket");
			return 1;
		}    
	}    
	//	if(f==1)
	//	{
	///		return 1;
	//	}
	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(client_port_no);   
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);
	int sin_size = sizeof(struct sockaddr_in);
	socklen_t * local= (socklen_t *) &sin_size;
	if(strcmp(conn_type,"tcp")==0)
	{
		if (connect(sock,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1)
		{
			perror("Connect");
			return 2;
		}
		printf("\n CLIENT : Connected to Port No %d\n",client_port_no);
	}
	while(1)
	{
		//		printf("CLIENT-----ENTERED\n");
		scan();
		FILE *ft;
		ft=fopen("history.txt","a");
		char tem[1050];
		memset(tem,'\0',1050);
		strcpy(tem,cmd);
		strcat(tem,"\n");
		fwrite(tem,1,strlen(tem),ft);
		fclose(ft);
		//printf("INPUT %s\n",parse_cmd[0]);
		//		printf("%s\n",parse_cmd[0]);
		if(strcmp(parse_cmd[0],"exit")==0)
		{
//			exit(0);
			return -10;
			break;
		}
		else if(strcmp(parse_cmd[0],"help")==0)
		{
			//printf("???\n");
			help_cmd();
		}
		else if(strcmp(parse_cmd[0],"history")==0)
		{
			ft=fopen("history.txt","r");
			int h;
			memset(tem,'\0',1050);
			//		printf("asdasdasd\n");
			while((h = fread(tem, 1, 1024, ft)) > 0)
			{
				printf("%s",tem);
				memset(tem,'\0',1050);
			}
			fclose(ft);
		}
		else if(strcmp(parse_cmd[0],"server-history")==0)
		{
			writestr(sock,sock,cmd);				
			int arr[1],size=0;
			readint(sock,sock,arr);
			size=arr[0];
			memset(recv_data,'\0',1030);
			read(sock,recv_data,arr[0]);
//			readint(sock,sock,arr);
//			command_cnt=arr[0];
//			for(i=0;i<command_cnt;i++)
			while(strcmp(recv_data,"EOF")!=0)
			{    
				printf("%s\n",recv_data);
				readint(sock,sock,arr);
				size=arr[0];
				memset(recv_data,'\0',1030);
				read(sock,recv_data,arr[0]);
	//			printf("%s\n",recv_data);
			}    

		}
		else if(strcmp(parse_cmd[0],"Download")==0)
		{
			if(cmd_cnt<2)
			{
				printf("Missing File Name\n");
			}
			else
			{
				//sendstr(sock,sock,server_addr,conn_type,cmd,1024);

				//				write(sock,cmd,1024);
				writestr(sock,sock,cmd);				
				memset(recv_data,'\0',1030);
				//read(sock,recv_data,1024);
				readstr(sock,sock,recv_data);
				//recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
				//		num_bytes_recieved=compute_length(recv_data);
				//		recv_data[num_bytes_recieved]='\0';
				//printf("lollll : %s\n",recv_data);
				if(strcmp(recv_data,"file doesn't exist")!=0)   // If file exists which you wish to download -----------
				{

					// If file exist first thing server does is send md5 encryption of file which
					// is recieved diffrently in tcp and udp

					memset(recv_data,'\0',sizeof(recv_data));

					read(sock,recv_data,33);
					//recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
					num_bytes_recieved=compute_length(recv_data);
					recv_data[num_bytes_recieved]='\0';

					// COPIED  md5 of file name ------------------------------------------------------

					strcpy(md5_recv,recv_data);

					// Revieved First packet ---------------------------------------
					recv_data_int=0;
					//recint(sock,sock,server_addr,conn_type,local,&recv_data_int,sizeof(recv_data_int));

					int arr[1];
					read(sock,arr,sizeof(arr));
					recv_data_int=arr[0];

					//printf("%s %d\n",md5_recv,arr[0]);
					// Recieved Size of first packet --------------------------------------------

					memset(recv_data,'\0',1030);
					read(sock,recv_data,recv_data_int);
					recv_data[recv_data_int]='\0';
					//recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
					num_bytes_recieved=compute_length(recv_data);
					//recv_data[num_bytes_recieved]='\0';

					// Open The File with same extension

					//printf("%d %s\n",recv_data_int,recv_data);
					FILE *fptr;
					fptr=fopen(parse_cmd[1],"w");
					while(strcmp(recv_data,"EOF")!=0)
					{
						fwrite(recv_data, 1, recv_data_int, fptr);
						/*	for(i=0;i<recv_data_int;i++)
							{
							fprintf(fptr,"%c",recv_data[i]);
							}*/


						//if(recv_data_int<0)
							//printf("%d\n",recv_data_int);
						//if(recv_data_int==0)
						//			printf("I:  %d\n",recv_data_int);
						//			if(recv_data_int==0)
						//				break;
						//	break;
						read(sock,arr,sizeof(arr));
						recv_data_int=arr[0];
						//	recint(sock,sock,server_addr,conn_type,local,&recv_data_int,sizeof(recv_data_int));
						//			printf("F ::%d\n",recv_data_int);

						// Recieved Size of the packet --------------------------------------------

						memset(recv_data,'\0',1030);
						read(sock,recv_data,recv_data_int);
						//recv_data[recv_data_int]='\0';
						//recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
						num_bytes_recieved=compute_length(recv_data);
						//	recv_data[num_bytes_recieved]='\0';
						//	if(recv_data_int<=0)
						//if(recv_data_int<0)
						//	printf("%d\n",recv_data_int);
						//	printf("%s\n",recv_data);
						//						if(recv_data_int==0)
						//							break;
						//printf("%d\n",recv_data_int,recv_data);
					}
					//printf("LETS GO AI\n");
					fclose(fptr);
					cal_md5(parse_cmd[1], md5_array);
					if(strcmp(md5_array,md5_recv)==0)
					{
						printf("File Download completed\n");
					}
					else
					{
						printf("check sum error\n");
					}

				}
				else
				{
					printf("No such file found ------------ERROR----------------------\n");
				}
			}
		}
		else if(strcmp(parse_cmd[0],"IndexGet")==0)
		{
			//printf("INdex\n");
			int arr[1];
			//			memset(cmd,'\0',sizeof(cmd));
			//		write(sock,cmd,1024);
			writestr(sock,sock,cmd);
			//sendstr(sock,sock,server_addr,conn_type,cmd,1024);
			read(sock,arr,sizeof(arr));
			recv_data_int=arr[0];
			//recint(sock,sock,server_addr,conn_type,local,&recv_data_int,sizeof(recv_data_int));
			int num_of_file=recv_data_int,i;
			//printf("%d\n",num_of_file);
			for(i=0;i<num_of_file;i++)
			{
				// Recieving name of the file-------------------------------------------------

				memset(recv_data,'\0',sizeof(recv_data));
				/*				read(sock,recv_data,1024);
				//			recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
				num_bytes_recieved=compute_length(recv_data);
				recv_data[num_bytes_recieved]='\0';

				 */
				readstr(sock,sock,recv_data);
				// Copied name of the file in the structure -------------------------------------------------

				strcpy(FileDescription[i].name,recv_data);

				// Recieving type of file --------------------------------------

				memset(recv_data,'\0',sizeof(recv_data));
				/*				read(sock,recv_data,1024);
				//recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
				num_bytes_recieved=compute_length(recv_data);
				recv_data[num_bytes_recieved]='\0';
				 */
				readstr(sock,sock,recv_data);
				// Copying type of file in struct -----------------------------------------

				strcpy(FileDescription[i].type,recv_data);

				// Recieving size of file --------------------------------

				read(sock,arr,sizeof(arr));
				recv_data_int=arr[0];
				//recint(sock,sock,server_addr,conn_type,local,&recv_data_int,sizeof(recv_data_int));

				FileDescription[i].size=recv_data_int;

				// Recieving time of last modification of file

				memset(recv_data,'\0',sizeof(recv_data));
				/*				read(sock,recv_data,1024);
				//recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
				num_bytes_recieved=compute_length(recv_data);
				recv_data[num_bytes_recieved]='\0';
				 */
				readstr(sock,sock,recv_data);
				// Copying last modified time in structure

				strcpy(FileDescription[i].time,recv_data);

			}
			if(strcmp(parse_cmd[1],"ShortList")==0)
			{
				char temp1[1000],temp2[1000];
				strcat(parse_cmd[2]," ");
				strcat(parse_cmd[2],parse_cmd[3]);
				strcat(parse_cmd[4]," ");
				strcat(parse_cmd[4],parse_cmd[5]);
				strcpy(temp1,parse_cmd[2]);
				strcpy(temp2,parse_cmd[4]);
				parse_imptime(temp1);
				parse_imptime(temp2);
				int i=0;
				while(i!=num_of_file)
				{
					time_parse(FileDescription[i].time);
					//					printf("%s	%s	%s\n",temp1,temp_time,temp2);
					if(compare_time(temp_time,temp1)>=0 && compare_time(temp2,temp_time)>=0)
					{
						printf("File Name - %s\nFile Size - %d\nFile Type - %s\nLast Modification Time - %s\n",FileDescription[i].name,FileDescription[i].size,FileDescription[i].type,FileDescription[i].time);
					}
					i++;
				}
			}
			else if(strcmp(parse_cmd[1],"LongList")==0)
			{
				//printf("LLL\n");
				int i=0;
				while(i!=num_of_file)
				{
					printf("File Name - %s\nFile Size - %d\nFile Type - %s\nLast Modification Time - %s\n",FileDescription[i].name,FileDescription[i].size,FileDescription[i].type,FileDescription[i].time);
					i++;
				}
			}
			else if(strcmp(parse_cmd[1],"regEx")==0)
			{
				memset(recv_data,'\0',sizeof(recv_data));
				readstr(sock,sock,recv_data);
				//recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
				num_bytes_recieved=compute_length(recv_data);
				//recv_data[num_bytes_recieved]='\0';
				for(;strcmp(recv_data,"EOF")!=0;)
				{
					int i=0;
					for(i=0;i<num_of_file;i++)
					{
						if(strcmp(recv_data,FileDescription[i].name)==0)
						{
							//printf("\n\n\n\n");
							printf("File Name - %s\nFile Size - %d\nFile Type - %s\nLast Modification Time - %s\n",FileDescription[i].name,FileDescription[i].size,FileDescription[i].type,FileDescription[i].time);
						}
					}
					memset(recv_data,'\0',sizeof(recv_data));
					readstr(sock,sock,recv_data);
					//recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
					num_bytes_recieved=compute_length(recv_data);
					//	recv_data[num_bytes_recieved]='\0';
				}
			}
		}
		else if(strcmp(parse_cmd[0],"fileHash")==0)
		{
			writestr(sock,sock,cmd);
			int arr[1];
			readint(sock,sock,arr);
			recv_data_int=arr[0];
			//sendstr(sock,sock,server_addr,conn_type,cmd,strlen(cmd));
			//recint(sock,sock,server_addr,conn_type,local,&recv_data_int,sizeof(recv_data_int));
			int num_of_file=recv_data_int,i;
			for(i=0;i<num_of_file;i++)
			{
				// Recieving name of the file-------------------------------------------------

				memset(recv_data,'\0',sizeof(recv_data));
				readstr(sock,sock,recv_data);
				//recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
				num_bytes_recieved=compute_length(recv_data);
				//	recv_data[num_bytes_recieved]='\0';

				// Copied name of the file in the structure -------------------------------------------------

				strcpy(FileDescription[i].name,recv_data);

				// Recieving type of file --------------------------------------

				memset(recv_data,'\0',sizeof(recv_data));
				readstr(sock,sock,recv_data);
				//	recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
				num_bytes_recieved=compute_length(recv_data);
				//		recv_data[num_bytes_recieved]='\0';

				// Copying type of file in struct -----------------------------------------

				strcpy(FileDescription[i].type,recv_data);

				// Recieving size of file --------------------------------
				readint(sock,sock,arr);
				//		recint(sock,sock,server_addr,conn_type,local,&recv_data_int,sizeof(recv_data_int));
				recv_data_int=arr[0];
				FileDescription[i].size=recv_data_int;

				// Recieving time of last modification of file

				memset(recv_data,'\0',sizeof(recv_data));
				readstr(sock,sock,recv_data);
				//recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
				num_bytes_recieved=compute_length(recv_data);
				//recv_data[num_bytes_recieved]='\0';

				// Copying last modified time in structure

				strcpy(FileDescription[i].time,recv_data);



				/*** File md5Hash Packet*******/

				memset(recv_data,'\0',sizeof(recv_data));
				//	readstr(sock,sock,recv_data);
				read(sock,recv_data,33);
				//recstr(sock,sock,server_addr,conn_type,local,recv_data,33);
				num_bytes_recieved=compute_length(recv_data);
				recv_data[num_bytes_recieved]='\0';

				strcpy(FileDescription[i].md5,recv_data);
			}
			if(strcmp(parse_cmd[1],"Verify")==0)
			{
				int i=0;
				while(i!=num_of_file)
				{
					if(strcmp(parse_cmd[2],FileDescription[i].name)==0)
					{
						printf("File Name - %s\nFile Size - %d\nFile Type - %s\nLast Modification Time - %s\nMd5 hash %s\n",FileDescription[i].name,FileDescription[i].size,FileDescription[i].type,FileDescription[i].time,FileDescription[i].md5);	
					}
					i++;
				}
				if(i==num_of_file)
				{
					printf("No such File On remote machine\n");
				}
			}
			else if(strcmp(parse_cmd[1],"checkAll")==0)
			{
				int i=0;
				while(i!=num_of_file)
				{
					printf("File Name - %s\nFile Size - %d\nFile Type - %s\nLast Modification Time - %s\nMd5 hash %s\n",FileDescription[i].name,FileDescription[i].size,FileDescription[i].type,FileDescription[i].time,FileDescription[i].md5);
					i++;
				}
			}
		}
		else if(strcmp(parse_cmd[0],"upload")==0)
		{
			if(cmd_cnt>=2)
			{
				FILE *fptr;
				fptr = fopen(parse_cmd[1],"r");
				//ifstream ifile(parse_cmd[1]);
				if(fptr)
				{
					int arr[1];
					// sending upload command ----------------------
					//write(sock,cmd,1024);
					writestr(sock,sock,cmd);
					//sendstr(sock,sock,server_addr,conn_type,cmd,1024);

					// recieving permission header

					memset(recv_data,'\0',sizeof(recv_data));
					//read(sock,recv_data,1024);
					readstr(sock,sock,recv_data);
					//					recstr(sock,sock,server_addr,conn_type,local,recv_data,1024);
					num_bytes_recieved=compute_length(recv_data);
					//recv_data[num_bytes_recieved]='\0';
					printf("%s\n",recv_data);
					if(strcmp(recv_data,"allowed")==0)
					{
						char ch;
						//			FILE *fptr;
						//			fptr = fopen(parse_cmd[1],"r");
						int cnt;
						write(sock,md5_array,33);
						//sendstr(sock,sock,server_addr,conn_type,md5_array,33);
						while(fscanf(fptr,"%c",&ch)!=EOF)
						{
							cnt=0;
							send_serv[cnt++]=ch;
							while(cnt<1024 && fscanf(fptr,"%c",&ch)!=EOF)
							{
								send_serv[cnt++]=ch;
							}
							send_serv[cnt]='\0';
							arr[0]=cnt;
							// sending packet size----------------
							write(sock,arr,sizeof(arr));
							//sendint(sock,sock,server_addr,conn_type,&cnt,sizeof(int));

							//sending packet
							write(sock,send_serv,cnt);
							//sendstr(sock,sock,server_addr,conn_type,send_serv,cnt);
						}
						int g_val=0;
						arr[0]=3;
						write(sock,arr,sizeof(arr));
						//sendint(sock,sock,server_addr,conn_type,&g_val,sizeof(int));
						memset(send_serv,'\0',sizeof(send_serv));
						strcpy(send_serv,"EOF");
						write(sock,send_serv,cnt);
						//sendstr(sock,sock,server_addr,conn_type,"EOF\0",1024);
					}
					else
					{
						printf("File upload not allowed\n");
					}
				}
				else
				{
					printf("Some Error\n");
				}
			}
			else
			{

			}
		}
		else
		{
			printf("Invalid command\n");
		}
	}
	return 0;
}

int serv_code(int server_port,char conn_type[])
{
	//printf("server\n");
	struct sockaddr_in server_addr,client_addr;
	int serv_socket,f,connfd;
	parse_s(conn_type);
	//	f=decide(conn_type,serv_socket);
	//	serv_socket=f;
	//	if(f==0)
	//		return;
	if(strcmp(conn_type,"tcp")==0){

		if ((serv_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("Socket");
			return -1;
		}    
	}    
	if(strcmp(conn_type,"udp")==0){

		if ((serv_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
			perror("Socket");
			return -1;
		}    
	}    
	//printf("%d\n",serv_socket);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);
	int l=bind(serv_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	if(l==-1)
	{
		printf("Error In Binding\n");
		return 1;
	}
	l=connection(serv_socket,conn_type);
	if(l==-1)
		return -1;
	char md5[33],rec_md5[33];
	int gi=0,i;
	while(1)
	{
		int sin_size = sizeof(struct sockaddr_in);
		socklen_t * local = (socklen_t *) &sin_size;
		if(strcmp(conn_type,"tcp")==0)
		{
			//printf("WAIT\n");
			connfd = accept(serv_socket, (struct sockaddr*)&client_addr ,local);
			//printf("ENTERED\n");
		}
		while(1)
		{
			int bytes_recieved;
			//printf("st\n");
			memset(rec,'\0',1030);
			recieve(connfd,serv_socket,client_addr,local,conn_type);
			//printf("1) %s\n",rec);
			if(strcmp(parse_rec_cmd[0],"exit")==0)
			{
				printf("Connection Closed\n");
				close(connfd);
			//	sendstr(connfd,serv_socket,client_addr,conn_type,rec,1024);
				return -10;
//				exit(0);
				break;
			}
			int *data,dt=0;
			char str[1030];
			int size=0,fl=0;
			if(strcmp((parse_rec_cmd[0]),"Download")==0)
			{
				int arr[1];
				FILE *fp;
				//printf("1\n");
				fp=fopen(parse_rec_cmd[1],"r");
				if(fp!=NULL)
				{
					//						sendstr(connfd,server_socket,client_addr,conn_type,str,size)
					//							sendint(connfd,server_socket,client_addr,conn_type,data,size);
					memset(str,'\0',1030);
				//	printf("%s\n",parse_rec_cmd[1]);
					strcpy(str,"file exists");
					size=1024;
					writestr(connfd,serv_socket,str);
					//		write(connfd,str,size);
					//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
					fl=cal_md5(parse_rec_cmd[1],md5);
					if(fl==0)
						printf("Error in md5\n");
					else
						printf("MD5 sum : %s\n",md5);
					char temp_c[2],c;
					int cnt=0;
					size=33;
					memset(str,'\0',1030);
					strcpy(str,md5);
					write(connfd,str,size);
					//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
					memset(str,'\0',1030);
				//	FILE *fp2;
				//	fp2=fopen("2.mp3","w");
					//while(fscanf(fp,"%c",&c)!=EOF)
					while((cnt = fread(str, 1, 1024, fp)) > 0)
					{
						strcpy(send_serv,str);
						size=sizeof(int);
						//dt=cnt;
						if(cnt<=0)
							printf("%d\n",cnt);
						//data=&dt;
						//						printf("%d\n",*data);
						arr[0]=cnt;
						write(connfd,arr,sizeof(arr));
						//sendint(connfd,serv_socket,client_addr,conn_type,data,size);
						size=cnt;
						write(connfd,str,size);
						//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
						memset(str,'\0',1030);
					}
					size=sizeof(int);
					fclose(fp);
				//	fclose(fp2);
					gi=0;
					data=&gi;
					arr[0]=3;
					write(connfd,arr,sizeof(arr));
					//sendint(connfd,serv_socket,client_addr,conn_type,data,size);
					memset(str,'\0',1030);
					strcpy(str,"EOF");
					size=sizeof(str);
				//	printf("size :: %d\n",size);
					size=arr[0];
				//	printf("SERV-Fin\n");
					write(connfd,str,size);
					//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
				}
				else
				{
					strcpy(str,"file doesn't exist");
					size=1024;
					sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
				}
			}
			if(strcmp(parse_rec_cmd[0],"server-history")==0)
			{
				FILE *ft;
				ft=fopen("history.txt","r");
				int h;
				char tem[1050];
				memset(tem,'\0',1050);
				//		printf("asdasdasd\n");
				int arr[1];
				while((h = fread(tem, 1, 1024, ft)) > 0)
				{
					arr[0]=h;
					write(connfd,arr,sizeof(arr));
					size=h;
					write(connfd,tem,size);
//					printf("%s",tem);
					memset(tem,'\0',1050);
				}
				fclose(ft);
				arr[0]=3;
				write(connfd,arr,sizeof(arr));
				memset(tem,'\0',1050);
				strcpy(tem,"EOF");
				write(connfd,tem,arr[0]);
			}
			if(strcmp((parse_rec_cmd[0]),"IndexGet")==0)
			{
			//	printf("2\n");
				get_file_st();
			///	printf("%d\n",serv_cnt);
				data=&serv_cnt;
				size=sizeof(int);
				//				sendint(connfd,serv_socket,client_addr,conn_type,data,size);
				int arr[1];
				arr[0]=serv_cnt;
				write(connfd,arr,sizeof(arr));
				for(i=0;i<serv_cnt;i++)
				{
					memset(str,'\0',1030);
					strcpy(str,filestruct[i].name);
					/*					size=1024;
										write(connfd,str,size);*/
					writestr(connfd,serv_socket,str);
					//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);

					memset(str,'\0',1030);
					strcpy(str,filestruct[i].type);
					/*		size=1024;
							write(connfd,str,size);*/
					writestr(connfd,serv_socket,str);
					//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);

					//strcpy(str,filestruct[i].size);
					//	size=1024;
					//	sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
					size=sizeof(int);
					data=&filestruct[i].size;
					arr[0]=filestruct[i].size;
					write(connfd,arr,sizeof(arr));
					//sendint(connfd,serv_socket,client_addr,conn_type,data,size);

					memset(str,'\0',1030);
					strcpy(str,filestruct[i].time);
					size=1024;
					/*		write(connfd,str,size);*/
					writestr(connfd,serv_socket,str);
					//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
				}
				if(rec_cmd_cnt>1 && strcmp(parse_rec_cmd[1],"regEx")==0)
				{
/*					memset(str,'\0',1030);
					strcpy(str,"ls | grep .");
					char *tmp=parse_rec_cmd[2];
					tmp++;
					strcat(str,tmp);
					strcat(str," >temp");
					system(str);
					FILE *fp;
					fp=fopen("temp","r");
					while(fscanf(fp,"%s",send_serv)!=EOF)
					{
						strcpy(str,send_serv);
						size=1024;
						writestr(connfd,serv_socket,str);
						//	sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
					}
					*/
					char *tmp=parse_rec_cmd[2];
					tmp++;
					for(i=0;i<serv_cnt;i++)
					{
						if(strstr(filestruct[i].name,tmp))
						{
							memset(str,'\0',1030);
							strcpy(str,filestruct[i].name);
							writestr(connfd,serv_socket,str);
						}
					}
					memset(str,'\0',1030);
					strcpy(str,"EOF");
					//size=1024;
					writestr(connfd,serv_socket,str);
					//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
				}
			}
			if(strcmp((parse_rec_cmd[0]),"fileHash")==0)
			{
			//	printf("3\n");
				get_file_st();
				data=&serv_cnt;
				size=sizeof(int);
				int arr[1];
				arr[0]=serv_cnt;
				writeint(connfd,serv_socket,arr);
				//		printf("%d\n",arr[);
				//		sendint(connfd,serv_socket,client_addr,conn_type,data,size);
				for(i=0;i<serv_cnt;i++)
				{
					memset(str,'\0',1030);
					strcpy(str,filestruct[i].name);
					size=1024;
					writestr(connfd,serv_socket,str);
					//			sendstr(connfd,serv_socket,client_addr,conn_type,str,size);

					memset(str,'\0',1030);
					strcpy(str,filestruct[i].type);
					size=1024;
					writestr(connfd,serv_socket,str);
					//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);

					//						strcpy(str,filestruct[i].size);
					//						size=1024;
					//						sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
					size=sizeof(int);
					data=&filestruct[i].size;
					arr[0]=filestruct[i].size;
					writeint(connfd,serv_socket,arr);
					//sendint(connfd,serv_socket,client_addr,conn_type,data,size);

					memset(str,'\0',1030);
					strcpy(str,filestruct[i].time);
					size=1024;
					writestr(connfd,serv_socket,str);
					//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);

					memset(str,'\0',1030);
					strcpy(str,filestruct[i].md5);
					size=33;
					write(connfd,str,size);
					//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
				}
			}
			if(strcmp(parse_rec_cmd[0],"upload")==0)
			{
			//	printf("4\n");
				if(up_permission==0)
				{
					memset(str,'\0',1030);
					strcpy(str,"deny");
					size=1024;
					writestr(connfd,serv_socket,str);
					//		sendstr(connfd,serv_socket,client_addr,conn_type,str,size);
					continue;
				}
				else
				{
					memset(str,'\0',1030);
					strcpy(str,"allowed");
					size=1024;
					writestr(connfd,serv_socket,str);
					//write(connfd,str,size);
					//sendstr(connfd,serv_socket,client_addr,conn_type,str,size);

					//			recstr(connfd,serv_socket,client_addr,conn_type,local,rec,size);
					size=33;
					memset(rec,'\0',1030);
					//readstr(connfd,serv_socket,rec);
					read(connfd,rec,size);
					//recstr(connfd,serv_socket,client_addr,conn_type,local,rec,size);
					//memset(rec,'\0',1030);
					strcpy(rec_md5,rec);
					int arr[1];
					fl=0;
					data=&fl;
					size=sizeof(int);
					read(connfd,arr,sizeof(arr));
					fl=arr[0];
					//recint(connfd,serv_socket,client_addr,conn_type,local,data,size);

					size=fl;
					memset(rec,'\0',1030);
					read(connfd,rec,size);
					//recstr(connfd,serv_socket,client_addr,conn_type,local,rec,size);
					//					rec[strlen(rec)]='\0';

					FILE *filept;
					filept=fopen(parse_rec_cmd[1],"w");
					while(strcmp(rec,"EOF")!=0)
					{
						for(i=0;i<fl;i++)
						{
							fprintf(filept,"%c",rec[i]);
							//							printf("123123: %c\n",rec[i]);
						}

						fl=0;
						data=&fl;
						size=sizeof(int);
						read(connfd,arr,sizeof(arr));
						fl=arr[0];
						//			recint(connfd,serv_socket,client_addr,conn_type,local,data,size);
						//			if(fl==0)
						//				break;
						size=fl;
						memset(rec,'\0',1030);
						read(connfd,rec,size);
						//		recstr(connfd,serv_socket,client_addr,conn_type,local,rec,size);
						//		rec[strlen(rec)]='\0';
						printf("%d\n",fl);
					}
					fclose(filept);
					/*		fl=cal_md5(parse_rec_cmd[1],md5);
							if(fl==0)
							printf("Error in md5\n");
							else
							printf("MD5 sum : %s\n",md5);
							if(strcmp(rec_md5,md5)==0)
							{
							printf("MD5 matched\n");
							}
							else
							printf("MD5 not matched\n");
					 */
				}
			}
		}
	}
	close(serv_socket);
	return 0;
}
int main()
{
	char c;
	FILE *fp;
	fp=fopen("history.txt","w");
	fclose(fp);
	int server_port,rec_port;
	scanf("%d",&server_port);
	scanf("%d",&rec_port);
	scanf("%s",conn_type);
	int pid;
	c=getchar();
	printf("%c\n",c);
	pid=fork();
	if(pid==-1)
		return 0;
	if(pid!=0)
	{
		serv_code(server_port,conn_type);
//		printf("Right\n");
	}
	else
	{
		int flag;
		while(client_code(rec_port)>0)
		{
//			printf("%s\n",conn_type);
			//		client_code(rec_port);
			sleep(1);
			//			printf("left\n");
			//			flag=client_code(rec_port);
			//			if(flag<0)
			//				break;
		}
//		printf("LEFT\n");
	}
	kill(0,SIGQUIT);
	return 0;
}
