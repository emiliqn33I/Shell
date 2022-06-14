

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include<sys/wait.h>

static void doPrint (char *s)
{
        if ( write (STDOUT_FILENO, s, strlen(s)) < 0 )
        {
        	perror("Greshka:");
        	return ;
        }
}


char **parse_cmdline(const char *cmdline )
{
	
	char masive[strlen(cmdline)+1];
	
	strcpy(masive, cmdline);
	
	char *token = strtok(masive, " ");
	
	int size = 2;// nachalno nqkakvo chislo za siza-a
	
	int i = 0;
	
	char **arrayche = malloc (sizeof (char *) * size);
	
	while (token != NULL) 
	{	
		arrayche[i] = (char * )malloc(sizeof(char) * strlen(token));
		
		strcpy(arrayche[i], token);
		
		i++;
		
		size += 1;
		
		arrayche = realloc(arrayche, ((size + 1) * sizeof(char *)));
		
		token = strtok(NULL, " ");
    	}
	
	arrayche[i-2] = NULL;
	
	return arrayche;
}


int main( )
{
	
	while(1)
	{
		int size = 100;
	
		char *arr = malloc(size);
		
		ssize_t  res;
		
		int offset = 0;
	
		doPrint("$ ");
		
		while(1)
		{
			res = read(STDIN_FILENO, &arr[offset], 1);
			
			if(res == 0)
			{
				return 0;
			
			}
			else if(res < 0)
			{
				perror("reading: ");
				return 0;
			}
			
			
			else if( (offset+1) > size)
			{
				size += 100;
				
				arr = realloc(arr, size);
			}
			
			else if(arr[offset] == '\n')
			{
				break;
			}
			offset++;
			
		}	
		
		int broqch = 0;
		int j = 0;
		char masive[200];
		
		for(int i = 0; arr[i] != '\0'; i++)// cikul za mahaneto na /bin/ za vtoriq argument na execv
		{
			if(broqch >= 2)
			{
				masive[j] = arr[i];
				j++;
			}
			if(arr[i] == '/')
			{
				broqch++;
			}
		}
		
		char **strMasive = parse_cmdline(masive);
		
		char **arrMasive = parse_cmdline(arr);	
		
		free(arr);
		
		pid_t child_pid = fork();
			
		int status;
		
		if(child_pid == 0)
		{
			if(execv(arrMasive[0], strMasive) < 0 )
			{
				perror(arrMasive[0]);
				
				exit(0);
				
				return 0;
			}
			else
			{
				exit(0);
					
				return 0;
			}
		}
		
		else if(child_pid > 0)
		{
			waitpid(child_pid, &status, 0); 
			
		}
		else if (child_pid < 0)
		{
			perror("fork: ");
			return 0;
		}
		
	}
	
	

	return 0;
}
