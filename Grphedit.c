/**
 * author: Vahid Mavaji
 * Year: 2000
 */

#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<dos.h>

#define M 30

void key(int x1, int y1, int x2, int y2, int delta, int choice);

void write(int a, int b, char *ch[M], int n, int k, int font, int snd);

void demo(void);

void screen(void);

void main() {

    int i = 0;
    initgraph(&i, &i, "c:\\borlandc\\bgi");
    demo();
    screen();
    getch();
    closegraph();
}

void demo(void) {
    int i = 0, j;
    char *ch1[M] = {"V", "A", "H", "I", "D", " ", "M", "A", "V", "A", "J", "I"};
    char *ch2[M] = {"p", "r", "e", "s", "e", "n", "t", "s", ":"};
    char *ch3[M] = {"H", "a", "r", "d", " ", "S", "t", "r", "o", "n", "g"};
    char *ch4[M] = {"@", "e", "d", "i", "t", "o", "r"};
    initgraph(&i, &i, "");
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
}

void screen(void) {
    int i, j;
    char *menu[6] = {"NEW", "OPEN", "SAVE", "FIND", "REPLACE", "EXIT"};
    char *h1[M] = {"H"};
    char *h2[M] = {"E"};
    setbkcolor(7);
    cleardevice();
    key(5, 5, 500, 30, 2, 1);
    key(5, 5, 500, 30, 1, 0);
    settextstyle(2, 0, 5);
    outtextxy(10, 10, "FileName:");

    key(5, 35, 500, 400, 2, 1);
    key(5, 35, 500, 400, 1, 0);

    key(15, 45, 490, 390, 2, 0);
    setfillstyle(1, 9);
    bar(19, 49, 486, 386);

    key(5, 405, 500, 440, 2, 1);
    key(5, 405, 500, 440, 1, 0);
    settextstyle(2, 0, 5);
    outtextxy(10, 415, "Comment:");

    write(550, 10, h1, 1, 8, 4, 0);
    write(550, 100, h2, 1, 8, 4, 0);

    for (i = 1; i <= 180; i += 30)
        key(530, 200 + i, 620, 225 + i, 1, 1);
    settextstyle(1, 0, 1);
    setcolor(8);
    for (i = 0; i < 6; i++)
        outtextxy(540, 203 + 30 * i, menu[i]);

    key(5, 450, 500, 470, 1, 1);
    settextstyle(2, 0, 4);
    outtextxy(15, 455, "Program Written By Vahid Mavaji 2000.");

    key(540, 420, 610, 440, 1, 0);

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
            delay(10);
            if (i != k) {
                setcolor(7);
                outtextxy(a - i * x + j * c + 2, b - i + 2, ch[j]);
                outtextxy(a - i * x + j * c, b - i, ch[j]);
            }
        }
        if (snd) {
            sound(3000);
            delay(300);
            nosound();
        }
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