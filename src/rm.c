#include "rm.h"

void remove_file(const char *path) {
    if(remove(path) != 0) {
        perror("Error removing file");
    }
}

void remove_directory(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    } else {printf("jnd");}

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char child_path[256];
        //snprintf(child_path, sizeof(child_path), "%s/%s", path, entry->d_name);

        struct stat st;
        lstat(child_path, &st);

        if (S_ISDIR(st.st_mode)) {
            remove_directory(child_path);
        } else {
            remove_file(child_path);
        }
    }

    closedir(dir);
}

void remutil(int argc, char **argv) {
    if (argc < 1) {
        fprintf(stderr, "Usage: rm [-r] <file/directory>\n");
        return;
    }

    int remove_directories = 0;

    if (argc == 2 && strcmp(argv[0], "-r") == 0) {
        remove_directories = 1;
    }

    if (remove_directories) {
        remove_directory(argv[1]);
    } else {
        remove_file(argv[0]);
    }
}