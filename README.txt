Implementação dos testes: 

Cada um dos testes tem o nome ao caso que se refere, ou são apenas testes aleatórios para
verficar se passa ou não.

- O test_div0 faz divisão por 0;
- O test_loop trava o programa em um loop infinito;
- O test_longo faz muito trabalho, mas eventualmente passa no teste 
(demora menos de 2s);
- O test_segmentation causa segmentation fault;
- O test_rapido é uma simples execução de um for que não faz nada;
- O test_lento é uma simples execução de um for que não faz nada, porém
com sleeps a cada iteração, dando a ideia de "lento";
- O test_rand gera um número aleatório e verifica a condição escolhida, 
esse teste pode passar ou falhar toda vez que rodado o programa.

Implementação runner:

Basicamente, o programa é iniciado e verifica os argumentos passados.

Caso não seja passado nenhum argumento, roda todos os testes, caso contrário, 
roda o teste especificado.

Para o teste especificado:

    - verifica se argc é igual a 2 ("./programa" e "teste a ser rodado");
    - se não for igual a dois (ou seja não passar argumento ou passar mais de 
    um argumento), todos os testes são rodados;
    - se não for igual a 2, apenas o teste passado como argumento:

        verifica-se a string argumento e a compara com o nome dos testes num for,
        executando apenas aquela que possui nome = argumento.

        Indica se o teste falhou, passou, time_out ou erro e retorna 0, indicando
        o fim do programa.

Rodando todos os testes: 
    São criados vários forks do programa principal, por meio de um for, 
    no qual cada processo filho irá rodar um teste diferente (filho1 roda test1,
    filho2 roda test2, etc.). Cada programa filho escreve a saída do programa em um
    arquivo temporário, cujo file descriptor é armazenado em um vetor de fds. 

    Cada teste, quando criado, pode-se definir o tempo no qual o teste deve completar
    sua execução pela chamada da função set_time_limit(f);

    As saídas dos processos filho padrão são:

        - 0: completou o teste e passou, printando [PASS];
        - 1: completou o teste e falhou, printando [FAIL];

    Além disso, é indicado o tempo levado para a execução do teste, que é impresso
    logo após o status/mensagem de por que falhou.

    ale destacar ainda, que no processo pai, os pids dos processos filhos são armazenados em um 
    vetor de filhos.

    O processo pai espera o término de cada um dos seus filhos, e para cada filho:

        - espera finalização de sua execução e obtem seu status.
        - Caso possui algum erro, escreve no arquivo temporario desse filho (por isso
        o vetor com os fds) o erro.
        - Caso não possua nenhum erro, o programa recebe o valor de saída (0 ou 1) e realiza ações
        de acordo:
            - se recebeu 0, o teste rodado por aquele filho passou, incrementando uma váriavel de passe;
            - se recebeu 1, não faz nada, afinal o teste falhou.

    Após agregar todos os resultados em seus respectivos arquivos temporários, o programa lê cada um desses
    arquivos e imprime no terminal, de maneira ordenada, os resultados dos testes obtidos anteriormente:

        teste_*: prints...
        teste_*: [STATUS]

    Ainda, cada um dos status é impresso com uma cor diferente:
        [PASS]: verde;
        [FAIL]: magenta;
        [ERRO]: vermelho;
        [TIME]: ciano.