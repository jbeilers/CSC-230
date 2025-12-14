//
// Created by jeile on 11/3/2025.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {
    FILE *fp = fopen("doubles.bin", "rb");

    double *space = malloc(sizeof(double));
    double total = 0;

    while (!feof(fp)) {
        fread(space, sizeof(double), 1, fp);
        total += *space;
        printf("%lf\n", *space);
    }


    printf("%lf", total);
    fclose(fp);
    exit(0);
}
