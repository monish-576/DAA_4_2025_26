#include<iostream>
using namespace std;

class Node {
public:
    int data;
    Node* next;

    
    Node(int val) {
        data = val;
        next = NULL;
    }
};

class stacks {
public:
    Node* top = NULL;

    void push(int val) {
        Node* newnode = new Node(val);

        newnode->next = top; 
        top = newnode;
    }

    void pop() {
        if(top == NULL) {
            cout << "Stack underflow" << endl;
        } else {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void isEmpty() {
        if(top == NULL)
            cout << "Stack is empty" << endl;
        else
            cout << "Stack is not empty" << endl;
    }

    void peek() {
        if(top == NULL)
            cout << "Stack is empty" << endl;
        else
            cout << top->data << endl;
    }

    void disp() {
        if(top == NULL) {
            cout << "Stack is empty" << endl;
            return;
        }

        Node* temp = top;
        while(temp != NULL) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

int main() {
    stacks s;

    s.push(10);
    s.push(20);
    s.push(30);

    s.pop();

    s.peek();
    s.disp();

    return 0;
}
