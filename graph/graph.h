/*!
 * \file graph.h
 *
 * \date Sep 24, 2013
 * \author Ricardo C. Corrêa (correa@ufrrj.br)
 *
 * \brief Macros and inline functions of a bitmap implementation of a graph adjacency matrix.
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <direct/bitmap.h>

/*!
 * \struct Graph
 * \brief Bitmap implementation of a graph adjacency matrix.
 *
 * In order to optimize indexing, the number of nodes in a row is augmented up to a power of 2.
 */
typedef struct {
	size_t 		n; 				//!< Number of vertices
	size_t 		m; 				//!< Number of edges
	BitMap *	matrix; 		//!< Bitmap adjacency matrix.
	BitMap *	swap; 			//!< Bitmap used in the swap operation.
	BitMap 		neig; 			//!< Returned as neighborhood of a specified vertex.
	int 		maxcq; 			//!< Size of the maximum clique.
	double 		maxwcq; 		//!< Weight of the maximum clique.
} Graph;

static inline unsigned long long getN(Graph * const g) {
	return g->n;
}

/*!
 * \fn Graph * newGraph(size_t n)
 * \brief Creates a new graph with the specified number of vertices and with no edges.
 *
 * \param n The number of vertices.
 *
 * \return The bitmap implementation of the graph.
 */
//Graph * newGraph(size_t n);
static inline Graph * newGraph(size_t n) {
	Graph * ret = (Graph *) malloc(sizeof(Graph));
	ret->matrix = newBitMapVector(n, n);
	ret->swap = newBitMap(n);
	setAsViewOf(ret->matrix, &ret->neig);
	ret->n = n;

//	printf("ret-matrix-R=%d  n=%d  logrow=%d  LOGB=%ld  matsz=%d\n",(int)ret->matrix->R, n, ret->logrowsizelogb, LOGB, ret->matrixsize);

	return ret;
}

/*!
 * \fn void delGraph(Graph * g)
 * \brief Deletes the bitmap associated with the specified graph.
 *
 * \param g The graph.
 */
//void delGraph(Graph * g);
static inline void delGraph(Graph * g) {
//	printf("g-matrix=%d\n",(int)g->matrix);
	freeBitMap(g->matrix);
	freeBitMap(g->swap);
//	free(g->matrix);
}

//!< Bitmap corresponding to the neigborhood of \c i in \c g.
static inline BitMap const * neig(Graph * const g, unsigned long long int i) {
	subMap(g->matrix, &g->neig, i);
	return &g->neig;
}

//!< Returns a non-zero value if \c i, \c j defines an edge in \c g, and 0 otherwise.
static inline int hasEdge(Graph * g, unsigned long long int i, unsigned long long int j) {
	subMap(g->matrix, &g->neig, i);
	return hasElement(&g->neig, j);
}

static inline void addEdge(Graph * g, unsigned long long int i, unsigned long long int j) {
	Node node;
	subMap(g->matrix, &g->neig, i);
	addElement(&g->neig, j);

	subMap(g->matrix, &g->neig, j);
	addElement(&g->neig, i);
}

static inline void invEdge(Graph * g, unsigned long long int i, unsigned long long int j) {
	Node node;
	subMap(g->matrix, &g->neig, i);
	invElement(&g->neig, j);

	subMap(g->matrix, &g->neig, j);
	invElement(&g->neig, i);
}

static inline void addArc(Graph * g, unsigned long long int i, unsigned long long int j) {
	Node node;
	subMap(g->matrix, &g->neig, i);
	addElement(&g->neig, j);
}

static inline void delArc(Graph * g, unsigned long long int i, unsigned long long int j) {
	Node node;
	subMap(g->matrix, &g->neig, i);
	delElement(&g->neig, j);
}

static inline void invArc(Graph * g, unsigned long long int i, unsigned long long int j) {
	Node node;
	subMap(g->matrix, &g->neig, i);
	invElement(&g->neig, j);
}

static inline void addAllEdges(Graph * g) {
	int i;
	for (i = 0; i < g->n; i++) {
		subMap(g->matrix, &g->neig, i);
		fill(&g->neig);
		invArc(g, i, i);
	}
}

static inline void swapVertices(Graph * g, unsigned long long int i, unsigned long long int j) {
	// swap lines
	transcript(g->swap, neig(g, i));
	subMap(g->matrix, &g->neig, i);
	transcript(&g->neig, neig(g, j));
	subMap(g->matrix, &g->neig, j);
	transcript(&g->neig, g->swap);

	// swap columns
	symDiff(g->swap, neig(g, j));
	Node node;
	for (begin(g->swap, &node); !end(&node); next(&node))
	{
		int k = getElement(&node);
		invArc(g,k,i);
		invArc(g,k,j);
	}
}

static inline Graph * newRenumbered(Graph * gg, int * R) {
	int n = gg->n;

	Graph * g = newGraph(n);

	int i;
	Node node;
	for (i = 0; i < n; i++)
		for (begin(neig(gg,i), &node); !end(&node); next(&node))
			addEdge(g,R[i],R[getElement(&node)]);

	return g;
}

#ifdef _DEBUG_
static inline void printNeigNodes(Graph * g, int i) {
	int lastnode = NODEIDX(g->n-1);
	int node;

	printf("NEIG(%d): [",i);
	for (node = lastnode; node >= 0; node--)
	printf(FORMX, NEIG(g,i)[node]);
	printf("]\n");
}

static inline void printNodes(Graph * g) {
	int i;
	for (i = 0; i < g->n; i++)
	printNeigNodes(g, i);
}
#endif

#endif /* GRAPH_H_ */
