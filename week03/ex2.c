#include <stdio.h>
#include <math.h>

// Define a structure Point with 2 real number fields x and y
struct Point {
    double x;
    double y;
};

// Function to compute the Euclidean distance between two points
double distance(struct Point p1, struct Point p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

// Function to compute the area of a triangle given its vertices
double area(struct Point A, struct Point B, struct Point C) {
    return 0.5 * (A.x * B.y - B.x * A.y + B.x * C.y - C.x * B.y + C.x * A.y - A.x * C.y);
}

int main() {
    // Define points A, B, and C as the vertices of the triangle ABC
    struct Point A = {2.5, 6};
    struct Point B = {1, 2.2};
    struct Point C = {10, 6};

    // Find the distance between points A and B
    double distAB = distance(A, B);
    printf("Distance between A and B: %.2lf\n", distAB);

    // Calculate the area of triangle ABC
    double triangleArea = area(A, B, C);
    if (triangleArea < 0) {
        triangleArea = -triangleArea; // Make sure the area is positive
    }
    printf("Area of triangle ABC: %.2lf\n", triangleArea);

    return 0;
}
