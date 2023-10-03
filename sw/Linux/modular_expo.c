unsigned long long modPow(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;

    while (exp > 0) {
        // If the least significant bit of exp is 1, multiply result with base
        if (exp & 1) {
            result = (result * base) % mod;
        }

        // Square the base and reduce modulo mod
        base = (base * base) % mod;

        // Right-shift exp to check the next bit
        exp >>= 1;
    }

    return result;
}