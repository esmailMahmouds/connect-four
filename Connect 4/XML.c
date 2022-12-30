#include <stdio.h>
#include <string.h>
#include <ctype.h>
void read_xml(int parameters[])
{
    static int error=0;

    int j=0; //counter for the parameters array//
    int index1,index2;
    char filename[FILENAME_MAX];
    if(error==0)
    {
        strcpy(filename,"config.XML");
    }
    else
    {
        printf("Enter the path of the file:\n");
        gets(filename);
    }
    FILE *p=NULL;
    p=fopen(filename,"r");
    if(p==NULL)
    {
         error++;
         printf("Configuration File not Found...");
         check_error(error,parameters);
         return;
    }

    int length=1;
    while(fgetc(p)!=EOF)
    {
        length++;       /**count character of the file**/
    }
    rewind(p);

    char text[length];
    /**store file characters in string**/
    for(int i=0; i<length-1; i++)
    {
        text[i]=fgetc(p);
    }
    text[length-1]='\0';
    /** check for configration tag**/
    index1=get_start("<Configurations>",length,text);
    index2=get_end("</Configurations>",length,text);
    if(index1 == -1 || index2 ==-1 || index1>index2)
    {
        error++;
        printf("File is corrupted...");
        check_error(error,parameters);
        return;
    }
    else
    {
        length=index2-index1;
        char config[length+1];             /**sub string for text between configration tag**/
        substring(text,config,index1,index2);
        config[length]='\0';
        char list[][14]={"<Height>","</Height>","<Width>","</Width>","<Highscores>","</Highscores>"};

        for(int i=0;i<6;i+=2)
        {
            index1=get_start(list[i],length,config);
            index2=get_end(list[i+1],length,config);
            if(index1 ==-1 || index2 ==-1 || index1>index2)
            {
                   error++;
                   printf("File is corrupted...");
                   check_error(error,parameters);
                   return;
            }
            else
            {
                parameters[j++]=number(index1,index2,config);
                if(parameters[j-1]==0)
                {
                    error++;
                    printf("missing value...");
                    check_error(error,parameters);
                    return;
                }
            }
        }
    }
}
/**function remove spaces from string**/
int remove_spaces(int n,char str[])
{
    for(int i=0; i<n; i++)
    {
        if(isspace(str[i]))
        {
            for(int j=i; j<n; j++)
            {
                str[j]=str[j+1];
            }
            n--;
            i--;
        }
    }
    return n;
}
/**functions return index between the two tags from string**/
int get_start(char str[],int length,char config[])
{
    int size=(int)strlen(str)+1;
    char sub[size];
    int index1=-1;
    for(int i=0; i<length-1; i++)
    {
        int found=0;
        int count=0;
        int j=i;
        for(j; j<i+size-1; j++)
        {
            sub[count++]=config[j];
        }
        sub[size-1]='\0';
        if(strncmp(sub,str,size) == 0)
        {
            index1=j;
        }
    }
    return index1;
}
int get_end(char str[],int length,char config[])
{
    int size=(int)strlen(str)+1;
    char sub[size];
    int index2=-1;
    for(int i=0; i<length-1; i++)
    {
        int count=0;
        int j=i;
        for(j; j<i+size-1; j++)
        {
            sub[count++]=config[j];
        }
        sub[size-1]='\0';
        if(strncmp(sub,str,size) == 0)
        {
            index2=i;
        }

    }
    return index2;
}
/**function return the numder between two index**/
int number(int index1,int index2,char config[])
{
    int r=0;
    int count =0;
    int size=index2-index1;
    char num[size];
    for(int k=index1; k<index2; k++)
    {
        if( isdigit(config[k]) || isspace(config[k]) )
           num[count++]=config[k];
    }
     remove_spaces(size,num);
    r=atoi(num);
    return r;
}
/**function make substring**/
void substring(char str[] ,char sub_str[] ,int index1,int index2)
{
    int count=0;
    for(int i=index1;i<index2;i++)
    {
        sub_str[count++]=str[i];
    }
}
/**setting parameter to defaults values*/
void set_defaults(int parameters[])
{
    parameters[0]=7;
    parameters[1]=9;
    parameters[2]=10;
    system("cls");
    printf("Couldn't find the file for 3 consecutive times\nParameters are set to Default Height=7,Width=9,Highscore=10\n");
    getch();
}
void check_error(int error,int parameters[])
{
    if(error<4)
    {
        read_xml(parameters);
    }
    else
    {
        set_defaults(parameters);
    }
}
