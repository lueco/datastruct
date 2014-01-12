/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2014 Lueco <lueco.x@gmail.com>
 * 
 * HASH is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * HASH is distributed in the hope that it will be useful, but
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
typedef struct countdef{
	int pv;
	int zhi;
}count;z
void HeapAdjust(count*  array, int i, int nLength)
{
    int nChild;
    count nTemp;
    for (nTemp = array[i]; 2 * i + 1 < nLength; i = nChild)
    {
        // 子结点的位置=2*（父结点位置）+ 1
        nChild = 2 * i + 1;
        // 得到子结点中较大的结点
        if ( nChild < nLength-1 && array[nChild + 1].pv < array[nChild].pv)
            ++nChild;
        // 如果较大的子结点大于父结点那么把较大的子结点往上移动，替换它的父结点
        if (nTemp.pv > array[nChild].pv)
        {
            array[i] = array[nChild];
            array[nChild]= nTemp;
        }
        else
        // 否则退出循环
            break;
    }
}
 
// 堆排序算法
void HeapSort(count* array,int length)
{ 
    int i;
	count tmp;
    // 调整序列的前半部分元素，调整完之后第一个元素是序列的最大的元素
    //length/2-1是第一个非叶节点，此处"/"为整除
    for (i = length -1/ 2 ; i >= 0; --i)
        HeapAdjust(array, i, length);
    // 从最后一个元素开始对序列进行调整，不断的缩小调整的范围直到第一个元素
    for (i = length - 1; i > 0; --i)
    {
        // 把第一个元素和当前的最后一个元素交换，
        // 保证当前的最后一个位置的元素都是在现在的这个序列之中最大的
      ///  Swap(&array[0], &array[i]);
          tmp = array[i];
          array[i] = array[0];
          array[0] = tmp;
        // 不断缩小调整heap的范围，每一次调整完毕保证第一个元素是当前序列的最大值
        HeapAdjust(array, 0, i);
    }
}
char **cz(int sz){
	return (char **)malloc(sizeof(char*)*sz);
}

count *sz(int sz){
	return (count *)malloc(sizeof(count)*sz); 
}

int cmp(char* str1,char* str2){
	int len1=strlen(str1);
	if (str2==NULL)
		return 0;
	int len2=strlen(str2);
	if (len1==len2){
		while (len1!=0){
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

void create(char** word,count* ct){
	FILE *read;
	char text[24];
	read = fopen("./vocabulary.txt","r");
	while(fgets(text,24,read)!=NULL){
		long hash=BKDRHash(text,word);
		if (cmp(text,word[hash])==0){
			char *tmp=(char *)malloc(sizeof(char)*strlen(text));
			strcpy(tmp,text);
			word[hash]=tmp;
			ct[hash].zhi=hash;
		}
		++(ct[hash].pv);
	}
	fclose(read);
}

void deal(char** word,count* ct){

	FILE *fp1;
	FILE *fp2;
	long hash;
	char text[1024];
	fp1 = fopen("./SearchWordInVocabulary.txt","r");
	fp2 = fopen("./SearchWordInVocabulary_Result.txt","w");
	int i=1;
	while(fgets(text,1024,fp1)!=NULL)
	{
		hash=BKDRHash(text,word);
		hash=ct[hash].pv;
		if (hash!=0)
			fprintf(fp2, "CASE %d:\n%ld\n",i,hash);
		else
			fprintf(fp2, "CASE %d:\nNO\n",i);
		i++;
	}
	fclose(fp1);
	fclose(fp2);
}

int partion(count* ct,int p,int r){ 
    int x = ct[r].pv;
    int middle = p;
	int j = 0;
    for (j = p ; j < r ; j++){ 
        if (ct[j].pv < x){ 
            count temp  = ct[middle]; 
            ct[middle] = ct[j]; 
            ct[j] = temp; 
            middle++; 
        } 
    } 
    count temp = ct[r]; 
    ct[r]     = ct[middle]; 
    ct[middle] = temp; 
    return middle; 
} 

void sortput(char** word,count* ct){
	
	FILE *fp2;
	long hash;
	fp2 = fopen("./MostFrequenceWord.txt","w");
	int i=0;
	while(i<10)
	{
		hash=ct[i].zhi;
		word[hash][strlen(word[hash])-2]='\0';
		fprintf(fp2,"%s %d\n",word[hash],ct[i].pv);
		i++;
	}
	fclose(fp2);
}
int main(){
	char** word=cz(MAX);
	count*  ct=sz(MAX);
	int i=0;
	for (i=0;i<MAX;++i){
		word[i]=NULL;
		ct[i].pv=0;
	}
	create(word,ct);
	deal(word,ct);
	HeapSort(ct,MAX);
	sortput(word,ct);
	return 0;
}

