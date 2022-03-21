#include <stdio.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    char text_top[100] = "Small string";
    char buf[16];
    for(int i = 0; i < 16; i++) {
        printf("%d", buf[i]);
    }
    return 0;
}
