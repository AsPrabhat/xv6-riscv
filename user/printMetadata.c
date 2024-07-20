#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

#define BLOCK_SIZE 512

// Function to print the contents of a block
void print_block_contents(char *buffer)
{
    int index;
    for (index = 0; index < BLOCK_SIZE; index++)
    {
        if (buffer[index] == '\0')
            break;
        fprintf(2, "%c", buffer[index]);
    }
    fprintf(2, "\n");
}

int main(int argc, char *argv[])
{
    // Check if correct number of arguments provided
    if (argc != 3)
    {
        fprintf(2, "Please provide the correct number of arguments\n");
        fprintf(2, "Usage: createfile <fileName> <fileSize>\n");
        exit(0);
    }

    // Extract fileName and fileSize from arguments. Also set the roll_no
    char *file_name = argv[1];
    int file_size = atoi(argv[2]);
    char rollNo[14] = {'C', 'O', '2','2', 'B', 'T', 'E', 'C', 'H', '1', '1', '0', '0', '9'};

    // Open the file
    int file_descriptor = open(file_name, O_CREATE | O_RDWR);
    if (file_descriptor < 0)
    {
        fprintf(2, "Error: Unable to open or create the file\n");
        exit(0);
    }

    // Allocate disk blocks and write roll_no in each block
    int i;
    char buffer[BLOCK_SIZE];
    for (i = 0; i < file_size; i++)
    {
        write(file_descriptor, rollNo + (i%14), 1);
    }

    // Close the file
    close(file_descriptor);

    // Re-open the file to get inode number and print block numbers and contents
    file_descriptor = open(file_name, O_RDWR);
    if (file_descriptor < 0)
    {
        fprintf(2, "Error: Unable to reopen the file while getting inode number and printing block numbers and contents\n");
        exit(0);
    }

    // Get inode number
    struct stat file_info;
    if (fstat(file_descriptor, &file_info) < 0)
    {
        fprintf(2, "Error: Unable to get file information\n");
        exit(0);
    }
    fprintf(2, "Inode number: %d\n", file_info.ino);

    // Print block numbers and contents
    for (i = 0; i < file_size; i += BLOCK_SIZE)
    {
        fprintf(2, "Block number: %d\n", i / BLOCK_SIZE);
        memset(buffer, '\0', BLOCK_SIZE);
        // Read block contents
        read(file_descriptor, buffer, BLOCK_SIZE);
        // Print block contents
        fprintf(2, "Block contents: ");
        print_block_contents(buffer);
    }

    // Close the file
    close(file_descriptor);

    exit(0);
}