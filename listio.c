#include "listio.h"

struct returnStruct buildHeader(){
    struct dataHeader *header = malloc(sizeof(struct dataHeader));
    struct returnStruct rtrn;
    rtrn.value = SUCCESS;

    if (header == NULL){
        return returnFail(rtrn);
    }
    rtrn.header = header;
    header->name = malloc(sizeof(char));
    if (header->name == NULL){
        return returnFail(rtrn);
    }
    header->name[0]=' ';
    header->length = 0;
    header->next = NULL;
    return rtrn;
}


int addString (struct dataHeader *header, char *str){
    struct dataString *c;
    struct dataString *toAdd;
    int addlen = trulen (str);

    if (header == NULL){
        return 0;
    }
    c = header->next;
    toAdd = malloc(sizeof(struct dataString));
    if (toAdd == NULL){
        return FAILURE;
    }
    toAdd->string = malloc(sizeof(char)*addlen);
    if (toAdd->string == NULL){
        return FAILURE;
    }
    strcpy(toAdd->string,str);
    toAdd->next = NULL;
    if(c == NULL){
        header->next = toAdd;
    } else {
        while(c->next != NULL){
            c = c->next;
        }
        c->next = toAdd;
    }
    header->length +=addlen;
    return SUCCESS;
}


int freeStructure(struct dataHeader *header) {
    freeString(header->next);
    free(header->name);
    free(header);
    return SUCCESS;
}


void freeString(struct dataString *string){
    if(string == NULL){
        return;
    }
    if(string->next != NULL){
        freeString(string->next);
    }
    free(string->string);
    free(string);
}


int writeStrings(char *filename, struct dataHeader *header){
    FILE *fp = fopen(filename,"w");
    int wint;
    struct dataString *c = header->next;

    wint=trulen(header->name);
    fwrite (&wint,sizeof(int),1,fp);
    fwrite (header->name,sizeof(char),trulen(header->name),fp);
    wint=header->length;
    fwrite(&wint,sizeof(int),1,fp);
    while(c!=NULL){
        wint=trulen(c->string);
        fwrite(&wint,sizeof(int),1,fp);
        fwrite(c->string,sizeof(char),trulen(c->string),fp);
        c = c->next;
    }
    fclose(fp);
    return SUCCESS;
}


struct returnStruct readStrings(char *filename){
    struct returnStruct rtrn;
    FILE *fp;
    int wint;
    char *wchar = NULL;
    int fileLen =0;
    int strSumLen =0;

    if (!(fp= fopen(filename,"r"))){
        return returnFail(rtrn);
    } else {
        rtrn = buildHeader();
        if (fread(&wint,sizeof(int),1,fp) == 0){
            return returnFileFail(rtrn,fp);
        }
        wchar = malloc(sizeof(char)*(wint+1));
        if (wchar == NULL){
            return returnFileFail(rtrn,fp);
        }
        if (fread(wchar,sizeof(char),wint,fp) == 0){
            return returnReadFail(rtrn,fp,wchar);
        }
        wchar[wint]='\0';
        if (setName(rtrn.header,wchar)==FAILURE){
            return returnReadFail(rtrn,fp,wchar);
        }
        if (fread(&wint,sizeof(int),1,fp) == 0){
            return returnReadFail(rtrn,fp,wchar);
        }
        fileLen = wint;
        while(1){
            if (fread(&wint,sizeof(int),1,fp) == 0){
                if (strSumLen != fileLen){
                    return returnReadFail(rtrn,fp,wchar);
                }
                break;
            } else {
                strSumLen += wint;
                if (strSumLen > fileLen){
                    return returnReadFail(rtrn,fp,wchar);
                }
                wchar = realloc(wchar,sizeof(char)*(wint+1));
                if (wchar == NULL){
                    fclose(fp);
                    return returnFileFail(rtrn,fp);
                }
                if ((int)fread(wchar,sizeof(char),wint,fp) != wint){
                    return returnReadFail(rtrn,fp,wchar);
                } 
                wchar[wint]='\0';
                if (trulen(wchar) != wint){
                    return returnReadFail(rtrn,fp,wchar);
                } else {
                    addString(rtrn.header,wchar);
                }
            }
        }
        fclose(fp);
        free(wchar);
        rtrn.value = SUCCESS;
        return rtrn;
    }
}


int setName(struct dataHeader *header, char *name){
    header->name = realloc(header->name,sizeof(char)*(strlen(name)+1));
    if (header->name == NULL){
        return FAILURE;
    }
    strcpy(header->name,name);
    return SUCCESS;
}


char *getName (struct dataHeader *header){
    if (strcmp(header->name,"")==0){
        return NULL;
    }
    return header->name;
}


int getLength(struct dataHeader *header){
    return header->length;
}


int printString(struct dataHeader *header){
    struct dataString *c;
    printf("%s\n",header->name);
    printf("%d\n",header->length);
    c = header->next;

    if(c==NULL){
        //printf("No String Stored\n");
    } else {
        while (c!=NULL){
            printf("%s\n",c->string);
            c=c->next;
        }
    }
    return SUCCESS;
}


void processDuplicates(char *str, char dupe){
    char c = str[0];
    char p = ' ';
    int index;
    int len = trulen(str);

    for(int i=1; i<len-1; i++){
        p=c;
        c=str[i];
        if(c==dupe && p==dupe){
            index = i-1;
            for(int j = index; j<len-1; j++){
                str[j]=str[j+1];
            }
            i--;
            c=p;
            len = trulen(str);
        }
    }
}


int processTags(struct dataHeader *header, char *tag){
    struct dataString *c = header->next;
    int numStrings = countStrings(header);
    char tagStart = tag[0];
    int taglen = strlen(tag);
    char**strings = malloc(sizeof(char*)*numStrings);
    char *cat;
    int count = 0;
    bool activeTag = false;
    int deletePoint;

    if(strings == NULL){
        return FAILURE;
    }
    while(c!=NULL){
        strings[count] = malloc(sizeof(char)*trulen(c->string));
        if (strings[count] == NULL){
            return FAILURE;
        }
        strcpy(strings[count],c->string);
        c=c->next;
        count++;
    }
    for(int i = 0; i<numStrings; i++){
        if (activeTag){
            deletePoint = 0;
        }
        for (int j = 0; j<strlen(strings[i]); j++){
            if (strings[i][j] == tagStart && strlen(strings[i])-j>taglen-1){
                if (!strcmpseg(strings[i],j,j+(taglen-1),tag,0,taglen-1)){
                    if (activeTag){
                        cat = strcpyseg(strings[i],j+3,trulen(strings[i]));
                        if (cat == NULL){
                            return FAILURE;
                        }
                        strings[i] = realloc(strings[i],sizeof(char)*(1+j));
                        if (strings[i] == NULL){
                            return FAILURE;
                        }
                        strings[i][deletePoint]='\0';
                        strings[i] = realloc(strings[i],sizeof(char)
                            *(strlen(strings[i])+trulen(cat)));
                        if (strings[i]==NULL){
                            return FAILURE;
                        }
                        strcat(strings[i],cat);
                        j = deletePoint;
                        free(cat);
                    } else {
                        activeTag=true;
                        deletePoint = j+3;
                        j=j+2;
                    }
                }
            } else if (!isWhitespace(strings[i][j])){
                activeTag=false;
            }
        }
    }
    c=header->next;
    count=0;
    while(c!=NULL){
        c->string = realloc(c->string,sizeof(char)*trulen(strings[count]));
        strcpy(c->string,strings[count]);
        c=c->next;
        count++;
    }
    for(int i = 0; i<numStrings; i++){
        free(strings[i]);
    }
    free(strings);
    return SUCCESS;
}


int pipeToPython (struct dataHeader *header){
    char *pyArgs[2];
    char str[80];
    int buf;
    pid_t pid;// = fork();
    int numStrings = countStrings(header);
    struct dataString *c = header->next;
    FILE *fp;
    int strNum;
    bool dead = false;
    pyArgs[0]="python";
    pyArgs[1]="main.py";
    pyArgs[2]=0;

    //Opening the queue to python
    fp = fopen("./q1","w");
    if (fp == NULL){
        return FAILURE;
    }
    fprintf(fp, "%d\n",numStrings);
    fprintf(fp, "%s\n",getName(header));
    for(int i = 0; i < numStrings; i++){
        fprintf(fp, "%s\n",c->string);
        c=c->next;
    }
    fclose(fp);
    //creating a child process to execute python
    pid = fork();
    if (pid == 0){
        execvp(pyArgs[0],pyArgs);
    } else if (pid <1) {
        return FAILURE;
    }else {
        //deleting the contents header structure
        freeString(header->next);
        header->next = NULL;
        header->length = 0;
        //creating a queue for python to return data
        mkfifo("./q2",0777);
        fp = fopen("./q2","r");
        if (fp == NULL){
            return FAILURE;
        }
        fscanf(fp,"%d",&strNum);
        //sleeping for one second incase python is slow
        sleep(1);
        for (int i =0; i<strNum;i++){
            for (int j =0; j<79; j++)
            {
                buf = fgetc(fp);
                if (buf == '\n'){
                    str[j]='\0';
                    break;
                } else {
                    str[j]=buf;
                }
            }
            str[79]='\0';
            //adding the returned values to the header structure
            if (addString(header,str) == FAILURE){
                return FAILURE;
            }
        }
        fclose(fp);
        remove("./q2");
    }
    //Politely attemping to kill the python program process
    kill(pid,SIGTERM);
    for (int i=0; !dead && i<5; i++){
        int status;
        if(waitpid(pid,&status,WNOHANG)==pid){
            dead = true;
            break;
        }
        sleep(1);
    }
    //Rudely killing the python program process
    if(!dead){
        kill(pid,SIGKILL);
    }
    return SUCCESS;
}

int processStrings(struct dataHeader *header){
    struct dataString *c = header->next;
    char *buffer = malloc(sizeof(char)*trulen(c->string));
    
    if (buffer == NULL){
        return FAILURE;
    }
    strcpy(buffer,c->string);
    while (c!=NULL){
        processDuplicates(c->string,' ');
        processDuplicates(c->string,'\t');
        strcpy(buffer,c->string);
        buffer = processCRNLs(buffer);
        if (buffer == NULL){
            return FAILURE;
        }
        c->string = realloc(c->string,sizeof(char)*trulen(buffer));
        if (c->string == NULL){
            return FAILURE;
        }
        strcpy(c->string,buffer);
        free(buffer);
        c=c->next;
        if (c== NULL)
            break;
        buffer = malloc(sizeof(char)*trulen(c->string));
        if (buffer == NULL){
            return FAILURE;
        }
    }
    if (processTags(header,"<P>\0") == FAILURE){
        return FAILURE;
    }
    updateLength(header);
    return pipeToPython(header);
}


char *processCRNLs(char *str){
    char current = str[0];
    char previous = ' ';
    char *cat;
    int len = trulen(str);

    for(int i =1; i <len-1; i++){
        previous=current;
        current=str[i];
        /*Replaces multiple \r andor \n with <P>*/
        if (previous == '\r' || previous == '\n'){
            if (current == '\r' || current == '\n'){
                for(int j = i+1; j <len; j++){
                    current = str[j];
                    if (current != '\r' && current != '\n'){
                        break;
                    } else {
                        for(int k = j; k<len-1; k++){
                            str[k] = str[k+1];
                        }
                        j--;
                    }
                }
                cat = strcpyseg(str,i+1,len);
                if (cat == NULL){
                    return NULL;
                }
                str = realloc(str,sizeof(char)*(i+3));
                if (str == NULL){
                    return NULL;
                }
                str[i-1] = '<';
                str[i] = 'P';
                i++;
                str[i] = '>';
                i++;
                str[i] = '\0';
                str =realloc(str,sizeof(char)*(trulen(str)+trulen(cat)));
                if (str == NULL){
                    return NULL;
                }
                strcat(str,cat);
                free(cat);
                len = trulen(str);
                i--;
                current = str[i];
            /*Replaces single \r or \n with <BR>*/
            } else {
                cat = strcpyseg(str,i,len);
                if (str == NULL){
                    return NULL;
                }
                str = realloc(str,sizeof(char)*(i+4));
                if (str == NULL){
                    return NULL;
                }
                str[i-1] = '<';
                str[i] = 'B';   
                i++;
                str[i] = 'R';    
                i++;
                str[i] = '>';
                i++;
                str[i] = '\0';
                str = realloc(str,sizeof(char)*(trulen(str)+trulen(cat)));
                if (str == NULL){
                    return NULL;
                }
                strcat(str,cat);
                free(cat);
                len = trulen(str);
                i--;
            }
        }
    }
    return str;
}


int strcmpseg (char*str1, int s1, int e1, char*str2, int s2, int e2){
    int result;
    char *cmp1;
    char *cmp2;
    cmp1 = strcpyseg(str1,s1,e1);
    cmp2 = strcpyseg(str2,s2,e2);
    result = strcmp(cmp1,cmp2);
    free(cmp1);
    free(cmp2);
    return result;
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


int trulen(char *str){
    return (int)strlen(str)+1;
}


void updateLength (struct dataHeader *header){
   struct dataString *c = header->next;
   header->length =0;
   while(c!=NULL){
        header->length+= trulen(c->string);
        c=c->next;
   } 
}


bool isWhitespace (char c){
    char whitespace[4] = " \t\r\n";
    for (int i = 0; i<4; i++){
        if (c == whitespace[i]){
            return true;
        }
    }
    return false;
}


int countStrings(struct dataHeader *header){
    struct dataString *c = header->next;
    int count = 0;
    while(c!=NULL){
        count++;
        c=c->next;
    }
    return count;
}


struct returnStruct returnFail(struct returnStruct rtrn){
    rtrn.header = NULL;
    rtrn.value = FAILURE;
    return rtrn;
}


struct returnStruct returnFileFail(struct returnStruct rtrn, FILE *fp){
    freeStructure(rtrn.header);
    fclose(fp);
    return returnFail (rtrn);
}


struct returnStruct returnReadFail(struct returnStruct rtrn, FILE *fp, char *wp){
    free(wp);
    return returnFileFail(rtrn,fp);
}
