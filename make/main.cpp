#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char*args[])
{
  int input_file;
  char buff[64];
  int count = 0; 

  if((input_file = open(args[1], O_RDONLY)) < 0){
    printf("Can\'t open file\n");
    exit(-1);
  }

  while (read(input_file, buff, 23)>0){
  	printf("%s",buff);
  	
  }

  if(close(input_file) < 0){
     printf("Can\'t close file\n");
  }

  return 0;
}
