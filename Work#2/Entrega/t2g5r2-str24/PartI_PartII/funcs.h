#include <stdlib.h>
#include <stdio.h>

typedef struct coords
{
    double *x;
    double *y;
    double *z;
    int n_pontos;
}coords;

struct timespec dif(struct timespec um, struct timespec dois);
struct timespec timesum(struct timespec um, struct timespec dois);

coords *structpnt(char *fileName);

void pre_processing(coords *coords);
void discardNotDrivable (coords *coord);
