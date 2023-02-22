// CPP Program to find sum of array
#include <stdio.h>
#include <pthread.h>

int part1 = 0, part2 = 0; // to count part of array
int n,m; // size of two arrays
int a[1000], b[1000]; // two arrays
int sum1[2] = {0}; // sum of parts for first array
int sum2[2] = {0}; // sum of parts for second array


void* sum_array1(void* arg){ 
    // Each thread computes sum of half part of array1
    int thread_part = part1++;
    int k;
    if(n%2==0){
        for (int i = thread_part * (n / 2); i < (thread_part + 1) * (n / 2); i++)
            sum1[thread_part] += a[i];
    }else{
        if(thread_part==0){
            for (int i = thread_part * (n / 2); i < (thread_part + 1) * (n / 2); i++)
                sum1[thread_part] += a[i];
        }else{
            for (int i = thread_part * (n / 2); i < (((thread_part + 1) * (n / 2))+1); i++)
                sum1[thread_part] += a[i];
        }
        
    }
}

void* sum_array2(void* arg){ 
    // Each thread computes sum of half part of array2
    int thread_part = part2++;
    int k;
    if(m%2==0){
        for (int i = thread_part * (m / 2); i < (thread_part + 1) * (m / 2); i++)
            sum2[thread_part] += b[i];
    }else{
        if(thread_part==0){
            for (int i = thread_part * (m / 2); i < (thread_part + 1) * (m / 2); i++)
                sum2[thread_part] += b[i];
        }else{
            for (int i = thread_part * (m / 2); i < (((thread_part + 1) * (m / 2))+1); i++)
                sum2[thread_part] += b[i];
        }
    }       
}

int main(){
    // first array input
    printf("Enter the size of array1 - ");
    scanf("%d",&n);
    printf("Enter the elements of array1 - ");
    for(int i=0; i<n; i++){
        scanf("%d",&a[i]);
    }

    //second array input
    printf("Enter the size of array2 - ");
    scanf("%d",&m);
    printf("Enter the elements of array2 - ");
    for(int i=0; i<m; i++){
        scanf("%d",&b[i]);
    }

    // 2 threads for first array and 2 threads for second array
    pthread_t threads[4];
  
    // Creating 2 threads for array 1
    for (int i = 0; i < 2; i++)
        pthread_create(&threads[i], NULL, sum_array1, (void*)NULL);
    
    // Creating 2 threads for array 2
    for (int i = 2; i < 4; i++)
        pthread_create(&threads[i], NULL, sum_array2, (void*)NULL);
  
    // waiting for all threads to complete
    for (int i = 0; i < 4; i++)
        pthread_join(threads[i], NULL);
  
    // adding sum of all 2 parts of first array 
    int total_sum1 = 0;
    for (int i = 0; i < 2; i++)
        total_sum1 += sum1[i];
    printf("Sum of given array1 is %d \n", total_sum1);

    // adding sum of all 2 parts of second array
    int total_sum2 = 0;
    for (int i = 0; i < 2; i++)
        total_sum2 += sum2[i];
    printf("Sum of given array2 is %d \n", total_sum2);
  
    return 0;
}