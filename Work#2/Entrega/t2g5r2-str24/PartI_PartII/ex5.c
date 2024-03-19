#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "funcs.h"
#include <string.h>

sem_t semaphore0;
sem_t semaphore1;
sem_t semaphore2;
#define INITIAL_MAX -INFINITY
#define INITIAL_MIN INFINITY

coords *structpnt(char *fileName)
{
    double max_x = INITIAL_MAX, max_y = INITIAL_MAX, max_z = INITIAL_MAX;
    double min_x = INITIAL_MIN, min_y = INITIAL_MIN, min_z = INITIAL_MIN;
    double mean_x = 0, mean_y = 0, mean_z = 0;
    double desvio_x = 0, desvio_y = 0, desvio_z = 0;
    double sum_x = 0, sum_y = 0, sum_z = 0;

    FILE *fp; // Ter a certeza que o ficheiro abre corretamente
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("Error Opening File\n");
        exit(1);
    }
    
    coords *aux;
    aux=malloc(sizeof(coords));

    aux->n_pontos = 0;
    char c; // Variável para o characterer lido do ficheiro, usado para contar o número de elementos

    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n')
            aux->n_pontos = aux->n_pontos + 1; // Incrementar o númerod e pontos caso haja uma nova linha
    
    fseek(fp, 0, SEEK_SET);
    
    //Alocação para aux->n_pontos linhas
    aux->x=malloc(aux->n_pontos*sizeof(double));
    aux->y=malloc(aux->n_pontos*sizeof(double));
    aux->z=malloc(aux->n_pontos*sizeof(double));
    
    char line[100];
    int i=0;
    char* temp;
    
    while (fgets(line, sizeof(line), fp)) // Processo para escolher maior e menor de cada coordenada
    {
        temp = strchr(line,' ');
        temp[0] = '\0';
        aux->x[i] = atof(line);
        strcpy(line,temp+1);
        if (aux->x[i]>max_x)
            max_x=aux->x[i];
        if (aux->x[i]<min_x)
            min_x=aux->x[i];
        sum_x+=aux->x[i];

        temp = strchr(line, ' ');
        temp[0] = '\0';
        aux->y[i] = atof(line);
        strcpy(line,temp+1);
        if (aux->y[i]>max_y)
            max_y=aux->y[i];
        if (aux->y[i]<min_y)
            min_y=aux->y[i];
        sum_y+=aux->y[i];

        aux->z[i] = atof(line);
        if (aux->z[i]>max_z)
            max_z=aux->z[i];
        if (aux->z[i]<min_z)
            min_z=aux->z[i];
        sum_z+=aux->z[i];
        i++;
    }
    
    // Cálculo das médias
    mean_x=sum_x/aux->n_pontos;
    mean_y=sum_y/aux->n_pontos;
    mean_z=sum_z/aux->n_pontos;

    for (int i=0; i<aux->n_pontos; i++)
    {
        desvio_x+=(aux->x[i] - mean_x)*(aux->x[i] - mean_x);
        desvio_y+=(aux->y[i] - mean_y)*(aux->y[i] - mean_y);
        desvio_z+=(aux->z[i] - mean_z)*(aux->z[i] - mean_z);
    }

    // Cálculo dos desvios
    desvio_x = sqrt((desvio_x/(aux->n_pontos-1)));
    desvio_y = sqrt((desvio_y/(aux->n_pontos-1)));
    desvio_z = sqrt((desvio_z/(aux->n_pontos-1)));

    printf("Para o ficheiro: %s\n",fileName);
    printf("Numero de pontos total: %d\n", aux->n_pontos);
    printf("\n - PARA X:\n");
    printf("Max = %lf     Min = %lf     Media = %lf     Desvio = %lf\n",max_x,min_x,mean_x,desvio_x);
    printf("\n - PARA Y:\n");
    printf("Max = %lf     Min = %lf     Media = %lf     Desvio = %lf\n",max_y,min_y,mean_y,desvio_y);
    printf("\n - PARA Z:\n");
    printf("Max = %lf     Min = %lf     Media = %lf     Desvio = %lf\n",max_z,min_z,mean_z,desvio_z);
    printf("\n---------------------------------------------------------------------------------------------------\n\n");

    fclose (fp);
    return aux;
}

struct timespec tempo;

struct coords* n_pontos[3];

char filepath[3][20] = {"point_cloud1.txt", "point_cloud2.txt", "point_cloud3.txt"};

void *thread_pointer (void *vargp )
{
	int count = 0;
	struct timespec sleep, frequencia, end_time, sleep_time;
	long diffs, diffn;
	frequencia.tv_nsec = 150000000;
	frequencia.tv_sec = 0;	
	
	while(count<3) // Para os três ficheiros
	{
        	if(count > 0)
        	{
            		sem_wait(&semaphore2); // Espera pelo thread para excutar depois do primeiro
        	}

		clock_gettime(CLOCK_MONOTONIC, &sleep_time);
		diffs = sleep_time.tv_sec-tempo.tv_sec;		
		diffn = sleep_time.tv_nsec-tempo.tv_nsec;

		if(diffn<0) 
		{  
			diffs-=1;  
			diffn+=1E9;
		}
		
		sleep.tv_sec = frequencia.tv_sec - diffs;
		sleep.tv_nsec = frequencia.tv_nsec - diffn;
		
		if(sleep.tv_nsec<0) 
		{ 
			sleep.tv_sec-=1;  
			sleep.tv_nsec+=1E9;
		}
		
		if (sleep.tv_nsec > 0)
		{
			clock_nanosleep(CLOCK_MONOTONIC, 0, &sleep, NULL);
		}
	
		n_pontos[count] = structpnt(filepath[count]);
		
		clock_gettime(CLOCK_MONOTONIC, &end_time);		
		diffs = end_time.tv_sec-tempo.tv_sec;		
		diffn = end_time.tv_nsec-tempo.tv_nsec;

		if(diffn<0) {  
			diffs-=1;  
			diffn+=1E9;
		}
		
		frequencia.tv_nsec += 150000000;
		
		if(frequencia.tv_nsec >= 1E9) 
		{ 
			frequencia.tv_sec+=1; 
			frequencia.tv_nsec-=1E9;
		}
		
		printf("\n\nFile \"%s\":\n", filepath[count]);
		printf("Deadline: %lds:%ldus\n", frequencia.tv_sec, frequencia.tv_nsec/1000);
		sem_post(&semaphore0);

		count++;
	}
	
	return 0;
}

void *thread_pre_processing (void *vargp)
{ 
	
	int count = 0;
	struct timespec end_time;
	long diffs, diffn;

	while(count<3) // Para os três ficheiros
	{
		sem_wait(&semaphore0); //Excutado apenas quando thread é acabado

		pre_processing(n_pontos[count]);
		
		clock_gettime(CLOCK_MONOTONIC, &end_time); //Obter o tempo final desta função
		diffs = end_time.tv_sec-tempo.tv_sec;		
		diffn = end_time.tv_nsec-tempo.tv_nsec;

		if(diffn<0) {  
			diffs-=1; 
			diffn+=1E9;
		}

		sem_post(&semaphore1); // Para primitir a excução do próximo thread
		
		count++;
	}
	
	return 0;
}

void *thread_discardNotDrivable (void *varpg )
{ 

	int count = 0;
	struct timespec end_time;
	long diffs, diffn;
	
	while(count<3) // Para os três ficheiros
	{
		sem_wait(&semaphore1); //Excutado apenas quando thread é acabado
		discardNotDrivable(n_pontos[count]);
		
		clock_gettime(CLOCK_MONOTONIC, &end_time); //Obter o tempo final desta função		
		diffs = end_time.tv_sec-tempo.tv_sec;		
		diffn = end_time.tv_nsec-tempo.tv_nsec;

		if(diffn<0) 
		{  
			diffs-=1;  
			diffn+=1E9;
		}
      
		printf ("Functions done at %lds:%ldus\n", diffs, diffn/1000);	
        	sem_post(&semaphore2);	
		count++;
	}
	
	return 0;
}

int main()
{
	pthread_t thread[3];
	sem_init(&semaphore0, 0, 0);
	sem_init(&semaphore1, 0, 0);
        sem_init(&semaphore2, 0, 0);
	
	clock_gettime(CLOCK_MONOTONIC, &tempo); // Tempo igual do tempo incial

	for (int i=0; i<3;i++)
		thread[i]=i;

	pthread_create(&thread[0],NULL, thread_pointer, NULL);
	pthread_create(&thread[1],NULL,thread_pre_processing ,NULL);
	pthread_create(&thread[2],NULL,thread_discardNotDrivable ,NULL);


	for(int i=0; i<3; i++)
        	pthread_join(thread[i],NULL);
	
	sem_destroy(&semaphore0);
	sem_destroy(&semaphore1);
        sem_destroy(&semaphore2);

	return 0;
}
