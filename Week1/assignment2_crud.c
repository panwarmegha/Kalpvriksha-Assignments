#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "users.txt"

typedef struct
{
    int id;
    char name[50];
    int age;
} User;

void addUser();
void displayUsers();
void updateUser();
void deleteUser();

int main()
{
    int choice;
    do
    {
        printf("User Management System");
        printf("1. Add User 2. Display Users  3. Update User  4. Delete User  5. Exit");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addUser();
            break;
        case 2:
            displayUsers();
            break;
        case 3:
            updateUser();
            break;
        case 4:
            deleteUser();
            break;
        case 5:
            printf("Exiting program. Goodbye!");
            break;
        default:
            printf("Invalid choice. Please select a valid option.");
        }

        
        while (getchar() != " ");
    } while (choice != 5);

    return 0;
}

void addUser()
{
    FILE *file = fopen(DATA_FILE, "a");
    if (file == NULL)
    {
        printf("Error: Unable to open file for writing.");
        return;
    }

    User newUser;
    printf("Enter User ID: ");
    scanf("%d", &newUser.id);
    printf("Enter User Name (no spaces): ");
    scanf("%49s", newUser.name); 
    printf("Enter User Age: ");
    scanf("%d", &newUser.age);

    fprintf(file, "%d %s %d", newUser.id, newUser.name, newUser.age);
    fclose(file);

    printf("User added successfully.");
}

void displayUsers()
{
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL)
    {
        printf("No users found to display.");
        return;
    }

    User user;
    printf("%-5s %-15s %-5s", "ID", "Name", "Age");
    printf("-----------------------------");
    while (fscanf(file, "%d %49s %d", &user.id, user.name, &user.age) == 3)
    {
        printf("%-5d %-15s %-5d", user.id, user.name, user.age);
    }
    fclose(file);
}

void updateUser()
{
    FILE *file = fopen(DATA_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL)
    {
        printf("Error: Unable to open file.");
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        return;
    }

    int targetId, userFound = 0;
    printf("Enter User ID to update: ");
    scanf("%d", &targetId);

    User user;
    while (fscanf(file, "%d %49s %d", &user.id, user.name, &user.age) == 3)
    {
        if (user.id == targetId)
        {
            printf("Updating User ID %d", targetId);
            printf("Enter new Name (no spaces): ");
            scanf("%49s", user.name);
            printf("Enter new Age: ");
            scanf("%d", &user.age);
            userFound = 1;
        }
        fprintf(tempFile, "%d %s %d", user.id, user.name, user.age);
    }

    fclose(file);
    fclose(tempFile);

    remove(DATA_FILE);
    rename("temp.txt", DATA_FILE);

    if (userFound)
        printf("User updated successfully.");
    else
        printf("User with ID %d not found.", targetId);
}

void deleteUser()
{
    FILE *file = fopen(DATA_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL)
    {
        printf("Error: Unable to open file.");
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        return;
    }

    int targetId, userFound = 0;
    printf("Enter User ID to delete: ");
    scanf("%d", &targetId);

    User user;
    while (fscanf(file, "%d %49s %d", &user.id, user.name, &user.age) == 3)
    {
        if (user.id == targetId)
        {
            userFound = 1; 
        }
        else
        {
            fprintf(tempFile, "%d %s %d", user.id, user.name, user.age);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(DATA_FILE);
    rename("temp.txt", DATA_FILE);

    if (userFound)
        printf("User deleted successfully.");
    else
        printf("User with ID %d not found.", targetId);
}