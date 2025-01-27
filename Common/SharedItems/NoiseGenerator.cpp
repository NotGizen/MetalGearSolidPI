#include "NoiseGenerator.h"
#include <cmath>
//OG
static int numOctaves = 7, primeIndex = 0;
//static float persistence = 0.5f;
// //-------------//
//static int numOctaves = 1, primeIndex = 0;
static float persistence = 1.f;
static int primes[10][3] = {
{ 995615039, 600173719, 701464987 }, { 831731269, 162318869, 136250887 }, { 174329291, 946737083, 245679977 },
{ 362489573, 795918041, 350777237 }, { 457025711, 880830799, 909678923 }, { 787070341, 177340217, 593320781 },
{ 405493717, 291031019, 391950901 }, { 458904767, 676625681, 424452397 }, { 531736441, 939683957, 810651871 },
{ 997169939, 842027887, 423882827 }
};
float NoiseGenerator::Noise(int i, int x, int y)
{
    int n = x + y * 57; n = (n << 13) ^ n;
    const int a = primes[i][0], b = primes[i][1], c = primes[i][2];
    const int t = (n * (n * n * a + b) + c) & 0x7fffffff;
    return 1.0f - (float)t / 1073741824.0f;
}

float NoiseGenerator::SmoothedNoise(int i, int x, int y)
{
    const float corners = (Noise(i, x - 1, y - 1) + Noise(i, x + 1, y - 1) + Noise(i, x - 1, y + 1) + Noise(i, x + 1, y + 1)) / 16;
    const float sides = (Noise(i, x - 1, y) + Noise(i, x + 1, y) + Noise(i, x, y - 1) + Noise(i, x, y + 1)) / 8;
    const float center = Noise(i, x, y) / 4;
    return corners + sides + center;
}

float NoiseGenerator::Interpolate(float a, float b, float x)
{
    const float ft = x * 3.1415927f, f = (1 - cosf(ft)) * 0.5f;
    return a * (1 - f) + b * f;
}

float NoiseGenerator::InterpolatedNoise(int i, float x, float y)
{
    const int integer_X = (int)x, integer_Y = (int)y;
    const float fractional_X = x - (float)integer_X, fractional_Y = y - (float)integer_Y;
    const float v1 = SmoothedNoise(i, integer_X, integer_Y);
    const float v2 = SmoothedNoise(i, integer_X + 1, integer_Y);
    const float v3 = SmoothedNoise(i, integer_X, integer_Y + 1);
    const float v4 = SmoothedNoise(i, integer_X + 1, integer_Y + 1);
    const float i1 = Interpolate(v1, v2, fractional_X);
    const float i2 = Interpolate(v3, v4, fractional_X);
    return Interpolate(i1, i2, fractional_Y);
}

float NoiseGenerator::noise2D(float x, float y)
{
    float total = 0, frequency = (float)(2 << numOctaves), amplitude = 1;
    for (int i = 0; i < numOctaves; ++i)
    {
        frequency /= 2, amplitude *= persistence;
        total += InterpolatedNoise((primeIndex + i) % 10, x / frequency, y / frequency) * amplitude;
    }
    return total / frequency;
}

std::vector<float> NoiseGenerator::generatePerlinNoiseTexture(int width, int height)
{
    std::vector<float> noiseData(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float nx = (float)x / width;
            float ny = (float)y / height;
            noiseData[y * width + x] = (noise2D(nx * 10.0f, ny * 10.0f) + 1.0f) / 2.0f; // Normalize to [0, 1]
        }
    }
    return noiseData;
}
