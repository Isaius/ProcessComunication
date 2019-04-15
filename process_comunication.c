#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main(){
	pid_t pid;
	int status;
	int fat_son[2];	// pipe pai->filho
	int son_fat[2]; // pipe filho->pai
	
	// criando o pipe de comunicação pai-filho
	if(pipe(fat_son)<0){
		perror("pipe");
		return -1;
	}
	// criando o pipe de comunicação filh-pai
	if(pipe(son_fat)<0){
		perror("pipe");
		return -1;
	}

	// criando um processo filho, similar ao pai
	pid = fork();

	// verificando se houve algum erro na criação
	if(pid == -1){
		perror("fork");
		return -1;
	}

	// processo filho
	if(pid){
		char word[5];
		int i = 0, j = 0;

		// um processo só pode utilizar um pipe de uma única forma
		// ou lendo, ou escrevendo. Sendo obrigatorio fechar a não utilizada
		// portanto, fechando a direção de escrita do pipe pai-filho
		close(fat_son[1]);
		// lendo o dado contido no pipe
		read(fat_son[0], word, sizeof(word));
		printf("\tFilho: recebi a palavra %s\n", word);
		sleep(1);
		// realizando a inversão da palavra lida
		int max = 4;
		for(j=0;j<4;j++){
			for(i=0; i<max; i++){
				char c = word[i];
				word[i]=word[i+1];
				word[i+1] = c;
			}
			max--;
			printf("\tFilho: Progresso da inversao: %s\n", word);
			sleep(1);
		}
		printf("\tFilho: Terminei! Vou sair agora.\n");
		// novamente, para usar deve-se fechar a outra funcionalidade
		// portanto, fechando o canal de leitura do pipe filho-pai
		close(son_fat[0]);
		// escrevendo a palavra invertida no pipe
		write(son_fat[1], word, strlen(word)+1);
		sleep(1);
		exit(1);

	}else{	// processo pai

		char msg[5];
		// fechando o canal de leitura do pipe pai-filho
		close(fat_son[0]);
		// escrevendo no pipe a palavra a ser invertida pelo filho

		printf("Pai: inverta a palavra Linux para mim.\n");
		write(fat_son[1], "Linux", strlen("Linux")+1);
		// agora o pai espera o filho terminar de processar a palvra
		sleep(1);
		printf("O pai esta esperando o filho...\n");
		wait(&status);
		// o filho terminou seu trabalho
		// fechando o canal de escrita do pipe filho-pai
		close(son_fat[1]);
		// lendo a palvra, agora invertida, do pipe filho-pai
		read(son_fat[0], msg, sizeof(msg));
	
		// imprimindo agora a palavra invertida
		printf("Pai: Palavra invertida: %s\n", msg);
	}

	exit(0);
}
