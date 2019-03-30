#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<time.h>
#define DELAY 1000000000
#include<stdbool.h>

void animation(void);
bool allocateMemory(int width, int height);
void freeMemory(int width);
void generateRandomCoordinates(int *x, int *y, int width, int height);
void generateBoardData(int width, int height, int bombs);
void discoverSurroundingBoxes(int x, int y, int width, int height);
void printBoard(int width, int height, int cursorX, int cursorY);
void refreshScreen(int width, int height, int cursorX, int cursorY);
bool hasWinned(int width, int height, int bombs);
bool saveGame(const char* filename, int width, int height, int bombs);
bool loadGame(const char* filename, int *width, int *height, int *bombs);



char **board;
char **displayedBoard;

const char BOMB = 'X';
const char EMPTY = 'E';
const char DISCOVERED = 'D';
const char FLAG = 'F';
int main()
{
    system("PAUSE");
    animation();//animation done
    //printing menu
    FILE*fp,*fp1;
    char ch3,ch,ch1,ch4;
    int position=1,pos;
    while(1)
    {
        ch3='a';
        fp=fopen("header.txt","r");
        while(ch3!=EOF)
        {
            ch3=fgetc(fp);
            printf("%c",ch3);
        }
        fclose(fp);

        if(position==1)
            printf("                          >>>>>>>>>>>>>>>START GAME<<<<<<<<<<<<<<<\n                                                                   OBJECTIVE\n                                                                                    CONTROLS\n                                                                                           LOAD GAME\n                                                                                              EXIT\n            ");
        else if(position==2)
            printf("                                                                                         START GAME\n                                                                           >>>>>>>>>>>>>>>OBJECTIVE<<<<<<<<<<<<<<<\n                                                                                           CONTROLS\n                                                                                           LOAD GAME\n                                                                                             EXIT");
        else if(position==3)
            printf("                                                                                         START GAME\n                                                                                          OBJECTIVE      \n                                                                          >>>>>>>>>>>>>>>>CONTROLS<<<<<<<<<<<<<<<\n                                                                                          LOAD GAME\n                                                                                            EXIT\n");
        else if(position==4)
        {
            printf("                                                                                          START GAME\n                                                                                           OBJECTIVE\n                                                                                           CONTROLS\n                                                                            >>>>>>>>>>>>>>>LOAD GAME<<<<<<<<<<<<<<<\n                                                                                             EXIT\n");
        }
        else if(position==5)
        {
            printf("                                                                                          START GAME\n                                                                                           OBJECTIVE\n                                                                                           CONTROLS\n                                                                                           LOAD GAME\n                                                                              >>>>>>>>>>>>>>>EXIT<<<<<<<<<<<<<<<\n");
        }
        ch=getche();
        system("cls");
        if(ch==13)
        {
            int WIDTH, HEIGHT, BOMBS;
            bool gameFinished=false;
            int code=0, choice=0;
            int cursorX=0, cursorY=0;
            char c;
            switch (position)
            {
            case 1:
                pos=1;
                while(1)
                {
                    //system("cls");
                    ch3='a';
                    fp=fopen("header.txt","r");
                    while(ch3!=EOF)
                    {
                        ch3=fgetc(fp);
                        printf("%c",ch3);
                    }
                    fclose(fp);
                    if(pos==1)
                        printf("                          >>>>>>>>>>>>>>>EASY<<<<<<<<<<<<<<<\n                                                                   MEDIUM\n                                                                                    HARD\n                                                                                           CUSTOM\n");
                    else if(pos==2)
                        printf("                                                                                         EASY\n                                                                           >>>>>>>>>>>>>>>MEDIUM<<<<<<<<<<<<<<<\n                                                                                           HARD\n                                                                                           CUSTOM\n");
                    else if(pos==3)
                        printf("                                                                                         EASY\n                                                                                          MEDIUM      \n                                                                          >>>>>>>>>>>>>>>>HARD<<<<<<<<<<<<<<<\n                                                                                          CUSTOM\n");
                    else if(pos==4)
                    {
                        printf("                                                                                          EASY\n                                                                                           MEDIUM\n                                                                                           HARD\n                                                                            >>>>>>>>>>>>>>>CUSTOM<<<<<<<<<<<<<<<\n");
                    }
                    ch1=getche();
                    system("cls");
                    if(ch1==13)
                    {


                        bool isOk = false;
                        clock_t start,end;
                        double timetaken;
                        switch(pos)
                        {
                        case 1:
                            srand(time(NULL));

                            WIDTH=8;
                            HEIGHT=8;
                            BOMBS=10;
                            if(allocateMemory(WIDTH, HEIGHT))
                            {
                                generateBoardData(WIDTH, HEIGHT, BOMBS);
                                refreshScreen(WIDTH, HEIGHT, cursorX, cursorY);
                            }
                            else
                            {
                                gameFinished=true;
                                code=1;
                            }
                            start=clock();
                            while (!gameFinished)
                            {
                                if (1/*kbhit()*/)
                                {
                                    c = getch();
                                    if(c==75)  // left
                                    {
                                        cursorX=cursorX==0?0:cursorX-1;
                                    }
                                    else if(c==72)  // up
                                    {
                                        cursorY=cursorY==0?0:cursorY-1;
                                    }
                                    else if(c==77)  // right
                                    {
                                        cursorX=cursorX==WIDTH-1?cursorX:cursorX+1;
                                    }
                                    else if(c==80)  // down
                                    {
                                        cursorY=cursorY==HEIGHT-1?cursorY:cursorY+1;
                                    }
                                    else if(c=='f')  // "f" for flag
                                    {
                                        displayedBoard[cursorX][cursorY] = FLAG;
                                    }
                                    else if(c==' ')  // space
                                    {
                                        c = board[cursorX][cursorY];
                                        if(c==BOMB)
                                        {
                                            displayedBoard[cursorX][cursorY] = board[cursorX][cursorY];
                                            gameFinished=true;
                                            code=2;
                                        }
                                        else if(c==EMPTY)
                                        {
                                            discoverSurroundingBoxes(cursorX, cursorY, WIDTH, HEIGHT);
                                        }
                                        else
                                        {
                                            displayedBoard[cursorX][cursorY] = board[cursorX][cursorY];
                                        }
                                    }
                                    else if(c=='q')
                                    {
                                        gameFinished=true;

                                        printf("\nWould you like to save the game ? (Y/n) ");
                                        c = getch();
                                        if(c=='Y' || c=='y' || c==13)
                                        {
                                            if(saveGame("game", WIDTH, HEIGHT, BOMBS))
                                            {
                                                code=4;
                                            }
                                            else
                                            {
                                                code=1;
                                            }
                                        }
                                        else
                                        {
                                            code=0;
                                        }
                                    }

                                    refreshScreen(WIDTH, HEIGHT, cursorX, cursorY);
                                    if(hasWinned(WIDTH, HEIGHT, BOMBS))
                                    {
                                        gameFinished=true;
                                        code=3;
                                    }
                                }
                            }
                            end=clock();
                            timetaken=(double)(end-start)/CLOCKS_PER_SEC;
                            freeMemory(WIDTH);

                            if(code==1)
                            {
                                printf("\nError happened...\n");
                            }
                            else if(code==2)
                            {
                                printf("\nYou lost !\n");
                            }
                            else if(code==3)
                            {
                                printf("\nYou winned !\n");
                                printf("Time Elasped %lf\n",timetaken);
                            }
                            else if(code==4)
                            {
                                printf("\nGame saved !\n");
                            }

                            printf("Press any key to exit\n");
                            getch();
                            system("cls");
                            //head=add(head);
                            //getche();
                            break;
                        case 2:
                            srand(time(NULL));

                            WIDTH=15;
                            HEIGHT=15;
                            BOMBS=45;
                            if(allocateMemory(WIDTH, HEIGHT))
                            {
                                generateBoardData(WIDTH, HEIGHT, BOMBS);
                                refreshScreen(WIDTH, HEIGHT, cursorX, cursorY);
                            }
                            else
                            {
                                gameFinished=true;
                                code=1;
                            }
                            start=clock();
                            while (!gameFinished)
                            {
                                if (1/*kbhit()*/)
                                {
                                    c = getch();
                                    if(c==75)  // left
                                    {
                                        cursorX=cursorX==0?0:cursorX-1;
                                    }
                                    else if(c==72)  // up
                                    {
                                        cursorY=cursorY==0?0:cursorY-1;
                                    }
                                    else if(c==77)  // right
                                    {
                                        cursorX=cursorX==WIDTH-1?cursorX:cursorX+1;
                                    }
                                    else if(c==80)  // down
                                    {
                                        cursorY=cursorY==HEIGHT-1?cursorY:cursorY+1;
                                    }
                                    else if(c=='f')  // "f" for flag
                                    {
                                        displayedBoard[cursorX][cursorY] = FLAG;
                                    }
                                    else if(c==' ')  // space
                                    {
                                        c = board[cursorX][cursorY];
                                        if(c==BOMB)
                                        {
                                            displayedBoard[cursorX][cursorY] = board[cursorX][cursorY];
                                            gameFinished=true;
                                            code=2;
                                        }
                                        else if(c==EMPTY)
                                        {
                                            discoverSurroundingBoxes(cursorX, cursorY, WIDTH, HEIGHT);
                                        }
                                        else
                                        {
                                            displayedBoard[cursorX][cursorY] = board[cursorX][cursorY];
                                        }
                                    }
                                    else if(c=='q')
                                    {
                                        gameFinished=true;

                                        printf("\nWould you like to save the game ? (Y/n) ");
                                        c = getch();
                                        if(c=='Y' || c=='y' || c==13)
                                        {
                                            if(saveGame("game", WIDTH, HEIGHT, BOMBS))
                                            {
                                                code=4;
                                            }
                                            else
                                            {
                                                code=1;
                                            }
                                        }
                                        else
                                        {
                                            code=0;
                                        }
                                    }

                                    refreshScreen(WIDTH, HEIGHT, cursorX, cursorY);
                                    if(hasWinned(WIDTH, HEIGHT, BOMBS))
                                    {
                                        gameFinished=true;
                                        code=3;
                                    }
                                }
                            }
                            end=clock();
                            timetaken=(double)(end-start)/CLOCKS_PER_SEC;
                            freeMemory(WIDTH);

                            if(code==1)
                            {
                                printf("\nError happened...\n");
                            }
                            else if(code==2)
                            {
                                printf("\nYou lost !\n");
                            }
                            else if(code==3)
                            {
                                printf("\nYou demined the field !\n");
                                printf("Time Elasped %lf\n",timetaken);
                            }
                            else if(code==4)
                            {
                                printf("\nGame saved !\n");
                            }

                            printf("Press any key to exit\n");
                            getch();
                            system("cls");
                            //head=add(head);
                            //getche();
                            break;
                        case 3:
                            srand(time(NULL));

                            WIDTH=20;
                            HEIGHT=20;
                            BOMBS=80;
                            if(allocateMemory(WIDTH, HEIGHT))
                            {
                                generateBoardData(WIDTH, HEIGHT, BOMBS);
                                refreshScreen(WIDTH, HEIGHT, cursorX, cursorY);
                            }
                            else
                            {
                                gameFinished=true;
                                code=1;
                            }
                            start=clock();
                            while (!gameFinished)
                            {
                                if (1/*kbhit()*/)
                                {
                                    c = getch();
                                    if(c==75)  // left
                                    {
                                        cursorX=cursorX==0?0:cursorX-1;
                                    }
                                    else if(c==72)  // up
                                    {
                                        cursorY=cursorY==0?0:cursorY-1;
                                    }
                                    else if(c==77)  // right
                                    {
                                        cursorX=cursorX==WIDTH-1?cursorX:cursorX+1;
                                    }
                                    else if(c==80)  // down
                                    {
                                        cursorY=cursorY==HEIGHT-1?cursorY:cursorY+1;
                                    }
                                    else if(c=='f')  // "f" for flag
                                    {
                                        displayedBoard[cursorX][cursorY] = FLAG;
                                    }
                                    else if(c==' ')  // space
                                    {
                                        c = board[cursorX][cursorY];
                                        if(c==BOMB)
                                        {
                                            displayedBoard[cursorX][cursorY] = board[cursorX][cursorY];
                                            gameFinished=true;
                                            code=2;
                                        }
                                        else if(c==EMPTY)
                                        {
                                            discoverSurroundingBoxes(cursorX, cursorY, WIDTH, HEIGHT);
                                        }
                                        else
                                        {
                                            displayedBoard[cursorX][cursorY] = board[cursorX][cursorY];
                                        }
                                    }
                                    else if(c=='q')
                                    {
                                        gameFinished=true;

                                        printf("\nWould you like to save the game ? (Y/n) ");
                                        c = getch();
                                        if(c=='Y' || c=='y' || c==13)
                                        {
                                            if(saveGame("game", WIDTH, HEIGHT, BOMBS))
                                            {
                                                code=4;
                                            }
                                            else
                                            {
                                                code=1;
                                            }
                                        }
                                        else
                                        {
                                            code=0;
                                        }
                                    }

                                    refreshScreen(WIDTH, HEIGHT, cursorX, cursorY);
                                    if(hasWinned(WIDTH, HEIGHT, BOMBS))
                                    {
                                        gameFinished=true;
                                        code=3;
                                    }
                                }
                            }
                            end=clock();
                            timetaken=(double)(end-start)/CLOCKS_PER_SEC;
                            freeMemory(WIDTH);

                            if(code==1)
                            {
                                printf("\nError happened...\n");
                            }
                            else if(code==2)
                            {
                                printf("\nYou lost !\n");
                            }
                            else if(code==3)
                            {
                                printf("\nYou winned !\n");
                                printf("Time Elasped %lf\n",timetaken);
                            }
                            else if(code==4)
                            {
                                printf("\nGame saved !\n");
                            }

                            printf("Press any key to exit\n");
                            getch();
                            system("cls");
                            //head=add(head);
                            //getche();
                            break;
                        case 4:
                            srand(time(NULL));
                            while(!isOk)
                            {
                                printf("Board width : ");
                                scanf("%d", &WIDTH);
                                printf("Board height : ");
                                scanf("%d", &HEIGHT);
                                printf("Bombs : ");
                                scanf("%d", &BOMBS);

                                if(WIDTH>1 && WIDTH<=100 && HEIGHT>1 && HEIGHT<=100 && BOMBS<WIDTH*HEIGHT)
                                {
                                    isOk=true;
                                }
                                else
                                {
                                    printf("\n Width and Height must be greater than 1 and less than or equal to 100.\nThe number of bombs must be smaller than the total amount of rows.\n\n");
                                }
                            }
                            if(allocateMemory(WIDTH, HEIGHT))
                            {
                                generateBoardData(WIDTH, HEIGHT, BOMBS);
                                refreshScreen(WIDTH, HEIGHT, cursorX, cursorY);
                            }
                            else
                            {
                                gameFinished=true;
                                code=1;
                            }
                            start=clock();
                            while (!gameFinished)
                            {
                                if (1/*kbhit()*/)
                                {
                                    c = getch();
                                    if(c==75)  // left
                                    {
                                        cursorX=cursorX==0?0:cursorX-1;
                                    }
                                    else if(c==72)  // up
                                    {
                                        cursorY=cursorY==0?0:cursorY-1;
                                    }
                                    else if(c==77)  // right
                                    {
                                        cursorX=cursorX==WIDTH-1?cursorX:cursorX+1;
                                    }
                                    else if(c==80)  // down
                                    {
                                        cursorY=cursorY==HEIGHT-1?cursorY:cursorY+1;
                                    }
                                    else if(c=='f')  // "f" for flag
                                    {
                                        displayedBoard[cursorX][cursorY] = FLAG;
                                    }
                                    else if(c==' ')  // space
                                    {
                                        c = board[cursorX][cursorY];
                                        if(c==BOMB)
                                        {
                                            displayedBoard[cursorX][cursorY] = board[cursorX][cursorY];
                                            gameFinished=true;
                                            code=2;
                                        }
                                        else if(c==EMPTY)
                                        {
                                            discoverSurroundingBoxes(cursorX, cursorY, WIDTH, HEIGHT);
                                        }
                                        else
                                        {
                                            displayedBoard[cursorX][cursorY] = board[cursorX][cursorY];
                                        }
                                    }
                                    else if(c=='q')
                                    {
                                        gameFinished=true;

                                        printf("\nWould you like to save the game ? (Y/n) ");
                                        c = getch();
                                        if(c=='Y' || c=='y' || c==13)
                                        {
                                            if(saveGame("game", WIDTH, HEIGHT, BOMBS))
                                            {
                                                code=4;
                                            }
                                            else
                                            {
                                                code=1;
                                            }
                                        }
                                        else
                                        {
                                            code=0;
                                        }
                                    }

                                    refreshScreen(WIDTH, HEIGHT, cursorX, cursorY);
                                    if(hasWinned(WIDTH, HEIGHT, BOMBS))
                                    {
                                        gameFinished=true;
                                        code=3;
                                    }
                                }
                            }
                            end=clock();
                            timetaken=(double)(end-start)/CLOCKS_PER_SEC;
                            freeMemory(WIDTH);

                            if(code==1)
                            {
                                printf("\nError happened...\n");
                            }
                            else if(code==2)
                            {
                                printf("\nYou lost !\n");
                            }
                            else if(code==3)
                            {
                                printf("\nYou winned !\n");
                                printf("time elasped   %lf\n",timetaken);
                            }
                            else if(code==4)
                            {
                                printf("\nGame saved !\n");
                            }

                            printf("Press any key to exit\n");
                            getch();
                            system("cls");
                            //head=add(head);
                            //getche();
                            break;
                        }
                        break;
                    }
                    if(ch1==72)
                    {
                        pos--;
                    }
                    else if(ch1==80)
                    {
                        pos++;
                    }
                    if(pos>4)
                    {
                        pos=1;
                    }
                    else if(pos<1)
                    {
                        pos=4;
                    }
                }
                //head=add(head);
                //getche();
                break;
            case 2:
                system("cls");
                ch3='a',ch4='a';
                fp=fopen("header.txt","r");
                while(ch3!=EOF)
                {
                    ch3=fgetc(fp);
                    printf("%c",ch3);
                }
                fclose(fp);
                fp1=fopen("objective.txt","r");
                while(ch4!=EOF)
                {
                    ch4=fgetc(fp);
                    printf("%c",ch4);
                }
                getche();
                system("cls");
                //print(head);
                //getche();
                break;
            case 3:
                system("cls");
                ch3='a',ch4='a';
                fp=fopen("header.txt","r");
                while(ch3!=EOF)
                {
                    ch3=fgetc(fp);
                    printf("%c",ch3);
                }
                fclose(fp);
                fp1=fopen("control.txt","r");
                while(ch4!=EOF)
                {
                    ch4=fgetc(fp);
                    printf("%c",ch4);
                }
                getche();
                system("cls");
                //user_exit(head);
                //exit(0);
                break;
            case 4:
                if(loadGame("game", &WIDTH, &HEIGHT, &BOMBS))
                {
                    refreshScreen(WIDTH, HEIGHT, cursorX, cursorY);
                }
                else
                {
                    printf("NO saved fie found");
                    getche();
                    exit(0);
                    //gameFinished=true;
                    //code=1;
                }
                while (!gameFinished)
                {
                    if (1/*kbhit()*/)
                    {
                        c = getch();
                        if(c==75)  // left
                        {
                            cursorX=cursorX==0?0:cursorX-1;
                        }
                        else if(c==72)  // up
                        {
                            cursorY=cursorY==0?0:cursorY-1;
                        }
                        else if(c==77)  // right
                        {
                            cursorX=cursorX==WIDTH-1?cursorX:cursorX+1;
                        }
                        else if(c==80)  // down
                        {
                            cursorY=cursorY==HEIGHT-1?cursorY:cursorY+1;
                        }
                        else if(c=='f')  // "f" for flag
                        {
                            displayedBoard[cursorX][cursorY] = FLAG;
                        }
                        else if(c==' ')  // space
                        {
                            c = board[cursorX][cursorY];
                            if(c==BOMB)
                            {
                                displayedBoard[cursorX][cursorY] = board[cursorX][cursorY];
                                gameFinished=true;
                                code=2;
                            }
                            else if(c==EMPTY)
                            {
                                discoverSurroundingBoxes(cursorX, cursorY, WIDTH, HEIGHT);
                            }
                            else
                            {
                                displayedBoard[cursorX][cursorY] = board[cursorX][cursorY];
                            }
                        }
                        else if(c=='q')
                        {
                            gameFinished=true;

                            printf("\nWould you like to save the game ? (Y/n) ");
                            c = getch();
                            if(c=='Y' || c=='y' || c==13)
                            {
                                if(saveGame("game", WIDTH, HEIGHT, BOMBS))
                                {
                                    code=4;
                                }
                                else
                                {
                                    code=1;
                                }
                            }
                            else
                            {
                                code=0;
                            }
                        }

                        refreshScreen(WIDTH, HEIGHT, cursorX, cursorY);
                        if(hasWinned(WIDTH, HEIGHT, BOMBS))
                        {
                            gameFinished=true;
                            code=3;
                        }
                    }
                }

                freeMemory(WIDTH);

                if(code==1)
                {
                    printf("\nError happened...\n");
                }
                else if(code==2)
                {
                    printf("\nYou lost !\n");
                }
                else if(code==3)
                {
                    printf("\nYou winned !\n");
                }
                else if(code==4)
                {
                    printf("\nGame saved !\n");
                }

                printf("Press any key to exit\n");
                getch();
                system("cls");
                break;
            case 5:
                printf("Thank You for playing ");
                exit(0);
                break;
            }
        }
        if(ch==72)
        {
            position--;
        }
        else if(ch==80)
        {
            position++;
        }
        if(position>5)
        {
            position=1;
        }
        else if(position<1)
        {
            position=5;
        }

    }

}
void animation(void)
{
    long long int i;
    char ch;
    //system("PAUSE");
    system("color 0E");
    FILE *fp;
    fp=fopen("game1.txt","r");
    while(ch!=EOF)
    {
        ch=fgetc(fp);
        printf("%c",ch);
    }
    for(i=0; i<DELAY; i++)
    {

    }
    fclose(fp);
    system("cls");
    ch='a';
    fp=fopen("game2.txt","r");
    while(ch!=EOF)
    {
        ch=fgetc(fp);
        printf("%c",ch);
    }
    system("color 09");
    for(i=0; i<DELAY; i++)
    {

    }
    fclose(fp);
    system("cls");
    ch='a';
    fp=fopen("game3.txt","r");
    while(ch!=EOF)
    {
        ch=fgetc(fp);
        printf("%c",ch);
    }
    system("color 0A");
    for(i=0; i<DELAY; i++)
    {

    }
    fclose(fp);
    system("cls");
    ch='a';
    fp=fopen("game4.txt","r");
    while(ch!=EOF)
    {
        ch=fgetc(fp);
        printf("%c",ch);
    }
    system("color 0B");
    for(i=0; i<DELAY; i++)
    {

    }
    fclose(fp);
    system("cls");
    ch='a';
    fp=fopen("game5.txt","r");
    while(ch!=EOF)
    {
        ch=fgetc(fp);
        printf("%c",ch);
    }
    system("color 0C");
    for(i=0; i<DELAY; i++)
    {

    }
    fclose(fp);
    system("cls");
    ch='a';
    fp=fopen("game6.txt","r");
    while(ch!=EOF)
    {
        ch=fgetc(fp);
        printf("%c",ch);
    }
    system("color 0D");
    for(i=0; i<DELAY; i++)
    {

    }
    fclose(fp);
    system("cls");
    ch='a';
    fp=fopen("game7.txt","r");
    while(ch!=EOF)
    {
        ch=fgetc(fp);
        printf("%c",ch);
    }
    system("color 0E");
    for(i=0; i<DELAY; i++)
    {

    }
    fclose(fp);
    system("cls");
    ch='a';
    fp=fopen("game8.txt","r");
    while(ch!=EOF)
    {
        ch=fgetc(fp);
        printf("%c",ch);
    }
    system("color 0F");
    for(i=0; i<DELAY; i++)
    {

    }
    fclose(fp);
    system("cls");
    ch='a';
    fp=fopen("game9.txt","r");
    while(ch!=EOF)
    {
        ch=fgetc(fp);
        printf("%c",ch);
    }
    system("color 0E");
    for(i=0; i<DELAY; i++)
    {

    }
    fclose(fp);
    system("cls");
    ch='a';
    fp=fopen("game10.txt","r");
    while(ch!=EOF)
    {
        ch=fgetc(fp);
        printf("%c",ch);
    }
    system("color 0C");
    for(i=0; i<DELAY; i++)
    {

    }
    fclose(fp);
    system("cls");
    ch='a';
    fp=fopen("game11.txt","r");
    while(ch!=EOF)
    {
        ch=fgetc(fp);
        printf("%c",ch);
    }
    system("color 0E");
    for(i=0; i<DELAY; i++)
    {

    }
    fclose(fp);
    system("cls");
}
bool allocateMemory(int width, int height)
{
    int i;
    if ((displayedBoard = malloc(width * sizeof(char*))) != NULL)
    {
        for(i=0 ; i<width ; i++)
        {
            if ((displayedBoard[i] = malloc(height * sizeof(char*))) == NULL)
            {
                return false;
            }
        }
    }

    if ((board = malloc(width * sizeof(char*))) != NULL)
    {
        for(i=0 ; i<width ; i++)
        {
            if ((board[i] = malloc(height * sizeof(char*))) == NULL)
            {
                return false;
            }
        }
    }

    return true;
}

void freeMemory(int width)
{
    int i;
    for(i=0 ; i<width ; i++)
    {
        free(board[i]);
        free(displayedBoard[i]);
    }
    free(board);
    free(displayedBoard);
}

void generateRandomCoordinates(int *x, int *y, int width, int height)
{
    *x = rand()%width;
    *y = rand()%height;
}

void generateBoardData(int width, int height, int bombs)
{
    int i,j,x,y;

    // generate bombs
    for(i=0 ; i<bombs ; i++)
    {
        generateRandomCoordinates(&x, &y, width, height);
        /*if(board[x][y]==BOMB)
        {
            for(;;)
            {
                generateRandomCoordinates(&x, &y, width, height);
                if(board[x][y]==BOMB)
                {
                    continue;
                }
                else
                {
                    board[x][y] = BOMB;
                    break;
                }
            }
        }*/
         board[x][y] = BOMB;
    }

    // set numbers
    int counter;
    for(i=0 ; i<width ; i++)
    {
        for(j=0 ; j<height ; j++)
        {
            counter=0;
            if(board[i][j]!=BOMB)
            {
                for(x=-1 ; x<2 ; x++)
                {
                    for(y=-1 ; y<2 ; y++)
                    {
                        if(x!=0 || y!=0)
                        {
                            if(i+x>=0 && i+x<width && j+y>=0 && j+y<height)
                            {
                                if(board[i+x][j+y]==BOMB)
                                {
                                    counter++;
                                }
                            }
                        }
                    }
                }
                if(counter!=0)
                {
                    board[i][j]=counter+48;
                }
                else
                {
                    board[i][j]=EMPTY;
                }
            }
            displayedBoard[i][j] = '.';
        }
    }
}

void discoverSurroundingBoxes(int x, int y, int width, int height)
{
    board[x][y]=DISCOVERED;
    displayedBoard[x][y]=DISCOVERED;

    int i;

    // search left
    if(x>0)
    {
        for(i=x-1 ; i>=0 ; i--)
        {
            if(board[i][y]==EMPTY && board[i][y]!=BOMB && board[i][y]!=FLAG)
            {
                discoverSurroundingBoxes(i,y,width,height);
            }
            else if(board[i][y]!=BOMB && board[i][y]!=DISCOVERED && board[i][y]!=EMPTY && board[i][y]!=FLAG && board[i+1][y]==DISCOVERED)
            {
                displayedBoard[i][y] = board[i][y];
            }
            else
            {
                break;
            }
        }
    }

    // search right
    if(x<width-1)
    {
        for(i=x+1 ; i<width ; i++)
        {
            if(board[i][y]==EMPTY && board[i][y]!=BOMB && board[i][y]!=FLAG)
            {
                discoverSurroundingBoxes(i,y,width,height);
            }
            else if(board[i][y]!=BOMB && board[i][y]!=DISCOVERED && board[i][y]!=EMPTY && board[i][y]!=FLAG && board[i-1][y]==DISCOVERED)
            {
                displayedBoard[i][y] = board[i][y];
            }
            else
            {
                break;
            }
        }
    }

    // search top
    if(y>0)
    {
        for(i=y-1 ; i>=0 ; i--)
        {
            if(board[x][i]==EMPTY && board[x][i]!=BOMB && board[x][i]!=FLAG)
            {
                discoverSurroundingBoxes(x,i,width,height);
            }
            else if(board[x][i]!=BOMB && board[x][i]!=DISCOVERED && board[x][i]!=EMPTY && board[x][i]!=FLAG && board[x][i+1]==DISCOVERED)
            {
                displayedBoard[x][i] = board[x][i];
            }
            else
            {
                break;
            }
        }
    }

    // search bottom
    if(y<height-1)
    {
        for(i=y+1 ; i<height ; i++)
        {
            if(board[x][i]==EMPTY && board[x][i]!=BOMB && board[x][i]!=FLAG)
            {
                discoverSurroundingBoxes(x,i,width,height);
            }
            else if(board[x][i]!=BOMB && displayedBoard[x][i]!=DISCOVERED && board[x][i]!=EMPTY && board[x][i]!=FLAG && board[x][i-1]==DISCOVERED)
            {
                displayedBoard[x][i] = board[x][i];
            }
            else
            {
                break;
            }
        }
    }
}

void printBoard(int width, int height, int cursorX, int cursorY)
{
    int i,j;
    for(i=0 ; i<height ; i++)
    {
        for(j=0 ; j<width ; j++)
        {
            printf("%c ", displayedBoard[j][i]);
        }
        printf("\n");

        if(i==cursorY)
        {
            for(j=0 ; j<width ; j++)
            {
                if(j==cursorX)
                {
                    printf("-");
                    break;
                }
                else
                    printf("  ");
            }
            printf("\n");
        }
    }
}

void refreshScreen(int width, int height, int cursorX, int cursorY)
{
    system("cls");
    printBoard(width, height, cursorX, cursorY);
}

bool hasWinned(int width, int height, int bombs)
{
    int i, j, discoveredBoxes=0;
    for(i=0 ; i<width ; i++)
    {
        for(j=0 ; j<height ; j++)
        {
            if(displayedBoard[i][j]==DISCOVERED || (displayedBoard[i][j]!='.' && displayedBoard[i][j]!=FLAG))
            {
                discoveredBoxes++;
            }
        }
    }
    return discoveredBoxes==width*height-bombs;
}

bool saveGame(const char* filename, int width, int height, int bombs)
{
    int i,j;
    FILE *file = fopen(filename, "w+");
    if(file==NULL)
    {
        return false;
    }
    fprintf(file, "%d\n%d\n%d\n", width, height, bombs);
    for(i=0 ; i<width ; i++)
    {
        for(j=0 ; j<height ; j++)
        {
            fprintf(file, "%d\n", board[i][j]);
            fprintf(file, "%d\n", displayedBoard[i][j]);
        }
    }
    fclose(file);
    return true;
}

bool loadGame(const char* filename, int *width, int *height, int *bombs)
{
    int i,j,x;
    FILE *file = fopen(filename, "r");
    if(file==NULL)
    {
        return false;
    }

    fscanf(file, "%d", width);
    fscanf(file, "%d", height);
    fscanf(file, "%d", bombs);

    if(!allocateMemory(*width, *height))
    {
        return false;
    }

    for(i=0 ; i<*width ; i++)
    {
        for(j=0 ; j<*height ; j++)
        {
            fscanf(file, "%d", &x);
            board[i][j]=x;
            fscanf(file, "%d", &x);
            displayedBoard[i][j]=x;
        }
    }
    fclose(file);
    return true;
}
