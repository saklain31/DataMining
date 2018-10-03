#include <bits/stdc++.h>

using namespace std;

#define D(x)            cerr << #x " = " << x << '\n'
#define DBG             cerr << "Hi!" << '\n'

//typedef int int;

const int N = 4;
const int MAX_L = 100;

int bucketCount = 0;

int it = 0;

struct bucket {
	vector <int> values;
	vector <struct bucket*> pointers;
	struct bucket* parent;
	//struct bucket* sibling;
	bool isLeaf;
	int id;
	bucket() {
        //values.reserve(N);
        //pointers.reserve(N+1);
        sibling = NULL;
        isLeaf = false;
        id = ++it;
	}
};

struct bucket* root;

vector <bucket*> buckets[MAX_L];

void dfs(bucket* node,int level) {
    if(node==NULL or node->id==0) return;
    buckets[level].push_back(node);
    for(int i=0;i<node->pointers.size();i++) {
        dfs(node->pointers[i],level+1);
    }
}


bucket *newLeaf(int val,bucket* par) {
    bucket* node = new bucket();
    node->isLeaf = true;
    node->parent = par;
    node->values.push_back(val);
    return node;
}

void leafInsertion(bucket* leaf,int val) {
    leaf->values.push_back(val);
    sort(leaf->values.begin(),leaf->values.end());
}

bucket* nonLeafBucket(int val,bucket* par,bucket* left,bucket* right) {
    bucket* node = new bucket();
    node->isLeaf = false;
    node->parent = par;
    node->values.push_back(val);
    node->pointers.push_back(left);
    node->pointers.push_back(right);
    return node;
}

void nonLeafInsertion(bucket* node,int val,bucket* left,bucket *right)
{
    for(int i=0;i<(int)node->pointers.size();i++) //Finding Appropriate Node for inserting
    {
        if(node->pointers[i] == left) 
        {
            node->values.insert(node->values.begin()+i,val);
            node->pointers.erase(node->pointers.begin()+i);
            node->pointers.insert(node->pointers.begin()+i,right);
            node->pointers.insert(node->pointers.begin()+i,left);
            break;
        }
    }

    if(node->values.size() == N) 
    {
        int mid = N/2;
        int rightval = mid+1;
        bucket* newnode = nonLeafBucket(node->values[rightval],node->parent,node->pointers[rightval],node->pointers[rightval+1]);

        node->pointers[rightval]->parent = newnode;
        node->pointers[rightval+1]->parent = newnode;

        for(int i=rightval+1;i<node->values.size();i++) 
        {
            newnode->values.push_back(node->values[i]);
            newnode->pointers.push_back(node->pointers[i+1]);
            newnode->pointers[i+1]->parent = newnode;
        }

        if(node->parent == NULL) 
        {
            bucket *temp = nonLeafBucket(node->values[mid],NULL,node,newnode);
            root = temp;
            node->parent = temp;
            newnode->parent = temp;
        }
        else 
        	nonLeafInsertion(node->parent,node->values[mid],node,newnode);

        int tag = node->values.size() - mid;
        for(int i=0;i<tag;i++) {
            node->values.pop_back();
            node->pointers.pop_back();
        }
    }
}

bucket* findLeaf(int val) 
{
    bucket* temp = root;
    while(temp->isLeaf == false) 
    {
        int id = -1;
        for(int i=0;i<temp->values.size();i++) 
        {
            if( temp->values[i] >= val ) 
            {
                id = i;
                break;
            }
        }
        if( id == -1 ) temp = temp->pointers[ temp->pointers.size()-1 ]; //Value is greater than all. So Last Pointer.
        else temp = temp->pointers[id]; 
    }
    return temp;
}

void valueInsertion(int val) {
    if(bucketCount == 0) 
    {
        root = newLeaf(val,NULL);
        bucketCount++;
        return;
    }
    bucket* leaf = findLeaf(val);

    leafInsertion(leaf,val);

    if( leaf->values.size() == N ) {

        bucket* childLeaf = NULL;
        int cnt = 0;
        for(int i=(N+1)/2;i<leaf->values.size();i++) //Inserting last elements to newly created Leaf Node
        {
            if(childLeaf == NULL) 
            	childLeaf = newLeaf(leaf->values[i],leaf->parent);
            else 
            	leafInsertion(childLeaf,leaf->values[i]);
            cnt++;
        }

        for(int i=0;i<cnt;i++) 
        	leaf->values.pop_back();

        if(leaf->parent==NULL) //Leaf has no parent.
        {
            bucket* temp = nonLeafBucket(childLeaf->values[0],NULL,leaf,childLeaf); //Creating root Node with 1st elem of Child
            root = temp;
            leaf->parent = temp;
            childLeaf->parent = temp;
        }
        else  
            nonLeafInsertion(leaf->parent,childLeaf->values[0],leaf,childLeaf);
       
    }
}


void fixParent(bucket* node) 
{
    if(node==NULL) 
    	return;

    for(int i=0;i<node->pointers.size();i++) 
    {
        node->pointers[i]->parent = node;
    }

    for(int i=0;i<node->pointers.size();i++) 
    {
        fixParent(node->pointers[i]);
    }
}

void printbucket(bucket* node) 
{
    cout << node->id << "-";
    for(int i=0;i<node->values.size();i++) {
        cout << "(" << (node->isLeaf ? 0 : node->pointers[i]->id ) << ")";
        cout << node->values[i];
    }
    cout << "(" << (node->isLeaf ? 0 : node->pointers[node->pointers.size()-1]->id ) << ")";
}

void print() {
    puts("\n---------------------------------------------------------------------------------");
    for(int i=0;i<MAX_L;i++) buckets[i].clear();
    dfs(root,0);
    for(int i=0;i<MAX_L;i++) {
        for(int j=0;j<buckets[i].size();j++) {
            if(j) printf("\t");
            printbucket(buckets[i][j]);
        }
        if( (int)buckets[i].size() != 0 ) puts("");
    }
    puts("\n---------------------------------------------------------------------------------");
}

int main() 
{
    int choice;
    int val;

    printf("Exit: 0\n");
    printf("Insert: 1\n");
    printf("Print: 2\n");

    while(true) 
    {
        cin >> choice;
        if(choice==0) 
        {
            return 0;
        }
        else if(choice==1) 
        {
            if(root!=NULL) 
            	fixParent(root);
            
            cin >> val;
            valueInsertion(val);
        }
        else if(choice==2) 
        {
            print();
        }
    }
    return 0;
}

