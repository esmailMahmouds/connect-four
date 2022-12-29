#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include "xml.h"
#define MaxScoreLen 9;
#define MaxNameLen 13;

int r,c,x;
char gameMode;
typedef struct Data{
    char name[13];
    int colour;
    int turns;
    int score;
    char chip;
}playerData;
playerData player1,player2;
typedef struct Scores{
    char name[13];
    int score;
}High;
char Empty[13];
typedef struct{
        int sampleColumn;
        int choiceScore;
}BestChoice;
long long timr;
int Animation=1;

void blue(){printf("\033[0;34m");}
void red(){printf("\033[1;31m");}
void yellow(){printf("\033[0;33m");}
void reset(){printf("\033[0m");}

void End();
void HighScoreLoad(int x);
void HighScoreSave(int x,int winner);
void Redo(int r,int c,char a[][c],int RU[]);
void Undo(int r,int c,char a[][c],int RU[]);
void load(int r,int c,char a[][c]);
void save(int r,int c,char a[][c],int RU[]);
int AI(int r,int c,char a[][c]);
int full(int r,int c,char a[][c]);
void count(int r,int c,char a[][c]);
int countFours(char x,int r,int c,char a[][c]);
void bar(int c);
void columnIndex(int c);
void board(int r,int c,char a[][c]);
void Rmve(char chip,int c,char a[][c],int e);
void fill(char chip ,int r,int c,char a[][c],int q);
int choose(int c,char a[][c]);
void Menu(int r,int c,char a[][c],int RU[]);
void gameHuman(int r,int c,char a[][c]);
void gameComputer(int r,int c,char a[][c]);
void initiate(int r,int c,char a[][c]);
void start(int r,int c,char a[][c]);
void MainMenu(int r,int c,char a[][c]);
//════════════════════════════════════════════════════════════════════════

int main(){
    int parameters[3];
    read_xml(parameters);
    r=parameters[0];
    c=parameters[1];
    x=parameters[2];
    char a[r][c];
    for(int i=0;i<13;i++)
        Empty[i]=' ';
    player1.colour=1;player1.chip='X';player1.turns=0;player1.score=0;
    player2.colour=2;player2.chip='O';player2.turns=0;player2.score=0;
    initiate(r,c,a);
    MainMenu(r,c,a);
}

//════════════════════════════════════════════════════════════════════════

void MainMenu(int r,int c,char a[][c]){
    char key;
    system("cls");
    printf("Main Menu\nPlease,stick to the Given inputs!\nS:Start\nL:Load\nH:HighScores\nQ:Exit\n");
    key=getch();
    switch(key){
        case'S':
        case's':
            start(r,c,a);
            break;
        case'L':
        case'l':
            initiate(r,c,a);
            load(r,c,a);
            count(r,c,a);
            player1.score=countFours(player1.chip,r,c,a);
            player2.score=countFours(player2.chip,r,c,a);
            if(gameMode=='h')
                gameHuman(r,c,a);
            if(gameMode=='c')
                gameComputer(r,c,a);
            break;
         case'H':
         case'h':
            HighScoreLoad(x);
            MainMenu(r,c,a);
            break;
        case'Q':
        case'q':
            exit(0);
            break;
        default:
            MainMenu(r,c,a);
}
}
void start(int r,int c,char a[][c]){
    char key;
    system("cls");
    initiate(r,c,a);
    count(r,c,a);
    printf("Select GameMode:\nH:Player Vs Player\nC:Player Vs Computer\n");
    key=getch();
    switch(key){
        case 'H':
        case 'h':
            gameHuman(r,c,a);
            break;
        case 'C':
        case 'c':
            system("cls");
            gameComputer(r,c,a);
            break;
        default:
            start(r,c,a);}
}
void initiate(int r,int c,char a[][c]){
    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++)
            a[i][j]=' ';
}
void gameComputer(int r,int c,char a[][c]){
    timr=time(NULL);
    int RU[c*r];
    int e=c/2;
    gameMode='c';
    board(r,c,a);
    while(full(r,c,a)==0){
        printf("\033[0m(~):Menu\n\033[1;31mYour Score:%d\t\033[0;33mComputer Score:%d\n\033[1;31mYour Moves:%d\t\033[0;33mComputer Moves:%d\n",player1.score,player2.score,player1.turns,player2.turns);
        if(player1.turns<=player2.turns){
            printf("\033[1;31mTime~%02d:%02d\n",(int)(time(NULL)-timr)/60,(int)(time(NULL)-timr)%60);
            printf("\033[1;31mChoose A Bin:");
            e=choose(c,a);
            if(e==-13){
                    Menu(r,c,a,RU);
                    board(r,c,a);
            }
            else{
                RU[player1.turns+player2.turns]=e;
                for(int i=player1.turns+player2.turns+1;i<r*c;i++)
                    RU[i]=-1;
                fill(player1.chip,r,c,a,e);
                player1.score=countFours(player1.chip,r,c,a);
                player1.turns++;
            }
        }
        if(player1.turns>player2.turns&&full(r,c,a)==0){
                e=AI(r,c,a);
                RU[player1.turns+player2.turns]=e;
                for(int i=player1.turns+player2.turns+1;i<r*c;i++)
                    RU[i]=-1;
                fill(player2.chip,r,c,a,e);
                blue();
                board(r,c,a);
                player2.score=countFours(player2.chip,r,c,a);
                player2.turns++;
        }
    }
    End();
}
void gameHuman(int r,int c,char a[][c]){
    timr=time(NULL);
    int RU[r*c];
    int e;
    board(r,c,a);
    gameMode='h';
    while(full(r,c,a)==0){
            if(player1.turns<=player2.turns){
                red();
                printf("\033[0m(~):Menu\n\033[1;31mPlayer 1 Score:%d\t\033[0;33mPlayer 2 Score:%d\n\033[1;31mPlayer 1 Moves:%d\t\033[0;33mPlayer 2 Moves:%d\n",player1.score,player2.score,player1.turns,player2.turns);
                printf("\033[1;31mTime~%02d:%02d\n",(int)(time(NULL)-timr)/60,(int)(time(NULL)-timr)%60);
                printf("Player 1 Can Choose A Bin:");
                e=choose(c,a);
                if(e==-13){
                    Menu(r,c,a,RU);
                    board(r,c,a);
                }
                else{
                    RU[player1.turns+player2.turns]=e;
                    for(int i=player1.turns+player2.turns+1;i<r*c;i++)
                        RU[i]=-1;
                    fill(player1.chip,r,c,a,e);
                    blue();
                    board(r,c,a);
                    player1.score=countFours(player1.chip,r,c,a);
                    player1.turns++;
                }
            }
            if(player1.turns>player2.turns&&full(r,c,a)==0){
                yellow();
               printf("\033[0m(~):Menu\n\033[0;33mPlayer 2 Score:%d\t\033[1;31mPlayer 1 Score:%d\n\033[0;33mPlayer 2 Moves:%d\t\033[1;31mPlayer 1 Moves:%d\n",player2.score,player1.score,player2.turns,player1.turns);
                printf("\033[0;33mTime~%02d:%02d\n",(int)(time(NULL)-timr)/60,(int)(time(NULL)-timr)%60);
                printf("Player 2 Can Choose A Bin:");
                e=choose(c,a);
                if(e==-13){
                    Menu(r,c,a,RU);
                    board(r,c,a);}
                else{
                    RU[player1.turns+player2.turns]=e;
                    for(int i=player1.turns+player2.turns+1;i<r*c;i++)
                        RU[i]=-1;
                    fill(player2.chip,r,c,a,e);
                    blue();
                    board(r,c,a);
                    player2.score=countFours(player2.chip,r,c,a);
                    player2.turns++;
                }
            }
    }
    End();
}
void Menu(int r,int c,char a[][c],int RU[]){
    char key;
    system("cls");
    board(r,c,a);
    reset();
    printf("Menu\nPlease,stick to the Given inputs!\nC:Close\nA:Animation\nS:Save\nU:undo\nR:Redo\nQ:Exit\n");
    key=getch();
    switch(key){
        case'C':
        case'c':
            break;
        case'S':
        case's':
            save(r,c,a,RU);
            Menu(r,c,a,RU);
            break;
        case'U':
        case'u':
            Undo(r,c,a,RU);
            Menu(r,c,a,RU);
            break;
        case'R':
        case'r':
            Redo(r,c,a,RU);
            Menu(r,c,a,RU);
            break;
        case'A':
        case'a':
            printf("(0)AnimationOFF\n(1)AnimationON\n");
            scanf("%d",&Animation);
            while(Animation!=0&&Animation!=1){
                printf("Choose A Valid Option!");
                scanf("%d",&Animation);
                }
            break;
        case'Q':
        case'q':
            MainMenu(r,c,a);
            break;
        default:
            player1.score=countFours(player1.chip,r,c,a);
            player2.score=countFours(player2.chip,r,c,a);
            Menu(r,c,a,RU);
    }
}
int choose(int c,char a[][c]){
    int i;
    char numberCols[9];
    if(c<=58){
        i=getch()-65;
        if(i==61)
            i=-13;
    }
    else{
        scanf(" %s",&numberCols);
        i=atoi(numberCols)-1;
        if(numberCols[0]=='~')
            i=-13;
    }
    if((a[0][i]==' ') && (i<c) && (i>=0) || (i==-13)){
        return i;
    }
    else{
        printf("\nInValid Cloumn!");
        choose(c,a);
    }
}
void fill(char chip ,int r,int c,char a[][c],int q){
    if(q>=0){
        int i;
        if(Animation==1){
            if(a[0][q]==' '&&a[1][q]!=' '){
                a[0][q]=chip;
            }
            else{
                a[0][q]=chip;
                for(i=1;i<r;i++){
                    if(a[i][q]==' '){
                        board(r,c,a);
                        a[i-1][q]=' ';
                        Beep(200,75);
                        a[i][q]=chip;
                    }
                }
            }
        Beep(500,30);
        Beep(300,40);
        Beep(400,50);
        }
        else{
            for(i=r-1;i>=0;i--){
                if(a[i][q]==' '){
                    a[i][q]=chip;
                    break;
                }
            }
        }
    }
}
void Rmve(char chip,int c,char a[][c],int e){
    if(e>=0){
        if(Animation==1){
            for(int i=0;i<r;i++){
                if(a[i][e]!=' '){
                    for(int j=i;j>0;j--){
                        a[j][e]=' ';
                        a[j-1][e]=chip;
                        board(r,c,a);
                        Beep(200,75);
                    }
                    a[0][e]=' ';
                    Beep(400,50);
                    Beep(300,40);
                    Beep(500,30);
                    break;
                }
            }
        }
        else{
            for(int i=0;i<r;i++){
                if(a[i][e]!=' '){
                    a[i][e]=' ';
                    break;
                }
            }
        }
    }
}
void board(int r,int c,char a[][c]){
    system("cls");
    blue();
    columnIndex(c);
    bar(c);
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            blue();
            printf(" %c ",186);
            if (a[i][j]=='X'){
                    red();
                    printf("%c",a[i][j]);}
            else if (a[i][j]=='O'){
                    yellow();
                    printf("%c",a[i][j]);}
            else printf("%c",a[i][j]);
            }
        blue();
        printf(" %c\n",186);
        bar(c);
    }
}
void columnIndex(int c){
    if(c<59){
        printf("  ");
        for(int i=65;(i<65+c)&&(i<=122);i++)
            printf(" %c  ",i);
        printf("\n");
    }
    else{
        printf("  ");
        for(int i=1;i<=c;i++)
            printf("%03d ",i);
        printf("\n");
    }
}
void bar(int c){
        printf(" ");
        for(int i=0;i<c;i++)
            printf("%c%c%c%c",206,205,205,205);
        printf("%c",206);
        printf("\n");
}
int countFours(char x,int r,int c,char a[][c]){
    int count=0,f,i,j,k;
    for(i=0;i<r;i++){
        for(j=0;j<(c-3);j++){
            f=0;
            for(k=j;k<(j+4);k++)
                if(a[i][k]==x)
                    f++;
            if(f==4)count++;
        }
    }
    for(j=0;j<c;j++){
        for(i=0;i<(r-3);i++){
            f=0;
            for(k=i;k<(i+4);k++)
                if(a[k][j]==x)
                    f++;
            if(f==4)count++;
        }
    }
    for(i=0;i<r-3;i++){
        for(j=0;j<c-3;j++){
            f=0;
            for(k=0;k<4;k++)
                if(a[i+k][j+k]==x)
                f++;
            if(f==4)count++;
        }
    }
    for(i=0;i<r-3;i++){
        for(j=3;j<c;j++){
            f=0;
            for(k=0;k<4;k++)
                if(a[i+k][j-k]==x)
                f++;
            if(f==4)count++;
        }
    }
    return count;
}
void count(int r,int c,char a[][c]){
    int c1=0,c2=0;
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
                if(a[i][j]=='X')c1++;
                else if(a[i][j]=='O')c2++;
        }
    }
    player1.turns=c1;
    player2.turns=c2;
}
int full(int r,int c,char a[][c]){
    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++)
            if(a[i][j]==' ')
                return 0;
    return 1;
}
int AI(int r,int c, char a[][c]){
    int animationState=Animation,tempScore,tempColumn,tempdiff,Chosen=0;
    Animation=0;
    char Test[r][c];
    BestChoice choices[c];
    int difference[c],difference1[c];
    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++)
            Test[i][j]=a[i][j];
    for(int i=0;i<c;i++){
        choices[i].choiceScore=-(c*r);
        difference[i]=(c*r);
        difference1[i]=-(c*r);
    }
    for(int i=0;i<c;i++){
            choices[i].sampleColumn=i;
            if(Test[0][i]==' '){
                fill(player2.chip,r,c,Test,i);
                for(int j=0;j<c;j++){
                        if(Test[0][j]==' '){
                            fill(player1.chip,r,c,Test,j);
                            for(int k=0;k<c;k++){
                                if(Test[0][k]==' '){
                                    fill(player2.chip,r,c,Test,k);
                                    difference1[k]=countFours(player2.chip,r,c,Test)-countFours(player1.chip,r,c,Test);
                                    Rmve(player2.chip,c,Test,k);
                                }
                            }
                            for(int ii=0;ii<c;ii++){
                                for(int jj=ii+1;jj<c;jj++){
                                    if(difference1[ii]<difference1[jj]){
                                        tempdiff=difference1[ii];
                                        difference1[ii]=difference1[jj];
                                        difference1[jj]=tempdiff;
                                    }
                                }
                            }
                            difference[j]=countFours(player2.chip,r,c,Test)-countFours(player1.chip,r,c,Test);
                            Rmve(player1.chip,c,Test,j);
                        }
                }
                for(int ii=0;ii<c;ii++){
                    for(int jj=ii+1;jj<c;jj++){
                        if(difference[ii]<difference[jj]){
                            tempdiff=difference[ii];
                            difference[ii]=difference[jj];
                            difference[jj]=tempdiff;
                        }
                    }
                }
                choices[i].choiceScore=difference[c-1]+difference1[0];
                Rmve(player2.chip,c,Test,i);
            }
    }
    for(int i=0;i<c;i++){
        for(int j=i+1;j<c;j++){
            if(choices[i].choiceScore<choices[j].choiceScore){
                tempScore=choices[i].choiceScore;
                tempColumn=choices[i].sampleColumn;
                choices[i].choiceScore=choices[j].choiceScore;
                choices[i].sampleColumn=choices[j].sampleColumn;
                choices[j].choiceScore=tempScore;
                choices[j].sampleColumn=tempColumn;
            }
        }
    }
    for(int i=0;i<c;i++)
        printf("(%d %d),",choices[i].sampleColumn,choices[i].choiceScore);
    getch();
    Animation=animationState;
    while(a[0][choices[Chosen].sampleColumn]!=' ')
        Chosen++;
    return choices[Chosen].sampleColumn;
}
void save(int r,int c,char a[][c],int RU[]){
    system("cls");
    char key;
    FILE *s=NULL;
    printf("choose slot:\n(a)slot1\n(b)slot2\n(c)slot3\n(d)Back\n");
    key=getch();
    switch(key){
        case 'A':
        case 'a':
            s=fopen("save1.bin","wb");
            break;
        case 'B':
        case 'b':
            s=fopen("save2.bin","wb");
            break;
        case 'C':
        case 'c':
            s=fopen("save3.bin","wb");
            break;
        case 'D':
        case 'd':
            Menu(r,c,a,RU);
        default:
            save(r,c,a,RU);
        }
    if(s==NULL)
        printf("something went wrong");
    for(int i=0; i<r; i++)
        for(int j=0; j<c; j++)
            fwrite(&a[i][j],sizeof(char),1,s);
    fwrite(&gameMode,sizeof(int),1,s);
    fclose(s);
}
void load(int r,int c,char a[][c]){
    system("cls");
    int key;
    FILE *s=NULL;
    printf("choose slot:\n(a)slot1\n(b)slot2\n(c)slot3\n(d)Back\n");
    key=getch();
    switch(key){
        case 'A':
        case 'a':
            s=fopen("save1.bin","rb");
            break;
        case 'B':
        case 'b':
            s=fopen("save2.bin","rb");
            break;
        case 'C':
        case 'c':
            s=fopen("save3.bin","rb");
            break;
        case 'D':
        case 'd':
            MainMenu(r,c,a);
            break;
        default:
            load(r,c,a);
        }
    if(s==NULL)
        load(r,c,a);
    for(int i=0; i<r; i++)
        for(int j=0; j<c; j++)
            fread(&a[i][j],sizeof(char),1,s);
    fread(&gameMode,sizeof(char),1,s);
    fclose(s);
}
void Undo(int r,int c,char a[][c],int RU[]){
    if(player1.turns>0){
        if(gameMode=='h'){
                if(player1.turns==player2.turns){
                    player2.turns--;
                    Rmve(player2.chip,c,a,RU[player1.turns+player2.turns]);
                }
                else{
                    player1.turns--;
                    Rmve(player1.chip,c,a,RU[player1.turns+player2.turns]);
                }
        }
        else if(gameMode=='c'){
                player1.turns--;
                Rmve(player2.chip,c,a,RU[player1.turns+player2.turns]);
                player2.turns--;
                Rmve(player1.chip,c,a,RU[player1.turns+player2.turns]);
        }
    }
}
void Redo(int r,int c,char a[][c],int RU[]){
    if(RU[player1.turns+player2.turns]!=-1){
        if(gameMode=='h'){
                if(player1.turns>player2.turns){
                    fill(player2.chip,r,c,a,RU[player1.turns+player2.turns]);
                    player2.turns++;
                }
                else{
                    fill(player1.chip,r,c,a,RU[player1.turns+player2.turns]);
                    player1.turns++;
                }
        }
        else if(gameMode=='c'){
                fill(player1.chip,r,c,a,RU[player1.turns+player2.turns]);
                player1.turns++;
                fill(player2.chip,r,c,a,RU[player1.turns+player2.turns]);
                player2.turns++;
        }
    }
}
void HighScoreSave(int x,int winner){
    High p[x],winnerData;
    for(int i=0;i<x;i++){
        strcpy(p[i].name,Empty);
        p[i].score=0;
    }
    system("cls");
    reset();
    printf("HighScores\nRank| Score| Name\n");
    int Found=0;
    if(winner==1){
        winnerData.score=player1.score;
        strcpy(winnerData.name,player1.name);
        }
    else if(winner==2){
        winnerData.score=player2.score;
        strcpy(winnerData.name,player2.name);
        }
    FILE *s=NULL;
    s=fopen("HighScores.dat","rb");
    if(s!=NULL){
        fread(p,sizeof(High),x,s);
    }
    else{
        strcpy(p[1].name,winnerData.name);
        p[1].score=winnerData.score;
    }
    fclose(s);
    FILE *f=NULL;
    f=fopen("HighScores.dat","wb");
    for(int i=0;i<x;i++){
        if(strcasecmp(p[i].name,winnerData.name)==0){
            Found=1;
            if(p[i].score<winnerData.score){
                p[i].score=winnerData.score;
            }
            break;
        }
    }
    if(Found==0){
            if(winnerData.score>p[x-1].score){
                p[x-1].score=winnerData.score;
                strcpy(p[x-1].name,winnerData.name);
            }
    }
    char tempName[13];
    int tempScore;
    for(int i=0;i<x;i++){
            for(int j=i+1;j<x;j++){
                    if(p[i].score<p[j].score){
                        tempScore=p[i].score;
                        strcpy(tempName,p[i].name);
                        p[i].score=p[j].score;
                        strcpy(p[i].name,p[j].name);
                        p[j].score=tempScore;
                        strcpy(p[j].name,tempName);
                    }
            }
    }
    fwrite(p,sizeof(High),x,f);
    fclose(f);
    for(int i=0;i<x;i++){
        printf("%d   . %05d|%s\n",i+1,p[i].score,p[i].name);
    }
    sleep(1);
}
void HighScoreLoad(int x){
    High p[x],winnerData;
    char Back;
    system("cls");
    printf("HighScores\nRank. Score| Name\n");
    FILE *s=NULL;
    s=fopen("HighScores.dat","rb");
    if(s==NULL){
        printf("NOT FOUND!");
        getch();
    }
    else{
        fread(p,sizeof(High),x,s);
        for(int i=0;i<x;i++){
            printf("%d   . %05d|%s\n",i+1,p[i].score,p[i].name);
            }
        printf("(b)Back:");
        Back=getch();
        if(Back!='b' && Back!='B')
            HighScoreLoad(x);
        fclose(s);
    }
}
void End(){
    system("cls");
    int winner;
    if(player1.score>player2.score){
        printf("\033[1;31mPlayer 1 Wins!\nName Of Our Champ:");
        scanf("%s",player1.name);
        HighScoreSave(x,1);
    }
    else if(player1.score<player2.score&&gameMode=='h'){
        printf("\033[0;33mPlayer 2 Wins!\nName Of Our Champ:");
        scanf("%s",player2.name);
        HighScoreSave(x,2);
    }
    else if(player1.score==player2.score&&gameMode=='h'){
        printf("\033[0mIt's A Draw\nName of Player1:");
        scanf("%s",player1.name);
        HighScoreSave(x,1);
        printf("\033[0m\nName of Player2:");
        scanf("%s",player2.name);
        HighScoreSave(x,2);
    }
    else
        printf("\033[0mCouldn't Defeat me!,Bye%c\n",259);
    char option;
    printf("|(Press any Button to Continue) OR (Q) to Leave us!|\n");
    option = getch();
    if(option=='q'||option=='Q')
        exit(0);
    main();
}

