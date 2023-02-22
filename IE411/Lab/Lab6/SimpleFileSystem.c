#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "fileSystemAPIs.h"

#define fileNameLimit 100
#define maxDataBlocksPerFile 10
#define maxNumDataBlocks 1000
#define totalSizeofDisk 4096000
#define blockSizeLimit 4096
#define maxNumOfFiles 100

//Data Type declarations 
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
    int free[maxNumDataBlocks];  // Bitmap vector
    int numEmptyBlocks;
    int numFullBlocks;
};

struct directory{
    struct file files[maxNumOfFiles]; // File Allocation table
    int maxFileSize; // Maximum number of blocks for a file
    int maxNumFiles; // Maximum number of files
};

struct dataBlock{
	char data[blockSizeLimit];
};

// Global variables
FILE* disk;
struct superBlock sb;
struct directory dir;

void mainMenu(){
	printf("Main Menu:\n\n");
	printf("1. List of all files\n");
	printf("2. Create new file\n");
	printf("3. Read file\n");
	printf("4. Edit file\n");
	printf("5. Delete file\n");
	printf("-. Any number to exit\n\n");
	printf("Enter task number: ");
}

void mount(){

	printf("Trying to mount disk - CFS.data\n");
	printf("Format the disk or use it as it is - type 1 or 2: ");
	int format;
	scanf("%d", &format);
	
	if(format==1){
		disk= fopen("CFS.data", "w+");
		if(!disk){
			printf("Error in mounting\n");
			exit(1);
		}
		else
			printf("Mount successful\n\n");
		
		// Initialize superblock
		sb.totalSize= totalSizeofDisk; // Bytes
		sb.blockSize= blockSizeLimit; // Bytes
		sb.totalNumBlocks= sb.totalSize/sb.blockSize;
		
		for(int i=0; i<sb.totalNumBlocks; i++)
			sb.free[i]=0;
		
		sb.numEmptyBlocks= maxNumDataBlocks;
		sb.numFullBlocks= 0;
		
		// Initialize for directory
		dir.maxFileSize= maxDataBlocksPerFile;   
		dir.maxNumFiles= sb.totalNumBlocks/dir.maxFileSize;

		for(int i=0; i<dir.maxNumFiles; i++)
			dir.files[i].isCreated=0;
		
		// Writing the fresh super block and the directory block
		fwrite(&sb, sizeof(struct superBlock), 1, disk);
		fwrite(&dir, sizeof(struct directory), 1, disk);
		
	}
	else{
		// open disk for read and write in append mode
		disk= fopen("CFS.data", "a+");
		if(!disk){
			printf("Error in mounting\n");
			exit(1);
		}
		else
			printf("Mount successful\n\n");
			
		// Reading the superblock and directory block
		fread(&sb, sizeof(struct superBlock), 1, disk);
		fread(&dir, sizeof(struct directory), 1, disk);
	}
}

void listFiles(){
	if(sb.numFullBlocks==0)
		printf("There is no file on the disk.\n");
	else{
		printf("\n_____CFS.data_____\n\n");
		printf("No.\tName\tOccupied_blocks\n");
		for(int i=0; i<dir.maxNumFiles; i++){
			if(dir.files[i].isCreated)
				printf("%d\t  %s\t  %d\n", i+1, dir.files[i].fileName, dir.files[i].numBlocks);
		}
	}
	printf("\n");
}

void createFile(){

	// Finding an empty space for new file
	int fileIndex;
	for(fileIndex=0; fileIndex<dir.maxNumFiles; fileIndex++){
		if(!dir.files[fileIndex].isCreated)
			break;
	}
	
	// Getting file name of the new file
	printf("Enter the name of the file you want to create: ");
	char removeSpace;
	scanf("%c", &removeSpace);
	scanf("%s", dir.files[fileIndex].fileName);
	
	//Initializing the block list
	for(int i=0; i<10; i++)
		dir.files[fileIndex].dataBlocks[i]=-1;		
			
	printf("\n_____Enter data(type # when finished)_____\n\n");
	
	// Removing any unwanted characters in input
	scanf("%c", &removeSpace);
			
	char inputChar='s';
	int numBlocks=1;
	int blockIndex=0;
	
	while(inputChar!='#'){
	
		// Finding a free datablock
		for(; blockIndex<sb.totalNumBlocks; blockIndex++){
			if(!sb.free[blockIndex])
				break;
		}
		
		// Moving file pointer to the appropriate position
		fseek(disk, (blockIndex-(ftell(disk) - sizeof(struct superBlock) - sizeof(struct directory)))*sb.blockSize, SEEK_CUR);
		
		// Getting user input 
		struct dataBlock db;
		int i;
		for(i=0; i<sb.blockSize; i++){
			//Getting input from the user
			if( (inputChar=getchar())!='#' )
				db.data[i]=inputChar;
			else
				break;
		}
		if(inputChar=='#')
			db.data[i]='#';
		
		// The user entered # so this is an empty file with 0 blocks
		if(numBlocks==1 && strcmp(db.data, "#")==0)
			dir.files[fileIndex].numBlocks=0;
			
		// For non empty files
		else{
			// Writing the user input to the file
			fwrite(&db, sizeof(struct dataBlock), 1, disk);
			
			// Updating the bit map
			sb.free[blockIndex]=1;
			
			// Adding the location of this datablock 
			dir.files[fileIndex].dataBlocks[numBlocks-1]=blockIndex;
			blockIndex++;
			
			if(inputChar!='#')
				numBlocks++;
		}
	}
	
	// Updating the number of blocks 
	dir.files[fileIndex].numBlocks=numBlocks;
			
	// Moving file pointer to the beginning 
	fseek(disk, 0, SEEK_SET);
			
	// Updating the number of Full and Empty datablocks 
	sb.numEmptyBlocks-=dir.files[fileIndex].numBlocks;
	sb.numFullBlocks+=dir.files[fileIndex].numBlocks;
	
	//Writing the updates superblock back to the disk
	fwrite(&sb, sizeof(struct superBlock), 1, disk);
			
	// Updating the file table in directory
	dir.files[fileIndex].isCreated=1;
	
	// Writing the updated directory block back to the disk
	fwrite(&dir, sizeof(struct directory), 1, disk);
}

void readFile(){

	// Getting filename user wants to read
	printf("Enter the name of the file you want to read: ");
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
		printf("No file found\n");
				
	else{
		printf("\n_____%s_____\n\n", dir.files[fileIndex].fileName);
		for(int i=0; i<10; i++){
			
			// If nothing left to read, come out of the loop
			if(dir.files[fileIndex].dataBlocks[i]==-1)
				break;

			// Move the file pointer to this data block
			fseek(disk, (dir.files[fileIndex].dataBlocks[i]-(ftell(disk)- sizeof(struct superBlock) - sizeof(struct directory)))*sb.blockSize, SEEK_CUR);
			
			//Read the data in the data block
			struct dataBlock db;
			fread(&db, sizeof(struct dataBlock), 1, disk);
			
			// Print the read data character by character		
			for(int i=0; i<sb.blockSize && db.data[i]!='#'; i++)
				printf("%c", db.data[i]);
		}
		printf("\n");

		// Moving the file pointer back to the start
		fseek(disk, sizeof(struct superBlock)+sizeof(struct directory), SEEK_SET);
	}
}

void editFile(){
	// Getting filename of the file that the user wants to read
	printf("Enter the name of the file you want to edit: ");
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
		printf("No file found\n");
				
	else{
		printf("\n\n1. Edit the block\n-. Other value to remove block\n\n_____\n\n");
		int i;
		for(i=0; i<10; i++){
			if(dir.files[fileIndex].dataBlocks[i]==-1)
				break;
				
			// Moving the file pointer to data block
			fseek(disk, (dir.files[fileIndex].dataBlocks[i]-(ftell(disk)- sizeof(struct superBlock) - sizeof(struct directory)))*sb.blockSize, SEEK_CUR);
			
			//Reading the data in the data block
			struct dataBlock db;
			fread(&db, sizeof(struct dataBlock), 1, disk);
			
			// Print the read data character by character		
			for(int i=0; i<sb.blockSize && db.data[i]!='#'; i++)
				printf("%c", db.data[i]);
			printf("\n\n");
				
			printf("Enter task number: ");
			int input;
			scanf("%d", &input);
			
			// Edit block
			if(input==1){
				printf("\nEnter data for the block (# to stop):\n");
				char removeSpace;
				scanf("%c", &removeSpace);
				fseek(disk, dir.files[fileIndex].dataBlocks[i]*sb.blockSize+sizeof(struct superBlock)+sizeof(struct directory), SEEK_SET);
				char inputChar;
				struct dataBlock db;
				int j;
				for(j=0; j<sb.blockSize; j++){
					if((inputChar=getchar())=='#'|| inputChar=='*')
						break;
					db.data[j]=inputChar;
				}
				
				// The user left the block empty so there is not data for this block
				if(strcmp(db.data, "#")==0){
					sb.free[dir.files[fileIndex].dataBlocks[i]]=0;
					for(int j=i; dir.files[fileIndex].dataBlocks[j]!=-1; j++)
						dir.files[fileIndex].dataBlocks[j]=dir.files[fileIndex].dataBlocks[j+1];
					dir.files[fileIndex].numBlocks--;
					sb.numFullBlocks--;
					sb.numEmptyBlocks++;
				}
				// Or write the new data for this block
				else{
					if(inputChar=='#')
						db.data[j]='#';
					fwrite(&db, sizeof(struct dataBlock), 1, disk);
				}
			}
			
			// Deleting block
			else{
				// Marking this block as free
				sb.free[dir.files[fileIndex].dataBlocks[i]]=0;
				// Removing this block from the file's data block list
				for(int j=i; dir.files[fileIndex].dataBlocks[j]!=-1; j++)
					dir.files[fileIndex].dataBlocks[j]=dir.files[fileIndex].dataBlocks[j+1];
				// Decrementing the number of blocks 
				dir.files[fileIndex].numBlocks--;
				// Updating the number of Empty and Full blocks
				sb.numFullBlocks--;
				sb.numEmptyBlocks++;
			}
		}
		printf("\n");
		
		// Moving the file pointer back to the start 
		fseek(disk, sizeof(struct superBlock)+sizeof(struct directory), SEEK_SET);
	}
}

void deleteFile(){
	
	// Getting filename of the file that the user wants to delete
	printf("Enter the name of the file you want to delete: ");
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
		printf("No file found\n");
				
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
		printf("\nFile %s is deleted successfully\n", dir.files[fileIndex].fileName);
	}
}

void unMount(){
	if(disk!=NULL){
		fclose(disk);
		printf("\nUnmount successful!\n");
	}
}





int main(){
	// Mounting the disk 
	mount();
	char retToMainMenu;
	
	do{
		// Main menu
		mainMenu();
		int task;
		scanf("%d", &task);
		
		if(task==1)
			listFiles();
		else if(task==2)
			createFile();
		else if(task==3)
			readFile();
		else if(task==4)
			editFile();
		else if(task==5)
			deleteFile();
		else
			break;	
		printf("Return to main menu(y/n): ");
		scanf(" %c", &retToMainMenu);
		
	}while(retToMainMenu=='y');
	
	// Unmounting the disk
	unMount();
	return 0;		
}
