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
		char word[7];
		int i = 0, j = 0;

		// um processo só pode utilizar um pipe de uma única forma
		// ou lendo, ou escrevendo. Sendo obrigatorio fechar a não utilizada
		// portanto, fechando a direção de escrita do pipe pai-filho
		close(fat_son[1]);
		// lendo o dado contido no pipe
		read(fat_son[0], word, sizeof(word));
		// setando o descritor
		dup2(son_fat[1],0);
		// fechando as duas vias para não dar erro no acesso do programa
		// executado com o execve
		close(son_fat[0]);
		close(son_fat[1]);
		// passando os argumentos para chamar o novo programa
		// que irá inverter a palavra passada pelo pai
		char *args[] = {"./inv", word, NULL};
		char *envp[] = {NULL};

		execve(args[0], args,envp);
		fprintf(stderr, "Error in execve\n");

	}else{	// processo pai

		char msg[5];
		// fechando o canal de leitura do pipe pai-filho
		close(fat_son[0]);
		printf("Pai: inverta a palavra Posix para mim.\n");
		// escrevendo no pipe os argumentos para o filho executar o execve
		write(fat_son[1], "Posix", strlen("Posix")+1);
		sleep(1);
		int pid_son;
		// obtendo o pid do filho apenas por preaução
		read(son_fat[0], &pid_son, sizeof(pid_son));
		// agora o pai espera o filho terminar de processar a palvra
		printf("O pai esta esperando o filho...\n");
		wait(&status);
		// o filho esta demorando demais e o pai vai encerrar sua tarefa
		printf("Pai: você está demorando demais. Vá pro quarto! [kill]\n");
		kill(pid_son, SIGTERM);
		// o filho terminou e o pai vai verificar o que ele fez
		read(son_fat[0], msg, sizeof(msg));

		printf("Palavra invertida: %s\n", msg);
	}
	exit(0);
}
