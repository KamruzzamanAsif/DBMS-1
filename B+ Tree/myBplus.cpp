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
} *root;

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

block* leafFinder(string english){
    block *temp = root;
    
    do{
        int i;
        for(i = 0; i <temp->currentNode-1; i++){
            if(temp->english[i] > english) 
                break;
        }
        temp = temp->childPtr[i];
    }while(temp->isLeaf == false);

    return temp;
}


void insert(string english, string bangla){
    block* leaf = leafFinder(english);
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