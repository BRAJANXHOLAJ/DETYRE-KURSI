#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

// Structure for Student
typedef struct {
    int id;
    char name[50];
    char surname[50];
    char dob[20];
} Student;

// Structure for Instructor
typedef struct {
    int id;
    char name[50];
    char surname[50];
    char dob[20];
    char courses[100]; // Stores categories of courses the instructor teaches
} Instructor;

// Structure for Course
typedef struct {
    int id;
    char start_date[20];
    char days[20];
    char start_time[20];
    char category[20];
    int instructor_id;
    int num_students;
    int max_students;
} Course;

// Function to register a student
void registerStudent() {
    FILE *fp;
    Student s;

    int go_back = 0;
    int option;
    
    printf("Enter 1 to go back to the main menu: ");
    scanf("%d", &option);
    if (option == 1) {
        go_back = 1;
        return;
    }

    printf("Enter student ID: ");
    scanf("%d", &s.id);

    printf("Enter student name: ");
    scanf("%s", s.name);

    printf("Enter student surname: ");
    scanf("%s", s.surname);

    printf("Enter student date of birth: ");
    scanf("%s", s.dob);

    fp = fopen("students.dat", "ab");
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
}

// Function to create a new instructor
void createInstructor() {
    FILE *fp;
    Instructor i;

    printf("Enter instructor ID: ");
    scanf("%d", &i.id);

    printf("Enter instructor name: ");
    scanf("%s", i.name);

    printf("Enter instructor surname: ");
    scanf("%s", i.surname);

    printf("Enter instructor date of birth: ");
    scanf("%s", i.dob);

    printf("Enter categories of courses the instructor teaches (separated by commas): ");
    scanf("%s", i.courses);

    fp = fopen("instructors.dat", "ab");
    fwrite(&i, sizeof(Instructor), 1, fp);
    fclose(fp);

    printf("Instructor created successfully!\n");
}

// Function to create a new course
void createCourse() {
    FILE *fp;
    Course c;

    printf("Enter course ID: ");
    scanf("%d", &c.id);

    printf("Enter start date (MM/DD/YYYY): ");
    scanf("%s", c.start_date);

    printf("Enter days (e.g., Monday, Wednesday): ");
    scanf("%s", c.days);

    printf("Enter start time (HH:MM AM/PM): ");
    scanf("%s", c.start_time);

    printf("Enter category (Robotics/Programming/GraphicDesign): ");
    scanf("%s", c.category);

    printf("Enter instructor ID: ");
    scanf("%d", &c.instructor_id);

    printf("Enter maximum number of students: ");
    scanf("%d", &c.max_students);

    c.num_students = 0; // Initially, no students are registered

    fp = fopen("courses.dat", "ab");
    fwrite(&c, sizeof(Course), 1, fp);
    fclose(fp);

    printf("Course created successfully!\n");
}

// Function to register a student for a course
void registerStudentForCourse() {
    FILE *fp;
    Student s;
    Course c;

    int student_id, course_id;

    printf("Enter student ID: ");
    scanf("%d", &student_id);

    printf("Enter course ID: ");
    scanf("%d", &course_id);

    fp = fopen("students.dat", "rb+");
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.id == student_id) {
            fclose(fp);
            fp = fopen("courses.dat", "rb+");
            while (fread(&c, sizeof(Course), 1, fp)) {
                if (c.id == course_id) {
                    if (c.num_students < c.max_students) {
                        c.num_students++;
                        fseek(fp, -sizeof(Course), SEEK_CUR);
                        fwrite(&c, sizeof(Course), 1, fp);
                        fclose(fp);

                        printf("Student registered for the course successfully!\n");
                        return;
                    } else {
                        printf("This group is full, choose another course!\n");
                        fclose(fp);
                        return;
                    }
                }
            }
            fclose(fp);
            printf("Course not found!\n");
            return;
        }
    }
    fclose(fp);
    printf("Student not found!\n");
}

// Function to delete an instructor
void deleteInstructor() {
    FILE *fp, *temp;
    Instructor i;
    int instructor_id, found = 0;

    printf("Enter instructor ID to delete: ");
    scanf("%d", &instructor_id);

    fp = fopen("instructors.dat", "rb");
    temp = fopen("temp.dat", "wb");
    while (fread(&i, sizeof(Instructor), 1, fp)) {
        if (i.id == instructor_id) {
            found = 1;
            continue;
        }
        fwrite(&i, sizeof(Instructor), 1, temp);
    }
    fclose(fp);
    fclose(temp);

    remove("instructors.dat");
    rename("temp.dat", "instructors.dat");

    if (found)
        printf("Instructor deleted successfully!\n");
    else
        printf("Instructor not found!\n");
}

// Function to delete a course
void deleteCourse() {
    FILE *fp, *temp;
    Course c;
    int course_id, found = 0;

    printf("Enter course ID to delete: ");
    scanf("%d", &course_id);

    fp = fopen("courses.dat", "rb");
    temp = fopen("temp.dat", "wb");
    while (fread(&c, sizeof(Course), 1, fp)) {
        if (c.id == course_id) {
            found = 1;
            continue;
        }
        fwrite(&c, sizeof(Course), 1, temp);
    }
    fclose(fp);
    fclose(temp);

    remove("courses.dat");
    rename("temp.dat", "courses.dat");

    if (found)
        printf("Course deleted successfully!\n");
    else
        printf("Course not found!\n");
}

// Function to deregister a student from a course
void deregisterStudentFromCourse() {
    FILE *fp, *temp;
    Student s;
    Course c;
    int student_id, course_id, found = 0;

    printf("Enter student ID: ");
    scanf("%d", &student_id);

    printf("Enter course ID: ");
    scanf("%d", &course_id);

    fp = fopen("students.dat", "rb");
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (s.id == student_id) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("Student not found!\n");
        return;
    }

    found = 0;

    fp = fopen("courses.dat", "rb");
    temp = fopen("temp.dat", "wb");
    while (fread(&c, sizeof(Course), 1, fp)) {
        if (c.id == course_id) {
            found = 1;
            if (c.num_students > 0) {
                c.num_students--;
                fseek(fp, -sizeof(Course), SEEK_CUR);
                fwrite(&c, sizeof(Course), 1, temp);
                printf("Student deregistered from the course successfully!\n");
            } else {
                printf("No students registered for this course!\n");
            }
        } else {
            fwrite(&c, sizeof(Course), 1, temp);
        }
    }
    fclose(fp);
    fclose(temp);

    if (!found)
        printf("Course not found!\n");

    remove("courses.dat");
    rename("temp.dat", "courses.dat");
}

// Function to list students in a course
void listStudentsInCourse() {
    FILE *fp;
    Course c;

    int course_id, found = 0;

    printf("Enter course ID: ");
    scanf("%d", &course_id);

    fp = fopen("courses.dat", "rb");
    while (fread(&c, sizeof(Course), 1, fp)) {
        if (c.id == course_id) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("Course not found!\n");
        return;
    }

    printf("Students in Course %d:\n", c.id);
    // You can implement logic to list students enrolled in this course
}

// Function to list courses for an instructor
void listCoursesForInstructor() {
    FILE *fp;
    Course c;

    int instructor_id, found = 0;

    printf("Enter instructor ID: ");
    scanf("%d", &instructor_id);

    fp = fopen("courses.dat", "rb");
    while (fread(&c, sizeof(Course), 1, fp)) {
        if (c.instructor_id == instructor_id) {
            found = 1;
            // Print course details here
        }
    }
    fclose(fp);

    if (!found)
        printf("No courses found for the instructor!\n");
}

// Function to list students for an instructor
void listStudentsForInstructor() {
    FILE *fp;
    Student s;
    Course c;

    int instructor_id, found = 0;

    printf("Enter instructor ID: ");
    scanf("%d", &instructor_id);

    fp = fopen("courses.dat", "rb");
    while (fread(&c, sizeof(Course), 1, fp)) {
        if (c.instructor_id == instructor_id) {
            found = 1;
            // List students for this course
            // You can implement this functionality similar to listStudentsInCourse function
        }
    }
    fclose(fp);

    if (!found)
        printf("No students found for the instructor!\n");
}

// Function to list all courses
void listAllCourses() {
    FILE *fp;
    Course c;

    fp = fopen("courses.dat", "rb");
    if (fp == NULL) {
        printf("No courses found!\n");
        return;
    }

    printf("List of All Courses:\n");
    printf("ID\tStart Date\tDays\tStart Time\tCategory\tInstructor ID\tMax Students\tNum Students\n");
    while (fread(&c, sizeof(Course), 1, fp)) {
        printf("%d\t%s\t%s\t%s\t%s\t%d\t%d\t%d\n", c.id, c.start_date, c.days, c.start_time, c.category, c.instructor_id, c.max_students, c.num_students);
    }
    fclose(fp);
}

// Function to list all students
void listAllStudents() {
    FILE *fp;
    Student s;

    fp = fopen("students.dat", "rb");
    if (fp == NULL) {
        printf("No students found!\n");
        return;
    }

    printf("List of All Students:\n");
    printf("ID\tName\tSurname\tDOB\n");
    while (fread(&s, sizeof(Student), 1, fp)) {
        printf("%d\t%s\t%s\t%s\n", s.id, s.name, s.surname, s.dob);
    }
    fclose(fp);
}

// Function to list all instructors
void listAllInstructors() {
    FILE *fp;
    Instructor i;

    fp = fopen("instructors.dat", "rb");
    if (fp == NULL) {
        printf("No instructors found!\n");
        return;
    }

    printf("List of All Instructors:\n");
    printf("ID\tName\tSurname\tDOB\tCourses\n");
    while (fread(&i, sizeof(Instructor), 1, fp)) {
        printf("%d\t%s\t%s\t%s\t%s\n", i.id, i.name, i.surname, i.dob, i.courses);
    }
    fclose(fp);
}

// Main menu function
void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Register Student\n");
    printf("2. Create Instructor\n");
    printf("3. Create Course\n");
    printf("4. Register Student for Course\n");
    printf("5. Delete Instructor\n");
    printf("6. Delete Course\n");
    printf("7. Deregister Student from Course\n");
    printf("8. List All Courses\n");
    printf("9. List All Students\n");
    printf("10. List All Instructors\n");
    printf("11. List Students in a Course\n");
    printf("12. List Courses for an Instructor\n");
    printf("13. List Students for an Instructor\n");
    printf("14. Exit\n");
}

int main() {
    int choice;
    int go_back;
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerStudent();
                
                break;
            case 2:
                createInstructor();
                break;
            case 3:
                createCourse();
                break;
            case 4:
                registerStudentForCourse();
                break;
            case 5:
                deleteInstructor();
                break;
            case 6:
                deleteCourse();
                break;
            case 7:
                deregisterStudentFromCourse();
                break;
            case 8:
                listAllCourses();
                break;
            case 9:
                listAllStudents();
                break;
            case 10:
                listAllInstructors();
                break;
            case 11:
                listStudentsInCourse();
                break;
            case 12:
                listCoursesForInstructor();
                break;
            case 13:
                listStudentsForInstructor();
                break;
            case 14:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 14);

    return 0;
}
