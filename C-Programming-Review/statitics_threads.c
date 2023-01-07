
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


//Variables and methods that will be used by all the threads

int average_V, min_V, max_V;

void *averageRunner(void *avg, int argc);
void *minRunner(void* min);
void *maxRunner(void *max);


int main(int argc, char *argv[]){

    pthread_t tid; //thread id
    pthread_attr_t attr; //thread attribute

    //passing the thread attribute to the thread

    pthread_attr_init(&attr);

  
    pthread_create(&tid, &attr, averageRunner, argv, argc);

    pthread_join(tid,NULL);

    printf("The retuned average is %d\n", average_V);


}

void *averageRunner(void *avg){ 
   int *avg_array = avg;
   for(int j = 0; j < argc; j++){
    printf("%d\n",avg_array[j]);
   }
    int i, sum= 0;
    printf("The Size of the array is %d now \n", size);
for(i = 0; i < argc; i++){
   sum += avg_array[i];
   printf("%d ", avg_array[i]);
}
average_V = sum/size;

//close the pthread after you are done executing
pthread_exit(0);

}

void *minRunner(void *min){
    
}
void *maxRunner(void *max){

}