#include "find.h"

struct Options parseOptions(int argc, char** argv) {
    struct Options options = {0, 0, 0, 0, NULL};

    for(int i = 0; i < argc; i++) {
        if(argv[i][0] == '-') {
            for(int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j])
                {
                case 'd':
                    options.isDirectory = 1;
                    break;
                case 'f':
                    options.isFile = 1;
                    break;
                case 'l':
                    options.isLink = 1;
                    break;
                case 's':
                    options.sort = 1;
                    break;
                default:
                    printf("Option only: -l, -d, -f, -s. Your option %c", argv[i][j]);
                    exit(-1);
                }
            }
        } else if (!options.dir) {
            options.dir = argv[i];
        } else(printf("Something wrong"));
    }
    
    if(options.dir == 0) {
        options.dir = "./";
    }

    if((options.isDirectory || options.isFile || options.isLink) == 0) {
        options.isDirectory = 1; options.isFile = 1; options.isLink = 1;
    }

    return options;
}

void dirWalk(char *path, struct Options *options) {
    struct dirent** dinfo;

    int quantity = scandir(path, &dinfo, NULL, options->sort ? alphasort : NULL);

    if(quantity < 0) {
		printf("Cannot open directory %s\n", path);
		return;
	}

    for(int i = 0; i < quantity; i++) {
        struct stat fileinfo;

        char* newPath = calloc(strlen(path) + strlen(dinfo[i]->d_name) + 2, sizeof(char));
        strcpy(newPath, path);
        strcat(newPath, "/");
        strcat(newPath, dinfo[i]->d_name);

        lstat(newPath, &fileinfo);

        if(!strcmp(".", dinfo[i]->d_name) || !strcmp("..", dinfo[i]->d_name)) {
            free(newPath);
            free(dinfo[i]);
            continue;
        }
        if(S_ISDIR(fileinfo.st_mode)) {
            if(options->isDirectory)
                printf("%s\n", newPath);
            dirWalk(newPath, options);
        }
        else if(S_ISLNK(fileinfo.st_mode) && options->isLink) {
            printf("%s\n", newPath);
        }
        else if(S_ISREG(fileinfo.st_mode) && options->isFile) {
            printf("%s\n", newPath);
        }

        free(dinfo[i]);
        free(newPath);
    }
    free(dinfo);
}

void find_m(int argc, char** argv) {
    struct Options options = parseOptions(argc, argv);
    dirWalk(options.dir, &options);
}