
#include <bits/stdc++.h>

using namespace std;

string s;
int k=5;

void convert(int n)
{
	if(n==0)
	{
		return;
	}
	else
	{
		char ch = (n%2)+'0';
		s=ch+s;
		convert(n/2);
	}
}



void appendzero(int k){
	while(s.size()<k){
		s="0"+s;
	}
}


void conv(int n)
{
	s="";
	convert(n);
	appendzero(k);
	cout<<s;
}

main()
{
	int n;
	
	while(1)
	{
		cin >> n;
		conv(n);
		//printf("zero %0d ",k,convert(n));
	}
}