
uint hash(uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}
static uint rng_state = hash(6567);
uint rand()
{
	
    // Xorshift algorithm from George Marsaglia's paper
    rng_state ^= (rng_state << 13);
    rng_state ^= (rng_state >> 17);
    rng_state ^= (rng_state << 5);
    return rng_state;
}

float randfloat(float low, float high)
{
	float f0 = float(rand()) * ((high-low) / 4294967296.0) + low;
	return f0;
}

