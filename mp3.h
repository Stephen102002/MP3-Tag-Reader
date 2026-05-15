#ifndef MP3_H
#define MP3_H

typedef struct
{
    char id[5];           // Frame ID (e.g., "TIT2")
    int size;             // Size of the frame data
    char flags[3];        // 3-byte flags
    char data[4096];      // Frame data buffer
} Frame;

void view_mp3(char *argv[]);
void convert_be_to_le(char* ptr);
void tag_name(char* tag, char* tagname);
void edit_mp3(int argc,char*argv[]);
void edittag_name(char* tag, char* edittag, char* tagname );
void mp3_help(char* argv[]);

/*void read_tag(song *s,FILE *fptr);
void read_size(song *s,FILE *fptr,int ver);
void read_flags(song *s,FILE *fptr);
void read_data(song *s,FILE *fptr);

void print_any_frame(song *s); */

#endif 