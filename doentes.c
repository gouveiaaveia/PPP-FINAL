#include "main.h"

pLista cria() {
    pLista aux = (pLista)malloc(sizeof(struct noLista));
    if (aux != NULL) {
        aux->pessoa_info.ID = 0;
        aux->lista_registro = NULL;
        aux->prox = NULL;
    }
    return aux;
}

int vazia(pLista lista) {
    return (lista->prox == NULL);
}

pLista destroi(pLista lista) {
    pLista temp_ptr;
    while (!vazia(lista)) {
        temp_ptr = lista;
        lista = lista->prox;
        free(temp_ptr);
    }
    free(lista);
    return NULL;
}

void procura(pLista lista, char *chave, pLista *ant, pLista *actual) {
    //int i = 0;
    //char *chave2 = (char *)malloc(MAX * sizeof(char));
    *ant = lista;
    *actual = lista->prox;
    /*
    while (chave[i] != '\0') {
        chave2[i] = tolower(chave[i]);
        i++;
    }
    */
    while (*actual != NULL && strcmp((*actual)->pessoa_info.nome, chave) < 0) {
        *ant = *actual;
        *actual = (*actual)->prox;
    }
    if (*actual != NULL && strcmp((*actual)->pessoa_info.nome, chave) != 0) {
        *actual = NULL;
    }
}

void insere(pLista lista, Registro info, pListaRegistro registro) {
    pLista novo, ant, inutil;
    novo = (pLista)malloc(sizeof(struct noLista));
    if (novo != NULL) {
        novo->pessoa_info = info;
        novo->lista_registro = registro;
        procura(lista, info.nome, &ant, &inutil);
        novo->prox = ant->prox;
        ant->prox = novo;
    }else{
        printf("Erro no sistema");
        return;
    }
}


pLista pesquisa(pLista lista, int id) {
    pLista aux = lista->prox;
    while (aux != NULL) {
        if (aux->pessoa_info.ID == id) {
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

void registro(pLista *lista, Registro info) {

    pListaRegistro lista_registo = cria_registro(); 
    insere(*lista, info, lista_registo); 
}

void remover_paciente(pLista *lista, int *id_atual) {
    int id;
    id = pedir_id();
    getchar();
    
    pLista ant = NULL;
    pLista aux = *lista;
    
    while (aux != NULL) {
        if (aux->pessoa_info.ID == id) {
            if (ant == NULL) {
                *lista = aux->prox;
            } else {
                ant->prox = aux->prox;
            }
            free(aux);
            printf("Paciente removido\n");
            break;
        }
        ant = aux;
        aux = aux->prox;
    }
    
    if (aux == NULL) {
        printf("Paciente com ID %d não encontrado.\n", id);
    }
    
    sleep(1.5);
    system("clear");
    menu(lista, id_atual);
}
