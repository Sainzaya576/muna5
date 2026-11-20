#include <stdlib.h>
#include <time.h>
#include "my_qsort.h"

#ifndef CUTOFF
#define CUTOFF 10
#endif

static int init_seed = 0;

static void insertion_sort(int a[], int lo, int hi)
{
        for (int i = lo + 1; i <= hi; i++) {
                int key = a[i];
                int j = i - 1;
                while (j >= lo && a[j] > key) {
                        a[j + 1] = a[j];
                        j--;
                }
                a[j + 1] = key;
        }
}

static void swap(int *a, int *b)
{
        int tmp = *a;
        *a = *b;
        *b = tmp;
}

static void random_shuffle(int a[], int lo, int hi)
{
        if (lo >= hi) {
                return;
        }
        if (init_seed == 0) {
                srand((unsigned int)time(NULL));
                init_seed = 1;
        }
        int len = hi - lo + 1;
        for (int i = lo; i <= hi; i++) {
                int j = lo + rand() % len;
                swap(&a[i], &a[j]);
        }
}

static int partition(int a[], int lo, int hi)
{
        int pivot = a[lo];
        int i = lo;
        int j = hi + 1;

        while (1) {
                while (a[++i] < pivot) {
                        if (i == hi) break;
                }
                while (a[--j] > pivot) {
                        if (j == lo) break;
                }
                if (i >= j) break;
                swap(&a[i], &a[j]);
        }
        swap(&a[lo], &a[j]);
        return j;
}

static void _single_pivot_qsort(int a[], int lo, int hi)
{
        if (lo >= hi) {
                return;
        }
        if (hi - lo + 1 <= CUTOFF) {
                insertion_sort(a, lo, hi);
                return;
        }
        int p = partition(a, lo, hi);
        _single_pivot_qsort(a, lo, p - 1);
        _single_pivot_qsort(a, p + 1, hi);
}

void single_pivot_qsort(int a[], int lo, int hi)
{
        random_shuffle(a, lo, hi);
        _single_pivot_qsort(a, lo, hi);
}

static void _dual_pivot_qsort(int a[], int lo, int hi)
{
        if (lo >= hi) {
                return;
        }
        if (hi - lo + 1 <= CUTOFF) {
                insertion_sort(a, lo, hi);
                return;
        }

        if (a[lo] > a[hi]) {
                swap(&a[lo], &a[hi]);
        }

        int p = a[lo];
        int q = a[hi];

        int lt = lo + 1;
        int gt = hi - 1;
        int k = lt;

        while (k <= gt) {
                if (a[k] < p) {
                        swap(&a[k], &a[lt]);
                        lt++;
                        k++;
                } else if (a[k] > q) {
                        while (k < gt && a[gt] > q) {
                                gt--;
                        }
                        swap(&a[k], &a[gt]);
                        gt--;
                        if (a[k] < p) {
                                swap(&a[k], &a[lt]);
                                lt++;
                        }
                        k++;
                } else {
                        k++;
                }
        }

        lt--;
        gt++;
        swap(&a[lo], &a[lt]);
        swap(&a[hi], &a[gt]);

        _dual_pivot_qsort(a, lo, lt - 1);
        if (p < q) {
                _dual_pivot_qsort(a, lt + 1, gt - 1);
        }
        _dual_pivot_qsort(a, gt + 1, hi);
}

void dual_pivot_qsort(int a[], int lo, int hi)
{
        random_shuffle(a, lo, hi);
        _dual_pivot_qsort(a, lo, hi);
}
