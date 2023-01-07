
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>


int main(){
int val;
printf("enter the number \n");
scanf("%d",&val );
int digit = 0;

do{
   val = val/10;
    digit++;
}while(val> 0);

printf("The number of digit are %d", digit);

}