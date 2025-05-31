#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *f1 = fopen(argv[1], "rb");
    FILE *f2 = fopen(argv[2], "rb");

    char c1 = fgetc(f1);
    char c2 = fgetc(f2);


    while (c1 != EOF && c2 != EOF) {
        if (c1 != c2) {
            printf("diff -> %d != %d\n", c1, c2);
            printf("ftell -> %ld \n", ftell(f1));
        }

        c1 = fgetc(f1);
        c2 = fgetc(f2);
    }

    return 0;
}