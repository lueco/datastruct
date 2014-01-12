/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2014 Lueco <lueco@lueco-Ubuntu>
 * 
 * docfind is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * docfind is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 100000
typedef struct relink{
	int doc;
	struct relink *next;
}relink;
char **cz(int sz){
	return (char **)malloc(sizeof(char*)*sz);
}

relink *sz(int sz){
	return (relink *)malloc(sizeof(relink)*sz); 
}
int insert(relink *node,int i){
	while(node->next!=NULL){
		if(i==node->next->doc)
			return 1;
		node=node->next;
	}
	node->next=(relink *)malloc(sizeof(relink));
	node->next->next=NULL;
	node->next->doc=i;
	return 0;
}
int cmp(char* str1,char* str2){
	int len1=strlen(str1);
	if (str2==NULL)
		return 0;
	int len2=strlen(str2);
	if (len1==len2){
		while (len1>=0){
			if (*(str1+len1)!=*(str2+len1))
				return 0;
			--len1;
		}
		return 1;
	}else
		return 0;
}

long BKDRHash(char* str,char** word){
	long seed = 31;
	long hash = 0;
	int i=0;
	int len=strlen(str)-1;
	for(i = 0; i<len; ++i){
		hash = (hash * seed) + *(str+i);
		hash=hash%MAX;
	}
	while (cmp(str,word[hash])==0){
		if (word[hash]==NULL)
			break;
		hash++;
		hash=hash%MAX;
	}
	return hash;
}

void create(char** word,relink* ct){
	FILE *read;
	int i=0;
	char text[24];
	char Doc[]="Document";
	char *tmp=NULL;
	read = fopen("./Document.txt","r");
	while(fscanf(read,"%s",text)&&(!feof(read))){
		if(cmp(text,Doc)!=0){
			i++;
			fscanf(read,"%s",text);
			continue;
		}
		long hash=BKDRHash(text,word);
		if (cmp(text,word[hash])==0){
			tmp=(char *)malloc(sizeof(char)*strlen(text));
			strcpy(tmp,text);
			word[hash]=tmp;
		}
		insert(ct+hash,i);
	}
	fclose(read);
}

void deal(char** word,relink* ct){

	FILE *fp1;
	FILE *fp2;
	long hash;
	char text[24];
	fp1 = fopen("./WordInDocument.txt","r");
	fp2 = fopen("./WordInDocument_Result.txt","w");
	relink *tmp=ct;
	int i=1;
	while(fscanf(fp1,"%s",text)&&(!feof(fp1)))
	{
		hash=BKDRHash(text,word);
		fprintf(fp2, "CASE %d:\n",i);
		tmp=ct+hash;
		tmp=tmp->next;
		if ((tmp!=NULL)&&(tmp->doc!=0))
			while(tmp!=NULL){
				fprintf(fp2, "%d ",tmp->doc);
				tmp=tmp->next;
			}
		fprintf(fp2, "\n");
		i++;
	}
	fclose(fp1);
	fclose(fp2);
}

int main(){
	char** word=cz(MAX);
	relink*  ct=sz(MAX);
	int i=0;
	for (i=0;i<MAX;++i){
		word[i]=NULL;
		ct[i].next=NULL;
		ct[i].doc=0;
	}
	create(word,ct);
	deal(word,ct);
	return 0;
}
