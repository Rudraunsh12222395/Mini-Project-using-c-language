#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define USERNAME "kiran"
#define PASSWORD "123"
#define COURSE_NAME_LENGTH 50
#define STUDENT_NAME_LENGTH 50
struct student {
    char name [STUDENT_NAME_LENGTH];
    int roll_no;
    int num_courses;
    char course_names[5][COURSE_NAME_LENGTH];
    int total_lectures[5];
    int lectures_attended[5];
};
void display_student_details(struct student s);
void insert_student_details();
void display_all_details();
void search_student_details();
void delete_student_details();
void modify_student_details();
int main() {
    int choice;
    char username[50];
    char password[50];
    printf("Welcome to Student Attendance Calculator!\n");
    printf("Please enter your username: ");
    scanf("%s", username);
    printf("Please enter your password: ");
    scanf("%s", password);
    if (strcmp(username, USERNAME) == 0 && strcmp(password, PASSWORD) == 0) {
        printf("Login successful!\n");
    } else {
        printf("Login failed! Exiting program.\n");
        return 0;
    }
    while (1) {
        printf("1. Insert student details\n");
        printf("2. Display all details\n");
        printf("3. Search specific information\n");
        printf("4. Delete student detail\n");
        printf("5. Modify student detail\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            scanf("%*[^\n]"); // clear input buffer
            continue;
        }
        getchar(); // consume the newline character left in the input buffer
        switch (choice) {
            case 1:
                insert_student_details();
                break;
            case 2:
                display_all_details();
                break;
            case 3:
                search_student_details();
                break;
            case 4:
                delete_student_details();
                break;
            case 5:
                modify_student_details();
                break;
            case 6:
                printf("Thank you for using Student Attendance Calculator!\n");
                exit(0);
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
    }
    return 0;
}
void insert_student_details() {
    struct student s;
    FILE *fp;
    int i;
    fp = fopen("student.txt", "ab+");
    printf("Enter name: ");
    fgets(s.name, STUDENT_NAME_LENGTH, stdin);
    printf("Enter roll no: ");
    scanf("%d", &s.roll_no);
    getchar();
    printf("Enter the number of courses (max 5): ");
    scanf("%d", &s.num_courses);
    getchar();
    if (s.num_courses > 5) {
        printf("Number of courses cannot be more than 5. Setting it to 5.\n");
        s.num_courses = 5;
    }
    for (i = 0; i < s.num_courses; i++) {
        printf("Enter name of course %d: ", i+1);
        scanf("%s", s.course_names[i]);
        printf("Enter total number of lectures for course %d: ", i+1);
        scanf("%d", &s.total_lectures[i]);
        printf("Enter number of lectures attended for course %d: ", i+1);
        scanf("%d", &s.lectures_attended[i]);
        getchar();
    }
    fwrite(&s, sizeof(struct student), 1, fp);
    printf("Record inserted successfully!\n");
    fclose(fp);
}
void display_student_details(struct student s) {
    int i;
    printf("    ----------------------------\n");
    printf("Name: %s", s.name);
    printf("Roll no: %d\n", s.roll_no);
    printf("Course-wise attendance:\n");
    float total_attended = 0, total_lectures = 0;
    for (i = 0; i < s.num_courses; i++) {
        float attendance_percentage = (float)s.lectures_attended[i] / s.total_lectures[i] * 100;
        printf("%s: %.2f%%\n", s.course_names[i], attendance_percentage);
        total_attended += s.lectures_attended[i];
        total_lectures += s.total_lectures[i];
    }
    printf("Aggregate attendance: %.2f%%\n", (total_attended / total_lectures) * 100);
    printf("    ----------------------------\n");
}
void display_all_details() {
    FILE *fp;
    struct student s;
    fp = fopen("student.txt", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }
    while (fread(&s, sizeof(struct student), 1, fp)) {
        display_student_details(s);
    }
    fclose(fp);
}
void search_student_details() {
    FILE *fp;
    struct student s;
    int roll_no;
    int flag = 0;
    fp = fopen("student.txt", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }
    printf("Enter roll no to search: ");
    scanf("%d", &roll_no);
    while (fread(&s, sizeof(struct student), 1, fp)) {
        if (s.roll_no == roll_no) {
            display_student_details(s);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf("No record found with roll no %d!\n", roll_no);
    }
    fclose(fp);
}
void delete_student_details() {
    FILE *fp, *ft;
    struct student s;
    int roll_no;
    int flag = 0;
    fp = fopen("student.txt", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }
    ft = fopen("temp.txt", "wb");
    printf("Enter roll no to delete: ");
    scanf("%d", &roll_no);
    while (fread(&s, sizeof(struct student), 1, fp)) {
        if (s.roll_no == roll_no) {
            flag = 1;
            continue;
        }
        fwrite(&s, sizeof(struct student), 1, ft);
    }
    if (flag == 0) {
        printf("No record found with roll no %d!\n", roll_no);
    }
    else {
        printf("Record deleted successfully!\n");
    }
    fclose(fp);
    fclose(ft);
    remove("student.txt");
    rename("temp.txt", "student.txt");
}
void modify_student_details() {
    FILE *fp;
    struct student s;
    int roll_no;
    int flag = 0;
    int i;
    fp = fopen("student.txt", "rb+");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }
    printf("Enter roll no to modify: ");
    scanf("%d", &roll_no);
    getchar();
    while (fread(&s, sizeof(struct student), 1, fp)) {
        if (s.roll_no == roll_no) {
            printf("Enter name: ");
            fgets(s.name, STUDENT_NAME_LENGTH, stdin);
            printf("Enter roll no: ");
            scanf("%d", &s.roll_no);
            printf("Enter the number of courses (max 5): ");
            scanf("%d", &s.num_courses);
            getchar();
            if (s.num_courses > 5) {
                printf("Number of courses cannot be more than 5. Setting it to 5.\n");
                s.num_courses = 5;
            }
            for (i = 0; i < s.num_courses; i++) {
                printf("Enter name of course %d: ", i+1);
                fgets(s.course_names[i], COURSE_NAME_LENGTH, stdin);
                printf("Enter total number of lectures for course %d: ", i+1);
                scanf("%d", &s.total_lectures[i]);
                printf("Enter number of lectures attended for course %d: ", i+1);
                scanf("%d", &s.lectures_attended[i]);
            }
            fseek(fp, -sizeof(struct student), SEEK_CUR);
            fwrite(&s, sizeof(struct student), 1, fp);
            flag = 1;
            break;
        }
    }
    if (flag == 0) {
        printf("No record found with roll no %d!\n", roll_no);
    }
    else {
        printf("Record modified successfully!\n");
    }
    fclose(fp);
}
