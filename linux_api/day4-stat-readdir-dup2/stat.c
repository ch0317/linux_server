#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


int main(int argc, char* argv[]){
    if(argc != 2)
    {
        printf("arg error");
        return -1;
    }
    
    struct stat sb;
    stat(argv[1], &sb);

    char stmode[11] = {0};
    if(S_ISREG(sb.st_mode)) stmode[0] = '-';
    if(S_ISDIR(sb.st_mode)) stmode[0] = 'd';
    if(S_ISCHR(sb.st_mode)) stmode[0] = 'c';
    if(S_ISBLK(sb.st_mode)) stmode[0] = 'b';
    if(S_ISFIFO(sb.st_mode)) stmode[0] = 'p';
    if(S_ISLNK(sb.st_mode)) stmode[0] = 'l';
    if(S_ISSOCK(sb.st_mode)) stmode[0] = 's';

    if(sb.st_mode & S_IRUSR) stmode[1] = 'r';
    if(sb.st_mode & S_IWUSR) stmode[2] = 'w';
    if(sb.st_mode & S_IXUSR) stmode[3] = 'x';

    if(sb.st_mode & S_IRGRP) stmode[4] = 'r';
    if(sb.st_mode & S_IWGRP) stmode[5] = 'w';
    if(sb.st_mode & S_IXGRP) stmode[6] = 'x';

    if(sb.st_mode & S_IROTH) stmode[7] = 'r';
    if(sb.st_mode & S_IWOTH) stmode[8] = 'w';
    if(sb.st_mode & S_IXOTH) stmode[9] = 'x';

    struct tm *filetm = localtime(&sb.st_atim.tv_sec);


    //printf("%s %d %s %s %d %s %s\n", stmode, sb.st_nlink, getpwuid(sb.st_uid)->pw_name, getgrgid(sb.st_gid)->gr_name, sb.st_size,"",argv[1]);
    printf("%s", stmode);
    return 0;
}
