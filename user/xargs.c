#include "kernel/types.h"
#include "kernel/param.h"
#include "user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "xargs: use xargs <command>\n");
        exit(1);
    }

    char *args[argc];
    for (int i = 1; i < argc; i++) {
        args[i-1] = argv[i];
    }
    
    char c;
    char buf[512], *p;
    p = buf;
    while ((read(0, &c, 1) == 1)) {
        if (c == '\n') {
            *p = 0;
            int pid = fork();
            if (pid < 0) {
                fprintf(2, "xargs: fork failed \n");
                exit(1);
            }

            if (pid == 0) {
                args[argc-1] = buf;
                args[argc] = 0;
                exec(argv[1], args);
            } else {
                wait(0);
                p = buf;
            }
        } else {
            *p++ = c;
        }
    }
    exit(0);
}