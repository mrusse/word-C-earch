#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HTML_OUTPUT "ws.html"
#define WORD_INPUT "words.txt"

#define DIM_X 14
#define DIM_Y 14

#define NUM_WORDS 14
#define FILE_WORD_COUNT 3000
#define MAX_WORD_LENGTH 16 // Longest word in file is AFRICANAMERICAN

//#define DEBUG

void wordPick(FILE *fp, char board[DIM_X][DIM_Y],
              char words[FILE_WORD_COUNT][MAX_WORD_LENGTH]);
int  validPick(char board[DIM_X][DIM_Y], char wordBuff[BUFSIZ], int x, int y,
               int length, int type, int direction);
void loadWords(FILE *wordFile, char words[FILE_WORD_COUNT][MAX_WORD_LENGTH]);

int overlap = 0;

int main(void)
{
    FILE *fp = fopen(HTML_OUTPUT, "a");
    if (!fp) {
        fprintf(stderr, "Could not open %s\n", HTML_OUTPUT);
        perror("fopen");
        exit(1);
    }

#ifndef DEBUG
    srand(time(NULL));
#else
    // better to debug with the same seed
    srand(0);
#endif

    fprintf(fp,
            "<html><head><style>table, th, td {border: 1px solid "
            "black;border-collapse: collapse;}th, td {padding: 5px;text-align: "
            "left;    }</style></head><body>");

    char board[DIM_X][DIM_Y];

    for (int i = 0; i < DIM_X; i++) {
        for (int j = 0; j < DIM_Y; j++) {
            board[i][j] = '*';
        }
    }

    fprintf(fp,
            "<table style=font-family:arial;width15%%><tr><th "
            "colspan=2>Words</th></tr><tr>");

    FILE *wordFile = fopen(WORD_INPUT, "r");
    if (!wordFile) {
        fprintf(stderr, "Could not open %s\n", WORD_INPUT);
        perror("fopen");
        exit(1);
    }

    char words[FILE_WORD_COUNT][MAX_WORD_LENGTH] = {0};

    loadWords(wordFile, words);

    wordPick(fp, board, words);

    fprintf(fp,
            "<p style=\"font-size: 24pt; font-family: Courier New, Courier, "
            "monospace\">");

    for (int i = 0; i < DIM_X; i++) {
        for (int j = 0; j < DIM_Y; j++) {
            if (board[j][i] == '*') {
                //comment this line for debug
                board[j][i] = 'A' + (rand() % 26); 
            }
            fprintf(fp, "%c ", board[j][i]);
        }
        fprintf(fp, "<br/>");
    }

    fprintf(fp, "</p>");
    fprintf(fp, "</table></body></html>");

    fclose(fp);
    fclose(wordFile);

    return 0;
}

void loadWords(FILE *wordFile, char words[FILE_WORD_COUNT][MAX_WORD_LENGTH])
{
    int wordsRead = 0;
    for (int i = 0; fgets(words[i], BUFSIZ, wordFile); i++) {
        wordsRead++;
    }
    if (wordsRead != FILE_WORD_COUNT) {
        fprintf(stderr, "Reading error. Expected %d words but got %d\n",
            FILE_WORD_COUNT, wordsRead);
        exit(1);
    }
}

void wordPick(FILE *fp, char board[DIM_X][DIM_Y],
              char words[FILE_WORD_COUNT][MAX_WORD_LENGTH])
{
    int  x, y, length, type, direction, lineNum, count;
    char wordBuff[BUFSIZ] = "";

    for (int i = 0; i < NUM_WORDS; i++) {
        do {
            lineNum = rand() % FILE_WORD_COUNT;
            strcpy(wordBuff, words[lineNum]);
        } while (strlen(wordBuff) < 7 || strlen(wordBuff) > 11);

        count = 0;

        do {
            length    = strlen(wordBuff) - 1;
            x         = (rand() % DIM_X);
            y         = (rand() % DIM_Y);
            type      = (rand() % 4);
            direction = (rand() % 2);

            count ++;
            //printf("%d\n",count);

            if (count > 100) {
                do {
                    lineNum = rand() % FILE_WORD_COUNT;
                    strcpy(wordBuff, words[lineNum]);
                } while (strlen(wordBuff) < 7 || strlen(wordBuff) > 11);
                
                count = 0;

            }        
        } while (!validPick(board, wordBuff, x, y, length, type, direction));

        //type 0 is horizontal, 1 is vertical, 2 is diagonal top left bottom right slant, 3 is other diagonal
        //direction 0 is left to right 1 is reverse (i think lol) 

        for (int j = 0; j < length; j++) {
            if (type == 0) {
                if (direction == 0) {
                    board[x + j][y] = wordBuff[j];
                }
                if (direction == 1) {
                    board[x - j][y] = wordBuff[j];
                }
            }

            if (type == 1) {
                if (direction == 0) {
                    board[x][y + j] = wordBuff[j];
                }
                if (direction == 1) {
                    board[x][y - j] = wordBuff[j];
                }
            }

            if (type == 2) {
                if (direction == 0) {
                    board[x + j][y + j] = wordBuff[j];
                }
                if (direction == 1) {
                    board[x - j][y - j] = wordBuff[j];
                }
            }

            if (type == 3) {
                if (direction == 0) {
                    board[x - j][y + j] = wordBuff[j];
                }
                if (direction == 1) {
                    board[x + j][y - j] = wordBuff[j];
                }
            }
        }

        if (i % 2 == 0 && i != 0) {
            fprintf(fp, "</tr><tr><td>%s</td>", wordBuff);
        } else {
            fprintf(fp, "<td>%s</td>", wordBuff);
        }
    }
}

int validPick(char board[DIM_X][DIM_Y], char wordBuff[BUFSIZ], int x, int y,
              int length, int type, int direction)
{
    if (type == 0) {
        if (direction == 0) {
            if ((x + length) <= DIM_X) {
                for (int i = 0; i < length; i++) {
                    if (wordBuff[i] != board[x + i][y] &&
                        board[x + i][y] != '*') {
                        overlap = 1;
                        return 0;
                    }
                }
                return 1;
            } else {
                return 0;
                overlap = 0;
            }
        }

        if (direction == 1) {
            if ((x - length) > -1) {
                for (int i = 0; i < length; i++) {
                    if (wordBuff[i] != board[x - i][y] &&
                        board[x - i][y] != '*') {
                        overlap = 1;
                        return 0;
                    }
                }
                return 1;
            } else {
                return 0;
                overlap = 0;
            }
        }
    }
    //---------------------//
    if (type == 1) {
        if (direction == 0) {
            if ((y + length) <= DIM_Y) {
                for (int i = 0; i < length; i++) {
                    if (wordBuff[i] != board[x][y + i] &&
                        board[x][y + i] != '*') {
                        overlap = 1;
                        return 0;
                    }
                }
                //printf(wordBuff);
                //printf("%d\n",length);
                return 1;
            } else {
                return 0;
                overlap = 0;
            }
        }

        if (direction == 1) {
            if ((y - length) > -1) {
                for (int i = 0; i < length; i++) {
                    if (wordBuff[i] != board[x][y - i] &&
                        board[x][y - i] != '*') {
                        overlap = 1;
                        return 0;
                    }
                }
                return 1;
            } else {
                return 0;
                overlap = 0;
            }
        }
    }
    //---------------------//
    if (type == 2) {
        if (direction == 0) {
            if ((x + length) <= DIM_X && (y + length) <= DIM_Y) {
                for (int i = 0; i < length; i++) {
                    if (wordBuff[i] != board[x + i][y + i] &&
                        board[x + i][y + i] != '*') {
                        overlap = 1;
                        return 0;
                    }
                }
                return 1;
            } else {
                return 0;
                overlap = 0;
            }
        }

        if (direction == 1) {
            if ((x - length) > -1 && (y - length) > -1) {
                for (int i = 0; i < length; i++) {
                    if (wordBuff[i] != board[x - i][y - i] &&
                        board[x - i][y - i] != '*') {
                        overlap = 1;
                        return 0;
                    }
                }
                return 1;
            } else {
                return 0;
                overlap = 0;
            }
        }
    }
    //---------------------//
    if (type == 3) {
        
        if (direction == 0) {
            if ((x - length) <= DIM_X && (y + length) <= DIM_Y) {
                for (int i = 0; i < length; i++) {
                    if (wordBuff[i] != board[x - i][y + i] &&
                        board[x - i][y + i] != '*') {
                        overlap = 1;
                        return 0;
                    }
                }
                return 1;
            } else {
                return 0;
                overlap = 0;
            }
        }

        if (direction == 1) {
            if ((x + length) > -1 && (y - length) > -1) {
                for (int i = 0; i < length; i++) {
                    if (wordBuff[i] != board[x + i][y - i] &&
                        board[x + i][y - i] != '*') {
                        overlap = 1;
                        return 0;
                    }
                }
                return 1;
            } else {
                return 0;
                overlap = 0;
            }
        }
    }
    fprintf(stderr, "Invalid choice\n");
    return 0;
}
