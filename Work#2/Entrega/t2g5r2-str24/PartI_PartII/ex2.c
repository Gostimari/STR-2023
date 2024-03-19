#include "funcs.h"
#include <math.h>

void pre_processing(coords *coords){
    int valid_points = 0;
    double *x_valid = malloc(coords->n_pontos * sizeof(double));
    double *y_valid = malloc(coords->n_pontos * sizeof(double));
    double *z_valid = malloc(coords->n_pontos * sizeof(double));

    // Loop through all points
    for(int i=0; i<coords->n_pontos; ++i){
        // Check if the point is valid
        if (!(coords->x[i]<0  || fabs(coords->y[i]>10) || fabs(coords->x[i]>30)|| ( fabs(coords->x[i]<2) && fabs(coords->y[i]<1) ))){
            // If valid, add to the valid arrays
            x_valid[valid_points] = coords->x[i];
            y_valid[valid_points] = coords->y[i];
            z_valid[valid_points] = coords->z[i];
            valid_points++;
        }
    }

    // Replace the old arrays with the new valid arrays
    free(coords->x);
    free(coords->y);
    free(coords->z);
    coords->x = x_valid;
    coords->y = y_valid;
    coords->z = z_valid;
    coords->n_pontos = valid_points;
}
