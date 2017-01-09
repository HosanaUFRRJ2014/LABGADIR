#include <graph.h>
#define true 1
#define false 0
#define VALOR_GRANDE 100000000

int count = 0;

/**Structs****/
typedef struct
{
	Node node; //vindo de graph.h
	
}Vertice;

typedef struct
{
	Vertice vertice;
	
}No_Vertice_L;


typedef struct
{
	No_Vertice_L nv1;
	No_Vertice_L nv2;
	
}No_Aresta_L;

/*Funções de manipulação das structs*/
Vertice * criar_vertice()
{
	Vertice *v = (Vertice *) malloc(sizeof(Vertice));

	// if(v != NULL)
	// {
	// 	v->node = 
	// 	//v->algumaCoisa = algumValor;
	// }

	return v;
}

No_Vertice_L *criar_No_Vertice_L()
{
	No_Vertice_L *nv  = (No_Vertice_L *) malloc(sizeof(No_Vertice_L));

	// if(nv != NULL)
	// {
	// 	nv->v = -1;
	// }

	return nv;
}

No_Aresta_L *criar_No_Aresta_L()
{
	No_Aresta_L *na  = (No_Aresta_L *) malloc(sizeof(No_Aresta_L));

	// if(na != NULL)
	// {
	// 	na->nv1 = -1;
	// 	na->nv2 = -1;
	// }

	return na;
}

/*Funções*/

unsigned int ident3() 
{
	return 2014780267;
}

void freeNo_Vertice_L(No_Vertice_L * no) 
{
	free(no);
}

// Vertice alvo(No_Vertice_L * const no) 
// {
// 	Vertice *v = criar_vertice();
// 	v = no->vertice;
//     return *v;
// }


//definir o alvo como como a conjunto de valores de acordo com o percurso em largura.
// Definir nós visitados e alcançados

int alvo(No_Vertice_L * const no)
{
	Node *noAux = &no->vertice.node;
	printf("alvo: %d\n",noAux->cur);
	//count++;
	//printf("count: %d\n",count );

	return noAux->cur;
}

No_Vertice_L * avancar_VL(No_Vertice_L * no) 
{
//	BitMap *vizinhos = newBitMap(g->n - 1);
	//neig(Graph * const g, unsigned long long int i)
	 // if(&no->vertice.node != NULL)
 	printf("\navancar_VL\n");
	next(&no->vertice.node);
	return no;
}

No_Vertice_L * partida_VL(Graph * const g, Vertice u) 
{
	Node aux = u.node;
	printf("\npartida_VL antes: %d\n",aux.cur);
	begin(g->matrix,&u.node);
	aux = u.node;
	printf("\npartida_VL depois: %d\n",aux.cur);
	No_Vertice_L *no = criar_No_Vertice_L();
	no->vertice = u;
	return no;
}

int chegada_VL(No_Vertice_L * no) 
{
	printf("chegada_VL\n");
	return end(&(no->vertice.node));
}

int eh_subbipartido(Graph * const g, BitMap const * r, BitMap * s, BitMap * t) 
{
	return 0;
}

int eh_bipartido(Graph * const g, BitMap * s, BitMap * t) 
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void freeNo_Aresta_L(No_Aresta_L * no) 
{
	free(no);
}

Vertice alvo_menor(No_Aresta_L * const no) 
{
	//return 0;
	Vertice v;
	return v;
}

Vertice alvo_maior(No_Aresta_L * const no) 
{
	//return 0;
	Vertice v;
	return v;
}

int chegada_AL(No_Aresta_L * no) 
{
	return 0;
}

No_Aresta_L * avancar_AL(No_Aresta_L * no) 
{
	return NULL;
}

No_Aresta_L * partida_AL(Graph * const g, Vertice u) 
{
	return NULL;
}

