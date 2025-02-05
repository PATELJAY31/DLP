
int main() {
    int a, b;

    // Taking input for the add function
    printf("Enter two numbers: ");
    scanf("%d %d", &a, &b);

    // Function call
    add(a, b);

    // User-defined data type example (struct)
    struct student {
        int id;
        float cgpa;
    };

    struct student s;
    s.id = 101;
    s.cgpa = 8.7;

    // Printing student details
    printf("Student ID: %d\n", s.id);
    printf("Student CGPA: %.2f\n", s.cgpa);

    return 0;
}