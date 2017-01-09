
#include <stdio.h>
#include <string.h>

#include <grinput.h>

static void getGraphSize(FILE* graphFile, size_t * n, size_t * m) {
	char type = ' ';
	char linestr[100];
	char* datastr;
	*n = 0;
	*m = 0;

	while (type != 'p') {
		type = fgetc(graphFile);
		if (type != EOF) {

			/* header */
			if (type == 'c') {
				datastr = fgets(linestr, 100, graphFile);
				if (datastr != NULL)
					printf("%s", linestr);
				else {
					*n = -1;
					return;
				}
			}

			/* Vertices */
			if (type == 'p') {
				datastr = fgets(linestr, 100, graphFile);
				if (datastr == NULL) {
					*n = -1;
					return;
				}
				datastr = strtok(linestr, " ");
				printf("\tdatastr:%s\n", datastr);
				if (!strcmp(datastr, "edge") || !strcmp(datastr, "col")) {
					datastr = strtok(NULL, " ");
					printf("\tdatastr:%s\n", datastr);
				}
				*n = atoi(datastr);

				datastr = strtok(NULL, " ");
				printf("\tdatastr:%s\n", datastr);
				*m = atoll(datastr);

				printf("Graph with (%ld) vertices and (%ld) edges.\n", *n, *m);
				printf("Density: %3.2f\n", 2.0 * ((double) *m) / (*n * (*n)));
			}
		}
	}
}

Graph * readDimacsGraph(FILE* graphFile) {
	char type = ' ';
	char linestr[100];
	char* datastr;
	int i, j;
	size_t n;
	size_t m;

	printf("\t#Starts reading...\n");
	fflush(NULL);
	getGraphSize(graphFile, &n, &m);

	////
	// Graph variables
	////
	Graph * g = newGraph(n);
	g->m = m;

	type = fgetc(graphFile);
	while (type != EOF) {
		/* Edges */
		if (type == 'e') {
			datastr = fgets(linestr, 100, graphFile);
			if (datastr == NULL)
				return 0;

			datastr = strtok(linestr, " ");
			i = atoi(datastr) - 1;

			datastr = strtok(NULL, " ");
			j = atoi(datastr) - 1;

			addEdge(g, i, j);
		} else {
			datastr = fgets(linestr, 100, graphFile);
			if (datastr != NULL)
				printf(" %s\n", linestr);
			else
				return NULL;
		}
		type = fgetc(graphFile);
	}
	printf("\t#Finishes reading graph\n");

	return g;
}

double * readDoubleWeights(size_t n, FILE* weightsFile) {
	char type = ' ';
	char linestr[100];
	char* datastr;
	int i;

	double * w;
	w = calloc(n, sizeof(double));

	i = 0;
	type = fgetc(weightsFile);
	while (i < n && type != EOF) {
		/* Edges */
		if (type == 'w') {
			datastr = fgets(linestr, 100, weightsFile);
			if (datastr == NULL)
				return 0;

			datastr = strtok(linestr, " ");
			w[i++] = atof(datastr);
		} else {
			datastr = fgets(linestr, 100, weightsFile);
			if (datastr != NULL)
				printf(" %s\n", linestr);
			else
				return NULL;
		}
		type = fgetc(weightsFile);
	}
	printf("\t#Finishes reading weights\n");

	return w;
}

double * readIntWeights(size_t n, FILE* weightsFile) {
	char type = ' ';
	char linestr[100];
	char* datastr;
	int i;

	double * w;
	w = calloc(n, sizeof(double));

	i = 0;
	type = fgetc(weightsFile);
	while (i < n && type != EOF) {
		/* Edges */
		if (type == 'w') {
			datastr = fgets(linestr, 100, weightsFile);
			if (datastr == NULL)
				return 0;

			datastr = strtok(linestr, " ");
			w[i++] = (int) (10 * atof(datastr));
		} else {
			datastr = fgets(linestr, 100, weightsFile);
			if (datastr != NULL)
				printf(" %s\n", linestr);
			else
				return NULL;
		}
		type = fgetc(weightsFile);
	}
	printf("\t#Finishes reading weights\n");

	return w;
}
