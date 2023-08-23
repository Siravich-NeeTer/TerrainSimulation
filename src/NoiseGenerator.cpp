#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator(unsigned int seed)
{
    m_Repeat = 0;
	std::shuffle(p.begin(), p.end(), std::default_random_engine(seed));
}

double NoiseGenerator::Perlin(double x, double y, double z)
{
    if (m_Repeat > 0)
    {
        x = std::fmod(x, m_Repeat);
        y = std::fmod(y, m_Repeat);
        z = std::fmod(z, m_Repeat);
    }

    // Convert x/y/z to int within range [0, 255]
    int xi = (int)x & 255;
    int yi = (int)y & 255;
    int zi = (int)z & 255;

    double xf = x - (int)x;
    double yf = y - (int)y;
    double zf = z - (int)z;

    double u = Fade(xf);
    double v = Fade(yf);
    double w = Fade(zf);

    /*
    * a represent x/y/z
    * b represent x+1/y+1/z+1
        aaa                     baa
            -------------------
            |                 |
            |                 |
            |                 |
            |                 |
            |                 |
            |                 |
            |                 |
            -------------------
        aba                     bba
    */
    int aaa, aba, aab, abb, baa, bba, bab, bbb;
    aaa = p[p[p[          xi ]+          yi ]+          zi ];
    aba = p[p[p[          xi ]+Increment(yi)]+          zi ];
    aab = p[p[p[          xi ]+          yi ]+Increment(zi)];
    abb = p[p[p[          xi ]+Increment(yi)]+Increment(zi)];
    baa = p[p[p[Increment(xi)]+          yi ]+          zi ];
    bba = p[p[p[Increment(xi)]+Increment(yi)]+          zi ];
    bab = p[p[p[Increment(xi)]+          yi ]+Increment(zi)];
    bbb = p[p[p[Increment(xi)]+Increment(yi)]+Increment(zi)];
    
    double x1, x2, y1, y2;
    // ------------------ Lerp on (Z) plane ---------------------
    x1 = glm::mix(  Gradient(aaa, xf, yf, zf),
                    Gradient(baa, xf - 1, yf, zf),
                    u);
    x2 = glm::mix(  Gradient(aba, xf, yf - 1, zf),
                    Gradient(bba, xf - 1, yf - 1, zf),
                    u);
    y1 = glm::mix(x1, x2, v);
    // ----------------------------------------------------------

    // ---------------- Lerp on (Z - 1) plane -------------------
    x1 = glm::mix(  Gradient(aab, xf, yf, zf - 1),
                    Gradient(bab, xf - 1, yf, zf - 1),
                    u);
    x2 = glm::mix(  Gradient(abb, xf, yf - 1, zf - 1),
                    Gradient(bbb, xf - 1, yf - 1, zf - 1),
                    u);
    y2 = glm::mix(x1, x2, v);
    // ----------------------------------------------------------


    //std::cout << (glm::mix(y1, y2, w) + 1) / 2.0f << "\n";
    // Return Lerp between [0.0f, 1.0f]
    return (glm::mix(y1, y2, w) + 1) / 2.0f;

}
double NoiseGenerator::Perlin(double x, double y, double z, int octaves, double persistance)
{
    double total = 0;
    double frequency = 1;
    double amplitude = 1;
    double maxValue = 0;
    for (int i = 0; i < octaves; ++i)
    {
        total += Perlin(x * frequency, y * frequency, z * frequency) * amplitude;
        
        maxValue += amplitude;

        amplitude *= persistance;
        frequency *= 2.0f;
    }

    return total / maxValue;
}
double NoiseGenerator::Perlin(double x, double y)
{
    return -1;
}
double NoiseGenerator::Perlin(double x, double y, int octaves, double persistance)
{
    return -1;
}

// ---------------------- Private Function ----------------------
double NoiseGenerator::Fade(double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}
double NoiseGenerator::Gradient(int hash, double x, double y, double z)
{
    int h = hash & 15;									// Take the hashed value and take the first 4 bits of it (15 == 0b1111)
    double u = h < 8 /* 0b1000 */ ? x : y;				// If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.

    double v;

    if (h < 4 /* 0b0100 */)								// If the first and second significant bits are 0 set v = y
        v = y;
    else if (h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/)// If the first and second significant bits are 1 set v = x
        v = x;
    else 												// If the first and second significant bits are not equal (0/1, 1/0) set v = z
        v = z;

    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v); // Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.

}
int NoiseGenerator::Increment(int x)
{
    x++;
    if (m_Repeat > 0)
        x %= m_Repeat;
    return x;
}
