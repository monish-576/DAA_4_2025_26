#include<bits/stdc++.h>

using namespace std;

#define MAX 100

int heap[MAX];
int heapSize=0;


void heapifyDown(int i )
  
{
  
  int smallest = i;
  int left = 2*i+1;
  int right = 2*i+2;     
  
  if(left<heapSize && heap[smallest] > heap[left]) smallest = left;
  if(right <heapSize && heap[smallest] > heap[right]) smallest =right;
  
  if(smallest !=i) {
    
    swap(heap[i],heap[smallest]);
    heapifyDown(smallest);
}
  
}


void heapifyUp(int i) {

while(i>0 && heap[(i-1)/2] > heap[i]) {
swap(heap[(i-1)/2] , heap[i]);
  i =(i-1)/2;
  
}
}


void insert(int val) {

  if(heapSize ==MAX) {
cout<<"overflow" ;
  return ;
  }
  
  heap[heapSize] = val;
  heapSize++;
  heapifyUp(heapSize-1);
}
int main() {
    int K, N;
    cin >> K >> N;

    for (int i = 0; i < N; i++) {
        int score;
        cin >> score;

        if (heapSize < K) {
            insert(score);
            if (heapSize < K)
                cout << -1 << endl;
            else
                cout << heap[0] << endl;
        }
        else {
            if (score > heap[0]) {
                heap[0] = score;
                heapifyDown(0);
            }
            cout << heap[0] << endl;
        }
    }
    return 0;
}

