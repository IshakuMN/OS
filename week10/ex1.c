#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h>

// Function to create a file and its hard links
void create_file_and_links(const char *path)
{
    char filepath[PATH_MAX];
    snprintf(filepath, PATH_MAX, "%s/myfile1.txt", path);
    int fd = open(filepath, O_CREAT | O_WRONLY, 0644);
    if (fd < 0)
    {
        perror("open");
        return;
    }
    write(fd, "Hello world.\n", 13);

    // Create hard links
    char linkpath[PATH_MAX];
    snprintf(linkpath, PATH_MAX, "%s/myfile11.txt", path);
    link(filepath, linkpath);

    snprintf(linkpath, PATH_MAX, "%s/myfile12.txt", path);
    link(filepath, linkpath);

    close(fd);
}

// Helper function to find hard links of a file
void find_hard_links(const char *dir_path, ino_t inode_to_find)
{
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char path[1024];

    if (!(dir = opendir(dir_path)))
    {
        perror("Unable to open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);

        if (stat(path, &statbuf) == -1)
        {
            perror("Failed to get file stats");
            continue;
        }

        if (statbuf.st_ino == inode_to_find)
        {
            printf("Found hard link: %s (inode: %lu)\n", path, (unsigned long)inode_to_find);
        }

        if (S_ISDIR(statbuf.st_mode))
        {
            find_hard_links(path, inode_to_find);
        }
    }

    closedir(dir);
}
// Function to find all hard links of a file
void find_all_hlinks(const char *path, const char *source)
{
    struct stat source_stat;

    // Get the inode number of the source file
    if (stat(source, &source_stat) == -1)
    {
        perror("Failed to get source file stats");
        return;
    }

    // Now search for hard links with the same inode number
    find_hard_links(path, source_stat.st_ino);
}

// Function to remove all but one hard link
void unlink_all(const char *path, const char *source)
{
    struct stat source_stat;
    ino_t inode;
    DIR *dir;
    struct dirent *entry;
    char file_path[PATH_MAX];

    // Get inode number of the source file
    if (stat(source, &source_stat) == -1)
    {
        perror("Failed to get source file stats");
        return;
    }
    inode = source_stat.st_ino;

    // Open the directory
    if (!(dir = opendir(path)))
    {
        perror("Unable to open directory");
        return;
    }

    int link_count = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        snprintf(file_path, PATH_MAX, "%s/%s", path, entry->d_name);

        if (stat(file_path, &source_stat) == -1)
        {
            perror("Failed to get file stats");
            continue;
        }

        // If it's a hard link to the source file, unlink it
        if (source_stat.st_ino == inode && strcmp(file_path, source) != 0)
        {
            unlink(file_path);
            link_count++;
        }
    }

    closedir(dir);

    if (link_count > 0)
    {
        printf("Removed %d hard links to %s\n", link_count, source);
    }
    else
    {
        printf("No additional hard links found for %s\n", source);
    }
}

// Function to create a symbolic link
void create_sym_link(const char *source, const char *link_path)
{
    if (symlink(source, link_path) == -1)
    {
        perror("Failed to create symbolic link");
    }
    else
    {
        printf("Symbolic link created: %s -> %s\n", link_path, source);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        return 1;
    }

    // Create a file and hard links
    create_file_and_links(argv[1]);

    // Find all hard links to myfile1.txt
    char source_file[PATH_MAX];
    snprintf(source_file, PATH_MAX, "%s/myfile1.txt", argv[1]);
    find_all_hlinks(argv[1], source_file);

    // Remove duplicate hard links of myfile11.txt
    char hard_link[PATH_MAX];
    snprintf(hard_link, PATH_MAX, "%s/myfile11.txt", argv[1]);
    unlink_all(argv[1], hard_link);

    // Create a symbolic link
    char sym_link[PATH_MAX];
    snprintf(sym_link, PATH_MAX, "%s/myfile13.txt", argv[1]);
    create_sym_link("/tmp/myfile1.txt", sym_link);

    return 0;
}
