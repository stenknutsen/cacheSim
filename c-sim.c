#include "c-sim.h"
#define FILE_LINE_SIZE 150
//
//
//
//
int main(int argc, char* argv[]){
	
	FILE *fp;
	char file_line[FILE_LINE_SIZE];


	//cache size is always argument 1. . . . 
	int cacheSize = atoi(argv[1]);
	//printf("Cache size is %d\n", cacheSize);


	int setNum = 0;
	int blockSize = 0;
	int setSize = 0;
	

	int blockBits = 0;
	int setBits = 0;

	char read_or_write;
	//char hexstring[50]="";
	int totalMiss = 0;
	int totalHit = 0;
	int totalRead = 0;
	int totalWrite = 0;


	//begin direct cache
	///
	/////
	//////
	///////
	if(strcmp(argv[2],"direct")==0){
		//printf("The associativity is direct\n" );
		//since direct, setSize is 1
		setSize = 1;
		//
		blockSize = atoi(argv[3]);
		//printf("Block size is %d\n", blockSize);
		//
		//calculate number of sets
		setNum = cacheSize / (setSize*blockSize);
		//printf("Number of sets is %d\n",setNum);
		//
		//calculate blockBits
		blockBits = log(blockSize)/log(2);
		//printf("blockBits is %d\n", blockBits);
		//
		//calculate setBits
		setBits = log(setNum)/log(2);
		//printf("setBits is %d\n", setBits);
		//
		//
		//printf("File to open is %s\n",argv[argc-1]);

		//open file (file is always last argument)
		fp = fopen(argv[argc-1], "r");


		//create direct cache
		unsigned int** cache = create_direct_cache(setNum);
		
		//read file and perform operations
		//
		//
		while(fgets(file_line, FILE_LINE_SIZE, fp)!=NULL){
			//if line does not contain hex value, continue
			if(file_line[0]!='0'){
				continue;
			}

			read_or_write = strip_RW_char(file_line);
			//printf("%s and the RWCHAR is %c\n", file_line, read_or_write);
			
			//get binary string
			char * binaryString = hexstring_to_binary(strip_hexstring(file_line));
			
			int setIndex  = get_setIndex(binaryString, blockBits, setBits);
			//printf("and the decimal setIndex is %d\n", setIndex );
			unsigned int tag = get_tag(binaryString, blockBits, setBits);
			//printf("Your tag is: %u\n", tag );

			if(read_or_write == 'R'){
				if(cache[setIndex][0]==0){
					totalMiss++;
					totalRead++;
					cache[setIndex][0] = 1;
					cache[setIndex][1] = tag;
					continue;
				}

				if(cache[setIndex][1]==tag){
					totalHit++;
					continue;
				}else{
					totalMiss++;
					totalRead++;
					cache[setIndex][0] = 1;
					cache[setIndex][1] = tag;
					continue;
				}
			}
			if(read_or_write == 'W'){
				if(cache[setIndex][0]==0){
					totalMiss++;
					totalRead++;
					totalWrite++;
					cache[setIndex][0] = 1;
					cache[setIndex][1] = tag;
					continue;
				}
				if(cache[setIndex][1]==tag){
					totalHit++;
					totalWrite++;
					continue;
				}else{
					totalMiss++;
					totalRead++;
					totalWrite++;
					cache[setIndex][0] = 1;
					cache[setIndex][1] = tag;
					continue;
				}
			}


		}//end of reading file and performing operations

		print_results(totalRead,totalWrite,totalHit,totalMiss);
		return 0;
	}//end of direct cache

	//begin fully associative cache
	///
	////
	/////
	//////
	///////
	////////
	/////////
	//////////
	///////////
	if(strcmp(argv[2],"assoc")==0){
		//printf("The associativity is fully assoc\n" );
		//since fully assoc, number of sets =1
		setNum = 1;
		//
		//get blockSize
		blockSize = atoi(argv[3]);
		//printf("Block size is %d\n", blockSize);
		//
		//calculate setSize
		setSize = cacheSize / (setNum*blockSize);
		//printf("Set size is %d\n", setSize);
		//
		//calculate block bits
		blockBits = log(blockSize)/log(2);
		//printf("blockBits is %d\n", blockBits);
		//
		//calculate setBits -- which should always be 0
		setBits = log(setNum)/log(2);
		//printf("setBits is %d\n", setBits);
		//
		//
		//printf("File to open is %s\n",argv[argc-1]);

		//open file (file is always last argument)
		fp = fopen(argv[argc-1], "r");

		//this cache will be a form of hash table
		//the hash structure will automatically keep track of
		//FIFO
		struct hash *cache=NULL;

		cache = create_new_hash(setNum);

		//start reading file and performing operations
		while(fgets(file_line, FILE_LINE_SIZE, fp)!=NULL){
			//
			if(file_line[0]!='0'){
				continue;
			}
			//
			//get whether R or W
			read_or_write = strip_RW_char(file_line);
			//
			//get binary string
			char * binaryString = hexstring_to_binary(strip_hexstring(file_line));
			//
			//get index
			int setIndex  = get_setIndex(binaryString, blockBits, setBits);
			//printf("and the decimal setIndex is %d\n", setIndex );
			//
			//get tag
			unsigned int tag = get_tag(binaryString, blockBits, setBits);
			//printf("Your tag is: %u\n", tag );

			if(search_cache(cache, setIndex, tag) == 1){
				totalHit++;
				if(read_or_write=='W'){
						totalWrite++;
					}
				continue;
			}
			//
			//if not full, insert
			if(cache[setIndex].valid < setSize){
				totalMiss++;
				totalRead++;
				if(read_or_write=='W'){
						totalWrite++;
					}
				struct node *newnode = create_node(tag);
				newnode->next = cache[setIndex].head;
				cache[setIndex].head = newnode;
				cache[setIndex].valid++;
				continue;

			}else{
				int i;
				totalMiss++;
				totalRead++;
				if(read_or_write=='W'){
						totalWrite++;
					}
				//delete last item . . .. 
				struct node *ptr;//create ptr
				ptr = cache[setIndex].head;//point ptr at indexed head
				for(i=1;i<setSize-1;i++){
					
					ptr = ptr->next;//advance ptr
				}
				ptr->next = NULL;
				//now insert
				struct node *newnode = create_node(tag);
				newnode->next = cache[setIndex].head;
				cache[setIndex].head = newnode;


			}//end else


		}//end reading file and performing operations

		

print_results(totalRead,totalWrite,totalHit,totalMiss);

return 0;




	}//end of assoc cache
	//
	//
	//
	//
	//And when all else fails . . . . . 
	//. . . . begin assoc:
	////
	//////
	////////
	//////////
	////////////
	//////////////
	////////////////
	//printf("The associativity is fully %s\n",  argv[2]);

	setSize = strip_n(argv[2]);
	//printf("The setSize is %d\n",setSize );
	//
	//
	blockSize = atoi(argv[3]);
	//printf("Block size is %d\n", blockSize);
	//
	//calculate number of sets
		setNum = cacheSize / (setSize*blockSize);
		//printf("Number of sets is %d\n",setNum);
	
	//
	//calculate blockBits
		blockBits = log(blockSize)/log(2);
		//printf("blockBits is %d\n", blockBits);
		//
		//calculate setBits
		setBits = log(setNum)/log(2);
		//printf("setBits is %d\n", setBits);
		//
		//
		//printf("File to open is %s\n",argv[argc-1]);

		//open file (file is always last argument)
		fp = fopen(argv[argc-1], "r");

		//this cache will be a form of hash table
		//the hash structure will automatically keep track of
		//FIFO
		struct hash *cache=NULL;

		cache = create_new_hash(setNum);

		//start reading file and performing operations
		while(fgets(file_line, FILE_LINE_SIZE, fp)!=NULL){
			//
			if(file_line[0]!='0'){
				continue;
			}
			//
			//get whether R or W
			read_or_write = strip_RW_char(file_line);
			//
			//get binary string
			char * binaryString = hexstring_to_binary(strip_hexstring(file_line));
			//
			//get index
			int setIndex  = get_setIndex(binaryString, blockBits, setBits);
			//printf("and the decimal setIndex is %d\n", setIndex );
			//
			//get tag
			unsigned int tag = get_tag(binaryString, blockBits, setBits);
			//printf("Your tag is: %u\n", tag );

			if(search_cache(cache, setIndex, tag) == 1){
				totalHit++;
				if(read_or_write=='W'){
						totalWrite++;
					}
				continue;
			}
			//
			//if not full, insert
			if(cache[setIndex].valid < setSize){
				totalMiss++;
				totalRead++;
				if(read_or_write=='W'){
						totalWrite++;
					}
				struct node *newnode = create_node(tag);
				newnode->next = cache[setIndex].head;
				cache[setIndex].head = newnode;
				cache[setIndex].valid++;
				continue;

			}else{
				int i;
				totalMiss++;
				totalRead++;
				if(read_or_write=='W'){
						totalWrite++;
					}
				//delete last item . . .. 
				struct node *ptr;//create ptr
				ptr = cache[setIndex].head;//point ptr at indexed head
				for(i=1;i<setSize-1;i++){
					
					ptr = ptr->next;//advance ptr
				}
				ptr->next = NULL;
				//now insert
				struct node *newnode = create_node(tag);
				newnode->next = cache[setIndex].head;
				cache[setIndex].head = newnode;


			}//end else


		}//end reading file and performing operations

		

print_results(totalRead,totalWrite,totalHit,totalMiss);

return 0;














/*
                      .M
                                 .:AMMO:
                        .:AMMMMMHIIIHMMM.
             ....   .AMMMMMMMMMMMHHHMHHMMMML:AMF"
             .:MMMMMLAMMMMMMMHMMMMMMHHIHHIIIHMMMML.
                 "WMMMMMMMMMMMMMMMMMMH:::::HMMMMMMHII:.
            .AMMMMMMMHHHMMMMMMMMMMHHHHHMMMMMMMMMAMMMHHHHL.
          .MMMMMMMMMMHHMMMMMMMMHHHHMMMMMMMMMMMMMHTWMHHHHHML
         .MMMMMMMMMMMMMMMMMMMHHHHHHHHHMHMMHHHHIII:::HMHHHHMM.
         .MMMMMMMMMMMMMMMMMMMMMMHHHHHHMHHHHHHIIIIIIIIHMHHHHHM.
         MMMMMMMMMMMMMMMMMHHMMHHHHHIIIHHH::IIHHII:::::IHHHHHHHL
         "MMMMMMMMMMMMMMMMHIIIHMMMMHHIIHHLI::IIHHHHIIIHHHHHHHHML
         .MMMMMMMMMMMMMM"WMMMHHHMMMMMMMMMMMLHHHMMMMMMHHHHHHHHHHH
        .MMMMMMMMMMMWWMW""YYHMMMMMMMMMMMMF""HMMMMMMMMMHHHHHHHH.
        .MMMMMMMMMM W" V                         W"WMMMMMHHHHHHHHHH
       "MMMMMMMMMM".                                 "WHHHMH"HHHHHHL
       MMMMMMMMMMF  .                                         IHHHHH.
       MMMMMMMMMM .                                  .        HHHHHHH
       MMMMMMMMMF. .                               .  .       HHHHHHH.
       MMMMMMMMM .     ,AWMMMMML.              ..    .  .     HHHHHHH.
     :MMMMMMMMM".  .  F"'    'WM:.         ,::HMMA, .  .      HHHHMMM
     :MMMMMMMMF.  . ."         WH..      AMM"'     "  .  .    HHHMMMM
      MMMMMMMM . .     ,;AAAHHWL"..     .:'                   HHHHHHH
      MMMMMMM:. . .   -MK"OTO L :I..    ...:HMA-.             "HHHHHH
 ,:IIIILTMMMMI::.      L,,,,.  ::I..    .. K"OTO"ML           'HHHHHH
 LHT::LIIIIMMI::. .      '""'.IHH:..    .. :.,,,,           '  HMMMH: HLI'
 ILTT::"IIITMII::.  .         .IIII.     . '""""             ' MMMFT:::.
 HML:::WMIINMHI:::.. .          .:I.     .   . .  .        '  .M"'.....I.
 "HWHINWI:.'.HHII::..          .HHI     .II.    .  .      . . :M.',, ..I:
  "MLI"ML': :HHII::...        MMHHL     :::::  . :..      .'.'.'HHTML.II:
   "MMLIHHWL:IHHII::....:I:" :MHHWHI:...:W,,"  '':::.      ..'  ":.HH:II:
     "MMMHITIIHHH:::::IWF"    """T99"'  '""    '.':II:..'.'..'  I'.HHIHI'
       YMMHII:IHHHH:::IT..     . .   ...  . .    ''THHI::.'.' .;H.""."H"
         HHII:MHHI"::IWWL     . .     .    .  .     HH"HHHIIHHH":HWWM"
          """ MMHI::HY""ML,          ...     . ..  :"  :HIIIIIILTMH"
               MMHI:.'    'HL,,,,,,,,..,,,......,:" . ''::HH "HWW
               'MMH:..   . 'MMML,: """MM""""MMM"      .'.IH'"MH"
                "MMHL..   .. "MMMMMML,MM,HMMMF    .   .IHM"
                  "MMHHL    .. "MMMMMMMMMMMM"  . .  '.IHF'
                    'MMMML    .. "MMMMMMMM"  .     .'HMF
                     HHHMML.                    .'MMF"
                    IHHHHHMML.               .'HMF"
                    HHHHHHITMML.           .'IF..
                    "HHHHHHIITML,.       ..:F...
                     'HHHHHHHHHMMWWWWWW::"......
                       HHHHHHHMMMMMMF"'........
                        HHHHHHHHHH............
                          HHHHHHHH...........
                           HHHHIII..........
                            HHIII..........
                             HII.........
                              "H........
                                ...... 
*/








	}//end main method