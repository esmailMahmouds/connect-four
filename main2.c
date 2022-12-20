#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
struct p{
    char name;
    int colour;
    int turns;
    int score;
    char chip;
}player1,player2;

void blue(){printf("\033[0;34m");}
void red(){printf("\033[1;31m");}
void yellow(){printf("\033[0;33m");}
void reset(){printf("\033[0m");}
void Menu(int r,int c);
void columnIndex(int c){
    printf(" ");
    for(int i=65;(i<65+c)&&(i<=122);i++)
        printf("%c ",i);
    printf("\n");
}
void bar(int c);
int countFours(char x,int r,int c,char a[][c]);
int Full(int r,int c,char a[][c]);
int choose(int c,char a[][c]);
void fill(int chip ,int r,int c,char a[][c],int q);
void board(int r,int c,char a[][c],int z);
void Turns(int i,int r,int c,char a[][c]);
void gameHuman(int r,int c,char a[][c]);
void gameComputer(int r,int c,char a[][c]);
void Start(int r,int c,char a[][c]);
void MainMenu(int r,int c,char a[][c],int x);
//════════════════════════════════════════════════════════════════════════
int main(){
    int r=7,c=9,x=10;
    char a[r][c];
    player1.colour=1;player1.chip='1';player1.turns=0;player1.score=0;
    player2.colour=2;player2.chip='2';player2.turns=0;player2.score=0;
    for(int i=0;i<r;i++)
        for(int j=0;j<c;j++)
            a[i][j]=' ';
    MainMenu(r,c,a,x);
}



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
    int q;
    system("cls");
    printf("Select GameMode:\n1:Player Vs Player\n2:Player Vs Computer\n");
    scanf("%d",&q);
    switch(q){
        case 1:
            gameHuman(r,c,a);
            break;
        case 2:
            printf("gameComputer");
            break;
        default:
            start(r,c,a);}
}
void gameComputer(int r,int c,char a[][c]){
}
void gameHuman(int r,int c,char a[][c]){
    blue();
    board(r,c,a,1);
    Turns(1,r,c,a);

}
void Turns(int i,int r,int c,char a[][c]){
    switch(i){
        case 1:
            red();
            printf("Player 1 Score:%d\nPlayer 1 Moves:%d\nPlayer 1 Can Choose A Bin:",player1.score,player1.turns);
            fill(player1.chip,r,c,a,choose(c,a));
            blue();
            board(r,c,a,i);
            player1.score=countFours('1',r,c,a);
            player1.turns++;
            i=2;
            break;
        case 2:
            yellow();
            printf("Player 2 Score:%d\nPlayer 2 Moves:%d\nPlayer 2 Can Choose A Bin:",player2.score,player2.turns);
            fill(player2.chip,r,c,a,choose(c,a));
            blue();
            board(r,c,a,i);
            player2.score=countFours('2',r,c,a);
            player2.turns++;
            i=1;
            break;
    }
    if(Full(r,c,a)==0)Turns(i,r,c,a);
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
    return count;
}
int Full(int r,int c,char a[][c]){
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
    if((a[0][i]==' ')&&(i<c)&&(i>=0))
        return i;
    else choose(c,a);
}
void board(int r,int c,char a[][c],int z){
    system("cls");
    columnIndex(c);
    bar(c);
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            blue();
            printf("%c",186);
            if (a[i][j]=='1'){
                    red();
                    printf("%c",'X');}
            else if (a[i][j]=='2'){
                    yellow();
                    printf("%c",'O');}
            else printf("%c",' ');
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
void Menu(int r,int c){
    char key;
    system("cls");
    printf("Menu\nPlease,stick to the Given inputs!\nC:Close\nS:Save\nU:undo\nR:Redo\nQ:Exit\n");
    scanf(" %c",&key);
    switch(key){
        case'C':
            break;
        case'S':
            printf("Save");
            break;
        case'U':
            printf("Undo");
            Menu(r,c);
            break;
         case'R':
            printf("Redo");
            Menu;
            break;
        case'Q':
            exit(0);
            break;
        default:
            Menu(r,c);
}}
