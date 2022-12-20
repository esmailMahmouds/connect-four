#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
struct p{
    char name;
    int colour;
    int turns;
    int score;
    char chip;
}player1,player2;
struct t{
    long long start;
}tm;

void blue(){printf("\033[0;34m");}
void red(){printf("\033[1;31m");}
void yellow(){printf("\033[0;33m");}
void reset(){printf("\033[0m");}
void columnIndex(int c);
void bar(int c);
int countFours(char x,int r,int c,char a[][c]);
int full(int r,int c,char a[][c]);
int choose(int c,char a[][c]);
void fill(int chip ,int r,int c,char a[][c],int q);
void board(int r,int c,char a[][c]);
void Menu(int r,int c,char a[][c]);
int Random(int e,int c,char a[][c]);
void Turns(int i,int r,int c,char a[][c]);
void gameHuman(int r,int c,char a[][c]);
void gameComputer(int w,int r,int c,char a[][c]);
void Start(int r,int c,char a[][c]);
void MainMenu(int r,int c,char a[][c],int x);
//════════════════════════════════════════════════════════════════════════

int main(){
    int r=7,c=9,x=10;
    char a[r][c];
    player1.colour=1;player1.chip='X';player1.turns=0;player1.score=0;
    player2.colour=2;player2.chip='O';player2.turns=0;player2.score=0;
    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++)
            a[i][j]=' ';
    MainMenu(r,c,a,x);
}

//════════════════════════════════════════════════════════════════════════

void MainMenu(int r,int c,char a[][c],int x){
    char key;
    system("cls");
    printf("Main Menu\nPlease,stick to the Given inputs!\nS:Start\nL:Load\nH:HighScore\nQ:Exit\n");
    scanf(" %c",&key);
    switch(key){
        case'S':
        case's':
            start(r,c,a);
            break;
        case'L':
        case'l':
            printf("Load");
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
            MainMenu(r,c,a,x);
}
}
void start(int r,int c,char a[][c]){
    int q,w;
    system("cls");
    printf("Select GameMode:\n1:Player Vs Player\n2:Player Vs Computer\n");
    scanf("%d",&q);
    tm.start=time(NULL);
    switch(q){
        case 1:
            gameHuman(r,c,a);
            break;
        case 2:
            system("cls");
            printf("(1):Player Starts\n(2):Computer Starts\n");
            scanf("%d",&w);
            gameComputer(w,r,c,a);
            break;
        default:
            start(r,c,a);}
}
void gameComputer(int w,int r,int c,char a[][c]){
    int e=c/2;
    board(r,c,a);
    while(full(r,c,a)==0){
        printf("(~):Menu\n\033[1;31mYour Score:%d\t\033[0;33mComputer Score:%d\n\033[1;31mYour Moves:%d\t\033[0;33mComputer Moves:%d\n",player1.score,player2.score,player1.turns,player2.turns);
        if(w==1){
            printf("\033[1;31mChoose A Bin:");
            e=choose(c,a);
                if(e==61){
                        Menu(r,c,a);
                        board(r,c,a);
                        }
                else{
                fill(player1.chip,r,c,a,e);
                blue();
                board(r,c,a);
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
}
void gameHuman(int r,int c,char a[][c]){
    board(r,c,a);
    Turns(1,r,c,a);
}
void Turns(int i,int r,int c,char a[][c]){
    int e;
    switch(i){
        case 1:
            red();
            printf("(~):Menu\nPlayer 1 Score:%d\nPlayer 1 Moves:%d\n",player1.score,player1.turns);
            printf("Time~%02d:%02d\n",(time(NULL)-tm.start)/60,(time(NULL)-tm.start)%60);
            printf("Player 1 Can Choose A Bin:");
            e=choose(c,a);
            if(e==61){
                    Menu(r,c,a);
                    board(r,c,a);}
            else{
            fill(player1.chip,r,c,a,e);
            blue();
            board(r,c,a);
            player1.score=countFours(player1.chip,r,c,a);
            player1.turns++;
            i=2;}
            break;
        case 2:
            yellow();
            printf("(~):Menu\nPlayer 2 Score:%d\nPlayer 2 Moves:%d\n",player2.score,player2.turns);
            printf("Time~%02d:%02d\n",(time(NULL)-tm.start)/60,(time(NULL)-tm.start)%60);
            printf("Player 2 Can Choose A Bin:");
            e=choose(c,a);
            if(e==61){
                    Menu(r,c,a);
                    board(r,c,a);}
            else{
            fill(player2.chip,r,c,a,e);
            blue();
            board(r,c,a);
            player2.score=countFours(player2.chip,r,c,a);
            player2.turns++;
            i=1;}
            break;
    }
    if(full(r,c,a)==0)Turns(i,r,c,a);
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
    for(i=r-1;i>=3;i--){
        for(j=c-1;j>=3;j--){
            f=0;
            for(k=0;k<4;k++)
                if(a[i-k][j-k]==x)
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
void fill(int chip ,int r,int c,char a[][c],int q){
    int i;
    for(i=r-1;i>=0;i--){
        if (a[i][q]==' '){
            a[i][q]= chip;
            break;              }
                       }
}
int choose(int c,char a[][c]){
    char i;
    scanf(" %c",&i);
    i=i-65;
    if((a[0][i]==' ')&&(i<c)&&(i>=0)||(i==61))
        return i;
    else
        choose(c,a);
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
    e=rand()%10;
    while(a[0][e]!=' '&&e<c)e++;
    while(a[0][e]!=' '&&e>=0)e--;
    return e;
}
void Menu(int r,int c,char a[][c]){
    char key;
    system("cls");
    printf("Menu\nPlease,stick to the Given inputs!\nC:Close\nS:Save\nU:undo\nR:Redo\nQ:Exit\n");
    scanf(" %c",&key);
    switch(key){
        case'C':
        case'c':
            break;
        case'S':
        case's':
            printf("Save");
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
            exit(0);
            break;
        default:
            Menu(r,c,a);
}}

