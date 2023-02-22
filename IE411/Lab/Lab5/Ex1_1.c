// CPP Program to find sum of array
#include <stdio.h>
#include <pthread.h>

int part = 0; // to count part of array
int n; // size of array
int a[1000]={0}; // array
int sum[2] = {0}; // sum of parts for array


void* sum_array(void* arg){
    // Each thread computes sum of half part of array
    int thread_part = part++;
    int k;
    if(n%2==0){
        for (int i = thread_part * (n / 2); i < (thread_part + 1) * (n / 2); i++)
            sum[thread_part] += a[i];
    }else{
        if(thread_part==0){
            for (int i = thread_part * (n / 2); i < (thread_part + 1) * (n / 2); i++)
                sum[thread_part] += a[i];
        }else{
            for (int i = thread_part * (n / 2); i < (((thread_part + 1) * (n / 2))+1); i++)
                sum[thread_part] += a[i];
        }
        
    }
    
}

int main()
{
    // array input
    printf("Enter the size of array - ");
    scanf("%d",&n);
    printf("Enter the elements of array - ");
    for(int i=0; i<n; i++){
        scanf("%d",&a[i]);
    }

    pthread_t threads[2]; // 2 threads for array
  
    // Creating 2 threads for array
    for (int i = 0; i < 2; i++)
        pthread_create(&threads[i], NULL, (void *)sum_array, (void*)&a[i]);
  
    // waiting for all threads to complete
    for (int i = 0; i < 2; i++)
        pthread_join(threads[i], NULL);
  
    // adding sum of all 2 parts of array
    int total_sum = 0;
    for (int i = 0; i < 2; i++)
        total_sum += sum[i];
  
    printf("Sum of given array is %d \n", total_sum);
  
    return 0;
}