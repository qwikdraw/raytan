#include "Formula.hpp"

glm::dvec4 formula2d::gradientRed(double x, double y)
{
	x -= 0.5;
	y -= 0.5;
	double radius = glm::sqrt(x * x + y * y) / 1.41421;
	return glm::dvec4(glm::pow(radius, 1.5), 0.0, 0.0, 0.0);
}

glm::dvec4 formula2d::gradientGreen(double x, double y)
{
	x -= 0.5;
	y -= 0.5;
	double radius = glm::sqrt(x * x + y * y) / 1.41421;
	return glm::dvec4(0.0, glm::pow(radius, 1.5), 0.0, 0.0);
}

glm::dvec4 formula2d::gradientBlue(double x, double y)
{
	x -= 0.5;
	y -= 0.5;
	double radius = glm::sqrt(x * x + y * y) / 1.41421;
	return glm::dvec4(0.0, 0.0, glm::pow(radius, 1.5), 0.0);
}

static double	checkerboardModulo(double x)
{
	// if x is -0.4 then the result is -0.4 - -1.0 = 0.6! A positive number! YAY!
	return x - glm::floor(x);
}

glm::dvec4 formula2d::checkerboard(double x, double y)
{
	// The '4's are for the checkerboard scaling - adjust to have different checkerboard values
	double res = (checkerboardModulo(y * 4) < 0.5) ^ (checkerboardModulo(x * 4) < 0.5);
	return res == 1 ? glm::dvec4(1.0, 1.0, 1.0, 0.0) : glm::dvec4(0.0, 0.0, 0.0, 0.0);
}

glm::dvec4 formula2d::sineWave(double x, double y)
{
	x *= 2;
	x *= glm::pi<double>() * 2.0 * 2.0;
	y *= glm::pi<double>() * 2.0 * 2.0;

	double a = (sin(x * 2.0) + 1.0) / 2.0;
	double b = (sin(y * 3.0) + 1.0) / 2.0;
	double c = (cos(x * 5.0) + 1.0) / 2.0;
	double d = (cos(y * 8.0) + 1.0) / 2.0;

	glm::dvec4 out = glm::dvec4(1);

	if (a > b && a > c)
		out.r = a;
	if (a > b * 2.0 + c - d)
		out.g = b;
	if (b - d + c * 3.0 > a)
		out.b = c * b;
	if  (a + d * 7.0 > c + b * 2.0)
		out *= 0.8;
	out *= (a + b + c + d) / 4.0;

	return glm::dvec4(glm::dvec3((out.x + out.y + out.z) / 3.0), 1.0);
}

// The Julia fractal
glm::dvec4		formula2d::julia(double x, double y)
{
	double	numIterations = 60.0;
	double	diameter = 5.0;

	double o_r;
	double o_i;
	double i = 0;
	x = (x - 0.5) * 3.0;
	y = (y - 0.5) * 3.0;
	while (i++ < numIterations)
	{
		o_r = x;
		o_i = y;
		x = o_r * o_r - o_i * o_i + 0.38;	// Real Constant
		y = 2.0 * o_r * o_i + -0.19;		// Imaginary Constant
		if ((x * x + y * y) > diameter)
			break ;
	}
	double color = i / numIterations;
	return glm::dvec4(1.0 - color, color, color * (i < numIterations), 0.0);
}

static double	perlinGrad(int hash, double x, double y, double z)
{
    switch(hash & 15)
    {
        case 0: return  x + y;
        case 1: return -x + y;
        case 2: return  x - y;
        case 3: return -x - y;
        case 4: return  x + z;
        case 5: return -x + z;
        case 6: return  x - z;
        case 7: return -x - z;
        case 8: return  y + z;
        case 9: return -y + z;
        case 10: return  y - z;
        case 11: return -y - z;
        case 12: return  y + x;
        case 13: return -y + z;
        case 14: return  y - x;
        case 15: return -y - z;
        default: return 0; // shouldn't happen
    }
    return 0;
}

// Linear Interpolation
static double	perlinLerp(double t, double a, double b) {
    return a + t * (b - a);
}

//	Smoothes the final output
static double	perlinFade(double t) {
	return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

static double	perlinCalc(double x, double y, double z)
{
	// Convert to int
	int intX = ((int)(glm::floor(x))) & 255;
	int intY = ((int)(glm::floor(y))) & 255;
	int intZ = ((int)(glm::floor(z))) & 255;

	x -= (int)glm::floor(x);
	y -= (int)glm::floor(y);
	z -= (int)glm::floor(z);

	double u = perlinFade(x);
	double v = perlinFade(y);
	double w = perlinFade(z);

	int aaa = perlinPerm[perlinPerm[perlinPerm[intX] + intY] + intZ];
	int aba = perlinPerm[perlinPerm[perlinPerm[intX] + intY + 1] + intZ];
	int aab = perlinPerm[perlinPerm[perlinPerm[intX] + intY] + intZ + 1];
	int abb = perlinPerm[perlinPerm[perlinPerm[intX] + intY + 1] + intZ + 1];
	int baa = perlinPerm[perlinPerm[perlinPerm[intX + 1] + intY] + intZ];
	int bba = perlinPerm[perlinPerm[perlinPerm[intX + 1] + intY + 1] + intZ];
	int bab = perlinPerm[perlinPerm[perlinPerm[intX + 1] + intY] + intZ + 1];
	int bbb = perlinPerm[perlinPerm[perlinPerm[intX + 1] + intY + 1] + intZ + 1];

	double x1 = perlinLerp(	u,
							perlinGrad(aaa, x, y, z),
							perlinGrad(baa, x - 1.0, y, z));
	double x2 = perlinLerp(	u,
							perlinGrad(aba, x, y - 1.0, z),
							perlinGrad(bba, x - 1.0, y - 1.0, z));
	double y1 = perlinLerp(v, x1, x2);

	x1 = perlinLerp( u,
					 perlinGrad(aab, x, y, z - 1.0),
					 perlinGrad(bab, x - 1.0, y, z - 1.0));
	x2 = perlinLerp( u,
					 perlinGrad(abb, x, y - 1.0, z - 1.0),
					 perlinGrad(bbb, x - 1.0, y - 1.0, z - 1.0));
	double y2 = perlinLerp(v, x1, x2);

	return perlinLerp(w, y1, y2);
}

// Normal Sampling
glm::dvec4		formula3d::perlinNoise(double x, double y, double z)
{
	glm::dvec4 out;
	double scale = 20.0;

	x *= scale;
	y *= scale;
	z *= scale;
	out.x = perlinCalc(x, y, z);
	out.y = perlinCalc(y, z, x);
	out.z = perlinCalc(z, x, y);
	out.w = 0.0;

	return out;
}

// A weird effect
glm::dvec4		formula3d::vortex(double x, double y, double z)
{
	glm::dvec4 g = formula3d::perlinNoise(x, y, z) * 0.5;

	g -= glm::floor(g);
	return glm::dvec4(0.5 * g.r, 0.3 * g.g, 1.0 - g.b, 0.0);
}

