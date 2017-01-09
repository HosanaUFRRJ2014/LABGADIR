#include <graph.h>
#include <heap.h>
#include <stdlib.h>

#define false 0
#define true 1

unsigned int ident2() 
{
	return 2014780267;
}

int eh_subaciclico(Graph * const g, BitMap * const s) 
{
	Graph * sub = newGraph(s->n);
	sub->matrix = s;

	return eh_aciclico(sub);
}



int eh_aciclico(Graph * const g)
{

	int n = g->n;


	BitMap * cands = newBitMap(getN(g));
	BitMap *const a = newBitMap(getN(g));
	Node node;

	int vetor[n], d[n], heap[n];

    int h = 0;
	for(h = 0; h < n; h++)
	{
		vetor[h] = 1;
		d[h] = 0;
		heap[h] = 0;
	}


	cands = pack(vetor, n, n);
    

	int v,w;

	int pos[n];

	for(v = 0; v < n; v++)
	{
		d[v] = cardOf(neig(g,v));
	}


	int compard(const void *i, const void *j)
	{
    	return d[*((const int *) j)] - d[*((const int *) i)];
    }

    int hash(const void *i) 
    {
        return *((const int *) i);
    }


    heapify_a(d, n, sizeof(int),compard,pos,hash);

    *heap = *d;


    while(n > 0 && d[heap[0]] <= 1)
    {
    	v = pos[heap[0]];
    	heappoll_a(heap, n, sizeof(int),compard,pos,hash);

    	n--;
    	if(d[v] == 1)
    	{
    		copyAll(a, cands);
    		intersectOf(a, a, neig(g,v));

    		begin(a,&node);
    		next(&node);
    		w = getElement(&node);
    		d[w] = d[w] - 1;
    		heapup_a(heap, n, sizeof(int),compard,pos,hash);
    	}


    }


    if(n == 0)
    	return true;


	return false;
}

