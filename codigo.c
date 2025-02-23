// KAMILY SILVA ANDRADE CRISPIM - MATRICULA - 242009945

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variáveis globais
FILE *arquivo_numeros;
int PONTUACAO_FINAL = 0;
int altura[5] = {0,0,0,0,0};
int matriz_jogo[10][5] = {0};
char nome[20];
int MARTELOS = 0; 
const int PONTUACAO_PARA_MARTELO = 10000; 
int ULTIMA_PONTUACAO_MARTELO = 0; 
int nro = 0;
int nro_seguinte = 0;

//struct jogador
struct dadosjogador {
    char nickname[20];
    int pontuacao;
};


// verifica quantos martelos tem
void VERIFICAR_MARTELO() {
    // Verifica se o jogador ganhou um martelo
    if (PONTUACAO_FINAL >= ULTIMA_PONTUACAO_MARTELO + PONTUACAO_PARA_MARTELO) {
        MARTELOS++; // Incrementa o número de martelos
        ULTIMA_PONTUACAO_MARTELO += PONTUACAO_PARA_MARTELO; // Atualiza a última pontuação em que o jogador ganhou um martelo
    }
}


//funcao de ordenar ranking

void ORDENAR_RANKING(struct dadosjogador ranking[], int n) {
    int i, j;
    struct dadosjogador aux;

    for (j = 0; j < n; j++) {
        for (i = 0; i < n - 1; i++) {
            if (ranking[i].pontuacao < ranking[i + 1].pontuacao) { // Ordena decrescente
                aux = ranking[i];
                ranking[i] = ranking[i + 1];
                ranking[i + 1] = aux;
            }
        }
    }
}

//funcao de elr ranking
void EXIBIR_RANKING() {
    FILE *arquivo_ranking;
    struct dadosjogador ranking[100]; // Vetor para armazenar os dados do ranking
    int n = 0;

    // Abre o arquivo para leitura
    arquivo_ranking = fopen("ranking.bin", "rb");
    if (arquivo_ranking == NULL) {
        printf("Nenhum ranking disponível.\n");
        printf("(Pressione Enter para retornar)\n");
        getchar();
        return;
    }

    // Lê os dados do arquivo e armazena no vetor
    while (fread(&ranking[n], sizeof(struct dadosjogador), 1, arquivo_ranking) == 1) {
        n++;
    }

    // Fecha o arquivo
    fclose(arquivo_ranking);

    // Ordena o ranking
    ORDENAR_RANKING(ranking, n);

    // Exibe o ranking ordenado
    printf("\n=== RANKING ===\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s - %d pontos\n", i + 1, ranking[i].nickname, ranking[i].pontuacao);
        
    }
    printf("\nPressione Enter para voltar ao menu.\n");
    getchar();
}


void SALVAR_RANKING() {
    FILE *arquivo_ranking;
    struct dadosjogador jogador;


    // Abre o arquivo em modo append (adiciona ao final)
    arquivo_ranking = fopen("ranking.bin", "ab");
    if (arquivo_ranking == NULL) {
        // Se o arquivo não existir, cria um novo
        arquivo_ranking = fopen("ranking.bin", "wb");
        if (arquivo_ranking == NULL) {
            printf("Erro ao criar o arquivo de ranking!\n");
            return;
        }
    }

    // Preenche a struct com os dados do jogador
    strncpy(jogador.nickname, nome, 19); // Copia até 19 caracteres
    jogador.nickname[19] = '\0'; // Garante que a string seja terminada
    jogador.pontuacao = PONTUACAO_FINAL;


    // Escreve a struct no arquivo
    if (fwrite(&jogador, sizeof(struct dadosjogador), 1, arquivo_ranking) != 1) {
        printf("Erro ao salvar os dados no ranking!\n");
    }


    // Fecha o arquivo
    if (arquivo_ranking != NULL) {
        fclose(arquivo_ranking);
    }
}

//funcao de pegar o numero do arquivo
int LER_PROXIMO_NUMERO() {
    int numero;
    if (fscanf(arquivo_numeros, "%d", &numero) == 1) {
        return numero;  // Retorna o número lido
    } else {
        return -1;  // Retorna -1 quando o arquivo acabar
    }
}

//funcao para zerar a altura e a matriz do jogo
void ZERANDO(){

    //zerando nome
    for(int  i = 0 ; i < 30 ;i++){

        nome[i]= '\0';
    }
   
    //zerando altura
    for(int i = 0 ; i < 5 ; i++){

        altura[i] = 0;
    }

    //zerando amtriz do jogo
    for(int  i = 0 ; i < 10 ;i++){
        for(int  j = 0 ; j < 5 ; j++){

            matriz_jogo[i][j] = 0;

        }
    }

    //zerando as outras variaveis
    PONTUACAO_FINAL = 0;

    MARTELOS = 0; 
    const int PONTUACAO_PARA_MARTELO = 10000; 
    ULTIMA_PONTUACAO_MARTELO = 0; 



}


//funcao que verifica em que parte da grid esta a posicao analisada
int POSICAO_GRID(int i, int j){


    if (i == 0) {  // linha mais baixa
           if (j == 0) return 5;  // canto inf esquerdo
           if (j == 4) return 6;  // canto inf direito
           return 1;  // Linha baixa, sem ser canto
   
   
       } else if (i == 9) {  // linha mais alta
           if (j == 0) return 7;  // canto sup esquerdo
           if (j == 4) return 8;  // canto sup direito
           return 2;  // Linha alta, sem ser canto
   
   
       } else if (j == 0) return 3;  // coluna mais à esquerda
       else if (j == 4) return 4;  // coluna mais à direita
       return 9;  // Caso do meio
   }
   

   //funcao para somar os blocos
int VIZINHOS(int i, int j){
       //eu passo por cada posicoes vendo os vizinhos
       //tem q  ver se a posicao na esta nos limitestes
        int alterou = 0;

        if(matriz_jogo[i][j] == 0){
        return 0;
       }
       

       int pgrid = POSICAO_GRID(i , j );
       int combo = 1;
   //depois de saber a posicao do elemento no espaco fisico da matriz, podemos ver os vizinhos
   
       if( (pgrid == 3)|| (pgrid == 7)|| (pgrid == 5)|| (pgrid == 9)|| (pgrid == 2)|| (pgrid == 1)){

           //verifica a dir
           if(matriz_jogo[i][j+1] == matriz_jogo[i][j]){

               combo*= 2;
               matriz_jogo[i][j+1] = 0;

               if(altura[j+1] == 1){
                altura[j+1] = 0;
               }

               if(altura[j+1] == 10){

                altura[j+1] = 9;

                }
                   
           }
   
   
       }
   
       if((pgrid == 2)|| (pgrid == 7)|| (pgrid == 3)|| (pgrid == 8)|| (pgrid == 9)|| (pgrid == 4)){

                   //verifica a embaixo
                   if(matriz_jogo[i-1][j] == matriz_jogo[i][j]){

                       combo*= 2;
                       matriz_jogo[i-1][j] = 0;
                       
                   }
   
       }
   
       if((pgrid == 8)|| (pgrid == 4)|| (pgrid == 6)|| (pgrid == 9)|| (pgrid == 2)|| (pgrid == 1)){
   
       
   
                   //verifica esq
                   if(matriz_jogo[i][j-1] == matriz_jogo[i][j]){
               
   
                       combo*= 2;
                       matriz_jogo[i][j-1] = 0;

                       if(altura[j-1] == 1){

                altura[j-1] = 0;

                        }

                        if(altura[j-1] == 10){

                            altura[j-1] = 9;
            
                            }
                        
                       
                               }
                   }
     
                
       matriz_jogo[i][j] *= combo;
       PONTUACAO_FINAL += matriz_jogo[i][j];
       VERIFICAR_MARTELO();

        return alterou;
       }


//funcao para cair os blocos
void GRAVIDADE(){

for(int  j = 0 ; j < 5 ; j++){
    int caiu;
    do{
       
        caiu = 0;


        for(int i = 0 ; i < 9 ; i++){


        if((matriz_jogo[i][j] == 0) && (matriz_jogo[i+1][j] != 0)){


            matriz_jogo[i][j] = matriz_jogo[i+1][j];
            matriz_jogo[i+1][j] = 0;
           
       
           
            caiu = 1;
        }
        }



        if(caiu){


            altura[j]--;

        }

    }while(caiu == 1);

}

}

//funcao que verifica se a vizinho precisa ser rodada de novo
void REAC_CADEIRA(){

int m;
do{
m = 0;
 for(int i = 0 ; i < 10 ; i++){
         for(int j = 0 ; j < 5 ; j++){

             m = VIZINHOS(i, j);

            }
             }

GRAVIDADE();

}while (m > 0 );


}


//funcao para ver se o jogador perdeu
int GAME_OVER(int nro){

    //se todas as colunas estiverem na altura max, e nro nao for igual a nenhum numero do topo, GAME OVER
    for (int i = 0; i < 5; i++) {  
        if (altura[i] < 10 || matriz_jogo[9][i] == nro) {  
            return 0; // pode continuar o jogo
        }
    }
    return 1; // quer dizer que todas as colunas estao cheias e nao ha mais possibilidade de jogo. Perdeu.



}


//funcao do Ranking
void RANKING(){
   
    EXIBIR_RANKING();
}

// Função que faz o bloco "cair" no lugar certo
int COLOCANDO_BLOCO(int nro, int coluna, int pos_preenchi ) {

int h = altura[coluna -1];
//vendo se ainda cabe blocos na coluna

//se a coluna inteira estiver preenchida
if (h == 10) {  

        if (matriz_jogo[9][coluna - 1] == nro) {  // Se nro for igual ao elemento do topo
           
            matriz_jogo[9][coluna-1] += nro;

            return 1;

        } else {  // Se não puder somar
            printf("A coluna %d está cheia! Pressione Enter para escolher outra.\n", coluna);
            getchar();
            return 0;
        }

    } else {  // Se a coluna não estiver cheia, coloca o bloco normalmente
        matriz_jogo[h][coluna - 1] = nro;
        altura[coluna - 1]++;  // Atualiza a altura da coluna

        int lixo = VIZINHOS(h, coluna - 1);
        GRAVIDADE();
       
        REAC_CADEIRA();

        return 1;
       
    }        

}

// Função para pedir o nome do usuário
void NOME_USUARIO(char nome[]) {
    printf("\n");
    printf("Bem vindo(a) ao jogo DROP MERGE de APC!!!\n");
    printf("\n");
    printf("-Informe seu nickname:   \n");
    printf("\n");
    scanf(" %[^\n]s", nome);
    getchar();
    printf("\n");
    printf("Bom jogo %s!\n", nome);
    printf("\n");
    printf("(pressione Enter para continuar)\n");
    printf("\n");
    getchar();
}

// Função para receber a coluna digitada pelo usuário
int COLUNA_DIGITADA() {
    int coluna;
    printf("Insira a coluna em que o bloco cairá (Ou '0' para ir para o menu inicial): \n");
    scanf("%d", &coluna);
    getchar();
   
    if (coluna >= 1 && coluna <= 5) {
        return coluna;  // Retorna a coluna válida
    } else if (coluna == 0) {
        return 0;
    } else {
        printf("Opção inválida, tente novamente!\n");
        return COLUNA_DIGITADA();  // Chama a função novamente até obter entrada válida
    }
}

// Função para limpar a tela
void LIMPA_TELA() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//funcao de usar o martelo

void USAR_MARTELO() {
    int linha, coluna;
    char continuar;

    while (MARTELOS > 0) {
       
        printf("*****Digite a linha e a coluna da célula que deseja martelar*****\n ");
        printf("(Você tem %d martelos)\n ", MARTELOS);
        scanf("%d %d", &linha, &coluna);
        getchar(); // Limpa o buffer do teclado

        // Verifica se a linha e a coluna são válidas
        if (linha < 1 || linha > 10 || coluna < 1 || coluna > 5) {
            printf("Posição inválida! Tente novamente.\n");
            continue; // Volta ao início do loop
        }

        // Converte para índices da matriz 
        linha--;
        coluna--;

        // Elimina a célula
        matriz_jogo[linha][coluna] = 0;
        MARTELOS--; // Reduz o número de martelos

        // Aplica a gravidade para fazer as células acima caírem
        GRAVIDADE();

        // Verifica combinações de vizinhos iguais
        REAC_CADEIRA();

         // Exibe o grid atualizado
         LIMPA_TELA();
         printf("           +----+  +----+\n");
         printf("           |%4d|  |%4d|\n", nro, nro_seguinte);
         printf("           +----+  +----+\n");
         printf("Martelos: %d\n", MARTELOS); // Exibe o número de martelos
 
         for (int i = 9; i >= 0; i--) {
             printf("  +----+----+----+----+----+\n  |");
             for (int j = 0; j < 5; j++) {
                 if (matriz_jogo[i][j] != 0)
                     printf("%4d|", matriz_jogo[i][j]);
                 else
                     printf("    |");
             }
             printf("\n");
         }
 
         printf("  +----+----+----+----+----+\n");
         printf("  +----+----+----+----+----+\n");
         printf("  |  1 |  2 |  3 |  4 |  5 |\n");
         printf("  +----+----+----+----+----+\n\n");




        printf("Célula martelada com sucesso!\n");
        printf("(Pressione Enter)\n");
        getchar();
    }

    if (MARTELOS == 0) {
        printf("Você não tem mais martelos disponíveis!\n");
        printf("(Pressione Enter)\n");
        getchar();
    }
}

// ve se eh game over msm
void QUASE_GAME_OVER() {
    while (GAME_OVER(nro)) { // Enquanto o grid estiver cheio
        if (MARTELOS > 0) {
            
                USAR_MARTELO(); // Permite que o jogador use martelos
           

        } else {
            
            printf("Você não tem martelos disponíveis. Fim de jogo!\n");
            break; // Sai do loop e encerra o jogo
        }
    }
}


// Função para mostrar o tabuleiro e capturar entrada do usuário
void TABULEIRO(int pos_preenchi) {

// Abre o arquivo
arquivo_numeros = fopen("numeros.txt", "r");
if (arquivo_numeros == NULL) {
    printf("Erro ao abrir o arquivo de números!\n");
    return;
}

// Lê os dois primeiros números do arquivo
 nro = LER_PROXIMO_NUMERO();
 nro_seguinte = LER_PROXIMO_NUMERO();

  while (1) {
        LIMPA_TELA();
        printf("           +----+  +----+\n");
        printf("           |%4d|  |%4d|\n", nro, nro_seguinte);
        printf("           +----+  +----+\n");
        printf("Martelos: %d\n", MARTELOS); 

        for (int i = 9; i >= 0; i--) {
            printf("  +----+----+----+----+----+\n  |");
            for (int j = 0; j < 5; j++) {
                if (matriz_jogo[i][j] != 0)
                    printf("%4d|", matriz_jogo[i][j]);
                else
                    printf("    |");
            }
            printf("\n");
        }

        printf("  +----+----+----+----+----+\n");
        printf("  +----+----+----+----+----+\n");
        printf("  |  1 |  2 |  3 |  4 |  5 |\n");
        printf("  +----+----+----+----+----+\n\n");

   // Verifica se o jogador ganhou (arquivo acabou)
   if (nro == -1 || nro_seguinte == -1) {
    printf("\n********** PARABÉNS! VOCÊ GANHOU! **********\n");
    printf("\n");
    printf("Todos os números do arquivo foram usados! Pressione Enter para voltar ao menu.\n");
    printf("\n");
    getchar();
    fclose(arquivo_numeros);  // Fecha o arquivo
    SALVAR_RANKING(); // salva pontuacao
    return;  // Retorna ao menu principal
}

if (GAME_OVER(nro)) {

    QUASE_GAME_OVER(); 
    if(GAME_OVER(nro)){

    printf("\n********** GAME OVER!!!!! **********\n");
    printf("\n");
    printf("Aperte Enter para voltar ao menu.\n");
    printf("\n");
    getchar();
    fclose(arquivo_numeros);  // Fecha o arquivo
    //SALVANDO RANKING
    SALVAR_RANKING();
    
   
    return;
}
}

int coluna = COLUNA_DIGITADA();
if (coluna == 0) {
    LIMPA_TELA();
    fclose(arquivo_numeros);  // Fecha o arquivo
    return;
}

int bloco_colocado = COLOCANDO_BLOCO(nro, coluna, pos_preenchi);
GRAVIDADE();

// Atualiza os números para a próxima jogada
if(bloco_colocado){
nro = nro_seguinte;
nro_seguinte = LER_PROXIMO_NUMERO();
  }

}
}


// Função que mostra o menu
int MENU() {
    LIMPA_TELA();
    int opcao;

    printf("**********************************\n");
    printf("*       Escolha uma opcao:       *\n");
    printf("*                                *\n");
    printf("* 1 - Jogar                      *\n");
    printf("* 2 - Configuracoes              *\n");
    printf("* 3 - Instrucoes                 *\n");
    printf("* 4 - Ranking                    *\n");
    printf("* 5 - Sair                       *\n");
    printf("*                                *\n");
    printf("**********************************\n");

    scanf("%d", &opcao);
    getchar();

    if (opcao >= 1 && opcao <= 5) {
        return opcao;
    } else {
        printf("Opcao invalida! Tente novamente!\n");
        LIMPA_TELA();
        return MENU();
    }
}

// Função para exibir o menu de configurações
void MENU_CONFIGURACOES() {
    int opcao;
    char confirmacao;

    do {
        LIMPA_TELA();
        printf("******CONFIGURAÇÕES ******\n");
        printf("1 - Zerar Ranking     \n");
        printf("2 - Modo Dificuldade     \n");
        printf("3 - Voltar ao Menu Principal  \n");
        printf("\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado

        switch (opcao) {
            case 1:
                // Zerar Ranking
                printf("Tem certeza que deseja zerar o ranking? (S/N): ");
                scanf(" %c", &confirmacao); // Espaço antes do %c para ignorar espaços em branco
                getchar(); // Limpa o buffer do teclado

                if (confirmacao == 'S' || confirmacao == 's') {
                    if (remove("ranking.bin") == 0) {
                        printf("Ranking zerado com sucesso!\n");
                        printf("(Pressione Enter para retornar)\n");
                        getchar();
                    } else {
                        printf("Erro ao zerar o ranking. O arquivo pode não existir.\n");
                        printf("(Pressione Enter para retornar)\n");
                        getchar();
                    }
                } else {
                    printf("Operação cancelada. O ranking não foi alterado.\n");
                    printf("(Pressione Enter para retornar)\n");
                        getchar();
                }
                break;

            case 2:
           
                //apenas retorna ao menu de configurações
                break;

            case 3:
            
            //vai para o menu principal
                break;

            default:
          
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (opcao != 3); // Repete até o usuário escolher "Voltar ao Menu Principal"
}

// Função principal
int main() {
    while (1) {
        int menu = MENU();

        if (menu == 3) {
            printf("***     Instrucoes: O objetivo do jogo eh empilhar os blocos para combinar aqueles com valores iguais.  Quando isso acontece, os valores das pecas combinadas se somam, e a nova peca resultante representa a pontuacao ganha pelo jogador.  O desafio eh alcancar a maior pontuacao possivel antes que todos os espacos do tabuleiro estejam preenchidos, pois, caso isso ocorra, o jogador perde.   O jogador zera o jogo ao conseguir preencher toda a grade com as pecas fornecidas, alcancando a pontuacao maxima. Apos a pontuacao **** o jogador ganha um martelo para retirar uma celula casoa grid fique cheia.    ***\n");
            printf("\n");
            printf("(Pressione Enter para voltar ao menu.)\n");
            getchar();

        } else if (menu == 1) {
            int pos_preenchi = 0;
           
            ZERANDO();
           
            for(int i = 0 ; i < 5 ; i++){

                altura[i] = 0;
            }


            NOME_USUARIO(nome);
            TABULEIRO( pos_preenchi);

        } else if (menu == 4) {
           
            RANKING();
            
         
        } else if (menu == 5) {
            printf("Programa encerrado.\n");
            break;

        }else if(menu == 2){
            MENU_CONFIGURACOES();
        }

    }

    return 0;
}

