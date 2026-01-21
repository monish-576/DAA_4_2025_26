#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) {
        val = x;
        next = NULL;
    }
};

class LinkedList {
public:
    ListNode* head;

    LinkedList() {
        head = NULL;
    }

    void insertFront(int val) {
        ListNode* node = new ListNode(val);
        node->next = head;
        head = node;
    }

    void insertBack(int val) {
        ListNode* node = new ListNode(val);
        if (head == NULL) {
            head = node;
            return;
        }
        ListNode* temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = node;
    }

    void deleteVal(int val) {
        if (head == NULL) return;

        if (head->val == val) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        ListNode* curr = head;
        while (curr->next != NULL && curr->next->val != val)
            curr = curr->next;

        if (curr->next != NULL) {
            ListNode* temp = curr->next;
            curr->next = temp->next;
            delete temp;
        }
    }

    void printList() {
        ListNode* temp = head;
        while (temp != NULL) {
            cout << temp->val << " -> ";
            temp = temp->next;
        }
        cout << "NULL\n";
    }
};

int main() {
    LinkedList ll;

    ll.insertBack(10);
    ll.insertBack(20);
    ll.insertFront(5);
    ll.insertBack(30);

    ll.printList();

    ll.deleteVal(10);
    ll.printList();

    return 0;
}
