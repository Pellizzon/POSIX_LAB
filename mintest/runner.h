#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int saved_stdout;
int interrupt = 0;

void sigint_handler(int num)
{
    dup2(saved_stdout, 1);
    printf("\nVocê deseja mesmo sair [s/n]? ");
    char c;
    scanf("%c", &c);
    printf("\n");
    if (c == 's')
    {
        interrupt = 1;
        // exit(0);
    }
}

int main(int argc, char *argv[])
{

    int size = sizeof(all_tests) / sizeof(test_data);
    int status, pass_count = 0;
    double start, stop;

    //Executa um único teste passado como argumento
    if (argc == 2)
    {
        for (int i = 0; i < size; i++)
        {
            if (!strcmp(all_tests[i].name, argv[1]))
            {
                printf("Running %s:\n", argv[1]);
                printf("=====================\n\n");
                clock_t begin = clock();
                if (all_tests[i].function() == 0)
                {
                    clock_t end = clock();
                    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                    printf("%s: \033[0;32m[PASS]\033[0m (%lfs)\n", all_tests[i].name, time_spent);
                    return 0;
                }
                clock_t end = clock();
                double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                printf(" (%lfs)\n", time_spent);
                return 1;
            }
        }
    }

    pid_t *filhos = malloc(sizeof(pid_t) * size);
    int *fd = malloc(sizeof(int) * size);
    saved_stdout = dup(1);

    printf("Running %d tests:\n", size);
    printf("=====================\n\n");

    for (int i = 0; i < size; i++)
    {
        fd[i] = open("/tmp", O_RDWR | O_TMPFILE);
        pid_t filho = fork();

        if (filho == 0)
        {
            dup2(fd[i], 1);
            clock_t begin = clock();
            if (all_tests[i].function() >= 0)
            {
                clock_t end = clock();
                double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                printf("%s: \033[0;32m[PASS]\033[0m (%lfs)\n", all_tests[i].name, time_spent);
                return 0;
            }
            clock_t end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf(" (%lfs)\n", time_spent);
            return 1;
        }
        filhos[i] = filho;
    }

    struct sigaction s;
    s.sa_handler = sigint_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = 0;

    sigaction(SIGINT, &s, NULL);

    sleep(2);
    int j = 0;
    while (interrupt && filhos[j])
    {
        kill(filhos[j], SIGINT);
        j++;
    }
    //espera todos os filhos terminarem e verifica o status de cada um
    int i = 0;
    while (waitpid(filhos[i], &status, 0) > 0)
    {
        dup2(fd[i], 1);
        if (WIFEXITED(status))
        {
            pass_count += !WEXITSTATUS(status);
            i++;
            continue;
        }

        if (WTERMSIG(status) == 14)
            printf("%s: \033[0;36m[TIME]\033[0m %s\n", all_tests[i].name, strsignal(WTERMSIG(status)));
        else if (WTERMSIG(status) == 2 && interrupt)
            printf("%s: \033[0;34m[STOP]\033[0m\n", all_tests[i].name);
        else if (WIFSIGNALED(status))
            printf("%s: \033[0;31m[ERRO]\033[0m %s\n", all_tests[i].name, strsignal(WTERMSIG(status)));

        i++;
    }

    //lê todos .temp e printa as saidas de forma organizada
    dup2(saved_stdout, 1);
    char buf[1];
    for (int i = 0; i < size; i++)
    {
        lseek(fd[i], 0, SEEK_SET);

        while ((read(fd[i], buf, 1)) > 0)
        {
            printf("%c", buf[0]);
        }
        close(fd[i]);
    }

    printf("\n=====================\n");
    printf("%d/%d tests passed\n", pass_count, size);
    return 0;
}