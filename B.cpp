#include <bits/stdc++.h>

using namespace std;

int N=4;
int bucketCount=0;
int p=0;

struct bucket {
	
    vector <int> values;
	vector <struct bucket*> pointers;
	struct bucket* parent;
	bool isLeaf;
	int id;

	bucket() 
    {
        isLeaf = false;
        id = ++p;
	}
};

struct bucket* root;

vector <bucket*> buckets[100];

bucket *newLeaf(int val,bucket* par) 
{
    bucket* node = new bucket();
    node->isLeaf = true;
    node->parent = par;
    node->values.push_back(val);
    return node;
}

void leafInsertion(bucket* leaf,int val) 
{
    leaf->values.push_back(val);
    sort(leaf->values.begin(),leaf->values.end());
}

bucket* nonLeafBucket(int val,bucket* par,bucket* left,bucket* right) 
{
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
        int div = N/2;
        int rightval = div+1;
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
            bucket *temp = nonLeafBucket(node->values[div],NULL,node,newnode);
            root = temp;
            node->parent = temp;
            newnode->parent = temp;
        }
        else 
        	nonLeafInsertion(node->parent,node->values[div],node,newnode);

        int tag = node->values.size() - div;
        for(int i=0;i<tag;i++) 
        {
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
        else if(temp->values[id] == val) temp = temp->pointers[id+1];
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

void findByDepth(bucket* node,int depth) 
{
    if(node==NULL or node->id==0) 
        return;
    buckets[depth].push_back(node);
    for(int i=0;i<node->pointers.size();i++) 
    {
        findByDepth(node->pointers[i],depth+1);
    }
}

void printbucket(bucket* node) 
{
    for(int i=0;i<node->values.size();i++) 
    {
        cout << "{" << (node->isLeaf ? 0 : node->pointers[i]->id ) << "}";
        cout << node->values[i];
        
    }
    cout << "{" << (node->isLeaf ? 0 : node->pointers[node->pointers.size()-1]->id ) << "}";
}

void print() {
    printf("\n---------------------------------------------------------------------------------\n");
    for(int i=0;i<100;i++) buckets[i].clear();

    findByDepth(root,0);

    for(int i=0;i<100;i++) {
        for(int j=0;j<buckets[i].size();j++) {
            if(j) 
                printf("\t");
            printbucket(buckets[i][j]);
        }
        if( (int)buckets[i].size() != 0 ) puts("");
    }
    printf("\n---------------------------------------------------------------------------------\n");
}


void serachValue(int v)
{
    bucket* nd = findLeaf(v);
    int flag=0;
    for(int i;i<nd->values.size();i++)
    {
        if(nd->values[i]==v)
        {
            flag=1;
            break;
        }
    }

    printf("*****************\n");
    if(flag)
        printf("%d is Present\n",v);
    else
        printf("%d is not Present\n",v);
    printf("*****************\n");
}

int main() 
{
    int choice;
    int val;
    

    scanf("%d",&N);

    while(true) 
    {
        printf("Enter 0 to exit\n");
        printf("Enter 1 to insert\n");
        printf("Enter 2 to print tree\n");
        printf("Enter 3 to search\n");
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
        else if(choice==3)
        {
            int k;
            printf("Enter value:\n");
            scanf("%d",&k);
            serachValue(k);
        }
    }
    return 0;
}

