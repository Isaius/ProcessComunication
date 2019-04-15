# ProcessComunication
Implementação de comunicação entre processos em C

O arquivo process_comunication.c implementa a comunicação entre processos usando a função fork() para criar um processo filho
e utiliza dois pipes para a comunicação entre eles. Um pipe no sentido pai->filho e outro no sentido contrário. Há como usar
apenas um pipe, mas por conveniência decidi usar dois.

O processo pai pede ao filho que inverta uma palavra que ele irá enviar pelo pipe pai->filho.
O filho lê a palavra enviada pelo pai no pipe pai->filho, inverte, e depois a escreve no pipe filho->pai.
O pai então lê o pipe filho->pai e imprime na tela. E termina usando a função exit().

Mais informações no código comentado.

================================================================================

O arquivo process_comunication_execve.c implementa o mesmo problema, com a mesma função de inverter uma palavra.
A diferença que dessa vez a parte que inverte a palavra está em outro código, contido no arquivo inv.c.
Para fazer com que a palavra seja invertida, o processo precisa invocar este programa inv para fazer o trabalho.
Para isso ele usa a função execve, que invoca um executavel c ou script no diretorio passado e lhe passa os argumentos.

Vale notar que o comando execve sobrescreve o codigo que estiver sendo executado no processo, por isso um filho usando
fork() é criado para o executar, para que o processo pai não seja sobrescrito e morto por consequencia.

A comunicação entre os dois processos ainda é feita utilizando dois pipes. O processo-pai envia a palavra a ser invertida
para o processo-filho pelo pipe pai->filho, que por sua vez coloca a palavra a ser invertida dentro do array de argumentos
do programa a ser invocado, inv, e então executa o comando.

O novo programa, agora executando o codigo contido em inv.c, recebe por agumento a palavra a ser invertida e a processa
e logo então escreve no pipe filho->pai a palavra invertida, para que ele, que está esperando a receba e possa imprimir.

Por ser um trabalho de matéria e por fim didáticos, foi soliciado que os programas fossem encerrados de formas diferentes
que possibilitam isso. Elas são a função exit() e a função kill(). Ambas terminam a execução de um processo. Como a função
exit() foi usada no outro programa, nesse teria que usar kill().

Para tanto, era necessario o pid (process id) do filho, então a primeira coisa que o filho quando começa a executar o codigo
de inv.c faz é passar seu pid para o pai.

O pai, que estava esperando o filho terminar utilizando a função wait() é avisado caso o filho seja interrompido, tanto
por acabar quanto por ser bloqueado. Como a intenção era usar kill() no filho, era melhor que ele fosse bloqueado, e por
isso a função sleep() é chamada, que bloqueia o proceso que a chamou pela quantidade de segundos passadade argumento.
Quando o filho, executando inv.c chama sleep(), o pai é avisado e vê que o filho esta demorando, e usa kill() nele com o pid
obtido anteriormente.

Como o filho já tinha terminado a inversão, ele lê a palavra no pipe filh->pai e imprime na tela.

Mais informações no código comentado.
