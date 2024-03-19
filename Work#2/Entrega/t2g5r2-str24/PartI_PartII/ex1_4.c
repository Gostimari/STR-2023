#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"
#include <math.h>
#include <string.h>
#include <time.h>

#define NS 1000000000
#define INITIAL_MAX -INFINITY
#define INITIAL_MIN INFINITY

/* Function to calculate time difference */
struct timespec dif(struct timespec start, struct timespec end)
{
    struct timespec result;
    
    if ((end.tv_nsec - start.tv_nsec) < 0)
    {
        result.tv_sec = end.tv_sec - start.tv_sec - 1;
        result.tv_nsec = NS + end.tv_nsec - start.tv_nsec;
    }
    else
    {
        result.tv_sec = end.tv_sec - start.tv_sec;
        result.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    
    return result;
}

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

int main(int argc, char *argv[])
{
    struct timespec start[3], end[3], tempo[3];
    struct timespec total_time = {0, 0};
    char *filename1="point_cloud1.txt", *filename2="point_cloud2.txt", *filename3="point_cloud3.txt"; 

    coords *cloud1, *cloud2, *cloud3;
    
    // CLOUD 1
    clock_gettime(CLOCK_MONOTONIC, &start[0]);
    cloud1 = structpnt(filename1);
    clock_gettime(CLOCK_MONOTONIC, &end[0]);

    clock_gettime(CLOCK_MONOTONIC, &start[1]);
    pre_processing(cloud1);
    clock_gettime(CLOCK_MONOTONIC, &end[1]);
    
    printf("Pontos para o pre_processing no point_cloud1: %d\n\n",cloud1->n_pontos);

    clock_gettime(CLOCK_MONOTONIC, &start[2]);
    discardNotDrivable(cloud1);
    clock_gettime(CLOCK_MONOTONIC, &end[2]);

    printf("Pontos para a discardNotDrivable no point_cloud1: %d\n\n",cloud1->n_pontos);

    tempo[0]=dif(start[0],end[0]);
    tempo[1]=dif(start[1],end[1]);
    tempo[2]=dif(start[2],end[2]);
    total_time.tv_nsec += tempo[0].tv_nsec + tempo[1].tv_nsec + tempo[2].tv_nsec; // Soma total dos tempos
    
    printf("\n\n - Para point cloud 1:\n\n");
    printf("Tempo de structpnt: 0.%.9ld\n",tempo[0].tv_nsec);
    printf("Tempo de pre_processing: 0.%.9ld\n",tempo[1].tv_nsec);
    printf("Tempo discardNotDrivable: 0.%.9ld\n",tempo[2].tv_nsec);
    printf("Tempo total: 0.%.9ld\n\n\n",total_time.tv_nsec);


    // CLOUD 2
    clock_gettime(CLOCK_MONOTONIC, &start[0]);
    cloud2 = structpnt(filename2);
    clock_gettime(CLOCK_MONOTONIC, &end[0]);

    clock_gettime(CLOCK_MONOTONIC, &start[1]);
    pre_processing(cloud2);
    clock_gettime(CLOCK_MONOTONIC, &end[1]);
    
    printf("Pontos para o pre_processing no point_cloud2: %d\n\n",cloud2->n_pontos);

    clock_gettime(CLOCK_MONOTONIC, &start[2]);
    discardNotDrivable(cloud2);
    clock_gettime(CLOCK_MONOTONIC, &end[2]);

    printf("Pontos para a discardNotDrivable no point_cloud2: %d\n\n",cloud2->n_pontos);

    tempo[0]=dif(start[0],end[0]);
    tempo[1]=dif(start[1],end[1]);
    tempo[2]=dif(start[2],end[2]);
    total_time.tv_nsec += tempo[0].tv_nsec + tempo[1].tv_nsec + tempo[2].tv_nsec; // Soma total dos tempos

    printf("\n\n - Para point cloud 2:\n\n");
    printf("Tempo de structpnt: 0.%.9ld\n",tempo[0].tv_nsec);
    printf("Tempo de pre_processing: 0.%.9ld\n",tempo[1].tv_nsec);
    printf("Tempo discardNotDrivable: 0.%.9ld\n",tempo[2].tv_nsec);
    printf("Tempo total: 0.%.9ld\n\n\n",total_time.tv_nsec);


    clock_gettime(CLOCK_MONOTONIC, &start[0]);
    cloud3 = structpnt(filename3);
    clock_gettime(CLOCK_MONOTONIC, &end[0]);

    clock_gettime(CLOCK_MONOTONIC, &start[1]);
    pre_processing(cloud3);
    clock_gettime(CLOCK_MONOTONIC, &end[1]);
    
    printf("Pontos para o pre_processing no point_cloud3: %d\n\n",cloud3->n_pontos);

    clock_gettime(CLOCK_MONOTONIC, &start[2]);
    discardNotDrivable(cloud3);
    clock_gettime(CLOCK_MONOTONIC, &end[2]);

    printf("Pontos para a discardNotDrivable no point_cloud3: %d\n\n",cloud3->n_pontos);

    tempo[0]=dif(start[0],end[0]);
    tempo[1]=dif(start[1],end[1]);
    tempo[2]=dif(start[2],end[2]);
    total_time.tv_nsec += tempo[0].tv_nsec + tempo[1].tv_nsec + tempo[2].tv_nsec; // Soma total dos tempos

    printf("\n\n - Para point cloud 3:\n\n");
    printf("Tempo de structpnt: 0.%.9ld\n",tempo[0].tv_nsec);
    printf("Tempo de pre_processing: 0.%.9ld\n",tempo[1].tv_nsec);
    printf("Tempo discardNotDrivable: 0.%.9ld\n",tempo[2].tv_nsec);
    printf("Tempo total: 0.%.9ld\n\n\n",total_time.tv_nsec);

    
    return 0;
}
