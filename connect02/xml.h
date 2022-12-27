#ifndef XML_H_INCLUDED
#define XML_H_INCLUDED
void read_xml(int parameters[]);

int  remove_spaces(int n,char str[]);

void substring(char str[] ,char sub_str[],int index1,int index2);

int get_start(char str[],int length,char config[]);

int get_end(char str[],int length,char config[]);

int number(int index1,int index2,char config[]);

void set_defaults(int parameters[]);

void check_error(int error,int parameters[]);
#endif // XML_H_INCLUDED
