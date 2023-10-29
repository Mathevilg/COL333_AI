#include<bits/stdc++.h>
using namespace std;



double findValue(double f){
    int tmp = f / 0.0001;
    double delta = f - (tmp * 0.0001);
    if (delta < 0.00004999999) return (tmp*0.0001);
    else return (tmp+1)*0.0001;
}



void print(double arr[], int len){
    for (int i=0; i<len; i++) cout << arr[i] << " ";
    cout << endl;
}

void convert(double pmf[], int size){
    double sum = 1.0000;
    int hash = 0;
    print(pmf, size);
    for (int i=0; i<size; i++){
        if (pmf[i] < 0.00015) {
            sum -= pmf[i];
            pmf[i] = 0.0001;
        }
        else {
            hash += (1 << i);
        }
    }
    print(pmf, size);
    for (int i=0; i<size; i++){
        if (hash && (1 << i)){
            pmf[i] /= sum;
            pmf[i] = findValue(pmf[i]);
        }
    }
    print(pmf, size);
    sum = 1.0000;
    int maxInd = max_element(pmf, pmf +size) - pmf;
    for (int i=0; i<size; i++){
        sum -= pmf[i];
    }
    pmf[maxInd] += sum;
    print(pmf, size);
}



int main(){
    double pmf[3] = {0.00086, 0.4997, 0.4998}  ;
    convert(pmf, 3);
    print(pmf, 3);
}