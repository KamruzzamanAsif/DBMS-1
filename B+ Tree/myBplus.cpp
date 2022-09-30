#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;

int MaxChild;
int MaxKey;

struct block{
    int currentNode;
    string* english;
    string* bangla;
    block* parent;
    block* sibling;
    block** childPtr;
    bool isLeaf;
} *rootBlock;

block* newBlock(){
    block* newBlock = new block();
    newBlock-> currentNode = 0;
    newBlock-> english = new string[MaxKey];
    newBlock-> bangla = new string[MaxKey];
    newBlock-> parent = NULL;
    newBlock-> sibling = NULL;
    newBlock-> childPtr = new block* [MaxChild];
    newBlock-> isLeaf = true;
    return newBlock;
}

block* suitableLeafBlockFinder(string english){
    block *tempBlock = rootBlock;
    
    do{
        int i;
        for(i = 0; i <tempBlock->currentNode-1; i++){
            if(tempBlock->english[i] > english) 
                break;
        }
        tempBlock = tempBlock->childPtr[i];
    }while(tempBlock->isLeaf == false);

    return tempBlock;
}

void insertIntoLeafBlock(block* leafBlock, string english, string bangla){
    int index = leafBlock->currentNode - 1;
    for( ; index>=0; index--){
        if(leafBlock->english[index] > english){
            leafBlock->english[index+1] = leafBlock->english[index];
            leafBlock->bangla[index+1] = leafBlock->bangla[index]; 
        }
        else{
            break;
        }
    }
    leafBlock->english[index+1] = english;
    leafBlock->bangla[index+1] = bangla; 

    leafBlock->currentNode++;
}

void middleBlocksHandeler(block* leftBlock, block* rightBlock, block* parentBlock, string value){
    
}

void splitHandeler(block* leafBlock, string english, string bangla){
    block* splitBlock = newBlock();

    int middle = 1 + ((MaxKey-1)/2); // taking the ceiling value
    int s_index = 0; // splitBlock index
    // we are making right biased tree here
    for(int i=middle; i<MaxKey; i++){
        splitBlock->english[s_index] = leafBlock->english[i];
        splitBlock->bangla[s_index] = leafBlock->bangla[i];
        s_index++; 
    }
    splitBlock->english[s_index] = english;
    splitBlock->bangla[s_index] = bangla;

    leafBlock->currentNode = middle;
    splitBlock->currentNode = s_index + 1;
    splitBlock->sibling = leafBlock->sibling;
    leafBlock->sibling = splitBlock;

    middleBlocksHandeler(leafBlock, splitBlock, leafBlock->parent, splitBlock->english[0]);
}

void insert(string english, string bangla){
    block* leafBlock = suitableLeafBlockFinder(english);
    
    if(leafBlock->currentNode < MaxKey){
        insertIntoLeafBlock(leafBlock, english, bangla);
    }
    else if(leafBlock->currentNode == MaxKey){
        splitHandeler(leafBlock, english, bangla);
    }
    else{
        cout<<"Insert Error (max key overflow)"<<endl;
    }
}





int main(void){
    cout<<"Enter number of child(fan out): ";
    cin>>MaxChild;
    MaxKey = MaxChild - 1;


    string english, bangla;
    ifstream ifile;
    ifile.open("dictionary.txt");
    if(!ifile){
        cout<<"Error: dictionary.txt not found";
        return 0;
    } 
    while(ifile>>english)
    {
    	getline(ifile,bangla);
    	insert(english,bangla);
    }
}