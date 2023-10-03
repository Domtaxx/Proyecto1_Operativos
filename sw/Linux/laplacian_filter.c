
void applyLaplacianFilter(unsigned char* input, unsigned char* output, int width, int height) {
    int kernel[3][3] = {
        {0,  1,  0},
        {1, -4,  1},
        {0,  1,  0}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sum = 0;
            int index = y*width + x;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int pixelValue = input[(y + ky) * width + (x + kx)];
                    sum += pixelValue * kernel[ky + 1][kx + 1];
                }
            }
            sum = sum > 255 ? 255 : (sum < 0 ? 0 : sum);
            output[index] = (unsigned char)sum;
        }
    }
} 