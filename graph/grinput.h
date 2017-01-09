/*!
 * \file grinput.h
 *
 * \date Sep 28, 2013
 * \author Ricardo C. Corrêa (correa@ufrrj.br)
 *
 * \brief Functions to read a graph from a file in the DIMACS format.
 */
#ifndef GRINPUT_H_
#define GRINPUT_H_

#include <graph.h>

/*!
 * \fn Graph * readDimacsGraph(FILE* graphFile)
 * \brief Reads a graph from a specified file in the DIMACS format.
 *
 * A new graph is created.
 *
 * \param graphFile The input file in the DIMACS format.
 *
 * \return The graph read from the specified file.
 */
Graph * readDimacsGraph(FILE* graphFile);

/*!
 * \fn readIntWeights(size_t n, FILE* weightsFile)
 * \brief Read and return an array of integer weights from a specified file
 *
 * A new array is allocated to store the weights.
 *
 * File format:
 *
 *       c random-1.clq
 *       c
 *       c Created on: Jan 20, 2011
 *       c     Author: correa
 *       c
 *       c 4096 values
 *       c
 *       w 0.0246
 *
 *
 * \param n the number of weights to read
 * \param weightsFile the file containing the weights
 */
double * readIntWeights(size_t n, FILE* weightsFile);

/*!
 * \fn readDoubleWeights(size_t n, FILE* weightsFile)
 * \brief Read and return an array of double weights from a specified file
 *
 * A new array is allocated to store the weights.
 *
 * File format:
 *
 *       c random-1.clq
 *       c
 *       c Created on: Jan 20, 2011
 *       c     Author: correa
 *       c
 *       c 4096 values
 *       c
 *       w 0.0246
 *
 *
 * \param n the number of weights to read
 * \param weightsFile the file containing the weights
 */
double * readDoubleWeights(size_t n, FILE* weightsFile);

#endif /* GRINPUT_H_ */
