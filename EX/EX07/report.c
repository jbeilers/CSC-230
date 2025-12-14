//
// Created by jeile on 9/14/2025.
//

#include "report.h"

#include <stdio.h>

void report(int eCount, int nCount, char names[ nCount ][ NAME_LIMIT + 1 ], double scores[ eCount ][ nCount ]) {
    printf("%5s | %5s | %18s | %18s\n", "event", "avg", "high", "low");
    printf("------+-------+--------------------+-------------------\n");


    for (int i = 0; i < eCount; i++) {
        // Calculate average, high, and low
        double sum = 0, high = scores[i][0], low = scores[i][0];
        int highIndex = 0, lowIndex = 0;

        for (int j = 0; j < nCount; j++) {
            double score = scores[i][j];
            sum+= score;
            if (score > high) {
                high = score;
                highIndex = j;
            } else if (score < low) {
                low = score;
                lowIndex = j;
            }
        }
        double avg = sum / nCount;

        printf("%5d | %5.2lf | %12s %5.2lf | %12s %5.2lf\n",
            i+1, avg, names[highIndex], scores[i][highIndex], names[lowIndex], scores[i][lowIndex]);
    }
}
