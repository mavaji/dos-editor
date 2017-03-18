/**
 * author: Vahid Mavaji
 * Year: 2000
 */

#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<dos.h>
#include<graphics.h>

#define STARTX 2
#define ENDX 64
#define STARTY 6
#define ENDY 21
#define M 30

struct line {
    char str[80];
    int lineno;
    struct line *plink, *nlink;
};

struct cur {
    int x;
    int y;
} cursor;

struct line *p, *new1, *start, *end;
int ins;
int cury;
char filename[80];
FILE *fp;

void key(int x1, int y1, int x2, int y2, int delta, int choice);

void write(int a, int b, char *ch[M], int n, int k, int font, int snd);

void demo(void);

int menu(void);

void edit(void);

void newf(void);

int savef(void);

void openf(int choice);

void quite(void);

void dbox(int x1, int y1, int x2, int y2, int color);

void box(int x1, int y1, int x2, int y2, int color);

void delstr(char s[], int pos);

void insstr(char s[], char ch, int pos);

int find(void);

void change(void);

int isstr(char s1[], char s2[], int where);

void changestr(char s[], char s1[], char s2[]);

void screen(void);

void ins_line(void);

void del_line(void);

void scrollup(void);

void scrolldown(void);

void pageup(void);

void pagedown(void);

void clearline(int lno);

void insert(void);

void fcursor(void);

void ESC(void);

void main(int argc, char *argv[]) {
    int choice;
    char ch;
    ins = 1;
    cursor.x = 1;
    cursor.y = 1;
    cury = 1;
    demo();
    if (argc > 1) strcpy(filename, argv[1]);
    else strcpy(filename, "NONE NAME");

    screen();
    fcursor();

    textcolor(15);
    textbackground(5);
    gotoxy(2, 3);
    cprintf("%s", filename);

    textbackground(1);
    textcolor(14);
    gotoxy(2, STARTY);
    cprintf(" ");


    p = (struct line *) malloc(sizeof(struct line));
    p->plink = NULL;
    p->nlink = NULL;
    strcpy(p->str, "");
    p->lineno = 1;
    start = p;
    end = p;
    gotoxy(2, 6);
    cprintf("%s", p->str);

    if (argc > 1) openf(1);
    while (1) {
        edit();
        choice = menu();
        switch (choice) {
            case 0:
                newf();
                break;
            case 1:
                openf(0);
                break;
            case 2:
                savef();
                break;
            case 3:
                find();
                break;
            case 4:
                change();
                break;
            case 5:
                quite();
                break;
        }
    }
}

int menu(void) {
    char *s[6] = {"NEW", "OPEN", "SAVE", "FIND", "REPLACE", "EXIT"};
    int y = 10;
    char ch;
    union REGS r;
    r.h.ah = 1;
    r.h.ch = -130;
    r.h.cl = -130;
    int86(0x10, &r, &r);
    box(70, y, 76, y, 4);
    textcolor(15);
    textbackground(4);
    gotoxy(70, y);
    cprintf("%s", s[0]);
    do {
        ch = getch();
        if ((int) ch != 0) {
            if ((int) ch == 13) {
                box(70, y, 76, y, 2);
                textcolor(15);
                textbackground(2);
                gotoxy(70, y);
                cprintf("%s", s[y - 10]);
                insert();
                insert();
                return (y - 10);
            } else if ((int) ch == 27) {
                box(70, y, 76, y, 2);
                textcolor(15);
                textbackground(2);
                gotoxy(70, y);
                cprintf("%s", s[y - 10]);
                insert();
                insert();
                return (-1);
            } else continue;
        }
        ch = getch();
        switch ((int) ch) {
            case 72:
                box(70, y, 76, y, 2);
                textcolor(15);
                textbackground(2);
                gotoxy(70, y);
                cprintf("%s", s[y - 10]);
                y--;
                if (y == 9)y = 15;
                box(70, y, 76, y, 4);
                textcolor(15);
                textbackground(4);
                gotoxy(70, y);
                cprintf("%s", s[y - 10]);
                break;
            case 80:
                box(70, y, 76, y, 2);
                textcolor(15);
                textbackground(2);
                gotoxy(70, y);
                cprintf("%s", s[y - 10]);
                y++;
                if (y == 16)y = 10;
                box(70, y, 76, y, 4);
                textcolor(15);
                textbackground(4);
                gotoxy(70, y);
                cprintf("%s", s[y - 10]);
                break;
        }
    } while (1);
}

void newf(void) {
    char ch;
    int i;
    textcolor(15);
    textbackground(8);
    gotoxy(10, 24);
    cprintf("Save The Current File(Y/N)?");
    do {
        ch = getch();
        if ((int) ch == 27) {
            ESC();
            return;
        }

    } while (toupper(ch) != 'Y' && toupper(ch) != 'N');
    box(2, 25, ENDX, 25, 4);
    if (toupper(ch) == 'Y') { if (savef() == 0)return; }
    strcpy(filename, "NONE NAME");

    screen();
    textcolor(15);
    textbackground(5);
    gotoxy(2, 3);
    cprintf("%s", filename);

    p = start;
    do {
        p = p->nlink;
        free(p->plink);
    } while (p != NULL);

    p = (struct line *) malloc(sizeof(struct line));
    p->plink = NULL;
    p->nlink = NULL;
    strcpy(p->str, "");
    p->lineno = 1;
    start = p;
    gotoxy(STARTX, STARTY - 1 + cury);
    cprintf("%s", p->str);
    cury = 1;
    cursor.y = 1;
    cursor.x = 1;
}

void openf(int choice) {
    struct line *po, *newp;
    char s[80], s1[80];
    char ch;
    int i, k = 1;

    textcolor(15);
    textbackground(8);
    if (choice == 0) {
        do {
            gotoxy(10, 24);
            cprintf("Enter [path]File Name:");
            gets(s);
        } while (strcmp(s, "") == 0);


        strcpy(filename, s);
    } else strcpy(s, filename);

    fp = fopen(s, "r+");
    if (fp == NULL) {
        box(10, 24, 65, 24, 8);
        textcolor(15);
        textbackground(8);
        gotoxy(10, 24);
        cprintf("Can\'t Open The File!");
        getch();
        box(10, 24, 65, 24, 8);
        return;
    }
    if (choice == 0) {
        box(10, 24, 65, 24, 8);
        newf();
        strcpy(filename, s);
    }
    box(2, 3, 65, 3, 5);
    textcolor(15);
    textbackground(5);
    gotoxy(2, 3);
    cprintf("%s", filename);

    po = start;
    while (k) {

        fgets(s, ENDX - STARTX, fp);

        if (!feof(fp))delstr(s, strlen(s)); else break;

        strcpy(po->str, s);
        newp = (struct line *) malloc(sizeof(struct line));
        newp->plink = po;
        newp->nlink = NULL;
        newp->lineno = po->lineno + 1;
        strcpy(newp->str, "");
        po->nlink = newp;
        po = newp;
    }
    fclose(fp);
    po = po->plink;
    free(po->nlink);
    po->nlink = NULL;
    po = start;
    textcolor(14);
    textbackground(1);
    for (i = 1; i <= ENDY - 5; i++) {
        gotoxy(STARTX, STARTY - 1 + i);
        cprintf("%s", po->str);
        if (po->nlink == NULL) break;
        po = po->nlink;
    }
    p = start;
    fclose(fp);
}

int savef(void) {
    struct line *po;
    char s[80];
    char ch;
    int i;
    if (strcmp(filename, "NONE NAME") == 0) {
        do {
            box(10, 24, 65, 24, 8);
            gotoxy(10, 24);
            textcolor(15);
            textbackground(8);
            cprintf("Enter[path]File Name:");
            ch = getch();
            if ((int) ch == 27) {
                ESC();
                return (0);
            } else cprintf("%c", ch);
            gets(s);
            insstr(s, ch, 1);
            box(10, 24, 65, 24, 8);
        } while (strcmp(s, "") == 0);
        strcpy(filename, s);

        fp = fopen(filename, "w+");
        if (fp == NULL) {
            box(10, 24, 65, 24, 8);
            textcolor(15);
            textbackground(8);
            gotoxy(10, 24);
            cprintf("Can\'t Creat The File!");
            getch();
            box(10, 24, 65, 24, 8);
            return (0);
        }
    } else {
        fp = fopen(filename, "r+");
        if (fp == NULL) {
            box(10, 24, 65, 24, 8);
            textcolor(15);
            textbackground(8);
            gotoxy(10, 24);
            cprintf("Can\'t Save In The File!");
            getch();
            box(10, 24, 65, 24, 8);
            return (0);
        }
    }
    rewind(fp);
    po = start;
    while (1) {
        fputs(po->str, fp);
        if (po->nlink != NULL)fputc('\n', fp);
        if (po->nlink == NULL) {
            fputc('\n', fp);
            fputc((char) 26, fp);
            fclose(fp);
            break;
        } else po = po->nlink;
    }
    box(2, 3, 65, 3, 5);
    textcolor(15);
    textbackground(5);
    gotoxy(2, 3);
    cprintf("%s", filename);
    fclose(fp);
    return (1);
}

void edit(void) {
    char temp[80];
    char ch;
    int i;

    textcolor(14);
    textbackground(1);
    strcpy(temp, "");
    while (1) {
        fcursor();
        textcolor(14);
        textbackground(1);

        gotoxy(STARTX - 1 + cursor.x, STARTY - 1 + cury);
        ch = getch();
        if (ch != 0) {
            switch ((int) ch) {
                case 8/*bkspace*/:
                    if (cursor.x == 1) {
                        if (p->plink != NULL) {
                            p = p->plink;
                            if (cursor.y > 1)cursor.y--;
                            if (cury > 1)cury--;
                            else {
                                p = p->nlink;
                                scrolldown();
                                p = p->plink;
                            }
                            cursor.x = STARTX + strlen(p->str);
                        } else break;
                    }
                    delstr(p->str, cursor.x - 1);
                    clearline(cury);
                    gotoxy(STARTX, STARTY - 1 + cury);
                    cprintf("%s", p->str);
                    cursor.x--;
                    break;

                case 13/*enter*/:
                    do {
                        new1 = (struct line *) malloc(sizeof(struct line));
                    } while (new1 == NULL);
                    new1->plink = p;
                    new1->nlink = p->nlink;
                    new1->lineno = p->lineno + 1;
                    strcpy(new1->str, "");
                    for (i = cursor.x - 1; i < strlen(p->str); i++)
                        new1->str[i - (cursor.x - 1)] = p->str[i];

                    new1->str[i - (cursor.x - 1)] = '\0';
                    p->str[cursor.x - 1] = '\0';
                    if (cury == ENDY - 5)
                        scrollup();

                    clearline(cury);

                    gotoxy(STARTX, STARTY - 1 + cury);
                    cprintf("%s", p->str);
                    p->nlink = new1;
                    p = new1->nlink;
                    p->plink = new1;
                    if (p != NULL && cury < ENDY - 5 - 1)ins_line();
                    clearline(cury + 1);
                    gotoxy(STARTX, STARTY - 1 + cury + 1);
                    cprintf("%s", new1->str);


                    cursor.x = 1;
                    cursor.y++;
                    if (cury < ENDY - 5)cury++;
                    if (p != NULL)p = p->plink;
                    else
                        p = new1;
                    break;


                default:
                    if (cursor.x < ENDX - 1) {
                        if (ins == 1) {
                            insstr(p->str, ch, cursor.x);
                            cursor.x++;
                        } else {
                            if (strcmp(p->str, "") != 0)delstr(p->str, cursor.x);
                            insstr(p->str, ch, cursor.x);
                            cursor.x++;
                        }
                        clearline(cury);
                        gotoxy(STARTX, STARTY - 1 + cury);
                        cprintf("%s", p->str);
                    }
                    break;
            }
            continue;
        } else ch = getch();
        switch ((int) ch) {
            case 71/*home*/:
                cursor.x = 1;
                break;
            case 79/*end*/:
                cursor.x = strlen(p->str) + 1;
                break;
            case 82/*insert*/:
                insert();
                break;

            case 73/*pgup*/:
                pageup();
                break;
            case 81/*pgdn*/:
                pagedown();
                break;
            case 83/*delete*/:
                if (cursor.x == strlen(p->str) + 1) {
                    if (p->nlink != NULL) {
                        new1 = p->nlink;
                        strcat(p->str, new1->str);
                        clearline(cury);
                        gotoxy(STARTX, STARTY - 1 + cury);
                        cprintf("%s", p->str);

                        if (new1->nlink != NULL) {
                            p->nlink = new1->nlink;
                            (new1->nlink)->plink = p;
                            new1->nlink = NULL;
                            new1->plink = NULL;
                            free(new1);
                            p = p->nlink;
                            del_line();
                            p = p->plink;
                        } else {
                            new1->nlink = NULL;
                            new1->plink = NULL;
                            free(new1);
                            p->nlink = NULL;
                            clearline(cury + 1);
                        }
                    }
                } else {
                    delstr(p->str, cursor.x);
                    clearline(cury);
                    gotoxy(STARTX, STARTY - 1 + cury);
                    cprintf("%s", p->str);
                }
                break;
            case 72/*up*/:
                if (p->plink != NULL) {
                    cursor.y--;
                    if (cury > 1)cury--;
                    else scrolldown();
                    p = p->plink;
                    if (cursor.x > strlen(p->str))cursor.x = 1 + strlen(p->str);
                }
                break;
            case 80/*down*/:
                if (p->nlink != NULL) {
                    cursor.y++;
                    if (cury < ENDY - 5)cury++;
                    else {
                        scrollup();
                        cury++;
                    }
                    p = p->nlink;
                    if (cursor.x > strlen(p->str))cursor.x = 1 + strlen(p->str);
                }
                break;
            case 75/*left*/:
                if (cursor.x > 1) cursor.x--;
                else {
                    if (p->plink != NULL) {

                        cursor.y--;
                        if (cury > 1)cury--;
                        else scrolldown();
                        cursor.x = strlen(p->str) + 1;
                        p = p->plink;
                    }
                }
                break;
            case 77/*right*/:
                if (cursor.x <= strlen(p->str)) cursor.x++;
                else {
                    if (p->nlink != NULL) {
                        cursor.x = 1;
                        cursor.y++;
                        if (cury < ENDY - 5)cury++;
                        else {
                            scrollup();
                            cury++;
                        }
                        p = p->nlink;
                    }
                }

                break;

            case 46/*ALT+C*/:
                strcpy(temp, p->str);
                break;

            case 25/*ALT+P*/:
                for (i = 0; i < strlen(temp); i++)
                    insstr(p->str, temp[i], i + cursor.x);
                clearline(cury);
                gotoxy(STARTX, STARTY - 1 + cury);
                cprintf("%s", p->str);
                break;

            case 33/*ALT+F*/:
                return;
            default:
                continue;
        }
    }
}

void dbox(int x1, int y1, int x2, int y2, int color) {
    int i;
    textcolor(color);
    gotoxy(x1, y1);
    cprintf("\xC9");
    gotoxy(x2, y1);
    cprintf("\xBB");
    gotoxy(x1, y2);
    cprintf("\xC8");
    gotoxy(x2, y2);
    cprintf("\xBC");

    for (i = x1 + 1; i < x2; i++) {
        gotoxy(i, y1);
        cprintf("\xCD");
        gotoxy(i, y2);
        cprintf("\xCD");
    }
    for (i = y1 + 1; i < y2; i++) {
        gotoxy(x1, i);
        cprintf("\xBA");
        gotoxy(x2, i);
        cprintf("\xBA");
    }
}

void box(int x1, int y1, int x2, int y2, int color) {
    int i, j;
    textcolor(color);
    for (i = x1; i <= x2; i++)
        for (j = y1; j <= y2; j++) {
            gotoxy(i, j);
            cprintf("\xDB");
        }
}

void screen(void) {
    textmode(C80);
    textbackground(7);
    clrscr();

    box(2, 3, 66, 4, 0);
    box(1, 2, 65, 3, 5);
    textcolor(14);
    textbackground(5);
    gotoxy(1, 2);
    cprintf(" FileName: ");

    box(2, 6, 66, 23, 0);
    box(1, 5, 65, 22, 1);
    textbackground(1);
    dbox(1, 5, 65, 22, 15);

    box(2, 24, 65, 24, 8);
    textcolor(15);
    textbackground(8);
    gotoxy(2, 24);
    cprintf("Comment>");

    box(71, 11, 77, 16, 0);
    box(70, 10, 76, 15, 2);
    textcolor(15);
    textbackground(2);
    gotoxy(70, 10);
    cprintf("NEW");
    gotoxy(70, 11);
    cprintf("OPEN");
    gotoxy(70, 12);
    cprintf("SAVE");
    gotoxy(70, 13);
    cprintf("FIND");
    gotoxy(70, 14);
    cprintf("REPLACE");
    gotoxy(70, 15);
    cprintf("EXIT");
}

void insstr(char s[], char ch, int pos) {
    int i;
    s[strlen(s) + 1] = '\0';
    for (i = strlen(s); i >= pos; i--)
        s[i] = s[i - 1];
    s[pos - 1] = ch;
}

void delstr(char s[], int pos) {
    int i;
    for (i = pos - 1; i < strlen(s) - 1; i++)
        s[i] = s[i + 1];
    s[i] = '\0';
}

int find(void) {
    int k, i, j, c;
    struct line *po;
    char s[80];

    box(10, 24, 65, 24, 8);
    textcolor(15);
    textbackground(8);
    gotoxy(10, 24);
    cprintf("Find What:");
    gets(s);

    c = cursor.y;
    po = p;
    while (1) {
        textcolor(14);
        textbackground(1);
        k = isstr(po->str, s, 0);
        if (k != -1) {
            p = po;
            for (i = 1; i < 16; i++) {
                clearline(i);
                gotoxy(STARTX, STARTY - 1 + i);
                cprintf("%s", po->str);
                if (po->nlink == NULL) {
                    for (j = i + 1; j <= 16; j++)
                        clearline(j);
                    break;
                }
                po = po->nlink;
            }
            for (i = 16; i > 1; i--)
                if (po->plink != NULL)po = po->plink;

            cury = 1;
            cursor.y = c;
            box(10, 24, 65, 24, 8);
            return (1);
        } else {
            if (po == NULL) {
                box(10, 24, 65, 24, 8);
                textcolor(15);
                textbackground(8);
                gotoxy(10, 24);
                cprintf("String Not Found!");
                getch();
                box(10, 24, 65, 24, 8);
                return (-1);
            }
            po = po->nlink;
            c++;
        }

    }
}

void change(void) {
    struct line *po;
    int i;
    char s1[80], s2[80];

    box(10, 24, 65, 24, 8);
    gotoxy(10, 24);
    cprintf("Change What:");
    gets(s1);

    box(10, 24, 65, 24, 8);
    gotoxy(10, 24);
    cprintf("To What:");
    gets(s2);
    box(10, 24, 65, 24, 8);

    po = start;
    while (1) {
        changestr(po->str, s1, s2);
        if (po->nlink == NULL) break;
        po = po->nlink;
    }
    po = p;
    for (i = cury; i > 1; i--)
        po = po->plink;
    textcolor(14);
    textbackground(1);
    for (i = 1; i <= 16; i++) {
        clearline(i);
        gotoxy(STARTX, STARTY - 1 + i);
        cprintf("%s", po->str);
        if (po->nlink == NULL) break;
        po = po->nlink;
    }
}

int isstr(char s1[], char s2[], int where) {
    int i, j;

    for (i = where; i < strlen(s1); i++)
        if (s2[0] == s1[i]) {
            for (j = 1; j < strlen(s2); j++)
                if (s2[j] != s1[j + i]) break;
            if (j < strlen(s2)) {
                i++;
                continue;
            } else break;
        }
    if (i < strlen(s1))
        return (i);
    else
        return (-1);
}

void changestr(char s[], char s1[], char s2[]) {
    int k, i;
    int t = 0;
    while (1) {
        if (t < strlen(s)) k = isstr(s, s1, t); else return;
        if (k == -1)
            return;
        else {
            for (i = 0; i < strlen(s1); i++)
                delstr(s, k + 1);

            for (i = k; i < k + strlen(s2); i++)
                insstr(s, s2[i - k], i + 1);

            t = k + strlen(s2);
        }

    }
}

void ins_line(void) {
    int i;
    struct line *po;
    po = p;
    while (1) {
        (po->lineno)++;
        if (po->nlink == NULL) break;
        po = po->nlink;
    }
    po = p;
    for (i = cury + 2; i <= ENDY - 5; i++) {
        clearline(i);
        gotoxy(STARTX, STARTY + i - 1);
        cprintf("%s", po->str);
        if (po->nlink == NULL) break;
        po = po->nlink;
    }
}

void del_line(void) {
    int i;
    struct line *po;
    po = p;
    while (1) {
        (po->lineno)--;
        if (po->nlink == NULL) break;
        po = po->nlink;
    }
    po = p;
    for (i = cury + 1; i <= ENDY - 5; i++) {
        clearline(i);
        gotoxy(STARTX, STARTY + i - 1);
        cprintf("%s", po->str);
        if (po->nlink == NULL && i < ENDY - 5) {
            clearline(i + 1);
            break;
        }
        po = po->nlink;
    }

}

void scrollup(void) {
    int i;
    struct line *po;
    po = p;
    for (i = ENDY - 1; i >= STARTY + 1; i--)
        p = p->plink;
    cury = 0;
    del_line();
    cury = ENDY - 5 - 1;
    p = po;

}

void scrolldown(void) {
    int i;
    struct line *po;
    po = p;
    p = p->plink;
    cury = -1;
    ins_line();
    cury = 1;
    p = po;

}

void pageup(void) {
    struct line *po;
    int i, j;

    if (cursor.y <= 16) {
        if (cursor.y > 1) {
            po = start;
            for (i = 1; i <= 16; i++) {
                clearline(i);
                gotoxy(STARTX, STARTY - 1 + i);
                cprintf("%s", po->str);
                if (po->nlink != NULL)po = po->nlink;
                else break;
            }
            cursor.y = 1;
            cury = 1;
            p = start;
        }

    } else {
        po = p;
        for (i = cury + 16; i > 1; i--)
            if (po->plink != NULL)po = po->plink;


        for (i = 1; i <= 16; i++) {
            clearline(i);
            gotoxy(STARTX, STARTY - 1 + i);
            cprintf("%s", po->str);
            po = po->nlink;
        }
        cursor.y -= 16;
        for (i = 16; i >= cury; i--)
            po = po->plink;
        p = po;
    }
}

void pagedown(void) {
    struct line *po;
    int i, j;
    po = p;
    for (i = cury; i <= 16; i++) {
        po = po->nlink;
        if (po == NULL) return;
    }

    for (i = 1; i <= 16; i++) {

        clearline(i);
        gotoxy(STARTX, STARTY - 1 + i);
        cprintf("%s", po->str);
        if (po->nlink == NULL) {
            p = po;
            cursor.y += 16 - (cury - i);
            cury = i;
            for (j = i + 1; j <= 16; j++) clearline(j);
            return;
        }/*if*/
        if (po->nlink != NULL) po = po->nlink;
    }
    cursor.y += 16;
    for (i = 16; i >= cury; i--)
        po = po->plink;
    p = po;
}

void clearline(int lno) {
    union REGS r;
    int i;
    char ch = ' ';
    r.h.ah = 1;
    r.h.ch = 40;
    r.h.ch = 40;
    int86(0x10, &r, &r);

    textbackground(1);
    for (i = 2; i <= ENDX; i++) {
        gotoxy(i, lno + STARTY - 1);
        cprintf("%c", ch);
    }
    insert();
    insert();
}

void insert(void) {
    union REGS in, out;

    if (ins == 1) {
        ins = 0;
        in.h.ah = 1;
        in.h.ch = 0;
        in.h.cl = 7;
        int86(0x10, &in, &out);
    } else {
        ins = 1;
        in.h.ah = 1;
        in.h.ch = 6;
        in.h.cl = 7;
        int86(0x10, &in, &out);
    }
    return;
}

void fcursor(void) {
    textcolor(15);
    gotoxy(3, 5);
    cprintf("%d:%d\xCD\xCD\xCD\xCD", cursor.y, cursor.x);
}

void ESC(void) {
    gotoxy(2, 24);
    clearline(19);
    return;
}

void quite(void) {
    char ch;
    box(10, 24, 65, 24, 8);
    textcolor(15);
    textbackground(8);
    gotoxy(10, 24);
    cprintf("Save The Current File(Y/N):");
    do {
        ch = getch();
    } while (toupper(ch) != 'Y' && toupper(ch) != 'N');
    if (toupper(ch) == 'Y') savef();
    exit(1);
}

void demo(void) {
    int i = 0, j;
    char *ch1[M] = {"V", "A", "H", "I", "D", " ", "M", "A", "V", "A", "J", "I"};
    char *ch2[M] = {"p", "r", "e", "s", "e", "n", "t", "s", ":"};
    char *ch3[M] = {"H", "a", "r", "d", " ", "S", "t", "r", "o", "n", "g"};
    char *ch4[M] = {"@", "e", "d", "i", "t", "o", "r"};
    initgraph(&i, &i, "c:\\borlandc\\bgi");
    setbkcolor(7);

    write(200, 70, ch1, 12, 4, 1, 1);
    write(250, 100, ch2, 11, 1, 3, 0);
    for (i = 1; i <= 30; i += 3)
        key(100 + i, 150 + i, 500 - i, 180 - i, 1, 1);
    write(100, 200, ch3, 11, 7, 1, 1);
    write(500, 270, ch4, 7, 5, 2, 0);

    j = 0;
    for (i = 1; i <= 150; i += 5) {
        key(1, 330 + i, 640, 480, 1, j);
        if (j)j = 0; else j = 1;
    }
    getch();
    closegraph();
}

void write(int a, int b, char *ch[M], int n, int k, int font, int snd) {
    int i, j, c;
    int x = 1;
    for (j = 0; j < n; j++) {
        for (i = 10; i >= k; i--) {
            setcolor(8);
            settextstyle(font, 0, i);
            c = textwidth("X");

            outtextxy(a - i * x + j * c + 2, b - i + 2, ch[j]);
            setcolor(15);
            outtextxy(a - i * x + j * c, b - i, ch[j]);
            delay(20);
            if (i != k) {
                setcolor(7);
                outtextxy(a - i * x + j * c + 2, b - i + 2, ch[j]);
                outtextxy(a - i * x + j * c, b - i, ch[j]);
            }
        }
/*if(snd)
{
 sound(3000);
 delay(1150);
 nosound();
} */
    }
}

void key(int x1, int y1, int x2, int y2, int delta, int choice) {
    int c;
    if (choice == 1)c = delta; else c = -delta;
    setfillstyle(1, 7);
    bar(x1, y1, x2, y2);
    if (choice)setfillstyle(1, 15); else setfillstyle(1, 8);
    bar(x1 - c, y1 - c, x1, y2);
    bar(x1, y1 - c, x2 + c, y1);
    if (choice)setfillstyle(1, 8); else setfillstyle(1, 15);
    bar(x1 - c, y2, x2, y2 + c);
    bar(x2, y1, x2 + c, y2 + c);
}