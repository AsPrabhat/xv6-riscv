#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int parent_to_child[2], child_to_parent[2];
    char buf;

    // Create pipes
    if (pipe(parent_to_child) < 0 || pipe(child_to_parent) < 0)
    {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    // Fork
    int pid = fork();
    if (pid < 0)
    {
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if (pid == 0)
    {                              // Child process
        close(parent_to_child[1]); // Close write end of parent_to_child pipe
        close(child_to_parent[0]); // Close read end of child_to_parent pipe

        // Read from parent
        read(parent_to_child[0], &buf, 1);
        printf("%d: received ping\n", getpid());

        // Write to parent
        write(child_to_parent[1], &buf, 1);
        close(parent_to_child[0]);
        close(child_to_parent[1]);
        exit(0);
    }
    else
    {                              // Parent process
        close(parent_to_child[0]); // Close read end of parent_to_child pipe
        close(child_to_parent[1]); // Close write end of child_to_parent pipe

        // Write to child
        write(parent_to_child[1], "x", 1);

        // Read from child
        read(child_to_parent[0], &buf, 1);
        printf("%d: received pong\n", getpid());

        close(parent_to_child[1]);
        close(child_to_parent[0]);
        exit(0);
    }
}