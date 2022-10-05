#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;

int MaxChild;
int MaxKey;

struct block{
    int currentNode;
    string* english;
    string* bangla;
    block* parentBlock;
    block* siblingBlock;
    block** childPtr;
    bool isLeaf;
};
struct block* rootBlock;

block* newBlock(){
    block* newBlock = new block();
    newBlock-> currentNode = 0;
    newBlock-> english = new string[MaxKey];
    newBlock-> bangla = new string[MaxKey];
    newBlock-> parentBlock = NULL;
    newBlock-> siblingBlock = NULL;
    newBlock-> childPtr = new block *[MaxChild];
    newBlock-> isLeaf = true;
    return newBlock;
}

block* suitableLeafBlockFinder(string english){
    block *tempBlock = rootBlock;
    
    while(tempBlock->isLeaf == false){
        int i;
        for(i = 0; i <tempBlock->currentNode; i++){
            if(tempBlock->english[i] > english) 
                break;
        }
        tempBlock = tempBlock->childPtr[i];
    }

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

void nullParentHandeler(block* leftBlock, block* rightBlock, block* parentBlock, string middle_value){
    parentBlock = newBlock();
    parentBlock->english[0] = middle_value;
    parentBlock->isLeaf = false;
    parentBlock->currentNode++;
    parentBlock->childPtr[0] = leftBlock;
    parentBlock->childPtr[1] = rightBlock;
    leftBlock->parentBlock = parentBlock;
    rightBlock->parentBlock = parentBlock;
    rootBlock = parentBlock;
}

void insertMiddleValueIntoParentBlock(block *parentBlock, block *rightBlock, string value){
    int index = parentBlock->currentNode - 1;
    for( ; index>=0; index--){
        if(parentBlock->english[index] <= value){
            break;
        }
        else{
            parentBlock->english[index+1] = parentBlock->english[index];
            parentBlock->childPtr[index+2] = parentBlock->childPtr[index+1];
        }
    }
    parentBlock->english[index+1] = value;
    parentBlock->childPtr[index+2] = rightBlock;
    parentBlock->currentNode++;
}

void middleBlocksHandeler(block* leftBlock, block* rightBlock, block* parentBlock, string middle_value){
    if(parentBlock == NULL){
        nullParentHandeler(leftBlock, rightBlock, parentBlock, middle_value);
    }
    else{
        rightBlock->parentBlock = parentBlock;
        insertMiddleValueIntoParentBlock(parentBlock, rightBlock, middle_value);
        if(parentBlock->currentNode == MaxKey){
            // parentBlockSplitHandler()
            block* splitBlock = newBlock();
            int middle = MaxKey/2;
            int s_index = 0;
            for(int i=middle; i<MaxKey; i++){
                splitBlock->english[s_index] = parentBlock->english[i];
                if(s_index == 0){
                    splitBlock->childPtr[0] = parentBlock->childPtr[i];
                    splitBlock->childPtr[0]->parentBlock = splitBlock;
                }
                splitBlock->childPtr[s_index+1] = parentBlock->childPtr[i+1];
                splitBlock->childPtr[s_index+1]->parentBlock = splitBlock;
                s_index++;
            }
            //update
            parentBlock->currentNode -= middle+1;
            splitBlock->currentNode = middle;
            middleBlocksHandeler(parentBlock, splitBlock, parentBlock->parentBlock, splitBlock->english[0]);
        }
    }
}

void LeafBlockSplitHandeler(block* leafBlock, string english, string bangla){
    block* splitBlock = newBlock();

    int middle = leafBlock->currentNode - MaxChild/2; // taking the ceiling value
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
    splitBlock->currentNode = MaxChild/2;
    splitBlock->siblingBlock = leafBlock->siblingBlock;
    leafBlock->siblingBlock = splitBlock;

    middleBlocksHandeler(leafBlock, splitBlock, leafBlock->parentBlock, splitBlock->english[0]);
}

void insert(string english, string bangla){
    block* leafBlock = suitableLeafBlockFinder(english);
 
    // if(leafBlock->currentNode < MaxKey){
    //     insertIntoLeafBlock(leafBlock, english, bangla);
    // }

    int index = leafBlock->currentNode - 1;
    if(index>-1){
        for( ; index>=0; index--){
            if(leafBlock->english[index] > english){
                leafBlock->english[index+1] = leafBlock->english[index];
                leafBlock->bangla[index+1] = leafBlock->bangla[index]; 
            }
            else{
                break;
            }
        }
    }
    leafBlock->english[index+1] = english;
    leafBlock->bangla[index+1] = bangla; 

    leafBlock->currentNode++;


    if(leafBlock->currentNode == MaxKey){
        LeafBlockSplitHandeler(leafBlock, english, bangla);
    }
}





int main(void){
    cout<<"Enter number of child(fan out): ";
    cin>>MaxChild;
    MaxKey = MaxChild - 1;
    cout<<MaxKey<<" "<<MaxChild<<endl;
    rootBlock = newBlock();
    
    string english, bangla;
    ifstream ifile;
    ifile.open("word.txt");
    if(!ifile){
        cout<<"Error: file not found";
        return 0;
    } 
    while(ifile>>english)
    {
    	getline(ifile,bangla);
    	insert(english,bangla);
    }

    while(1){
        cout<<"Enter word to search meaning: ";
        int flag = 0;
        string word;
        cin>>word;
        block* wordBlock = suitableLeafBlockFinder(word);
        for(int i=0; i<wordBlock->currentNode; i++){
            if(word == wordBlock->english[i]){
                cout<<wordBlock->bangla[i]<<endl;
                flag++;
            }
        }
        if(flag == 0)
            cout<<"word not found"<<endl;
    }
    
    return 0;
}