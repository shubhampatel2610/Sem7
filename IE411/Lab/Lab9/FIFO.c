//FIFO

#include <stdio.h> 
int main(){ 
    int incomingStream[]={1, 2, 3, 4, 2, 1, 5, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6}; 
    int pageFaults=0; 
    int frames=4; 
    int m, n, s, pages; 

    pages=sizeof(incomingStream)/sizeof(incomingStream[0]); 
    printf("Incoming \t \t Frame 1 \t \t Frame 2 \t \t Frame 3 \t \t Frame4"); 
    int temp[frames]; 

    for(m=0; m<frames; m++){ 
        temp[m]=-1; 
    } 

    for (m=0; m<pages; m++){ 
        s=0; 
        for(n=0; n<frames; n++){ 
            if(incomingStream[m]==temp[n]){ 
                s++; 
                pageFaults--; 
            } 
        } 
        pageFaults++;
        if((pageFaults<=frames)&&(s==0)){ 
            temp[m]=incomingStream[m]; 
        }else if(s==0){ 
            temp[(pageFaults-1)%frames]=incomingStream[m]; 
        } 

        printf("\n"); 
        printf("%d\t\t\t", incomingStream[m]); 

        for(n=0; n<frames; n++){ 
            if (temp[n]!=-1){
                printf("%d\t\t\t", temp[n]); 
            }else{
                printf(" - \t\t\t"); 
            }
        } 
    } 
    
    printf("\nTotal Page Faults:\t%d\n", pageFaults); 

return 0; 
}
