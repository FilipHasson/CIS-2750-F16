#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_QUERY 512
#define HOSTNAME "dursley.socs.uoguelph.ca"
#define USERNAME "fhasson"
#define PASSWORD "0878596"
#define DATABASE "fhasson"

void error (char *msg, MYSQL *mysql){
	printf("%s\n%s\n",msg,mysql_error(mysql));
	exit(1);
}

void clrstr(char *buf){
	buf[0] = '\0';
}

void needHelp (){
	printf("Improper usage './db -help' for options\n");
	exit(1);
}

void init(MYSQL *mysql){
	mysql_init(mysql);
	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if(!mysql_real_connect(mysql, HOSTNAME, USERNAME, PASSWORD,DATABASE, 0, NULL, 0)){
		error("Could not connect to host.",mysql);
	}
}

void dropTable(MYSQL *mysql){
	char query[MAX_QUERY];
	clrstr(query);
	strcat(query,"drop table htmls");
	if(mysql_query(mysql,query)){
		error("fail drop 1",mysql);
	}
}

void createTable(MYSQL *mysql){
	char query[MAX_QUERY];
	
	clrstr(query);
	strcat(query,"create table htmls (htmltext text,");
	strcat(query,"length int,");
	strcat(query,"name char(50),");
	strcat(query,"stored datetime,");
	strcat(query,"primary key(name) )");
	if(mysql_query(mysql,query)){
		error("Could not create table!",mysql);
	}
	clrstr(query);
}

void fillTable(MYSQL *mysql){
	char query[MAX_QUERY];
	char *records[] = {
		"insert into htmls values ('textsss1',8,'file1',now())",
		"insert into htmls values ('textsss1',2,'file2',now())",
		"insert into htmls values ('textsss1',2,'file3',now())"
	};
	clrstr(query);
	strcat(query,"create table htmls (htmltext text,");
	strcat(query,"length int,");
	strcat(query,"name char(50),");
	strcat(query,"stored datetime,");
	strcat(query,"primary key(name) )");

	mysql_query(mysql,query);
	clrstr(query);

	for(int x = 0; x < 3; x++){
		if(mysql_query(mysql, records[x])){
			printf("Failure to insert: %s\n",records[x]);
			error("Could not insert record",mysql);
		}
	}
}

void showTable(MYSQL *mysql){
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];
	strcpy(query, "select name, length, stored from htmls order by name");	
	if(mysql_query(mysql, query)){
		error("fail select 2",mysql);
	}
	if (!(res = mysql_store_result(mysql))){
		error("fail store 2",mysql);
	}
	printf("name length date\n");
	while ((row = mysql_fetch_row(res))) {
		for (int i=0; i < mysql_num_fields(res); i++){
			printf("%s ", row[i]);
		}
		printf("\n");
	}
	mysql_free_result(res);
}

void printTable (MYSQL *mysql){
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];
	strcpy(query, "select * from htmls order by name");	
	if(mysql_query(mysql, query)){
		error("fail select 2",mysql);
	}
	if (!(res = mysql_store_result(mysql))){
		error("fail store 2",mysql);
	}
	while ((row = mysql_fetch_row(res))) {
		for (int i=0; i < mysql_num_fields(res); i++){
			printf("%s ", row[i]);
		}
		printf("\n");
	}
	mysql_free_result(res);
}


char *strcpyseg (char*src, int start, int end){
    int length = end-start+1;
    int index;
    char *dest = malloc(sizeof(char)*(length+1));
    if (dest == NULL){
        return NULL;
    }
    index =0;
    for (int i = start; i <= end-1; i++){
        dest[index] = src[i];
        index++;
    }
    dest[index]='\0';
    return dest;
}

void uploadHTML(MYSQL *mysql, char *path){
	FILE *f = fopen(path,"r");
	char *name = strcpyseg(path,6,strlen(path)+1);//files/test.html
	char *text;// = malloc();
	char *query;
	int size;
	fseek(f, 0L, SEEK_END);
	size = ftell(f);
	rewind(f);
	
	text = malloc(sizeof(char)*(size+1));

	fread(text,sizeof(char),size,f);
	text[size]='\0';

	for(int i =0; i < strlen(name); i++){
		if (name[i]=='.'){
			name[i]='\0';
		}
	}

	query=malloc(sizeof(char)*(50+strlen(text)+strlen(name)));
	sprintf(query,"insert into htmls values ('%s',%d,'%s',now())",text,size,name);

	if(mysql_query(mysql, query)){
		printf("Failure to insert: %s\n",query);
		error("Could not insert record",mysql);
	}

	fclose(f);
	free(query);
	free(text);
	free(name);
}

void convert (char *str){
	return;
}

int main (int argc, char *argv[]){
	MYSQL mysql;
	if (argc == 2){
		if (!strcmp("-help",argv[1])){
			printf("USAGE:\n./db <flag>\n");
			printf("FLAGS:\n-clear\tremoves all rows from table in database\n");
			printf("-reset\tdeletes the table from the database\n");
			printf("-show\tview the name, length and date for files stored in the db\n");
//			printf("-create\tcreates a table named htmls\n");
//			printf("-fill\tfills the table with dummy rows\n");
//			printf("-print\tprints the mysql table\n");
//			printf("-store\tenter a path to upload file\n");
			printf("-help\tview these options again\n");
		} else if (!strcmp("-clear",argv[1])){
			init(&mysql);
			dropTable(&mysql);
			createTable(&mysql);
			mysql_close(&mysql);
		} else if (!strcmp("-reset",argv[1])){
			init(&mysql);
			dropTable(&mysql);
			mysql_close(&mysql);
		} else if (!strcmp("-show",argv[1])){
			init(&mysql);
			showTable(&mysql);
			mysql_close(&mysql);
		} else if (!strcmp("-create",argv[1])){
			init(&mysql);
			createTable(&mysql);
			mysql_close(&mysql);
			mysql_library_end();
		} else if (!strcmp("-fill",argv[1])){
			init(&mysql);
			fillTable(&mysql);
			mysql_close(&mysql);	
		} else if (!strcmp("-print",argv[1])){
			init(&mysql);
			printTable(&mysql);
			mysql_close(&mysql);
		} else {
			needHelp();
		}
	} else if (argc == 3){
		if (!strcmp("-store",argv[1])){
			init(&mysql);
			uploadHTML(&mysql,argv[2]);
			mysql_close(&mysql);
		} else if (!strcmp("-convert",argv[1])){
			convert(argv[2]);
		} else {
			needHelp();
		}
	} else {
		needHelp();
	}
	mysql_server_end();
	return 0;
}