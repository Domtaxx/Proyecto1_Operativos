

unsigned char* decrypt(unsigned int* c, int len, int d, int n){
    unsigned char * decry_msg = malloc(len * sizeof(char));
    for(int i = 0; i < len; i++){
        unsigned long long m = modPow(c[i], d, n);
        decry_msg[i] = (unsigned char)m;
    }
    return decry_msg;
}

unsigned int single_decrypt(unsigned int c, int d, int n){
    unsigned long long m = modPow(c, d, n);
    return (unsigned int)m;
}
