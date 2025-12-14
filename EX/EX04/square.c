// Count the nuber of points inside a 20 x 20 square centered on the origin.

#include <stdio.h>
#include <stdlib.h>

// Return the absolute value of val.
double absolute( double val )
{
  return val < 0 ? -val : val;
}

// Return 1 if the given x, y point is inside the 20 x 20 square.
int inside( double x, double y )
{
  return (absolute(x) <= 10 && absolute(y) <= 10) ? 1 : 0;
}

// Read 2D points from the input and return how many are
// inside the 20 x 20 square.
int countPoints()
{
    float x = 0, y = 0;
    int result = scanf("%f %f", &x, &y);
    return (result <= 0) ? 0 : (int)(inside(x, y) == 1) + countPoints();
}

int main()
{
  // Count all the points and print the result.
  printf( "%d\n", countPoints() );
  
  return EXIT_SUCCESS;
}
