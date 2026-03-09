#include<iostream>
using namespace std;
int lowerbound(int arr[],int n)
{
    int k=5;
    int low=0,high=5;
    while(low<=high)
    {
        int mid=(low+high)/2;
        if(arr[mid]>=n)
        {
            k=mid;
            high=mid-1;
        }
        else
        {
            low=mid+1;
        }
    }
    return k;
}
int upperbound(int arr[],int n)
{
    int k=5;
    int low=0,high=5;
    while(low<=high)
    {
        int mid=(low+high)/2;
        if(arr[mid]>n)
        {
            k=mid;
            high=mid-1;
        }
        else
        {
            low=mid+1;
        }
    }
    return k;
}
int main()
{
    int n;
    cin>>n;
    int arr1[6]={2,3,1,5,6,4};
    int arr2[6]={1,2,4,4,5,8};
    int p=0;
    for(int i=0;i<6;i++)
    {
    int k=lowerbound(arr2,arr1[i]);
    int m=upperbound(arr2,arr1[i]);
    if(k-m!=0)
    p+=(m-k);
    }
   cout<<p;
    return 0;
}
