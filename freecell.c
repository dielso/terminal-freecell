//dielson s. de o.  junior. // 2021004346 // freecell // ecop12

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

enum naipes{copas = 0, ouros, paus, espadas};

typedef struct t{ // define a estrutura da carta num monte
    int carta[2];
    struct t *prox;
}tCarta;

// define a qtd de cartas em cada monte
int cont_temp[4]={0,0,0,0};
int cont_mesa[8]={7,7,7,7,6,6,6,6};
int cont_naipe[4]={0,0,0,0};

//cria os topos de cada monte
tCarta *mesa[8];
tCarta *naipe[4];
tCarta *temp[4];

void SetColor(int ForgC)
 {
     WORD wColor;

      HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO csbi;

                       //We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
                 //Mask out all but the background attribute, and add in the forgournd     color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
 }


void inicializa() //coloca como vazia todos os montes
{
    int i;
    for(i=0;i<8;i++)
    {
        mesa[i] = NULL;
        if(i>3) continue;
        naipe[i] = NULL;
        temp[i] = NULL;
    }
}

void gerarBaralho() // cria cartas e as distribui entre os montes naipes
{
    int i, j;
    tCarta *nova;

    for(i=0; i<13; i++) //cria cartas
    {
        for(j=copas;j<=espadas;j++)
        {
            nova = (tCarta*) malloc(sizeof (tCarta));
            nova->carta[0] = i;
            nova->carta[1] = j;
            nova->prox = naipe[j]; //guarda a carta no naipe
            naipe[j] = nova;
        }
    }
}

void embaralha() // distribui aleatoriamente cartas dos naipes para a mesa, e as devolve também aleatoriamente
{
    srand(time(NULL));
    int i , j;
    int esvazia;

    tCarta *ultima = NULL;

    for(i=0; i<13; i++)
    {
        for(j=copas;j<=espadas;j++) // retira as cartas do naipe aleatoriamente
        {
            if(naipe[j] == NULL) continue;

            esvazia = rand()%8;

            ultima = mesa[esvazia];

            mesa[esvazia] = naipe[j];
            naipe[j] = naipe[j]->prox;
            mesa[esvazia]->prox = ultima;
        }
    }
    j=0;
    ultima = NULL;

    while(1) // repõe aleatoriamente as cartas no naipe
    {
        if(mesa[j] == NULL){
            j++;
            if(j>7) break;
            continue;
        }
        esvazia = rand()%4;

        ultima = naipe[esvazia];

        naipe[esvazia] = mesa[j];
        mesa[j] = mesa[j]->prox;
        naipe[esvazia]->prox = ultima;
    }
}

void distribuirMesa() // chama a embaralha() e distribui as cartas na mesa
{
    embaralha();
    embaralha();
    embaralha();
    srand(time(NULL));
    int i , j,n;
    int esvazia;

    tCarta *ultima = NULL;

    j=0;
    n=7;
    while(n--) //retira as cartas do naipe e as coloca nas mesas até que estejam vazios
    {
        for(i=0;i<8;i++)
        {
            if(naipe[j] == NULL){
                j++;
                if(j>3) break;
            }

            esvazia = i;

            ultima = mesa[esvazia];
            mesa[esvazia] = naipe[j];
            naipe[j] = naipe[j]->prox;
            mesa[esvazia]->prox = ultima;
        }
    }
}

void imprimir() // imprime os montes
{
    tCarta *atual;
    int i, pico = 0;
    printf("[TEMP]:\t\t");
    for(i=0;i<4;i++) // imprime o topo do monte temp
    {
        if(temp[i]!= NULL){
            if(temp[i]->carta[1] == ouros || temp[i]->carta[1]==copas) SetColor(4);
            printf("%d:[%d %c] ", i+1, temp[i]->carta[0]+1, temp[i]->carta[1]+3);
        }
        else
            printf("%d:[   ] ", i+1);
        SetColor(15);
    }
    printf("\n\n");

    for(i=0;i<4;i++)
    {
        printf("[NAIPE]: %d:\t", i+1);
        atual = naipe[i];
        while(atual!=NULL) // imprime o monte naipe
        {

            if(atual!= NULL){
                if(atual->carta[1] == ouros || atual->carta[1]==copas) SetColor(4);
                printf("%[%2d %c]", atual->carta[0]+1, atual->carta[1]+3);
            }
            atual = atual->prox;
            SetColor(15);
        }
         printf("\n");
    }
    printf("\n");


    for(i=0;i<8;i++)
    {
        printf("[MESA] %d:\t", i+1);
        atual = mesa[i];
        while(atual!=NULL) // imprime o monte mesa
        {
            if(atual!= NULL){
                if(atual->carta[1] == ouros || atual->carta[1]==copas) SetColor(4);
                printf("%[%2d %c]", atual->carta[0]+1, atual->carta[1]+3);
            }
            atual = atual->prox;
            SetColor(15);
        }
         printf("\n");
    }
    printf("\n");

}

int moveMesaTemp()
{
    int pos1=1, pos2=2;
    tCarta *meio;
    printf("digite a mesa e o temporario: ");
    scanf(" %d %d", &pos1, &pos2);
    pos1--; pos2--;

    if(pos1>7 || pos2>3 || mesa[pos1]==NULL) return 0; //verifica se a entrada é valida

    //tira e bota

    meio = mesa[pos1];
    mesa[pos1] = mesa[pos1]->prox; // tira da mesa

    meio->prox = temp[pos2]; // salva o proximo
    temp[pos2] = meio; // salva a carta

    cont_temp[pos2]++;
    cont_mesa[pos1]--;

    return 1;
}

int moveTempMesa()
{
    int pos1=1, pos2=2;
    tCarta *meio;
    printf("digite o temporario e a mesa: ");
    scanf(" %d %d", &pos1, &pos2);
    pos1--; pos2--;

    if(mesa[pos2] != NULL){ // se a mesa não estiver vazia e...
        if(pos1>3 || pos2>7 || temp[pos1]==NULL || cont_mesa[pos2]==7) return 0; // se a entrada for invalida -> fora

        if((temp[pos1]->carta[0]+1 != mesa[pos2]->carta[0]) || // se a carta do temp for maior que a carta da mesa -> fora
           !(((temp[pos1]->carta[1] == paus || temp[pos1]->carta[1] == espadas)&&
             (mesa[pos2]->carta[1] == copas || mesa[pos2]->carta[1] == ouros))||
           ((temp[pos1]->carta[1] == ouros || temp[pos1]->carta[1] == copas)&&
             (mesa[pos2]->carta[1] == paus || mesa[pos2]->carta[1] == espadas)))) return 0; // se os naipes forem de cores iguais -> fora
    }
    // remove do temp, adiciona à mesa
    meio = temp[pos1];
    temp[pos1] = temp[pos1]->prox;

    meio->prox = mesa[pos2];
    mesa[pos2] = meio;

    cont_mesa[pos2]++;
    cont_temp[pos1]--;

    return 1;
}

int moveMesaNaipe()
{
    int pos1=1, pos2=2;
    tCarta *meio;
    printf("digite a mesa e o naipe: ");
    scanf(" %d %d", &pos1, &pos2);
    pos1--; pos2--;

    if(pos1>7 || pos2>3 || mesa[pos1]==NULL || cont_naipe[pos2]==13) return 0; // verifica se a entrada foi valida

    if(naipe[pos2] != NULL){
        if((mesa[pos1]->carta[0] != ((naipe[pos2]->carta[0])+1) || //verifica se a carta é a próxima
           mesa[pos1]->carta[1] != naipe[pos2]->carta[1])) return 0; // verifica se o naipe é o mesmo
    }
    else if(mesa[pos1]->carta[0] != 0) return 0;

    //tira d'um bota n'outro
    meio = mesa[pos1];
    mesa[pos1] = mesa[pos1]->prox;

    meio->prox = naipe[pos2];
    naipe[pos2] = meio;

    cont_naipe[pos2]++;
    cont_mesa[pos1]--;

    return 1;
}

void destroi() // libera o espaço alocado
{
    tCarta *atual, *destroi;
    int i;
    for(i=0;i<8; i++){
        atual = mesa[i];
        while(atual!=NULL){
            destroi = atual;
            atual = atual->prox;
            free(destroi);
        }
    }
    for(i=0;i<4; i++){
        atual = naipe[i];
        while(atual!=NULL){
            destroi = atual;
            atual = atual->prox;
            free(destroi);
        }
    }
    for(i=0;i<4; i++){
        atual = temp[i];
        while(atual!=NULL){
            destroi = atual;
            atual = atual->prox;
            free(destroi);
        }
    }
}

int moveNaipeTemp()
{
    int pos1=1, pos2=2;
    tCarta *meio;
    printf("digite o naipe e o temporario: ");
    scanf(" %d %d", &pos1, &pos2);
    pos1--; pos2--;

    if(pos1>3 || pos2>3 || naipe[pos1]==NULL) return 0; //verifica se a entrada é valida
    //tira e bota

    meio = naipe[pos1];
    naipe[pos1] = naipe[pos1]->prox;

    meio->prox = temp[pos2];
    temp[pos2] = meio;

    cont_temp[pos2]++;
    cont_naipe[pos1]--;

    return 1;
}

int moveTempNaipe()
{
    int pos1=1, pos2=2;
    tCarta *meio;
    printf("digite o temporario e o naipe: ");
    scanf(" %d %d", &pos1, &pos2);
    pos1--; pos2--;

    if(pos1>3 || pos2>3 || temp[pos1]==NULL || cont_naipe[pos2]==13) return 0; // verifica se a entrada foi valida

    if(naipe[pos2] != NULL){
        if((temp[pos1]->carta[0] != ((naipe[pos2]->carta[0])+1) || //verifica se a carta é a próxima
           temp[pos1]->carta[1] != naipe[pos2]->carta[1])) return 0; // verifica se o naipe é o mesmo
    }
    else if(temp[pos1]->carta[0] != 0) return 0;

    //tira d'um bota n'outro
    meio = temp[pos1];
    temp[pos1] = temp[pos1]->prox;

    meio->prox = naipe[pos2];
    naipe[pos2] = meio;

    cont_naipe[pos2]++;
    cont_temp[pos1]--;

    return 1;
}

int moveNaipeMesa()
{
    int pos1=1, pos2=2;
    tCarta *meio;
    printf("digite o naipe e a mesa: ");
    scanf(" %d %d", &pos1, &pos2);
    pos1--; pos2--;

    if(mesa[pos2] != NULL){ // se a mesa não estiver vazia e...
        if(pos1>3 || pos2>7 || naipe[pos1]==NULL || cont_mesa[pos2]==7) return 0; // se a entrada for invalida -> fora

        if((naipe[pos1]->carta[0]+1 != mesa[pos2]->carta[0]) || // se a carta do temp for maior que a carta da mesa -> fora
           !(((naipe[pos1]->carta[1] == paus || naipe[pos1]->carta[1] == espadas)&&
             (mesa[pos2]->carta[1] == copas || mesa[pos2]->carta[1] == ouros))||
           ((naipe[pos1]->carta[1] == ouros || naipe[pos1]->carta[1] == copas)&&
             (mesa[pos2]->carta[1] == paus || mesa[pos2]->carta[1] == espadas)))) return 0; // se os naipes forem de cores iguais -> fora
    }
    // remove do naipe, adiciona à mesa
    meio = naipe[pos1];
    naipe[pos1] = naipe[pos1]->prox;

    meio->prox = mesa[pos2];
    mesa[pos2] = meio;

    cont_mesa[pos2]++;
    cont_naipe[pos1]--;

    return 1;
}

int moveMesaMesa()
{
    int i, pos1=1, pos2=2, qtd =0, op;
    tCarta *meio, *maior;
    printf("(1) uma ou (2) varias? ");
    scanf(" %d", &op);

    if(op == 1){
        printf("digite a mesa origem e a mesa destino: ");
        scanf("%d %d", &pos1, &pos2);
        pos1--; pos2--;

        if(pos1>7 || pos2>7 || mesa[pos1]==NULL) return 0; // se a entrada for invalida -> fora

        if(mesa[pos2] != NULL){ // se a mesa destino não estiver vazia e...
            if((mesa[pos1]->carta[0]+1 != mesa[pos2]->carta[0]) || // se a carta da origem for maior ou igual a carta do destino -> fora
               !(((mesa[pos1]->carta[1] == paus || mesa[pos1]->carta[1] == espadas)&&
                 (mesa[pos2]->carta[1] == copas || mesa[pos2]->carta[1] == ouros))||
               ((mesa[pos1]->carta[1] == ouros || mesa[pos1]->carta[1] == copas)&&
                 (mesa[pos2]->carta[1] == paus || mesa[pos2]->carta[1] == espadas)))) return 0; // se os naipes forem de cores iguais -> fora
        }
        // remove da mesa origem, adiciona à outra
        meio = mesa[pos1];
        mesa[pos1] = mesa[pos1]->prox;

        meio->prox = mesa[pos2];
        mesa[pos2] = meio;

        cont_mesa[pos2]++;
        cont_mesa[pos1]--;
    }
    else{
        printf("digite a mesa origem, a mesa destino e a quantidade a ser movida: ");
        scanf("%d %d %d", &pos1, &pos2, &qtd);
        pos1--; pos2--;

        if(pos1>7 || pos2>7 || qtd>cont_mesa[pos1] || mesa[pos1]==NULL) return 0; // se a entrada for invalida -> fora
        // definir qual é o maior e seu naipe

        maior = mesa[pos1];
        for(i=1; i<qtd;i++){
            meio = maior->prox;
            if(meio->carta[0] == maior->carta[0]+1 && // se for maior
               (((maior->carta[1] == paus || maior->carta[1] == espadas)&&
                 (meio->carta[1] == copas || meio->carta[1] == ouros))||
               ((maior->carta[1] == ouros || maior->carta[1] == copas)&&
                 (meio->carta[1] == paus || meio->carta[1] == espadas)))){ // e de naipe diferente
                maior = meio; // prossegue
            }
            else return 0; // entrada inválida
        }



        if(mesa[pos2] != NULL){ // se a mesa destino não estiver vazia e...
            if((maior->carta[0]+1 != mesa[pos2]->carta[0]) || // se a carta do maior for maior que ou igual a carta da mesa -> fora
               !(((maior->carta[1] == paus || maior->carta[1] == espadas)&&
                 (mesa[pos2]->carta[1] == copas || mesa[pos2]->carta[1] == ouros))||
               ((maior->carta[1] == ouros || maior->carta[1] == copas)&&
                 (mesa[pos2]->carta[1] == paus || mesa[pos2]->carta[1] == espadas)))) return 0; // se os naipes forem de cores iguais -> fora
        }


        meio = mesa[pos1];
        mesa[pos1] = maior->prox; // corta a mesa de origem
        maior->prox = mesa[pos2]; // "cola" o pedaço da origem no destino
        mesa[pos2] = meio;  // redefine o topo do destino como o topo anterior da origem

        cont_mesa[pos1]-=qtd;
        cont_mesa[pos2]+=qtd;
    }

    return 1;
}


int main(int argc, char **argv){
    int op=0;
    srand(time(NULL));
    inicializa();
    gerarBaralho();
    distribuirMesa();
    while(op!=8){
        imprimir();
        printf("(1)move Mesa-Temp\n");
        printf("(2)move Mesa-Naip\n");
        printf("(3)move Temp-Mesa\n");
        printf("(4)move Naip-Temp\n");
        printf("(5)move Temp-Naip\n");
        printf("(6)move Naip-Mesa\n");
        printf("(7)move Mesa-Mesa\n");
        printf("(8)sair\n");
        printf("opcao: ");
        scanf(" %d", &op);
        getchar();
        switch(op){
            case 1:
                if(moveMesaTemp()) printf("movido\n");
                else printf("entrada invalida\n");
                break;
            case 2:
                if(moveMesaNaipe()) printf("movido\n");
                else printf("entrada invalida\n");
                break;
            case 3:
                if(moveTempMesa()) printf("movido\n");
                else printf("entrada invalida\n");
                break;
            case 4:
                if(moveNaipeTemp()) printf("movido\n");
                else printf("entrada invalida\n");
                break;
            case 5:
                if(moveTempNaipe()) printf("movido\n");
                else printf("entrada invalida\n");
                break;
            case 6:
                if(moveNaipeMesa()) printf("movido\n");
                else printf("entrada invalida\n");
                break;
            case 7:
                if(moveMesaMesa()) printf("movido\n");
                else printf("entrada invalida\n");
                break;
            case 8:
                break;
            default:
                printf("opcao Invalida.\n");
                break;
        }

        if(cont_naipe[0] == 13 && cont_naipe[1] == 13 && cont_naipe[2] == 13 && cont_naipe[3] == 13){
            printf("\nParabens por essa vitoria. Seus pais estariam orgulhosos de voce.\n");
            op = 8;
            ShellExecute(NULL, "open", "https://www.youtube.com/watch?v=gUkaoy1Pr7E", NULL, NULL, SW_SHOWNORMAL);
            imprimir();
        }
    }

    destroi();

    return 0;
}
