#include<bits/stdc++.h>
#include<string.h>
using namespace std;
using namespace std::chrono;
    
int MaxChild, MaxKey; 

struct Block
{
    string *en, *bn;
    int currentNode;
    bool isLeaf;
    Block *parentBlock, *siblingBlock, **childPtr;
}*rootBlock;

Block *newBlock(void);
Block *search(string, int*);
void insertLeaf(string, string);
void insertParent(Block*, string, Block*, Block*);

int main(void)
{
	cout << "Enter number of Child (fan out): ";
	cin >> MaxChild;
	MaxKey = MaxChild-1;

    rootBlock = newBlock();
    Block *leafBlock;
    int i=0,counter;
    string en,bn,searchEnglish;

    ifstream ifile;
    ifile.open("dictionary.txt");
    if(!ifile){
        cout<<"Error: file not found";
        return 0;
    } 
    while(ifile>>en)
    {
    	getline(ifile,bn);
    	insertLeaf(en,bn);
    }

    cout<<"Enter english word to search: ";
    while(cin>>searchEnglish)
    {
        auto start = high_resolution_clock::now();

        leafBlock= search(searchEnglish,&counter);
        for(i=0; i<leafBlock->currentNode; i++) if(searchEnglish==leafBlock->en[i]) break;
        if(i==leafBlock->currentNode) cout<<"no word founds"<<endl;
        else cout<<searchEnglish<<" "<<leafBlock->bn[i]<<endl;

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
	    cout << "Time taken for searching "<< duration.count()<< " microseconds and steps "<<counter<<endl;
    	cout<<"enter text to search: ";
    }

    return 0;
}


Block* newBlock()
{
    Block *temp = new Block();
    temp->en=new string[MaxKey];
    temp->bn=new string[MaxKey];
    temp->currentNode = 0;
    temp->isLeaf = true;
    temp->parentBlock = NULL;
    temp->siblingBlock = NULL;
    temp->childPtr = new Block *[MaxChild];
    return temp;
}

Block *search(string en, int *counter)
{
    Block *temp = rootBlock;
    int sum=0;
    while(temp->isLeaf==false)
    {
        int i;
        for(i=0; i<temp->currentNode; i++) 
            if(en<temp->en[i]) break;

        temp = temp->childPtr[i];
        sum++;
    }
    *counter = sum;
    return temp;
}

void insertParent(Block *parentBlock,string value,Block *leftBlock,Block *rightBlock)
{
    if(parentBlock == NULL)        
    {
        parentBlock = newBlock();
        parentBlock->en[0] = value;
        parentBlock->isLeaf = false;
        parentBlock->childPtr[0] = leftBlock;
        parentBlock->childPtr[1] = rightBlock;
        parentBlock->currentNode++;
        rootBlock = parentBlock;
        leftBlock->parentBlock = parentBlock;
        rightBlock->parentBlock = parentBlock;
        return;
    }
    else{
        rightBlock->parentBlock = parentBlock;
        int i = parentBlock->currentNode-1;
        for( ;i>=0; i--)
        {
            if(parentBlock->en[i] > value){
                parentBlock->en[i+1] = parentBlock->en[i];
                parentBlock->childPtr[i+2] = parentBlock->childPtr[i+1];
            }
            else break;
        }
        parentBlock->en[i+1] = value;
        parentBlock->childPtr[i+2] = rightBlock;
        parentBlock->currentNode++;

        if(parentBlock->currentNode == MaxKey)
        {
            Block *splitBlock = newBlock();
            splitBlock->isLeaf = false;
            int mid = parentBlock->currentNode-(MaxKey)/2, j=0;
            splitBlock->childPtr[j] = parentBlock->childPtr[mid];
            splitBlock->childPtr[j]->parentBlock = splitBlock;
            for(int i=mid; i<MaxKey; i++)
            {
                splitBlock->en[j] = parentBlock->en[i];
                splitBlock->childPtr[j+1] = parentBlock->childPtr[i+1];
                splitBlock->childPtr[j+1]->parentBlock = splitBlock;
                j++;
            }
            parentBlock->currentNode -= (MaxKey)/2+1;
            splitBlock->currentNode = (MaxKey)/2;
            insertParent(parentBlock->parentBlock,parentBlock->en[parentBlock->currentNode],parentBlock,splitBlock);
        }
    }
}

void insertLeaf(string en,string bn)
{
    int counter=0;
    Block *leafBlock = search(en,&counter);    

    if(leafBlock->currentNode>=1){
        int i = leafBlock->currentNode - 1;
        for(; i>=0; i--)
        {
            if(en<leafBlock->en[i])                
            {
                leafBlock->en[i+1] = leafBlock->en[i];
                leafBlock->bn[i+1] = leafBlock->bn[i];
            }
            else break;
        }
        leafBlock->en[i+1] = en;
        leafBlock->bn[i+1] = bn;
        leafBlock->currentNode++;  
    }
    else{
        leafBlock->en[0] = en;
        leafBlock->bn[0] = bn;
        leafBlock->currentNode = 1;
    }

    if(leafBlock->currentNode==MaxKey)               
    {
        Block *splitBlock = newBlock();
        int j=0;
        for(int i=leafBlock->currentNode-MaxChild/2;i<MaxKey; i++)
        {
            splitBlock->en[j] = leafBlock->en[i];
            splitBlock->bn[j] = leafBlock->bn[i];
            j++;
        }
        leafBlock->currentNode-= MaxChild/2;
        splitBlock->currentNode = MaxChild/2;
        splitBlock->siblingBlock = leafBlock->siblingBlock;
        leafBlock->siblingBlock = splitBlock;
        insertParent(leafBlock->parentBlock,splitBlock->en[0],leafBlock,splitBlock);
    }
}
