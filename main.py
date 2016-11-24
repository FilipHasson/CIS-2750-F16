#!/usr/bin/python
import os, sys

#open pipe
filename = "./q1"
pipe = open(filename,"r")
#get the number of strings and the title 
total = int(pipe.readline())
title = pipe.readline() [:-1]
strlist = []
#read from pipe
for i in range (total):
    line = pipe.readline() [:-1]
    strlist.append(line);
os.remove(filename)
tags = []
tagStr = []
#open the .info file
if os.path.isfile(title+".info") == True:
    info = open(title+".info","r")
    infodata = info.readlines()
    #parse the info file data
    for i in range (len(infodata)):
        tags.append(infodata[i][0])
        tagStr.append(infodata[i].rsplit("\r",1)[0])
        tagStr[i] = tagStr[i][2:]
    #Delete improper tags from .info file
    for i in range (len(tagStr) -1,-1,-1):
        if tagStr[i]=="":
            del tagStr[i]
            del tags[i]
    for i in range (len(tags) -1,-1,-1):
        if tags[i] != 'B' and tags[i] != 'I' and tags[i] != 'U':
            del tagStr[i]
            del tags[i]
#create html tags
title = "<TITLE>"+title
title = title+"</TITLE>"
htmlHead = ["<HTML>","<HEAD>",title,"</HEAD>","<BODY>"]
htmlBody = ""
htmlEnd = ["</BODY>","</HTML>"]
#store all data from pipe into 1 string
for i in range (len(strlist)):
    htmlBody = htmlBody+strlist[i]
#Handle tagging
findLength =0
tagStart =0
tagEnd =0
newTag = True
k=0
for i in range (len(tagStr)):
    findLength = 0
    k=0
    for j in range (len(htmlBody)):
        if htmlBody[k] == tagStr[i][findLength]:
            findLength = findLength+1
            if newTag == True:
                newTag = False
                tagStart = k
                tagEnd = tagStart+len(tagStr[i])
        elif htmlBody[k] == "<":
            if htmlBody[k+1]== "B" or htmlBody[k+1]== "I" or htmlBody[k+1]== "U":
                if htmlBody[k+2] == ">":
                    k = k+2
                    tagEnd = tagEnd+3
                else:
                    newTag = 1
                    findLength = 0
            elif htmlBody[k+1]=="/":
                if htmlBody[k+2]=="B" or htmlBody[k+2]=="I" or htmlBody[k+2]=="U":
                    if htmlBody[k+3] == ">":
                        k = k+3
                        tagEnd = tagEnd+4
                    else:
                        newTag = 1
                        findLength = 0
                else:
                    newTag = 1
                    findLength = 0  
            else:
                newTag = 1
                findLength = 0
        else:
            newTag = 1
            findLength = 0
        if findLength == len(tagStr[i]):
            if htmlBody[k+1].isalpha() == False:
                temp = htmlBody[:tagStart]
                temp = temp+"<"+tags[i]+">"
                temp = temp+htmlBody[tagStart:tagEnd]
                temp = temp+"</"+tags[i]+">"
                temp = temp+htmlBody[tagEnd:]
                k= k+7
                htmlBody = temp
                findLength = 0
            else:
                newTag = 1
                findLength = 0
        k = k+1
        if k >=len(htmlBody):
            break
htmlBodyList = []
current = 0
while len(htmlBody)-current > 78:
    htmlBodyList.append(htmlBody[current:current+78]);
    current = current+78
htmlBodyList.append(htmlBody[current:]);
htmlList = htmlHead+htmlBodyList+htmlEnd
filename2 = "./q2"
pipe2 = os.open(filename2, os.O_WRONLY)
wrlen =""
wrlen = wrlen+str(len(htmlList))
os.write(pipe2,wrlen)
for i in range (len(htmlList)):
   os.write(pipe2,htmlList[i]+"\n")
os.close(pipe2)
