#include <bits/stdc++.h>
using namespace std;
int op=0;

void complexRec(int n) {


   if (n <= 2) {
       op++;
       return;
   }


   int p = n;
   while (p > 0) {
       vector<int> temp(n);
       for (int i = 0; i < n; i++) {
           temp[i] = i ^ p;
           op++;
       }
       p >>= 1;
   }


   vector<int> small(n);
   for (int i = 0; i < n; i++) {
       op++;
       small[i] = i * i;
   }


   if (n % 3 == 0) {
       op++;
       reverse(small.begin(), small.end());
   } else {
       op++;
       reverse(small.begin(), small.end());
   }


   complexRec(n / 2);
   complexRec(n / 2);
   complexRec(n / 2);
}
int main()
{
    int n;
    cin>>n;
    complexRec(n);
    cout<<"operations:"<<op<<endl;
    int k=n,l=0;
    while(k!=0)
    {
        k=k/2;
        l++;
    }
    cout<<"Depth:"<<l-1<<endl;
    cout<<"Reccurence Relation:T(n)=3T(n/2)+nlogn"<<endl;
    cout<<"Time Complexity:O(n^log3)";
}