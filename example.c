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
    int i = 1 / 0;
    return 0;
}

int test_loop()
{
    while (1)
        ;
    return 0;
}

int test_longo()
{
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
    *(int *)0 = 0;
    return 0;
}

int test_rapido()
{
    for (int i = 0; i < 5; i++)
    {
    }
}

int test_lento()
{
    for (int i = 0; i < 5; i++)
    {
        sleep(1);
    }
}

int test_prints()
{
    for (int i = 0; i < 40; i++)
    {
        test_printf("varios prints\n");
    }
    return 0;
}

int test_rand()
{
    srand(time(NULL));
    test_assert(rand() % 10 < 6, "Valor maior que 5");
    return 0;
}

test_list = {TEST(test1), TEST(test2), TEST(test3), TEST(test_div0),
             TEST(test_loop), TEST(test_longo), TEST(test_segmentation),
             TEST(test_rapido), TEST(test_lento), TEST(test_prints),
             TEST(test_rand)};

#include "mintest/runner.h"
