#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// ANSI Colors
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

// Function to copy file using the cp command with execveat()
void copy_to_tmp(const char *source) {
    char dest[256];
    snprintf(dest, sizeof(dest), "/tmp/%s", strrchr(source, '/') + 1);

    // Prepare arguments for cp
    char *cp_args[] = { "cp", source, dest, NULL };

    // Execute the cp command using execveat()
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        // Open the path for cp using O_PATH
        int fd = open("/bin/cp", O_PATH);
        if (fd == -1) {
            perror("open");
            _exit(1);
        }

        // Execute cp command to copy the file
        execveat(fd, "/bin/cp", cp_args, NULL, AT_EMPTY_PATH);
        perror("execveat");
        _exit(1);
    } else {
        wait(NULL);
    }
}

// Function to execute a command from /tmp/ using execveat()
void execute_cmd(const char *cmd, char *args[], const char *color) {
    printf("%sExecuting: %s from /tmp/%s%s\n", color, args[0], strrchr(cmd, '/') + 1, RESET);
    sleep(2);

    char tmp_path[256];
    snprintf(tmp_path, sizeof(tmp_path), "/tmp/%s", strrchr(cmd, '/') + 1);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        int fd = open(tmp_path, O_PATH);
        if (fd == -1) {
            perror("open");
            _exit(1);
        }

        // Use execveat() to execute the file from /tmp/
        execveat(fd, "", args, NULL, AT_EMPTY_PATH);
        perror("execveat");
        _exit(1);
    } else {
        wait(NULL);
    }
}

int main() {
    printf(GREEN "Copying executables to /tmp/...\n" RESET);

    // Define commands
    char *cmds[] = { "/usr/bin/id", "/usr/bin/whoami", "/bin/uname", "/bin/cat" };
    char *id_args[] = { "id", NULL };
    char *whoami_args[] = { "whoami", NULL };
    char *uname_args[] = { "uname", "-a", NULL };
    char *cat_args[] = { "cat", "/etc/passwd", NULL };

    // Copy executables to /tmp/ using cp command
    for (int i = 0; i < 4; i++) {
        copy_to_tmp(cmds[i]);
    }

    // Execute commands from /tmp/ using execveat()
    execute_cmd("/usr/bin/id", id_args, CYAN);
    execute_cmd("/usr/bin/whoami", whoami_args, GREEN);
    execute_cmd("/bin/uname", uname_args, YELLOW);
    execute_cmd("/bin/cat", cat_args, CYAN);

    return 0;
}
