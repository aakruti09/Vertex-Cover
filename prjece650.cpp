#include <memory>
#include <iostream>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <sstream>
#include <regex>
#include <vector>
#include <stddef.h>
#include <pthread.h>
using namespace Minisat;
using namespace std;
int v;
vector<vector<int>> e;

void print1d(vector<int> v){
  int l=v.size();
  for(int i=0;i<l;i++){
    cout<<v[i]<<"\t";
  }
  cout<<endl;
}

void print2d(vector<vector<int>> vec){
  int gi=vec.size();
  for (int i=0;i<gi;i++){
    int gii=vec[i].size();
    for (int j=0;j<gii;j++){
      cout<<vec[i][j]<<"\t";
    }
    cout<<endl;
  }
}

void printout(string name,vector<int> p){
  cout<<name<<": ";
  int psize=(int)p.size();
  for(int i=0;i<psize;i++){
    if(i==(psize-1)){
      cout<<p[i];
    }
    else{
      cout<<p[i]<<",";
    }
  }
  cout<<endl;
}

void *reduce_to_cnf(void *) {
  unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
  int n=v;
  bool res3=false;
  vector<int> ans;
  
  for(int k=1;k<=n;k++){
    // cout<<"\n\nk : "<<k<<endl;
    vector<vector<Lit>> l(n,vector<Lit>(k));
    Minisat::vec<Lit> t;
  
    //Seting new model literals
    for(int i=0;i<n;i++){
      for(int j=0;j<k;j++){
	l[i][j]=Minisat::mkLit(solver->newVar());
      }
    }

    //Clause 1 adding
    // cout<<"In clause 1"<<endl;
    for(int i=0;i<k;i++){
      t.clear();
      for(int j=0;j<n;j++){
	t.push(l[j][i]);
	// cout<<"l["<<j+1<<"]["<<i+1<<"] ";
      }
      // cout<<endl;
      solver->addClause(t);
    }
    /*bool res=solver->solve();
    // cout<<"Result is : "<<res<<endl;
    for(int i=0;i<n;i++){
      for(int j=0;j<k;j++){
	if(!toInt(solver->modelValue(l[i][j]))){
	  cout<<"l["<<i+1<<"]["<<j+1<<"] : "<<"True\t";
	}
	else{
	  cout<<"l["<<i+1<<"]["<<j+1<<"] : "<<"False\t";
	}	  
      }
    }
    cout<<endl;*/

    //Clause 2 adding
    // cout<<"In clause 2"<<endl;
    for(int i=0;i<n;i++){
      for (int j=0;j<k;j++){
	for(int m=0;m<k;m++){
	  if(j<m){
	    solver->addClause(~l[i][j],~l[i][m]);
	    // cout<<"~l["<<i+1<<"]["<<j+1<<"],~l["<<i+1<<"]["<<m+1<<"]"<<endl;
	  }
	}
      }
    }
    /*bool res1=solver->solve();
    cout<<"Result after 2 set of clauses: "<<res1<<endl;
    for(int i=0;i<n;i++){
      for(int j=0;j<k;j++){
	if(!toInt(solver->modelValue(l[i][j]))){
	  cout<<"l["<<i+1<<"]["<<j+1<<"] : "<<"True\t";
	}
	else{
	  cout<<"l["<<i+1<<"]["<<j+1<<"] : "<<"False\t";
	}	  
      }
    }
    cout<<endl;*/


    //Clause 3 adding
    // cout<<"In clause 3"<<endl;
    for(int i=0;i<n;i++){
      //cout<<"i:"<<i<<endl;
      for (int j=0;j<n;j++){
	//cout<<"\tj:"<<j<<endl;
	if(i<j){
	  for(int m=0;m<k;m++){
	    //cout<<"\t\tm:"<<m;
	    solver->addClause(~l[i][m],~l[j][m]);
	    // cout<<"~l["<<i+1<<"]["<<m+1<<"],~l["<<j+1<<"]["<<m+1<<"]"<<endl;
	  }
	}
      }
    }
    /*bool res2=solver->solve();
    cout<<"Result after clauses 3 : "<<res2<<endl;
    for(int i=0;i<n;i++){
      for(int j=0;j<k;j++){
	if(!toInt(solver->modelValue(l[i][j]))){
	  cout<<"l["<<i+1<<"]["<<j+1<<"] : "<<"True\t";
	}
	else{
	  cout<<"l["<<i+1<<"]["<<j+1<<"] : "<<"False\t";
	}	  
      }
    }
    cout<<endl;*/  

  
    //Clause 4 adding
    // cout<<"In clause 4"<<endl;
    int es=(int)e.size();
    for(int i=0;i<es;i++){
      // cout<<e[i][0]<<"\t";
      // cout<<e[i][1]<<endl;
      t.clear();
      for(int j=0;j<k;j++){
	t.push(l[e[i][0]][j]);
	t.push(l[e[i][1]][j]);
	// cout<<"l["<<e[i][0]+1<<"]["<<j+1<<"] l["<<e[i][1]+1<<"]["<<j+1<<"]"<<" ";
      }
      // cout<<endl;
      solver->addClause(t);
    }
  
    res3=solver->solve();
    // cout<<"Result after clauses 4 : "<<res3<<endl;
    /*for(int i=0;i<n;i++){
      for(int j=0;j<k;j++){
	if(!toInt(solver->modelValue(l[i][j]))){
	  cout<<"l["<<i+1<<"]["<<j+1<<"] : "<<"True\t";
	}
	else{
	  cout<<"l["<<i+1<<"]["<<j+1<<"] : "<<"False\t";
	}	  
      }
    }
    cout<<endl;*/

    if(res3==1){
      for(int i=0;i<n;i++){
	for(int j=0;j<k;j++){
	  if(!toInt(solver->modelValue(l[i][j]))){
	    ans.push_back(i);
	  }
	}
      }
      // cout<<"Answer is : "<<endl;
      sort(ans.begin(),ans.end());
      printout("CNF-SAT-VC",ans);
      // for(int i=0;i<int(ans.size());i++){
      // 	cout<<ans[i]<<" ";
      // }
      // if(ans.size()<=0)
      // 	cout<<" Blank"<<endl;
      // cout<<endl;
      break;
    }
    solver.reset(new Minisat::Solver());
  }// Big k loop ended
  if(res3==0){
    cout<<"CNF-SAT-VC: "<<endl;
  }
  return 0;
}//reduce to cnf ended

vector<vector<int>> make_G(int v,vector<vector<int>> e){
  int el=e.size();
  vector<vector<int>> g;
  for(int i=0;i<v;i++){
    vector<int> temp;
    for(int j=0;j<el;j++){
      if(e[j][0]==i)
	temp.push_back(e[j][1]);
      else if(e[j][1]==i)
	temp.push_back(e[j][0]);
    }
    g.push_back(temp);
  }
  return g;
}

string find_degree(vector<vector<int>> g){
  int max_degree=0,max_v=-1;
  int g_size=(int)g.size();
  for (int i=0;i<g_size;i++){
    int int_size=g[i].size();
    if (int_size>max_degree){
      max_degree=int_size;
      max_v=i;
    }
  }
  string s1=to_string(max_degree)+" "+to_string(max_v);
  //cout<<s1;
  return s1;
}



void *vc1(void *){
  vector<int> vc;
  vector<vector<int>> g=make_G(v,e);
  //cout<<"G created"<<endl;
  //print2d(g);
  int gs=(int)g.size();
  while(gs>0){
    gs--;
    string deg_str=find_degree(g);
    // cout<<"Returned max degree: "<<deg_str;
    istringstream hd(deg_str);
    int h_deg,h_ver;
    hd>>h_deg;
    hd>>h_ver;
    //cout<<"Highest degree : "<<h_deg
    //	<<"\tVertex : "<<h_ver<<endl;
    if(h_deg==0 and h_ver==-1){
      break;
    }
    vc.push_back(h_ver);
    //cout<<"Vertex cover:"<<endl;
    //print1d(vc);
    
    //g.erase(g.begin()+h_ver);
    // cout<<"After erasing G"<<endl;
    // print2d(g);
    
    for(int i=0;i<(int)g.size();i++){
      for(int j=0;j<(int)g[i].size();j++){
	if(i==h_ver){
	  g[i].clear();	 
	}
	if(g[i][j]==h_ver){
	  // cout<<"In g["<<i<<"]"<<"\t";
	  // cout<<"Proper index:"<<j<<endl;
	  g[i].erase(g[i].begin()+j);
	}
      }
    }
    // cout<<"After erasing all egdes related to vertex selected"<<endl;
    // print2d(g);
  }

  // cout<<"Vector Cover:"<<endl;
  // print1d(vc);
  std::sort(vc.begin(),vc.end());
  // cout<<"Sorted vector"<<endl;
  // print1d(vc);
  printout("APPROX-VC-1",vc);
  return 0;
}

void *vc2(void *){
  // cout<<"In vc2"<<endl;
  vector<vector<int>> ec(e);
  vector<int> vcc;
  while(ec.size()>0){
    int u=ec[0][0];
    int v=ec[0][1];
    // cout<<"u:"<<u<<"\tv:"<<v<<endl;
    vcc.push_back(u);
    vcc.push_back(v);
    // cout<<"Vertex Cover:"<<endl;
    // print1d(vc);
  
    for(int i=(int)ec.size()-1;i>=0;i--){
      // cout<<"i"<<i<<endl;
      // cout<<ec[i][0]<<","<<ec[i][1]<<endl;
      if(ec[i][0]==u || ec[i][1]==u || ec[i][0]==v || ec[i][1]==v){
	// cout<<"In if"<<ec[i][0]<<","<<ec[i][1]<<endl;
	ec.erase(ec.begin()+i);
	//e[i].clear(); 
      }
    }
    // cout<<"After removing edges are:"<<endl;
    // print2d(e);
    // cout<<"After removing edges ec : "<<endl;
    // print2d(ec);
  }
  // cout<<"Final vertex cover:"<<endl;
  // print1d(vc);
  sort(vcc.begin(),vcc.end());
  printout("APPROX-VC-2",vcc);
  return 0;
}


void *input_output(void *lval) {
  int *b=(int *)malloc(sizeof(int));
  string line=*(string *) lval;
  //cout<<"Line:"<<line<<endl;
   
  do
  {
    istringstream input(line);
    ws(input);
    char cmd;
    input>>cmd;

    if (cmd=='V'){
      string v1,v2;
      v2=line;
      istringstream vin(v2);
      ws(vin);
      char vt;
      vin>>vt;  //For V
      vin>>v1;  //For number
      regex vpat("([0-9]+)");
      if (regex_match (v1,vpat)){
	//cout<<"Matched"<<endl;
        if(v!=0){
	  e.clear();
	}
	ws(input);
	input>>v;
	if (!input.eof()){
	  cerr<<"Error: Invalid V command"<<endl;
	  *b=1;
	  pthread_exit(b);
	}
      }
      else{
	cout<<"Error: Enter valid integer number in V command"<<endl;
	v=0;
	e.clear();
	*b=1;
	pthread_exit(b);
      }
    } //If V completed
    else if (cmd=='E'){
      string l1,l2;
      l2=line;
      istringstream ein(l2);
      ws(ein);
      char ct;
      ein>>ct;
      ein>>l1;
      //cout<<"Line 1:"<<l1<<endl;
      regex pattern("^(\\{)((<)([0-9]+)(,)([0-9]+)(>))?((,)(<)([0-9]+)(,)([0-9]+)(>))*(})");
      if (regex_match (l1,pattern)){
	//cout<<"Matched"<<endl;
	e.clear(); 
	char charac;
	int n1,n2;
	ws(input);
	while(!input.eof()){
	  vector<int> temp;
	  input>>charac;  //For { , comma , }
	  if (charac=='}' || charac=='>'){   //exit
	    break;
	  }
	  else if(charac==',' || charac=='{'){
	    input>>charac;  //For < char
	    if (charac=='}')
	      break;
	  }
	  else {
	    cerr<<"Error: Invalid character : "<<charac;
	    break;
	  }
	  input>>n1;  //For first vertex of edge
	  input>>charac;  //For comma
	  input>>n2;  //For second vertex of edge
	  input>>charac;  //For > char
      
	  if(n2>=v || n1>=v || n2<0 || n1<0){
	    cerr<<"Error: Edges to unknown vertex"<<endl;
	    e.clear();
	    *b=1;
	    pthread_exit(b);
	    break;
	  }
	  else if (n1==n2){
	    cerr<<"Error: Edge to own vertex is invalid in undirected graph"<<endl;
	    e.clear();
	    *b=1;
	    pthread_exit(b);
	    break;
	  }
	  else{
	    temp.push_back(n1);
	    temp.push_back(n2);
	    e.push_back(temp);
	    //print1d(temp);
	  }
	}
        *b=0;
	pthread_exit(b);
      }
      else {
	cerr<<"Error: Invalid command"<<endl;
        *b=1;
	pthread_exit(b);
      }
    } //elif E completed
    else{
      cerr<<"Error: Invalid command"<<endl;
      *b=1;
      pthread_exit(b);
    }
  } while(getline(cin,line));
  return 0;
}



int main(){
  v=0;
  pthread_t t1,t2,t3,t4;
  string line;
  int flag=0;
  while(getline(cin,line))
  {
    void *ioreturn;
    pthread_create(&t1,nullptr,input_output,(void *)&line);
    pthread_join(t1,&ioreturn);
    int b=*((int *)ioreturn);
    free(ioreturn);
    //cout<<"Boolean value is: "<<b<<endl;
    if(b==0){
      // cout<<"Vertex:"<<v<<endl;
      // cout<<"Edges:"<<endl;
      // print2d(e);
      //if v and e are empty then dont call
      if((v>0) && ((int)e.size()>0)){
	struct timespec ts1;
	int er;
	pthread_create(&t2,nullptr,reduce_to_cnf,nullptr);
	//pthread_join(t1,nullptr);
	if (clock_gettime(CLOCK_REALTIME, &ts1) == -1) {
	  cerr<<"Error: clock realtime"<<endl;
	}
	ts1.tv_sec+=60;
	er=pthread_timedjoin_np(t2,nullptr,&ts1);
	if(er) {
	  cout<<"CNF-SAT-VC: timeout"<<endl;
	  flag=1;
	  //cerr<<"Error : "<<er<<endl;
	}
      }
      else if(flag==1){
	cout<<"CNF-SAT-VC: timeout"<<endl;
      }
      else{
	cout<<"CNF-SAT-VC: "<<endl;
      }
      pthread_create(&t3,nullptr,vc1,nullptr);
      pthread_join(t3,nullptr);
      pthread_create(&t4,nullptr,vc2,nullptr);
      pthread_join(t4,nullptr);
    }
  }
}
