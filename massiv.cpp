#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
using namespace std;

int main()
{
    // пользователь задает массив
    int length; 
    cout<<"Vvedite razmer massiva\n";
    cin>>length;
    int array[length]; 
    cout<<"Vvedite elementu massiva\n";
    for(int i = 0; i < length; i++)
    cin >> array[i]; // Вводим с клавиатуры значение ячейки массива
    
    for (int i=0;i<length;i++)
    cout<<array[i]<<" ";
    cout<<endl;
    
    // массив задаем через случайные числа
    int n=5;
    int i,j,temp=0;
    setlocale(0,"rus");
    srand(time(0));
    // вывести заданный массив
    int a[n]={};
    for (i=0; i<n; i++)
    {
        a[i]=rand()%10;
        cout<<a[i]<<" ";
    }
    cout<<endl;
    
    // значения массива распределить по убыванию
    for(int i = 0; i < (n - 1); ++i)
    {            
        for(int j = 0; j < (n - 1); ++j)
        {     
            if (a[j + 1] > a[j]) 
            {
                temp = a[j + 1]; 
                a[j + 1] = a[j]; 
                a[j] = temp;
            }
        }
    }
    // вывести итог
    for (i=0;i<n;i++)
    cout<<a[i]<<" ";
    cout<<endl;
    
    int k = 3;
    k--;  //т.к. индексация с нуля
    for(int i = k; i < n-1;++i)
    a[i] = a[i+1];
    n--;

    cout << "Массив, после удаление 3-его элемента: ";
    for (i=0;i<n;i++)
    cout<<a[i]<<" ";
    
    return 0;
}
