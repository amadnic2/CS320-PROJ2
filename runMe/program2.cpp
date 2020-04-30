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
	
	int sets = 512/way;
	int setAssociative[sets][way];
	int hNc[9][256];
	int logOf = log2(sets);
	
	for (int i=0; i<sets; i++){
		for (int j=0; j<way; j++){	
			setAssociative[i][j] = -1;
		}
	}
	
	for (int i=0; i <9; i++){
		for (int j =0; j<256; j++){
			hNc[i][j] = 0;
		}
	}

	int hits = 0;
	int total = 0;
	int index;
	string ldstr;
	int addr;
	int tag;

	ifstream inFile(file);
	while (inFile >> ldstr >> hex >> address){
         	addr = addr>>5;
		index = addr % sets;
		tag = addr>>logSize;
		int isThere = 0;
		//int LRUret = -1;

		for (int i=0; i<way; i++){
			if (setAssociative[index][i] == tag){
				hits++;
				int start = 0;
				if (i%2 == 1){
					start = 1;
					i--;
				}
				i/= 2;
				for (int j=8; j> -1; j--){
					hNc[i][j] = start;
					if (i%2 == 1){
						start= 1;
						i--;
					}
					else{
						start = 0;
					}
					i /= 2;
				 }
				//LRUret = hotCold(k, coldness);
				isThere = 1;
				break;
			
			}
		}
			
		if (isThere == 0){
			int coldest = 0;
			for (int i=0; i<9; i++){
				if (hNc[i][coldest] == 0){
					hNc[i][coldest] = 1;
					coldest = (coldest*2)+1;
				}
				else{
					hNc[i][coldest] = 0;
					coldest = coldest*2;
				}
			}
			setAssociative[index][coldest] = tag;
		}
		total++;
	}
	cout << hits << "," << total;
}
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

void SetAssociativeNextLine(char * file, int way){
    int hit = 0;
    int total = 0;
    int setIndex;
    int nextIndex;
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
      nextIndex = (address+1) % (sets);
      tag = address >> logOf;
      bool isThere = 0;
      bool inNext = 0;
      int newPlace;
      int nextPlace;
      int oldRecency;
      int nextRecency;


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
      if(isThere){
        for(int i = 0; i < way; i++){
          if(i != newPlace){
            if(recency[setIndex][i] > 0 && recency[setIndex][i] > oldRecency)  recency[setIndex][i]--;
        }
       }
      }

 
      for(int i = 0; i < way; i++){
        //check if there
         if(setAssociative[nextIndex][i] == tag){
	   nextRecency = recency[nextIndex][i];
           recency[nextIndex][i] = way-1;
           inNext = 1;
           nextPlace = i;
	   break;
        }
      }
      if(inNext){
        for(int i = 0; i < way; i++){
          if(i != nextPlace){
            if(recency[nextIndex][i] > 0 && recency[nextIndex][i] > nextRecency)  recency[nextIndex][i]--;
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
     if(!inNext){
         bool replaceN = 1;
         for(int i = 0; i < way; i++){
           //check if room
           if(setAssociative[nextIndex][i] == -1){
              recency[nextIndex][i]= way-1;
              setAssociative[nextIndex][i] = tag;
              replaceN = 0;
              nextPlace = i;
              break;
           }
         }

         //update recency
         if(!replaceN){
           for(int i = 0; i < way; i++){
             if(i == nextPlace) continue;
             else{
               if(recency[nextIndex][i] > 0)  recency[nextIndex][i]--;
             }
           }
         }

         if(replaceN){
         //find replacement
          int leastR = way;
          for(int i= 0; i < way; i++){
            if(recency[nextIndex][i] < leastR){
              leastR = recency[nextIndex][i];
              nextPlace = i;
           }
          }

          //set replacement tag and recency. uses first least recenct slot
          for(int i = 0; i < way; i++){
            if(i == nextPlace){
              setAssociative[nextIndex][i] = tag;
              recency[nextIndex][i] =way -1;
            }

            else{
              if(recency[nextIndex][i] > 0)  recency[nextIndex][i]--;
            }

          }
        }
     }
     total++;
   }
    cout<<hit<<",";
    cout<<total<<"; ";
}

void SetAssociativeNextLineMiss(char * file, int way){
    int hit = 0;
    int total = 0;
    int setIndex;
    int nextIndex;
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
      nextIndex = (address+1) % (sets);
      tag = address >> logOf;
      bool isThere = 0;
      bool inNext = 0;
      int newPlace;
      int nextPlace;
      int oldRecency;
      int nextRecency; 


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
     
    
      for(int i = 0; i < way; i++){
        //check if there
         if(setAssociative[nextIndex][i] == tag){
	   nextRecency = recency[nextIndex][i];
           recency[nextIndex][i] = way-1;
           inNext = 1;
           nextPlace = i;
	   break;
        }
      }
	
      if(inNext){
        for(int i = 0; i < way; i++){
          if(i != nextPlace){
          
            if(recency[nextIndex][i] > 0 && recency[nextIndex][i] > nextRecency)  recency[nextIndex][i]--;
        }
       }
      }
	    
     if(!inNext){
         bool replace = 1;
         for(int i = 0; i < way; i++){
           //check if room
           if(setAssociative[nextIndex][i] == -1){
              recency[nextIndex][i]= way-1;
              setAssociative[nextIndex][i] = tag;
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
               if(recency[nextIndex][i] > 0)  recency[nextIndex][i]--;
             }
           }
         }

         if(replace){
         //find replacement
          int leastR = way;
          for(int i= 0; i < way; i++){
            if(recency[nextIndex][i] < leastR){
              leastR = recency[nextIndex][i];
              newPlace = i;
           }
          }

          //set replacement tag and recency. uses first least recenct slot
          for(int i = 0; i < way; i++){
            if(i == newPlace){
              setAssociative[nextIndex][i] =tag;
              recency[nextIndex][i] =way -1;
            }

            else{
              if(recency[nextIndex][i] > 0)  recency[nextIndex][i]--;
            }

          }
        }
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
  SetAssociativeWriteMiss(argv[1], 2);
  SetAssociativeWriteMiss(argv[1],4);
  SetAssociativeWriteMiss(argv[1],8);
  SetAssociativeWriteMiss(argv[1], 16);
  cout<<"\n";
  SetAssociativeNextLine(argv[1], 2);
  SetAssociativeNextLine(argv[1],4);
  SetAssociativeNextLine(argv[1],8);
  SetAssociativeNextLine(argv[1], 16);
  cout<<"\n";
  SetAssociativeNextLineMiss(argv[1], 2);
  SetAssociativeNextLineMiss(argv[1],4);
  SetAssociativeNextLineMiss(argv[1],8);
  SetAssociativeNextLineMiss(argv[1], 16);
   
}
