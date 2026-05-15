#include<string.h>
#include<stdio.h>
#include "mp3.h"

int main(int argc,char *argv[])
{
    
    if(argc<2)
    {
        printf("Insufficient arguments\n");
    }
    else if(argc==3 && strcmp(argv[1],"-v")==0)
    {
        view_mp3(argv);
    }
    else if(argc==5 && strcmp(argv[1],"-e")==0)
    {
        edit_mp3(argc,argv);
    }
   else if(strcmp(argv[1],"-h")==0)
    {
        mp3_help(argv);
    }
    else
    {
        printf("Invalid arguements.....TRY AGAIN.......\n");
        return 0;
    }
     return 0;
}