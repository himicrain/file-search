#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dir.h"

//打印链表中符合要求的文件数据
void print_list(struct PATH* head,int low ,int high){
    struct PATH* p;
    p = head;
    int count = 0;
    while(p->next != NULL){
        p = p->next;
        int size  = p->size;
        if(size >=low && size <= high){
            count ++;
            printf("%12d %s\n",size,p->path);
        }

    }
    printf("%s%d\n","Total number of links found = ",count);
}

//递归遍历文件，如果是文件夹那么递归遍历，如果是文件， 那么将文件路径和文件尺寸加入链表中
void search_path(char* current_path, struct PATH* head){
    DIR* pdir;
    struct dirent* ent;
    int i=0;
    char childpath[255];
    pdir = opendir(current_path);
    memset(childpath,0,sizeof(childpath));
    
    while((ent=readdir(pdir)) != NULL){

        if(ent->d_type & DT_DIR){
            if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0){
                continue;
            }
            sprintf(childpath,"%s/%s",current_path,ent->d_name);
            search_path(childpath,head);
        }else{
	    //声明一个结构空间，将数据存入struct，并将该struct加入链表尾
            struct PATH* path = (struct PATH*)malloc(sizeof(struct PATH));
            path->next = NULL;
            
            path->path = (char*)malloc(sizeof(char)*255);
            sprintf(path->path,"%s/%s",current_path,ent->d_name);

            struct stat statbuf;  
            stat(path->path ,&statbuf);  
            path->size = statbuf.st_size;

            struct PATH* p;
            p = head;
            while(p->next != NULL){
                p = p->next;
            }
            p->next = path;
        }
    }


}


int main(int argc,char *argv[]){       
    char current_path[255];
    if(argv[1][0] == '/'){
        strcpy(current_path,argv[1]);
    }else{
        getcwd(current_path,sizeof(current_path));
        strcat(current_path,"/");
        strcat(current_path,argv[1]);
    }
    current_path[254] = '\0';
    printf("--------");
    printf("%s\n",current_path);
    
    struct PATH* head;

    
    head = (struct PATH*)malloc(sizeof(struct PATH));

    head->next = NULL;
    head->path = (char*)malloc(sizeof(char)*255);
    sprintf(head->path,"%s",current_path);
    head->size = 0;

    search_path(current_path,head); 
    int low = 0;
    int high = 0;
    sscanf(argv[2],"%d",&low);
    sscanf(argv[3],"%d",&high);
    printf("%s[%d, %d]\n","Links with size (in kilobytes) in ",low*1024,high*1024);
    print_list(head,low*1024,high*1024);
    
    return 0;  
}
