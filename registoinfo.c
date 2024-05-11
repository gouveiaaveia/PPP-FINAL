#include "main.h"

pListaRegistro cria_registro() {
    pListaRegistro aux = (pListaRegistro)malloc(sizeof(struct noListaRegistro));
    if (aux != NULL) {
        aux->prox = NULL;
    }
    return aux;
}

int vazia_registro(pListaRegistro lista) {
    return (lista->prox == NULL);
}

pListaRegistro destroi_registro(pListaRegistro lista) {
    pListaRegistro temp_ptr;
    while (!vazia_registro(lista)) {
        temp_ptr = lista;
        lista = lista->prox;
        free(temp_ptr);
    }
    free(lista);
    return NULL;
}

void procura_registro(pListaRegistro lista, int chave, pListaRegistro *ant, pListaRegistro *actual) {
    *ant = lista;
    *actual = lista->prox;
    while (*actual != NULL && (*actual)->info.tensao_maxima > chave ) {
        *ant = *actual;
        *actual = (*actual)->prox;
    }
    if (*actual != NULL && (*actual)->info.tensao_maxima != chave) {
        *actual = NULL;
    }
}

void insere_registro(pListaRegistro lista, Info info) {
    pListaRegistro novo, ant, inutil;
    novo = (pListaRegistro)malloc(sizeof(struct noListaRegistro));
    if (novo != NULL) {
        novo->info = info;
        procura_registro(lista, info.tensao_maxima, &ant, &inutil);
        novo->prox = ant->prox;
        ant->prox = novo;
    }
}

pListaRegistro pesquisa_registro(pListaRegistro lista, int id) {
    pListaRegistro aux = lista->prox;
    while (aux != NULL) {
        if (aux->info.id == id) {
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

void registro_medicoes(pLista *lista, pListaRegistro lista2, int id) {

    pLista aux = pesquisa(*lista, id);
    insere_registro(aux->lista_registro, lista2->info);
}

void mostrar_informacao(pLista lista, int id, int *id_atual) {
    system("clear"); 
    pLista aux = pesquisa(lista, id);
    if (aux != NULL) {
        printf("ID: %d\n", aux->pessoa_info.ID);
        printf("Nome: %s\n", aux->pessoa_info.nome);
        printf("Data de Nascimento: %d/%d/%d\n", aux->pessoa_info.dia, aux->pessoa_info.mes, aux->pessoa_info.ano);
        printf("Medições:\n");
        pListaRegistro medicao_atual = aux->lista_registro->prox;
        while (medicao_atual != NULL) {
            printf("Data: %d/%d/%d, Tensão Máxima: %d, Tensão Mínima: %d, Altura: %.2f, Peso: %.2f\n",
                   medicao_atual->info.dia, medicao_atual->info.mes, medicao_atual->info.ano,
                   medicao_atual->info.tensao_maxima, medicao_atual->info.tensao_minima,
                   medicao_atual->info.altura, medicao_atual->info.peso);
            medicao_atual = medicao_atual->prox;
        }
        printf("\n");
    } else {
        printf("Paciente com ID %d não encontrado.\n", id);
    }
    voltar();
    menu(&lista, id_atual);
}
