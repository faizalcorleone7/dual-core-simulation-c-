#include<iostream> 
#include<malloc.h>
#include<vector>
using namespace std;
typedef struct Time
{
	int mode;         /*mode = 1:- computation; mode = 2:- I/O */ 
	int t;
	struct Time *next;
}time;
typedef struct Process
{
	int check;
	int id,totaltime;
	int n;
	struct Process *next;
	time *head,*end;
}process;
typedef struct Batch
{
	int n1;					//n1=no. of processes in that batch
	process *h,*e;
}batch;
int total=0,totalwait=0;   //total=addition of computation time of all processes; totalwait=addition of i/o time of all processes; 
process* getp()
{
		process *p=(process*)malloc(sizeof(process));
		p->totaltime=0;
		p->check=0;
		cout<<"p->totaltime initialised"<<endl;
		cout<<"getp()"<<endl;
		cout<<"Process id: ";
		cin>>p->id;
		cout<<"Size of time array: ";
		cin>>p->n;
		cout<<"For time array:"<<endl;
		for (int i=0;i<p->n;i++)
		{
			time *t1=(time*)malloc(sizeof(time));
			int TIME,MODE;
			cout<<"Give input for time first and then give input of mode: "<<endl;
			cin>>TIME>>MODE;  
			p->totaltime=p->totaltime+TIME;
			if (MODE==1)
			{
				total=total+TIME;
			}
			else
			{
				totalwait=totalwait+TIME;
			}
			t1->mode=MODE;
			t1->t=TIME;
			if (i==0)                    //insertion into time array of process
			{
				p->head=t1;
				p->end=p->head;
				p->end->next=NULL;
			}
			else
			{
				p->end->next=t1;
				p->end=p->end->next;
				p->end->next=NULL;
			}
		}			
	return p;	
};
batch getb()
{
	batch b;
	cout<<"getb()"<<endl;
	cout<<"Enter no. of processes: ";
	cin>>b.n1;
	for (int i=0;i<b.n1;i++)
	{		
		process *t1=getp();		
		if (i==0)				//insertion into process array in a batch
		{
			b.h=t1;
			b.e=t1;
			b.e->next=NULL;
		}
		else
		{
			b.e->next=t1;
			b.e=b.e->next;
			b.e->next=NULL;
		}
	}
	return b;	
};
void print(process *p)
{
	cout<<"Process id= "<<p->id<<"\t";		
	cout<<"Totaltime= "<<p->totaltime<<endl;
}
int c,abc;
// c = 2:- process terminate; c = 1:- process moved to waiting queue; c = 0:- process moved to waiting queue after running in cpu
process *run(process *p)   
{
	if (p->head->next==NULL && p->head->mode==1)
	{
		abc=p->totaltime;
		p->totaltime=0;
		p->head->t=0;
		c=2;	
	}
	else
	{
		if (p->head->mode==2)
		{
			c=1;				
		}
		else
		{
		p->totaltime=p->totaltime-p->head->t;
		c=0;		
		time *f=p->head;			//removing head of time array of process
		abc=f->t;
		p->head=p->head->next;
		delete f;
		}
	}  
	return p;
};
process *buffer1;
batch removeprocess(batch b)
{
	if (b.h!=NULL)
	{
	b.n1=b.n1-1;
	process *pz=b.h,*z=pz;
	b.h=b.h->next;
	z->next=NULL;	
	buffer1=z;
	return b;
	}	
	return b;
};
batch addprocess(process *p,batch b)
{
	b.n1=b.n1+1;
	if (b.h==NULL)			//insertion in the linked list of processes in that batch
	{		
		b.h=p;
		b.e=b.h;
		b.e->next=NULL;
		b.n1=1;
	}
	b.e->next=p;	
	b.e=b.e->next;
	b.e->next=NULL;
	return b;	
};
int run1(vector<process*> wait, int q)
{
	if (wait.size()==0){return 0;}
	int j,s=0;
	for (vector<process*>::iterator it=wait.begin();it!=wait.end();++it)
	{
		if (s<=q)
		{
			process *z=*it;
			s=s+z->head->t;
			j=j+1;
		}
		else
		{
			j=j-1;
			break;
		}
	}
	return j;
};
batch sjf(batch b)
{	if (b.h!=NULL)
	{
	process *buffer=new process,*t=b.h,*track=t;		
	int m=b.h->totaltime;
	for (int i=0;i<b.n1;i++)
	{		
	t=track;
	m=t->totaltime;
	process *new1;
	while (t!=NULL)
	{		
		if (t->totaltime<=m)
		{			
			new1=t;
			m=t->totaltime;			
		}
		t=t->next;	
	}
	t=track;	
	buffer->totaltime=new1->totaltime;		//value swapping in L.L.
	buffer->end=new1->end;
	buffer->head=new1->head;
	buffer->id=new1->id;
	buffer->n=new1->n;	
	new1->totaltime=t->totaltime;
	new1->end=t->end;
	new1->head=t->head;
	new1->id=t->id;
	new1->n=t->n;	
	t->totaltime=buffer->totaltime;
	t->end=buffer->end;
	t->head=buffer->head;
	t->id=buffer->id;
	t->n=buffer->n;		
	track=track->next;	
	}
	}
return b;
};
batch priority(batch b)
{
	if (b.h!=NULL)
	{process *buffer=new process,*t=b.h,*track=t;		
	int m=b.h->totaltime;
	for (int i=0;i<b.n1;i++)
	{	
	t=track;
	m=t->totaltime;
	process *new1;
	while (t!=NULL)
	{		
		if (t->totaltime>=m)
		{			
			new1=t;
			m=t->totaltime;			
		}
		t=t->next;	
	}
	t=track;	
	buffer->totaltime=new1->totaltime;
	buffer->end=new1->end;
	buffer->head=new1->head;
	buffer->id=new1->id;
	buffer->n=new1->n;	
	new1->totaltime=t->totaltime;
	new1->end=t->end;
	new1->head=t->head;
	new1->id=t->id;
	new1->n=t->n;	
	t->totaltime=buffer->totaltime;
	t->end=buffer->end;
	t->head=buffer->head;
	t->id=buffer->id;
	t->n=buffer->n;		
	track=track->next;
	}
	}
return b;
};
int main()
{
	batch b1,b2,b3;
	b1=getb();
	b2=getb();
	b3=getb();
	process *cpu;
	vector<process*> wait;	//waiting
	while(total>0) 	
	{		
		b1=sjf(b1);			
		b3=priority(b3);		
		b1=removeprocess(b1);
		cpu=buffer1;
		cout<<"cpu= ";
		print(cpu);
		cout<<"\n"<<endl;
		if (b2.h!=NULL && b3.h!=NULL)
		{
		b2=removeprocess(b2);
		b1=addprocess(buffer1,b1);				
		b3=removeprocess(b3);
		b2=addprocess(buffer1,b2);
		}
		else if(b2.h==NULL && b3.h!=NULL)		
		{
		b3=removeprocess(b3);
		b2=addprocess(buffer1,b2);
		b2=removeprocess(b2);
		b1=addprocess(buffer1,b1);
		}	
		else if (b2.h!=NULL && b3.h==NULL)
		{
			b2=removeprocess(b2);
			b1=addprocess(buffer1,b1);				
		}
		else
		{}
		cout<<"Now into run functions section"<<endl;	
		cpu=run(cpu);    // return 2:- process terminate; return 1:- process moved to waiting queue; return 0:- process moved to waiting queue after being run in cpu 
		process *cpu1=cpu; 		 //for removing process from cpu to waiting queue like buffer1 process 
		if (c!=2)		
		{
		int a=run1(wait,abc);		
		cout<<"Process(es) that ran in this time slice in waiting vector: "<<endl;
		if (a!=0)
		{
		for (int j=0;j<a;j++)
		{
			vector<process*>::iterator it=wait.begin();
			process *z=*it;
			print(z);
			cout<<endl;
			z->totaltime=z->totaltime-z->head->t;
			totalwait=totalwait-z->head->t;
			if (z->head->next==NULL)
			{
				delete z;
			}
			else
			{	
				z->head=z->head->next;			
				b3=addprocess(z,b3);
			}
			wait.erase(wait.begin());
		}
		}
		}
		if (c==1)
		{
			cout<<"Process in cpu moved to waiting queue "<<endl;
			wait.push_back(cpu1);
		}
		else if (c==2)
		{
			total=total-abc;						
			cout<<"Process in cpu is terminated"<<endl;
		}
		else
		{
			total=total-abc;
			wait.push_back(cpu1);
		}		
		if (total<0)
		{
			total=0;		
		}		
		cout<<"Time remaining for cpu = "<<total<<"\n";		
		cout<<"Time remaining for waiting queue= "<<totalwait<<endl;
		cout<<"----------------------------------------------------------------------"<<endl;
	}
	if (totalwait!=0)
	{
		cout<<"Processes being run in waiting queue after all computational operations of each process has been run"<<endl;
		while (totalwait>0)
		{
		for (int j=0;j<(int)wait.size();j++)
		{
			vector<process*>::iterator it=wait.begin();
			process *z=*it;
			print(z);
			cout<<endl;
			totalwait=totalwait-z->head->t;			
			wait.erase(wait.begin());
		}	
		}
	}
	return 0;
}
