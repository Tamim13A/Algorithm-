#include<iostream>
using namespace std;

//swap 2 element
void swap(int * a,int *b){
    int temp= *a;
    *a= *b;
    *b=temp;
}
//partition function
int partition(int arr[],int low,int high,int n){
    int pivot=arr[high];
    int i=low-1;
    for(int j=low;j<high;j++){
        if(arr[j]<pivot){
            i++;
            swap(&arr[i],&arr[j]);
        }
    }
    swap(&arr[i+1], &arr[high]);
    return i+1;
}
//Quick sort
void quickSort(int arr[],int low,int high,int n){
    if(low<high){
        int pi=partition(arr,low,high,n);
        //output each partiton
        cout<<"Array after partitioning around pivot " << arr[pi] << " : ";
        for(int i=0;i<n;i++){
            cout<<arr[i]<< " ";
        }
        cout<<endl;
        //calls for L&R partition
        quickSort(arr,low,pi-1,n);
        quickSort(arr,pi+1,high,n);
    }
}
int main(){
    int n;
    cout<<"Enter the number of elements: ";
    cin>>n;
    int arr[n];
    //input arr element
    cout<<"Enter " << n << " elements: "<<endl;
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    cout<<"\nInitial Array: ";
    for(int i=0;i<n;i++){
        cout<<arr[i] << " ";
    }
    cout<< "\n\n";
    //perform QS
    quickSort(arr,0,n-1,n);
    //Final sorted arr
    cout<<"\nSorted Array: ";
    for(int i=0;i<n;i++){
        cout<< arr[i]<< " ";
    }
    cout<<endl;

    return 0;
}