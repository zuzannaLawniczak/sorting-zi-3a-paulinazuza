#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

unsigned int ns[] = { 10, 100, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};

void fill_increasing(int *t, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++)
    {
        t[i] = i;
    }
}


void fill_decreasing(int *t, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++)
    {
        t[i] = n - i - 1;
    }
}


void fill_vshape(int *t, unsigned int n)
{
    int v;
    v = n / 2;
    for(int i = 0; i < v; i++)
    {
        t[i] = (v - i) * 2;
    }
    for(int i = v; i < n; i++)
    {
        t[i] = (i - v) * 2 + 1;
    }
}


// SELECTION SORT
void swap(int *t, int i, int j)
{
    int tmp = t[i];
    t[i] = t[j];
    t[j] = tmp;
}

int argmin(int *t, int s, int n)
{
    int argmin = s;
    for(int i = s; i < n; i++)
    {
        if(t[i] < t[argmin]) argmin = i;
    }
    return argmin;
}

void selection_sort(int *t, unsigned int n)
{
    int i, j;
    for(i = 0; i < n; i++)
    {
        j = argmin(t, i, n);
        swap(t, i, j);
    }
}

// INSERTION SORT
void insertion_sort(int *t, unsigned int n) {
    int key, i;
    for(int j = 1; j < n; j++)
    {
        key = t[j];
        i = j;
        while(i > 0 && t[i-1] > key)
        {
            t[i] = t[i-1];
            i--;
        }
        t[i] = key;
    }
}

// QUICK SORT 
int partition(int *t, int p, int r)
{
    int x, i;
    x = t[r];
    i = p - 1;
    for(int j = p; j <= r - 1; j++)
    {
        if(t[j] < x)
        {
            i = i + 1;
            swap(t, i, j);
        }
    }
    i = i + 1;
    swap(t, i, r);
    return i;
}

int random_partition(int *t, int p, int r)
{
    int i = p + (rand() % (r - p));
    swap(t, i, r);
    return partition(t, p, r);
}

void quick_sort(int *t, int p, int r)
{
    int q;
    if(p < r)
    {
        q = partition(t, p, r); //dzielimy tablice na dwie czesci; q oznacza punkt podzialu
        quick_sort(t, p, q - 1); //wywolujemy rekurencyjnie quicksort dla pierwszej czesci tablicy
        quick_sort(t, q + 1, r); //wywolujemy rekurencyjnie quicksort dla drugiej czesci tablicy
    }
}

void quick_sort(int *t, unsigned int n)
{
    quick_sort(t, 0, n - 1);
}
// HEAP SORT 

void heapify(int *t, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if(l < n && t[l] > t[largest]) largest = l;
    if(r < n && t[r] > t[largest]) largest = r;
    if(largest != i){
        swap(t, i, largest);
        heapify(t, n, largest); //rekurencyjnie
    }
}


void heap_sort(int *t, unsigned int n) {
    for(int i = n / 2 - 1; i >= 0; i--){ //budowanie stosu, zmiana układu tablicy
        heapify(t, n, i);
    }
    for(int i = n - 1; i >= 0; i--){ //wypakowywanie elemntów ze stosu jeden po drugim
        swap(t, 0, i); 
        heapify(t, i, 0); //wywołuje max
    }
}

// LICZBY LOSOWE
void fill_random(int *t, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        t[i] = rand();
    }
}

void is_random(int *t, unsigned int n) {
    return;
}

void is_increasing(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] > t[i - 1]);
    }
}

void is_decreasing(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] < t[i - 1]);
    }
}

void is_vshape(int *t, unsigned int n) {
    int *begin = t;
    int *end = t + n - 1;

    while (end - begin > 1) {
        assert(*begin > *end);
        begin++;
        assert(*end > *begin);
        end--;
    }
}

void is_sorted(int *t, unsigned int n) {
    for (unsigned int i = 1; i < n; i++) {
        assert(t[i] >= t[i - 1]);
    }
}

void (*fill_functions[])(int *, unsigned int) = { fill_random, fill_increasing, fill_decreasing, fill_vshape };
void (*check_functions[])(int *, unsigned int) = { is_random, is_increasing, is_decreasing, is_vshape };
void (*sort_functions[])(int *, unsigned int) = { selection_sort, insertion_sort, quick_sort, heap_sort };

char *fill_names[] = { "Random", "Increasing", "Decreasing", "V-Shape" };
char *sort_names[] = { "Selection sort", "Insertion sort", "Quick sort", "Heap sort" };

int main()
{
    FILE * data;
    data = fopen("data2.txt", "w");
    for (unsigned int i = 0; i < sizeof(sort_functions) / sizeof(*sort_functions); i++) {
        void (*sort)(int *, unsigned int) = sort_functions[i];

        for (unsigned int j = 0; j < sizeof(fill_functions) / sizeof(*fill_functions); j++) {
            void (*fill)(int *, unsigned int) = fill_functions[j];
            void (*check)(int *, unsigned int) = check_functions[j];

            for (unsigned int k = 0; k < sizeof(ns) / sizeof(*ns); k++) {
                unsigned int n = ns[k];
                int *t = (int *) malloc(n * sizeof(*t));

                fill(t, n);
                check(t, n);

                clock_t begin = clock();
                sort(t, n);
                clock_t end = clock();
                is_sorted(t, n);

                fprintf(data, "%s, %s\t%u\t%f\n", sort_names[i], fill_names[j], n, (double)(end - begin) / (double) CLOCKS_PER_SEC);
                free(t);
            }
        }
    }
    fclose(data);
    return 0;
}
