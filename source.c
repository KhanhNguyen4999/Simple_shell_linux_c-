#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/stat.h>

#define MAX_LINE 80

//Ham lay input
int getInput(char input[], char history[]){
    printf("osh>");
    fflush(stdout);
    //gets(input);
     fgets(input,MAX_LINE,stdin);// khong nen dung gets vi tu C11 da bi xoa so roi
    // xoa dau \n cuoi cung
    input[strlen(input)-1]='\0';
    if(strcmp(input, "!!") == 0){
        if(strlen(history) != 0){
            printf("%s\n", history);
            strcpy(input, history);
        }
        else{
            printf("No command in history!!!\n");
            return -1;
        }
    }
    else if(strcmp(input,"exit")==0)
        return 0;
    if(strlen(input) > 0){
        strcpy(history, input);
    }
    return 1;
}

//Ham execute thong thuong khong dung pipe
void execArgs(char *args[], int len){
    // Khởi tạo sự kiện chia tiến trình
    __pid_t child_pid;
    int flag=0;
    // Kiểm tra phần tử cuối cùng của mảng các token có dấu & hay không?
    if(*args[len-1]=='&'){// Nghĩa là tiến trinh f và tiến trình con chạy song song
        flag=1;
        args[len-1]=NULL;
        len--;
    }
    child_pid=fork();
    // Kiểm tra xem tiến trình đang chạy là tiến trình cha hay tiến trình con
    if(child_pid == 0) {// Tiến trình đang chạy là tiến trình con
        if(len >= 3){// Nếu là chuyển hướng output
            if(strcmp(args[len - 2], ">" ) == 0){
                args[len - 2] = NULL;
                // Mở file để  ghi vào
                int out = open(args[len - 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                // Chuyển từ thiết bị xuất chuẩn (màn hình) sang file
                dup2(out, STDOUT_FILENO);
                //Đóng file
                close(out);
            }            
            else if(strcmp(args[len- 2], "<" ) == 0){ // Nếu là chuyển hướng input
                args[len - 2] = NULL;
                // Mở file input ra
                int in = open(args[len - 1], O_RDONLY);
                // Chuyển hướng input từ thiết bị chuyển (keyboard) -> sang lấy từ file 
                dup2(in, STDIN_FILENO);
                // Đóng file
                close(in);
            }
        }
        // Thực thi lệnh
        if (execvp(args[0], args) < 0){
            printf("\nCould not execute in command...");
        }
        exit(1);

    }
    else if(child_pid > 0){ // Là tiến trình cha
    // Trường hợp không có dấu &
    if (flag==0)
    {
        wait(NULL);
    }
        return;
    }else {//Không thể  chia được tiến trình

        printf("\nCan't fork!!!");
        return;
    }
}

//Ham parse ra cac doi so
int parseInput(char *args[], char input[]){
    int count = 0;
    char *tok = strtok(input," ");
    while(tok != NULL){
        args[count++] = tok;
        tok = strtok(NULL, " ");
    }
    args[count] = tok;
    return count;
}

//Ham kiem tra doi so truyen vao co dung pipe hay khong? Neu co tra ve vi tri, nguoc lai tra ve -1
int checkPipe(char *args[], int len){
    for(int i = 0; i < len; i++){
        if(strcmp(args[i], "|") == 0)
            return i;
    }
    return -1;
}

//Ham execute dung pipe
void execArgsPipe(char *args[], char *argsPipe[], int flag){
    __pid_t pid;

    int fd[2];// file decription
    if(pipe(fd) < 0){// Khởi tạo 1 đường ống dẫn
        printf("\nCan't init pipe!!!!");
        return;
    }
    // Tại sự kiện chia tiến trình
    pid = fork();

    if(pid == 0){// Tiến trình con
        dup2(fd[1], STDOUT_FILENO);// ghi vào file fd[1]
        // Đóng phần fd[1] không sử dụng
        close(fd[0]);
        // Đóng phần fd[2] đã sử dụng xong
        close(fd[1]);
        // Thực thi lệnh 
        if(execvp(args[0],args) < 0){
            printf("\nCan't execute pipe 1...");
        }
        exit(1);
    }else if(pid >0){// chương trình cha
        
        pid = fork();
        if(pid == 0){
            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            close(fd[0]);
            if(execvp(argsPipe[0], argsPipe) < 0){
                printf("\nCan't execute pipe 2...");
            }
            
            exit(1);
        }
        else if(pid > 0){
            int status;
            close(fd[0]);
            close(fd[1]);
            if(flag==0)
                waitpid(pid, &status, 0);
            return;
        }
        else {
            printf("\nCan't fork!!!");
            return;
        }
    }
    else{
        printf("\nCan't fork!!!");
        return;
    }
}

//Ham exec tong quat cho tat ca truong hop
void exec(char *args[], int len){
    int flag=0;// không có &
    int iPipe = checkPipe(args, len);
    if(iPipe == -1){
        execArgs(args, len);
        return;
    } else {
        char *argsPipe[MAX_LINE / 2 + 1];
        int i;
        for(i = 0; i < len - iPipe - 1; i++){
            argsPipe[i] = strdup(args[iPipe + i +1]);
        }

        argsPipe[i] = NULL; 
        if(*argsPipe[i-1]=='&'){
            flag=1;// có &
            argsPipe[i-1] = NULL;//xóa dấu & trong lệnh
        }
        args[iPipe] = NULL;
        execArgsPipe(args, argsPipe,flag);
        return;
    }
}

int main(void)
{
    char *args[MAX_LINE / 2 + 1];
    char history[100];
    history[0] = '\0';
    int shouldRun = 1;
    int status = 0;

    while(shouldRun) {
        char input[100];
        int len;
        int res=getInput(input, history);
        if( res== 1){// thực hiện lệnh
            len = parseInput(args, input);
            exec(args, len);
        }
        else if(res==0)// exit
        {
            shouldRun=0;
        }
    }
    return 0;
}