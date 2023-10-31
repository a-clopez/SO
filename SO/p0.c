//Pablo Seoane Vázquez pablo.seoane.vazquez@udc.es
//Antón Calviño López  a.clopez@udc.es


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "dynamic_list.h"
#include "dynamic_list.c"
#include <sys/utsname.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>


#define MAXLINEA 2048
#define MAXNAME 256




void ProcesarComando(char *tr[], tList *cList,tList *fList);


void storeOnList(char *tr[], tList *list) {
    struct tNode node;
    char store[MAXLINEA];
    strcpy(store, "");
    int i = 0;

    while(tr[i] != NULL) {
        strcat(store, tr[i]);
        strcat(store, " ");
        i++;
    }

    //insert the new string on the list
    strcpy(node.data.text,store);
    insertItem(node.data, LNULL, list);

    //the values on the string are cleared
    memset(store, 0, sizeof store);
}

struct CMD{
	char * name;
	void (*func)(char **, tList ,tList *);
};

void printHist(tPosL pos, tList cList) {
    //prints the contents of the given list
    tPosL aux = first(cList);
    struct tNode node;
    int i = 0;

    while(aux < pos){
        node.data = getItem (aux, cList);
        printf("%i -> %s\n", i, node.data.text);
        aux = next(aux, cList);
        i++;
    }
    node.data = getItem (aux, cList);
    printf("%i -> %s\n", i, node.data.text);
}

int TrocearCadena(char * cadena, char * tr[]){
 	int i=1;
	if ((tr[0]=strtok(cadena," \n\t"))==NULL)
		return 0;
	while ((tr[i]=strtok(NULL," \n\t"))!=NULL)
		i++;
    return i;
}

char LetraTF (mode_t m)
{
    switch (m&S_IFMT) { //and bit a bit con los bits de formato,0170000
        case S_IFSOCK: return 's'; //socket
        case S_IFLNK: return 'l'; //symbolic link
        case S_IFREG: return '-'; //fichero normal
        case S_IFBLK: return 'b'; //block device
        case S_IFDIR: return 'd'; //directorio
        case S_IFCHR: return 'c'; //char device
        case S_IFIFO: return 'p'; //pipe
        default: return '?'; //desconocido, no deberia aparecer
    }
}
/*las siguientes funciones devuelven los permisos de un fichero en formato rwx----*/
/*a partir del campo st_mode de la estructura stat */
/*las tres son correctas pero usan distintas estrategias de asignaciÃ³n de memoria*/

char * ConvierteModo (mode_t m, char *permisos)
{
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    //propietario
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    //grupo
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    //resto
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    //setuid, setgid y stickybit
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}


char * ConvierteModo2 (mode_t m)
{
    static char permisos[12];
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    //propietario
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    //grupo
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    //resto
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    //setuid, setgid y stickybit
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

char * ConvierteModo3 (mode_t m)
{
    char *permisos;

    if ((permisos=(char *) malloc (12))==NULL)
        return NULL;
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    //propietario
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    //grupo
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    //resto
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    //setuid, setgid y stickybit
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

void Cmd_authors(char *tr[], tList cList, tList *fList)
{
	if (tr[0]==NULL){
		printf("Pablo Seoane Vázquez pablo.seoane.vazquez@udc.es\n");
		printf("Antón Calviño López  a.clopez@udc.es\n");
		return;
	}
	if (!strcmp(tr[0],"-l")){
		printf ("pablo.seoane.vazquez@udc.es\n");
		printf ("a.clopez@udc.es\n");
		}
	else if (!strcmp(tr[0],"-n")){
		printf("Pablo Seoane Vázquez\n");
		printf("Antón Calviño López\n");
		}
	else{
		printf("Pablo Seoane Vázquez pablo.seoane.vazquez@udc.es\n");
		printf("Antón Calviño López  a.clopez@udc.es\n");
		}
}

void Cmd_quit(char *tr[], tList cList, tList *fList)
{
    while(!isEmptyList(cList)) {
        deleteAtPosition(first(cList), &cList);
    }
    while(!isEmptyList(*fList)) {
        deleteAtPosition(first(*fList), fList);
    }
    free(*tr);
	exit(0);

}

void Cmd_pid(char *tr[], tList cList, tList *fList)
{
	if(tr[0]==NULL)
		printf("Shell's pid is %d\n",getpid());
	else if(!strcmp(tr[0],"-p"))
		printf("Shell's parent pid is %d\n",getppid());
	
}

void Cmd_chdir(char *tr[], tList cList, tList *fList){
	char dir[MAXNAME];
	getcwd(dir,sizeof(dir));
	if(tr[0]==NULL)
		printf("Current directory is %s\n",dir);
	else{
		if(chdir(tr[0])!=0)
			printf("Error in changing directory\n");
}
			
		
}

void Cmd_date(char *tr[], tList cList, tList *fList){
	time_t t;
	t=time(NULL);
	struct tm tm= *localtime(&t);
	printf("%d-%d-%d\n",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
}

void Cmd_time(char *tr[], tList cList, tList *fList){
    time_t t;
    t=time(NULL);
    struct tm tm;
    tm=*localtime(&t);
    printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void Cmd_help(char *tr[], tList cList, tList *fList){
	char *cmd[MAXNAME] ={"authors","quit","exit","bye","pid","chdir","date","time","help","open","close","dup","infosys","hist","comand","listopen"," "};
	if(tr[0]==NULL){
		int i =0;
		while(strcmp(cmd[i]," ")){
			printf("%s ",cmd[i]);i++;
			}
		printf("\n");
	}
	else if(!strcmp(tr[0],"authors"))
		printf("Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names\n");
	else if(!strcmp(tr[0],"quit"))
		printf("Ends the shell\n");
	else if(!strcmp(tr[0],"exit"))
		printf("Ends the shell\n");
	else if(!strcmp(tr[0],"bye"))
		printf("Ends the shell\n");
	else if(!strcmp(tr[0],"pid"))
		printf("Prints the pid of the process executing the shell. pid -p rints the pid of the shell’s parent process.\n");
	else if(!strcmp(tr[0],"chdir")){
		printf("Changes the current working directory of the shell to dir (using the chdir system call). When invoked without auguments it prints the current working directory (using the getcwd system call.\n");}
	else if(!strcmp(tr[0],"date"))
		printf("Prints the current date in the format DD/MM/YYYY\n");
	else if(!strcmp(tr[0],"time"))
		printf("Prints the current time in the format hh:mm:ss\n");
	else if(!strcmp(tr[0],"help"))
		printf("help displays a list of available commands. help cmd gives a brief help on the usage of comand cmd\n");
	else if(!strcmp(tr[0],"open"))
		printf(" Opens a file and adds it (together with the file descriptor and the opening mode to the list of shell open files. For the mode we’ll use cr for O CREAT, ap for O APPEND, ex for O EXCL, ro for O RDONLY, rw for O RDWR, wo for O WRONLY and tr for O TRUNC. The pro- vided file ayudaP0.c provides some code that can be used for this lab assignment\n");
	else if(!strcmp(tr[0],"close"))
		printf("Closes the df file descriptor and eliminates the corresponding item from the list\n");
	else if(!strcmp(tr[0],"dup"))
		printf("Duplicates the df file descriptor (using the dup system call, creating the corresponding new entry on the file list\n");
	else if(!strcmp(tr[0],"infosys"))
		printf("Prints information on the machine running the shell (as obtained via the uname system call/library function)\n");
	else if(!strcmp(tr[0],"hist"))
		printf("Shows/clears the historic of commands executed by this shell. In order to do this, a list to store all the commands input to the shell must be implemented. hist -c clears the historic, that’s to say, empties the list – hist Prints all the comands that have been input with their order number – hist -c Clears (empties) the list of historic commands – hist -N Prints the first N comands\n");
	else if(!strcmp(tr[0],"comand"))
		printf("Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names\n");
	else if(!strcmp(tr[0],"listopen"))
		printf("Lists the shell open files. For each file it lists its descriptor, the file name and the opening mode. The shell will inherit from its parent process open descriptors 0, 1 and 2 (stdin, stout, and stderr). To get the opening mode from a descriptor (df) we can use fcntl(fd,F GETFL).\n");
}


void Cmd_commandN(char *tr[], tList cList, tList *fList){
	if(isEmptyList(cList)){printf("empty list\n");return;}
	tPosL p = first(cList);
	struct tNode node;
	bool big = false;
	
	int a=atoi(tr[0]);
        	for(int i=0; i<a-1; i++){
        		if(next(p,cList)==NULL) {
        			printf("This number is too big\n");
        			i=a;
        			big=true;
        		}
        		else{
        			p=next(p,cList);
        		}
    		}
    		if(!big){
    		node.data = getItem (p, cList);
    		TrocearCadena(node.data.text,tr);
		printf("Executing this command: %s\n", node.data.text);
		ProcesarComando(tr,&cList,fList);
		}
    		
    		
}

void Cmd_hist(char *tr[], tList cList, tList *fList) {
    if(isEmptyList(cList)){printf("list is empty\n");return;}
    tPosL p = first(cList);

    if(tr[0] == NULL) printHist(last(cList),cList);
    else {
        //clear list
        if(!strcmp(tr[0], "-c")){
            while(!isEmptyList(cList)){
                p = last(cList);
                deleteAtPosition(p, &cList);
            }
        }
            //print until given number
        else{
        	int a=abs(atoi(tr[0]));
        	for(int i=0; i<a-1; i++){
        		if(next(p,cList)==NULL) printHist(last(cList),cList);
        		
        		else{
        			p=next(p,cList);
        		}
    		}
            printHist(p,cList);
        }
    }
}


void Cmd_infosys(char *tr[], tList cList, tList *fList){
    struct utsname unameData;
    uname(&unameData);
    printf("%s %s %s %s %s\n", unameData.sysname,unameData.nodename,unameData.release,unameData.version,unameData.machine);
}

void Cmd_listopen(char *tr[], tList cList, tList *fList){
    int pos=last_int(*fList);
    struct tNode node;
    tPosL aux=first(*fList);

    if(isEmptyList(*fList))
        printf("List is empty\n");
    else {
        for (int i = 0; i <= pos; i++) {
            node.data = getItem(aux, *fList);
            aux= next(aux,*fList);
            int mode=node.data.mode;
            //if (tr[1] == NULL)
            //    printf("descriptor %d: %s\n", i + 1, &node.data.text);
            if (mode==64)
                printf("Descriptor %d: %s O_CREAT\n", node.data.df, node.data.text);
            else if (mode==128)
                printf("Descriptor %d: %s O_EXCL\n", node.data.df , node.data.text);
            else if (mode==0)
                printf("Descriptor %d: %s O_RDONLY\n", node.data.df, node.data.text);
            else if (mode==1)
                printf("Descriptor %d: %s O_WRONLY\n", node.data.df, node.data.text);
            else if (mode==2)
                printf("Descriptor %d: %s O_RDWR\n", node.data.df, node.data.text);
            else if (mode==1024)
                printf("Descriptor %d: %s O_APPEND\n", node.data.df, node.data.text);
            else if (mode==512)
                printf("Descriptor %d: %s O_TRUNC\n", node.data.df, node.data.text);
            else {
                printf("Error: command not executed\n");
                break;
            }
        }
    }
}


void Cmd_open (char * tr[], tList cList, tList *fList)
{
    int i,df, mode=77;

    char a[MAXLINEA];
    memset(a,'\0',sizeof(a));
    
    if (tr[0]==NULL) { //no hay parametro
       	Cmd_listopen(tr,cList,fList);
        return;
    }
    for (i=1; tr[i]!=NULL; i++)
      if (!strcmp(tr[i],"cr")) mode=O_CREAT;         //create
      else if (!strcmp(tr[i],"ex"))mode=O_EXCL;     //create if can't error
      else if (!strcmp(tr[i],"ro")) mode=O_RDONLY;   //read only
      else if (!strcmp(tr[i],"wo")) mode=O_WRONLY; //write only
      else if (!strcmp(tr[i],"rw")) mode=O_RDWR;    //read and write
      else if (!strcmp(tr[i],"ap")) mode=O_APPEND;   //appends at the end
      else if (!strcmp(tr[i],"tr")) mode=O_TRUNC;  //deletes everything
      else break;

    df= open(tr[0],mode,0777);
    if ((df==-1)||(tr[1]==NULL))
        printf("Imposible to open file\n");
    else{
    	tItemL archivo;
   	    strcpy(archivo.text,tr[0]);
        archivo.df=df;
        archivo.mode=mode;
        insertItem(archivo,LNULL,fList);
        printf ("Anadida entrada a la tabla ficheros abiertos %d\n", archivo.df);
	}
}

void Cmd_close (char *tr[], tList cList, tList *fList)
{ 
    int df=0;
    tItemL item;
    item.df=20000000;
    tPosL pos;
    tPosL aux;
    int id=0;
    char *val=tr[0];
    if (val!=NULL) id= *val - '0';
    for (pos=first(*fList); item.df!=id; pos= next(pos,*fList)){
        if (pos!=NULL){
            item = getItem(pos, *fList);
        }else return;
    }
    aux=findItem_df(id,*fList);
    if (tr[0]==NULL){
        Cmd_listopen(tr,cList,fList);
        return;
    }else {
        if(aux==NULL) printf("Error: file cannot be closed\n");
        else {
            df = item.df;
            if (df == -1 || close(df) == -1)
                printf("Error: file cannot be closed\n");
            else {
                printf("The item has been deleted\n");
                deleteAtPosition(aux, fList);
            }
        }
    }
}

void Cmd_dup (char * tr[], tList cList, tList *fList)
{
    int df;
    char aux[MAXNAME],*p;
    tItemL archivo, item;
    int last=last_int(*fList);
    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { //no hay parametro
        Cmd_listopen(tr,cList,fList);                //o el descriptor es menor que 0
        return;
    }
    if ((df=atoi(tr[0]))>last){
        printf("Cannot duplicate descriptor\n");
    }
    else {
        int id= *tr[0] - '0';
        item.df=id+1;
        for (tPosL pos=first(*fList); item.df!=id; pos= next(pos,*fList)){
            item = getItem(pos, *fList);
        }
        strcpy(archivo.text,item.text);
        archivo.df= last+1;
        archivo.mode=item.mode;
        printf ("Anadida entrada a la tabla ficheros abiertos %d\n", archivo.df);
        sprintf(aux, "dup %d (%s)", df, archivo.text);
        strcpy(archivo.text,aux);
        insertItem(archivo,LNULL,fList);
    }
}



void Cmd_create (char *tr[], tList clist, tList *fList){
    if(tr[1]==NULL){
        int check;
        check=mkdir(tr[0],0777);
        if (!check) printf("Directory created successfully\n");
        else printf("Directory could not be created\n");
    }else {
        if (!strcmp(tr[0], "-f")){
            FILE* ocheck=fopen(tr[1],"w");
            if (ocheck==NULL)
                printf("File could not be created\n");
            else printf("File created successfully\n");

        }
    }
}
void Cmd_delete(char *tr[], tList cList, tList *fList){
    int n = 0,i=0;
    bool conti = true;
    while(tr[i]!=NULL){
    struct dirent *carp;
    DIR *dir = opendir(tr[i]);
    if (remove(tr[i])==0 && conti){
        printf("You've removed %s sucessfully\n",tr[i]);
        conti=false;
    }
    else if(dir == NULL && conti){
        printf("Doesn't exist such file or directory: %s\n",tr[i]);
        conti=false;
    }
    else {
        while((carp= readdir(dir)) != NULL && conti) {
            if (++n>2){
            break;
            }
        }
        closedir(dir);
        if (n<=2 && conti) {
            rmdir(tr[i]);
            printf("Removed %s successfully\n", tr[i]);
            conti=false;
        }
        if(conti) printf("Imposible borrar %s\n",tr[i]);
    }
    conti=true;i++;
}
}

void del (char* direct){
    struct stat buf;
    struct dirent *dir;
    DIR* de;

    if(lstat(direct,&buf)==-1){
        printf ("Error");
        return;
    }
    if (S_ISDIR(buf.st_mode)){
        if ((de = opendir(direct))==NULL){
            printf("Error");
            return;
        }
        while((dir= readdir(de))!=NULL){
            char path[MAXLINEA];
            if (strcmp(dir->d_name,".")==0 || strcmp(dir->d_name,"..")==0){continue;}
            sprintf(path,"%s/%s",direct,dir->d_name);
            del(path);
        }
        closedir(de);
    }
    if(remove(direct)==-1){
        printf("Error");
    }
}


void Cmd_deletetree(char *tr[], tList cList, tList *fList){
    char d[MAXLINEA]={0};
    if (tr[0]==NULL){
        printf("%s\n",getcwd(d,MAXLINEA));
        return;
    }if (!strcmp(tr[0],".")){
        printf("Cannot delete %s: Invalid argument\n", tr[0]);
    }for (int i=0; tr[i]!=NULL;i++) {
        del(tr[i]);
    }
}

void print_stats(struct stat buf,char *name,bool acc,bool link){
    struct tm tm;
    if(acc)
        tm = *localtime(&buf.st_atim.tv_sec);
    else
        tm = *localtime(&buf.st_mtim.tv_sec);
    printf("%d-%d-%d-",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    printf("%02d:%02d\t", tm.tm_hour, tm.tm_min);
    printf("%3ld ",buf.st_nlink);
    printf("(%7ld)\t" ,buf.st_ino);
    printf("%s\t%s ",getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name);
    printf("%s %d %s", ConvierteModo2(buf.st_mode),(int)buf.st_size,name);
    if(!link)
        printf("\n");

}

void Cmd_stat (char *tr[], tList cList, tList *fList) {
    char dir[MAXNAME] = {0};
    getcwd(dir, sizeof(dir));
    struct dirent *de;
    struct stat buf;
    char path[2048];
    bool is_long=false,is_link=false,is_acc=false;
    int i = 0;
    bool exist=true;
    bool a=false;
    if (tr[0] != NULL) {
        while (tr[i] != NULL){
            if (!strcmp(tr[i], "-long"))
                is_long=true;
            if (!strcmp(tr[i], "-acc"))
                is_acc=true;
            if(!strcmp(tr[i],"-link"))
                is_link=true;
            i++;
        }
        if (is_long){
            i=1;
            if (tr[i] ==NULL){
                DIR *cdr = opendir(dir);//Si solo mandan stat
                printf("%s\n", dir);
                return;
            }
            while (tr[i] != NULL){
                a=false;
                exist=true;
                if (!strcmp(tr[i], "-long" )|| !strcmp(tr[i],"-acc") || !strcmp(tr[i],"-link"))
                    i++;
                else{
                    a=true;
                    char link[MAXLINEA];
                    while (tr[i] != NULL) {//check if they pass a valid file
                        DIR *dr = opendir(tr[i]);
                        if (dr == NULL) {
                            FILE *file = fopen(tr[i], "r");
                            if (file == NULL) {
                                printf("Doesn't exist such file or directory\n");
                                exist=false;
                            }
                        }
                        if (S_ISLNK(buf.st_mode) && exist && is_link) {//check if there is a link
                                ssize_t length = readlink(tr[i], link, sizeof(link) - 1);
                                lstat(tr[i], &buf);
                                print_stats(buf, tr[i], is_acc, is_link);
                                if (length != -1) {
                                    link[length] = '\0';
                                    printf("-> %s\n", link);

                            }
                        }else if (exist){//no hay link
                            lstat(tr[i], &buf);
                            print_stats(buf, tr[i], is_acc,is_link);
                            break;
                        }
                    }
                    i++;
                }
            }
            if(a)return;
            DIR *cdr = opendir(dir);//Si solo mandan stat
            printf("%s\n", dir);
            return;
        }//Si no tienes long
        i=0;
        while (tr[i]!=NULL){
            a=false;
            exist=true;
            if (!strcmp(tr[i],"-acc") || !strcmp(tr[i],"-link"))
                i++;
            else {
                a=true;
                DIR *dr = opendir(tr[i]);
                if (dr == NULL) {
                    FILE *file = fopen(tr[i], "r");
                    if (file == NULL) {
                        printf("Doesn't exist such file or directory: %s\n", tr[i]);
                        exist = false;
                    }
                }
                if(exist){
                lstat(tr[i], &buf);
                printf("%d %s\n", (int) buf.st_size, tr[i]);
                }
                i++;
            }if(a)return;
            DIR *cdr = opendir(dir);//Si solo mandan stat
            printf("%s\n", dir);
            return;
        }
    }
    DIR *cdr = opendir(dir);//Si solo mandan stat
    printf("%s\n", dir);
}
        /*
        if (!strcmp(tr[0], "-long")) {//check long
            if (tr[1] != NULL) {
                if (!strcmp(tr[1], "-link")) {//check link
                    i = 2;
                    if (tr[i] == NULL) {//check if they pass a file
                        printf("%s\n", dir);
                        return;
                    }
                    char link[MAXLINEA];
                    while (tr[i] != NULL) {//check if they pass a valid file
                        DIR *dr = opendir(tr[i]);
                        if (dr == NULL) {
                            FILE *file = fopen(tr[i], "r");
                            if (file == NULL) {
                                printf("Doesn't exist such file or directory\n");
                            }
                        }
                        if (S_ISLNK(buf.st_mode)) {//check if there is a link
                            ssize_t length = readlink(tr[i], link, sizeof(link) - 1);
                            lstat(tr[i], &buf);
                            print_stats(buf, tr[i], 2);
                            if (length != -1) {
                                link[length] = '\0';
                                printf("-> %s\n", link);
                            }
                        } else {
                            lstat(tr[i], &buf);
                            print_stats(buf, tr[i], 1);
                        }
                        i++;
                    }
                    return;
                }
                if (!strcmp(tr[1], "-acc")) {//check if acc
                    i = 2;
                    if (tr[i] == NULL) {//check if they pass a file
                        printf("%s\n", dir);
                        return;
                    }
                    while (tr[i] != NULL) {
                        DIR *dr = opendir(tr[i]);
                        if (dr == NULL) {
                            FILE *file = fopen(tr[i], "r");
                            if (file == NULL) {
                                printf("Doesn't exist such file or directory: %s\n", tr[i]);
                            }
                        }
                        lstat(tr[i], &buf);
                        print_stats(buf, tr[i], 0);
                        i++;
                    }
                    return;
                }
                i = 1;
                while (tr[i] != NULL) {//default case of long
                    DIR *dr = opendir(tr[i]);
                    if (dr == NULL) {
                        FILE *file = fopen(tr[i], "r");
                        if (file == NULL) {
                            printf("Doesn't exist such file or directory: %s\n", tr[i]);
                        }
                    }
                    lstat(tr[i], &buf);
                    print_stats(buf, tr[i], 1);
                    i++;
                }
                return;

            } else {
                DIR *cdr = opendir(dir);//Aqui con este opendir se piertedn muchos bytes por alguna razón
                printf("%s\n", dir);
                return;
            }
        } else {
            if (!strcmp(tr[0], "-link") || !strcmp(tr[0], "-acc")){//check stat + -link or -acc
                i=1;
                if (tr[i] == NULL) {//check if they pass a file
                    printf("%s\n", dir);
                    return;
                }
            while (tr[i] != NULL) {
                DIR *dr = opendir(tr[i]);
                if (dr == NULL) {
                    FILE *file = fopen(tr[i], "r");
                    if (file == NULL) {
                        printf("Doesn't exist such file or directory: %s\n", tr[i]);
                    }
                }
                lstat(tr[i], &buf);
                printf("%d %s\n",(int)buf.st_size,tr[i]);
                i++;
            }
                return;
            }else{//check for only stat + file
                i=0;
                while (tr[i] != NULL) {
                    DIR *dr = opendir(tr[i]);
                    if (dr == NULL) {
                        FILE *file = fopen(tr[i], "r");
                        if (file == NULL) {
                            printf("Doesn't exist such file or directory: %s\n", tr[i]);

                        } else {
                            lstat(tr[i], &buf);
                            printf("%d %s\n", (int) buf.st_size, tr[i]);
                        }
                    }
                    else {
                        lstat(tr[i], &buf);
                        printf("%d %s\n", (int) buf.st_size, tr[i]);
                    }
                    i++;
                }
                return;
            }
        }

         */
void list_aux (char tr[], bool hidden) {
    int i =0;
    struct dirent *de;
    struct stat buf;
    struct stat file;
    char name;
    char path[MAXLINEA];
    DIR *dr = opendir(&tr[i]);
    if (dr == NULL) {
        return;
    }
    printf("*****************%s\n", &tr[i]);
    while ((de = readdir(dr)) != NULL) {
        sprintf(path, "%s/%s", &tr[i], de->d_name);
        lstat(path, &buf);
        if ((!strcmp(de->d_name, "."))||(!strcmp(de->d_name, ".."))) {
            if (hidden == true) {
                printf("%d %s\n", (int) buf.st_size, de->d_name);
            }
        } else {
            printf("%d %s\n", (int) buf.st_size, de->d_name);
        }
    }
    closedir(dr);
    }


void list_rec (char tr[], bool hidden, int rec) {
    int i=0;
    struct dirent *de = NULL;
    struct stat buf;
    struct stat file;
    char name;
    char path[MAXLINEA];
        DIR *dr = opendir(&tr[i]);
        if (dr == NULL) {
           return;
        }
        while ((de = readdir(dr)) != NULL) {
            if ((!strcmp(de->d_name, "."))||(!strcmp(de->d_name, ".."))) {
                if (hidden == true) {
                    sprintf(path, "%s/%s", tr, de->d_name);
                    if(rec==2){
                        list_rec(path, hidden, rec);
                    }list_aux(path, hidden);
                    if(rec==1) {
                        list_rec(path, hidden, rec);
                    }

                }
                else sprintf(path, "%s/%s", &tr[i], de->d_name);
            } else {
                sprintf(path, "%s/%s", &tr[i], de->d_name);
                if(rec==2){
                    list_rec(path, hidden, rec);
                }list_aux(path, hidden);
                if(rec==1) {
                    list_rec(path, hidden, rec);
                }
            }
        }
        closedir(dr);
    }


void Cmd_list(char *tr[], tList cList, tList *fList){
    int i=0;
    bool perm;
    bool hidden=false;
    bool stats=false;
    int rec=0;
    struct stat buf;
    char path[MAXLINEA];
    if (tr[0]==NULL){
        printf("%s\n", getcwd(path,MAXLINEA));
        return;
    }
    if(!strcmp(tr[i],"-hid")){
        hidden=true;
        i++;
    }
    if(!strcmp(tr[i],"-long")){
        stats=true;
        i++;
        if(tr[1]==NULL){
            lstat(tr[0],&buf);
            print_stats(buf,tr[0],false,false);
        }
    }
    if(tr[i]!=NULL){
        if(!strcmp(tr[i],"-reca")){
            rec=1;
            i++;
        }
    }
    if(tr[i]!=NULL){
    if(!strcmp(tr[i],"-recb")){
        rec=2;
        i++;
    }
    }

    for (i;tr[i]!=NULL;i++){
        perm = access(tr[i],R_OK);
        if (perm) {
            printf("You dont have permissions\n");
            return;
        }
        lstat(tr[i],&buf);
        if(!S_ISDIR(buf.st_mode)){
            if(stats){
                print_stats(buf,tr[i],false,false);
            }
            else list_aux(*tr,hidden); //a sustituir por stat
            return;
        }
        if(opendir(tr[i])==NULL){
            printf("Cannot access %s: No such file or directory\n",tr[i]);
            return;
        }
        if(rec==2){
            list_rec(tr[i],hidden,rec);
        }
        list_aux(tr[i],hidden);
        if(rec==1){
            list_rec(tr[i],hidden,rec);
        }

    }
}

void ProcesarComando(char *tr[], tList *cList,tList *fList)
{
	int i;

	static struct CMD C[]={
		{"authors",Cmd_authors},
		{"quit",Cmd_quit},
		{"exit",Cmd_quit},
		{"bye",Cmd_quit},
		{"pid",      Cmd_pid},
		{"chdir",Cmd_chdir},
		{"date",     Cmd_date},
		{"time",     Cmd_time},
		{"hist",     Cmd_hist},
		{"help",     Cmd_help},
		{"infosys",  Cmd_infosys},
		{"command",  Cmd_commandN},
        {"open",     Cmd_open},
        {"close",    Cmd_close},
        {"listopen",  Cmd_listopen},
        {"dup",  Cmd_dup},
        {"create",  Cmd_create},
        {"list",  Cmd_list},
        {"stat",  Cmd_stat},
        {"delete",  Cmd_delete},
        {"deltree",  Cmd_deletetree},
		{NULL,NULL}
	};

	if (tr[0]==NULL)
		return;
	
	for (i=0;C[i].name!=NULL;i++){
		if (!strcmp(tr[0],C[i].name)){
			(*C[i].func)(tr+1, *cList, fList);
			return;
		}
	}
    i=0;
    tItemL item;
    strcpy(item.text,"");
    while(tr[i] != NULL) {
        strcat(item.text,tr[i]);
        strcat(item.text," ");
        i++;
    }
    insertItem( item,LNULL,cList);
	printf("Command not found\n");
}

int main(){
	char *linea;
	char *tr[MAXNAME/2];
    tList commandlist;
    tList filelist;
	linea=malloc(MAXNAME);	//if it's inside the loop it allocates memory in each iteration
	createEmptyList(&commandlist);
	createEmptyList(&filelist);
    init_filelist(&filelist);
	while (1) {
		printf("-->");
		fgets(linea,MAXNAME,stdin);
		TrocearCadena(linea,tr);
		ProcesarComando(tr,&commandlist,&filelist);
		storeOnList(tr,&commandlist);
	}
	
}
