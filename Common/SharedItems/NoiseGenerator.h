#pragma once
#include <vector>
class NoiseGenerator
{
public:
    float Noise(int i, int x, int y);
    float SmoothedNoise(int i, int x, int y);
    float Interpolate(float a, float b, float x);
    float InterpolatedNoise(int i, float x, float y);
    float noise2D(float x, float y);
    std::vector<float> generatePerlinNoiseTexture(int width, int height);

private:
    
};
