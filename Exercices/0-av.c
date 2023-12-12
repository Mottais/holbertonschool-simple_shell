#include <stdio.h>


int main(int ac, char **av)
{
    (void) ac;
    printf("arg[0] = nom de la fonction = %s \n", av[0]);
    av ++;
    while(av[0])
    {
        
        printf("%s  ", av[0]);
        av ++;
    }
    putchar(10);    
}
