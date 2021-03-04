#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void output(FILE* file){
	char ch;
	
	do{
		ch = getc(file);
		printf("%c",ch );	
	}while(ch != EOF);
	
	if (ferror (file)){
      		fprintf (stderr, "Output to stream failed.\n");
      		exit (EXIT_FAILURE);
    	}
    	
}

int main(){
	//first ask user to provide a process name and use it during entire program
	char process_name[20];
	printf("give me a process or command name\n");
	scanf("%s",process_name);
	
	//using popen to creat a stream file to acquire the process id of current process
	//using argument pidof. In the end after print out the file we have pid
	
	char command[20] = "pidof ";
	strcat(command,process_name);
	
	FILE* file = popen(command,"r");
	
	if(file == NULL){
		printf("error occured during stream file");
	}
	
	output(file);
	int status =5;
	//we found through manual that if pclose doesnt appear 0 
	//after popen we have some trouble. it is also a way to 
	//check existance of process
    	if (pclose (file) != 0){
    		printf("error, there might not exist this process\n");
	}else{
		int child1,child2,child3;
		
	//if we directly use 3 forks we will have 8 children instead of 3
	// there for we need to let child done his job then we create next child
		child1 = fork();
	// the following code all used a command called execvp 
	//we will just provide the address of the command and add some arguments.
	//we can manipulates all the linux commands as we want
		if(child1 == 0){
			char *argv[3] = {"Command",process_name,NULL};
			printf("%s: ",process_name);
			execvp("whereis",argv);
		}else{
			child2 = fork();
			if(child2 == 0){
				char argument[20] = " man ";
				strcat(argument,process_name); 
				char *argv[] = {"Command","-e",argument,NULL};
				execvp("gnome-terminal",argv);
			}else{
				child3 = fork();
				if(child3 == 0 ){
					char argument[100] = "http://www.google.com/search?q=";
					strcat(argument,process_name);
					char *argv[] = {"Command",argument,NULL};
					execvp("firefox",argv);
				}
			}
		}
		
	}
   	
   	wait(&status);	
   	
	return 0;
}
