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

void sigint_handler(int num)
{
    printf("\nVocê deseja mesmo sair [s/n]? ");
    char c;
    scanf("%c", &c);
    if (c == 's')
    {
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    struct sigaction s;
    s.sa_handler = sigint_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = 0;

    sigaction(SIGINT, &s, NULL);

    int size = sizeof(all_tests) / sizeof(test_data);
    int status, pass_count = 0;
    double start, stop;

    //Executa um único teste passado como argumento
    if (argv[1] != NULL)
    {
        for (int i = 0; i < size; i++)
        {
            if (!strcmp(all_tests[i].name, argv[1]))
            {
                printf("Running %s:\n", argv[1]);
                printf("=====================\n\n");
                alarm(2);
                clock_t begin = clock();
                if (all_tests[i].function() == 0)
                {
                    clock_t end = clock();
                    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                    printf("%s: [PASS] (%lfs)\n", all_tests[i].name, time_spent);
                    return 0; //success
                }
                clock_t end = clock();
                double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                printf("(%lfs)\n", time_spent);
                return 1; //fail
            }
        }
    }

    char buffer[10];
    pid_t v[size];
    int fd[size];
    int saved_stdout = dup(1);

    printf("Running %d tests:\n", size);
    printf("=====================\n\n");

    for (int i = 0; i < size; i++)
    {
        fd[i] = open("/tmp", O_RDWR | O_TMPFILE);

        pid_t filho = fork();

        if (filho == 0)
        {
            alarm(2);
            dup2(fd[i], 1);
            clock_t begin = clock();
            if (all_tests[i].function() == 0)
            {
                clock_t end = clock();
                double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
                printf("%s: [PASS] (%lfs)\n", all_tests[i].name, time_spent);
                return 0; //success
            }
            clock_t end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            printf("(%lfs)\n", time_spent);
            return 1; //fail
        }
        v[i] = filho;
    }

    //espera todos os filhos terminarem e verifica o status de cada um
    int i = 0;
    while (wait(&status) > 0)
    {
        dup2(fd[i], 1);
        if (WTERMSIG(status) == 14)
        {
            printf("%s: [TIME] %s\n", all_tests[i].name, strsignal(WTERMSIG(status)));
            //duvida -> retorna 0...
        }
        else if (WIFSIGNALED(status))
            printf("%s: [ERRO] %s\n", all_tests[i].name, strsignal(WTERMSIG(status)));
        else
        {
            pass_count++;
        }
        i++;
    }

    //lê todos .temp e printa as saidas de forma organizada
    dup2(saved_stdout, 1);
    for (int i = 0; i < size; i++)
    {
        char buf[1];
        lseek(fd[i], 0, SEEK_SET);
        while ((read(fd[i], buf, 1)) > 0)
        {
            printf("%c", buf[0]);
        }
    }

    printf("\n=====================\n");
    printf("%d/%d tests passed\n", pass_count, size);
    return 0;
}
