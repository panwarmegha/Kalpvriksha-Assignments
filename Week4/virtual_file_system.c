#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define MIN_BLOCKS 1
#define MAX_NUM_BLOCKS 5000
#define MAX_FILENAME_LEN 50
#define MAX_WRITE_BUFFER 2048
#define MAX_BLOCKS_PER_FILE 100
#define MAX_INPUT_LEN 128

char **virtualDisk;

typedef struct FreeBlock {
    int index;
    struct FreeBlock *next;
    struct FreeBlock *previous;
} FreeBlock;

typedef struct File {
    char name[MAX_FILENAME_LEN];
    int isDirectory;
    struct File *parent;
    struct File *next;
    struct File *previous;
    struct File *children;
    int blockCount;
    int blocks[MAX_BLOCKS_PER_FILE];
    int size;
} File;

File *root = NULL;
File *cwd = NULL;
FreeBlock *freeBlockHead = NULL;
int usedBlocks = 0;

void initDisk();
void initFreeBlocks();
void initFileSystem();
void readWriteBuffer(char *dataInput);
File* findChild(char *name);
int getFreeBlock();
void releaseBlock(int index);

void mkdirCmd(char *name);
void rmdirCmd(char *name);
void lsCmd();
void cdCmd(char *name);
void printPath(File *directory);
void createCmd(char *name);
void writeCmd(char *name, char *data);
void readCmd(char *name);
void deleteCmd(char *name);
void dfCmd();

void freeAll(File *directory);
void cleanup();

void processCommand(char *cmd, char *arg1, char *arg2);
void readUserInput(char *cmd, char *arg1, char *arg2);


int main() 
{
    initFileSystem();
    char cmd[MAX_INPUT_LEN], nameArg[MAX_INPUT_LEN], writeBuffer[MAX_WRITE_BUFFER];
    while (1) 
    {
        printf("%s > ", (cwd == root) ? "/" : cwd->name);
        readUserInput(cmd, nameArg, writeBuffer);
        if (strcmp(cmd, "exit") == 0) 
        {
            cleanup();
            printf("Memory released. Exiting program...\n");
            break;
        }
        processCommand(cmd, nameArg, writeBuffer);
    }
    return 0;
}


void initDisk() 
{
    virtualDisk = malloc(MAX_NUM_BLOCKS * sizeof(char *));
    for (int index = 0; index < MAX_NUM_BLOCKS; index++)
        virtualDisk[index] = calloc(BLOCK_SIZE, sizeof(char));
}

void initFreeBlocks() 
{
    FreeBlock *previous = NULL;
    for (int index = 0; index < MAX_NUM_BLOCKS; index++) {
        FreeBlock *node = malloc(sizeof(FreeBlock));
        node->index = index;
        node->next = NULL;
        node->previous = previous;
        if (previous) previous->next = node;
        else freeBlockHead = node;
        previous = node;
    }
}

void initFileSystem() 
{
    initDisk();
    initFreeBlocks();
    root = malloc(sizeof(File));
    strcpy(root->name, "/");
    root->isDirectory = 1;
    root->parent = NULL;
    root->next = root->previous = root;
    root->children = NULL;
    root->blockCount = 0;
    root->size = 0;
    cwd = root;
    printf("Virtual File System initialized successfully.\n");
}

void readWriteBuffer(char *dataInput) 
{
    char *ptr = dataInput;
    int ch = getchar();
    while (ch == ' ' || ch == '\t') ch = getchar();
    if (ch == '\n' || ch == EOF)
    { 
        *dataInput = '\0'; return; 
    }
    if (ch == '"') 
    {
        while ((ch = getchar()) != '"' && ch != EOF && (ptr - dataInput) < MAX_WRITE_BUFFER - 1)
            *ptr++ = (char)ch;
    } 
    else 
    {
        *ptr++ = (char)ch;
        while ((ch = getchar()) != '\n' && ch != EOF && (ptr - dataInput) < MAX_WRITE_BUFFER - 1)
            *ptr++ = (char)ch;
    }
    *ptr = '\0';
}

File* findChild(char *name) 
{
    if (!cwd->children) return NULL;
    File *current = cwd->children;
    do {
        if (strcmp(current->name, name) == 0) return current;
        current = current->next;
    } while (current != cwd->children);
    return NULL;
}

int getFreeBlock() 
{
    if (!freeBlockHead) return -1;
    int idx = freeBlockHead->index;
    FreeBlock *tmp = freeBlockHead;
    freeBlockHead = freeBlockHead->next;
    if (freeBlockHead) freeBlockHead->previous = NULL;
    free(tmp);
    usedBlocks++;
    return idx;
}

void releaseBlock(int index) 
{
    FreeBlock *node = malloc(sizeof(FreeBlock));
    node->index = index;
    node->next = freeBlockHead;
    node->previous = NULL;
    if (freeBlockHead) freeBlockHead->previous = node;
    freeBlockHead = node;
    usedBlocks--;
}

void printPath(File *directory) 
{
    if (!directory) return;
    if (directory->parent) 
    {
        printPath(directory->parent);
        if (strcmp(directory->name, "/") != 0)
            printf("/%s", directory->name);
    } 
    else printf("/");
}

void mkdirCmd(char *name) {
    if (strlen(name) > MAX_FILENAME_LEN) 
    { 
        printf("Error: Name too long.\n"); 
        return; 
    }
    if (findChild(name)) 
    { 
        printf("Directory already exists.\n"); 
        return; 
    }
    File *newDirectory = malloc(sizeof(File));
    strcpy(newDirectory->name, name);
    newDirectory->isDirectory = 1;
    newDirectory->parent = cwd;
    newDirectory->children = NULL;
    newDirectory->blockCount = 0;
    newDirectory->size = 0;
    if (!cwd->children) 
    {
        cwd->children = newDirectory;
        newDirectory->next = newDirectory->previous = newDirectory;
    } 
    else 
    {
        File *first = cwd->children;
        File *last = first->previous;
        newDirectory->next = first;
        newDirectory->previous = last;
        last->next = newDirectory;
        first->previous = newDirectory;
    }
    printf("Directory '%s' created.\n", name);
}

void rmdirCmd(char *name) 
{
    File *target = findChild(name);
    if (!target) 
    { 
        printf("Directory not found.\n"); 
        return; 
    }
    if (!target->isDirectory) 
    { 
        printf("'%s' is a file.\n", name); 
        return; 
    }
    if (target->children) 
    { 
        printf("Directory not empty.\n"); 
        return; 
    }
    if (target == cwd->children && target->next == target) cwd->children = NULL;
    else 
    {
        if (target == cwd->children) cwd->children = target->next;
        target->previous->next = target->next;
        target->next->previous = target->previous;
    }
    free(target);
    printf("Directory removed.\n");
}

void lsCmd() 
{
    if (!cwd->children) 
    { 
        printf("(empty)\n"); 
        return; 
    }
    File *current = cwd->children;
    do {
        printf("%s%s  ", current->name, current->isDirectory ? "/" : "");
        current = current->next;
    } while (current != cwd->children);
    printf("\n");
}

void cdCmd(char *name) 
{
    if (strcmp(name, "..") == 0) 
    {
        if (cwd->parent) cwd = cwd->parent;
        else printf("Already at root.\n");
        return;
    }
    File *target = findChild(name);
    if (!target || !target->isDirectory) 
    { 
        printf("Directory not found.\n"); 
        return; 
    }
    cwd = target;
}

void createCmd(char *name) 
{
    if (strlen(name) > MAX_FILENAME_LEN) 
    { 
        printf("Error: Name too long.\n"); 
        return; 
    }
    if (findChild(name)) 
    { 
        printf("File already exists.\n"); 
        return; 
    }
    File *newFile = malloc(sizeof(File));
    strcpy(newFile->name, name);
    newFile->isDirectory = 0;
    newFile->parent = cwd;
    newFile->blockCount = 0;
    newFile->size = 0;
    newFile->children = NULL;
    if (!cwd->children) 
    {
        cwd->children = newFile;
        newFile->next = newFile->previous = newFile;
    } 
    else 
    {
        File *first = cwd->children;
        File *last = first->previous;
        newFile->next = first;
        newFile->previous = last;
        last->next = newFile;
        first->previous = newFile;
    }
    printf("File '%s' created.\n", name);
}

void writeCmd(char *name, char *data) 
{
    File *target = findChild(name);
    if (!target || target->isDirectory) 
    { 
        printf("File not found.\n"); 
        return; 
    }
    for (int index = 0; index < target->blockCount; index++)
        releaseBlock(target->blocks[index]);
    target->blockCount = 0;
    target->size = 0;
    int bytes = strlen(data);
    if (bytes == 0) 
    { 
        printf("Empty data.\n"); 
        return; 
    }
    int blocksNeeded = (bytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (blocksNeeded > MAX_BLOCKS_PER_FILE) 
    {
        printf("File too large.\n");
        return;
    }
    for (int index = 0; index < blocksNeeded; index++) 
    {
        int blk = getFreeBlock();
        if (blk == -1) { printf("Disk full.\n"); return; }
        target->blocks[target->blockCount++] = blk;
        int remaining = bytes - index * BLOCK_SIZE;
        int toCopy = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        memcpy(virtualDisk[blk], data + index * BLOCK_SIZE, toCopy);
    }
    target->size = bytes;
    printf("Data written successfully (%d bytes).\n", bytes);
}

void readCmd(char *name) 
{
    File *target = findChild(name);
    if (!target || target->isDirectory) 
    { 
        printf("File not found.\n"); 
        return; 
    }
    if (target->blockCount == 0) 
    { 
        printf("(empty)\n"); 
        return; 
    }
    int remaining = target->size;
    for (int index = 0; index < target->blockCount; index++) 
    {
        int toRead = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;
        fwrite(virtualDisk[target->blocks[index]], 1, toRead, stdout);
        remaining -= toRead;
    }
    printf("\n");
}

void deleteCmd(char *name) 
{
    File *target = findChild(name);
    if (!target) 
    { 
        printf("File not found.\n"); 
        return; 
    }
    if (target->isDirectory) 
    { 
        printf("'%s' is a directory.\n", name); 
        return; 
    }
    for (int index = 0; index < target->blockCount; index++)
        releaseBlock(target->blocks[index]);
    if (target == cwd->children && target->next == target) cwd->children = NULL;
    else 
    {
        if (target == cwd->children) cwd->children = target->next;
        target->previous->next = target->next;
        target->next->previous = target->previous;
    }
    free(target);
    printf("File deleted.\n");
}

void dfCmd() 
{
    printf("Total Blocks: %d\nUsed Blocks: %d\nFree Blocks: %d\nDisk Usage: %.2f%%\n",
           MAX_NUM_BLOCKS, usedBlocks, MAX_NUM_BLOCKS - usedBlocks,
           (usedBlocks * 100.0) / MAX_NUM_BLOCKS);
}

void freeAll(File *directory) 
{
    if (directory->isDirectory && directory->children) 
    {
        File *first = directory->children;
        first->previous->next = NULL;
        File *current = first;
        while (current) {
            File *next = current->next;
            freeAll(current);
            current = next;
        }
    }
    free(directory);
}

void cleanup() 
{
    if (!root) return;
    freeAll(root);
    FreeBlock *current = freeBlockHead;
    while (current) {
        FreeBlock *next = current->next;
        free(current);
        current = next;
    }
    for (int index = 0; index < MAX_NUM_BLOCKS; index++) free(virtualDisk[index]);
    free(virtualDisk);
}

void readUserInput(char *cmd, char *arg1, char *arg2) 
{
    if (scanf("%s", cmd) != 1) return;
    if (strcmp(cmd, "write") == 0) 
    {
        scanf("%s", arg1);
        readWriteBuffer(arg2);
    } 
    else if (strcmp(cmd, "mkdir") == 0 ||
               strcmp(cmd, "rmdir") == 0 ||
               strcmp(cmd, "cd") == 0 ||
               strcmp(cmd, "create") == 0 ||
               strcmp(cmd, "read") == 0 ||
               strcmp(cmd, "delete") == 0) 
               {
        scanf("%s", arg1);
        arg2[0] = '\0';
    }
    else 
    {
        arg1[0] = arg2[0] = '\0';
    }
}

void processCommand(char *cmd, char *arg1, char *arg2) 
{
    if (strcmp(cmd, "mkdir") == 0) mkdirCmd(arg1);
    else if (strcmp(cmd, "rmdir") == 0) rmdirCmd(arg1);
    else if (strcmp(cmd, "ls") == 0) lsCmd();
    else if (strcmp(cmd, "cd") == 0) cdCmd(arg1);
    else if (strcmp(cmd, "pwd") == 0) 
    { 
        printPath(cwd); 
        printf("\n"); 
    }
    else if (strcmp(cmd, "create") == 0) createCmd(arg1);
    else if (strcmp(cmd, "write") == 0) writeCmd(arg1, arg2);
    else if (strcmp(cmd, "read") == 0) readCmd(arg1);
    else if (strcmp(cmd, "delete") == 0) deleteCmd(arg1);
    else if (strcmp(cmd, "df") == 0) dfCmd();
    else if (strcmp(cmd, "exit") == 0) return;
    else 
    {
        printf("Unknown command.\n");
        int c; while ((c = getchar()) != '\n' && c != EOF);
    }
}