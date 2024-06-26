#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}
/*
 Implemente la función createTreeMap en el archivo treemap.c. Esta función recibe la función de comparación de claves y crea un mapa (TreeMap) inicializando sus variables. 
 El siguiente código muestra como inicializar la función de comparación. Reserve memoria, inicialice el resto de variables y retorne el mapa.
*/

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2))
{
  TreeMap *new = (TreeMap*)malloc(sizeof(TreeMap));
  if(new == NULL){
    return NULL;
  }
  //Inicializar variables
  new->root = NULL;
  new->current = NULL;
  new->lower_than = lower_than;
  //Retornar el mapa
  return new;
}

/*
 Implemente la función void insertTreeMap(TreeMap * tree, void* key, void * value). Esta función inserta un nuevo dato (key,value) en el árbol y hace que el current apunte al nuevo nodo. Para insertar un dato, primero debe realizar una búsqueda para encontrar donde debería ubicarse. Luego crear el nuevo nodo y enlazarlo. Si la clave del dato ya existe retorne sin hacer nada (recuerde que el mapa no permite claves repetidas).
*/

void insertTreeMap(TreeMap * tree, void* key, void * value)
{
  if(searchTreeMap(tree, key) == NULL)//Si la clave no esta en el arbol
  {
    TreeNode *new = createTreeNode(key, value);
    if(tree->root == NULL)
    {
      tree->root = new;
      tree->current = new;
    }
    else
    {
      TreeNode *aux = tree->root;
      while(aux != NULL)
      {
        if(tree->lower_than(key, aux->pair->key))
        {
          if(aux->left == NULL)
          {
            aux->left = new;
            new->parent = aux;
            tree->current = new;
            return;
          }
          else
          {
            aux = aux->left;
          }
        }
        else
        {
          if(aux->right == NULL)
          {
            aux->right = new;
            new->parent = aux;
            tree->current = new;
            return;
          }
          else
          {
            aux = aux->right;
          }
        }
      }
    } 
  } 
}

/*
Implemente la función TreeNode * minimum(TreeNode * x). 
Esta función retorna el nodo con la mínima clave ubicado en el subárbol con raiz x. 
Para obtener el nodo tiene que, a partir del nodo x, irse por la rama izquierda hasta llegar al final del subárbol. Si x no tiene hijo izquierdo se retorna el mismo nodo.
*/
TreeNode * minimum(TreeNode * x)
{
  if(x->left == NULL)
    return x;
  else
  {
    while(x->left != NULL)
    {
      x = x->left;
    }
    return x;
  }
}

/*
Implemente la función void removeNode(TreeMap * tree, TreeNode* node).
Esta función elimina el nodo node del árbol tree. Recuerde que para eliminar un node existen 3 casos: Nodo sin hijos: Se anula el puntero del padre que apuntaba al nodo Nodo con un hijo: El padre del nodo pasa a ser padre de su hijo Nodo con dos hijos: Descienda al hijo derecho y obtenga el menor nodo del subárbol (con la función minimum). Reemplace los datos (key,value) de node con los del nodo "minimum". Elimine el nodo minimum (para hacerlo puede usar la misma función removeNode).
*/

void removeNode(TreeMap * tree, TreeNode* node)
{
  //nodo sin hijos
  if(node->left == NULL && node->right == NULL)
  {
    if(node->parent == NULL)
    {
      tree->root = NULL;
    }
    else if(node->parent->left == node)
    {
      node->parent->left = NULL;
    }
    else{
      node->parent->right = NULL;
    }
    free(node);
  }
  //nodo con un hijo
  else if(node->left == NULL || node->right == NULL)
  {
    //
    TreeNode* child = NULL;
    if(node->left != NULL)
    {
      child = node->left;
    }
    else
    {
      child = node->right;
    }
    
    if(node->parent == NULL)
    {
      //nodo raiz con un solo hijo
      tree->root = child;
      child->parent = NULL;
    }
    else if(node->parent ->left == node)
    {
      //Nodo es hijo izquierdo
      node->parent->left = child;
      child->parent = node->parent;
    }
    else
    {
      //Nodo es hijo derecho
      node->parent->right = child;
      child->parent = node->parent;
    }
    free(node);
  }
  else
  {
    //nodo con dos hijos
    TreeNode * sucesor = minimum(node->right);
    //NODE->PAIR->KEY = SUCESOR->PAIR->KEY
    //NODE->PAIR->VALUE = SUCESOR->PAIR->VALUE
    node->pair = sucesor->pair;
    removeNode(tree, sucesor);
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


/*
Implemente la función Pair* searchTreeMap(TreeMap* tree, void* key), la cual busca el nodo con clave igual a key y retorna el Pair asociado al nodo. Si no se encuentra la clave retorna NULL. Recuerde hacer que el current apunte al nodo encontrado.
*/

Pair * searchTreeMap(TreeMap * tree, void* key){
  tree->current = NULL;

  TreeNode *aux = tree->root;
  while(aux != NULL)
  {
    if(is_equal(tree, aux->pair->key, key))
    {
      tree->current = aux;
      return aux->pair;
    }
    else if(tree->lower_than(key, aux->pair->key))
    {
      aux = aux->left;
    }
    else{
      aux = aux->right;
    }
  }
  return NULL;
}

/*
La función Pair* upperBound(TreeMap* tree, void* key) retorna el Pair con clave igual a key. En caso de no encontrarlo retorna el primer par asociado a una clave mayor o igual a key. Para implementarla puede realizar una búsqueda normal y usar un puntero a nodo auxiliar ub_node que vaya guardando el nodo con la menor clave mayor o igual a key. Finalmente retorne el par del nodo ub_node.
*/
Pair * upperBound(TreeMap * tree, void* key)
{
  TreeNode* aux = tree->root;
  TreeNode* ubNode = NULL;

  if(aux == NULL) return NULL;
  while(aux != NULL)
  {
    // lower_than, recibe dos parámetros (las claves) y retorna un entero (1 si key1<key2 y 0 si no).
    // is_equal, función para saber si dos claves son iguales.
    int cmpKeys = tree->lower_than(key, aux->pair->key);
    if(is_equal(tree, key, aux->pair->key))
    {
      return aux->pair;
    }
    else if(cmpKeys == 1){
      ubNode = aux;
      aux = aux->left;
    }
    else{
      aux = aux->right;
    }
  }    
  if(ubNode != NULL)
  {
    return ubNode->pair;
  }  
  else{
    return NULL;
  }
}

/*
Implemente las funciones para recorrer la estructura: Pair* firstTreeMap(TreeMap* tree) retorna el primer Pair del mapa (el menor). 
Pair* nextTreeMap(TreeMap* tree) retornar el siguiente Pair del mapa a partir del puntero TreeNode* current. Recuerde actualizar este puntero.
*/

Pair * firstTreeMap(TreeMap * tree)
{
  TreeNode* aux = tree->root;
  if(aux == NULL)
  {
    return NULL; //Arbol vacio
  }
  while(aux->left != NULL)
  {
    aux = aux->left;
  }
  tree->current = aux;
  return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree)
{
  TreeNode* aux = tree->current;
  if(aux == NULL)
  {
    return NULL; 
  }
  
  // Si el nodo tiene un hijo derecho, el siguiente será el nodo más a la izquierda de ese subárbol
  if(aux->right != NULL)
  {
    aux = aux->right;
    while(aux->left != NULL)
    {
      aux = aux->left;
    }
  }
  else
  {
    // Retroceder al nodo padre hasta encontrar un nodo que sea el hijo izquierdo de su padre
    TreeNode * parent = aux->parent;
    while(parent != NULL && aux == parent->right)
    {
      aux = parent;
      parent = parent->parent;
    }
    aux = parent;
  }
  tree->current = aux; //Actualizar current del arbol
  
  if(aux != NULL)
  {
    return aux->pair;
  }
  else return NULL;
}
