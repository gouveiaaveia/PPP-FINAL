#include "main.h"

int main() {
    int id_atual = 0;
    int ler_ficheiro;
    pLista lista = cria();

    ler_ficheiro = carregar_pacientes_do_ficheiro(&lista, &id_atual);
    //Apenas lê o ficheiro de registos se o ficheiro dos pacientes foi carregado
    if(ler_ficheiro == 1){
        carregar_registos_pacientes_ficheiro(&lista);
    }
    printf("Bem vindo ao UC Hospital\n");
    menu(&lista, &id_atual);
    return 0;
}

void menu(pLista *lista, int *id_atual) {
    int opcao;
    printf("----------MENU---------\n");
    printf("1. Registar paciente\n2. Remover paciente\n3. Lista de pacientes\n4. Informações\n5. Sair\n");
    printf("OPÇÃO: ");
    scanf("%d", &opcao);
    getchar();
    system("clear");

    switch (opcao) {
        case 1:
            pedir_informacao_registo(lista, id_atual);
            break;
        case 2:
            remover_paciente(lista, id_atual);
            break;
        case 3:
            imprime(*lista, id_atual);
            break;
        case 4:
            mais_informacao(lista, id_atual);
            break;
        case 5:
            criar_ficheiro_doentes(lista);
            criar_ficheiro_registos(lista);
            destroi(*lista);
            exit(0);
            break;
        default:
            printf("Opção inválida!!\n");
            menu(lista, id_atual);
            break;
    }
}

void mais_informacao(pLista *lista, int *id_atual) {
    int opcao;
    int id;
    printf("----------MENU---------\n");
    printf("1. Pacientes com Hipertensão\n2. Tensão máxima\n3. Informação de paciente\n4. Adicionar medidas\n5. Voltar\n");
    printf("OPÇÃO: ");
    scanf("%d", &opcao);
    getchar();
    system("clear");

    switch (opcao) {
        case 1:
            pacientes_tensao_alta(*lista, id_atual);
            break;
        case 2:
            tensao_maxima(*lista, id_atual);
            break;
        case 3:
            id = pedir_id();
            mostrar_informacao(*lista, id, id_atual);
            break;
        case 4:
            id = pedir_id();
            pedir_medicoes(lista, id, id_atual);
            break;
        case 5:
            menu(lista, id_atual);
            break;
        default:
            printf("Opção inválida!!\n");
            sleep(1);
            menu(lista, id_atual);
            break;
    }
}

void pedir_informacao_registo(pLista *lista, int *id_atual){
    Registro info;
    int valor;

    info.ID = ++(*id_atual);

    do{
        printf("Data de nascimento (dd/mm/aa): ");
        valor = scanf("%d/%d/%d", &info.dia, &info.mes, &info.ano);
        if(valor != 3) {
            while(getchar() != '\n');
        }
    }while(verificar_data(info.dia, info.mes, info.ano) || valor != 3);
    getchar();

    do{
        printf("Nome: ");
        fgets(info.nome, MAX, stdin);
    }while(verificar_nome(info.nome));
    
    info.nome[strlen(info.nome) -1 ] = '\0';

    char cartao[MAX];
    do{
        printf("Cartão de cidadão (XXXXXXXX-X-XXX): ");
        fgets(cartao, MAX, stdin);
        cartao[strlen(cartao) - 1] = '\0';
    }while(verificar_bi(cartao));

    strcpy(info.cartao_cidadao, cartao);

    do {
        printf("Telefone: ");
        if (scanf("%d", &info.telefone) != 1 || info.telefone < 100000000 || info.telefone > 999999999) {
            printf("Telefone inválido. Por favor, insira um número de 9 dígitos.\n");
            while (getchar() != '\n');
        }
    } while (info.telefone < 100000000 || info.telefone > 999999999);
    getchar();

    do{
        printf("Email: ");
        fgets(info.email, MAX, stdin);
    }while(verificar_email(info.email));

    registro(lista, info);
    printf("Paciente registado!\n");
    sleep(2);
    system("clear");
    menu(lista, id_atual);
}

void pedir_medicoes(pLista *lista, int id, int *id_atual) {
    pLista aux = pesquisa(*lista, id);
    pListaRegistro medicao_atual = (struct noListaRegistro *)malloc(sizeof(struct noListaRegistro));

    if (medicao_atual == NULL) {
        printf("Erro na alocação de memória\n");
        return;
    }

    if (aux != NULL) {
        int valor;
        medicao_atual->info.id = id;

        do {
            printf("Data (dd/mm/aa): ");
            valor = scanf("%d/%d/%d", &(medicao_atual->info.dia), &(medicao_atual->info.mes), &(medicao_atual->info.ano));
            if (valor != 3) {
                printf("Formato de data inválido. Tente novamente.\n");
                while (getchar() != '\n'); 
            }
        } while (valor != 3 || verificar_data(medicao_atual->info.dia, medicao_atual->info.mes, medicao_atual->info.ano) || verifica_data_consula_nascimento(aux, medicao_atual));

        do {
            printf("Tensão mínima: ");
            valor = scanf("%d", &(medicao_atual->info.tensao_minima));
            if (valor != 1) {
                printf("Valor inválido. Tente novamente.\n");
                while (getchar() != '\n');
            }
        } while (valor != 1);

        do {
            printf("Tensão máxima: ");
            valor = scanf("%d", &(medicao_atual->info.tensao_maxima));
            if (valor != 1) {
                printf("Valor inválido. Tente novamente.\n");
                while (getchar() != '\n');
            }
        } while (valor != 1);

        char *avaliacao = (char *)malloc(MAX * sizeof(char));
        if (avaliacao == NULL){
            printf("Erro na avaliação\n");
            return;
        }

        avaliar_tensao(avaliacao, &(medicao_atual->info.tensao_maxima), &(medicao_atual->info.tensao_minima));
        strcpy(medicao_atual->info.avaliacao_tesao, avaliacao);

        do {
            printf("Altura (cm): ");
            valor = scanf("%f", &(medicao_atual->info.altura));
            if (valor != 1) {
                printf("Valor inválido. Tente novamente.\n");
                while (getchar() != '\n'); 
            }
        } while (valor != 1 || verificar_altura(medicao_atual->info.altura));

        do {
            printf("Peso (kg): ");
            valor = scanf("%f", &(medicao_atual->info.peso));
            if (valor != 1) {
                printf("Valor inválido. Tente novamente.\n");
                while (getchar() != '\n');
            }
        } while (valor != 1 || verificar_peso(medicao_atual->info.peso));
    } else {
        printf("Paciente com ID %d não encontrado.\n", id);
        voltar();
        menu(lista, id_atual);
    }

    registro_medicoes(lista, medicao_atual, id);
    free(medicao_atual);
    printf("Medições registradas!\n");

    sleep(2);
    system("clear");
    menu(lista, id_atual);
}

void imprime(pLista lista, int *id_atual) {
    pLista aux = lista->prox;
    printf("Lista de pacientes\n");
    printf("     ID  ||    NOME\n");
    while (aux) {
        printf("%8d || %s\n", aux->pessoa_info.ID, aux->pessoa_info.nome);
        aux = aux->prox;
    }
    voltar();
    menu(&lista, id_atual);
}

void voltar() {
    int opcao;
    printf("1. Voltar\n");
    scanf("%d", &opcao);
    if(opcao == 1) {
        system("clear");
        return;
    }
    else {
        printf("Opção inválida!\n");
        voltar(); 
    }
}

int pedir_id(){
    int id;
    int valor;
    do{
        printf("ID do paciente: ");
        valor = scanf("%d", &id);
        if(valor != 1){
            printf("Inválido\n");
            while(getchar() != '\n'); 
        }
    }while(valor != 1);

    return id;
}


void criar_ficheiro_doentes(pLista *lista){
    FILE *file;
    file = fopen("doentes.txt", "a");
    pLista aux = (*lista)->prox;

    if(aux == NULL){
        printf("Sem pacientes registados\n");
        fclose(file);
        return;
    }

    while (aux != NULL)
    {
        fprintf(file, "%d\n%s\n%d/%d/%d\n%s\n%d\n%s\n", aux->pessoa_info.ID,aux->pessoa_info.nome, aux->pessoa_info.dia, aux->pessoa_info.mes,
                aux->pessoa_info.ano, aux->pessoa_info.cartao_cidadao, aux->pessoa_info.telefone, aux->pessoa_info.email);
        aux = aux->prox;
    }

    fclose(file);
}

void criar_ficheiro_registos(pLista *lista){
    FILE *file;
    file = fopen("registos.txt", "a");
    pLista aux = (*lista);

    if(aux == NULL){
        printf("Sem pacientes registados\n");
        fclose(file);
        return;
    }

    while(aux != NULL){
        pListaRegistro paciente_atual;

        if(aux->lista_registro != NULL){
            paciente_atual = aux->lista_registro->prox;
        }else{
            paciente_atual = NULL;
        }

        if(paciente_atual == NULL){
            aux = aux->prox;
            continue; 
        }

        while(paciente_atual != NULL){
            fprintf(file, "%d\n%d/%d/%d\n%d\n%d\n%s\n%f\n%f\n", paciente_atual->info.id, paciente_atual->info.dia, paciente_atual->info.mes, paciente_atual->info.ano,
                    paciente_atual->info.tensao_maxima, paciente_atual->info.tensao_minima, paciente_atual->info.avaliacao_tesao, paciente_atual->info.peso, paciente_atual->info.altura);
            paciente_atual = paciente_atual->prox;
        }
        aux = aux->prox; 
    }
    fclose(file);
}


int carregar_pacientes_do_ficheiro(pLista * lista, int *id_atual){
    FILE *file;
    Registro aux;
    file = fopen("doentes.txt", "r");
    if(file == NULL){
        return 0;
    }

    while(fscanf(file, "%d\n%[^\n] %d/%d/%d\n%[^\n] %d\n%[^\n]", &(aux.ID), aux.nome, &(aux.dia), &(aux.mes), &(aux.ano), aux.cartao_cidadao,
                &(aux.telefone), aux.email) == 8){
                    if(aux.ID > *id_atual){
                        *id_atual = aux.ID;
                    }
                    registro(lista, aux);
                }
    fclose(file);

    file = fopen("doentes.txt", "w");
    fclose(file);
    return 1;
}

void carregar_registos_pacientes_ficheiro(pLista *lista){
    FILE *file;

    file = fopen("registos.txt", "r");

    if(file == NULL){
        printf("ERRO A ABRIR O FICHEIRO DE DADOS");
        return;
    }

    pListaRegistro paciente_atual = (struct noListaRegistro *)malloc(sizeof(struct noListaRegistro));
    if (paciente_atual == NULL) {
        printf("Erro ao alocar memória para registos.\n");
        fclose(file);
        return;
    }

    while(fscanf(file, "%d\n%d/%d/%d\n%d\n%d\n%[^\n] %f\n%f", &(paciente_atual->info.id), &(paciente_atual->info.dia), &(paciente_atual->info.mes), &(paciente_atual->info.ano),
            &(paciente_atual->info.tensao_maxima), &(paciente_atual->info.tensao_minima), paciente_atual->info.avaliacao_tesao, &(paciente_atual->info.peso), &(paciente_atual->info.altura)) == 9){

            pLista paciente = pesquisa(*lista, paciente_atual->info.id);

            if (paciente != NULL) {
                insere_registro(paciente->lista_registro, paciente_atual->info);
            } else {
                printf("Paciente com ID %d não encontrado.\n", paciente_atual->info.id);
                free(paciente_atual);
            }                
        }
    free(paciente_atual);
    fclose(file);

    file = fopen("registos.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de registos para limpeza.\n");
        return;
    }
    fclose(file);
}



