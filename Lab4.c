#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define N 1000
#define INF 1e9
//Create Struct Process
struct process{
	char name[15];
	int id;
       	float arr, brust, finish;
	float wait_time, rp_time, turnround_time;   
	float remain_time;

};

int num_process; //Number of Processes

// Create Queue
struct QNode { 
    struct process key; 
    struct QNode* next; 
}; 
typedef struct QNode* pQNode;
  
// Create Queue
struct Queue { 
    struct QNode *front, *rear; 
    int sz;
}; 
typedef struct Queue* pQueue;
  
//Create a Node
pQNode CreateNode(struct process k) 
{ 
    pQNode temp = (pQNode)malloc(sizeof(struct QNode)); 
    //Assign value of process k to temp Node
    strcpy((temp->key).name,k.name);
    (temp->key).id = k.id;
    (temp->key).arr = k.arr;
    (temp->key).brust = k.brust;
    (temp->key).finish = k.finish;
    (temp->key).wait_time = k.wait_time;
    (temp->key).rp_time = k.rp_time;
    (temp->key).remain_time = k.remain_time;
    (temp->key).turnround_time = k.turnround_time;

    temp->next = NULL; 
    return temp; 
} 
//Create a Queue
pQueue createQueue() 
{ 
    pQueue q = (pQueue)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL; 
    q->sz = 0;
    return q; 
} 

void push(pQueue q, struct process k) 
{ 
    pQNode temp = CreateNode(k); 
    
    (q->sz)++;
    if (q->rear == NULL) { 
        q->front = q->rear = temp; 
        return; 
    } 
  
    q->rear->next = temp; 
    q->rear = temp; 
    
} 
  
void pop(pQueue q) 
{ 
   
    if (q->front == NULL){
        q->sz = 0; 
        return; 
    }
   
    pQNode temp = q->front; 
  
    q->front = (q->front)->next; 
  
    if (q->front == NULL) 
        q->rear = NULL; 
    (q->sz)--;
    free(temp); 
} 

int isEmpty(pQueue q){
	if (q->sz == 0) return 1;
	return 0;
}
//----------------------------------------------------------------






//-------------------------------------------------
void swap(struct process* a,struct process* b){
	struct process tmp = *a;
	*a = *b;
	*b = tmp;
}


//-----------------------------------------------

void show(struct process P[]){	
	// Sort Increasing Id
	for (int i = 0; i < num_process - 1; i++)
	       	for (int j = i + 1; j < num_process; j++)
			if (P[j].id < P[i].id) swap(&P[i],&P[j]);


	printf(" _______________________________________________________________________________________________________________ \n");
	printf("|  Process Name  | Arrival Time | Burst Time | Completion Time | Turnaround Time | Waiting Time | Response Time |\n");
	printf("|________________|______________|____________|_________________|_________________|______________|_______________|\n");
	
	float avg_waiting_time = 0;
	float avg_turnaround = 0;
	float avg_respone_time = 0;
	for(int i=0;i<num_process;i++){
		printf("|%-16s|%-14f|%-12f|%-17f|%-17f|%-14f|%-15f|\n", P[i].name, P[i].arr, P[i].brust, P[i].finish, P[i].turnround_time, P[i].wait_time, P[i].rp_time);
		
		avg_waiting_time += P[i].wait_time;
		avg_turnaround += P[i].turnround_time;
		avg_respone_time += P[i].rp_time;
	}

	printf("|________________|______________|____________|_________________|_________________|______________|_______________|\n");
	avg_waiting_time /= num_process;
	avg_turnaround /= num_process;
	avg_respone_time /= num_process;

	printf("Average waiting time : %.2f\n", avg_waiting_time);
	printf("Average turn around time: %.2f\n", avg_turnaround);
	printf("Average respone time: %.2f\n",avg_respone_time);
}


void RR(struct process P[]){
	//Quantumtime
	float Quantum_time;
	printf("Enter Quantum time: ");
	scanf("%f",&Quantum_time);

	//Create a Queue
	pQueue Q = createQueue();
	//Time elapsed
	float time_elapsed = 0;
	//Check if done yet
	int done[num_process];
	for (int i = 0; i < num_process; i++) done[i] = 0;
	//QueueIndex 	
	int numP = 0;
	//Number of remaining processes
	int cntP = num_process;
	//While there are remaining processes
	while (cntP > 0){
		//While index < number of preocesses
		while (numP < num_process){
			//If process is not done yet
			if (done[numP] == 0){
				//Push into Queue
				push(Q,P[numP]);
				//Set time elapsed , add QueueIndex by 1 , move to next process
				time_elapsed = P[numP++].arr;
				break;
			}
			//If done , move to next process
			numP++;
		}
		//While there are processes in Queue
		while (!isEmpty(Q)){
			//Set index = process id
			int idx = ((Q->front)->key).id;
			pop(Q);
			//If process is running first time
			if (P[idx].remain_time == P[idx].brust){
				//Calculate response time
				P[idx].rp_time = time_elapsed - P[idx].arr;	
			}
			//Running time
			int rtime = P[idx].remain_time;
			if (rtime > Quantum_time) rtime = Quantum_time;
			//Update time elapsed
			time_elapsed += rtime;
			//Update remain time
			P[idx].remain_time -= rtime;
			//While index < number of processes and process has arrived
			while (numP < num_process && P[numP].arr <= time_elapsed) {
				//Push process from Queue
				push(Q,P[numP]);
				//Increse QuqueIndex
				numP++;
			}
			//If finish running
			if (P[idx].remain_time == 0) {
				//Set finish time, turnaround time, waiting time
				P[idx].finish = time_elapsed;
				P[idx].turnround_time = P[idx].finish - P[idx].arr;
				P[idx].wait_time = P[idx].turnround_time - P[idx].brust;
				done[idx] = 1;
				//Decrease No Reamaning Processes
				cntP--;
			}
			//If not finish remove from Queue	
			else push(Q,P[idx]);
				
		}

	}
	show(P);


}



void reset(struct process P[]){
	for (int i = 0; i < num_process; i++){
		P[i].finish = P[i].turnround_time = P[i].wait_time = P[i].rp_time = 0;
		P[i].remain_time = P[i].brust;
	}
}

void sort(struct process P[]){
	for (int i = 0; i < num_process - 1; i++)
	       	for (int j = i + 1; j < num_process; j++)
			if (P[j].arr < P[i].arr)swap(&P[i],&P[j]);
			else if (P[i].arr == P[j].arr && P[j].id < P[i].id) swap(&P[i],&P[j]);
}

void Process_Sceduling_Algo(){
	struct process P[N];
	printf("Enter number of process: ");
	scanf("%d",&num_process);
	
	for (int i = 0; i < num_process; i++){
		printf("Enter Process Name: "); scanf("%s", P[i].name);
		printf("Enter Arrival Time: "); scanf("%f", &P[i].arr);
		printf("Enter Burst Time  : "); scanf("%f", &P[i].brust);
		P[i].finish = P[i].turnround_time = P[i].wait_time = P[i].rp_time = 0;
		P[i].remain_time = P[i].brust;
		P[i].id = i;
		printf("\n");
	}
		reset(P);
		sort(P);
		RR(P);
}



int main(){
	Process_Sceduling_Algo();
	return 0;
}