
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cctype>
#include <limits>
using namespace std;

struct Event
{
    int eventID;
    string eventName;
    string participantName;
    char phone[11];
    Event* prev;
    Event* next;
};

Event* head = NULL;
Event* tail = NULL;

bool validName(string s)
{
    if(s.empty()) return false;
    for(size_t i=0;i<s.length();i++)
        if(!(isalpha(s[i]) || s[i]==' '))
            return false;
    return true;
}

bool validPhone(char p[])
{
    if(strlen(p)!=10) return false;
    for(int i=0;i<10;i++)
        if(!isdigit(p[i])) return false;
    return true;
}

int getInt(string msg)
{
    int x;
    while(true)
    {
        cout<<msg;
        if(cin>>x) return x;
        cout<<"Invalid input. Numbers only.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
    }
}

string getName(string msg)
{
    string s;
    while(true)
    {
        cout<<msg;
        getline(cin>>ws,s);
        if(validName(s)) return s;
        cout<<"Letters only. Try again.\n";
    }
}

void getPhone(char p[])
{
    while(true)
    {
        cout<<"Enter Phone Number (10 digits): ";
        cin>>p;
        if(validPhone(p)) return;
        cout<<"Invalid phone number.\n";
    }
}

bool duplicateID(int id)
{
    Event* t=head;
    while(t)
    {
        if(t->eventID==id) return true;
        t=t->next;
    }
    return false;
}

bool login()
{
    string user,pass;

    cout<<"========================================\n";
    cout<<" EVENT REGISTRATION MANAGEMENT SYSTEM\n";
    cout<<"========================================\n\n";

    for(int i=1;i<=3;i++)
    {
        cout<<"Enter Username: ";
        cin>>user;

        cout<<"Enter Password: ";
        cin>>pass;

        if(user=="admin" && pass=="1234")
        {
            cout<<"\nLogin Successful!\n";
            return true;
        }

        cout<<"Invalid Username or Password.\n";
    }
    return false;
}

Event* createEvent()
{
    Event* e=new Event;

    do
    {
        e->eventID=getInt("Enter Event ID: ");
        if(duplicateID(e->eventID))
            cout<<"Duplicate Event ID not allowed.\n";
    }while(duplicateID(e->eventID));

    e->eventName=getName("Enter Event Name: ");
    e->participantName=getName("Enter Participant Name: ");
    getPhone(e->phone);

    e->prev=NULL;
    e->next=NULL;

    return e;
}

void insertBeginning()
{
    Event* e=createEvent();

    if(head==NULL)
        head=tail=e;
    else
    {
        e->next=head;
        head->prev=e;
        head=e;
    }
}

void insertEnd()
{
    Event* e=createEvent();

    if(head==NULL)
        head=tail=e;
    else
    {
        tail->next=e;
        e->prev=tail;
        tail=e;
    }
}

void insertPosition()
{
    int pos=getInt("Enter Position: ");

    if(pos<=1 || head==NULL)
    {
        insertBeginning();
        return;
    }

    Event* e=createEvent();
    Event* t=head;
    int count=1;

    while(t->next!=NULL && count<pos-1)
    {
        t=t->next;
        count++;
    }

    if(t->next==NULL)
    {
        t->next=e;
        e->prev=t;
        tail=e;
    }
    else
    {
        e->next=t->next;
        e->prev=t;
        t->next->prev=e;
        t->next=e;
    }
}

void deleteBeginning()
{
    if(head==NULL) return;

    Event* t=head;
    head=head->next;

    if(head) head->prev=NULL;
    else tail=NULL;

    delete t;
}

void deleteEnd()
{
    if(tail==NULL) return;

    Event* t=tail;
    tail=tail->prev;

    if(tail) tail->next=NULL;
    else head=NULL;

    delete t;
}

void deletePosition()
{
    int pos=getInt("Enter Position To Delete: ");

    if(pos==1)
    {
        deleteBeginning();
        return;
    }

    Event* t=head;
    int count=1;

    while(t && count<pos)
    {
        t=t->next;
        count++;
    }

    if(!t) return;

    if(t==tail)
    {
        deleteEnd();
        return;
    }

    t->prev->next=t->next;
    t->next->prev=t->prev;
    delete t;
}

void displayForward()
{
    Event* t=head;

    while(t)
    {
        cout<<"\nID: "<<t->eventID;
        cout<<"\nEvent: "<<t->eventName;
        cout<<"\nParticipant: "<<t->participantName;
        cout<<"\nPhone: "<<t->phone<<"\n";
        t=t->next;
    }
}

void displayBackward()
{
    Event* t=tail;

    while(t)
    {
        cout<<"\nID: "<<t->eventID;
        cout<<"\nEvent: "<<t->eventName;
        cout<<"\nParticipant: "<<t->participantName;
        cout<<"\nPhone: "<<t->phone<<"\n";
        t=t->prev;
    }
}

void searchID()
{
    int id=getInt("Enter Event ID: ");
    Event* t=head;

    while(t)
    {
        if(t->eventID==id)
        {
            cout<<"Found: "<<t->participantName<<endl;
            return;
        }
        t=t->next;
    }
}

void updateRecord()
{
    int id=getInt("Enter Event ID To Update: ");
    Event* t=head;

    while(t)
    {
        if(t->eventID==id)
        {
            t->eventName=getName("New Event Name: ");
            t->participantName=getName("New Participant Name: ");
            getPhone(t->phone);
            return;
        }
        t=t->next;
    }
}

void sortRecords()
{
    for(Event* i=head;i;i=i->next)
        for(Event* j=i->next;j;j=j->next)
            if(i->eventID>j->eventID)
            {
                swap(i->eventID,j->eventID);
                swap(i->eventName,j->eventName);
                swap(i->participantName,j->participantName);

                char temp[11];
                strcpy(temp,i->phone);
                strcpy(i->phone,j->phone);
                strcpy(j->phone,temp);
            }
}

void generateReport()
{
    ofstream file("EventReport.txt");
    Event* t=head;

    while(t)
    {
        file<<t->eventID<<" | "<<t->eventName<<" | "
            <<t->participantName<<" | "<<t->phone<<"\n";
        t=t->next;
    }
    file.close();
}

int main()
{
    if(!login()) return 0;

    int choice;

    do
    {
        cout<<"\n========== EVENT REGISTRATION MANAGEMENT SYSTEM ==========\n";
        cout<<"1. Insert at Beginning\n";
        cout<<"2. Insert at End\n";
        cout<<"3. Insert at Specific Position\n";
        cout<<"4. Delete at Beginning\n";
        cout<<"5. Delete at End\n";
        cout<<"6. Delete at Specific Position\n";
        cout<<"7. Search Record by Event ID\n";
        cout<<"8. Update Record\n";
        cout<<"9. Display Forward\n";
        cout<<"10. Display Backward\n";
        cout<<"11. Sort by Event ID\n";
        cout<<"12. Generate Report File\n";
        cout<<"13. Exit\n";

        choice=getInt("Enter Your Choice: ");

        switch(choice)
        {
            case 1: insertBeginning(); break;
            case 2: insertEnd(); break;
            case 3: insertPosition(); break;
            case 4: deleteBeginning(); break;
            case 5: deleteEnd(); break;
            case 6: deletePosition(); break;
            case 7: searchID(); break;
            case 8: updateRecord(); break;
            case 9: displayForward(); break;
            case 10: displayBackward(); break;
            case 11: sortRecords(); break;
            case 12: generateReport(); cout<<"Report Generated.\n"; break;
            case 13: cout<<"Thank You For Using The System.\n"; break;
            default: cout<<"Invalid Choice.\n";
        }

    }while(choice!=13);

    return 0;
}
