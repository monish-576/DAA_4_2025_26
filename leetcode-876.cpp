class Solution {
public:
    ListNode* middleNode(ListNode* head) {
        ListNode *t1=head,*t2=head;
        int c=0;
        while(t1!=NULL)
        {
            c++;
            t1=t1->next;
        }
        int b=(c/2);
        while(b>0)
        {
            t2=t2->next;
            b--;
        }
        return t2;
    }
};