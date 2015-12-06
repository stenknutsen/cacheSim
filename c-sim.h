#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//
//
//
//PROTOTYPE LIST
//
//
//
char * hexstring_to_binary(char hexstring[]);
unsigned int hexstring_to_decimal(char hexstring[]);
unsigned int binary_to_decimal(char binaryString[]);
char strip_RW_char(char rawInput[]);
char * strip_hexstring(char rawInput[]);
int get_setIndex(char binaryString[], int blockBits, int setBits);
unsigned int get_tag(char binaryString[], int blockBits, int setBits);
unsigned int** create_direct_cache(int setNum);
void print_results(int totalRead, int totalWrite, int totalHit, int totalMiss);
unsigned int** create_fully_assoc_cache(int setSize);
int find_empty_line(unsigned int **cache, int setSize);
int find_line_to_evict(unsigned int **cache, int setSize);
unsigned int** create_deerect_cache(int setNum, int blockSize);
int get_blockNum(char binaryString[], int blockBits);
int strip_n(char str[]);
struct hash* create_new_hash(int setNum);
int search_cache(struct hash *cache, int setIndex, unsigned int tag);
//
//
//
//END PROTOTYPE LIST


char * hexstring_to_binary(char hexstring[])
{
   unsigned int n = (int)strtol(hexstring, NULL, 0);
   int c, d, count;
   char *pointer;
 
   count = 0;
   pointer = (char*)malloc(32+1);
 
   if ( pointer == NULL )
      exit(EXIT_FAILURE);
 
   for ( c = 31 ; c >= 0 ; c-- )
   {
      d = n >> c;
 
      if ( d & 1 )
         *(pointer+count) = 1 + '0';
      else
         *(pointer+count) = 0 + '0';
 
      count++;
   }
   *(pointer+count) = '\0';
 
   return  pointer;
}


unsigned int hexstring_to_decimal(char hexstring[]){

	unsigned int number = (int)strtol(hexstring, NULL, 0);

	return number;
}


unsigned int binary_to_decimal(char binaryString[]){
	int i, n, binDigit;
	unsigned int total = 0;
	n = 0;
	for(i=strlen(binaryString)-1;i>=0;i--){
		binDigit = 0;
		if(binaryString[i]=='1'){
			binDigit = 1;
		}
		//note: pow is evidenlty double, so cast to unsigned long
		total = total + (binDigit*((unsigned int)(pow(2, n))));
		//printf("charat %d was %c, n is %d and total is %u\n",i,binaryString[i], n, total);
		n++;
	}

	return total;
}


char strip_RW_char(char rawInput[]){
	char rwchar;
	int i =0;
	while(rawInput[i]!=' '){
		i++;
	}

	i++;
	rwchar = rawInput[i];

	return rwchar;
}

char * strip_hexstring(char rawInput[]){
	char *pointer;
	pointer = (char*)malloc(32+1);
	int i= 0;
	while(rawInput[i]!=' '){
		i++;
	}
	i = i+3;
	int count = 0;


	while(rawInput[i]!='\n'){

		*(pointer+count) = rawInput[i];
		i++;
		count++;
	}
	

	*(pointer+count) = '\0';

	return pointer;

}

int get_setIndex(char binaryString[], int blockBits, int setBits){
	char *pointer;
	int index;
	pointer = (char*)malloc(setBits+1);
	int i, count=0;

	for(i=strlen(binaryString) - blockBits - setBits;i<strlen(binaryString)- blockBits;i++){
		*(pointer+count) = binaryString[i];
		count++;
	}
	*(pointer+count) = '\0';
	index = binary_to_decimal(pointer);
	return index;
}
//
///
//// 
unsigned int get_tag(char binaryString[], int blockBits, int setBits){
	char *pointer;
	unsigned int tag;
	pointer = (char*)malloc(strlen(binaryString) - blockBits - setBits);
	int i;

	for(i=0;i<strlen(binaryString) - blockBits - setBits;i++){
		*(pointer+i) = binaryString[i];
		
	}
	
	*(pointer+i) = '\0';
	tag = binary_to_decimal(pointer);
	return tag;//change to tag . . . .. 
}
//
///
////
unsigned int** create_direct_cache(int setNum){
	unsigned int** cache;
	int i;
	cache = (unsigned int**)malloc(setNum*sizeof(unsigned int*));
	for(i=0;i<setNum;i++){
		cache[i] = (unsigned int*)malloc(2*sizeof(unsigned int));
	}

	for(i=0;i<setNum;i++){
		cache[i][0]=0;
		cache[i][1]=0;
	}
	//print for testing purposes only. . . . 
	//for(i=0;i<setNum;i++){
	//	printf("%d :  [%u] [%u] \n",i, cache[i][0],cache[i][1]);
		
	//}

	return cache;
}
//
///
////
void print_results(int totalRead, int totalWrite, int totalHit, int totalMiss){
	printf("Memory reads: %d\n",totalRead );
	printf("Memory writes: %d\n", totalWrite );
	printf("Cache hits: %d\n", totalHit );
	printf("Cache misses: %d\n", totalMiss );
}
//
///
////
unsigned int** create_fully_assoc_cache(int setSize){
	unsigned int** cache;
	int i;
	cache = (unsigned int**)malloc(setSize*sizeof(unsigned int*));
	for(i=0;i<setSize;i++){
		cache[i] = (unsigned int*)malloc(2*sizeof(unsigned int));
	}

	for(i=0;i<setSize;i++){
		cache[i][0]=0;
		cache[i][1]=0;
	}
	return cache;
}//end create_fully_assoc_cache
//
///
////
int find_empty_line(unsigned int **cache, int setSize){
	int i;
	int foundLine = -1;
	for(i=0;i<setSize;i++){
		if(cache[i][0] == 0){
			foundLine = i;
			return foundLine;
		}
	}


	return foundLine;
}
//
///
////
int find_line_to_evict(unsigned int **cache, int setSize){
	int i;
	int evictedLine = 0;
	for(i=0;i<setSize;i++){
		
		if(cache[i][0]>evictedLine){
			
			evictedLine = i;
			
		}

	}
		return evictedLine;

}
//
///
////
unsigned int** create_deerect_cache(int setNum, int blockSize){
	unsigned int** cache;
	int i, j;
	cache = (unsigned int**)malloc(setNum*sizeof(unsigned int*));
	for(i=0;i<setNum;i++){
		cache[i] = (unsigned int*)malloc((2+blockSize)*sizeof(unsigned int));
	}

	for(i=0;i<setNum;i++){
		for(j=0;j<(2+blockSize);j++){
			cache[i][j]=0;
		}
	}
	

	return cache;

}//end create deerect
//
///
////
int get_blockNum(char binaryString[], int blockBits){
	char *pointer;
	pointer = (char*)malloc(blockBits+1);
	int blockNum, i, count=0;

	for(i=strlen(binaryString) - blockBits;i<strlen(binaryString);i++){
		*(pointer+count) = binaryString[i];
		count++;

	}
	*(pointer+count) = '\0';
	blockNum = binary_to_decimal(pointer);


return blockNum;
}//end get_blocknum


int strip_n(char str[]){
	char *pointer;
	int returnValue;
	int i, count = 0;
	//int beginIndex = 6;
	pointer = (char*)malloc((strlen(str) - 6)+1);

	for(i=6;i<strlen(str);i++){
		*(pointer+count) = str[i];
		count++;
	}
	*(pointer+count) = '\0';

	returnValue = atoi(pointer);
	//atoi the char . . 

	return returnValue;
}

////////////////
////////////////
////////////////
struct node{
	unsigned int data;
	struct node *next;
};

struct hash{
	struct node *head;
	int valid;
};

struct node * create_node(unsigned int num){
	struct node *ptr;
	ptr = (struct node *) malloc(sizeof(struct node));
	if(ptr == NULL){
		return(struct node *) NULL;
	}else{
		ptr->data = num;
		ptr->next = NULL;
		return ptr;
	}
}//end create_node

struct hash* create_new_hash(int setNum){
	struct hash *hashTable;
	hashTable = (struct hash*)calloc(setNum, sizeof(struct hash));
	int i;
	for(i=0;i<setNum;i++){
		hashTable[i].head = NULL;
		//hashTable[i].valid = 0;
	}

	return hashTable;
}
//
//
int search_cache(struct hash *cache, int setIndex, unsigned int tag){
	if(cache[setIndex].valid == 0){
		return 0;
	}

	struct node *ptr;//create ptr
	ptr = cache[setIndex].head;//point ptr at indexed head
	while(ptr!=NULL){
		if(ptr->data == tag){
			return 1;
		}
		ptr = ptr->next;//advance ptr
	}
	
	return 0;
}
//
//












