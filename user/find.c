#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(const char *path, const char *filename);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "find: use find <path> <filename> \n");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}

void find(const char *path, const char *filename) {
    char subdir[512], *p;
    
    int fd;
    struct dirent de; // directory entry: inum: inode number, name: filename
    struct stat st;
    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if(st.type != T_DIR) {
        fprintf(2, "find: %s is not a directory \n", path);
        close(fd);
        return;
    }

    strcpy(subdir, path);
    p = subdir + strlen(subdir);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;
        
        memcpy(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        struct stat entryst;
        if (stat(subdir, &entryst) < 0) {
            fprintf(2, "find: cannot stat %s\n", path);
            close(fd);
            return;
        }

        switch(entryst.type) {
        case T_DEVICE:
            break;
        case T_FILE:
            // compare the filename
            if (strcmp(filename, de.name) == 0) {
                printf("%s\n", subdir);
            }
            break;
        case T_DIR:
            // search in the subdirectory (recursively)
            if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                find(subdir, filename);
            }
            break;
        }
        
    }
    close(fd);
    return;
}