#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
char gameMode;
struct playerData{
    char name;
    int colour;
    int turns;
    int score;
    char chip;
}player1,player2;
long long timr;


void blue(){printf("\033[0;34m");}
void red(){printf("\033[1;31m");}
void yellow(){printf("\033[0;33m");}
void reset(){printf("\033[0m");}
void columnIndex(int c);
void bar(int c);
int countFours(char x,int r,int c,char a[][c]);
void count(int r,int c,char a[][c]);
int full(int r,int c,char a[][c]);
void initiate(int r,int c,char a[][c]);
int choose(int c,char a[][c]);
void fill(int chip ,int r,int c,char a[][c],int q);
void board(int r,int c,char a[][c]);
void Menu(int r,int c,char a[][c]);
int Random(int e,int c,char a[][c]);
void Turns(int turn,int r,int c,char a[][c]);
void gameHuman(int r,int c,char a[][c]);
void gameComputer(int r,int c,char a[][c]);
void start(int r,int c,char a[][c]);
void MainMenu(int r,int c,char a[][c]);
void save(int r,int c,char a[][c]);
void load(int r,int c,char a[][c]);
void End();
//════════════════════════════════════════════════════════════════════════

int main(){
    int r=31,c=61,x=10;
    char a[r][c];
    player1.colour=1;player1.chip='X';player1.turns=0;player1.score=0;
    player2.colour=2;player2.chip='O';player2.turns=0;player2.score=0;
    initiate(r,c,a);
    MainMenu(r,c,a);
}

//════════════════════════════════════════════════════════════════════════

void MainMenu(int r,int c,char a[][c]){
    char key;
    system("cls");
    printf("Main Menu\nPlease,stick to the Given inputs!\nS:Start\nL:Load\nH:HighScore\nQ:Exit\n");
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
            printf("HighScore");
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
void gameComputer(int r,int c,char a[][c]){
    timr=time(NULL);
    int e=c/2,w=1;
    gameMode='c';
    board(r,c,a);
    while(full(r,c,a)==0){
        printf("\033[0m(~):Menu\n\033[1;31mYour Score:%d\t\033[0;33mComputer Score:%d\n\033[1;31mYour Moves:%d\t\033[0;33mComputer Moves:%d\n",player1.score,player2.score,player1.turns,player2.turns);
        if(w==1){
            printf("\033[1;31mTime~%02d:%02d\n",(int)(time(NULL)-timr)/60,(int)(time(NULL)-timr)%60);
            printf("\033[1;31mChoose A Bin:");
            e=choose(c,a);
                if(e==61){
                        Menu(r,c,a);
                        board(r,c,a);
                        }
                else{
                fill(player1.chip,r,c,a,e);
                player1.score=countFours(player1.chip,r,c,a);
                player1.turns++;
                w=2;
                }
        }
        if(w==2){
                e=Random(e,c,a);
                fill(player2.chip,r,c,a,e);
                blue();
                board(r,c,a);
                player2.score=countFours(player2.chip,r,c,a);
                player2.turns++;
                w=1;
        }

    }
    End();
}
void gameHuman(int r,int c,char a[][c]){
    int turn=1;
    if(player1.turns>player2.turns)turn=2;
    timr=time(NULL);
    board(r,c,a);
    gameMode='h';
    Turns(turn,r,c,a);
    End();
}
void Turns(int turn,int r,int c,char a[][c]){
    int e;
    switch(turn){
        case 1:
            red();
            printf("(~):Menu\nPlayer 1 Score:%d\nPlayer 1 Moves:%d\n",player1.score,player1.turns);
            printf("Time~%02d:%02d\n",(int)(time(NULL)-timr)/60,(int)(time(NULL)-timr)%60);
            printf("Player 1 Can Choose A Bin:");
            e=choose(c,a);
            if(e==61){
                    if(c>61){
                        fill(player1.chip,r,c,a,e);
                        blue();
                        board(r,c,a);
                        player1.score=countFours(player1.chip,r,c,a);
                        player1.turns++;
                        turn=2;
                    }
                    Menu(r,c,a);
                    board(r,c,a);}
            else{
            fill(player1.chip,r,c,a,e);
            blue();
            board(r,c,a);
            player1.score=countFours(player1.chip,r,c,a);
            player1.turns++;
            turn=2;}
            break;
        case 2:
            yellow();
            printf("(~):Menu\nPlayer 2 Score:%d\nPlayer 2 Moves:%d\n",player2.score,player2.turns);
            printf("Time~%02d:%02d\n",(int)(time(NULL)-timr)/60,(int)(time(NULL)-timr)%60);
            printf("Player 2 Can Choose A Bin:");
            e=choose(c,a);
            if(e==61){
                    if(c>61){
                        fill(player1.chip,r,c,a,e);
                        blue();
                        board(r,c,a);
                        player1.score=countFours(player1.chip,r,c,a);
                        player1.turns++;
                        turn=2;
                    }
                    Menu(r,c,a);
                    board(r,c,a);}
            else{
            fill(player2.chip,r,c,a,e);
            blue();
            board(r,c,a);
            player2.score=countFours(player2.chip,r,c,a);
            player2.turns++;
            turn=1;}
            break;
    }
    if(full(r,c,a)==0)Turns(turn,r,c,a);
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
int full(int r,int c,char a[][c]){
    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++)
            if(a[i][j]==' ')
                return 0;
    return 1;
}
void initiate(int r,int c,char a[][c]){
    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++)
            a[i][j]=' ';
}
void fill(int chip ,int r,int c,char a[][c],int q){
    int i;
    for(i=r-1;i>=0;i--){
        if (a[i][q]==' '){
            a[i][q]= chip;
            break;              }
                       }
}
int choose(int c,char a[][c]){
    int i;
    if(c<=58){
        i=getch()-65;}
        else{
            if(1 != scanf("%d",&i))
                scanf(" %c",&i);
        }
    if((a[0][i]==' ') && (i<c) && (i>=0) || (i==61))
        return i;
    else{
        printf("\nInValid Cloumn!");
        choose(c,a);
    }
}
void columnIndex(int c){
    printf(" ");
    for(int i=65;(i<65+c)&&(i<=122);i++)
        printf("%c ",i);
    printf("\n");
}
void board(int r,int c,char a[][c]){
    system("cls");
    blue();
    columnIndex(c);
    bar(c);
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            blue();
            printf("%c",186);
            if (a[i][j]=='X'){
                    red();
                    printf("%c",a[i][j]);}
            else if (a[i][j]=='O'){
                    yellow();
                    printf("%c",a[i][j]);}
            else printf("%c",a[i][j]);
            }
        blue();
        printf("%c\n",186);
        bar(c);
}}
void bar(int c){
    for(int k=0;k<c+1;k++)
        printf("%c%c",206,205);
    printf("\n");
}
int Random(int e,int c, char a[][c]){
    srand(time(NULL));
    e=rand()%c;
    while(a[0][e]!=' '&&e<c)e++;
    while(a[0][e]!=' '&&e>=0)e--;
    return e;
}
void save(int r,int c,char a[][c]){
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
            Menu(r,c,a);
        default:
            save(r,c,a);
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
void Menu(int r,int c,char a[][c]){
    char key;
    system("cls");
    printf("Menu\nPlease,stick to the Given inputs!\nC:Close\nS:Save\nU:undo\nR:Redo\nQ:Exit\n");
    key=getch();
    switch(key){
        case'C':
        case'c':
            break;
        case'S':
        case's':
            save(r,c,a);
            Menu(r,c,a);
            break;
        case'U':
        case'u':
            printf("Undo");
            break;
        case'R':
        case'r':
            printf("Redo");
            break;
        case'Q':
        case'q':
            main();
            break;
        default:
            Menu(r,c,a);
}}
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
void End(){
if(player1.score>player2.score)
        printf("\033[1;31mPlayer 1 Wins!");
    else if(player1.score<player2.score)
        printf("\033[0;33mPlayer 2 Wins!");
    else
        printf("\033[0mIt's A Draw");
exit(0);
}
void HighScore(){


}
