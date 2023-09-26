#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 2048
#define MAX_FILENAME 63
#define MAX_FILE_SIZE 1024
#define MAX_FILES_IN_DIR 100
#define MAX_DIRS_IN_DIR 100

struct File {
    unsigned int id; // Unique identifier for the file
    char name[MAX_FILENAME];
    size_t size;
    char data[MAX_FILE_SIZE];
    struct Directory* directory;
};

struct Directory {
    char name[MAX_FILENAME];
    struct File files[MAX_FILES_IN_DIR];
    struct Directory* sub_dirs[MAX_DIRS_IN_DIR];
    unsigned char nf; // Number of files in the directory
    unsigned char nd; // Number of sub-directories in the directory
    char path[MAX_PATH];
};

// Function to overwrite the content of a file
void overwrite_to_file(struct File* file, const char* str) {
    if (file) {
        strcpy(file->data, str);
        file->size = strlen(str);
    }
}

// Function to append content to a file
void append_to_file(struct File* file, const char* str) {
    if (file) {
        strcat(file->data, str);
        file->size = strlen(file->data);
    }
}

// Function to print the path of a file
void printp_file(struct File* file) {
    if (file) {
        printf("%s", file->directory->path);
        printf("/%s\n", file->name);
    }
}

// Function to add a file to a directory
void add_file(struct File* file, struct Directory* dir) {
    if (file && dir) {
        if (dir->nf < MAX_FILES_IN_DIR) {
            dir->files[dir->nf] = *file;
            dir->nf++;
            file->directory = dir;
        } else {
            printf("Directory is full. Cannot add more files.\n");
        }
    }
}
// Function to create the root directory
struct Directory* create_root_directory() {
    struct Directory* root = (struct Directory*)malloc(sizeof(struct Directory));
    strcpy(root->name, "/");
    root->nf = 0;
    root->nd = 0;
    strcpy(root->path, "/"); // Initialize the path with "/"
    return root;
}


int main() {
    // Create the root directory
    struct Directory* root = create_root_directory();

// Create subdirectories
struct Directory* home = (struct Directory*)malloc(sizeof(struct Directory));
strcpy(home->name, "home");
home->nf = 0;
home->nd = 0;
sprintf(home->path, "%s/%s", root->path, home->name); // Use "/" as the separator
add_file(home, root);

struct Directory* bin = (struct Directory*)malloc(sizeof(struct Directory));
strcpy(bin->name, "bin");
bin->nf = 0;
bin->nd = 0;
sprintf(bin->path, "%s/%s", root->path, bin->name); // Use "/" as the separator
add_file(bin, root);



    // Create files
    struct File bash;
    bash.id = 1;
    strcpy(bash.name, "bash");
    strcpy(bash.data, "Bourne Again Shell!!");
    bash.size = strlen(bash.data);
    bash.directory = bin;
    add_file(&bash, bin);

    struct File ex3_1;
    ex3_1.id = 2;
    strcpy(ex3_1.name, "ex3_1.c");
    strcpy(ex3_1.data, "int printf(const char * format, ...);");
    ex3_1.size = strlen(ex3_1.data);
    ex3_1.directory = home;
    add_file(&ex3_1, home);

    struct File ex3_2;
    ex3_2.id = 3;
    strcpy(ex3_2.name, "ex3_2.c");
    strcpy(ex3_2.data, "// This is a comment in C language");
    ex3_2.size = strlen(ex3_2.data);
    ex3_2.directory = home;
    add_file(&ex3_2, home);

    // Append content to ex3_1.c
    append_to_file(&ex3_1, "int main(){printf(\"Hello World!\");}");

    // Print the paths of all files
    printf("Paths of all files:\n");
    printp_file(&bash);
    printp_file(&ex3_1);
    printp_file(&ex3_2);



    // Free allocated memory
    free(home);
    free(bin);
    free(root);

    return 0;
}
