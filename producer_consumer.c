#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#define BUFFERSIZE 8                                        /* Keeping the fixed size of buffer to 8*/
#define MAX 64		                                        /* how many numbers to producer produces */
#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
#define sleep(x) Sleep(x)
#endif


pthread_mutex_t	the_mutex; 
//pthread_cond_t	condc, condp;	                          /* used for signaling */
pthread_t tidp,tidc;
sem_t full,empty;
int buffer[BUFFERSIZE],count,n1=4,n2=4,p,c;                   /*int buffer = 0;	buffer used between producer and consumer */



void initialize()
{
	pthread_mutex_init(&the_mutex,NULL);   				      /*initialize the mutex*/
	sem_init(&full,1,0);                  				      /*initialize the conditional variables (or)semaphores for producer*/
	sem_init(&empty,1,BUFFERSIZE);        				      /*initialize the conditional variables (or)semaphores for consumer*/
	count=0;                           				          /*initialize the counter variable for buffer*/
}


void *producer(void *ptr) 							           /* produce data */
{        				
	int i,j,wait,item; 
	for (p = 1; p <= n1; p++){      
	     sem_wait(&empty);                                   /*wait for the producer to produce items*/
		 pthread_mutex_lock(&the_mutex);                     /* get exclusive access to buffer locked by producer*/
			for(j=count;j<MAX;j++){  	
				item=j;
				printf("\n producer %d has produced items= %d\n", p,item);/*producer starts producing items*/
				buffer[count] = item;							      /* put item in buffer */
					if(count==(BUFFERSIZE-1)){						      /*if buffer is full*/
						printf("\n buffer is full, wait until the consumer consumes items..");
							//sem_wait(&empty);
						    pthread_mutex_unlock(&the_mutex);			      /* release access to buffer */
							sem_post(&full);    						      /*signal the consumer*/
			sleep(1);
			count--;
		}
			count++;
	}
	pthread_mutex_unlock(&the_mutex);
	sem_post(&full);
	}
}


void *consumer(void *ptr) 							      	/* consume data */
{				
	int i;
	for (c = 1; c <= n2; c++) {
		sem_wait(&full); 				                               /* get exclusive access to buffer */
		pthread_mutex_lock(&the_mutex);	                              /*wait until consumer consumes the items*/
		sleep(1);
		for(i=0;i<MAX;i++) {
			printf("\n consumer %d has consumed items:%d\n", p, buffer[count]);
			if(count==0){
				printf("\n buffer is empty wait until produces the items..");
				pthread_mutex_unlock(&the_mutex);		   /* release access to buffer-unlock by consumer*/
				sem_post(&empty);   					   /*signal producer*/
				sleep(1);
				count++;
			}
		count--;
	}
	pthread_mutex_unlock(&the_mutex);          	           /*unlock by consumer*/
	sem_post(&empty);								       /*signal producer*/
	}
}
	


int main()
{         
	initialize();                         	/*calling initialize function*/
	/* printf("\nEnter the no of producers: ");
	scanf("%d",&n1);
	printf("\nEnter the no of consumers: ");
	scanf("%d",&n2); */
	
	
	pthread_create(&tidp,NULL,producer, NULL);			/*creating producer threads*/
	pthread_create(&tidc,NULL,consumer, NULL);   		/*creating consumer threads*/
	for(p=1;p<=n1;p++)
	pthread_join(tidp,NULL);							/*joining producer and consumer threads*/
	for(c=1;c<=n2;c++)
    pthread_join(tidc,NULL);
	exit(0);
}


