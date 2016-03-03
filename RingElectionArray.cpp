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

    bool storeStatus()
    {
        return status;
    }
};

void sense(node *a, int lead, int num, bool alive);
void initiateElection (node *a, int lead, int num, int *leader);
void bringBackToLife(node *a, int ldrr);
void randomNodeFailure (node *a, int sz);

int main()
{
    // Input Nodes and set value
    srand(time(NULL));
    int num, i, pr, tmp;
    cout<<"Enter the number of nodes: ";
    cin>>num;

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
    int ind = 0,j;

    bool oldStatus[num];

    for (i = 0; i<num; i++,ind++)
    {
        A[i].initial(p, num);
        for (j = 0; j<i; j++)
        {
            if (A[j].priority==A[i].priority)
            {
                A[i].initial(p, num);
                j = 0;
            }
        }
        if (A[i].priority>leader[0])
        {
            leader[0] = A[i].priority;
            leader [1] = i;
        }
        oldStatus[i] = true;
    }

    // Network created, display the nodes

    for (i = 0; i<num; i++)
    {
        A[i].disp(i, num);
        A[i].leaderIndex = leader[1];
    }

    int counter = 1;
    int resp = 1, ldrr=0;
    int dead[num], dindex=-1, reborn, btolife=0, oldbtolife = -1;
    int randt;
    clock_t start[num];

    // Simulate election

    while (resp == 1)
    {
        if (counter==1)
            cout<<"\nIntitial Leader : Node "<<leader[1]<<" : "<<leader[0]<<endl;
        else
            cout<<"\nCurrent Leader : Node "<<leader[1]<<" : "<<leader[0]<<endl;
        tmp = rand()%10;
        ldrr = leader[1];

        // Waiting for the leader to fail

        while (true)
        {
            if (tmp == 0)
                {
                    dindex = (dindex + 1)%num;
                    sense(A, leader[1], num, false);
                    dead[dindex] = leader[1];
                    tmp = leader[1];
                    oldStatus[tmp] = false;
                    A[tmp].leaderIndex = -1;
                    start[dindex] = clock();
                    break;
                }
            else
                sense(A, leader[1], num, true);
            tmp = rand()%10;
        }

        // Find nodes back to live again
        for (i = 0; i<num; i++)
        {
            if (oldStatus[i]==false && A[i].status==true)
            {
                cout<<"\n\n\t***BREAKING NEWS***\n";
                cout<<"Node ["<<i<<"] "<<A[i].PID<<" : "<<A[i].priority<<" is back to LIVE!"<<endl;
            }
            oldStatus[i]=A[i].status;
        }

        // Initiate election
        initiateElection(A, leader[1], num, leader);

        if (leader[1]==-1)
        {
            cout<<"\nAll nodes failed. Exiting.\n";
            exit(0);
        }
        else
        {
            tmp = leader[1];
            cout<<"\nNew Leader Node ["<<leader[1]<<"] "<<A[tmp].PID<<" : "<<leader[0]<<endl;

            for (reborn = btolife; reborn>dindex; )
            {
                randt = (3 + rand()%5)*1000;
                if ((clock()-start[reborn])>randt)
                {
                    tmp = dead[reborn];
                    A[tmp].status = true;
                    oldbtolife = btolife;
                    btolife = reborn;
                    reborn = (reborn+1)%num;
                }
                else
                    break;
            }
            if (btolife<=dindex)
            {
                for (reborn = btolife; reborn<=dindex; )
                {
                    randt = (3 + rand()%5)*1000;
                    if ((clock()-start[reborn])>randt)
                    {
                        tmp = dead[reborn];
                        A[tmp].status = true;
                        oldbtolife = btolife;
                        btolife = reborn;
                        reborn = (reborn + 1)%num;
                    }
                    else
                        break;
                }
            }
            for (i = 0; i<num; i++)
            {
                if (A[i].status)
                A[i].leaderIndex = leader[1];
            }
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

void bringBackToLife(node *a, int ldrr)
{
    a[ldrr].status=true;
}

void randomNodeFailure (node *a, int sz)
{
    int i = rand()%4;
    if (i==0)
    {
        X:
        i = rand()%sz;
        if (a[i].status)
            a[i].status = false;
        else
            goto X;
    }
}
