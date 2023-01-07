#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum; //shared data by threads
void *runner(void *param); //threads call this funtion

int main(int argc, char*argv[]){

    pthread_t tid ; //thread identifier
    pthread_attr_t attr; // set of threads attributes

    //set the attribute of the thread
    pthread_attr_init(&attr);

    //create the thread by providing its runner
    pthread_create(&tid,&attr,runner,argv[1]);

    //wait for the thread to exit

    pthread_join(tid,NULL);

    printf("sum from out thread is = %d\n",sum);
}

//function that the thread will run in that is the thread will execute here

void *runner(void *param){
    int i , upper = atoi(param);
    sum = 0;

    for ( i = 0; i <= upper; i++)
    {
       sum += i;
    
    }
    //exiting the thread after execution
    pthread_exit(0);
    
}