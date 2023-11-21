#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_BLOCK 1024
#define TOTAL_BLOCKS 128
#define SIZE_SUPER_BLOCK 1024
#define MAX_FILES 16

typedef struct
{
    char file_name[16];
    int file_size;
    int pointer_blocks[8];
    int is_active;
} file_inode;

int create_file(char file_name[16], int file_size)
{
    FILE *file_system = fopen("disk0", "r+");
    if (!file_system)
    {
        printf("Failed to open disk file.\n");
        return -1;
    }

    char blockStatusList[TOTAL_BLOCKS];
    fseek(file_system, 0, SEEK_SET);
    fread(blockStatusList, sizeof(char), TOTAL_BLOCKS, file_system);

    int availableBlocks = 0;
    for (int i = 0; i < TOTAL_BLOCKS; i++)
    {
        if (blockStatusList[i] == 0)
        {
            availableBlocks++;
        }
    }

    if (availableBlocks < file_size)
    {
        printf("Disk space is not sufficient.\n");
        fclose(file_system);
        return -1;
    }

    file_inode inode;
    int inodeIndex = -1;

    fseek(file_system, SIZE_SUPER_BLOCK, SEEK_SET);
    for (int i = 0; i < MAX_FILES; i++)
    {
        fread(&inode, sizeof(file_inode), 1, file_system);
        if (inode.is_active == 0)
        {
            inodeIndex = i;
            break;
        }
    }

    if (inodeIndex == -1)
    {
        printf("No empty inode available.\n");
        fclose(file_system);
        return -1;
    }

    fseek(file_system, SIZE_SUPER_BLOCK + inodeIndex * sizeof(file_inode), SEEK_SET);
    inode.is_active = 1;
    strncpy(inode.file_name, file_name, 15);
    inode.file_size = file_size;

    int allocatedBlocks = 0;
    for (int i = 0; allocatedBlocks < file_size && i < TOTAL_BLOCKS; i++)
    {
        if (blockStatusList[i] == 0)
        {
            blockStatusList[i] = 1;
            inode.pointer_blocks[allocatedBlocks++] = i;
        }
    }

    fseek(file_system, 0, SEEK_SET);
    fwrite(blockStatusList, sizeof(char), TOTAL_BLOCKS, file_system);

    fseek(file_system, SIZE_SUPER_BLOCK + inodeIndex * sizeof(file_inode), SEEK_SET);
    fwrite(&inode, sizeof(file_inode), 1, file_system);

    fclose(file_system);
    return 0;
}
int delete_file(char file_name[16])
{
    FILE *file_system = fopen("disk0", "r+");
    if (!file_system)
    {
        printf("Failed to open disk file.\n");
        return -1;
    }

    char blockAvailabilityList[TOTAL_BLOCKS];
    fseek(file_system, 0, SEEK_SET);
    fread(blockAvailabilityList, sizeof(char), TOTAL_BLOCKS, file_system);

    file_inode inode;

    fseek(file_system, SIZE_SUPER_BLOCK, SEEK_SET);
    for (int i = 0; i < MAX_FILES; i++)
    {
        fread(&inode, sizeof(file_inode), 1, file_system);
        if (inode.is_active == 1 && strcmp(inode.file_name, file_name) == 0)
        {
            inode.is_active = 0;

            for (int j = 0; j < inode.file_size; j++)
            {
                blockAvailabilityList[inode.pointer_blocks[j]] = 0;
            }

            fseek(file_system, SIZE_SUPER_BLOCK + i * sizeof(file_inode), SEEK_SET);
            fwrite(&inode, sizeof(file_inode), 1, file_system);

            fseek(file_system, SIZE_SUPER_BLOCK, SEEK_SET);
            fwrite(blockAvailabilityList, sizeof(char), TOTAL_BLOCKS, file_system);

            fclose(file_system);
            return 0;
        }
    }

    printf("File not found or already removed.\n");
    fclose(file_system);
    return -1;
}

int list_files(void)
{
    FILE *file_system = fopen("disk0", "r");
    if (!file_system)
    {
        printf("Failed to open disk file.\n");
        return -1;
    }

    file_inode inode;

    fseek(file_system, SIZE_SUPER_BLOCK, SEEK_SET);
    for (int i = 0; i < MAX_FILES; i++)
    {
        fread(&inode, sizeof(file_inode), 1, file_system);
        if (inode.is_active == 1)
        {
            printf("Filename: %s, Size: %d blocks\n", inode.file_name, inode.file_size);
        }
    }

    fclose(file_system);
    return 0;
}

int read_from_file(char file_name[16], int block_number, char buffer[SIZE_BLOCK])
{
    FILE *file_system = fopen("disk0", "r");
    if (!file_system)
    {
        printf("Failed to open disk file.\n");
        return -1;
    }

    file_inode inode;
    int file_found = 0;

    fseek(file_system, SIZE_SUPER_BLOCK, SEEK_SET);
    while (fread(&inode, sizeof(file_inode), 1, file_system) == 1)
    {
        if (inode.is_active == 1 && strcmp(inode.file_name, file_name) == 0)
        {
            file_found = 1;
            if (block_number >= inode.file_size)
            {
                printf("Block number is out of file size range.\n");
                fclose(file_system);
                return -1;
            }
            int block_pos = SIZE_SUPER_BLOCK + MAX_FILES * sizeof(file_inode) + (inode.pointer_blocks[block_number] * SIZE_BLOCK);
            fseek(file_system, block_pos, SEEK_SET);
            fread(buffer, sizeof(char), SIZE_BLOCK, file_system);
            break;
        }
    }

    if (!file_found)
    {
        printf("File not located.\n");
        fclose(file_system);
        return -1;
    }

    fclose(file_system);
    return 0;
}

int write_to_file(char file_name[16], int block_number, char buffer[SIZE_BLOCK])
{
    FILE *file_system = fopen("disk0", "r+");
    if (!file_system)
    {
        printf("Failed to open disk file.\n");
        return -1;
    }

    file_inode inode;
    int file_found = 0;

    fseek(file_system, SIZE_SUPER_BLOCK, SEEK_SET);
    while (fread(&inode, sizeof(file_inode), 1, file_system) == 1)
    {
        if (inode.is_active == 1 && strcmp(inode.file_name, file_name) == 0)
        {
            file_found = 1;
            if (block_number >= inode.file_size)
            {
                printf("Block number is out of file size range.\n");
                fclose(file_system);
                return -1;
            }
            int block_pos = SIZE_SUPER_BLOCK + MAX_FILES * sizeof(file_inode) + (inode.pointer_blocks[block_number] * SIZE_BLOCK);
            fseek(file_system, block_pos, SEEK_SET);
            fwrite(buffer, sizeof(char), SIZE_BLOCK, file_system);
            break;
        }
    }

    if (!file_found)
    {
        printf("File not located.\n");
        fclose(file_system);
        return -1;
    }

    fclose(file_system);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s [command file]\n", argv[0]);
        return -1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile)
    {
        printf("Failed to open command file.\n");
        return -1;
    }

    char command, file_name[16];
    int file_size, block_num;
    char buffer[SIZE_BLOCK];

    while (fscanf(inputFile, " %c", &command) != EOF)
    {
        switch (command)
        {
        case 'C':
            fscanf(inputFile, "%s %d", file_name, &file_size);
            create_file(file_name, file_size);
            break;
        case 'D':
            fscanf(inputFile, "%s", file_name);
            delete_file(file_name);
            break;
        case 'L':
            list_files();
            break;
        case 'R':
            fscanf(inputFile, "%s %d", file_name, &block_num);
            read_from_file(file_name, block_num, buffer);
            break;
        case 'W':
            fscanf(inputFile, "%s %d", file_name, &block_num);
            strcpy(buffer, "Sample data for write");
            write_to_file(file_name, block_num, buffer);
            break;
        }
    }

    fclose(inputFile);
    return 0;
}
