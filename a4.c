#include "listio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mysql/mysql.h>

#define MAX_QUERY 512
#define HOSTNAME "dursley.socs.uoguelph.ca"
#define USERNAME "fhasson"
#define PASSWORD "0878596"
#define DATABASE "fhasson"

int file_exist(char*path){
    struct stat buffer;
    return (stat(path,&buffer) == 0);
}

void error (char *msg, MYSQL *mysql){
    printf("%s\n%s\n",msg,mysql_error(mysql));
    exit(1);
}

void clrstr(char *buf){
    buf[0] = '\0';
}

void needHelp (){
    printf("Improper usage './a4 -help' for options\n");
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

void createTableNoErr(MYSQL *mysql){
    char query[MAX_QUERY];

    clrstr(query);
    strcat(query,"create table htmls (htmltext text,");
    strcat(query,"length int,");
    strcat(query,"name char(50),");
    strcat(query,"stored datetime,");
    strcat(query,"primary key(name) )");

    mysql_query(mysql,query);
    clrstr(query);
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


void writeHTML (struct dataHeader *header){
    struct dataString *c;
    FILE *fp;
    char *title = malloc(sizeof(char)*(strlen(header->name)+6));

    if (title == NULL){
        return;
    }
    strcpy(title,header->name);
    strcat(title,".html");
    fp = fopen(title,"w+");
    if (fp == NULL){
        return;
    }

    c = header->next;

    if(c==NULL){
        //printf("No String Stored\n");
    } else {
        while (c!=NULL){
            fprintf(fp,"%s",c->string);
            c=c->next;
        }
    }
    fclose(fp);
    free(title);
    return;
}

void uploadHTML(MYSQL *mysql, char *path){
    FILE *f = fopen(path,"r");
    char *name = strcpyseg(path,6,strlen(path)+1);//files/test.html
    char *text;// = malloc();
    char *query;
    char query2[60000];
    //char *cleanText;
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

    //cleanText = sanitizeApos(text);
    query=malloc(sizeof(char)*(50+strlen(text)+strlen(name)));
    mysql_real_escape_string(mysql,query2,text,strlen(text));
    sprintf(query,"insert into htmls values ('%s',%d,'%s',now())",query2,size,name);

    if(mysql_query(mysql, query)){
        printf("Failure to insert: %s\n",query);
        error("Could not insert record",mysql);
    }

    fclose(f);
    free(query);
    free(text);
    //free(cleanText);
    free(name);
}

void printHTML (char *pathHtml){
    int c;
    char one = ' ';
    char two = ' ';
    char three = ' ';
    FILE *fp = fopen(pathHtml,"r");
    if(fp){
        while((c = getc(fp)) != EOF){
            putchar(c);
            one = two;
            two = three;
            three = c;
            if (one == '<' && two == 'P' && three == '>'){
                putchar('\n');
            }
        }
        fclose(fp);
    }
    putchar('\n');
}

int convert (char*path){
    struct returnStruct catch;
    struct dataHeader *header;
    FILE *fp;
    int size = 80;
    int c;
    int nc = 0;
    char *pathHtml=malloc(sizeof(char)*(strlen(path)+6));
    char *buffer = malloc(sizeof(char)*size);

    catch = buildHeader();
    header = catch.header;
    strcpy(pathHtml,path);
    strcat(pathHtml,".html\0");
    if (!file_exist(pathHtml)){
        if(!file_exist(path)){
            printf("This file does not exist\n");
            return FAILURE;
        }
        setName(header,path);
        fp = fopen(path,"r");
        while(1){
            c = fgetc(fp);
            if(c == EOF){
                buffer[nc]='\0';
                addString(header,buffer);
                break;
            }
            if(nc == size-2){
                buffer[nc] = c;
                buffer[nc+1] = '\0';
                addString(header,buffer);
                nc = 0;
            } else {
                buffer[nc] = c;
                nc++;
            }
        }
        fclose(fp);
        processStrings(header);
        writeHTML(header);
    }
    //printHTML(pathHtml);
    free(buffer);
    free(pathHtml);
    freeStructure(header);
    return SUCCESS;
}

void printFiles(MYSQL *mysql){
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[MAX_QUERY];
    FILE *fp = fopen ("files.tmp","w+");
    strcpy(query, "select name from htmls order by name"); 
    if(mysql_query(mysql, query)){
        error("fail select 2",mysql);
    }
    if (!(res = mysql_store_result(mysql))){
        error("fail store 2",mysql);
    }
    while ((row = mysql_fetch_row(res))) {
        for (int i=0; i < mysql_num_fields(res); i++){
            fprintf(fp,"%s ", row[i]);
        }
        fprintf(fp,"\n");
    }

    fclose(fp);
    mysql_free_result(res);
}

void fetchHtml (MYSQL *mysql, char *path){
    MYSQL_ROW row;
    MYSQL_RES *res;
    char query[MAX_QUERY];
    FILE *fp = fopen("html.tmp","w+");
    strcpy(query,"select name, htmltext from htmls order by name");
    if(mysql_query(mysql, query)){
        error("fail select 2",mysql);
    }
    if (!(res = mysql_store_result(mysql))){
        error("fail store 2",mysql);
    }

    while ((row = mysql_fetch_row(res))) {
        if (!strcmp (row[0],path)){
            fprintf(fp,"%s",row[1]);
        }
    }

    fclose(fp);     
    mysql_free_result(res);
}

int main (int argc, char *argv[]){
    MYSQL mysql;
    char *fullpath;
    if (argc == 2){
        if (!strcmp("-help",argv[1])){
//            printf("USAGE:\n./a4 <flag>\n");
//            printf("FLAGS:\n-clear\tremoves all rows from table in database\n");
//            printf("-reset\tdeletes the table from the database\n");
//            printf("-show\tview the name, length and date for files stored in the db\n");
//            printf("-create\tcreates a table named htmls\n");
//            printf("-print\tprints the mysql table\n");
//            printf("-store\tenter a path to upload file\n");
//            printf("-convert\tenter a path to convertable file\n");
//            printf("-fetch\tfetch the file\n");
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
        } else if (!strcmp("-print",argv[1])){
            init(&mysql);
            printTable(&mysql);
            mysql_close(&mysql);
        } else if (!strcmp("-files",argv[1])){
            init(&mysql);
            printFiles(&mysql);
            mysql_close(&mysql);
        } else {
            needHelp();
        }
    } else if (argc == 3){
        if (!strcmp("-store",argv[1])){
            convert(argv[2]);
            fullpath = malloc(sizeof(char)*(strlen(argv[2])+6));
            strcpy(fullpath,argv[2]);
            strcat(fullpath,".html\0");
            init(&mysql);
            createTableNoErr(&mysql);
            uploadHTML(&mysql,fullpath);
            mysql_close(&mysql);
            remove(fullpath);
            free(fullpath);
        } else if (!strcmp("-convert",argv[1])){
            convert(argv[2]);
        } else if (!strcmp("-fetch",argv[1])) {
            init(&mysql);
            fetchHtml(&mysql,argv[2]);
            mysql_close(&mysql);
        } else {
            needHelp();
        }
    } else {
        needHelp();
    }
    mysql_server_end();
    return 0;
}