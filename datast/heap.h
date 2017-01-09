/*!
 * \file heap.h
 *
 * \date Sep 1, 2016
 * \author Ricardo C. Corrêa (correa@ufrrj.br)
 *
 * \brief Heap manipulation functions
 *
 * A heap is a way to organize the elements of an array that allows for fast retrieval of the element with the smallest value at any moment,
 * even repeatedly, while allowing for fast insertion of new elements.
 *
 * The element with the smallest value is always the first element in the array. The order of the other elements depends on the particular implementation,
 * but it is consistent throughout all heap-related functions of this header.
 *
 * The type of the elements in the heap is arbitrary. Distinct elements are allowed to be of different types, but they must be all of same size (in bytes).
 * The elements are compared using a comparison function provided in every function call.
 * The comparison function must return an integer less than, equal to, or greater than zero if the first argument is considered to be respectively
 * less than, equal to, or greater than the second. If two members compare as equal, their order in the sorted array is undefined.
 *
 * In the following example, some of the heap functions are used to sort an array \c a in a nondecreasing order of \c b.
 * Each element in \c a is an index of an element in \c b.
 *
 * \code{.c}
     void heapsort(int a[], int n, int b[]) {

       int compard(const void *i, const void *j) {
         return b[*((const int *) j)] - b[*((const int *) i)];
       }

       heapify(a, n, sizeof(int), compard);

       while (n > 0) {
         int e = a[0];
         heappoll(a, n, sizeof(int), compard);
         a[--n] = e;
       }
     }
   \endcode
 *
 * The following is a version of the previous function with an additional parameter: an array \c pos that indicates the position of each element in \c a.
 * More precisely, array \c pos is such that \c pos[a[i]]=i. This property is remains valid after the execution of the function below.
 *
 * \code{.c}
     void heapsort_a(int a[], int n, int b[], int pos[]) {

       int compard(const void *i, const void *j) {
         return b[*((const int *) j)] - b[*((const int *) i)];
       }

       int hash(const void *i) {
	     return *((const int *) i);
       }

       heapify_a(a, n, sizeof(int), compard, pos, hash);

       while (n > 0) {
         int e = a[0];
         heappoll_a(a, n, sizeof(int), compard, pos, hash);
         a[--n] = e;
       }
     }
   \endcode
 */

#include <stddef.h>

#ifndef HEAP_H_
#define HEAP_H_

/*!
 * \fn void heapify(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
 * \brief Rearrange the elements in a specified array in such a way that they form a heap according to a specified comparison function
 *
 * The contents of the specified array are partially sorted in ascending order according to a comparison function,
 * which is called with two arguments that point to the objects being compared.
 * The comparison function must return an integer less than, equal to, or greater than zero if the first argument is considered to be respectively
 * less than, equal to, or greater than the second. If two members compare as equal, their order in the sorted array is undefined.
 *
 * \param base Points to the start of the array to be heapified
 * \param nmemb Number of members in the specified array
 * \param size In bytes of each member in the specified array
 * \param compar Points to a comparison function
 */
void heapify(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *));

/*!
 * \fn void heapify_a(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *), int * pos, int (*hash)(const void *))
 * \brief Rearrange the elements in a specified array in such a way that they form a heap according to a specified comparison function, keeping a specified array of positions updated
 *
 * The contents of the specified array are partially sorted in ascending order according to a comparison function,
 * which is called with two arguments that point to the objects being compared.
 * The comparison function must return an integer less than, equal to, or greater than zero if the first argument is considered to be respectively
 * less than, equal to, or greater than the second.  If two members compare as equal, their order in the sorted array is undefined.
 *
 * An array of positions is an array \c pos such that \c pos[hash[e]] is the index of element \c e in the heap.
 * This function keeps this property valid including for the elements that are moved within the array.
 *
 * \param base Points to the start of the array to be heapified
 * \param nmemb Number of members in the specified array
 * \param size In bytes of each member in the specified array
 * \param compar Points to a comparison function
 * \param pos Array that indicates the position of each element in the heap
 * \param hash Hash function that gives the index of each element in \c pos
 */
void heapify_a(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *), int * pos,
		int (*hash)(const void *));
// The same as above with an additional argument passed to each call to the compar function
void heapify_r(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *), void * arg);

/*!
 * \fn int heapup(void *base, int i, size_t size,	int (*compar)(const void *, const void *))
 * \brief Move a specified element upward in the head
 *
 * \param base Points to the start of the array to be heapified
 * \param i Element to move upward
 * \param size In bytes of each member in the specified array
 * \param compar Points to a comparison function
 */
int heapup(void *base, int i, size_t size,	int (*compar)(const void *, const void *));

/*!
 * \fn int heapup_a(void *base, int i, size_t size, int(*compar)(const void *, const void *), int * pos, int(*hash)(const void *))
 *
 * \brief Move a specified element upward in the head, keeping a specified array of positions updated
 *
 * An array of positions is an array \c pos such that \c pos[hash[e]] is the index of element \c e in the heap.
 * This function keeps this property valid including for the elements that are moved within the array.
 *
 * \param base Points to the start of the array to be heapified
 * \param i Element to move upward
 * \param size In bytes of each member in the specified array
 * \param compar Points to a comparison function
 * \param pos Array that indicates the position of each element in the heap
 * \param hash Hash function that gives the index of each element in \c pos
 */
int heapup_a(void *base, int i, size_t size, int (*compar)(const void *, const void *), int * pos,
		int (*hash)(const void *));

int heapup_r(void *b, int i, size_t size,
		int (*compar)(const void *, const void *, void *), void * arg);

int heapdown(void *base, int i, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *));
int heapdown_a(void *b, int i, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *), int * pos,
		int (*hash)(const void *));
int heapdown_r(void *base, int i, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *), void * arg);

// Add a new element <tt>x</tt> to a specified array, which is assumed to be partially sorted in a heap manner.
// It is also assumed that the specified array has enough place for the new element.
// The remaining arguments are similar to the ones of <tt>heapify</tt>.
void heapoffer(void *x, void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *));
void heapoffer_a(void *x, void *b, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *), int * pos,
		int (*hash)(const void *));
void heapoffer_r(void *x, void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *), void * arg);

/*!
 * \fn void heappoll(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
 * \brief Remove the first element of the heap
 *
 * \param base Points to the start of the array to be heapified
 * \param nmemb Number of members in the specified array
 * \param size In bytes of each member in the specified array
 * \param compar Points to a comparison function
 */
void heappoll(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *));

/*!
 * \fn void heappoll_a(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *), int * pos, int (*hash)(const void *))
 * \brief Remove the first element of the head, keeping a specified array of positions updated
 *
 * An array of positions is an array \c pos such that \c pos[hash[e]] is the index of element \c e in the heap.
 * This function keeps this property valid including for the elements that are moved within the array.
 *
 * \param base Points to the start of the array to be heapified
 * \param nmemb Number of members in the specified array
 * \param size In bytes of each member in the specified array
 * \param compar Points to a comparison function
 * \param pos Array that indicates the position of each element in the heap
 * \param hash Hash function that gives the index of each element in \c pos
 */
void heappoll_a(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *), int * pos,
		int (*hash)(const void *));
void heappoll_r(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *), void * arg);

#endif /*HEAP_H_*/
