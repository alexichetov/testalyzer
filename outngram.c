//gcc outngram.c -o outngram -g -fsanitize=address -std=c99
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
    static int monogram[128];
    static int bigram  [128][128];
    static int trigram [128][128][128];

    FILE * corpus;
    FILE * ngram;
    corpus = fopen("corpus", "r");
    ngram  = fopen("ngram" , "w");

    int total;
    char c,d,e;

    while((c = fgetc(corpus)) > 128 || c < 0){}
    if (c >= 'A' && c <= 'Z') {c += 32;}
    monogram[c]++;
    total++;

    while((d = fgetc(corpus)) > 128 || d < 0){}
    if (d >= 'A' && d <= 'Z') {d += 32;}
    monogram[d]++;
    bigram[c][d]++;
    total++;

    while ((e = fgetc(corpus)) != EOF)
    {
        if (e <= 128 && e >= 0)
        {
            if (e >= 'A' && e <= 'Z') {e += 32;}
            if (e == '\n') {e = ' ';}
            monogram[e]++;
            bigram[d][e]++;
            trigram[c][d][e]++;
            total++;
            c = d;
            d = e;
        }
    }

    long double perc        = 0;
    long double mono_total  = 0;
    long double mono_actual = 0;
    long double mono_kept   = 0;
    long double bi_total    = 0;
    long double bi_actual   = 0;
    long double bi_kept     = 0;
    long double tri_total   = 0;
    long double tri_actual  = 0;
    long double tri_kept    = 0;

    for (int i = 0; i < 128; i++) {
        if (monogram[i] != 0) {
            perc = monogram[i] * 100;
            perc /= total;
            mono_total += perc;
            mono_actual += perc;
            if (perc > 0)
            {
                mono_kept += perc;
                fprintf(ngram, "%c%c%c%c%La\n", i, ' ', ' ', ' ', perc);
            }
        }
    }

    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 128; j++) {
            if (bigram[i][j] != 0) {
                perc = bigram[i][j] * 100;
                perc /= total - 1;
                bi_total += perc;
                if (i != ' ' && j != ' ')
                {
                    bi_actual += perc;
                    if (perc > 0)
                    {
                        bi_kept += perc;
                        fprintf(ngram, "%c%c%c%c%La\n", i, j, ' ', '-', perc);
                    }
                }
            }
        }
    }

    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 128; j++) {
            for (int k = 0; k < 128; k++) {
                if (trigram[i][j][k] != 0) {
                    perc = trigram[i][j][k] * 100;
                    perc /= total - 2;
                    tri_total += perc;
                    if (i != ' ' && k != ' ')
                    {
                        tri_actual += perc;
                        if (perc > 0)
                        {
                            tri_kept += perc;
                            fprintf(ngram, "%c%c%c%c%La\n", i, j, k, ':', perc);
                        }
                    }
                }
            }
        }
    }
    printf("%LF/%LF/%LF %LF/%LF/%LF %LF/%LF/%LF\n", mono_kept, mono_actual, mono_total,
        bi_kept, bi_actual, bi_total, tri_kept, tri_actual, tri_total);

    fclose(corpus);
    fclose(ngram);
    return EXIT_SUCCESS;
}
