#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <algorithm>

using namespace std;

const int biggest = 1024;

void DirectMap(char * file, int size){

    int hit = 0;
    int total = 0;
    int index;
    int tag;
    //initialize array
    int directMap[size];
    for(int i = 0; i < 1; i++){
      directMap[i] = -1;
    }

    //open file
  	ifstream infile(file);
    string ldSt;
    int address;
    int logOf = log2(size);

  	// The following loop will read a line at a time
  while(infile >> ldSt >> std::hex >> address) {
      address = address >> 5;
      index = address % size;
      tag = address >> logOf;

      if(directMap[index] != tag){ //may have to change this to "tag"
        directMap[index] = tag; //may have to change this to "tag"
      }

      else hit++;

      total++;
   }
   cout<<hit<<",";
  cout<<total<<"; ";

}

void SetAssociative(char * file, int way){
    int hit = 0;
    int total = 0;
    int setIndex;
    int tag;
    int sets = 512/way;
    int setAssociative[sets][way];
    int recency[sets][way];

    for(int i = 0; i < sets; i++){
      for(int j = 0; j < way; j++){
        setAssociative[i][j] = -1;
        recency[i][j] = 0;
      }
    }

    //open file
    ifstream infile(file);
    string ldSt;
    int address;
    int logOf = log2(sets);

    // The following loop will read a line at a time
    while(infile >> ldSt >> std::hex >> address) {
      address = address >> 5;
      setIndex = address % (sets);
      tag = address >> logOf;
      bool isThere = 0;
      int newPlace;
      int oldRecency;


      for(int i = 0; i < way; i++){
        //check if there
         if(setAssociative[setIndex][i] == tag){
           hit++;
	   oldRecency = recency[setIndex][i];
           recency[setIndex][i] = way-1;
           isThere = 1;
           newPlace = i;
	   break;
        }
      }

      // update recency
      if(isThere){
        for(int i = 0; i < way; i++){
          if(i != newPlace){
          
            if(recency[setIndex][i] > 0 && recency[setIndex][i] > oldRecency)  recency[setIndex][i]--;
        }
       }
      }

      if(!isThere){
         bool replace = 1;
         for(int i = 0; i < way; i++){
           //check if room
           if(setAssociative[setIndex][i] == -1){
              recency[setIndex][i]= way-1;
              setAssociative[setIndex][i] = tag;
              replace = 0;
              newPlace = i;
              break;
           }
         }

         //update recency
         if(!replace){
           for(int i = 0; i < way; i++){
             if(i == newPlace) continue;
             else{
               if(recency[setIndex][i] > 0)  recency[setIndex][i]--;
             }
           }
         }

         if(replace){
         //find replacement
          int leastR = way;
          for(int i= 0; i < way; i++){
            if(recency[setIndex][i] < leastR){
              leastR = recency[setIndex][i];
              newPlace = i;
           }
          }

          //set replacement tag and recency. uses first least recenct slot
          for(int i = 0; i < way; i++){
            if(i == newPlace){
              setAssociative[setIndex][i] =tag;
              recency[setIndex][i] =way -1;
            }

            else{
              if(recency[setIndex][i] > 0)  recency[setIndex][i]--;
            }

          }
        }
     }
     total++;
   }
    cout<<hit<<",";
    cout<<total<<"; ";
}

void HnC_fullAssociative(char * file, int way){
	
	int cache[way];
	int hNc[511];
	
	
	
	for (int j=0; j<way; j++){	
		cache[j] = -1;
	}
	
	
	for (int i=0; i <511; i++){
		hNc[i] = 0;
	}

	int hits = 0;
	int total = 0;
	string ldstr;
	int address;
	int tag;

	ifstream inFile(file);
	while (inFile >> ldstr >> hex >> address){
         	address = address>>5;
		tag = address;
		int isThere = 0;

		for (int i=0; i<way; i++){
			if (cache[i] == tag){
		                int hc_index;
				hc_index = 255 + i/2;
				hits++;
				int start;
				if(i%2 == 1) start =1;
				else start = 0;
				while( hc_index >=  0){
					int temp = hc_index;
					hNc[hc_index] =  start;
					if (hc_index%2 == 1){
						start= 1;
						hc_index -=2;
					}
					else{
						hc_index--;
						start = 0;
					}
					hc_index /= 2;
					if(temp == 0) break;
				 }
				
				isThere = 1;
				break;
			
			}
		}
			
		if (isThere == 0){
			int coldest = 0;
			while(coldest*2	+2 <= 512){
				if (hNc[coldest] == 0){
					hNc[coldest] = 1;
					coldest = (coldest*2)+2;
				}
				else{
					hNc[coldest] = 0;
					coldest =( coldest*2) +1;
				}
			}
			cache[coldest] = tag;
	         	}
		
		total++;
	}
	cout << hits << "," << total;
}


void SetAssociativeWriteMiss(char * file, int way){
    int hit = 0;
    int total = 0;
    int setIndex;
    int tag;
    int sets = 512/way;
    int setAssociative[sets][way];
    int recency[sets][way];

    for(int i = 0; i < sets; i++){
      for(int j = 0; j < way; j++){
        setAssociative[i][j] = -1;
        recency[i][j] = 0;
      }
    }

    //open file
    ifstream infile(file);
    string ldSt;
    int address;
    int logOf = log2(sets);

    // The following loop will read a line at a time
    while(infile >> ldSt >> std::hex >> address) {
      address = address >> 5;
      setIndex = address % (sets);
      tag = address >> logOf;
      bool isThere = 0;
      int newPlace;
      int oldRecency;


      for(int i = 0; i < way; i++){
        //check if there
         if(setAssociative[setIndex][i] == tag){
           hit++;
	   oldRecency = recency[setIndex][i];
           recency[setIndex][i] = way-1;
           isThere = 1;
           newPlace = i;
	   break;
        }
      }

      // update recency
      if(isThere){
        for(int i = 0; i < way; i++){
          if(i != newPlace){
          
            if(recency[setIndex][i] > 0 && recency[setIndex][i] > oldRecency)  recency[setIndex][i]--;
        }
       }
      }

      if(!isThere && ldSt == "L" ){
         bool replace = 1;
         for(int i = 0; i < way; i++){
           //check if room
           if(setAssociative[setIndex][i] == -1){
              recency[setIndex][i]= way-1;
              setAssociative[setIndex][i] = tag;
              replace = 0;
              newPlace = i;
              break;
           }
         }

         //update recency
         if(!replace){
           for(int i = 0; i < way; i++){
             if(i == newPlace) continue;
             else{
               if(recency[setIndex][i] > 0)  recency[setIndex][i]--;
             }
           }
         }

         if(replace){
         //find replacement
          int leastR = way;
          for(int i= 0; i < way; i++){
            if(recency[setIndex][i] < leastR){
              leastR = recency[setIndex][i];
              newPlace = i;
           }
          }

          //set replacement tag and recency. uses first least recenct slot
          for(int i = 0; i < way; i++){
            if(i == newPlace){
              setAssociative[setIndex][i] =tag;
              recency[setIndex][i] =way -1;
            }

            else{
              if(recency[setIndex][i] > 0)  recency[setIndex][i]--;
            }

          }
        }
     }
     total++;
   }
    cout<<hit<<",";
    cout<<total<<"; ";
}

//had to add this because code for the following two functions became too long to debug
int LRU(int index, int way, int hit_index, int recency[][biggest]){
	//if theres a hit the index of the hit is passed
	if (hit_index != -1){
		int past = recency[index][hit_index];
                
			recency[index][hit_index] = way-1;
			for (int i=0; i<way; i++){
				if (i != hit_index && recency[index][i] < past && recency[index][i] > 0){
					(recency[index][i])--;
				}	
			}
			
		return -1;
	}
	//otherwise its a miss
	else{
          int newPlace;
	  bool replace = 1;
          for(int i = 0; i < way; i++){
           //check if room
           if(recency[index][i] == -1){
              recency[index][i]= way-1;
              replace = 0;
              newPlace = i;
             
           }
         }

         //update recency
         if(!replace){
           for(int i = 0; i < way; i++){
             if(i == newPlace) continue;
             else{
               if(recency[index][i] > 0)  recency[index][i]--;
             }
           }
	   return newPlace;
         }

         if(replace){
         //find replacement
          int leastR = way;
          for(int i= 0; i < way; i++){
            if(recency[index][i] < leastR){
              leastR = recency[index][i];
              newPlace = i;
           }
          }

          //set replacement tag and recency. uses first least recenct slot
          for(int i = 0; i < way; i++){
            if(i == newPlace){
              recency[index][i] =way -1;
            }

            else{
              if(recency[index][i] > 0)  recency[index][i]--;
            }

          }
	 }
	
	 return newPlace;
	}
 
}
void SA_NextLine(char * file, int way){ 
    
    int hit = 0;
    int total = 0;
    int setIndex;
    int nextIndex;
    int tag;
    int sets = 512/way;
    int setAssociative[sets][way];
    int recency[sets][biggest];
    
    for(int i = 0; i < sets; i++){
      for(int j = 0; j < way; j++){
        setAssociative[i][j] = -1;
        recency[i][j] = -1;
      }
    }
   

    //open file
    ifstream infile(file);
    string ldSt;
    int address;
    int logOf = log2(sets);
    
    // The following loop will read a line at a time
    while(infile >> ldSt >> std::hex >> address) {
      address = address >> 5;
      setIndex = address % (sets);
      nextIndex = (address+1) % sets;
      tag = address >> logOf;
      bool isThere = 0;
      bool inNext = 0;;
      int LRU_val = -1;


      for(int i = 0; i < way; i++){
        //check if there
         if(setAssociative[setIndex][i] == tag){
           hit++;
	   LRU_val = LRU(setIndex, way, i, recency);
	   isThere =1;
           break;
         }
     }

    
      
      if(!isThere){
      	  LRU_val = LRU(setIndex, way, -1, recency);
	  setAssociative[setIndex][LRU_val]= tag;
     }
	      
      for(int i = 0; i < way; i++){
        //check if there
         if(setAssociative[nextIndex][i] == tag){
	   LRU_val =  LRU(nextIndex, way, i, recency);
	   inNext =1;
           break;
         }
     }
      
     if(!inNext){
          LRU_val = LRU(nextIndex, way, -1, recency);
	  setAssociative[nextIndex][LRU_val] = tag;
     }
    
    
    
    total++;
    }
   
    cout<<hit<<",";
    cout<<total<<"; ";

}

void SA_NextLineMiss(char * file, int way){
    int hit = 0;
    int total = 0;
    int setIndex;
    int nextIndex;
    int tag;
    int sets = 512/way;
    int setAssociative[sets][way];
    int recency[sets][biggest];

    for(int i = 0; i < sets; i++){
      for(int j = 0; j < way; j++){
        setAssociative[i][j] = -1;
        recency[i][j] = -1;
      }
    }

    //open file
    ifstream infile(file);
    string ldSt;
    int address;
    int logOf = log2(sets);

    // The following loop will read a line at a time
    while(infile >> ldSt >> std::hex >> address) {
      address = address >> 5;
      setIndex = address % (sets);
      nextIndex = (address+1) % sets;
      tag = address >> logOf;
      bool isThere = 0;
      bool inNext = 0;;
      int LRU_val = -1;


      for(int i = 0; i < way; i++){
        //check if there
         if(setAssociative[setIndex][i] == tag){
           hit++;
	   LRU(setIndex, way, i, recency);
	   isThere =1;
           break;
     	 }
     }

      if(!isThere){
      	  LRU_val = LRU(setIndex, way, -1, recency);
	  setAssociative[setIndex][LRU_val]= tag;
     
	      
      	for(int i = 0; i < way; i++){
        //check if there
         	if(setAssociative[nextIndex][i] == tag){
	  	 LRU(nextIndex, way, i, recency);
	   	inNext =1;
           	break;
      		}	
	}
      
     	if(!inNext){
         	 LRU_val = LRU(nextIndex, way, -1, recency);
	  	setAssociative[nextIndex][LRU_val] = tag;
	}
     }
    
     total++;
   }
    cout<<hit<<",";
    cout<<total<<"; ";
}


int main(int argc, char * argv[]){
  DirectMap(argv[1], 32);
  DirectMap(argv[1], 128);
  DirectMap(argv[1], 512);
  DirectMap(argv[1], 1024);
  cout<<"\n";
  SetAssociative(argv[1], 2);
  SetAssociative(argv[1],4);
  SetAssociative(argv[1],8);
  SetAssociative(argv[1], 16);
  cout<<"\n";
  SetAssociative(argv[1], 512);
  cout<<"\n";
  HnC_fullAssociative(argv[1], 512);
  cout<<"\n";
  SetAssociativeWriteMiss(argv[1], 2);
  SetAssociativeWriteMiss(argv[1],4);
  SetAssociativeWriteMiss(argv[1],8);
  SetAssociativeWriteMiss(argv[1], 16);
  cout<<"\n";
  SA_NextLine(argv[1], 2);
  SA_NextLine(argv[1],4);
  SA_NextLine(argv[1],8);
  SA_NextLine(argv[1], 16);
  cout<<"\n";
  SA_NextLineMiss(argv[1], 2);
  SA_NextLineMiss(argv[1],4);
  SA_NextLineMiss(argv[1],8);
  SA_NextLineMiss(argv[1], 16);
   
}
