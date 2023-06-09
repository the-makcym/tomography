#pragma once

#include "globals.h"
#include "geometry.h"

using namespace std;


Matrix radon(Area &area, Config &config);

Matrix radon_const(Area &area, Config &config);

Matrix convolution(Matrix &radon_im, Config &config);

real backprojection(Pnt &pnt, Matrix &conv, Config &config);

Matrix inv_radon(Matrix &radon_im, Config &config);
