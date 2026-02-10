#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define FILENAME "students.dat"

// Structure to store student information
struct Student {
    int rollNo;
    char name[50];
    int age;
    float marks;
    char department[30];
};

// Function prototypes
void addStudent();
void displayAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void loadFromFile();
void saveToFile();
void displayMenu();

// Global variables
struct Student students[MAX_STUDENTS];
int studentCount = 0;

int main() {
    int choice;
    
    // Load existing data from file
    loadFromFile();
    
    printf("=== Student Management System ===\n");
    
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                printf("\nExiting and saving data...\n");
                saveToFile();
                printf("Thank you for using Student Management System!\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while(choice != 6);
    
    return 0;
}

// Display menu
void displayMenu() {
    printf("\n=== MENU ===\n");
    printf("1. Add Student Record\n");
    printf("2. Display All Students\n");
    printf("3. Search Student\n");
    printf("4. Update Student Record\n");
    printf("5. Delete Student Record\n");
    printf("6. Exit\n");
    printf("============\n");
}

// Add new student
void addStudent() {
    if(studentCount >= MAX_STUDENTS) {
        printf("\nError: Database is full!\n");
        return;
    }
    
    struct Student newStudent;
    
    printf("\n=== Add New Student ===\n");
    printf("Enter Roll Number: ");
    scanf("%d", &newStudent.rollNo);
    
    // Check if roll number already exists
    for(int i = 0; i < studentCount; i++) {
        if(students[i].rollNo == newStudent.rollNo) {
            printf("Error: Roll number already exists!\n");
            return;
        }
    }
    
    printf("Enter Name: ");
    scanf(" %[^\n]", newStudent.name);
    
    printf("Enter Age: ");
    scanf("%d", &newStudent.age);
    
    printf("Enter Marks: ");
    scanf("%f", &newStudent.marks);
    
    printf("Enter Department: ");
    scanf(" %[^\n]", newStudent.department);
    
    students[studentCount] = newStudent;
    studentCount++;
    
    printf("\nStudent record added successfully!\n");
    saveToFile();
}

// Display all students
void displayAllStudents() {
    if(studentCount == 0) {
        printf("\nNo student records found!\n");
        return;
    }
    
    printf("\n=== Student Records ===\n");
    printf("%-10s %-25s %-8s %-8s %-20s\n", "Roll No", "Name", "Age", "Marks", "Department");
    printf("--------------------------------------------------------------------------------\n");
    
    for(int i = 0; i < studentCount; i++) {
        printf("%-10d %-25s %-8d %-8.2f %-20s\n", 
               students[i].rollNo,
               students[i].name,
               students[i].age,
               students[i].marks,
               students[i].department);
    }
    printf("--------------------------------------------------------------------------------\n");
    printf("Total Students: %d\n", studentCount);
}

// Search student by roll number
void searchStudent() {
    int rollNo;
    int found = 0;
    
    printf("\n=== Search Student ===\n");
    printf("Enter Roll Number to search: ");
    scanf("%d", &rollNo);
    
    for(int i = 0; i < studentCount; i++) {
        if(students[i].rollNo == rollNo) {
            printf("\nStudent Found!\n");
            printf("Roll Number: %d\n", students[i].rollNo);
            printf("Name: %s\n", students[i].name);
            printf("Age: %d\n", students[i].age);
            printf("Marks: %.2f\n", students[i].marks);
            printf("Department: %s\n", students[i].department);
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("\nStudent with Roll Number %d not found!\n", rollNo);
    }
}

// Update student record
void updateStudent() {
    int rollNo;
    int found = 0;
    
    printf("\n=== Update Student Record ===\n");
    printf("Enter Roll Number to update: ");
    scanf("%d", &rollNo);
    
    for(int i = 0; i < studentCount; i++) {
        if(students[i].rollNo == rollNo) {
            printf("\nCurrent Details:\n");
            printf("Name: %s\n", students[i].name);
            printf("Age: %d\n", students[i].age);
            printf("Marks: %.2f\n", students[i].marks);
            printf("Department: %s\n", students[i].department);
            
            printf("\nEnter New Details:\n");
            printf("Enter Name: ");
            scanf(" %[^\n]", students[i].name);
            
            printf("Enter Age: ");
            scanf("%d", &students[i].age);
            
            printf("Enter Marks: ");
            scanf("%f", &students[i].marks);
            
            printf("Enter Department: ");
            scanf(" %[^\n]", students[i].department);
            
            printf("\nStudent record updated successfully!\n");
            saveToFile();
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("\nStudent with Roll Number %d not found!\n", rollNo);
    }
}

// Delete student record
void deleteStudent() {
    int rollNo;
    int found = 0;
    
    printf("\n=== Delete Student Record ===\n");
    printf("Enter Roll Number to delete: ");
    scanf("%d", &rollNo);
    
    for(int i = 0; i < studentCount; i++) {
        if(students[i].rollNo == rollNo) {
            // Shift all records after this position
            for(int j = i; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            studentCount--;
            printf("\nStudent record deleted successfully!\n");
            saveToFile();
            found = 1;
            break;
        }
    }
    
    if(!found) {
        printf("\nStudent with Roll Number %d not found!\n", rollNo);
    }
}

// Load data from file
void loadFromFile() {
    FILE *file = fopen(FILENAME, "rb");
    
    if(file == NULL) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }
    
    fread(&studentCount, sizeof(int), 1, file);
    fread(students, sizeof(struct Student), studentCount, file);
    
    fclose(file);
    printf("Data loaded successfully. Total students: %d\n", studentCount);
}

// Save data to file
void saveToFile() {
    FILE *file = fopen(FILENAME, "wb");
    
    if(file == NULL) {
        printf("Error: Unable to save data!\n");
        return;
    }
    
    fwrite(&studentCount, sizeof(int), 1, file);
    fwrite(students, sizeof(struct Student), studentCount, file);
    
    fclose(file);
}
