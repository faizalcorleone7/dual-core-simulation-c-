#include<bits/stdc++.h> 		//n - independent processes on dual core
using namespace std;
typedef struct Time
{								//1 - > computation, 2 - > input/output
	int time,mode;
}t;
typedef struct process
{
	int id,totaltime;
	vector<t> tarr;
}p;
bool sortsjf(p p1,p p2)
{
	return p1.totaltime<p2.totaltime;
};
bool sortljf(p p1,p p2)
{
	return p1.totaltime>p2.totaltime;
};
p getp()
{
	int instances;
	p input;
	cout<<"Enter process ID: ";
	cin>>input.id;
	input.totaltime=0;
	cout<<"Give number of instances: ";
	cin>>instances;
	for (int i=0;i<instances;i++)
	{
		t inputt;
		cout<<"Give input for mode: ";
		cin>>inputt.mode;
		cout<<"Give input for time: ";
		cin>>inputt.time;
		input.totaltime=input.totaltime+inputt.time;
		input.tarr.push_back(inputt);
	}		
	return input;
};
int cruntime(p p1)
{
	if (p1.tarr[0].mode==2)	
	{
		return 0;
	}
	return p1.tarr[0].time;
};
p min1(p p1,p p2)			 //min time spent in cpu component
{
	if (p1.tarr[0].mode==p2.tarr[0].mode)	
	{
		if (p1.tarr[0].time<p2.tarr[0].time)
		{
			return p1;
		}
		return p2;
	}
	else
	{
		if (p1.tarr[0].mode==2)
		{
			return p1;
		}
		else
		{
			return p2;
		}
	}
};
p max1(p p1,p p2)			 //max time spent in cpu component
{
	if (p1.tarr[0].mode==p2.tarr[0].mode)	
	{
		if (p1.tarr[0].time>p2.tarr[0].time)
		{
			return p1;
		}
		return p2;
	}
	else
	{
		if (p1.tarr[0].mode==1)
		{
			return p1;
		}
		else
		{
			return p2;
		}
	}
};
void print(p p1)
{
	cout<<"p1.id = "<<p1.id<<endl;
	cout<<"p1.totaltime = "<<p1.totaltime<<endl;	
	cout<<"p1.mode = "<<p1.tarr[0].mode<<endl;
	cout<<"p1.time = "<<p1.tarr[0].time<<endl;
};
int calruntime(vector<p> v)
{
	int s=0;
	for (int i=0;i<v.size();i++)
	{
		for (int j=0;j<v[i].tarr.size();j++)
		{
		if (v[i].tarr[j].mode==1)
		{s=s+v[i].tarr[j].time;}
		}
	}
	return s;
};
int calwaittime(vector<p> v)
{
	int s=0;
	for (int i=0;i<v.size();i++)
	{
		for (int j=0;j<v[i].tarr.size();j++)
		{
		if (v[i].tarr[j].mode==2)
		{s=s+v[i].tarr[j].time;}
		}
	}
	return s;
};
int status1,status2;
int run(p &cpu,int &status)	//status = 1 -> directly to waiting queue, 2 -> process terminates in cpu, 3 -> normal case i.e. sending to waiting queue
{
	if (cpu.tarr[0].mode==2)
	{
		status=1;
		return 0;	
	}	
	else if (cpu.tarr.size()==1)
	{
		status=2;
		int time1=cpu.tarr[0].time;
		cpu.totaltime=0;
		cpu.tarr.erase(cpu.tarr.begin());
		return time1;	
	}
	else
	{
		status=3;		
		int time1=cpu.tarr[0].time;
		cpu.totaltime=cpu.totaltime-cpu.tarr[0].time;
		cpu.tarr.erase(cpu.tarr.begin());
		return time1;
	}
};
vector<p> status_read(vector<p> &wait,p &cpu,int &status,int no)	//printing of status and shifting to waiting queue
{
	//run(cpu,status);
	if (status==1)
	{
		cout<<"Process in cpu "<<no<<" shifted to waiting queue directly"<<endl;
		p cpucopy=cpu;
		wait.push_back(cpucopy);
	}
	else if (status==2)
	{
		cout<<"Process in cpu "<<no<<" terminated"<<endl;			
	}
	else
	{
		cout<<"Process in cpu "<<no<<" shifted to waiting queue after running in cpu"<<endl;
		p cpucopy=cpu;
		wait.push_back(cpucopy);
	}
	return wait;	
};
int runwaitqueue(vector<p> &v,int time1)
{
	cout<<"runwaitqueue function entered"<<endl;
	if (v.size()==0)
	{
		return -1;
	}
	if (time1==0 || v[0].tarr[0].time>time1)	//for process which doesn't execute fully in waiting queue
	{
		//cout<<"if case followed"<<endl;
		//waitt=waitt-time1;
		cout<<"Process(es) that ran in waiting queue during this time slice - "<<endl;
		print(v[0]);
		v[0].totaltime=v[0].totaltime-time1;
		v[0].tarr[0].time=v[0].tarr[0].time-time1;
		if (v[0].tarr[0].time==0)
		{
			v[0].tarr.erase(v[0].tarr.begin());
		}
		return -1;
	}
	int s=0,j=-1;
	for (int i=0;i<v.size();i++)	//for process(es) that runs in waiting queue and are removed
	{
		j=i;
		if (s<time1)
		{
			s=v[i].tarr[0].time+s;
		}	
		else
		{
			j=i;
			break;
		}
	}		
	cout<<"time1 = "<<time1<<endl;
	cout<<"no_of_p_wait = "<<j<<endl;
	return j;
};
vector<p> run_in_wait_queue(int no_of_p_wait,vector<p> &wait,vector<p> &b2,int &waitt)		//actual erasing of processes in wait queue
{
			if (no_of_p_wait>=0)
			{
				//int no=min((int)wait.size()-1,no_of_p_wait);
				for (int i=0;i<no_of_p_wait;i++)
				{
					waitt=waitt-wait[i].tarr[0].time;
					cout<<"Process(es) that ran in waiting queue during this time slice - "<<endl;
					print(wait[i]);
					wait[i].totaltime=wait[i].totaltime-wait[i].tarr[0].time;
					if (wait[i].tarr.size()>1)	//for pushing process into ready queue
					{
						p push=wait[i];
						push.tarr.erase(push.tarr.begin());
						b2.push_back(push);
					}
				}
				wait.erase(wait.begin(),wait.begin()+min((int)wait.size(),no_of_p_wait+1));				
			}							
	return b2;
};
void runcpu1_firstpart(p &cpu1,vector<p> &b1,vector<p> &b2,vector<p> &wait,int &runtime_exec1,int &runtime_exec2,int &waitt,int &runtt)
{
	// moving of processes from waiting queue to ready queue while process in cpu1 is executing
	cout<<"runcpu1_firstpart"<<endl;
	int no=runwaitqueue(wait,runtime_exec1);
	//cout<<"firstpart no = "<<no<<endl;
	if (no>=0)		//shifting of processes takes place
	{
		b2=run_in_wait_queue(no,wait,b2,waitt);
	}
	else
	{
		if (wait.size()>0)		//if no process is shifted
		{
			wait[0].tarr[0].time=wait[0].tarr[0].time-runtime_exec1;
			waitt=waitt-runtime_exec1;
			wait[0].totaltime=wait[0].totaltime-runtime_exec1;
		}
	}	
	run(cpu1,status1);
	wait=status_read(wait,cpu1,status1,1);
};
void runcpu1_secondpart(p &cpu2,p &cpu1,vector<p> &b1,vector<p> &b2,vector<p> &wait,int &runtime_exec1,int &runtime_exec2,int &waitt,int &runtt)
{
	//cout<<"b1.size() = "<<b1.size()<<endl;
	cout<<"runcpu1_secondpart"<<endl;
	int t=0,s1=0;
	if (runtime_exec2!=0)
	{
		t=runtime_exec2-runtime_exec1;
	}
	int c=0;
	//cout<<"t = "<<t<<endl;
	if (cpu2.id==-1)
	{
		t=0;		
	}
	if (t!=0)
	{
	while(s1<t)
	{
		//cout<<"c = "<<c<<endl;
		c=c+1;
		if (cruntime(b1[0])>t)		//no process goes from b1 to cpu1
		{
		cout<<"no process goes from b1 to cpu1"<<endl;
		s1=t;
		run(cpu1,status1);
		wait=status_read(wait,cpu1,status1,1);
		if (wait.size()>0)																					//1
		{
			cout<<"Shifted to waiting queue - ";
			print(wait[wait.size()-1]);
		//	cout<<";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;"<<endl;
		}
		}	
		else						
		{			
			cout<<"process goes from b1 to cpu1"<<endl;
			run(cpu1,status1);
			wait=status_read(wait,cpu1,status1,1);			
			if (wait.size()>0)
			{
				cout<<"Shifted to waiting queue - ";
				print(wait[wait.size()-1]);
		//		cout<<";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;"<<endl;
			}
			//cout<<"secondpart status_read"<<endl;
			if (b1.size()!=0)
			{
			cpu1=b1[0];							//process goes from b1 to cpu1
			b1.erase(b1.begin());
			print(cpu1);
	//		cout<<"b1.size() after deletion ="<<b1.size()<<endl;
			}
			else
			{
	//			cout<<"b1.size() after deletion ="<<b1.size()<<endl;
				break;
			}
			if (b2.size()!=0)							
			{
			b1.push_back(b2[0]);					//process goes from b2 to b1
			cout<<"Process going from b2 to b1 = ";
			print(b2[0]);
			b2.erase(b2.begin());
			}			
			sort(b1.begin(),b1.end(),sortsjf);
			s1=s1+cruntime(cpu1);		//time subtraction
			if (wait.size()!=0)				//for waiting queue process during process running in cpu1
			{
				if (cruntime(cpu1)<wait[0].tarr[0].time)
				{
					wait[0].tarr[0].time=wait[0].tarr[0].time-cruntime(cpu1);		//no process goes from waiting queue to b2
					wait[0].totaltime=wait[0].totaltime-cruntime(cpu1);
				}
				else
				{
					int j1=runwaitqueue(wait,cruntime(cpu1));		//process goes from waiting queue to b2
					j1=min(j1,(int)wait.size());
					b2=run_in_wait_queue(j1,wait,b2,waitt);
					sort(b2.begin(),b2.end(),sortljf);
				}
			}
		}
	}
	}
	run(cpu1,status1);
	wait=status_read(wait,cpu1,status1,1);
};
//void sequential(p &cpu2,p &cpu1,vector<p> &b1,vector<p> &b2,vector<p> &wait,int &runt,int &waitt,int totalrun);
int main()
{
	int n1,n2;	
	vector<p> b1,b2,wait;
	cout<<"Enter number of processes for first queue: ";  
	cin>>n1;
	for (int i=0;i<n1;i++)
	{
		p input=getp();
		b1.push_back(input);	
		cout<<endl;	
	}
	cout<<"Enter number of processes for second queue: "; 
	cin>>n2;
	for (int i=0;i<n2;i++)
	{
		p input=getp();
		b2.push_back(input);	
		cout<<endl;	
	}
	p cpu1,cpu2;
	int totalrun=0;
	int runt=0,waitt=0;
	p defaultp;
	defaultp.id=-1;
	defaultp.totaltime=-1;
	t defaultt;
	defaultt.mode=-1;
	defaultt.time=-1;
	defaultp.tarr.push_back(defaultt);
	runt=calruntime(b1)+calruntime(b2);	
	waitt=calwaittime(b1)+calwaittime(b2);	
	while (runt>0)
	{
		jump:
		cout<<"Total computation running time = "<<totalrun<<endl;
		if (b1.size()>0)
		{sort(b1.begin(),b1.end(),sortsjf);		}
		if (b2.size()>0)
		{sort(b2.begin(),b2.end(),sortljf);}	//scheduling part		
		if (b1.size()>=2)
		{
			cpu1=min1(b1[0],b1[1]);
			cpu2=max1(b1[1],b1[0]);
		}
		else if (b1.size()>0)
		{
			cpu1=b1[0];
			cpu2=defaultp;
		}
		else
		{
			cpu1=defaultp;
			cpu2=defaultp;
		}										//assigning the processes to the cpus		
		int t=min(2,(int)b1.size());
		if (b1.size()>0)
		{b1.erase(b1.begin(),b1.begin()+t);} //cout<<"1111111111111"<<endl;
		if (b2.size()>1)
		{
			b1.push_back(b2[0]);
			b1.push_back(b2[1]);
			b2.erase(b2.begin(),b2.begin()+2);  //cout<<">1>1!>>1>1>1>!!>!.!"<<endl;
		}
		else if (b2.size()==1)
		{
			b1.push_back(b2[0]);
			b2.erase(b2.begin());  //cout<<"111111111111111///////////////////////////1111111111111"<<endl;
		}								//shifing the processes		
		if (cpu1.id==defaultp.id)
		{
			cpu1=b1[0];
			b1.erase(b1.begin());
		}
		cout<<"cpu1 = ";
		print(cpu1);
		cout<<"cpu2 = ";
		print(cpu2);
		int runtime_exec1=run(cpu1,status1);		//process 4 ki small problem yaha hai, thodi code ki lines and variables ko yaha waha karna hai
		runt=runt-runtime_exec1;				
		int t1=runtime_exec1;		//t1-> for how long the max time taking process run in the cpu cores
		int runtime_exec2=0;		
		if (cpu2.id!=-1)
		{			
			runtime_exec2=run(cpu2,status2);				
			runt=runt-runtime_exec2;
			t1=runtime_exec2;
		}	
		//cout<<"The runtimes done"<<endl;				//time slice reduction
		if (cpu2.id!=-1)		
		{
		runcpu1_firstpart(cpu1,b1,b2,wait,runtime_exec1,runtime_exec2,waitt,runt);				//for the transfer of processes from waiting queue to ready queue 
		runcpu1_secondpart(cpu2,cpu1,b1,b2,wait,runtime_exec1,runtime_exec2,waitt,runt);		//for the processes that run in cpu1 while the longer process runs in cpu2 and also subsequent transfer of processes 		
		wait=status_read(wait,cpu2,status2,2);
		totalrun=totalrun+t1;		
		cout<<"----------------------------------------------------------"<<endl;	
		}
		else
		{
			wait=status_read(wait,cpu1,status1,1);
			totalrun=totalrun+t1;
			int no_of_process = runwaitqueue(wait,t1);					
			b2=run_in_wait_queue(no_of_process,wait,b2,waitt);
			break;
		}		
	}
	if (cpu2.id==-1)
	{
		cout<<"Totalrun = "<<totalrun<<endl;
		cout<<"---------------------------------------------------------"<<endl;
		//cout<<"b1.size() = "<<b1.size()<<endl;
		//cout<<"b2.size() = "<<b2.size()<<endl;
		//cout<<"wait.size() = "<<wait.size()<<endl;
	if (b1.size()==0 && b2.size()==0)
	{		
		for (int i=0;i<wait.size()-1;i++)
		{
			for (int j=i+1;j<wait.size();j++)
			{
				if (wait[i].id==wait[j].id)
				{
					wait.erase(wait.begin()+j);
					j=j-1;
				}
			}
		}
		/*
		for (long long int i=0;i<wait.size();i++)
		{
		cout<<wait[i].id<<" ";}
		*/
	bool c=true;
	while (c)	
		{
		if (wait.size()==0)
		{
			break;
		}
		totalrun=totalrun+wait[0].tarr[0].time;
		waitt=waitt-wait[0].tarr[0].time;
		wait[0].totaltime=wait[0].totaltime-wait[0].tarr[0].time;		
		wait[0].tarr[0].time=0;
		if (wait[0].tarr.size()>1)
		{						
			print(wait[0]);
			wait[0].tarr.erase(wait[0].tarr.begin());
			cout<<"Total (till now) running time = "<<totalrun<<endl;
			b2.push_back(wait[0]);
			wait.erase(wait.begin());
			c=false;
			goto jump;
		}
		else
		{					
			print(wait[0]);
			wait[0].tarr.erase(wait[0].tarr.begin());
			cout<<"Total (till now) running time = "<<totalrun<<endl;			
			wait.erase(wait.begin());			
		}
		}
	}		
	}
	if (waitt>0 && wait.size()>0)
	{
		cout<<"Process(es) run after computation time for process(es) in CPU is over - "<<endl;
		for (int i=0;i<wait.size();i++)
		{
			print(wait[i]);
		}
	}
	return 0;
}
