#include <iostream>

using namespace std;

int P(int n, int a[1000]){
  int cntP=0;
  for (int i = 0; i < n - 1; i++) {//сортировка пузырьком
    for (int j = n - 2; j >= i; j--) {
      if (a[j] > a[j + 1]) {
        int t = a[j];
        a[j] = a[j + 1];
        a[j + 1] = t;
        cntP ++;//считаем число перестановок
        
      }
    }
  }
  return cntP;
}

int V(int n, int a[1000]){
  int cntV = 0;
  for (int i = 0; i < n-1; i++){//метод выбора минимального элемента
    int min = i;
    for (int j = i+1; j < n; j++){
      if (a[j] < a[min]){
        min = j;
      }
    }
    if (min != i){//если номер минимального элемента не равен i, то меняем местами a[i] и a[min]
      int t = a[i];
      a[i] = a[min];
      a[min] = t;
      cntV ++;
    }
  }
  return cntV;
}

int main(){
  const int n = 1000; 
  int summP = 0; 
  int summV = 0;
  for (int i = 0; i < 5; i++){// 5 - число создаваемых массивов
    int a[n];
    int a2[n];
    for (int j = 0; j < n; j++){
      a[j] = rand()%100;
      a2[j] = rand()%100;
    }
    summP += P(n,a);
    summV += V(n,a2);
  }
  cout << "Среднее число перестановок методом пузырька " << summP/5 << endl;
  cout << "Среднее число перестановок методом выбора " << summV/5 << endl;
}