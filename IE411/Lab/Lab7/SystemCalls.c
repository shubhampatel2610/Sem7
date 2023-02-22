#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "fileSystemAPIs.h"



// maximum of 100 files supports
#define maxNumOfFiles 100

// The max size of file is 10 KB 
#define maxDataBlocksPerFile 10

// The max length of file name is 100 characters
#define fileNameLimit 100

#define maxNumDataBlocks 1000

#define totalSizeofDisk 4096000

#define blockSizeLimit 4096



struct file{
    int isCreated;
    char fileName[fileNameLimit];
    int dataBlocks[maxDataBlocksPerFile];
    int numBlocks;
};



struct superBlock{
    int totalSize;
    int blockSize;
    int totalNumBlocks;
    int free[maxNumDataBlocks]; 
    int numEmptyBlocks;
    int numFullBlocks;
};



struct directory{
    struct file files[maxNumOfFiles]; // File Allocation table
    int maxFileSize; // Maximum number of blocks allowed to a file
    int maxNumFiles; // Maximum number of files allowed on the disk
};



struct dataBlock{
    char data[blockSizeLimit];
};



FILE* disk;
struct superBlock sb;
struct directory dir;



void mainMenu(){
    printf("Main Menu:\n\n");
    printf("1. Create file\n");
    printf("2. List files in the disk\n");
    printf("3. Delete a file\n");
    printf("-. Any other number to exit\n\n");
    printf("Enter choice: ");
}



void mount(){
    printf("Trying to mount disk named CFS.data\n");
    printf("Do you want to format the disk or use it as it is?(1 or 2): ");
    int format;
    scanf("%d", &format);
    if(format==1){
        disk= fopen("CFS.data", "w+");
    if(!disk){
        printf("Error mounting the disk! Exiting\n");
        exit(1);
    }else
        printf("successfully mount!\n\n");

    // superblock
    sb.totalSize= totalSizeofDisk; 
    sb.blockSize= blockSizeLimit; 
    sb.totalNumBlocks= sb.totalSize/sb.blockSize;

    for(int i=0; i<sb.totalNumBlocks; i++)
        sb.free[i]=0;
    sb.numEmptyBlocks= maxNumDataBlocks;
    sb.numFullBlocks= 0;

    // Initialization for directory
    dir.maxFileSize= maxDataBlocksPerFile; // This is 10 blocks or 10*4096 Bytes
    dir.maxNumFiles= sb.totalNumBlocks/dir.maxFileSize;

    for(int i=0; i<dir.maxNumFiles; i++)
        dir.files[i].isCreated=0;
    // Writing the fresh super block and the directory block on the disk

    fwrite(&sb, sizeof(struct superBlock), 1, disk);
    fwrite(&dir, sizeof(struct directory), 1, disk);
    }else{
    // opening the disk for read and write in append mode
    disk= fopen("CFS.data", "a+");
    if(!disk){
        printf("Error mounting the disk! Exiting\n");
        exit(1);
    }else
    printf("successfully mount!\n\n");
    // Reading the superblock and directory block from the disk
    fread(&sb, sizeof(struct superBlock), 1, disk);
    fread(&dir, sizeof(struct directory), 1, disk);
    }
}



// listFiles Function
void listFiles(){
    if(sb.numFullBlocks==0)
        printf("Disk is empty!\n");
    else{
        printf("\n----------------------------CFS.data--------------------------\n\n");
        printf("File No.\tFile Name\tNumber of Blocks Occupied\n");
    for(int i=0; i<dir.maxNumFiles; i++){
        if(dir.files[i].isCreated)
            printf("%d\t %s\t %d\n", i+1, dir.files[i].fileName, dir.files[i].numBlocks);
        }
    }
printf("\n");
}


// createFile Function
void createFile(){
    // Finding an empty space in file allocation table for the new filer
    int fileIndex;
    for(fileIndex=0; fileIndex<dir.maxNumFiles; fileIndex++){
        if(!dir.files[fileIndex].isCreated)
            break;
        }
    // Getting file name of the new file from the user
    printf("Enter the name of the file to create: ");
    char removeSpace;
    scanf("%c", &removeSpace);
    scanf("%s", dir.files[fileIndex].fileName);
    //Initializing the block list for the newly created file
    for(int i=0; i<10; i++)
        dir.files[fileIndex].dataBlocks[i]=-1;
    printf("\n---------Enter data (input $ when finished)------------\n\n");
    // Removing any unwanted characters in the input stream
    scanf("%c", &removeSpace);
    char inputChar='s';
    int numBlocks=1;
    int blockIndex=0;

    while(inputChar!='$'){
    // Finding a free datablock on disk
        for(; blockIndex<sb.totalNumBlocks; blockIndex++){
            if(!sb.free[blockIndex])
                break;
        }
    // Moving the file pointer to the appropriate position
    fseek(disk, (blockIndex-(ftell(disk) - sizeof(struct superBlock) -
    sizeof(struct directory)))*sb.blockSize, SEEK_CUR);
    // Getting user input for file data
    struct dataBlock db;
    int i;
    for(i=0; i<sb.blockSize; i++){
    //Getting input from the user
        if( (inputChar=getchar())!='$' )
            db.data[i]=inputChar;
        else
            break;
    }

    if(inputChar=='$')
        db.data[i]='$';

    // The only data user entered was # so this is an empty file with 0 blocks allocated to it

    if(numBlocks==1 && strcmp(db.data, "$")==0)
        dir.files[fileIndex].numBlocks=0;
    // For non empty files
    else{
    // Writing the user input to the file
        fwrite(&db, sizeof(struct dataBlock), 1, disk);
    // Updating the bit map in the super block
        sb.free[blockIndex]=1;
    // Adding the location of this datablock 
        dir.files[fileIndex].dataBlocks[numBlocks-1]=blockIndex;
        blockIndex++;
        if(inputChar!='$')
            numBlocks++;
    }
}

    // Updating the number of blocks
    dir.files[fileIndex].numBlocks=numBlocks;

    // Moving file pointer to the beginning of the disk
    fseek(disk, 0, SEEK_SET);

    // Updating the number of Full and Empty datablocks
    sb.numEmptyBlocks-=dir.files[fileIndex].numBlocks;
    sb.numFullBlocks+=dir.files[fileIndex].numBlocks;

    //Writing the updates superblock back to the disk
    fwrite(&sb, sizeof(struct superBlock), 1, disk);

    // Updating the file table in directory
    dir.files[fileIndex].isCreated=1;

    //Writing the updated directory block back to the disk
    fwrite(&dir, sizeof(struct directory), 1, disk);
}



// deleteFile function
void deleteFile(){
    // Getting filename of the file 
    printf("Enter the name of the file to delete: ");
    char removeSpace;
    scanf("%c", &removeSpace);
    char inputName[100];
    scanf("%s", inputName);
    // Looking for the required file 
    int fileIndex;

for(fileIndex=0; fileIndex<dir.maxNumFiles; fileIndex++){
    if(strcmp(dir.files[fileIndex].fileName, inputName)==0)
        break;
}

if(fileIndex==dir.maxNumFiles)
    printf("No such file present!\n");
else{
    for(int i=0; i<10; i++){
        if(dir.files[fileIndex].dataBlocks[i]!=-1)
            sb.free[dir.files[fileIndex].dataBlocks[i]]=0;
        else
            break;
}

    dir.files[fileIndex].isCreated=0;
    sb.numEmptyBlocks+=dir.files[fileIndex].numBlocks;
    sb.numFullBlocks-=dir.files[fileIndex].numBlocks;
    printf("\nFile %s is deleted\n", dir.files[fileIndex].fileName);
    }
}


// unMount Function
void unMount(){
    if(disk!=NULL){
        fclose(disk);
        printf("\nsuccessfully unmount!\n");
    }
}



int main(){
//mounting the disk
mount();
char retToMainMenu;

do{
    //Printing the main menu
    mainMenu();
    int choice;
    scanf("%d",&choice);

    if(choice==2)
        listFiles();
    else if(choice==1)
        createFile();
    else if(choice==3)
        deleteFile();
    else
        break;
    printf("Return to main menu?(y/n); ");
    scanf(" %c", &retToMainMenu);
}

while(retToMainMenu=='y');

//unmounting the disk
unMount();
return 0;
}