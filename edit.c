#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "mp3.h"

void read_frame(Frame *f, FILE *fp) //read id,size,flags,data from file
{
    fread(f->id, 4, 1, fp); //read tag id
    f->id[4] = '\0';

    fread(&f->size, 4, 1, fp); // read size
    convert_be_to_le((char *)&f->size); //convert size to little endian

    fread(f->flags, 3, 1, fp); //read flags
    f->flags[2] = '\0';

    fread(f->data, f->size - 1, 1, fp); //read data
    f->data[f->size - 1] = '\0';
}

void write_frame(Frame *f, FILE *fp) //write id,size,flags,data to file
{
    fwrite(f->id, 4, 1, fp); //write tag id 

    int size = f->size;  
    convert_be_to_le((char *)&size);
    fwrite(&size, 4, 1, fp);

    fwrite(f->flags, 3, 1, fp);
    fwrite(f->data, f->size - 1, 1, fp);
}

void edit_mp3(int argc, char *argv[])
{
    FILE *fp1 = fopen(argv[4], "r"); //open original file
    if (!fp1)
     {
         printf("File not found\n");
          exit(0); 
    }
    char edit_id[5], edit_name[20];  //to store edit tad id and name
    edittag_name(argv[2], edit_id, edit_name);

    char buffer[4];  //buffer for checking mp3 file
    fread(buffer, 3, 1, fp1); // read first 3 bytes for mp3 check
    buffer[3] = '\0';
    if (strcmp(buffer, "ID3") != 0) //check if it is mp3 file
    {
        printf("%s is not an MP3 file\n", argv[4]);
        exit(0);
    }

    FILE *fp2 = fopen("temp.mp3", "w"); //open temporary file for writing
    fwrite(buffer, 3, 1, fp2); //write "ID3" TO temp file

    char header[7];  //to store header bytes
    fread(header, 7, 1, fp1); //read header from original file
    fwrite(header, 7, 1, fp2); //write header to temp file

    printf("Selected edit option is %s : %s\n", edit_name, argv[3]); //display selected edit option

    for (int i = 0; i < 6; i++)
    {
        Frame f; //define frame structure

        // ---- Read old frame ----
        read_frame(&f, fp1); //read frame from original file

        // ---- If this is the frame to modify ----
        if (strcmp(f.id, edit_id) == 0) //check if it is the frame to be edited
        {
            int newlen = strlen(argv[3]) + 1; //new length of data

            f.size = newlen;
            strcpy(f.data, argv[3]);

            write_frame(&f, fp2);

            // Copy remaining MP3 file
            char ch;
            while (fread(&ch, 1, 1, fp1) == 1) //read byte by byte
                fwrite(&ch, 1, 1, fp2);

            fclose(fp1);
            fclose(fp2);
            remove(argv[4]);
            rename("temp.mp3", argv[4]);

            printf("--------- %s Updated Successfully ---------\n", edit_name);
            exit(0);
        }
        else
        {
            // Write frame as-is
            write_frame(&f, fp2);
        }
    }
}

void edittag_name(char *tag, char *edittag, char *tagname)
{
    char TAGID[6][3] = {"-t", "-a", "-A", "-y", "-C", "-c"};
    char TAGS[6][5]  = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    char NAMES[6][20]={"Title","Artist","Album","Year","Music","Comment"};
    int flag=0;
    for(int i=0; i<6; i++)
    {
        if(strcmp(tag, TAGID[i]) == 0)
        {
            strcpy(edittag, TAGS[i]);
            strcpy(tagname, NAMES[i]);
            return;
        }
        else
        {
            flag=1;
        }
    }
    if(flag==1)
    {
        printf("Invalid edit option %s\n",tag);
        exit(0);
    }
}
