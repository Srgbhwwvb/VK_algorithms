#include <iostream>
#include <random>
#include <assert.h>
//6_4.Реализуйте стратегию выбора опорного элемента “случайный элемент”.Функцию 
//Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

using namespace std;

template<class T>
bool isLess(const T& l, const T& r)
{
    return l < r;
}

template<class T>
int partition(T arr[], int l, int r, bool (*isLess)(const T&, const T&)) {
    int randomelem = l + rand() % (r - l + 1);
    T x = arr[randomelem];
    swap(arr[randomelem], arr[r]);
    int i = r;
    for (int j = r-1; j >= l; j--) {
        if (!isLess(arr[j], x)) {
            i--;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i], arr[r]);
    return i;
}

template<class T>
T kthSmallest(T arr[], int l, int r, int k, bool (*isLess)(const T&, const T&) ){
    while (l <= r) {
        int pos = partition(arr, l, r, isLess);
        if (pos == k - 1) {
            return arr[pos];
        }
        if (pos < k - 1) {
            l = pos + 1;
        }
        else {
            r = pos - 1;
        }
    }
    return -1;
}


int main() {
    int n = 0;
    cin >> n;
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }
    cout << kthSmallest(arr, 0, n - 1, n * 0.1 + 1, isLess) << endl;
    cout << kthSmallest(arr, 0, n - 1, n * 0.5 + 1, isLess) << endl;
    cout << kthSmallest(arr, 0, n - 1, n * 0.9 + 1, isLess);
    delete[] arr;
    
    return 0;
}