#include <graph.h>

unsigned int ident1() 
{
	return 2014780267;
}

BitMap * viz_comum(Graph * const g, unsigned int u, unsigned int v) 
{
	BitMap * viz = newBitMap(getN(g));
	intersectOf(viz, neig(g,u), neig(g,v));
	return viz;
}

int eh_clique(Graph * const g, BitMap * const s) 
{
	BitMap * aux = cloneBitMap(s);

	Node node;
	for (begin(s, &node); !end(&node); next(&node)) 
	{
		delNode(aux, &node);
		//delElement(aux, &node);
		if (!isSubset(aux, neig(g, getElement(&node))))
			return 0;
	}

	freeBitMap(aux);
	return 1;
}

int viz_comum_eh_clique(Graph * const g, unsigned int u, unsigned int v) 
{
	BitMap * viz = viz_comum(g, u, v);
	int ret = eh_clique(g, viz);
	freeBitMap(viz);
	return ret;
}

void subgraus(Graph * const g, BitMap * const s, unsigned int * d) 
{
	Node node;
	BitMap * aux = similarBitMap(s);
	for (begin(s, &node); !end(&node); next(&node)) 
	{
		unsigned int v = getElement(&node);
		intersectOf(aux, s, neig(g, v));
		d[v] = cardOf(aux);
	}
	
	freeBitMap(aux);
}

int conta_arestas(Graph * const g, BitMap * const s) 
{
	int numArestas = 0;
	Node node;
    BitMap const *vizinhos;

	int cardinalidade = 0;	

	for(begin(s, &node); !end(&node); next(&node))
	{
		vizinhos = neig(g,getElement(&node));
		BitMap *novo = newBitMap(g->n);
		intersectOf(novo, vizinhos,s);

		cardinalidade = cardOf(novo);

		numArestas += cardinalidade;

	}	
	

	return numArestas/2;
}

int viz_comum_conta_arestas(Graph * const g, unsigned int u, unsigned int v) 
{
	BitMap * viz = viz_comum(g, u, v);

	return conta_arestas(g,viz);
}

BitMap * grau_impar(Graph * const g, BitMap * const s) 
{

	BitMap *grauImpar = newBitMap(getN(g));
	Node node;
    BitMap const *vizinhos;

	int cardinalidade = 0;	

	for(begin(s, &node); !end(&node); next(&node))
	{
		vizinhos = neig(g,getElement(&node));
		BitMap *novo = newBitMap(g->n);
		intersectOf(novo, vizinhos,s);

		cardinalidade = cardOf(novo);

		if(cardinalidade % 2  != 0)
		{
			addNode(grauImpar,&node);
			//addElement(grauImpar,&node);
		}

	}	
	

	return grauImpar;
}
