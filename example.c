#include "mintest/macros.h"
#include <unistd.h>
#include <time.h>

int test1()
{
    test_printf("Hello! %d %f\n", 3, 3.14);
    return 0;
}

int test2()
{
    test_assert(1 == 0, "This always fails!");
    printf("This never runs!\n");
    test_assert(1 == 1, "Neither this.");
    return 0;
}

int test3()
{
    test_printf("<-- Name of the function before the printf!\n");
    test_assert(1 == 1, "This always succeeds");
    return 0;
}

int test_div0()
{
    test_printf("Teste divisão por zero\n");
    int i = 1 / 0;
    return 0;
}

int test_loop()
{
    test_printf("Teste loop\n");
    while (1)
        ;
    return 0;
}

int test_longo()
{
    test_printf("Teste bem longo que termina\n");
    for (int i = 0; i < 260; i++)
    {
        for (int j = 0; j < 260; j++)
        {
            for (int k = 0; k < i * j; k++)
            {
            }
        }
    }
    return 0;
}

int test_segmentation()
{
    test_printf("Teste segmentation fault\n");
    *(int *)0 = 0;
    return 0;
}

int test_rapido()
{
    test_printf("Teste for i < 5 (sem sleep)\n");
    for (int i = 0; i < 5; i++)
    {
    }
    return 0;
}

int test_lento()
{
    test_printf("Teste for i < 5 (com sleep)\n");
    for (int i = 0; i < 5; i++)
    {
        sleep(1);
    }
    return 0;
}

int test_prints()
{
    for (int i = 0; i < 50; i++)
    {
        test_printf("varios prints\n");
    }
    return 0;
}

int test_rand()
{
    test_printf("Teste aleatório\n");
    srand(time(NULL));
    test_assert(rand() % 10 < 6, "Valor maior que 5");
    return 0;
}

test_list = {TEST(test_prints), TEST(test1), TEST(test2), TEST(test3), TEST(test_div0),
             TEST(test_loop), TEST(test_longo), TEST(test_segmentation),
             TEST(test_rapido), TEST(test_lento), TEST(test_rand)};

#include "mintest/runner.h"
