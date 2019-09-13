#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct node{

    struct node** children;
    char value;
    char word[21];
    uint32_t order;
    int bool;

}node;

node* initNode(char key){

    node* element = malloc(sizeof(node));

    element->order = -1;
    element->bool = 0;
    element->value = key;
    element->children = (node**)malloc(26*sizeof(node*));
    
    for(int i = 0 ; i < 26 ; i++){

       element->children[i] = (node*)malloc(sizeof(node));
       element->children[i] = NULL;
    }
    
    return element;

}

int indice(char* key, int digit){

    int numb = key[digit - 1];
    return (numb - 'a');

}

node* insert(node* root, char key[21], uint32_t digit, uint32_t order){

    if(root == NULL){

        root = initNode(key[digit - 2]);   

    }
    if(digit - 1 == strlen(key)){

        root->order = order; 
        strcpy(root->word, key);
    }
    else{

        root->children[indice(key, digit)] = insert(root->children[indice(key, digit)], key, digit + 1, order);

    }

    return root;
}

int vir = 0;
void down(node* root, int limit, int atual, FILE** output){

    for(int i = 0; i < 26; i++){

        if(root->children[i] != NULL){

            if(root->children[i]->order != -1 && root->children[i]->bool == 0 && atual < limit){
                if(vir == 1){


                    fprintf(*output, ",");   
                    fprintf(*output, "%s", root->children[i]->word);

                }
                else{

                    fprintf(*output,"%s", root->children[i]->word);
                    vir = 1;

                }

                root->children[i]->bool = 1;
                
            }
            if(atual < limit ){

                down(root->children[i], limit, atual+1, output);
               
            }
            
            else{

                return;

            }
        }
    }
}

void clean(node* root){
    for(int i = 0; i < 26; i++){
        if(root->children[i] != NULL){
            if(root->children[i]->bool == 1){
                root->children[i]->bool = 0;
            }
        }
    }
}

void search(node* root, char key[21], FILE** output){

    node* aux = root;   
    
    int z = 0;
    fprintf(*output,"%s:", key);
    for(int h = 1 ; h <= strlen(key) ; h++){   
        for(int i = 0; i < 26 ; i++){
            if(aux->children[i] != NULL){
                if(aux->children[i]->value == key[z]){
               
                    aux = aux->children[i];
                    down(aux, h*2, h, output);
                    clean(aux);
                    z++;
                    break;
                }
            }
        }   
        if(z == 0){
            fprintf(*output,"-");
            break;
        }  
    }
    vir = 0;
    fprintf(*output,"\n");
}

int main(int argc, char const *argv[])
{

    FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    node *root = initNode('X');

    int count;
    fscanf(input, "%d", &count);
    char key[21];

    for(int i = 0 ; i < count ; i++){

        fscanf(input, "%s", key);
        root = insert(root, key, 1, i);

    }

    fscanf(input, "%d", &count);
 
    for(int i = 0 ; i < count ; i++){

        fscanf(input, "%s", key);
        search(root, key, &output);
   
    }
    
    fclose(input);
	fclose(output);

    return 0;
}
