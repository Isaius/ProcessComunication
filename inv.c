#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(int argc, char *argv[]){
		int i = 0, j = 0, pid;
		printf("\tFilho executando! Vou inverter!\n");
		pid = getpid();
		// enviando para o pai o pid para que ele possa termina-lo
		write(STDIN_FILENO, &pid, sizeof(pid));

		char str[5];
		// armazenando a palavra a ser invertida
		strcpy(str, argv[1]);

		// imprimindo a palavra recebida por agumeto
		printf("\tRecebi a palavra %s\n", str);
		// realizando a inversão da palavra lida
		int max = 4;
		for(j=0;j<4;j++){
			for(i=0; i<max; i++){
				char c = str[i];
				str[i]=str[i+1];
				str[i+1] = c;
			}
			printf("\tProgresso da inversão da palavra: %s\n", str);
			max--;
		}
		printf("\t. . .\n");
		// escrevendo a palavra invertida no pipe filho->pai
		write(STDIN_FILENO, str, sizeof(str));

		// este sleep gigante é para o filho ficar bloqueado e o pai usar o kill
		// pois chamar sleep bloqueia o processo
		sleep(500);
}
