#ifndef PROCESSING_H_
#define PROCESSING_H_

#include <stdio.h>
#include <stdlib.h>

#include "Blur.h"
#include "PreProcessing.h"
#include "SdlHelperFunctions.h"
#include "Rescaling.h"
#include "Drawing.h"
#include "Component.h"
#include <string.h>
#include <math.h>
#include "Vector.h"
#include "Morph.h"
#include <unistd.h>
#include <sys/wait.h>
#include "Hough.h"

void GetGrid(char *imgPath);

#endif
