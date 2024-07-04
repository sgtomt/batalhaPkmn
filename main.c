#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pkmn {
    char nome[15];
    double ataque;
    double defesa;
    double vida;
    char tipo[9]; 
};


double superEfetivo(char atacante[8], char defensor[8]){
    if (strcmp(atacante, "agua") == 0 && strcmp (defensor, "fogo") == 0) return 1.2;
    if (strcmp(atacante, "fogo") == 0 && strcmp (defensor, "gelo") == 0) return 1.2;
    if (strcmp(atacante, "gelo") == 0 && strcmp (defensor, "pedra") == 0) return 1.2;
    if (strcmp(atacante, "pedra") == 0 && strcmp (defensor, "eletrico") == 0) return 1.2;
    if (strcmp(atacante, "eletrico") == 0 && strcmp (defensor, "agua") == 0) return 1.2;
    return 1.0;
}

int verificaVida(double x){
    return (x <= 0) ? 1 : 0;
}

void atacaPokemon(struct pkmn *atacante, struct pkmn *defensor){
    double multiplicador = superEfetivo(atacante->tipo, defensor->tipo);
    double ataqueAtacante = atacante->ataque * multiplicador;
    if(ataqueAtacante > (*defensor).defesa){
        (*defensor).vida -= (ataqueAtacante - (*defensor).defesa);
    } else defensor->vida--;
}

int encerraBatalha (int total, int derrotados){
    return (derrotados < total) ? 1 : 0;
}


void statusVidaPokemons (struct pkmn *pokemonP1, int qtdP1, struct pkmn *pokemonP2, int qtdP2){
    printf("Pokemons sobreviventes:\n");
    for(int i=0; i<qtdP1; i++){
        if(pokemonP1[i].vida > 0) printf("%s\n", pokemonP1[i].nome);
    }

    for(int i=0; i<qtdP2; i++){
        if(pokemonP2[i].vida > 0) printf("%s\n", pokemonP2[i].nome);
    }

    printf("Pokemons derrotados:\n");
    for(int i=0; i<qtdP1; i++){
        if(pokemonP1[i].vida <= 0) printf("%s\n", pokemonP1[i].nome);
    }

    for(int i=0; i<qtdP2; i++){
        if(pokemonP2[i].vida <= 0) printf("%s\n", pokemonP2[i].nome);
    }
}

int main(){

    int quantidadePkmnP1=0, quantidadePkmnP2=0;
    int batalha = 1;

    FILE *file = fopen("pkmn.txt", "rt");

    if(file == NULL){
        printf("Erro ao abrir o arquivo txt");
        return 1;
    }

    //leitura da quantidade de pokemons de cada jogador e criação de cada pokemon
    fscanf(file, "%d %d", &quantidadePkmnP1, &quantidadePkmnP2);

    // reserva de memória para os pokemons do txt
    struct pkmn *pokemonP1 = (struct pkmn*)malloc(sizeof(struct pkmn) * quantidadePkmnP1);
    struct pkmn *pokemonP2 = (struct pkmn*)malloc(sizeof(struct pkmn) * quantidadePkmnP2);
    if( pokemonP1 == NULL || pokemonP2 == NULL){
        printf("Erro ao alocar memória [alocaMemoriaPkmn]\n");
        return 1;
    }

    fgetc(file); //elimina a primeira linha do arquivo

    //loop para atribuir as características dos pokemons de cada jogador P1 e P2 respectivamente
    for(int i=0; i < quantidadePkmnP1; i++){
        fscanf(file, "%s %lf %lf %lf %s", pokemonP1[i].nome, &pokemonP1[i].ataque, &pokemonP1[i].defesa, &pokemonP1[i].vida, pokemonP1[i].tipo);
    }

    for(int i=0; i < quantidadePkmnP2; i++){
        fscanf(file, "%s %lf %lf %lf %s", pokemonP2[i].nome, &pokemonP2[i].ataque, &pokemonP2[i].defesa, &pokemonP2[i].vida, pokemonP2[i].tipo);
    }

    int pkmnP1=0, pkmnP2 =0;
    while(batalha == 1){
        //ataque do jogador 1
        atacaPokemon(&pokemonP1[pkmnP1], &pokemonP2[pkmnP2]);
        if(verificaVida(pokemonP2[pkmnP2].vida) == 1) {
            printf("%s venceu %s \n", pokemonP1[pkmnP1].nome, pokemonP2[pkmnP2].nome);
            pkmnP2++;
            batalha = encerraBatalha(quantidadePkmnP2, pkmnP2); //verifica se o defensor ainda tem pokemons
            if (batalha==0) {
                printf("Jogador 1 venceu \n");
                break;
            } 
        }

        //ataque do jogador 2
        atacaPokemon(&pokemonP2[pkmnP2], &pokemonP1[pkmnP1]);
        if(verificaVida(pokemonP1[pkmnP1].vida) == 1) {
            printf("%s venceu %s \n", pokemonP2[pkmnP2].nome, pokemonP1[pkmnP1].nome);
            pkmnP1++;
            batalha = encerraBatalha(quantidadePkmnP1, pkmnP1); //verifica se o defensor ainda tem pokemons
            if (batalha==0) {
            printf("Jogador 2 venceu \n");
            break;
            } 
        }
    }

    statusVidaPokemons(pokemonP1, quantidadePkmnP1, pokemonP2, quantidadePkmnP2);

    getchar();
    fclose (file);
    free(pokemonP1);
    free(pokemonP2);
    return 0;
}