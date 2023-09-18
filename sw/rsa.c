#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int gcd(int a, int h)
{
    int temp;
    while (1) {
        temp = a % h;
        if (temp == 0)
            return h;
        a = h;
        h = temp;
    }
}

void printArray(char* arr, int len)
{
    int i;
    for (i = 0; i < len; i++){
        printf("%d ", arr[i]);
    }
}

char* encrypt(char* msg, int len, int e, int n){
    char * encry_msg = malloc(len * sizeof(char));
    for(int i = 0; i < len; i++){
        double c = pow(msg[i], e);
        c = fmod(c, n);
        encry_msg[i] = (char)c;
    }
    return encry_msg;
}

char* decrypt(char* c, int len, int d, int n){
    char * decry_msg = malloc(len * sizeof(char));
    for(int i = 0; i < len; i++){
        double m = pow(c[i], d);
        m = fmod(m, n);
        decry_msg[i] = (char)m;
    }
    return decry_msg;
}


int main(){
    int p = 3;
    int q = 7;

    int n = p * q;


    double e = 2;
    double phi = (p - 1) * (q - 1);
    while (e < phi) {
        // e must be co-prime to phi and
        // smaller than phi.
        if (gcd(e, phi) == 1)
            break;
        else
            e++;
    }

    int k = 2; // A constant value
    double d = (1 + (k * phi)) / e;

    // char msg = 12;
    char msg[2] = {12, 12};
    int len = 2;
    printf("\nMessage data: ");
    printArray(msg, len);

    char* cryp_msg, *org_msg;
    cryp_msg = encrypt(msg, len, e, n);
    printf("\nEncrypted data: ");
    printArray(cryp_msg, len);
    org_msg = decrypt(cryp_msg, len, d, n);
    // printf("\nOriginal Message Sent = %d", org_msg);
    printf("\nOriginal Message Sent: ");
    printArray(org_msg, len);

}