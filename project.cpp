#include<iostream>
#include<math.h>
using namespace std;

double readReal(string q="Enter a real number: "){ //Ham doc so thuc
    cout << q;
    double f;
    cin >> f;
    return f;
}
int readInt(string q="Enter a integer number: "){ // Ham doc so nguyen
    cout << q;
    int i;
    cin >> i;
    return i;
}

void increase(int &n){ // Ham tang 1 don vi
    n++;
}

void decrease(int &n){ // Ham giam 1 don vi
    n--;
}

double measureP(double x, int n){
    double P = 2018 * pow(x, n);
    for(int i = 2; i <=n; ++i)
        P += pow(x, i) / pow (3.0, i-1);
    return P;

}
int sumPositiveGreaterFive(int n){
    int sum = 0;
    for(int i = 5; i <= n; ++i)
        sum += (i%2)?0:i;
    return sum;
}
int main(int argc, char const *argv[])
{
    double x = readReal(); // Doc bien thuc x
    int n = readInt();// Dc bien nguyen n
    int m = readInt();// Doc bien nguyen m
    increase(n); // Tang n len 1 don vi
    increase(m); // Tang m len 1 don vi
    cout << "n: " << n << ", m: " << m << endl;
    cout << "P(" << x << ", " << n << ") = " << measureP(x,n) << endl;
    int sumInNM = sumPositiveGreaterFive(m) - sumPositiveGreaterFive(n);
    if (sumInNM >= 0)
        cout << "Tong cac so chan lon hon 5 trong doan ["<<n<<","<<m<<"] bang " << sumInNM << endl;
    else
        cout << "Loi, hay nhap n < m!" << endl;
    return 0;
}
