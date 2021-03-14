#include <iostream>
#include<bits/stdc++.h>
#include<vector>
#include<map>
#include <chrono>
using namespace std;
using namespace std::chrono;
ofstream g("date.out");
bool check(vector<int> arr)
{
    for(int i = 0; i < arr.size()-1; ++i)
        if(arr[i] > arr[i+1])
            return 0;
    return 1;
}
//bubble sort
void bubble_sort(vector<int> &arr)
{
    int i,j;
    int n=arr.size();
    for(i=0;i<n-1;i++)
        for(j=0;j<n-i-1;j++)
        if(arr[j]>arr[j+1])
        swap(arr[j],arr[j+1]);
}
//count sort
void count_sort(vector<int> &arr)
{
    map<int,int>fr;
    for(int no:arr)
    {
        fr[no]++;
    }
    int i=0;
    for(auto p:fr)
    {
        while(p.second--)
        {
            arr[i++]=p.first;
        }
    }
}
//radix sort

void count_sort_radix(vector<int> &arr,int exp,int baza)
{
    vector<int> final_array;
    int i;
    vector<int> freq;
    freq.assign(baza,0);
    final_array.assign(arr.size(),0);
    for(i=0;i<arr.size();i++)
        freq[(arr[i]/exp)%baza]++;
    for(i=1;i<=baza-1;i++)
        freq[i]=freq[i]+freq[i-1];
    for(i=arr.size()-1;i>=0;i--)
    {
        final_array[freq[(arr[i]/exp)%baza]-1]=arr[i];
        freq[(arr[i]/exp)%baza]--;
    }
    for(i=0;i<arr.size();i++)
        arr[i]=final_array[i];
}
void radix_sort(vector<int> &arr,int baza)
{
    int maxi,exp;
    maxi=*max_element(arr.begin(), arr.end());
    for(exp=1;maxi/exp>0;exp *=baza)
        count_sort_radix(arr,exp,baza);
}
//merge sort
void combine(vector<int> &arr,int left,int middle,int right)
{
    int n1=middle-left+1;
    int n2=right-middle;
    int i,j;
    vector<int>left_arr;
    vector<int>right_arr;
    left_arr.assign(n1,0);
    right_arr.assign(n2,0);
    for(i=0;i<n1;i++)
        left_arr[i]=arr[left+i];
    for(j=0;j<n2;j++)
        right_arr[j]=arr[middle+1+j];
    i=0;
    j=0;
    int index=left;
    while(i<n1 && j<n2){
        if(left_arr[i]<=right_arr[j]){
            arr[index]=left_arr[i];
            i++;
        }
        else{
            arr[index]=right_arr[j];
            j++;
        }
        index++;
    }
    while(i<n1){
        arr[index]=left_arr[i];
        i++;
        index++;
    }
    while(j<n2){
        arr[index]=right_arr[j];
        j++;
        index++;
    }
}
void merge_sort(vector<int> &arr,int left,int right){
    int middle;
    if(left>=right){
        return;
    }
    middle=left+(right-left)/2;
    merge_sort(arr,left,middle);
    merge_sort(arr,middle+1,right);
    combine(arr,left,middle,right);
}
//quicksort cu pivotul ultimul element/random
int partitions(vector<int> &arr,int first,int last)
{
    int i,j;
    int piv=arr[last];
    i=first-1;
    for(j=first;j<=last-1;j++)
        if(arr[j]<piv)
    {
        i++;
        swap(arr[j],arr[i]);
    }
    i++;
    swap(arr[i],arr[last]);
    return i;
}
int partitions2(vector<int> &arr,int first,int last)
{
    srand(time(NULL));
    int random = first + rand()%(first - last);
    swap(arr[random], arr[last]);
    return partitions(arr,first,last);
}
void quicksort(vector<int> &arr,int first,int last)
{
    if(first<last)
    {
        int position=partitions(arr,first,last);
        quicksort(arr,first,position-1);
        quicksort(arr,position+1,last);
    }
}
void quicksort_rand(vector<int> &arr,int first,int last)
{
    if (first < last) {
        int pos = partitions2(arr,first,last);
        quicksort_rand(arr,first, pos - 1);
        quicksort_rand(arr, pos + 1,last);
    }
}
//printing function
void print(vector<int> &arr)
{
    int i;
    for(i=0;i<arr.size();i++)
        cout<<arr[i]<<" ";
}
int main()
{
    int tests,size;

    cin>>tests;
    for(int i=1;i<=tests;i++)
   {
       g<<"Test no:"<<i<<endl;
    //size=rand()%1000000;
    size=10000;
    vector<int> arr(size,0);
    srand(time(0));
    generate(arr.begin(),arr.end(),rand);
    vector<int>arr2;
    arr2=arr;

//bubble sort time
try{
    auto start = high_resolution_clock::now();
    //insert function here
    bubble_sort(arr2);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    if(check(arr2)==0)
        g<<"FAILED"<<endl;
    else
        g << "Time taken by bubblesort: "<< duration.count() << " microseconds" << endl;
        }
        catch (...) {
        g<<"It is not possible";
    }

    arr2=arr;
    try{
    auto start2 = high_resolution_clock::now();
    count_sort(arr2);
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop2 - start2);
    if(check(arr2)==0)
        g<<"FAILED"<<endl;
    else
        g << "Time taken by countsort: "<< duration2.count() << " microseconds" << endl;}
        catch (...) {
        g<<"It is not possible";
    }


    arr2=arr;
    try{
    auto start3 = high_resolution_clock::now();
    radix_sort(arr2,10);
     auto stop3 = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(stop3 - start3);
    if(check(arr2)==0)
        g<<"FAILED"<<endl;
    else
        g << "Time taken by radix_sort_base10: "<< duration3.count() << " microseconds" << endl;
   }
        catch (...) {
        g<<"It is not possible";
    }

    arr2=arr;
    try{
    auto start4 = high_resolution_clock::now();
    radix_sort(arr2,2);
    auto stop4 = high_resolution_clock::now();
    auto duration4 = duration_cast<microseconds>(stop4 - start4);
    if(check(arr2)==0)
        g<<"FAILED"<<endl;
    else
        g << "Time taken by radix_sort_base2: "<< duration4.count() << " microseconds" << endl;
   }
    catch (...) {
        g<<"It is not possible";
    }

    arr2=arr;
    try{
    auto start5 = high_resolution_clock::now();
    merge_sort(arr2,0,arr2.size()-1);
    auto stop5 = high_resolution_clock::now();
    auto duration5 = duration_cast<microseconds>(stop5 - start5);
    if(check(arr2)==0)
        g<<"FAILED"<<endl;
    else
        g << "Time taken by merge_sort: "<< duration5.count() << " microseconds" << endl;
   }
        catch (...) {
        g<<"It is not possible";
    }

    arr2=arr;
    try{
    auto start6 = high_resolution_clock::now();
    quicksort(arr2,0,arr2.size()-1);
    auto stop6 = high_resolution_clock::now();
    auto duration6 = duration_cast<microseconds>(stop6 - start6);
    if(check(arr2)==0)
        g<<"FAILED"<<endl;
    else
        g << "Time taken by quick_sort with pivot last element: "<< duration6.count() << " microseconds" << endl;
    }
    catch (...) {
        g<<"It is not possible";
    }

    arr2=arr;
try{
    auto start7 = high_resolution_clock::now();
    quicksort_rand(arr2,0,arr2.size()-1);
    auto stop7 = high_resolution_clock::now();
    auto duration7 = duration_cast<microseconds>(stop7 - start7);
    if(check(arr2)==0)
        g<<"FAILED"<<endl;
    else
        g << "Time taken by quicksort with pivot chosen random: "<< duration7.count() << " microseconds" << endl;
}
    catch (...) {
        g<<"It is not possible";
    }

    arr2=arr;
     try {

    auto start8 = high_resolution_clock::now();
    sort(arr2.begin(), arr2.end());
    auto stop8 = high_resolution_clock::now();
    auto duration8 = duration_cast<microseconds>(stop8 - start8);
    if(check(arr2)==0)
        g<<"FAILED"<<endl;
    else
        g << "Time taken by function sort from C++: "<< duration8.count() << " microseconds" << endl;

    }
    catch (...) {
        g<<"It is not possible";
    }


    g<<endl;

   }

    return 0;
}
