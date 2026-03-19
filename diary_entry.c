#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TEXT 500

typedef struct {
    int id;
    char timestamp[30];
    char note[MAX_TEXT];
} DiaryEntry;

void addEntry();
void viewEntries();
void deleteEntry();
void getTimestamp(char *buffer);

const char *FILENAME = "my_diary.dat";

int main() {
    int choice;

    while (1) {
        printf("\n--- Digital Personal Diary ---\n");
        printf("1. Add Entry\n2. View All Entries\n3. Delete Entry\n4. Exit\n");
        printf("Selection: ");
        scanf("%d", &choice);
        getchar(); 
        switch (choice) {
            case 1: addEntry(); break;
            case 2: viewEntries(); break;
            case 3: deleteEntry(); break;
            case 4: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

void getTimestamp(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", tm);
}

void addEntry() {
    FILE *file = fopen(FILENAME, "ab"); 
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    DiaryEntry entry;
    
    fseek(file, 0, SEEK_END);
    entry.id = (int)(ftell(file) / sizeof(DiaryEntry)) + 1;

    getTimestamp(entry.timestamp);

    printf("Enter your diary note (max %d chars):\n", MAX_TEXT);
    fgets(entry.note, MAX_TEXT, stdin);
    entry.note[strcspn(entry.note, "\n")] = 0; 
    fwrite(&entry, sizeof(DiaryEntry), 1, file);
    fclose(file);
    printf("Entry saved successfully!\n");
}

void viewEntries() {
    FILE *file = fopen(FILENAME, "rb"); 
    if (!file) {
        printf("\nNo entries found. Start writing today!\n");
        return;
    }

    DiaryEntry entry;
    printf("\n--- Your Entries ---\n");
    while (fread(&entry, sizeof(DiaryEntry), 1, file)) {
        printf("[%d] %s\n   %s\n\n", entry.id, entry.timestamp, entry.note);
    }
    fclose(file);
}

void deleteEntry() {
    int targetId, found = 0;
    printf("Enter the ID of the entry to delete: ");
    scanf("%d", &targetId);

    FILE *file = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (!file || !temp) {
        printf("Error processing deletion.\n");
        return;
    }

    DiaryEntry entry;
    while (fread(&entry, sizeof(DiaryEntry), 1, file)) {
        if (entry.id == targetId) {
            found = 1; 
        } else {
            fwrite(&entry, sizeof(DiaryEntry), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.dat", FILENAME);

    if (found) printf("Entry deleted.\n");
    else printf("Entry ID not found.\n");
}