#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

extern char **environ;

int main(int argc, char *argv[])
{
    char * pipfile;
    int pipfile_len;
    {
        char * dir;
        {
            /* argv[1] is the path to the script file */
            char * s = realpath(argv[1], NULL);
            if (s == NULL) {
                perror("realpath");
                exit(EXIT_FAILURE);
            }
            dir = dirname(s);
            free(s);
        }

        int max_depth;
        {
            char * s = getenv("PIPENV_MAX_DEPTH");
            if (s != NULL) {
                max_depth = atoi(s);
            } else {
                max_depth = 3;
            }
        }

        while (true) {
            size_t dir_len = strlen(dir);
            char * buffer = malloc(dir_len + 9);
            memcpy(buffer, dir, dir_len);
            memcpy(buffer + dir_len, "/Pipfile", 9);
            if (access(buffer, F_OK) != -1) {
                pipfile = buffer;
                pipfile_len = dir_len + 8;
                break;
            }
            free(buffer);
            if (max_depth == 0) {
                // couldn't find Pipfile
                pipfile = NULL;
                pipfile_len = 0;
                break;
            }
            --max_depth;
            dir = dirname(dir);
        }
    }


    /* construct new_argv */

    char **new_argv;
    {
        new_argv = malloc((argc + 4) * sizeof(char *));

        char ** p = new_argv;
        *p++ = "/usr/bin/env";
        if (pipfile != NULL) {
            *p++ = "pipenv";
            *p++ = "run";
        }
        *p++ = "python";

        int i;
        for(i=1; i!=argc; ++i) {
            *p++ = argv[i];
        }
        *p++ = NULL;
    }


    /* construct new_envp */

    char **new_envp;
    {
        /* get length of environ */

        int len;
        {
            char **p;
            for (p=environ, len=0; *p!=NULL; ++p, ++len) {}
        }

        new_envp = malloc((len+2) * sizeof(char*));
        memcpy(new_envp, environ, (len+1) * sizeof(char*));

        char **p = new_envp + len;

        if (pipfile != NULL) {
            char *e = malloc(16+pipfile_len);
            memcpy(e, "PIPENV_PIPFILE=", 15);
            memcpy(e+15, pipfile, pipfile_len+1);
            *p++ = e;
        }

        *p++ = NULL;
    }

    execve(new_argv[0], new_argv, new_envp);

    /* execvpe() only returns on error */

    perror("execve"); 
    exit(EXIT_FAILURE);
}
