#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
void DrawGrid(int r,int c,char grid[][c])
{
    for(int j=0; j<c; j++)
    {
        printf(" %c   ",65+j); // letter of each column
    }
    printf("\n");
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {
            printf(" %c ||",grid[i][j]);
        }
        printf("\n");
        for(int k=0; k<c; k++)
        {
            printf("=====");
        }
        printf("\n");
    }
}
void empty(int r,int c,char grid[r][c])
{
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {
            grid[i][j]=' '; //set empty grid
        }
    }
}
void blue()
{
    printf("\033[0;34m");
}
void red()
{
    printf("\033[1;31m");
}
void yellow()
{
    printf("\033[0;33m");
}
void reset()
{
    printf("\033[0m");
}
void save(int r,int c,char grid[][c])
{
    FILE *s=NULL;
    s=fopen("save.bin","wb");
    if(s==NULL)
    {
        printf("something went wrong");
        //return -1;
    }
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {
            fwrite(&grid[i][j],sizeof(char),1,s);
        }
    }
    fclose(s);
}
void load(int r,int c,char grid[][c],char x[][c])
{
    FILE *s=NULL;
    s=fopen("save.bin","rb");
    if(s==NULL)
    {
        printf("something went wrong");
        //return -1;
    }
    //char x[r][c];
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
        {
            fread(&x[i][j],sizeof(char),1,s);
        }
    }
    fclose(s);
}
int main()
{

    int r,c;
    char key;
    //char filename[FILENAME_MAX];
    scanf("%d%d",&r,&c);
    system("cls");
    char grid[r][c];
    char x[r][c];
    empty(r,c,x);
    empty(r,c,grid);
    DrawGrid(r,c,grid);
    /**start game
    with emptygrid**/
    int k;
    for(int i=0; i<3; i++)
    {
        printf("enter column ");
        scanf("%d",&k);
        grid[r-1][k]='o';     /**taking sample input**/
        system("cls");
        DrawGrid(r,c,grid);
        printf("Do you want to save at this piont? (Y:YES/N:NO) \n");
        scanf(" %c",&key);
        switch(key)
        {
        case 'Y':
        case 'y':
            save(r,c,grid);
            break;
        case 'N':
        case 'n':
            break;
        default:
            printf("INVALID CHOICE");
        }
    }
    load(r,c,grid,x);
   char key1;
    printf("Do you want to load a game? (Y:YES/N:NO)\n");
    scanf("%c",&key1);
    switch(key1)
        {
        case 'Y':
        case 'y':
            load(r,c,grid,x);
            break;
        case 'N':
        case 'n':
            break;
        default:
            printf("INVALID CHOICE");
        }
        system("cls");
    printf("saved version\n");
    DrawGrid(r,c,x);
    //fgets(filename,FILENAME_MAX,stdin);

//    FILE *s=NULL;
//    s=fopen("save.bin","wb");
//    if(s==NULL)
//    {
//        printf("something went wrong");
//        return -1;
//    }
//    for(int i=0; i<r; i++)
//    {
//        for(int j=0; j<c; j++)
//        {
//            fwrite(&grid[i][j],sizeof(char),1,s);
//        }
//    }
//    fclose(s);

//    s=fopen("save.bin","rb");
//    if(s==NULL)
//    {
//        printf("something went wrong");
//        return -1;
//    }
//    char x[r][c];
//    for(int i=0; i<r; i++)
//    {
//        for(int j=0; j<c; j++)
//        {
//            fread(&x[i][j],sizeof(char),1,s);
//        }
//    }
//    fclose(s);


//    FILE *p=NULL;
//     p=fopen("name.bin","wb");
//    if(p==NULL)
//    {
//        return -1;
//    }
//    int x;
//    for(int i=5;i<10;i++)
//    {
//        fwrite(&i,sizeof(int),1,p);
//    }
//    fclose(p);
//    p=fopen("name.bin","rb");
//    if(p==NULL)
//    {
//        return -1;
//    }
//    for(int i=0;i<5;i++)
//    {
//        fread(&x,sizeof(int),1,p);
//        printf("x: %d\n",x);
//    }
//    fclose(p);

//    char name[30];
//    int age;
//    rewind(p);
//    fscanf(p,"%s %d",name,&age);
//    for(int i=0;i<100;i++)
//    {
//        fprintf(p,"%d\n",i);
//    }

//    printf("name=%s\n age :%d",name,age);
    return 0;
}
