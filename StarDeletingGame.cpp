#include <fstream>
#include <iostream>
#include<vector>
#include<stdlib.h>
#include<memory.h>
#include<algorithm>
#define Trace(m) cout<<#m"="<<m;
#define Run(m) cout<<"Run:"#m"; \nResult:{";m;cout<<"}"<<endl;
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
#define NUM_ROWS 10
#define NUM_COLS 10
enum COLOR{
	NONE,RED,GREEN,BLUE,PURPLE,YELLOW
};

template<class T>
void printArr(vector<T>&arr){
	for(int i=0;i<arr.size();i++){
		arr[i].print();
	}
}
struct Pos{
	int r,c;
	void print(){
		cout<<"("<<r<<","<<c<<")";
	}
	Pos(){	
	}
	bool operator==(const Pos&p)const {
		return r==p.r&&c==p.c;
	}
	Pos(int tr,int tc):r(tr),c(tc){
	}
	Pos left(){
		return Pos(r,c-1);
	}
	Pos right(){
		return Pos(r,c+1);
	}
	Pos up(){
		return Pos(r+1,c);
	}
	Pos down(){
		return Pos(r-1,c);
	}
	bool operator<(const Pos &p)const{
		return getIndex()<p.getIndex();
	}
	
	int getIndex()const{
		return r*NUM_COLS+c;
	}
};
class Map{
	int map[NUM_ROWS][NUM_COLS];
	public:
		int getNumRest()const{
			int num=0;
			for(int i=0;i<NUM_ROWS;i++){
				for(int j=0;j<NUM_COLS;j++){
					if(map[i][j]!=0){
						num++;
					}
				}
			}
			return num;
		}
		bool operator==(const Map&m)const{
			for(int i=0;i<NUM_ROWS;i++){
				for(int j=0;j<NUM_COLS;j++){
					if(map[i][j]!=m.map[i][j])return false;
				}
			}
			return true;
		}
		Map(){
		}
		Map(Map&m){
			for(int i=0;i<NUM_ROWS;i++){
				for(int j=0;j<NUM_COLS;j++){
					map[i][j]=m.map[i][j];
				}
			}
		}
		void init(){
			for(int i=0;i<NUM_ROWS;i++){
				for(int j=0;j<NUM_COLS;j++){
					map[i][j]=0;
				}
			}
		}
		void testData(){
			init();
			map[9][0]=1;
			map[7][0]=1;map[7][2]=2;map[7][4]=1;map[7][5]=4;
			map[6][0]=1;map[6][1]=1;map[6][2]=1;
			map[2][0]=1;map[2][8]=1;
			map[1][0]=3;map[1][1]=3;map[1][3]=1;map[1][5]=1;
			map[0][0]=1;map[0][1]=3;map[0][2]=1;map[0][3]=1;
		}
		int get(int r,int c)const{
			return map[r][c];
		}

		void print(){
			//cout<<"Map(10,10)"<<endl;
			for(int i=-2;i<=NUM_ROWS;i++){
				for(int j=-2;j<=NUM_COLS;j++){
					if(i==-2||j==-2){
						if(i==-2&&0<=j&&j<NUM_COLS){							
							cout<<j;cout<<' ';
						}else if(j==-2&&0<=i&&i<NUM_ROWS){
							cout<<9-i;cout<<' ';
						}else{
							cout<<"  ";
						}
							
					}else 	if(i==-1||j==-1||i==NUM_ROWS||j==NUM_COLS){
						if(i==-1){
							if(j==-1){
								cout<<"**";
							}else if(j==NUM_COLS){
								cout<<"**";
							}else{
								cout<<"**";
							}
						}else if(i==NUM_ROWS){
							if(j==-1){
								cout<<"**";
							}else if(j==NUM_COLS){
								cout<<"**";
							}else{
								cout<<"**";
							}
						}else{
							cout<<"**";							
						}
					}else{					
						int t=map[NUM_ROWS-1-i][j];
						if(t==NONE){
							cout<<' ';
						}else{
							cout<<t;
						}
						cout<<' ';
					}
				}
				cout<<endl;
			}
		}
		
	
		void inputData(){
			//init();
			cout<<"NONE:0,RED:1,GREEN:2,BLUE:3,PURPLE:4,YELLOW:5"<<endl;			
				for(int j=0;j<NUM_COLS;j++){
					cout<<"next cols("<<j+1<<"):"<<endl;
					int i;
					for(i=0;i<NUM_ROWS;i++){
						int t;
						while(true){
							cin>>t;
							if(t<=5&&t>=-1)break;
							cout<<"Input gain row("<<i+1<<")=(0~5)"<<endl;
						}
						if(t==-1)break;						
						map[i][j]=t;
					}
					while(i<NUM_ROWS){
						map[i][j]=0;
						i++;
					}
				}	
		}
		void VHsqueen(){
			int tmap[NUM_ROWS][NUM_COLS];
			memset(&tmap[0][0],0,NUM_ROWS*NUM_COLS*sizeof(int));
			int tj=0;
				for(int j=0;j<NUM_COLS;j++){
					int ti=0;						
					for(int i=0;i<NUM_ROWS;i++){
						if(map[i][j]==0)continue;
						tmap[ti][tj]=map[i][j];
						ti++;
					}
					if(tmap[0][tj]!=0)tj++;
				}
			memcpy(&map[0][0],&tmap[0][0],NUM_ROWS*NUM_COLS*sizeof(int));	
		}
		vector<Pos>getLinkingPos(int r,int c){
			vector<Pos> pos;
			vector<Pos> edges;
			Pos start(r,c);
			//start
			edges.push_back(start);
			while(!edges.empty()){
				Pos t=edges.back();
				pos.push_back(t);
				edges.pop_back();
				//
				vector<Pos> newPs;
				newPs.push_back(t.left());
				newPs.push_back(t.right());
				newPs.push_back(t.up());
				newPs.push_back(t.down());
				for(int i=0;i<newPs.size();i++){
					Pos p=newPs[i];					
					if(p.r<0||p.r>=NUM_ROWS||p.c<0||p.c>=NUM_COLS)continue;
					if(map[p.r][p.c]!=map[t.r][t.c])continue;
				//	cout<<"test:";p.print();cout<<endl;
					if(find(edges.begin(),edges.end(),p)!=edges.end())continue;
					if(find(pos.begin(),pos.end(),p)!=pos.end())continue;
					//else~~~
				//	p.print();cout<<"had been added"<<endl;
					edges.push_back(p);
				}				
			}			
			return pos;
		}
		bool doAction(int r,int c){
			if(r<0||r>=NUM_ROWS||c<0||c>=NUM_COLS)return false;
			vector<Pos>p=getLinkingPos(r,c);
			if(p.size()<=1)return false;
			for(int i=0;i<p.size();i++){
				Pos t=p[i];
				map[t.r][t.c]=0;
			}
			VHsqueen();
			return true;
		}
		bool doActions(vector<Pos>&ps){
			bool res=true;
			for(int i=0;i<ps.size();i++){
				if(!doAction(ps[i].r,ps[i].c))res=false;
			}
			return res;
		}
		bool isEnd(){	
			
				for(int j=0;j<NUM_COLS;j++){
					if(map[0][j]==0)return true;
					for(int i=0;i<NUM_ROWS-1;i++){
						if(map[i][j]==0)continue;
						int cc=map[i][j];
						if(map[i+1][j]==cc)return false;
						if(j+1<NUM_COLS && map[i][j+1]==cc)return false;						
					}
				}
				return true;
		}
};
Pos getMinPos(vector<Pos>&ps){
	int min=ps[0].getIndex();
	int minI=0;
	for(int i=1;i<ps.size();i++){
		if(ps[i].getIndex()<min){
			minI=i;
			min=ps[i].getIndex();
		}
	}
	return ps[minI];
}
	struct Action{
		Pos p;
	};
	struct Node{
		Map map;
		Pos inAction;
		Node*parent;
		int mark;
		Node(){
			parent=0;
		}
		Node(Map&m){
			map=m;
			mark=map.getNumRest();
			parent=NULL;
		}
		Node(const Node&n){
			map=n.map;
			inAction=n.inAction;
			parent=n.parent;
			mark=n.mark;
		}
	/*	int getNumRest()const{
			return map.getNumRest();
		}*/
		bool operator<(const Node&n){
			return mark<n.mark;
		}
		vector<Node*>getSuccessives(){
			vector<Pos>ps=getActions();
			vector<Node*> nodes;
			nodes.resize(ps.size());			
			for(int i=0;i<ps.size();i++){
				nodes[i]=new Node();
				Node &node=*nodes[i];
				node.parent=this;
				node.map=map;
				node.inAction=ps[i];
				node.mark=node.map.getNumRest();
				node.map.doAction(node.inAction.r,node.inAction.c);
			}
			return nodes;
		}
		vector<Pos>getActions(){
			vector<Pos>actions;
			for(int j=0;j<NUM_COLS;j++){
				if(map.get(0,j)==0)break;
				for(int i=0;i<NUM_ROWS-1;i++){
					if(map.get(i,j)==0)continue;
					int cc=map.get(i,j);
					if(map.get(i+1,j)==cc){
						actions.push_back(Pos(i,j));
					}
					if(j+1<NUM_COLS && map.get(i,j+1)==cc){
						actions.push_back(Pos(i,j));
					}
				}
			}
			vector<Pos>tactions;
			for(int i=0;i<actions.size();i++){
				Pos a=actions[i];
				vector<Pos>ps=map.getLinkingPos(a.r,a.c);
				Pos p=getMinPos(ps);
				tactions.push_back(p);
			}
			actions.resize(0);
			sort(tactions.begin(),tactions.end());
			unique_copy(tactions.begin(),tactions.end(),back_inserter(actions));
			return actions;			
		}
	};
class Solution{
	Map map;
	vector<Pos>actions;	
	public:
	Solution(Map&m):map(m){		
	}
	void AStarSearch(Node&startNode){
		int counter=0;
		vector<Node*>close;
		vector<Node*>open;
		//
		vector<Node*>endNodes;
		//
		Node *pnode;		
		pnode=new Node(startNode);
		open.push_back(pnode);
		//
		while(!open.empty()&&counter<20000){
			if(++counter%100==0){
				cout<<counter<<",";
				cout.flush();
			}
			int minMark=open[0]->mark;
			int minI=0;
			for(int i=1;i<open.size();i++){
				if(open[i]->mark<minMark){
					minI=i;
					minMark=open[i]->mark;
				}
			}
			pnode=open[minI];
			open.erase(open.begin()+minI);
			close.push_back(pnode);
			/*
			//What wrong with the following code???!!!!
			vector<Node*>::iterator minI=min_element(open.begin(),open.end());
			pnode=*minI;
			open.erase(minI);			
			close.push_back(pnode);
			*/
			/*
			pnode=open.back();
			open.pop_back();
			close.push_back(pnode);*/
			//
			vector<Node*>successives=pnode->getSuccessives();
			if(successives.empty()){
				endNodes.push_back(pnode);
			}
			//
			//TEST::
			if(counter>10000&&successives.size()>2)successives.resize(2);
			for(int i=0;i<successives.size();i++){
				Node* t=successives[i];
				int j;
				for(j=0;j<close.size();j++){
					if(t->map==close[j]->map){
						t=NULL;
						break;
					}
				}
				if(t!=NULL){
				for(j=0;j<open.size();j++){
					if(t->map==open[j]->map){
						t=NULL;
						break;
					}
				}	
				}
				if(t!=NULL){
					open.push_back(t);			
				}else{
					delete successives[i];
				}
			}
		}
		for(int i=0;i<open.size();i++){
			endNodes.push_back(open[i]);
		}
		pnode=*min_element(endNodes.begin(),endNodes.end());
		while(1){		
			vector<Node*>nodes=pnode->getSuccessives();
			if(nodes.empty())break;
			int randI=rand()%nodes.size();
			pnode=nodes[randI];			
		}
		while(pnode->parent!=NULL){
			actions.push_back(pnode->inAction);
			pnode=pnode->parent;
		}
		reverse(actions.begin(),actions.end());
		
		//release memory
		for(int j=0;j<close.size();j++){
			delete close[j];
		}			
		/*		
		while(1){		
			vector<Node*>nodes=pnode->getSuccessives();
			if(nodes.empty())break;
			int randI=rand()%nodes.size();
			pnode=nodes[randI];			
		}
		while(pnode->parent!=NULL){
			actions.push_back(pnode->inAction);
			pnode=pnode->parent;
		}
		reverse(actions.begin(),actions.end());*/
	}
	void solve(){
		Node n(map);
		AStarSearch(n);		
	}
	void print(){
		cout<<"Solution:{"<<endl;
		printArr(actions);
		cout<<"}"<<endl;
		Map tmap=map;
		if(!tmap.doActions(actions)){
			cout<<"Error:something wrong with solution."<<endl;
		}
		cout<<"End map(rest:"<<tmap.getNumRest()<<"):";
		tmap.print();
		cout<<"End Solution"<<endl;				
	}
	
};
void playGame(Map&map){
	cout<<"start game."<<endl;
	while(!map.isEnd()){
		int r,c;
		map.print();
		cout<<"r:";cin>>r;
		cout<<"c:";cin>>c;
		map.doAction(r,c);
	}
	map.print();
	cout<<"game end."<<endl;
}
void solveGame(Map&map){
	Solution s(map);
	cout<<"Start solving..."<<endl;
	s.solve();
	cout<<"end solve."<<endl;
	s.print();
}
int main(int argc, char** argv) {
	Map map;
	map.inputData();
//	map.testData();
	map.VHsqueen();
	map.print();
	//
	cout<<"Solve(0) or play(1):"<<endl;
	int mode=0;
	cin>>mode;
	if(mode==0){
		solveGame(map);
	}else{
		playGame(map);
	}
	
	
	//
	
	
	//
	system("pause");
	return 0;
}
/*
Test Data:
2 1 5 3 4 5 3 4 5 3
3 1 2 2 2 4 1 2 2 3
5 5 4 1 3 5 3 2 4 1
5 4 2 3 3 4 4 5 1 2
3 1 2 2 1 2 4 3 1 4
5 3 3 5 3 3 1 3 5 3
3 4 4 1 3 1 5 2 4 5
3 5 1 1 2 3 4 4 1 2
4 3 5 4 4 5 4 4 1 3
2 5 5 1 4 4 5 3 2 1
0
*/
