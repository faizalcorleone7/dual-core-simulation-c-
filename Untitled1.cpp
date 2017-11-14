#include<bits/stdc++.h> 		//sequential process
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
int status;
//status = 1 -> shift directly to waiting queue, status = 2 -> terminate after running in cpu, status = 3 -> shift to waiting queue after running in cpu
int run(p &cpu,int runt)			
{
	cout<<"Run function entered"<<endl;
	if (cpu.tarr[0].mode==2)
	{
		status=1;
		cout<<0<<endl;
		return 0;	
	}	
	else if (cpu.tarr.size()==1)
	{
		status=2;
		int time1=cpu.tarr[0].time;
		cpu.totaltime=0;
		cpu.tarr.erase(cpu.tarr.begin());
		cout<<"time1 = "<<time1<<endl;
		return time1;
		//return runt;
	}
	else
	{
		status=3;		
		int time1=cpu.tarr[0].time;
		cpu.totaltime=cpu.totaltime-cpu.tarr[0].time;
		cpu.tarr.erase(cpu.tarr.begin());
		cout<<"time1 = "<<time1<<endl;
		return time1;
	}
};
int runwaitqueue(vector<p> v,int time1)
{
	cout<<"runwaitqueue function entered"<<endl;
	if (v.size()==0)
	{
		return -1;
	}
	if (time1==0 || v[0].tarr[0].time>time1)
	{
		//cout<<"if case followed"<<endl;
		return -1;
	}
	int s=0,j=-1;
	for (int i=0;i<v.size();i++)
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
	return j;
};
int main()
{
	int running_time=0;
	int e=1;
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
	int runt=0,waitt=0;
	cout<<"Successful input"<<endl;	
	runt=calruntime(b1)+calruntime(b2);	
	waitt=calwaittime(b1)+calwaittime(b2);
	//cout<<runt<<" "<<waitt<<endl;		
	p cpu;
	while (runt>0)
	{
		jump:
		if (b1.size()>0)
		{
		sort(b1.begin(),b1.end(),sortsjf);
		}
		sort(b2.begin(),b2.end(),sortljf);					
		if (b1.size()>0)
		{
		cpu=b1[0];
		cout<<"cpu = ";
		print(cpu);		
		b1.erase(b1.begin());
		}
		else
		{
			if (b2.size()>0)
			{
				b1.push_back(b2[0]);
				b2.erase(b2.begin());				
				sort(b1.begin(),b1.end(),sortsjf);
				cpu=b1[0];
				cout<<"cpu = ";
				print(cpu);
				b1.erase(b1.begin());
			}
			else
			{
			//	cout<<"b2.size() = "<<b2.size()<<endl;
				break;	
			}
		}
		if (b2.size()>0)
		{
			b1.push_back(b2[0]);
			b2.erase(b2.begin());
		}
		cout<<"Shifting of processes done"<<endl;
		int runtime_exec=run(cpu,runt);			//status = 1 -> shift directly to waiting queue, status = 2 -> terminate after running in cpu, status = 3 -> shift to waiting queue after running in cpu
		runt=runt-runtime_exec;
		running_time=running_time+runtime_exec;
		if (status!=1)
		{
			int no_of_p_wait=runwaitqueue(wait,runtime_exec);
			cout<<"no_of_p_wait = "<<no_of_p_wait<<endl;			
			if (no_of_p_wait>=0)
			{
				//int no=min((int)wait.size()-1,no_of_p_wait);
				for (int i=0;i<=no_of_p_wait;i++)
				{
					waitt=waitt-wait[i].tarr[0].time;
					cout<<"Process(es) that ran in waiting queue during this time slice - "<<endl;
					print(wait[i]);
					wait[i].totaltime=wait[i].totaltime-wait[i].tarr[0].time;
					if (wait[i].tarr.size()>1)
					{
						p push=wait[i];
						push.tarr.erase(push.tarr.begin());
						b2.push_back(push);
					}
				}
				wait.erase(wait.begin(),wait.begin()+no_of_p_wait+1);
			}
			else if (wait.size()>0)
			{				
				waitt=waitt-runtime_exec;
				wait[0].tarr[0].time=wait[0].tarr[0].time-runtime_exec;
				wait[0].totaltime=wait[0].totaltime-runtime_exec;
			}
		}
		if (status==1)
		{
			cout<<"Process in cpu shifted to waiting queue directly"<<endl;
			p cpucopy=cpu;
			wait.push_back(cpucopy);
		}
		else if (status==2)
		{
			cout<<"Process in cpu terminated"<<endl;			
		}
		else
		{
			cout<<"Process in cpu shifted to waiting queue after running in cpu"<<endl;
			p cpucopy=cpu;
			wait.push_back(cpucopy);
		}	
		cout<<"Total (till now) running time = "<<running_time<<endl;
		cout<<"Total computation running time remaining = "<<runt<<endl;
		cout<<"Total input/output time remaining = "<<waitt<<endl;
		cout<<"----------------------------------------------------------"<<endl;	
	}	
	if (runt>0)			//if no process in b1 or b2
	{	
	bool c=true;
	while (c)	
		{
		running_time=running_time+wait[0].tarr[0].time;
		waitt=waitt-wait[0].tarr[0].time;
		wait[0].totaltime=wait[0].totaltime-wait[0].tarr[0].time;		
		if (wait[0].tarr.size()>1)
		{					
			print(wait[0]);
			wait[0].tarr.erase(wait[0].tarr.begin());
			cout<<"Total (till now) running time = "<<running_time<<endl;
			cout<<"Total computation running time remaining = "<<runt<<endl;
			cout<<"Total input/output time remaining = "<<waitt<<endl;	
			b2.push_back(wait[0]);
			wait.erase(wait.begin());
			c=false;
			goto jump;			
		}
		else
		{
			if (wait.size()==0)
			{
				break;
			}			
			print(wait[0]);
			wait[0].tarr.erase(wait[0].tarr.begin());
			cout<<"Total (till now) running time = "<<running_time<<endl;
			cout<<"Total computation running time remaining = "<<runt<<endl;
			cout<<"Total input/output time remaining = "<<waitt<<endl;		
			wait.erase(wait.begin());			
		}
		}
	}
	cout<<"---------------------------------------------------"<<endl;
	if (waitt>0)
	{
		cout<<"Process(es) run after computation time for process(es) in CPU is over - "<<endl;
		for (int i=0;i<wait.size();i++)
		{
			running_time=running_time+wait[i].totaltime;			
			print(wait[i]);
			cout<<endl;
		}
	}
	cout<<"Total running time = "<<running_time;
	return 0;
}
