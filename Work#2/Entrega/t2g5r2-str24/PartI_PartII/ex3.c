#include "funcs.h"
#include <math.h>
#define ACCEPTABLE_ERROR 0.001
#define XMAX 100 // Substitua 100 pelo valor real de xmax
#define YMAX 100 // Substitua 100 pelo valor real de ymax
#define YMIN 0   // Substitua 0 pelo valor real de ymin

void discardNotDrivable (coords *coord)
{
    int *valid_indices = malloc(coord->n_pontos * sizeof(int));
    int *rem_indexs = malloc(coord->n_pontos * sizeof(int)); // Adicionado
    int valid_count = 0;

    for (int i = 0; i < coord->n_pontos; i++)
    {
        int index = 0;
        if (coord->x[i] < XMAX && coord->y[i] < YMAX && coord->y[i] > YMIN)
        {
            double zmax = -1000;
            double zmin = 1000;
            for (int k = i; k < coord->n_pontos; k++)
            {
                if (fabs(coord->x[k] - coord->x[i]) < ACCEPTABLE_ERROR && fabs(coord->y[k] - coord->y[i]) < ACCEPTABLE_ERROR)
                {
                    rem_indexs[index++] = k;
                    if (coord->z[k] > zmax)
                        zmax = coord->z[k];
                    if (coord->z[k] < zmin)
                        zmin = coord->z[k];
                }
            }
            if ((zmax - zmin > 1.0 || zmax > 1.5))
            {
                for (int j = 0; j < index; j++)
                {
                    valid_indices[valid_count++] = rem_indexs[j];
                }
            }
        }
    }

    double *x_valid = malloc(valid_count * sizeof(double));
    double *y_valid = malloc(valid_count * sizeof(double));
    double *z_valid = malloc(valid_count * sizeof(double));

    for (int i = 0; i < valid_count; i++)
    {
        x_valid[i] = coord->x[valid_indices[i]];
        y_valid[i] = coord->y[valid_indices[i]];
        z_valid[i] = coord->z[valid_indices[i]];
    }

    free(coord->x);
    free(coord->y);
    free(coord->z);
    coord->x = x_valid;
    coord->y = y_valid;
    coord->z = z_valid;
    coord->n_pontos = valid_count;

    free(valid_indices);
    free(rem_indexs); // Adicionado
}
