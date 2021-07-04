#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <climits>
#include <iterator>
using namespace std;

class Node{
	public:
		int vertex;
		Node* next;	
};	

Node* newNode(int);
bool existsEdge(Node*, int, int);
void createEdge(Node*, int, int, int);
void display(Node*, int);
vector<int> breadthFirstSearch(Node*, int, int);
int meet_join(Node*, int, int, int);
void maximal_minimal(Node*, int);
void deleteAdjList(Node*, int);


int main()
{
	int N,choice,x,y;
	
	cout<<"Enter number of elements in poset:"<<endl;
	cin>>N;
	
	Node* List1 = new Node[N]; 
	Node* List2 = new Node[N];
	
	for(int i=0; i<N; i++){
		List1[i].vertex = List2[i].vertex = i;
		List1[i].next = List2[i].next = NULL;
	}
	
	do{
		cout<<endl<<"MENU:"<<endl;
		cout<<"1. Add order x1 ≤ y1, x2 ≤ y2, x3 ≤ y3,...."<<endl;
		cout<<"2. Check if x ≤ y"<<endl;
		cout<<"3. Get Join(x,y)"<<endl; 
		cout<<"4. Get Meet(x,y)"<<endl;
		cout<<"5. Get Maximal elements"<<endl;
		cout<<"6. Get Minimal elements"<<endl;
		cout<<"7. View adjacency list"<<endl;
		cout<<"8. Exit"<<endl<<endl;
		
		cin>>choice;
		
		switch(choice)
		{
			case 1: {	
						cout<<"Enter x1,y1, x2,y2, x3,y3,.... respectively:"<<endl;
						char delimiter;
						while(scanf("%d%d%c", &x, &y, &delimiter)){
							if(x!=y && !existsEdge(List1, x, y)){
								createEdge(List1, N, x, y);
								createEdge(List2, N, y, x);
							}	
							if(delimiter=='\n') break;
						}
						break;
					}
		
			case 2: {
						cout<<"Enter x and y:"<<endl;
						cin>>x>>y;
						vector<int> rel = breadthFirstSearch(List1, N, x);
						if(find(rel.begin(), rel.end(), y) != rel.end())
							cout<<"Yes"<<endl;
						else cout<<"No"<<endl;
						break;
					}
					
			case 3: { 
						cout<<"Enter x and y:"<<endl;
						cin>>x>>y;
						int join = meet_join(List1, N, x, y);
						if(join==-1)
						cout<<"Join does not exist"<<endl;
						else cout<<"Join("<<x<<","<<y<<") = "<<join<<endl;
						break;
					}
					
			case 4: {
						cout<<"Enter x and y:"<<endl;
						cin>>x>>y;
						int meet = meet_join(List2, N, x, y);
						if(meet==-1)
						cout<<"Meet does not exist"<<endl;
						else cout<<"Meet("<<x<<","<<y<<") = "<<meet<<endl;
						break;
					}
					
			case 5: {
						cout<<"Maximal elements: ";
						maximal_minimal(List1, N);
						cout<<endl;
						break;
					}
			
			case 6: {
						cout<<"Minimal elements: ";
						maximal_minimal(List2, N);
						cout<<endl;
						break;
					}	
			
			case 7: {
						display(List1, N);
						break;
					}	
			
		}
		
	}while(choice>0 && choice <8);		
	
	deleteAdjList(List1, N);
	deleteAdjList(List2, N);
	
	return 0;
}	


Node* newNode(int v)
{
	Node* node = new Node;
	node->vertex = v;
	node->next = NULL;
	return node;
}


bool existsEdge(Node* L, int v1, int v2)
{	
	Node* temp = L[v1].next;
	if(!temp)
		return false;
	else{
		while(temp->next && temp->vertex < v2)
			temp = temp->next;
		if(temp->vertex != v2)
			return false;
	}
	return true;
}	


void createEdge(Node* L, int N, int v1, int v2)
{
	if(L[v1].next == NULL)
		L[v1].next = newNode(v2);
	
	else{
		Node* temp = L[v1].next;
		while(temp->next)
			temp = temp->next;
		temp->next = newNode(v2);
	}			
}


void display(Node* L, int N)
{
	cout<<"Adjacency list for cover relation"<<endl;
	for(int i=0; i<N; i++){
		
		cout<<L[i].vertex<<":→ ";
		Node* temp = L[i].next;
		if(temp){
			while(temp->next){
				cout<<temp->vertex<<"→ ";
				temp = temp->next;
			}
			cout<<temp->vertex<<endl;
		}
		else cout<<"NULL"<<endl;	
	}
}		
	
		
vector<int> breadthFirstSearch(Node* L, int N, int v)
{
	int d[N];
	queue<int> Q; 
	vector<int> bfs;
	
	for(int i=0; i<N; i++)
		d[i]= INT_MAX;
	d[v]= 0;
	Q.push(v);
	
	while(!Q.empty()){
		
		int u = Q.front();
		Q.pop();
		bfs.push_back(u);
		
		Node* temp= L[u].next;
		while(temp){
			if(d[temp->vertex] == INT_MAX){
				d[temp->vertex] = d[u]+1;
				Q.push(temp->vertex);
			}
			temp = temp->next;
		}
	}
	return bfs;		
}


int meet_join(Node* L, int N, int v1, int v2)
{
	vector<int> bfs1 = breadthFirstSearch(L, N, v1);
	vector<int> bfs2 = breadthFirstSearch(L, N, v2);
	
	sort(bfs1.begin(),bfs1.end());
	sort(bfs2.begin(),bfs2.end());
	
	vector<int> V;
	set_intersection(bfs1.begin(), bfs1.end(), 
					 bfs2.begin(), bfs2.end(), 
					 back_inserter(V));
	
	int i,j,size = V.size();
	vector<int> inBlack(size, 0);
	
	for(i=0; i<size; i++){
		for(j=0; j<size; j++){
			if(i!=j && existsEdge(L, V[j], V[i]))
					inBlack[i]++;
		}
	}			
	
	if(count(inBlack.begin(),inBlack.end(),0) != 1)
		return -1;
		
	vector<int>::iterator it = find(inBlack.begin(),inBlack.end(),0);
	return V[it-inBlack.begin()];
}	


void maximal_minimal(Node* L, int N)
{
	for(int i=0; i<N; i++){
		if(!(L[i].next))
			cout<<i<<" ";
	}		
}


void deleteAdjList(Node* L, int N)
{
	for(int i=0; i<N; i++){								 
		
		if(L[i].next){
			Node *temp = L[i].next, *ntemp;
			while(temp){
				ntemp = temp->next;
				delete(temp);
				temp = ntemp;
			}
		}
	}
	delete(L);
}


