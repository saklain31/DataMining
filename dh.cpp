
#include <bits/stdc++.h>

using namespace std;

#define D(x)            cerr << #x " = " << x << '\n'
#define DBG             cerr << "Hi!" << '\n'

const int MAXP = 1000000; // 1<<N , maximum number of prefixes for hash
const int MAXB = 10000; // maximum number of buckets

int N; // hash prefix size
int B; // nucket size
int mod;
int GD; // global depth

int it = 0;

struct node { // Bucket
    int id;
    int depth;
    set <int> data;
    node* nextBucket;

    node(int _depth) {
        depth = _depth;
        id = ++it;
        nextBucket = NULL;
    }
};

node* pointers[MAXP];
node* temp[MAXP];
bool vis[MAXB];


void init() {
    GD = 0;
    pointers[0] = new node(0);
}

void printBucket(node* buc) {
    cout << "(" << buc->depth << ")[ ";
    for(auto x : buc->data) {
        cout << x << " ";
    }
    cout << "]   ";
}

void printBinary(int n,int lim) {
	for(int i=lim-1;i>=0;i--) {
		if( n & (1<<i)) cout << 1;
		else cout << 0;
	}
}


void Print() {
    puts("\n\n-----------------------------------------------");
    memset(vis,0,sizeof(vis));
    for(int i=0;i<(1<<GD);i++) {
        printBinary(i,GD);
        cout  << " --> ";
        if(!vis[pointers[i]->id]) {
            vis[pointers[i]->id] = true;
            node* cur = pointers[i];
            while(cur) {
                printBucket(cur);
                cur = cur->nextBucket;
            }
        }
        else
        {
            cout << ".....";
        }
        puts("");
    }
    puts("-----------------------------------------------\n\n");
}


void Insert(int val) {
    int id = val % mod;
    id = id >> (N-GD);

    node* buc = pointers[id];
    buc->data.insert(val);

    if(buc->data.size() > B) {
        if(buc->depth == GD) {
            if(GD == N) {
                node* cur = buc;
                while(true) {
                    if(cur->data.size()>B) {
                        cur->data.erase(val);
                        if(cur->nextBucket == NULL) {
                            cur->nextBucket = new node(buc->depth);
                        }
                        cur = cur->nextBucket;
                    }
                    else break;
                    cur->data.insert(val);
                }
                return;
            }
            puts("Case 1");

            for(int i=0;i<(1<<GD);i++) temp[i] = pointers[i];

            for(int i=0;i<(1<<GD);i++) {
                pointers[i<<1] = temp[i];
                pointers[(i<<1)+1] = temp[i];
            }

            GD++;
            buc->depth++;
            node* nbuc = new node(buc->depth);

            int a = id << 1;
            int b = a + 1;

            for(auto x : buc->data) {
                int cur = (x % mod) >> (N - GD);
                if( cur == b ) {
                    nbuc->data.insert(x);
                }
            }
            for(auto x : nbuc->data) {
                buc->data.erase(x);
            }
            for(int i=0;i<(1<<GD);i++) {
                int cur = i;
                if( cur == a ) pointers[i] = buc;
                if( cur == b ) pointers[i] = nbuc;
            }

            if(buc->data.size() > B) {
                buc->data.erase(val);
                puts("Recursive Insertion");
                Insert(val);
            }
            if(nbuc->data.size() > B) {
                nbuc->data.erase(val);
                puts("Recursive Insertion");
                Insert(val);
            }

        }
        else {
            puts("Case 2");
            D(buc->id);
            int a = (*buc->data.begin() % mod) >> (N-buc->depth);
            a <<= 1;
            int b = a + 1;

            buc->depth++;

            node* nbuc = new node(buc->depth);

            for(auto x : buc->data) {
                int cur = (x % mod) >> (N - buc->depth);
                if( cur == b ) {
                    nbuc->data.insert(x);
                }
            }
            for(auto x : nbuc->data) {
                buc->data.erase(x);
            }

            for(int i=0;i<(1<<GD);i++) {
                int cur = i >> (GD - buc->depth);
                if( cur == a ) pointers[i] = buc;
                if( cur == b ) pointers[i] = nbuc;
            }

            if(buc->data.size() > B) {
                buc->data.erase(val);
                puts("Recursive Insertion");
                Insert(val);
            }
            if(nbuc->data.size() > B) {
                nbuc->data.erase(val);
                puts("Recursive Insertion");
                Insert(val);
            }
        }
    }
}

//void Delete(int val) {
//    int id = val & ((1<<GD)-1);
//    node* buc = pointers[id];
//
//    if(buc->data.find(val)!=buc->data.end()) {
//        buc->data.erase(val);
//        printf("%d deleted successfully!\n",val);
//        return;
//    }
//    printf("%d is not found in the hash table!\n",val);
//}

int main() {
//    freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);

    init();

//    N = 3;
//    mod = (1<<N);
//    B = 3;

	//printBinary(10,5);

    //printf("Enter N : ");
    cin >> N;
    mod = (1<<N);
    //printf("Enter the size of each bucket : ");
    cin >> B;

    int cmd, val;

    while(true) {
        puts("1: insert");
        puts("2: print");
        puts("0: exit");

        cin >> cmd;

        if(cmd == 1) {
            cin >> val;
            Insert(val);
        }
        else if(cmd == 2) {
            Print();
        }
        else if(cmd == 0) {
            break;
        }
    }

    return 0;
}



