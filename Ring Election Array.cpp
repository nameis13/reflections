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
    void bringBackToLife(node *a)
    {

    }
};

void sense(node *a, int lead, int num, bool alive);
void initiateElection (node *a, int lead, int num, int *leader);
void bringBackToLife(node *a, int ldrr);

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
    }

    // Network created, display the nodes

    for (i = 0; i<num; i++)
    A[i].disp(i, num);


    int counter = 1;
    int resp = 1, ldrr=0;
    int dead[num], dindex=-1, reborn, btolife=0;
    clock_t start[num];
    double duration;

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
                    sense(A, leader[1], num, false);
                    int dead[(++dindex)%num] = leader[1];
                    start[(++dindex)%num] = clock();
                    break;
                }
            else
                sense(A, leader[1], num, true);
            tmp = rand()%10;
        }

        // Leader has failed, initiate an election

        initiateElection(A, leader[1], num, leader);

        for (reborn = btolife; reborn<=dindex; reborn++)
        {
            if ((clock()-start[reborn])>5000)
            {
                tmp = dead[reborn];
                A[tmp].status = true;
                btolife = reborn;
            }
        }
     //   bringBackToLife(A, leader[1]);
        if (leader[1]==-1)
        {
            cout<<"\nAll nodes failed. Exiting.\n";
            exit(0);
        }
        else
        {
            tmp = leader[1];
            cout<<"\nNew Leader Node ["<<leader[1]<<"] "<<A[tmp].PID<<" : "<<leader[0]<<endl;
            cout<<"\nPress 1 to continue : ";
            cin>>resp;
            if ((clock()-start)>5000)
    //            bringBackToLife(A, );
            if (resp!=1)
                exit(0);
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
    int cmp;
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
        }
    cout<<"\n\n\tElection Finished!"<<endl;
    leader[0] = newLeader[0];
    leader[1] = newLeader[1];
}

void bringBackToLife(node *a, int ldrr)
{
    a[ldrr].status=true;
}
