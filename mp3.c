#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "mp3.h"

void view_mp3(char* argv[])
{
    printf("View Selected\n");
    FILE* fp = NULL;
    fp = fopen(argv[2], "r");
    if(fp == NULL)
    {
        printf("file not found\n");  //if file is not found
       
        exit(0);
    }

    char buffer[4];  //buffer string for storing the data
    fread(buffer, 3, 1, fp);
    buffer[3] = '\0';
    if(!(strcmp(buffer, "ID3")))  //to check if it is mp3 file
    {
        printf("%s is an MP3 file\n", argv[2]);
    }
    else
    {
        printf("%s is not an MP3 file\n", argv[2]);                                                                                                                                                                                                                                                                                
        exit(0);
    }

    fseek(fp, 7, SEEK_CUR);  //Now fp at 10th index or with 11th byte.
    printf("------------------------------------------------------------------------\n");
    printf("\tMP3 Tag Reader and Editor for ID3v2\n");
    printf("------------------------------------------------------------------------\n");
    for(int i = 0; i < 6; i++)
    {
        //To get size of the content bytes
        char tag[5];
        fread(tag, 4, 1, fp);
        tag[4] = '\0';
        int size; //to store size
        fread(&size, sizeof(int), 1, fp); 

        //to convert size to big endian.
        char* ptr = (char*)&size;  // since size is integer we shoud type cast address to character address
        convert_be_to_le(ptr);   //function call for conversion.

        fseek(fp, 3, SEEK_CUR);  //skip 3 bytes

        char content[size]; //content string
        fread(content, size - 1, 1, fp);  //read content
        content[size-1] = '\0';

        //to find tag name
        char tagname[20];
        tag_name(tag, tagname);  //function call for tag name
        printf("%-10s :\t%-50s\n", tagname, content);  //for displaying tagname and content
    }
    printf("------------------------------------------------------------------------\n");
    fclose(fp);
}

void convert_be_to_le(char* ptr)  //function definition for big endian to little endiness conversion
{
    for(int i = 0; i < 2; i++)
    {
        char tmp = ptr[i];
        ptr[i] = ptr[3 - i];
        ptr[3 - i] = tmp;
    }
}

void tag_name(char* tag, char* tagname )  //functing definition for finding tagname to print
{
    char TAGS[6][5] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    char TAGSNAME[6][20] = {"Title", "Artist", "Album", "Year", "Music", "Comment"};
    for(int i = 0; i < 6; i++)
    {
        if(!(strcmp(tag, TAGS[i]))) 
        {
            strcpy(tagname, TAGSNAME[i]);
            break;
        }
    }
}
void mp3_help(char* argv[])  //function defintion for help menu.
{
    printf("\n\t\t>>>>>Help menu<<<<<\n");
    printf("+--------+------------------------------------------------------+\n");
    printf("|Options |\tSyntax\t\t\t\t\t\t|\n");
    printf("+--------+------------------------------------------------------+\n");
    printf("|1. View |\t./a.out -v <filename.mp3>\t\t\t|\n");
    printf("|2. Edit |\t./a.out -e -edit_tag new_one <filename.mp3>\t|\n");
    printf("|3. Help |\t./a.out --help\t\t\t\t\t|\n");
    printf("+--------+------------------------------------------------------+\n");
    printf("+-------------------------------+\n");
    printf("|     >>>>>Tags<<<<<\t\t|\n");
    printf("+-------------------------------+\n");
    printf("|-t : Modifies a title tag\t|\n|-a : Modifies a Artist tag\t|\n|-A : Modifies a Album tag\t|\n|-y : Modifies a Year tag\t|\n|-C : Modifies a Content tag\t|\n|-c : Modifies a Comment tag\t|\n");
    printf("+-------------------------------+\n");
}
