/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2014 Lueco <lueco@lueco-Ubuntu>
 * 
 * Trie is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Trie is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 26
typedef struct Trie{
    struct Trie *next[MAX];
    long v;   //根据需要变化
	char *word;
}Trie;
Trie *root;
typedef struct SortNode{
    int v;   //根据需要变化
	char word[24];
}SortNode;
void createTrie(char *str){
	int len = strlen(str)-1;
	int i=0,j=0;
    Trie *p = root, *q;
    for(i=0; i<len; ++i){
        int id = str[i]-97;
        if(p->next[id] == NULL){
            q = (Trie *)malloc(sizeof(Trie));
            q->v = 0;    //初始v==1
            for(j=0; j<MAX; ++j)
                q->next[j] = NULL;
			q->word=NULL;
            p->next[id] = q;
            p = p->next[id];
		}
		else
			p = p->next[id];
    }
    p->v++;   //若为结尾，则将v改成-1表示
	p->word=(char *)malloc(sizeof(str));
	strcpy(p->word,str);
}
int findTrie(char *str)
{
    int len = strlen(str)-1;
    Trie *p = root;
	int i=0;
    for(i=0; i<len; ++i)
    {
        int id = str[i]-'0';
        p = p->next[id];
        if(p == NULL)   //若为空集，表示不存以此为前缀的串
            return 0;
        if(p->v != 0)   //字符集中已有串是此串的前缀
            return 1;
    }
    return -1;   //此串是字符集中某串的前缀
}
void create(){
	FILE *read;
	char text[24];
 	read = fopen("./vocabulary.txt","r");
 	while(fgets(text,24,read)!=NULL){
			createTrie(text);
	}
	fclose(read);
}
void printr(FILE *fp2,Trie *p){
	int i=0;
	if(p->word!=NULL)
		fprintf(fp2, "%s",p->word);
	for(i=0;i<MAX;++i){
		if(p->next[i]!=NULL)
			printr(fp2,p->next[i]);
	}
}
void deal(){
	int i=1,j=9;
	FILE *fp1;
	FILE *fp2;
	char text[24];
	Trie *p=root;
	fp1 = fopen("./TotPrefixWord.txt","r");
	fp2 = fopen("./TotPrefixWord_Result.txt","w");
	while(fgets(text,24,fp1)!=NULL)
	{
		fprintf(fp2, "CASE %d:\n",i);
		p=root;
		for(j=0;j<strlen(text)-2;++j){
			if(p==NULL)
				break;
			p=p->next[text[j]-97];
		}
		if(p!=NULL)
			printr(fp2,p);
		++i;
	}
	fclose(fp1);
	fclose(fp2);
}
void SortN(Trie *p,SortNode sn[]){
	int i=0,j=0,k=0;
	SortNode tmp;
	for(i=0;i<MAX;++i){
		if(p->next[i]!=NULL){
			for(j=0;j<10;++j){
				if(p->next[i]->v>sn[j].v){
					for(k=8;k>=j;--k){
						tmp.v=sn[k].v;
						strcpy(tmp.word,sn[k].word);
						sn[k].v=sn[k+1].v;
						strcpy(sn[k].word,sn[k+1].word);
						sn[k+1].v=tmp.v;
						strcpy(sn[k+1].word,tmp.word);
					}
					sn[j].v=p->next[i]->v;
					strcpy(sn[j].word,p->next[i]->word);
					sn[j].word[strlen(sn[j].word)-1]='\0';
					break;
				}
			}
			SortN (p->next[i],sn);
		}
	}
}
void Sort(){
	int i=1,j=0;
	FILE *fp1;
	FILE *fp2;
	char text[24];
	Trie *p=root;
	fp1 = fopen("./PrefixFrequence.txt","r");
	fp2 = fopen("./PrefixFrequence_Result.txt","w");
	SortNode sn[10];
	while(fgets(text,24,fp1)!=NULL)
	{
		for(j=0;j<10;++j){
			sn[j].word[0]='\0';
			sn[j].v=0;
		}
		fprintf(fp2, "CASE %d:\n",i);
		p=root;
		for(j=0;j<strlen(text)-2;++j){
			if(p==NULL)
				break;
			p=p->next[text[j]-97];
		}
		if(p!=NULL){
			if(p->v!=0){
				sn[0].v=p->v;
				strcpy(sn[0].word,p->word);
				sn[0].word[strlen(sn[0].word)-1]='\0';
			}
			SortN(p,sn);
		}
		for(j=0;j<10;++j){
			if(sn[j].v==0)
				break;
			fprintf(fp2, "%s %d\n",sn[j].word,sn[j].v);
		}
		++i;
	}
	fclose(fp1);
	fclose(fp2);
}
int main(){
	int i=0;
	Trie wordtree;
	for(i=0;i<MAX;++i)
		wordtree.next[i]=NULL;
	wordtree.v=1;
	root = &wordtree;
	create();
	deal();
	Sort();
	return 0;
}

