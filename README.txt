Implementação dos testes: 

Cada um dos testes tem o nome ao caso que se refere, ou são apenas testes aleatórios para
verficar se passa ou não.

- O test1 realiza print e retorna 0, sempre deve passar;
- O test2 realiza alguns asserts, os primeiros passam, até chegar no primeiro 
que falha. Tudo que está abaixo da linha que falhou não é rodado;
- O test3 só demonstra o uso de um test_printf seguido de um test_assert que passa;
- O test_div0 faz divisão por 0;
- O test_loop trava o programa em um loop infinito;
- O test_long faz muito trabalho, mas eventualmente passa no teste 
(demora menos de 2s);
- O test_segmentation causa segmentation fault;
- O fast_test é uma simples execução de um for que não faz nada;
- O slow_test é uma simples execução de um for que não faz nada, porém
com sleeps a cada iteração, dando a ideia de "lento";
- O test_prints realiza 50 prints (bom para demonstrar saída organizada dos prints);
- O test_rand gera um número aleatório e verifica a condição escolhida, 
esse teste pode passar ou falhar toda vez que rodado o programa.

Os testes estão no arquivo example.c. Para executá-los, basta compilar o programa e rodá-lo:

    gcc example.c -o example && ./example

Implementação runner:

Basicamente, o programa é iniciado e verifica os argumentos passados.

Caso não seja passado nenhum argumento, roda todos os testes, caso contrário, 
roda o teste especificado.

Para o teste especificado:

    - verifica se argc é igual a 2 ("./programa" e "teste a ser rodado");
    - se não for igual a dois (ou seja não passar argumento ou passar mais de 
    um argumento), todos os testes são rodados;
    - se for igual a 2, apenas o teste passado como argumento será executado:

        verifica-se a string argumento e a compara com o nome dos testes em um for,
        executando apenas aquela que possui nome = argumento.

        Indica se o teste falhou, passou, time_out ou erro e retorna 0, indicando
        o fim do programa.

Para rodar o teste especificado, basta compilar o programa e passar qual teste será
executado:

    gcc example.c -o example && ./example test_rand

Rodando todos os testes: 
    São criados vários forks do programa principal, por meio de um for, 
    no qual cada processo filho irá rodar um teste diferente (filho1 roda primeiroteste de test_list, 
    filho 2 o segundo, etc.). Cada programa filho escreve a saída do programa em um
    arquivo temporário, cujo file descriptor é armazenado em um vetor de file descriptors (fds). 

    Cada teste, quando criado, pode-se definir o tempo no qual o teste deve completar
    sua execução pela chamada da função set_time_limit(f);

    As saídas dos processos filho padrão são:

        - 0: completou o teste e passou, printando [PASS];
        - 1: completou o teste e falhou, printando [FAIL];

    Além disso, é indicado o tempo levado para a execução do teste, que é impresso
    logo após o status/mensagem de por que falhou.

    Vale destacar ainda, que no processo pai, os pids dos processos filhos são armazenados em um 
    vetor de filhos.

    =====================================================================================================================

    Se for pressionado ctrl+c, aparece uma mensagem pedindo confirmação se o usuário deseja sair:
    Quando pressionado, cada processo filho recebe um SIGSTOP (todos os filhos possuem o mesmo handler, 
    que irá lançar esse sinal para cada um). O processo pai, por sua vez, é responsável por mostrar a mensagem de 
    confirmação e executar ações de acordo.

    No caso em que é digitado [y], todos os processos filhos recebem um sinal SIGCONT e em seguida SIGTERM, 
    sendo assim finalizados caso ainda estivessem rodando.

    No caso em que é digitado [n] (ou qualquer outra coisa que não seja
    [y]), todos os processos filhos recebem um sinal SIGCONT, e continuam sua execução.

    Em seguida, o fluxo do processo pai continua. Se ctrl+c não for pressionado, a parte anterior é irrelevante...
    
    Obs.: no caso em que é digitado algo diferente de [y], o teste que demora mas termina (test_long) está recebendo 
    alarme na maioria das vezes, poucos foram os casos em que ele passou quando eu testava...
    
    =====================================================================================================================

    O processo pai espera o término de cada um dos seus filhos, e para cada filho:

        - espera finalização de sua execução e obtem seu status.
        - Caso possui algum erro, escreve no arquivo temporario desse filho o erro
        (por isso o vetor com os fds).
        - Caso não possua nenhum erro, o programa recebe o valor de saída (0 ou 1) e realiza ações
        de acordo:
            - se recebeu 0, o teste rodado por aquele filho passou, incrementando uma váriavel de passe;
            - se recebeu 1, não faz nada, afinal o teste falhou.

    Após agregar todos os resultados em seus respectivos arquivos temporários, o programa lê cada um desses
    arquivos e imprime no terminal, de maneira ordenada, os resultados dos testes obtidos anteriormente:

        test_*: prints...
        test_*: [STATUS]

    Ainda, cada um dos status é impresso com uma cor diferente:
        [PASS]: verde;
        [FAIL]: magenta;
        [ERRO]: vermelho;
        [TIME]: ciano;
        [STOP]: azul.
