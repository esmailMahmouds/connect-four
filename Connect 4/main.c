#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include "xml.h"
#define MaxScoreLen 9;
#define MaxNameLen 13;

typedef struct{
        int r;
        int c;
        int x;
}rcx;
rcx prmtrs,prmtrsHold;
char gameMode;
typedef struct{
    char name[13];
    int colour;
    int turns;
    int score;
    char chip;
}playerData;
typedef struct{
    char name[13];
    int score;
}HighScr;
typedef struct{
        int sampleColumn;
        int choiceScore;
}BestChoice;
playerData player1,player2;
char Empty[13];
long long timr;
int Animation=1;
int firstTime=0;

void blue(){printf("\033[0;34m");}
void red(){printf("\033[1;31m");}
void yellow(){printf("\033[0;33m");}
void reset(){printf("\033[0m");}

void End();
void HighScoreLoad();
void HighScoreSave(int winner);
void Redo(char a[][prmtrs.c],int RU[]);
void Undo(char a[][prmtrs.c],int RU[]);
void load(char a[][prmtrs.c]);
void save(char a[][prmtrs.c],int RU[]);
int AI(char a[][prmtrs.c],char one,char two);
int full(char a[][prmtrs.c]);
void countMovs(char a[][prmtrs.c]);
int countFours(char chip,char a[][prmtrs.c]);
void bar();
void columnIndex();
void board(char a[][prmtrs.c]);
void Rmve(char chip,char a[][prmtrs.c],int e);
void fill(char chip ,char a[][prmtrs.c],int q);
int choose(char a[][prmtrs.c]);
void Menu(char a[][prmtrs.c],int RU[]);
void gameHuman(char a[][prmtrs.c]);
void gameComputer(char a[][prmtrs.c]);
void initiate(char a[][prmtrs.c]);
void start(char a[][prmtrs.c]);
void MainMenu(char a[][prmtrs.c]);
//════════════════════════════════════════════════════════════════════════

int main(){
    int parameters[3];
    if(firstTime==0){
        read_xml(parameters);
        prmtrsHold.r=prmtrs.r=parameters[0];
        prmtrsHold.c=prmtrs.c=parameters[1];
        prmtrsHold.x=prmtrs.x=parameters[2];
        firstTime++;
    }
    prmtrs.x=prmtrsHold.x;
    prmtrs.c=prmtrsHold.c;
    prmtrs.r=prmtrsHold.r;
    char a[prmtrs.r][prmtrs.c];
    for(int i=0;i<13;i++)
        Empty[i]=' ';
    player1.colour=1;player1.chip='X';player1.turns=0;player1.score=0;
    player2.colour=2;player2.chip='O';player2.turns=0;player2.score=0;
    initiate(a);
    MainMenu(a);
}

//════════════════════════════════════════════════════════════════════════

void MainMenu(char a[][prmtrs.c]){
    char key;
    system("cls");
    printf("Main Menu\nPlease,stick to the Given inputs!\nS:Start\nL:Load\nH:HighScores\nQ:Exit\n");
    key=getch();
    switch(key){
        case'S':
        case's':
            start(a);
            break;
        case'L':
        case'l':
            load(a);
            break;
        case'H':
        case'h':
            HighScoreLoad();
            MainMenu(a);
            break;
        case'Q':
        case'q':
            exit(0);
            break;
        default:
            MainMenu(a);
    }
}
void start(char a[][prmtrs.c]){
    char key;
    system("cls");
    initiate(a);
    countMovs(a);
    player1.score=0;
    player2.score=0;/*Initializing each time the User Starts a NewGame*/
    printf("Select GameMode:\nH:Player Vs Player\nC:Player Vs Computer\n");
    key=getch();
    switch(key){
        case 'H':
        case 'h':
            gameHuman(a);
            break;
        case 'C':
        case 'c':
            system("cls");
            gameComputer(a);
            break;
        default:
            start(a);}
}
void initiate(char a[][prmtrs.c]){
    //initializing all elements of the array to ' '
    for(int i=0;i<prmtrs.r;i++)
        for(int j=0;j<prmtrs.c;j++)
            a[i][j]=' ';
}
void gameComputer(char a[][prmtrs.c]){
    timr=time(NULL);
    int RU[prmtrs.c*prmtrs.r];/*Arrray to save each move taken to operate undo and redo with*/
    for(int i=0;i<prmtrs.c*prmtrs.r;i++)
        RU[i]=-1;/*intializing all chosen columns to -1 to control where we undo and redo to*/
    int e;/*Selected column*/
    gameMode='c';
    board(a);
    while(full(a)==0)/*Operating as long as a isn't full*/
        {
        printf("\033[0m(~):Menu\n\033[1;31mYour Score:%d\t\033[0;33mComputer Score:%d\n\033[1;31mYour Moves:%d\t\033[0;33mComputer Moves:%d\n",player1.score,player2.score,player1.turns,player2.turns);
        if(player1.turns<=player2.turns)/*as player1 starts he plays if his turns is equal to that of player 2*/
        {
            printf("\033[1;31mTime~%02d:%02d\n",(int)(time(NULL)-timr)/60,(int)(time(NULL)-timr)%60);
            printf("\033[1;31mChoose A Bin:");
            e=choose(a);
            if(e==-13)//An assumption Chosen number to open the inGame menu
            {
                    Menu(a,RU);
                    board(a);
            }
            else{
                RU[player1.turns+player2.turns]=e;//Assigning the chosen column in the array by using the turns as an index
                for(int i=player1.turns+player2.turns+1;i<prmtrs.r*prmtrs.c;i++)//if a player plays all remaining Redos are semi disable
                    RU[i]=-1;
                fill(player1.chip,a,e);
                player1.score=countFours(player1.chip,a);
                player1.turns++;
            }
        }
        if(player1.turns>player2.turns&&full(a)==0){
                e=AI(a,'X','O');
                RU[player1.turns+player2.turns]=e;
                for(int i=player1.turns+player2.turns+1;i<prmtrs.r*prmtrs.c;i++)
                    RU[i]=-1;
                fill(player2.chip,a,e);
                blue();
                board(a);
                player2.score=countFours(player2.chip,a);
                player2.turns++;
        }
    }
    End(a);
}
void gameHuman(char a[][prmtrs.c]){
    timr=time(NULL);
    int RU[prmtrs.r*prmtrs.c];
    for(int i=0;i<prmtrs.c*prmtrs.r;i++)
        RU[i]=-1;
    int e;
    board(a);
    gameMode='h';
    while(full(a)==0){
            if(player1.turns<=player2.turns){
                red();
                printf("\033[0m(~):Menu\n\033[1;31mPlayer 1 Score:%d\t\033[0;33mPlayer 2 Score:%d\n\033[1;31mPlayer 1 Moves:%d\t\033[0;33mPlayer 2 Moves:%d\n",player1.score,player2.score,player1.turns,player2.turns);
                printf("\033[1;31mTime~%02d:%02d\n",(int)(time(NULL)-timr)/60,(int)(time(NULL)-timr)%60);
                printf("BestMove for X is Column {%d}\n",AI(a,'O','X'));
                printf("Player 1 Can Choose A Bin:");
                e=choose(a);
                if(e==-13){
                    Menu(a,RU);
                    board(a);
                }
                else{
                    RU[player1.turns+player2.turns]=e;
                    for(int i=player1.turns+player2.turns+1;i<prmtrs.r*prmtrs.c;i++)
                        RU[i]=-1;
                    fill(player1.chip,a,e);
                    blue();
                    board(a);
                    player1.score=countFours(player1.chip,a);
                    player1.turns++;
                }
            }
            if(player1.turns>player2.turns&&full(a)==0){
                yellow();
                printf("\033[0m(~):Menu\n\033[0;33mPlayer 2 Score:%d\t\033[1;31mPlayer 1 Score:%d\n\033[0;33mPlayer 2 Moves:%d\t\033[1;31mPlayer 1 Moves:%d\n",player2.score,player1.score,player2.turns,player1.turns);
                printf("\033[0;33mTime~%02d:%02d\n",(int)(time(NULL)-timr)/60,(int)(time(NULL)-timr)%60);
                printf("BestMove for O is Column {%d}\n",AI(a,'X','O'));
                printf("Player 2 Can Choose A Bin:");
                e=choose(a);
                if(e==-13){
                    Menu(a,RU);
                    board(a);}
                else{
                    RU[player1.turns+player2.turns]=e;
                    for(int i=player1.turns+player2.turns+1;i<prmtrs.r*prmtrs.c;i++)
                        RU[i]=-1;
                    fill(player2.chip,a,e);
                    blue();
                    board(a);
                    player2.score=countFours(player2.chip,a);
                    player2.turns++;
                }
            }
    }
    End(a);
}
void Menu(char a[][prmtrs.c],int RU[]){
    char key;
    system("cls");
    board(a);
    reset();
    printf("Menu\nPlease,stick to the Given inputs!\nC:Close\nA:Animation\nS:Save\nU:undo\nR:Redo\nQ:Exit\n");
    key=getch();
    switch(key){
        case'C':
        case'c':
            break;
        case'S':
        case's':
            save(a,RU);
            Menu(a,RU);
            break;
        case'U':
        case'u':
            Undo(a,RU);
            Menu(a,RU);
            break;
        case'R':
        case'r':
            Redo(a,RU);
            Menu(a,RU);
            break;
        case'A':
        case'a':
            //An InGame Menu Option to disable the Falling chips Animation
            printf("(0)AnimationOFF\n(1)AnimationON\n");
            scanf("%d",&Animation);
            while(Animation!=0&&Animation!=1){
                printf("Choose A Valid Option!");
                scanf("%d",&Animation);
                }
            break;
        case'Q':
        case'q':
            main();
            break;
        default:
            player1.score=countFours(player1.chip,a);
            player2.score=countFours(player2.chip,a);
            Menu(a,RU);
    }
}
int choose(char a[][prmtrs.c]){
    int i;
    char numberCols[9];
    //For Using Letters as Column Indexing but it fulfills the requirements for 58 column Max
    if(prmtrs.c<=58){
        i=getch()-65;
        if(i==61)
            i=-13;
    }
    //For Using Numbers as Column Indexing from 58 and More
    else{
        scanf(" %s",&numberCols);
        i=atoi(numberCols)-1;
        if(numberCols[0]=='~')
            i=-13;
    }
    if((a[0][i]==' ') && (i<prmtrs.c) && (i>=0) || (i==-13)){
        return i;
    }
    else{
        printf("\nInValid Cloumn!");
        choose(a);
    }
}
void fill(char chip ,char a[][prmtrs.c],int q){
    if(q>=0){
        int i;
        if(Animation==1){
            if(a[0][q]==' '&&a[1][q]!=' '){
                a[0][q]=chip;
            }
            else{
                a[0][q]=chip;
                for(i=1;i<prmtrs.r;i++){
                    if(a[i][q]==' '){//Alternate printing and deleting till the bottom of the bottom while flashing the board Inbetween
                        board(a);
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
        else{//if Falling Animation is turned Off chips are put in a single frame
            for(i=prmtrs.r-1;i>=0;i--){
                if(a[i][q]==' '){
                    a[i][q]=chip;
                    break;
                }
            }
        }
    }
}
void Rmve(char chip,char a[][prmtrs.c],int e){
    //same Concept of Filling but in Reverse
    if(e>=0){
        if(Animation==1){
            for(int i=0;i<prmtrs.r;i++){
                if(a[i][e]!=' '){
                    for(int j=i;j>0;j--){
                        a[j][e]=' ';
                        a[j-1][e]=chip;
                        board(a);
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
            for(int i=0;i<prmtrs.r;i++){
                if(a[i][e]!=' '){
                    a[i][e]=' ';
                    break;
                }
            }
        }
    }
}
void board(char a[][prmtrs.c]){
    system("cls");
    blue();
    columnIndex();
    bar();
    for(int i=0;i<prmtrs.r;i++){
        for(int j=0;j<prmtrs.c;j++){
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
        bar();
    }
}
void columnIndex(){
    if(prmtrs.c<59){
        printf("  ");
        for(int i=65;(i<65+prmtrs.c)&&(i<=122);i++)
            printf(" %c  ",i);
        printf("\n");
    }
    else{
        printf("  ");
        for(int i=1;i<=prmtrs.c;i++)
            printf("%03d ",i);
        printf("\n");
    }
}
void bar(){
        printf(" ");
        for(int i=0;i<prmtrs.c;i++)
            printf("%c%c%c%c",206,205,205,205);//printing ╬═══ in a loop
        printf("%c",206);//finishing each bar in the grid with a ╬
        printf("\n");
}
int countFours(char chip,char a[][prmtrs.c]){
    int count=0,f,i,j,k;
    for(i=0;i<prmtrs.r;i++){//Counting in rows
        for(j=0;j<(prmtrs.c-3);j++){
            f=0;
            for(k=j;k<(j+4);k++)
                if(a[i][k]==chip)
                    f++;
            if(f==4)count++;
        }
    }
    for(j=0;j<prmtrs.c;j++){//Counting in Columns
        for(i=0;i<(prmtrs.r-3);i++){
            f=0;
            for(k=i;k<(i+4);k++)
                if(a[k][j]==chip)
                    f++;
            if(f==4)count++;
        }
    }
    for(i=0;i<prmtrs.r-3;i++){//Counting in 120 degree diagonals
        for(j=0;j<prmtrs.c-3;j++){
            f=0;
            for(k=0;k<4;k++)
                if(a[i+k][j+k]==chip)
                f++;
            if(f==4)count++;
        }
    }
    for(i=0;i<prmtrs.r-3;i++){//Counting in 60 degree diagonals
        for(j=3;j<prmtrs.c;j++){
            f=0;
            for(k=0;k<4;k++)
                if(a[i+k][j-k]==chip)
                f++;
            if(f==4)count++;
        }
    }
    return count;
}
void countMovs(char a[][prmtrs.c]){
    int c1=0,c2=0;
    for(int i=0;i<prmtrs.r;i++){
        for(int j=0;j<prmtrs.c;j++){
                if(a[i][j]=='X')c1++;
                else if(a[i][j]=='O')c2++;
        }
    }
    //counting each chip and assigning them to players' turns
    player1.turns=c1;
    player2.turns=c2;
}
int full(char a[][prmtrs.c]){
    //checking if the grid is full and returning 1 or not and returning 0
    for(int i=0;i<prmtrs.r;i++)
        for(int j=0;j<prmtrs.c;j++)
            if(a[i][j]==' ')
                return 0;
    return 1;
}
int AI(char a[][prmtrs.c],char one,char two){
    int animationState=Animation,tempScore,tempColumn,tempdiff,Chosen=0;
    Animation=0;
    char Test[prmtrs.r][prmtrs.c];
    BestChoice choices[prmtrs.c];
    int difference[prmtrs.c],difference1[prmtrs.c];
    for(int i=0;i<prmtrs.r;i++)
        for(int j=0;j<prmtrs.c;j++)
            Test[i][j]=a[i][j];
    for(int i=0;i<prmtrs.c;i++){
        choices[i].choiceScore=-(prmtrs.c*prmtrs.r);
        difference[i]=(prmtrs.c*prmtrs.r);
        difference1[i]=-(prmtrs.c*prmtrs.r);
    }
    for(int i=0;i<prmtrs.c;i++){
            choices[i].sampleColumn=i;
            if(Test[0][i]==' '){
                fill(two,Test,i);
                for(int j=0;j<prmtrs.c;j++){
                        if(Test[0][j]==' '){
                            fill(one,Test,j);
                            if(prmtrs.c<20){
                                for(int k=0;k<prmtrs.c;k++){
                                    if(Test[0][k]==' '){
                                        fill(two,Test,k);
                                        difference1[k]=countFours(two,Test)-countFours(one,Test);
                                        Rmve(two,Test,k);
                                    }
                                }
                                for(int ii=0;ii<prmtrs.c;ii++){
                                    for(int jj=ii+1;jj<prmtrs.c;jj++){
                                        if(difference1[ii]<difference1[jj]){
                                            tempdiff=difference1[ii];
                                            difference1[ii]=difference1[jj];
                                            difference1[jj]=tempdiff;
                                        }
                                    }
                                }
                            }
                            else
                                difference1[0]=0;
                            difference[j]=countFours(two,Test)-countFours(one,Test);
                            Rmve(one,Test,j);
                        }
                }
                for(int ii=0;ii<prmtrs.c;ii++){
                    for(int jj=ii+1;jj<prmtrs.c;jj++){
                        if(difference[ii]<difference[jj]){
                            tempdiff=difference[ii];
                            difference[ii]=difference[jj];
                            difference[jj]=tempdiff;
                        }
                    }
                }
                choices[i].choiceScore=difference[prmtrs.c-1]+difference1[0];
                Rmve(two,Test,i);
            }
    }
    for(int i=0;i<prmtrs.c;i++){
        for(int j=i+1;j<prmtrs.c;j++){
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
    Animation=animationState;
    while(a[0][choices[Chosen].sampleColumn]!=' ')
        Chosen++;
    return choices[Chosen].sampleColumn;
}
void save(char a[][prmtrs.c],int RU[]){
    system("cls");
    char key;
    FILE *s=NULL;
    printf("choose slot:\n(a)slot1\n(b)slot2\n(c)slot3\n(d)Back\n");
    key=getch();
    //Choosing A save Slot to save in by the user
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
            Menu(a,RU);
        default:
            save(a,RU);
        }
    if(s==NULL)
        printf("something went wrong");
    fwrite(&prmtrs,sizeof(rcx),1,s);//Writing the dimensions of the saved Game
    for(int i=0; i<prmtrs.r; i++)
        for(int j=0; j<prmtrs.c; j++)
            fwrite(&a[i][j],sizeof(char),1,s);//Writing the Game Board
    fwrite(&gameMode,sizeof(int),1,s);//Labeling the GameMode
    fclose(s);
}
void load(char a[][prmtrs.c]){
    //Same as Save but Reversed
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
            MainMenu(a);
            break;
        default:
            load(a);
        }
    if(s==NULL)
        load(a);
    fread(&prmtrs,sizeof(rcx),1,s);
    char b[prmtrs.r][prmtrs.c];
    initiate(b);
    for(int i=0; i<prmtrs.r; i++)
        for(int j=0; j<prmtrs.c; j++)
            fread(&b[i][j],sizeof(char),1,s);
    fread(&gameMode,sizeof(char),1,s);
    fclose(s);
    countMovs(b);
    player1.score=countFours(player1.chip,b);
    player2.score=countFours(player2.chip,b);
    if(gameMode=='h')
        gameHuman(b);
    if(gameMode=='c')
        gameComputer(b);
}
void Undo(char a[][prmtrs.c],int RU[]){
    if(player1.turns>0&&RU[player1.turns+player2.turns-1]!=-1){
        if(gameMode=='h'){
                if(player1.turns==player2.turns){
                    player2.turns--;
                    Rmve(player2.chip,a,RU[player1.turns+player2.turns]);
                }
                else{
                    player1.turns--;
                    Rmve(player1.chip,a,RU[player1.turns+player2.turns]);
                }
        }
        else if(gameMode=='c'){
                player1.turns--;
                Rmve(player2.chip,a,RU[player1.turns+player2.turns]);
                player2.turns--;
                Rmve(player1.chip,a,RU[player1.turns+player2.turns]);
        }
    }
}
void Redo(char a[][prmtrs.c],int RU[]){
    if(RU[player1.turns+player2.turns]!=-1){
        if(gameMode=='h'){
                if(player1.turns>player2.turns){
                    fill(player2.chip,a,RU[player1.turns+player2.turns]);
                    player2.turns++;
                }
                else{
                    fill(player1.chip,a,RU[player1.turns+player2.turns]);
                    player1.turns++;
                }
        }
        else if(gameMode=='c'){
                fill(player1.chip,a,RU[player1.turns+player2.turns]);
                player1.turns++;
                fill(player2.chip,a,RU[player1.turns+player2.turns]);
                player2.turns++;
        }
    }
}
void HighScoreSave(int winner){
    HighScr p[prmtrs.x],winnerData;//initializing a struct elements for holding the players data
    for(int i=0;i<prmtrs.x;i++){
        strcpy(p[i].name,Empty);
        p[i].score=0;
    }
    system("cls");
    reset();
    printf("HighScores\nRank  . Score| Name\n");
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
    s=fopen("HighScores.dat","rb");//Opening the file for reading 1st to operate on the already saved
    if(s!=NULL){//if the file was found we read
        fread(p,sizeof(HighScr),prmtrs.x,s);
        for(int i=0;i<prmtrs.x;i++){//looking for the winner's name in our structs
        if(strcasecmp(p[i].name,winnerData.name)==0){
            Found=1;
            if(p[i].score<winnerData.score){//Comparing old and new scores
                p[i].score=winnerData.score;
            }
            break;
        }
    }
    if(Found==0){
            if(winnerData.score>p[prmtrs.x-1].score){//if it wasn't found we put it in place of the last if it had a bigger score than him
                p[prmtrs.x-1].score=winnerData.score;
                strcpy(p[prmtrs.x-1].name,winnerData.name);
            }
    }
    char tempName[13];
    int tempScore;
    for(int i=0;i<prmtrs.x;i++){//we sort our array of structs
            for(int j=i+1;j<prmtrs.x;j++){
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
    }
    else{//if not we put our winner data directly the 1st place in the struct
        strcpy(p[0].name,winnerData.name);
        p[0].score=winnerData.score;
    }
    fclose(s);//close that was opened for reading
    FILE *f=NULL;
    f=fopen("HighScores.dat","wb");
    fwrite(p,sizeof(HighScr),prmtrs.x,f);
    fclose(f);
    for(int i=0;i<prmtrs.x;i++){
        printf("%02d  . %05d|%s\n",i+1,p[i].score,p[i].name);
    }
}
void HighScoreLoad(){
    HighScr p[prmtrs.x],winnerData;
    for(int i=0;i<prmtrs.x;i++){
        strcpy(p[i].name,Empty);
        p[i].score=0;
    }
    char Back;
    system("cls");
    printf("HighScores\nRank  . Score| Name\n");
    FILE *s=NULL;
    s=fopen("HighScores.dat","rb");
    if(s==NULL){
        printf("NOT FOUND!");
        getch();
    }
    else{
        fread(p,sizeof(HighScr),prmtrs.x,s);
        for(int i=0;i<prmtrs.x;i++){
            printf("%02d    . %05d|%s\n",i+1,p[i].score,p[i].name);
            }
        printf("(b)Back:");
        Back=getch();
        if(Back!='b' && Back!='B')
            HighScoreLoad(prmtrs.x);
        fclose(s);
    }
}
void End(){
    system("cls");
    int winner;
    if(player1.score>player2.score){
        printf("\033[1;31mPlayer 1 Wins!\nName Of Our Champ:");
        scanf("%s",player1.name);
        HighScoreSave(1);
    }
    else if(player1.score<player2.score&&gameMode=='h'){
        printf("\033[0;33mPlayer 2 Wins!\nName Of Our Champ:");
        scanf("%s",player2.name);
        HighScoreSave(2);
    }
    else if(player1.score==player2.score&&gameMode=='h'){
        printf("\033[0mIt's A Draw\nName of Player1:");
        scanf("%s",player1.name);
        HighScoreSave(1);
        printf("\033[0m\nName of Player2:");
        scanf("%s",player2.name);
        HighScoreSave(2);
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
