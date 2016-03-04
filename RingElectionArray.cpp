#include<bits/stdc++.h>
#include<iostream>
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include<fstream>
#include <cstdio>
#include <ctime>
#define MAX 10

using namespace std;

int eMsg[MAX];

class node
{
    public:
    int priority;
    bool status;
    char *PID;
    int leaderIndex;
    void initial (bool rnd, int n)
    {
        if (rnd == true)
        priority = rand()%(10*n);
        else
        {
            cout<<"\nEnter priority: ";
            cin>>priority;
        }
        PID = (char *)malloc(sizeof(char)*5);
        int t = rand()%26,i;
        PID[0] = t + 65;
        for (i = 1; i<4; i++)
        {
            t = rand()%10;
            PID[i] = t + 48;
        }
        PID[i] = '\0';
        status = true;
    }

    void disp(int i, int n)
    {
        cout<<"\n["<<i<<"] "<<PID<<" : "<<priority;
        cout<<endl;
    }

    int tellLeader (int i, int j)
    {
        cout<<"\nNode "<<i<<" asks "<<j<<" for the current leader!\n";
        return leaderIndex;
    }
};

void initializeNodes(node *a, int num, bool p, bool *oldStatus, int *leader, clock_t *start);
void sense(node *a, int lead, int num, bool alive);
void upAgain(node *A, int num, bool *oldStatus);
void initiateElection (node *a, int lead, int num, int *leader);
void backToLife(node *A, int num, clock_t *start, int *randt);
void randomNodeFailure (node *a, int sz, clock_t *ck);
void setLeader(node *A, int *leader, int num);
void waitToFail(node *A, int *leader, int num, bool *oldStatus, clock_t *start, int *randt);

int main()
{
    // Input Nodes and set value
    srand(time(NULL));
    int num, i, pr, tmp;
    cout<<"Enter the number of nodes: ";
    cin>>num;

    clock_t *start = (clock_t *)malloc(sizeof(clock_t)*num);

    cout<<"Random priority? [1] True else False: ";
    cin>>pr;

    bool p;
    if (pr==1)
        p = true;
    else
        p = false;

    class node *A = (node *)malloc(sizeof(node)*num);
    int *leader = (int *)malloc(sizeof(int)*2);
    leader[0] = 0;
    leader[1] = 0;
    bool *oldStatus = (bool *)malloc(sizeof(bool)*num);

    initializeNodes(A, num, p, oldStatus, leader, start);

    // Network created, display the nodes

    for (i = 0; i<num; i++)
    {
        A[i].disp(i, num);
        A[i].leaderIndex = leader[1];
    }

    int counter = 1;
    int resp = 1;
    int *randt = (int *)malloc(sizeof(int)*num);

    // Simulate election

    while (resp == 1)
    {
        if (counter==1)
            cout<<"\nIntitial Leader : Node "<<leader[1]<<" : "<<leader[0]<<endl;
        else
            cout<<"\nCurrent Leader : Node "<<leader[1]<<" : "<<leader[0]<<endl;
        tmp = rand()%10;


        waitToFail(A, leader, num, oldStatus, start, randt);    // Waiting for the leader to fail
        randomNodeFailure(A, num, start);                       // Fail a random node
        upAgain(A, num, oldStatus);                             // Find nodes back to live again
        initiateElection(A, leader[1], num, leader);            // Initiate election

        if (leader[1]==-1)
        {
            cout<<"\nAll nodes failed. Exiting.\n";
            exit(0);
        }
        else
        {
            tmp = leader[1];
            cout<<"\nNew Leader Node ["<<leader[1]<<"] "<<A[tmp].PID<<" : "<<leader[0]<<endl;
            backToLife(A, num, start, randt);
            setLeader(A, leader, num);
            cout<<"\nPress 1 to continue : ";
            cin>>resp;
            if (resp!=1)
                exit(0);
            else
                cout<<"\n--------------------------------"<<endl;
        }
    }
    return 0;
}

void sense(node *a, int lead, int num, bool alive)
{
    int i = 0, cmp = lead-1;
    if (cmp<0)
        i = num-1;
    else
        i = cmp;
    cout<<"\nNode ["<<i<<"] "<<a[i].PID<<" : "<<a[i].priority<<" senses ["<<(i+1)%num<<"] ";
    cout<<a[(i+1)%num].PID<<" : "<<a[(i+1)%num].priority;
    if (alive == true)
        cout<<" as alive"<<endl;
    else
        {
            cout<<" as dead"<<endl;
            a[(i+1)%num].status = false;
        }
}

void initiateElection (node *a, int lead, int num, int *leader)
{
    cout<<"\n\tElection Initiated!\n";
    int currLeader = lead, i,ctr = 0, index = 0;
    int *newLeader = (int *)malloc(sizeof(int)*2);
    newLeader[0] = -1;
    newLeader[1] = -1;
    int cmp, al=0;
    cmp = lead - 1;
    if (cmp<0)
        cmp = num - 1;
    if (lead!=0)
        i = lead - 1;
    else
        i = num - 1;
    cout<<"Election Message : ";
    for (; ;i++,index++)
        {
            al++;
            i = i%num;
            if (i==cmp)
                {
                    ctr++;
                }
            if (ctr>1)
                break;
            if (a[i].status == true)
            {
                eMsg[index] = a[i].priority;
                if (a[i].priority>newLeader[0])
                {
                    newLeader[0] = a[i].priority;
                    newLeader[1] = i;
                }
                cout<<a[i].priority<<"\t";
            }
            else
                cout<<a[i].priority<<" [Dead]\t";
            if (al%4==0)
                cout<<endl;
        }
    cout<<"\n\n\tElection Finished!"<<endl;
    leader[0] = newLeader[0];
    leader[1] = newLeader[1];
}

void randomNodeFailure (node *a, int sz, clock_t *ck)
{
    int i = rand()%4;
    if (i==0)
    {
        X:
        i = rand()%sz;
        if (a[i].status)
            {
                a[i].status = false;
                ck[i] = clock()/100;
            }
        else
            goto X;
    }
}

void initializeNodes(node *A, int num, bool p, bool *oldStatus, int *leader, clock_t *start)
{
    int i,j;
    for (i = 0; i<num; i++)
    {
        A[i].initial(p, num);
        start[i] = 0;
        for (j = 0; j<i; j++)
        {
            if (A[j].priority==A[i].priority)
            {
                if (p==false)
                    cout<<"\nSorry, multiple nodes can't have the same priority!\n";
                A[i].initial(p, num);
                j=0;
            }
        }

        if (!p && A[i].priority<0)
            {
                cout<<"Illegal priority. Exiting\n";
                exit(0);
            }

        if (A[i].priority>leader[0])
        {
            leader[0] = A[i].priority;
            leader [1] = i;
        }

        oldStatus[i] = true;
    }
}

void upAgain(node *A, int num, bool *oldStatus)
{
    for (int i = 0; i<num; i++)
        {
            if (oldStatus[i]==false && A[i].status==true)
            {
                cout<<"\n\n\t\t***BREAKING NEWS***\n";
                cout<<"\tNode ["<<i<<"] "<<A[i].PID<<" : "<<A[i].priority<<" is back to LIVE!"<<endl;
            }
            oldStatus[i]=A[i].status;
        }
}

void backToLife(node *A, int num, clock_t *start, int *randt)
{
    for (int i = 0; i<num; i++)
            {
                if (start[i]!=0)
                {
                    if (((clock()/100) - start[i])>randt[i])
                    {
                        A[i].status = true;
                        start[i] = 0;
                    }
                }
            }
}

void setLeader(node *A, int *leader, int num)
{
    for (int i = 0; i<num; i++)
        {
            if (A[i].status)
                A[i].leaderIndex = leader[1];
        }
}

void waitToFail(node *A, int *leader, int num, bool *oldStatus, clock_t *start, int *randt)
{
    int tmp;
    while (true)
        {
            if (tmp == 0)
                {
                    sense(A, leader[1], num, false);
                    tmp = leader[1];
                    oldStatus[tmp] = false;
                    A[tmp].leaderIndex = -1;
                    start[tmp] = clock()/100;
                    randt[tmp] = (3 + rand()%8)*10;
                    break;
                }
            else
                sense(A, leader[1], num, true);
            tmp = rand()%10;
        }
}
