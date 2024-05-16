#include "includes.h"
#include <string.h>
#include <stdlib.h>


//文字列を逆順にする関数
void rev_char(char *str){
    //i: 初期文字の添字, j: 終端文字の添字
	int i,j;
	char temp='a';
	char list[MAX];
	strcpy(list, str);
	int size = strlen(list);
	
	for(i = 0,j = size - 2;i < size / 2; i++, j--){
		temp=list[i];
		list[i]=list[j];
		list[j]=temp;
	}
	strcpy(str, list);
}