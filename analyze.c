//To Do structs? then bigram/trigram analysis then generator
//gcc analyze.c -o analyze -g -fsanitize=address -std=c99
#include <stdio.h>
#include <stdlib.h>

long double percentage, percentage_mono, percentage_bi, percentage_tri;
char a, b, c, d;
int e, count, count_mono, count_bi, count_tri;
static long double monogram[128];
static long double bigram  [128][128];
static long double trigram [128][128][128];
static char matrix[3][10];
long double SFB, SFS, LSB, LSS, BetterFSB, BetterFSS, SRB, SRS;

void ReadNgrams(FILE * ngram)
{
    while ((e = fscanf(ngram, "%c%c%c%c%La\n", &a, &b, &c, &d, &percentage)) != EOF)
    {
        count++;
        if (d == ' ')
        {
            monogram[a] = percentage;
            count_mono++;
            percentage_mono += percentage;
        }
        else if (d == '-')
        {
            bigram[a][b] = percentage;
            count_bi++;
            percentage_bi += percentage;
        }
        else if (d == ':')
        {
            trigram[a][b][c] = percentage;
            count_tri++;
            percentage_tri += percentage;
        }
    }
}

void ReadLayout(FILE * layout)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            fscanf(layout, " %c ", &matrix[i][j]);
        }
    }
}
long double row(int row)
{
    long double total = 0;
    for (int i = 0; i < 10; i++)
    {
        total += monogram[matrix[row][i]];
    }
    return total;
}
long double finger(int column)
{
    long double total = 0;
    for (int i = 0; i < 3; i++)
    {
        total += monogram[matrix[i][column]];
    }
    return total;
}
void MonogramAnalyze()
{
    printf("Left Pinky   : %LF%%\n", finger(0));
    printf("Left Ring    : %LF%%\n", finger(1));
    printf("Left Middle  : %LF%%\n", finger(2));
    printf("Left Index   : %LF%%\n", finger(3) + finger(4));
    printf("Right Index  : %LF%%\n", finger(5) + finger(6));
    printf("Right Middle : %LF%%\n", finger(7));
    printf("Right Ring   : %LF%%\n", finger(8));
    printf("Right Pinky  : %LF%%\n", finger(9));
    printf("Middle Thumb : %LF%%\n", monogram[' ']);
    puts("");
    printf("Top Row      : %LF%%\n", row(0));
    printf("Home Row     : %LF%%\n", row(1));
    printf("Bottom Row   : %LF%%\n", row(2));
    printf("Space Row    : %LF%%\n", monogram[' ']);
    puts("");
}
int checkSameColumn(int column1, int column2)
{
    if (column1 == column2)
    {
        return 1;
    }
    if ((column1 == 3 && column2 == 4) || (column1 == 4 && column2 == 3))
    {
        return 1;
    }
    if ((column1 == 5 && column2 == 6) || (column1 == 6 && column2 == 5))
    {
        return 1;
    }
    return 0;
}
int checkSameHand(int column1, int column2)
{
    if (column1 <= 4 && column2 >= 5)
    {
        return 0;
    }
    if (column1 >= 5 && column2 <= 4)
    {
        return 0;
    }
    return 1;
}
void BiAnalyzeHelper(int row1, int column1, int row2, int column2)
{
    int sameRow, sameColumn, sameHand;
    sameRow = row1 == row2;
    sameColumn = checkSameColumn(column1, column2);
    sameHand = checkSameHand(column1, column2);
    long double percentage = bigram[matrix[row1][column1]][matrix[row2][column2]];

    if (sameColumn)
    {
        SFB += percentage;
    }
    if (sameRow && !sameColumn && sameHand)
    {
        SRB += percentage;
    }
    if ((column1 == 2 && column2 == 4) || (column1 == 4 && column2 == 2)
        || (column1 == 5 && column2 == 7) || (column1 == 7 && column2 == 5))
    {
        LSB += percentage;
    }
    if (sameHand && (row1 - row2 == 2 || row1 - row2 == -2) && (column1 != column2)
        && (column1 == 1 || column1 == 2 || column1 == 7 || column1 == 8
        || column2 == 1 || column2 == 2 || column2 == 7 || column2 == 8))
    {
        BetterFSB += percentage;
    }
}
void BigramAnalyze()
{
    SFB = LSB =  BetterFSB =  SRB =  0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 10; l++)
                {
                    if (!(i == k && j == l))
                    {
                        BiAnalyzeHelper(i, j, k, l);
                    }
                }
            }
        }
    }
    printf("SFB          : %LF%%\n", SFB);
    printf("LSB          : %LF%%\n", LSB);
    printf("BetterFSB    : %LF%%\n", BetterFSB);
    printf("SRB          : %LF%%\n", SRB);
    puts("");
}
void TriAnalyzeHelper(int row1, int column1, int row2, int column2)
{
    int sameRow, sameColumn, sameHand;
    sameRow = row1 == row2;
    sameColumn = checkSameColumn(column1, column2);
    sameHand = checkSameHand(column1, column2);
    long double percentage = 0;
    for (int i = 0; i < 128; i++)
    {
        percentage += trigram[matrix[row1][column1]][i][matrix[row2][column2]];
    }

    if (sameColumn)
    {
        SFS += percentage;
    }
    if (sameRow && !sameColumn && sameHand)
    {
        SRS += percentage;
    }
    if ((column1 == 2 && column2 == 4) || (column1 == 4 && column2 == 2)
        || (column1 == 5 && column2 == 7) || (column1 == 7 && column2 == 5))
    {
        LSS += percentage;
    }
    if (sameHand && (row1 - row2 == 2 || row1 - row2 == -2) && (column1 != column2)
        && (column1 == 1 || column1 == 2 || column1 == 7 || column1 == 8
        || column2 == 1 || column2 == 2 || column2 == 7 || column2 == 8))
    {
        BetterFSS += percentage;
    }
}
void TrigramAnalyze()
{
    SFS = LSS =  BetterFSS =  SRS =  0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 10; l++)
                {
                    if (!(i == k && j == l))
                    {
                        TriAnalyzeHelper(i, j, k, l);
                    }
                }
            }
        }
    }
    printf("SFS          : %LF%%\n", SFS);
    printf("LSS          : %LF%%\n", LSS);
    printf("BetterFSS    : %LF%%\n", BetterFSS);
    printf("SRS          : %LF%%\n", SRS);
    puts("");
}

void Analyze(FILE * layout)
{
    ReadLayout(layout);
    MonogramAnalyze();
    BigramAnalyze();
    TrigramAnalyze();
}

int main (int argc, char **argv)
{
    FILE * ngram;
    ngram = fopen("ngram", "r");
    ReadNgrams(ngram);
    fclose(ngram);

    printf("%d\n%d %d %d\n%LF %LF %LF\n", count, count_mono, count_bi, count_tri,
        percentage_mono, percentage_bi, percentage_tri);

    FILE * layout;
    for (int i = 1; i < argc; i++)
    {
        layout = fopen(argv[i], "r");
        printf("%s\n", argv[i]);
        Analyze(layout);
        fclose(layout);
    }
    return EXIT_SUCCESS;
}
