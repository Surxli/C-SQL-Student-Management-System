#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

sqlite3 *d#b;

// Callback function to display data
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// Create table
void createTable() {
    char *errMsg = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS students ("
                      "id INTEGER PRIMARY KEY,"
                      "name TEXT,"
                      "age INTEGER,"
                      "marks REAL);";

    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        printf("Error creating table: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Table ready.\n");
    }
}

// Insert student
void insertStudent() {
    int id, age;
    float marks;
    char name[50];

    printf("Enter ID Name Age Marks: ");
    scanf("%d %s %d %f", &id, name, &age, &marks);

    char sql[200];
    sprintf(sql, "INSERT INTO students VALUES(%d,'%s',%d,%f);",
            id, name, age, marks);

    if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
        printf("Insert failed (maybe duplicate ID).\n");
    } else {
        printf("Student added successfully.\n");
    }
}

// Display all students
void displayStudents() {
    char *errMsg = 0;
    const char *sql = "SELECT * FROM students;";

    printf("\n--- Student Records ---\n");

    if (sqlite3_exec(db, sql, callback, 0, &errMsg) != SQLITE_OK) {
        printf("Error fetching data: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

// Search student
void searchStudent() {
    int id;
    printf("Enter ID to search: ");
    scanf("%d", &id);

    char sql[100];
    sprintf(sql, "SELECT * FROM students WHERE id=%d;", id);

    sqlite3_exec(db, sql, callback, 0, 0);
}

// Update marks
void updateStudent() {
    int id;
    float marks;

    printf("Enter ID and new marks: ");
    scanf("%d %f", &id, &marks);

    char sql[150];
    sprintf(sql, "UPDATE students SET marks=%f WHERE id=%d;", marks, id);

    if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
        printf("Update failed.\n");
    } else {
        printf("Marks updated successfully.\n");
    }
}

// Delete student
void deleteStudent() {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    char sql[100];
    sprintf(sql, "DELETE FROM students WHERE id=%d;", id);

    if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
        printf("Delete failed.\n");
    } else {
        printf("Student deleted successfully.\n");
    }
}

// Main function
int main() {
    if (sqlite3_open("students.db", &db)) {
        printf("Can't open database\n");
        return 0;
    }

    createTable();

    int choice;

    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Insert Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Marks\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        switch (choice) {
            case 1: insertStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6:
                sqlite3_close(db);
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}