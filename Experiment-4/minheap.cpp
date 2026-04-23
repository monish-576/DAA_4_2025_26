#include <bits/stdc++.h>
using namespace std;

#define MAX 100

int heap[MAX];
int heapSize = 0;

void heapifyDown(int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heapSize && heap[smallest] > heap[left]) smallest = left;
    if (right < heapSize && heap[smallest] > heap[right]) smallest = right;

    if (smallest != i) {
        swap(heap[i], heap[smallest]);
        heapifyDown(smallest);
    }
}

void heapifyUp(int i) {
    while (i > 0 && heap[(i - 1) / 2] > heap[i]) {
        swap(heap[(i - 1) / 2], heap[i]);
        i = (i - 1) / 2;
    }
}

void insert(int val) {
    if (heapSize == MAX) {
        cout << "Overflow" << endl;
        return;
    }
    heap[heapSize] = val;
    heapSize++;
    heapifyUp(heapSize - 1);
}

void deleteRoot() {
    if (heapSize == 0) {
        cout << "No element" << endl;
        return;
    }
    heap[0] = heap[heapSize - 1];
    heapSize--;
    heapifyDown(0);
}

void deleteAtIndex(int index) {
    if (index < 0 || index >= heapSize) {
        cout << "Invalid index" << endl;
        return;
    }
    heap[index] = heap[heapSize - 1];
    heapSize--;
    heapifyDown(index);
    heapifyUp(index);
}

bool search(int val) {
    for (int i = 0; i < heapSize; i++) {
        if (heap[i] == val) return true;
    }
    return false;
}

void printHeap() {
    for (int i = 0; i < heapSize; i++) {
        cout << heap[i] << " ";
    }
    cout << endl;
}

int main() {
    insert(10);
    insert(5);
    insert(20);
    insert(2);
    insert(15);

    cout << "Heap elements: ";
    printHeap();

    cout << "Searching 15: " << (search(15) ? "Found" : "Not Found") << endl;
    cout << "Searching 100: " << (search(100) ? "Found" : "Not Found") << endl;

    cout << "Deleting element at index 2" << endl;
    deleteAtIndex(2);

    cout << "Heap after deletion: ";
    printHeap();

    cout << "Deleting root element" << endl;
    deleteRoot();

    cout << "Heap after deleting root: ";
    printHeap();

    return 0;
}