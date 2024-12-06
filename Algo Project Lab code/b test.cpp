#include<iostream>
#include<vector>
#include<queue>
#include<climits>
using namespace std;

#define V 5
typedef pair<int, int> iPair;
void printMST(int parent[], int n, int graph[V][V]){
cout<<"Edge \tWeight\n";
for(int i=1; i<n;i++)
cout<<parent[i]<< " -- "<< i <<"\t"<<graph[i][parent[i]]<<"\n";
}
void primMST(int graph[V][V]){
int parent[V];
int key[V];
bool mstSet[V];
for(int i=0; i<V; i++){
key[i]= INT_MAX;
mstSet[i]= false;
}

key[0]=0;
parent[0]=-1;
priority_queue<iPair, vector<iPair>,greater<iPair>> pq;
pq.push(make_pair(0, 0));
while(!pq.empty()){
int u = pq.top().second;
pq.pop();
mstSet[u]=true;
for(int v=0; v<V; v++){
if(graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
key[v] = graph[u][v];
parent[v] = u;
pq.push(make_pair(key[v], v));
}
}
}
printMST(parent, V, graph);
}
int main() {
int graph[V][V]={
{0,2,0,6,0},
{2,0,3,8,5},
{0,3,0,0,7},
{6,8,0,0,9},
{0,5,7,9,0}
};
primMST(graph);
return 0;
}