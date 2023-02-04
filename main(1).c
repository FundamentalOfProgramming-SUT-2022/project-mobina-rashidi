#include <stdio.h>

#include <string.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <stdlib.h>

#include <stdbool.h>

#include <unistd.h>

#include <stdio.h>

#include <sys/stat.h>

#include <stdbool.h>

#include <dirent.h>

#include<unistd.h>



char * copy_str(char * file_name,int line_no,int start_pos,int size,char * direction){

    char line[1000][1000];
	int k = 0 ;
    FILE *fptr = NULL; char * res = (char *) malloc(size) ;
        int i = 0; int end_pos ;
    int tot;

printf("%s\n%d %d %d\n%s\n" , file_name , line_no , start_pos , size , direction ) ;

    fptr = fopen(file_name, "r");
    while(fgets(line[i], 1000, fptr))
	{
        line[i][strlen(line[i]) - 1] = '\0';
        i++;
    }
    tot = i;

    for(i = 0; i < tot; ++i)
    {
        if(i+1==line_no){
        //printf("%s\n", line[i]);
         if(strcmp(direction,"--f") == 0){

            int end_pos = start_pos+size ;
        for(int j = start_pos ; j <end_pos ; j++){
            res[k] = line[i][j] ;
            ++k ;
        }
        }

         if(strcmp(direction,"--b") == 0){

             start_pos -= size ;
             end_pos = size + start_pos ;

            for(int j = start_pos ; j<end_pos  ; j++){

                res[k] = line[i][j] ;

                ++k ;
            }
         }
}

    }



    fclose(fptr);
    strcat(res , "\0" ) ;

  printf("%s\n" , res);



    return res ;
}







void remove_quote(char * str){
    if(str[0] == '"'){
        memcpy(str, str + 1, strlen(str)-1);
        str [strlen(str) - 2] = 0;
    }
}
char * cut_str(char * file_name,int line_no,int start_pos,int size,char * direction){
    //cutstr –-file 200.txt –-pos 2:1 --size 3 --b

    char * cut = (char *) malloc(size);
    int cut_index = 0;

    char file_name_tmp[1024];
    strcpy(file_name_tmp, file_name);
    strcat(file_name_tmp, ".tmp");

    FILE * fp = fopen(file_name, "r");
    FILE * fp_write = fopen(file_name_tmp, "w+");
    if (fp == NULL || fp_write == NULL) {
        printf("Warning");
    }else{

        char buff [1024];
        char buff_ch;
        //go to line no:
        int _curr_line = 0;


        if(strcmp(direction, "--f") == 0){
            //forward
            // nothing to be done
        }else{
            //backward
            start_pos -= size; // its backward
            if(start_pos < 0) start_pos = 0;
        }

        int result;
        int line_counter = 0;

        while(result = fscanf(fp, "%[^\n]\n", buff) != -1){
            line_counter ++;
            strcat(buff, "\n"); //append new line

            int pos=0;
            for(int i=1;i<=strlen(buff);i++){



                if(line_counter >= line_no){




                    if(line_counter == line_no && pos++ < start_pos){
                        fputc(buff[i-1], fp_write);
                    }else{
                        if(size > 0){
                            //skip write
                            size -- ;
                            cut[cut_index++] = buff[i-1];
                        }else{
                            fputc(buff[i-1], fp_write);
                        }
                    }

                }else{
                    fputc(buff[i-1], fp_write);
                }
            }
        }

    }

    fclose(fp);
    fclose(fp_write);

    remove(file_name);
    rename(file_name_tmp, file_name);


    return cut;
}

void copy(char * file_src, char * file_dst){
    FILE * fp = fopen(file_src, "r");
    FILE * fp_write = fopen(file_dst, "w+");
    if (fp == NULL || fp_write == NULL) {
        printf("Warning");
    }else{
        char ch;
        while ((ch = fgetc(fp)) != EOF)
            fputc(ch, fp_write);
    }
    fclose(fp);
    fclose(fp_write);
}

void format(char * file_name){
    format_step_1(file_name);
    format_step_2(file_name);
}
void format_step_2(char * file_name){
    char file_tmp [1024];

    strcpy(file_tmp, file_name);
    strcat(file_tmp, ".tmp");
    FILE * f = fopen(file_name,"r");
    FILE * f_w = fopen(file_tmp,"w+");


    char buffer [8000];


    int ident = 0;
    do{
        int result = fscanf(f, "%[^\n]\n", buffer);

        if(result > 0){

            if(buffer[0] == '}'){
                for(int j=1;j<=ident - 1;j++){
                    fprintf(f_w, "\t");
                }
                fprintf(f_w, "%s\n", buffer);
            }else{
                for(int j=1;j<=ident;j++){
                    fprintf(f_w, "\t");
                }
                fprintf(f_w, "%s\n", buffer);
            }

            if(buffer[strlen(buffer) - 1] == '{'){
                ident ++;
            }
            if(buffer[0] == '}'){
                ident --;
            }
        }else{
            break;
        }
    }while(1);


    fclose(f);
    fclose(f_w);


    remove(file_name);
    rename(file_tmp, file_name);
}
void format_step_1(char * file_name){
    char file_tmp [1024];

    strcpy(file_tmp, file_name);
    strcat(file_tmp, ".tmp");
    FILE * f = fopen(file_name,"r");
    FILE * f_w = fopen(file_tmp,"w+");


    char buffer [8000];
    fscanf(f, "%[^\n]s", buffer);

    for(int i=0;i < strlen(buffer);i++){
        char ch = buffer[i];

        bool is_next_open = false;
        int n=1;
        do{

            if(i + n < strlen(buffer)){
                char next = buffer[i + n];
                if(next == ' ' || next == '\t' || next == '\n'){
                    //continue
                }else if(next == '{'){
                    is_next_open = true;
                    break;
                }else{
                    break; // it's a character not space and {
                }
            }else{
                break;
            }
            n++;
        }while(1);

        if(is_next_open){


            if(ch == '{'){
                fprintf(f_w, "{\n"); // between two {{ we need new line
            }else{
                fprintf(f_w,"%c ", ch);
            }

            i += n - 1; // skip spaces
            continue;
        }

        fprintf(f_w,"%c", ch);
        if(ch == '{'){
            fprintf(f_w,"\n");

            int n=1;
            do{
                if(i + n < strlen(buffer)){
                    char next = buffer[i + n];
                    if(next == ' ' || next == '\t' || next == '\n'){
                        i ++; //skip next
                    }else{
                        break;
                    }
                }
                n++;
            }while(1);
            continue;
        }

        if(i + 1 < strlen(buffer)){
            char next = buffer[i + 1];
            if(next == '}'){
                fprintf(f_w,"\n");
            }
        }
    }


    fclose(f);
    fclose(f_w);

    remove(file_name);
    rename(file_tmp, file_name);
}



void tree(char * dir,int depth, int max_depth){

    char buffer_space [1024];

    strcpy(buffer_space, "");
    for(int i=1;i<=(depth+1)*2;i++){
        strcat(buffer_space, "-");
    }

    if(max_depth <= -2){
        printf("Warning parameter cant be <= -2\n", depth);
        return;
    }
    if(depth == 0){
        //printf("tree %d\n", depth);
        printf("root\n");
    }
    if(max_depth == 0){
        return;
    }

    DIR *dp;
    struct stat filestat;
    struct dirent *ep;
    dp = opendir (dir);
    if (dp != NULL)
    {
        while ((ep = readdir (dp)) != NULL)

        {
            if(strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0)continue;



            stat(ep->d_name,&filestat);

            char buff [1024];
            strcpy(buff, dir);
            strcat(buff, "/");
            strcat(buff, ep->d_name);

            DIR *tmp = opendir(buff);

            if(tmp != NULL){
                //printf("%s\n", ep->d_name);
                closedir (tmp);

                printf(buffer_space);
                printf("|%s:\n", ep->d_name);
                if(max_depth > depth + 1 || max_depth == -1)
                    tree(buff, depth + 1, max_depth);
            }else{
                printf(buffer_space);
                printf("%s\n", ep->d_name);
            }



        }

        closedir (dp);
    }
}



int main()
{
    int j = 0;
    int i = 0;
    int value, sum, insert_txt, num, cat1;
    char text[10000];
    char cat_f[1000];
    char add[1000];
    char firstcom[1000];
    char adress[1000];
    int creat = 0;
    int insert = 0;
    char create[] = "createfile";
    char file[] = "--file";
    char insert1[] = "insertstr";
    char str[] = "--str";
    char cat[] = "cat";






    char * clipboard = NULL ;
    char * undo_info_command [1024];
    char * undo_info_str [1024];
    int undo_info_line_no;
    int undo_info_pos;



    do{
        char command[1000] = "";
        printf("\n");
        printf("-------------------");
        printf("\nEnter Command:\n");
        printf("-------------------");
        printf("\n");
        gets(command); //get command from console

        char * items[10]; //maximum 10 params
        for(int i=0;i<10;i++){
            items[i] = (char *) malloc(1024);
        }

        int command_size = strlen(command);

        if(command_size > 0){
            char * ref = command;
            int i = 0;
            int item_index = 0;
            do{
                char it[100]="";

                sscanf(ref + i, "%s", it);
                int it_length = strlen(it);

                // if we reach a string with start of quote and not end with quote  like { "hello }
                //
                if(it[0] == '"' && it[strlen(it) - 1] != '"'){
                    char tmp[100];

                    do{
                        int a,b;
                        a = i + it_length;

                        sscanf(ref + i + it_length + 1, "%s", tmp);
                        char space[2] = " ";
                        strcat(it, space); //add space
                        strcat(it, tmp); // add string

                        it_length += strlen(tmp) + 1;
                        b = it_length;
                    }while(tmp[strlen(tmp) - 1] != '"'); // if we don't get to the quote we need to keep trying until we get one

                }





                char item[1024];
                strcpy(item, it);
                remove_quote(item); //remove any quote from start/end
                strcpy(items[item_index++], item);


                i += it_length + 1;
                if(i > command_size) break;

            }while(true);


            //now we have parameters


            //action [createfile]:
            if (strcmp(items[0], "createfile") == 0){
                //now get next params:
                if(item_index == 3){
                    if(strcmp(items[1], "--file") == 0){
                        createfile(items[2]);
                    }else{
                        printf("Invalid Command");
                    }
                }else{
                    printf("Invalid Command");
                }
            }

            //action [cat]:
            if (strcmp(items[0], "cat") == 0){
                //now get next params:
                if(item_index == 3){
                    if(strcmp(items[1], "--file") == 0){
                        cattextfile(items[2]);
                    }else{
                        printf("Invalid Command");
                    }
                }else{
                    printf("Invalid Command");
                }
            }




        //action [insertstr]:
        // insertstr –-file 200.txt –-str "Salam\nXBOX" -–pos 2:5
        if (strcmp(items[0], "insertstr") == 0){
            //now get next params:
            if(strcmp(items[1], "--file") == 0){
                if(strcmp(items[3], "--str") == 0){
                    if(strcmp(items[5], "--pos") == 0){
                        char * param6 = items[6]; //<line no>:<start position>
                        int line_no;
                        int start_pos;

                        //split line_no:start_pos :
                        sscanf(param6, "%d:%d", &line_no, &start_pos);

                        char * filename = items[2];
                        char * str = items[4];
                        insert_str(filename, str, line_no, start_pos);

                        // undo info :
                        strcpy(undo_info_str, str);
                        strcpy(undo_info_command, items[0]);
                        undo_info_pos = start_pos;
                        undo_info_line_no = line_no;

                    }
                }
            }
        }


        //removestr –-file 200.txt –-pos 2:1 --size 3 --f
        if (strcmp(items[0], "removestr") == 0){
            //now get next params:
            if(strcmp(items[1], "--file") == 0){
                if(strcmp(items[3], "--pos") == 0){
                    char * param6 = items[4]; //<line no>:<start position>
                    int line_no;
                    int start_pos;
                    //split line_no:start_pos :
                    sscanf(param6, "%d:%d", &line_no, &start_pos);

                    if(strcmp(items[5], "--size") == 0){
                        int size;
                        sscanf(items[6], "%d", &size);

                        if(strcmp(items[7], "--f") == 0 || strcmp(items[7], "--b") == 0){
                            char * filename = items[2];
                            char * direction = items[7]; // --f or --b
                            char * cut = cut_str(filename, line_no, start_pos, size, direction);

                            // undo info :
                            strcpy(undo_info_str, cut);
                            strcpy(undo_info_command, items[0]);
                            undo_info_pos = start_pos;
                            undo_info_line_no = line_no;
                        }
                    }
                }

            }
        }



                if (strcmp(items[0], "copystr") == 0){
            //now get next params:
            if(strcmp(items[1], "--file") == 0){
                if(strcmp(items[3], "--pos") == 0){
                    char * param6 = items[4]; //<line no>:<start position>
                    int line_no;
                    int start_pos;
                    //split line_no:start_pos :
                    sscanf(param6, "%d:%d", &line_no, &start_pos);

                    if(strcmp(items[5], "--size") == 0){
                        int size;
                        sscanf(items[6], "%d", &size);

                        if(strcmp(items[7], "--f") == 0 || strcmp(items[7], "--b") == 0){
                            char * filename = items[2];
                            char * direction = items[7]; // --f or --b
         clipboard =copy_str(filename, line_no, start_pos, size, direction) ;
                            printf("string is in clipboard! [%s]", clipboard);


                        }
                    }
                }

            }
        }













        //cutstr –-file 200.txt –-pos 2:1 --size 3 --f
        if (strcmp(items[0], "cutstr") == 0){
            //now get next params:
            if(strcmp(items[1], "--file") == 0){
                if(strcmp(items[3], "--pos") == 0){
                    char * param6 = items[4]; //<line no>:<start position>
                    int line_no;
                    int start_pos;
                    //split line_no:start_pos :
                    sscanf(param6, "%d:%d", &line_no, &start_pos);

                    if(strcmp(items[5], "--size") == 0){
                        int size;
                        sscanf(items[6], "%d", &size);

                        if(strcmp(items[7], "--f") == 0 || strcmp(items[7], "--b") == 0){
                            char * filename = items[2];
                            char * direction = items[7]; // --f or --b
                            clipboard = cut_str(filename, line_no, start_pos, size, direction);
                            printf("Cut string is in clipboard! [%s]", clipboard);

                            strcpy(undo_info_str, clipboard);
                            strcpy(undo_info_command, "removestr"); // cut is remove
                            undo_info_pos = start_pos;
                            undo_info_line_no = line_no;
                        }
                    }
                }

            }
        }

        //pastestr –-file 200.txt –-pos 2:1
        if (strcmp(items[0], "pastestr") == 0){
            //now get next params:
            if(strcmp(items[1], "--file") == 0){
                if(strcmp(items[3], "--pos") == 0){
                    char * param6 = items[4]; //<line no>:<start position>
                    int line_no;
                    int start_pos;
                    //split line_no:start_pos :
                    sscanf(param6, "%d:%d", &line_no, &start_pos);

                    char * filename = items[2];
                    paste_str(filename, line_no, start_pos, clipboard);
                    printf("Done");
                    //clipboard = NULL;

                    // undo info :
                    strcpy(undo_info_str, clipboard);
                    strcpy(undo_info_command, "insertstr");//paste is insert
                    undo_info_pos = start_pos;
                    undo_info_line_no = line_no;
                }

            }
        }



        //removestr –-file 200.txt –-pos 2:1 --size 3 --f
        //insertstr –-file 200.txt –-str "Salam" -–pos 2:5
        //undo –-file 200.txt
        if (strcmp(items[0], "undo") == 0){
            //now get next params:
            if(strcmp(items[1], "--file") == 0){
                char * file_name = items[2];

                if (strcmp(undo_info_command, "insertstr") == 0){
                    //we need to removestr :
                    remove_str(file_name, undo_info_line_no, undo_info_pos, strlen(undo_info_str), "--f");

                    // undo info :
                    strcpy(undo_info_command, "removestr");
                }else if (strcmp(undo_info_command, "removestr") == 0){
                    //we need to removestr :
                    insert_str(file_name,undo_info_str, undo_info_line_no, undo_info_pos);

                    // undo info :
                    strcpy(undo_info_command, "insertstr");
                }

            }
        }


        if (strcmp(items[0], "auto-indent") == 0){
            format(items[1]);
        }

        if (strcmp(items[0], "compare") == 0){
            compare(items[1], items[2]);
        }


        if (strcmp(items[0], "tree") == 0){
            char path[1024] = "c:/Users/mobin/Documents/root";
            int max_depth = 0;
            sscanf(items[1], "%d", &max_depth);
            tree(path, 0,max_depth);
        }





        else
            printf("invalid command");



        }//keep get commands from console




    }while(true);





}



void createfile(char arr[100]) {



   int n;
   int j = 0;
   int count = 0;
   char add[1000];
   int number = 0;
   int k = strlen(arr);
   char address[1000];
   for (int i = 0; i <= k; i++) {
      address[i] = arr[i];
   }
   char * result = address;
   char p[1000];
   int t = 0;

   k = k - 2;

   for (int i = 0; i < 1000; i++)
      add[i] = 0;

   for (int i = 0; result[i]; i++) {
      if (result[i] == '/') {
         count++;
      }
   }

   while (number < count) {

      while (result[j] != '/') {
         add[j] = result[j];
         ++j;
      }
      mkdir(add);
      char tmp[] = "/";
      strcat(add, tmp);
      ++j;

      number++;
   }

   filecheck(result);
   FILE * f = fopen(result, "w+");
   fclose(f);
}

void filecheck(const char * filename) {

   if (!access(filename, F_OK)) {
      printf("The File %s\t was Found\n", filename);
   }
}

void cattextfile(char arr[1000]) {
   char address[1000];
   int k = strlen(arr);
   const int sz = 255;
   char str[sz];
   FILE * fp;
   for (int i = 0; i <= k; i++) {
      address[i] = arr[i];
   }
   char * result = address;

   fp = fopen(result, "r");
   if (fp == NULL) {
      printf("warning");
   }
   while (fgets(str, sz, fp) != NULL) {
      printf("%s", str);
   };

   fclose(fp);
}

void paste_str(char * file_name, int line_no,int start_pos, char * clipboard){
    insert_str(file_name, clipboard, line_no, start_pos);
}

void remove_str(char * file_name,int line_no,int start_pos,int size,char * direction){

    char file_name_tmp[1024];
    strcpy(file_name_tmp, file_name);
    strcat(file_name_tmp, ".tmp");

    FILE * fp = fopen(file_name, "r");
    FILE * fp_write = fopen(file_name_tmp, "w+");
    if (fp == NULL || fp_write == NULL) {
        printf("Warning");
    }else{

        char buff [1024];
        char buff_ch;
        //go to line no:
        int _curr_line = 0;


        if(strcmp(direction, "--f") == 0){
            //forward
            // nothing to be done
        }else{
            //backward
            start_pos -= size; // its backward
            if(start_pos < 0) start_pos = 0;
        }

        int result;
        int line_counter = 0;

        while(result = fscanf(fp, "%[^\n]\n", buff) != -1){
            line_counter ++;
            strcat(buff, "\n"); //append new line

            int pos=0;
            for(int i=1;i<=strlen(buff);i++){



                if(line_counter >= line_no){




                    if(line_counter == line_no && pos++ < start_pos){
                        fputc(buff[i-1], fp_write);
                    }else{
                        if(size > 0){
                            //skip write
                            size -- ;
                        }else{
                            fputc(buff[i-1], fp_write);
                        }
                    }

                }else{
                    fputc(buff[i-1], fp_write);
                }
            }
        }

    }

    fclose(fp);
    fclose(fp_write);

    remove(file_name);
    rename(file_name_tmp, file_name);
}

void insert_str(char * file_name, char * str, int line_no, int start_pos){

    char file_name_tmp[1024];
    strcpy(file_name_tmp, file_name);
    strcat(file_name_tmp, ".tmp");

    FILE * fp = fopen(file_name, "r");
    FILE * fp_write = fopen(file_name_tmp, "w+");
    if (fp == NULL || fp_write == NULL) {
        printf("Warning");
    }else{

        char buff [1024];
        char buff_ch;
        //go to line no:
        int _curr_line = 0;




        int result;
        int line_counter = 0;
        while(result = fscanf(fp, "%[^\n]\n", buff) != -1){
            line_counter ++;
            if(line_counter == line_no){
                for(int i=1;i<=strlen(buff);i++){
                    if(i - 1 == start_pos) {
                        for(int j=0;j<strlen(str);j++){
                            char ch = str[j];
                            if(j+1<strlen(str)){
                                char next_ch = str[j+1];
                                if(ch == '\\' && next_ch == '\\'){
                                    fputc(ch, fp_write);
                                    j++; //skip the next
                                }else if(ch == '\\' && next_ch == 'n'){ // \n
                                    fputs("\n", fp_write);
                                    j++; //skip the next
                                }else{
                                    fputc(ch, fp_write); //if no special chars
                                }

                            }else{
                                fputc(ch, fp_write);
                            }

                        }
                    }
                    fputc(buff[i-1], fp_write);
                }
                fputc('\n', fp_write);
            }else{
                fprintf(fp_write, "%s\n", buff);
            }

        }

    }

    fclose(fp);
    fclose(fp_write);

    remove(file_name);
    rename(file_name_tmp, file_name);
}

int get_line_count(char * fname){
    FILE *f = fopen(fname, "r");
    char buff_1[1024];

    int n=0;
    while(fscanf(f, "%[^\n]\n", buff_1) != -1){
       n++;
    }

    fclose(f);
    return n;
}
void compare(char * file_name_1, char * file_name_2){

    FILE *f1 = fopen(file_name_1, "r");
    FILE *f2 = fopen(file_name_2, "r");


    char buff_1[1024];
    char buff_2[1024];


    int n=0;
    do{
        int result_1 = fscanf(f1, "%[^\n]\n", buff_1);
        int result_2 = fscanf(f2, "%[^\n]\n", buff_2);

        if(result_1 != -1 && result_2 != -1){
            n++;
            if(strcmp(buff_1, buff_2) != 0){
                printf("============ #%d ============\n%s\n%s\n",n, buff_1, buff_2);
            }



        }else{

            if(result_1 != -1){
                int count = get_line_count(file_name_1);
                printf("<<<<<<<<<<<< #%d TO #%d <<<<<<<<<<<< File1\n", n, count);

                do{
                    printf("%s\n", buff_1);
                }while(result_1 = fscanf(f1, "%[^\n]\n", buff_1) != -1);
            }

            if(result_2 != -1){
                int count = get_line_count(file_name_2);
                printf("<<<<<<<<<<<< #%d TO #%d <<<<<<<<<<<< File2\n", n, count);

                do{
                    printf("%s\n", buff_2);
                }while(result_2 = fscanf(f2, "%[^\n]\n", buff_2) != -1);
            }


            if(result_1 == -1 && result_2 == -1) break;
        }

    }while(1);


    fclose(f1);
    fclose(f2);
}
