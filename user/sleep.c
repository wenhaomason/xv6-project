#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(2, "Please pass the arguement.\n");
        exit(1);
    }
    
    int n = atoi(argv[1]);
    sleep(n);
    exit(0);
}