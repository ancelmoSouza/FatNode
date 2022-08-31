#include<iostream>
#include<cstdlib>
#include<ctime>

using namespace std;

template <typename T>struct version_tuple{
    T v; //valor associado a um campo na versão i
    int i; //versão na qual o valor é v
     version_tuple<T>(T v, int i)
    {
        this->v = v;
        this->i = i;
    }
    version_tuple<T>() = default;
};

struct fat_node
{
    int size;
    version_tuple<int> valores[10];
    version_tuple<fat_node*> prox[10];
};

struct head
{
    int current_version;
    version_tuple<fat_node*> prox[10];
};

//Push() => Adiciona um novo nó na cabeça da lista
void inserir(head * h, int value)
{
    //Caso (1): Inserção em lista vazia, versão 0 é criada
    if(h->current_version == -1){
        fat_node *novo = new fat_node;
        novo->valores[0] = version_tuple<int>(value, 0);
        novo->prox[0] = version_tuple<fat_node*>(h->prox[0].v, 0);
        novo->size = 1;

        h->prox[0].v = novo;
        h->prox[0].i = 0;
        h->current_version = 0;

    }else{ //Caso (2): Inserção em uma lista não vazia, nova_versão = current_version + 1
        fat_node *novo = new fat_node;
        novo->valores[0] = version_tuple<int>(value, h->current_version + 1);
        novo->prox[0] = version_tuple<fat_node*>(h->prox[h->current_version].v, h->current_version + 1);
        novo->size = 1;

        h->prox[h->current_version + 1] = version_tuple<fat_node*>(novo, h->current_version + 1);
        h->current_version += 1;
    }

}


//Pop() => Remove o nó na cabeça da lista da versão mais atual
void remover(head * h){
    fat_node * novo = h->prox[h->current_version].v;
    h->prox[h->current_version + 1] = novo->prox[novo->size -1];
    h->prox[h->current_version].i = h->current_version +  1;
    h->current_version++;
}

//Função que busca um nó na lista em uma versão i e retorna se ele existir na lista
// na versão i ou 'nullptr' caso ele não exista naquela lista
fat_node *find(int value, int version, head* h){
    fat_node *novo = new fat_node;
    int v = 0;
    int vj = 0;

    //Caso seja solicitada a busca em uma versão posterior a mais recente da lista
    // a busca será inválida
    if(version > h->current_version){
        return nullptr;
    }

    //Encontra o ponteiro para a menor versão maior que a versão procurada
    while(v <= version){
        novo = h->prox[v].v;
        v++;
    }

    //Atribui a variável novo o valor do nó que encabeça a lista
    // na maior versão menor q a versão buscada
    novo = h->prox[v - 1].v;

    while (novo->prox[vj].v != nullptr) {
        for(int k = 0; k < novo->size; k++){
            //Verifica se a versão do valor seguinte é maior q a versão buscada
            if(novo->valores[k + 1].i > version || k + 1 == novo->size){ // Vesirifica se esse é o valor do nó
                                                                         //maior versão menor que a versão q to buscando
                if(novo->valores[k].v == value)
                    return novo;
                else
                    novo = novo->prox[k].v;
            }
        }

    }

    return novo->prox[0].v;

}



void inserir_inner(head *h, int value, int v_prev){

    if(h == nullptr)
        inserir(h, value);

    fat_node *preview_node = find(v_prev, h->current_version, h);
    int vf = 0;

    if(preview_node != nullptr){
        h->current_version += 1;
        h->prox[h->current_version].v = h->prox[h->current_version - 1].v;
        h->prox[h->current_version].i = h->current_version;

        fat_node *novo = new fat_node;
        novo->valores[0].v = value;
        novo->valores[0].i = h->current_version;
        novo->prox[0].v = preview_node->prox[preview_node->size - 1].v;
        novo->prox[0].i = h->current_version;
        novo->size = 1;

        preview_node->valores[preview_node->size].v = preview_node->valores[preview_node->size - 1].v;
        preview_node->valores[preview_node->size].i = h->current_version;
        preview_node->prox[preview_node->size].v = novo;
        preview_node->prox[preview_node->size].i = h->current_version;
        preview_node->size += 1;

    }else{
        inserir(h, value);
    }
}


void randomList(head * h){
    unsigned seed = time(0);
    for(int i = 0; i <= 10; i++){
        srand(seed);
        inserir(h, 1 + rand() % 30);
    }

}

void show(head * h, int version){
    fat_node *novo = new fat_node;
    int cv = 0;
    if(version > h->current_version){
        cout << "ERRO: Versão selecionada é superior a versão mais recente" << endl;
        return;
    }

    novo = h->prox[version].v;

    while(novo != nullptr){
        for(int i = 0;novo != nullptr && i < novo->size; i++){
            if(novo->valores[i + 1].i > version || i + 1 == novo->size){
               cout << novo->valores[i].v << endl;
               novo = novo->prox[i].v;
             }
        }

    }
}

int main(){
    head * h = new head;
    h->current_version = -1;
    h->prox[0] = version_tuple<fat_node*>(nullptr, -1);

    // randomList(h);
    inserir(h, 5);
    inserir(h, 2);
    inserir(h, 3);


    inserir_inner(h, 4, 2);
    show(h, 3);


}





