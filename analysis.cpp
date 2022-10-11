#include <memory>
#include <iostream>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <sstream>
#include <regex>
#include <vector>
#include <stddef.h>
#include <pthread.h>
#include <fstream>
#include <math.h>
using namespace Minisat;
using namespace std;
int v;
vector<vector<int>> e;
ifstream fin;
vector<double> rcnf,acnf,scnf;
vector<double> rvc1,arvc1,srvc1;
vector<double> rvc2,arvc2,srvc2;
vector<int> xacnf,xcnf,xacv1,xcv1,xacv2,xcv2;
int p,q,r;

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
    cout<<"\t";
  }
  cout<<endl;
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

int approx_avg(vector<int> vi){
  int summ=0;
  for (int i=0;i<10;i++){
    summ+=vi[i];
  }
  //cout<<"Sum:"<<summ<<endl;
  int avg=summ/10;
  //cout<<"Approx Average:"<<avg<<endl;
  return avg;
}

double vavg(vector<double> vi){
  double summ=0;
  for(int i=0;i<vi.size();i++){
    summ+=vi[i];
  }
  double vavg1=summ/100.0;
  //cout<<"Average is "<<vavg1<<endl;
  return vavg1;
}

double stddev(vector<double> vi,double vavge){
  double diffsq=0;
  for(int i=0; i<vi.size(); i++){
    diffsq+=pow((vi[i]-vavge),2.0);
  }
  double std1=sqrt(diffsq/100);
  //cout<<"Standard deviation: "<<std1<<endl;
  return std1;
}

double runtime(pthread_t &tid, clockid_t &clk){
  double rtime;
  struct timespec ts;
  int r=pthread_getcpuclockid(tid,&clk);
  if(!r){
    int a=clock_gettime(clk,&ts);
    if(!a){
      int s=ts.tv_sec;
      double ms=ts.tv_nsec/1000000000.0;
      rtime=s+ms;
      cout<<"Rtime: "<<rtime<<"\t";
      cout// <<"Tid: "<<tid<<"\t"
	  <<"Seconds: "<<s<<"\t"
	  <<"Mili Seconds: "<<ms<<endl;
	//<<"Converting:"<<ms/1000.0<<endl;
	  // <<"MicroSeconds: "<<ts.tv_nsec*0.001<<"\t"
	  // <<"NanoSeconds: "<<ts.tv_nsec<<endl;
    }
    else if (a){
      cerr<<"Error: clock_gettime()"<<endl;
      return -1;
    }
  }
  else if(r){
    cerr<<"Error: pthread_getcpuclockid"<<endl;
  }
  return rtime;
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

  //approx ratio
  /*cout<<"P:"<<p<<endl;
  xacnf.push_back(ans.size());
  if(p==9){
    int got_avg=approx_avg(xacnf);
    xcnf.push_back(got_avg);
    // cout<<"Approx sizes for vertex "<<v<<endl;
    // print1d(xacnf);
    // cout<<"Got average:"<<got_avg;
    cout<<"Average approx vector:"<<endl;
    print1d(xcnf);
    xacnf.clear();
    p=0;
  }
  else
    p=p+1;
  */

  //Now runtime
  double rtemp3;
  clockid_t clk3;
  pthread_t p3=pthread_self();
    
  for(int i=0;i<10;i++){
    rtemp3=runtime(p3,clk3);
    rcnf.push_back(rtemp3);
  }
  cout<<"Size: "<<rcnf.size()<<endl;
  if(rcnf.size()==100){
    for(auto i=rcnf.begin();i!=rcnf.end();i++){
      cout<<*i<<"\t";
    }
    cout<<endl;
    double ans_avg3=vavg(rcnf);
    cout<<"Average cnf: "<<ans_avg3<<endl;
    acnf.push_back(ans_avg3);
    double ans_stddev3=stddev(rcnf,ans_avg3);
    cout<<"Standard Deviation cnf: "<<ans_stddev3<<endl;
    scnf.push_back(ans_stddev3);
    if(v==15){
      cout<<"All average of cnf"<<endl;
      for(auto i=acnf.begin();i!=acnf.end();i++){
	cout<<*i<<"\t";
      }
      cout<<"\nAll standard deviation of cnf"<<endl;
      for(auto i=scnf.begin();i!=scnf.end();i++){
	cout<<*i<<"\t";
      }
      cout<<endl;
    }
    rcnf.clear();
  }
  
  
} 
//reduce to cnf ended

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

  //approx ratio
  cout<<"Q:"<<q<<endl;
  xacv1.push_back(vc.size());
  if(q==9){
    int got_avg1=approx_avg(xacv1);
    xcv1.push_back(got_avg1);
    // cout<<"Approx sizes for vertex "<<v<<endl;
    // print1d(xacv1);
    // cout<<"Got average:"<<got_avg1<<endl;
    cout<<"Average approx vector:"<<endl;
    print1d(xcv1);
    xacv1.clear();
    q=0;
  }
  else
    q=q+1;
  

  /*if(rvc1.size()==100){
    rvc1.clear();
  }
  //Now runtime
  double rtemp;
  clockid_t clk;
  pthread_t p1=pthread_self();
    
  for(int i=0;i<10;i++){
    rtemp=runtime(p1,clk);
    rvc1.push_back(rtemp);
  }
  cout<<"Size: "<<rvc1.size()<<endl;
  if(rvc1.size()==100){
    for(auto i=rvc1.begin();i!=rvc1.end();i++){
      cout<<*i<<"\t";
    }
    cout<<endl;
    double ans_avg=vavg(rvc1);
    cout<<"Average : "<<ans_avg<<endl;
    arvc1.push_back(ans_avg);
    double ans_stddev=stddev(rvc1,ans_avg);
    cout<<"Standard Deviation : "<<ans_stddev<<endl;
    srvc1.push_back(ans_stddev);
    if(v==50){
      cout<<"All average of vc1"<<endl;
      for(auto i=arvc1.begin();i!=arvc1.end();i++){
	cout<<*i<<"\t";
      }
      cout<<"\nAll standard deviation of vc1"<<endl;
      for(auto i=srvc1.begin();i!=srvc1.end();i++){
	cout<<*i<<"\t";
      }
      cout<<endl;
    }
    }*/
}

void *vc2(void *){
  // cout<<"In vc2"<<endl;
  vector<vector<int>> ec(e);
  vector<int> vc2;
  while(ec.size()>0){
    int u=ec[0][0];
    int v=ec[0][1];
    //cout<<"u:"<<u<<"\tv:"<<v<<endl;
    vc2.push_back(u);
    vc2.push_back(v);
    // cout<<"Vertex Cover:"<<endl;
    // print1d(vc);
  
    for(int i=(int)ec.size()-1;i>=0;i--){
      //cout<<"i"<<i<<endl;
      //cout<<e[i][0]<<","<<e[i][1]<<endl;
      if(ec[i][0]==u || ec[i][1]==u || ec[i][0]==v || ec[i][1]==v){
	//cout<<"In if"<<e[i][0]<<","<<e[i][1]<<endl;
	ec.erase(ec.begin()+i);
	//e[i].clear(); 
      }
    }
    // cout<<"After removing edges are:"<<endl;
    // print2d(ec);
    // print2d(e);
  }
  // cout<<"Final vertex cover:"<<endl;
  // print1d(vc);
  sort(vc2.begin(),vc2.end());
  printout("APPROX-VC-2",vc2);

  //approx ratio
  cout<<"R:"<<r<<endl;
  xacv2.push_back(vc2.size());
  if(r==9){
    int got_avg2=approx_avg(xacv2);
    xcv2.push_back(got_avg2);
    // cout<<"Approx sizes for vertex "<<v<<endl;
    // print1d(xacv2);
    // cout<<"Got average:"<<got_avg2<<endl;
    cout<<"Average approx vector:"<<endl;
    print1d(xcv2);
    xacv2.clear();
    r=0;
  }
  else
    r=r+1;
  
  //Now runtime
  /*double rtemp2;
  clockid_t clk2;
  pthread_t p2=pthread_self();
    
  for(int i=0;i<10;i++){
    rtemp2=runtime(p2,clk2);
    rvc2.push_back(rtemp2);
  }
  cout<<"Size: "<<rvc2.size()<<endl;
  if(rvc2.size()==100){
    for(auto i=rvc2.begin();i!=rvc2.end();i++){
      cout<<*i<<"\t";
    }
    cout<<endl;
    double ans_avg2=vavg(rvc2);
    cout<<"Average vc2: "<<ans_avg2<<endl;
    arvc2.push_back(ans_avg2);
    double ans_stddev2=stddev(rvc2,ans_avg2);
    cout<<"Standard Deviation vc2: "<<ans_stddev2<<endl;
    srvc2.push_back(ans_stddev2);
    if(v==50){
      cout<<"All average of vc2"<<endl;
      for(auto i=arvc2.begin();i!=arvc2.end();i++){
	cout<<*i<<"\t";
      }
      cout<<"\nAll standard deviation of vc2"<<endl;
      for(auto i=srvc2.begin();i!=srvc2.end();i++){
	cout<<*i<<"\t";
      }
      cout<<endl;
    }
    rvc2.clear();
    }*/
}


void *input_output(void *lval) {
  int *b=(int *)malloc(sizeof(int));
  string line=*(string *) lval;
  int cnt=0;
  
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
	//cout<<"V "<<v<<endl;

	/*if (!input.eof()){
	  cerr<<"Error: Invalid V command"<<endl;
	  *b=1;
	  pthread_exit(b);
	  }*/
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
      cout<<line<<endl;
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
	// cout<<"Edges "<<endl;
	// print2d(e);
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
  } while(getline(fin,line));
}



int main(){
  v=0;
  pthread_t t1,t2,t3,t4;
  string line;
  
  fin.open("input.txt");
  int flag=0;
  while(getline(fin,line))
  {
    cout<<line<<endl;
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
      // if v and e are empty then dont call
     
      if((v>0) && ((int)e.size()>0) && flag==0){
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
      /*pthread_create(&t3,nullptr,vc1,nullptr);
      pthread_join(t3,nullptr); 
      pthread_create(&t4,nullptr,vc2,nullptr);
      pthread_join(t4,nullptr); */
    }
  }
  fin.close();
}
