// Source file for image class



// Include files

#include "R2/R2.h"
#include "R2Pixel.h"
#include "R2Image.h"
#include "svd.h"
#include <iostream>
#include <utility>
#include <vector>
#include <math.h>

////////////////////////////////////////////////////////////////////////
// Constructors/Destructors
////////////////////////////////////////////////////////////////////////


R2Image::
R2Image(void)
  : pixels(NULL),
    npixels(0),
    width(0),
    height(0)
{
}



R2Image::
R2Image(const char *filename)
  : pixels(NULL),
    npixels(0),
    width(0),
    height(0)
{
  // Read image
  Read(filename);
}



R2Image::
R2Image(int width, int height)
  : pixels(NULL),
    npixels(width * height),
    width(width),
    height(height)
{
  // Allocate pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);
}



R2Image::
R2Image(int width, int height, const R2Pixel *p)
  : pixels(NULL),
    npixels(width * height),
    width(width),
    height(height)
{
  // Allocate pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);

  // Copy pixels
  for (int i = 0; i < npixels; i++)
    pixels[i] = p[i];
}



R2Image::
R2Image(const R2Image& image)
  : pixels(NULL),
    npixels(image.npixels),
    width(image.width),
    height(image.height)

{
  // Allocate pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);

  // Copy pixels
  for (int i = 0; i < npixels; i++)
    pixels[i] = image.pixels[i];
}



R2Image::
~R2Image(void)
{
  // Free image pixels
  if (pixels) delete [] pixels;
}



R2Image& R2Image::
operator=(const R2Image& image)
{
  // Delete previous pixels
  if (pixels) { delete [] pixels; pixels = NULL; }

  // Reset width and height
  npixels = image.npixels;
  width = image.width;
  height = image.height;

  // Allocate new pixels
  pixels = new R2Pixel [ npixels ];
  assert(pixels);

  // Copy pixels
  for (int i = 0; i < npixels; i++)
    pixels[i] = image.pixels[i];

  // Return image
  return *this;
}


void R2Image::
svdTest(void)
{
	// fit a 2D conic to five points
	R2Point p1(1.2,3.5);
	R2Point p2(2.1,2.2);
	R2Point p3(0.2,1.6);
	R2Point p4(0.0,0.5);
	R2Point p5(-0.2,4.2);

	// build the 5x6 matrix of equations
	double** linEquations = dmatrix(1,5,1,6);

	linEquations[1][1] = p1[0]*p1[0];
	linEquations[1][2] = p1[0]*p1[1];
	linEquations[1][3] = p1[1]*p1[1];
	linEquations[1][4] = p1[0];
	linEquations[1][5] = p1[1];
	linEquations[1][6] = 1.0;

	linEquations[2][1] = p2[0]*p2[0];
	linEquations[2][2] = p2[0]*p2[1];
	linEquations[2][3] = p2[1]*p2[1];
	linEquations[2][4] = p2[0];
	linEquations[2][5] = p2[1];
	linEquations[2][6] = 1.0;

	linEquations[3][1] = p3[0]*p3[0];
	linEquations[3][2] = p3[0]*p3[1];
	linEquations[3][3] = p3[1]*p3[1];
	linEquations[3][4] = p3[0];
	linEquations[3][5] = p3[1];
	linEquations[3][6] = 1.0;

	linEquations[4][1] = p4[0]*p4[0];
	linEquations[4][2] = p4[0]*p4[1];
	linEquations[4][3] = p4[1]*p4[1];
	linEquations[4][4] = p4[0];
	linEquations[4][5] = p4[1];
	linEquations[4][6] = 1.0;

	linEquations[5][1] = p5[0]*p5[0];
	linEquations[5][2] = p5[0]*p5[1];
	linEquations[5][3] = p5[1]*p5[1];
	linEquations[5][4] = p5[0];
	linEquations[5][5] = p5[1];
	linEquations[5][6] = 1.0;

	printf("\n Fitting a conic to five points:\n");
	printf("Point #1: %f,%f\n",p1[0],p1[1]);
	printf("Point #2: %f,%f\n",p2[0],p2[1]);
	printf("Point #3: %f,%f\n",p3[0],p3[1]);
	printf("Point #4: %f,%f\n",p4[0],p4[1]);
	printf("Point #5: %f,%f\n",p5[0],p5[1]);

	// compute the SVD
	double singularValues[7]; // 1..6
	double** nullspaceMatrix = dmatrix(1,6,1,6);
	svdcmp(linEquations, 5, 6, singularValues, nullspaceMatrix);

	// get the result
	printf("\n Singular values: %f, %f, %f, %f, %f, %f\n",singularValues[1],singularValues[2],singularValues[3],singularValues[4],singularValues[5],singularValues[6]);

	// find the smallest singular value:
	int smallestIndex = 1;
	for(int i=2;i<7;i++) if(singularValues[i]<singularValues[smallestIndex]) smallestIndex=i;

	// solution is the nullspace of the matrix, which is the column in V corresponding to the smallest singular value (which should be 0)
	printf("Conic coefficients: %f, %f, %f, %f, %f, %f\n",nullspaceMatrix[1][smallestIndex],nullspaceMatrix[2][smallestIndex],nullspaceMatrix[3][smallestIndex],nullspaceMatrix[4][smallestIndex],nullspaceMatrix[5][smallestIndex],nullspaceMatrix[6][smallestIndex]);

	// make sure the solution is correct:
	printf("Equation #1 result: %f\n",	p1[0]*p1[0]*nullspaceMatrix[1][smallestIndex] +
										p1[0]*p1[1]*nullspaceMatrix[2][smallestIndex] +
										p1[1]*p1[1]*nullspaceMatrix[3][smallestIndex] +
										p1[0]*nullspaceMatrix[4][smallestIndex] +
										p1[1]*nullspaceMatrix[5][smallestIndex] +
										nullspaceMatrix[6][smallestIndex]);

	printf("Equation #2 result: %f\n",	p2[0]*p2[0]*nullspaceMatrix[1][smallestIndex] +
										p2[0]*p2[1]*nullspaceMatrix[2][smallestIndex] +
										p2[1]*p2[1]*nullspaceMatrix[3][smallestIndex] +
										p2[0]*nullspaceMatrix[4][smallestIndex] +
										p2[1]*nullspaceMatrix[5][smallestIndex] +
										nullspaceMatrix[6][smallestIndex]);

	printf("Equation #3 result: %f\n",	p3[0]*p3[0]*nullspaceMatrix[1][smallestIndex] +
										p3[0]*p3[1]*nullspaceMatrix[2][smallestIndex] +
										p3[1]*p3[1]*nullspaceMatrix[3][smallestIndex] +
										p3[0]*nullspaceMatrix[4][smallestIndex] +
										p3[1]*nullspaceMatrix[5][smallestIndex] +
										nullspaceMatrix[6][smallestIndex]);

	printf("Equation #4 result: %f\n",	p4[0]*p4[0]*nullspaceMatrix[1][smallestIndex] +
										p4[0]*p4[1]*nullspaceMatrix[2][smallestIndex] +
										p4[1]*p4[1]*nullspaceMatrix[3][smallestIndex] +
										p4[0]*nullspaceMatrix[4][smallestIndex] +
										p4[1]*nullspaceMatrix[5][smallestIndex] +
										nullspaceMatrix[6][smallestIndex]);

	printf("Equation #5 result: %f\n",	p5[0]*p5[0]*nullspaceMatrix[1][smallestIndex] +
										p5[0]*p5[1]*nullspaceMatrix[2][smallestIndex] +
										p5[1]*p5[1]*nullspaceMatrix[3][smallestIndex] +
										p5[0]*nullspaceMatrix[4][smallestIndex] +
										p5[1]*nullspaceMatrix[5][smallestIndex] +
										nullspaceMatrix[6][smallestIndex]);

	R2Point test_point(0.34,-2.8);

	printf("A point off the conic: %f\n",	test_point[0]*test_point[0]*nullspaceMatrix[1][smallestIndex] +
											test_point[0]*test_point[1]*nullspaceMatrix[2][smallestIndex] +
											test_point[1]*test_point[1]*nullspaceMatrix[3][smallestIndex] +
											test_point[0]*nullspaceMatrix[4][smallestIndex] +
											test_point[1]*nullspaceMatrix[5][smallestIndex] +
											nullspaceMatrix[6][smallestIndex]);

	return;
}



////////////////////////////////////////////////////////////////////////
// Image processing functions
// YOU IMPLEMENT THE FUNCTIONS IN THIS SECTION
////////////////////////////////////////////////////////////////////////

// Per-pixel Operations ////////////////////////////////////////////////

void R2Image::
Brighten(double factor)
{
  // Brighten the image by multiplying each pixel component by the factor.
  // This is implemented for you as an example of how to access and set pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0;  j < height; j++) {
      Pixel(i,j) *= factor;
      Pixel(i,j).Clamp();
    }
  }
}

void R2Image::
SobelX(void)
{
	// Apply the Sobel oprator to the image in X direction
  R2Image temp(*this);

  double weights[3][3] = {{1.0, 0.0, -1.0},
                          {2.0, 0.0, -2.0},
                          {1.0, 0.0, -1.0}};

  for (int y = 1; y <= height - 1; y++) {
    for (int x = 1; x <= width - 1; x++) {
      R2Pixel val;
      for (int ly = -1; ly <= 1; ly++) {
        for (int lx = -1; lx <= 1; lx++) {
          //printf("The weight: %d\n", weights[lx + 1][ly + 1]);
          val += Pixel(x + lx, y + ly) * weights[lx + 1][ly + 1];
        }
      }
      temp.Pixel(x, y) = val;
      //temp.Pixel(x, y).Clamp();
    }
  }
  *this = temp;
}

void R2Image::
SobelY(void)
{
	// Apply the Sobel oprator to the image in Y direction
  R2Image temp(*this);

  double weights[][3] = {{-1.0, -2.0, -1.0},
                         {0.0, 0.0, 0.0},
                         {1.0, 2.0, 1.0}};

  for (int y = 1; y <= height - 1; y++) {
    for (int x = 1; x <= width - 1; x++) {
      R2Pixel* val = new R2Pixel();
      for (int ly = -1; ly <= 1; ly++) {
        for (int lx = -1; lx <= 1; lx++) {
          //printf("The weight: %d\n", weights[lx + 1][ly + 1]);
          *val += Pixel(x + lx, y + ly) * weights[lx + 1][ly + 1];
        }
      }
      temp.Pixel(x, y) = *val;
      //temp.Pixel(x, y).Clamp();
    }
  }
  *this = temp;
}

void R2Image::
LoG(void)
{
  // Apply the LoG oprator to the image

  // FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
  fprintf(stderr, "LoG() not implemented\n");
}



void R2Image::
ChangeSaturation(double factor)
{
  // Changes the saturation of an image
  // Find a formula that changes the saturation without affecting the image brightness

  // FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
  fprintf(stderr, "ChangeSaturation(%g) not implemented\n", factor);
}


// Linear filtering ////////////////////////////////////////////////
void R2Image::
Blur(double sigma)
{
  // Gaussian blur of the image. Separable solution is preferred
  int curveWidth = 3 * sigma;
  int size = curveWidth * 2 + 1;
  // FILL IN IMPLEMENTATION HERE (REMOVE PRINT STATEMENT WHEN DONE)
  R2Image temp(*this);

  // Compute the weights based on the sigma value
  double totWeight = 0.0;
  double lineWeights[size];
  for (int i = -curveWidth; i <= curveWidth; i++) {
    int index = i + curveWidth;
    lineWeights[index] = exp(- (i * i) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
    totWeight += lineWeights[index];
  }

  //normalize the kernel
  for (int i = 0; i < size; i++) {
    lineWeights[i] /= totWeight;
    //std :: cout << lineWeights[i] << "\n";
    //std :: cout << totWeight  << "\n";
  }

  //vertical blur: Gaussian first pass
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      R2Pixel* val = new R2Pixel();
      for (int ly = -curveWidth; ly <= curveWidth; ly++) {
        *val += lineWeights[ly + curveWidth] * Pixel(x, std::max(0, std::min(y + ly, height -1)));
      }
      temp.Pixel(x, y) = *val;
      //temp.Pixel(x, y).Clamp();
    }
  }

  //Horizontal blur: Gaussian second pass
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      R2Pixel* val = new R2Pixel();
      for (int lx = -curveWidth; lx <= curveWidth; lx++) {
        *val += lineWeights[lx + curveWidth] * temp.Pixel(std::max(0, std::min(x + lx, width -1)), y);
      }
      Pixel(x, y) = *val;
      //Pixel(x, y).Clamp();
    }
  }
}

void R2Image::
Greyscale()
{
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      R2Pixel pix = Pixel(x, y);
      double average = (pix.Red() + pix.Blue() + pix.Green()) / 3.0;
      Pixel(x, y) = R2Pixel(average, average, average, 1);
    }
  }
}

struct Feature
{
  int centerX;
  int centerY;
  R2Pixel HarrisValue;

  Feature( )
  {
      centerX = -1;
      centerY = -1;
  }


  Feature(int x, int y, R2Pixel val)
  {
      centerX = x;
      centerY = y;
      HarrisValue = val;
  }

  bool operator<(const Feature& feature) const {
      double valueIntensity = HarrisValue[0] + HarrisValue[1] + HarrisValue[2];
      double featureIntensity = feature.HarrisValue[0] + feature.HarrisValue[1] + feature.HarrisValue[2];

      return valueIntensity < featureIntensity;
  }

};

std::vector<std::pair<int, int> > R2Image::
Harris(double sigma)
{
  // Harris corner detector. Make use of the previously developed filters, such as the Gaussian blur filter
	// Output should be 50% grey at flat regions, white at corners and black/dark near edges
  //this->Greyscale();
  R2Image* sobelX2 = new R2Image(*this);
  sobelX2->SobelX();
  R2Image* sobelY2 = new R2Image(*this);
  sobelY2->SobelY();
  R2Image* sobelXY = new R2Image(width, height);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      sobelXY->Pixel(x, y) = sobelX2->Pixel(x, y) * sobelY2->Pixel(x, y);
      sobelX2->Pixel(x, y) *= sobelX2->Pixel(x, y);
      sobelY2->Pixel(x, y) *= sobelY2->Pixel(x, y);
    }
  }

  printf("Blurrrrrring...\n");
  sobelX2->Blur(sigma);
  printf("first blur done\n");
  sobelY2->Blur(sigma);
  printf("second blur done\n");
  sobelXY->Blur(sigma);

  printf("Finished blrrring...\n");
  R2Pixel offset(.5, .5, .5, 0);
  std::vector<Feature> harris2DArray;

  printf("Trying to compute Harris score now...\n");
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
        R2Pixel harrisPix(sobelX2->Pixel(x, y) * sobelY2->Pixel(x, y) - sobelXY->Pixel(x, y) * sobelXY->Pixel(x, y)
                      - .04 * ((sobelX2->Pixel(x, y) + sobelY2->Pixel(x, y)) * (sobelX2->Pixel(x, y) + sobelY2->Pixel(x, y))));
        harrisPix += R2Pixel(.5, .5, .5, .5);
        harris2DArray.push_back(Feature(x, y, harrisPix));
    }
  }
  std::cout << "There are in total "<< harris2DArray.size() << " feature points\n";
  printf("Harris scores are computed!\n");

  free(sobelXY);
  free(sobelX2);
  free(sobelY2);
  std::vector<std::pair<int, int> > featureData;

  //sort the harris2DArray
  std::sort(harris2DArray.begin(), harris2DArray.end());
  int index = (int)(harris2DArray.size() - 1);
  int count = 0;

  while (count < 150) {
    int flag = false;
    int x = harris2DArray[index].centerX;
    int y = harris2DArray[index].centerY;
    for (int i=0;i<featureData.size();i++) {
      if (abs(x - featureData[i].first) < 10 && abs(y - featureData[i].second) < 10) {
        flag = true;
      }
    }
    if (flag == false) {
      count++;
      std::cout << "feature point #" << count << " is located: " << x << ", " << y << "\n";
      std::cout << "And currently the index is at " << index << "\n";
      std::pair<int, int> newPair = std::make_pair(x, y);
      featureData.push_back(newPair);
    }
    index--;
  }

  return featureData;
}


void R2Image::
Sharpen()
{
  // Sharpen an image using a linear filter. Use a kernel of your choosing.
  R2Image temp(*this);

  double weights[3][3] = {{0.0, -1.0, 0.0},
                          {-1.0, 5.0, -1.0},
                          {0.0, -1.0, 0.0}};

  for (int y = 1; y <= height - 1; y++) {
    for (int x = 1; x <= width - 1; x++) {
      R2Pixel* val = new R2Pixel();
      for (int ly = -1; ly <= 1; ly++) {
        for (int lx = -1; lx <= 1; lx++) {
          //printf("The weight: %d\n", weights[lx + 1][ly + 1]);
          *val += Pixel(x + lx, y + ly) * weights[lx + 1][ly + 1];
        }
      }
      temp.Pixel(x, y) = *val;
      temp.Pixel(x, y).Clamp();
    }
  }
  *this = temp;
}

void R2Image::
drawLine(int x1, int y1, int x2, int y2, std::string color)
{
  double deltaX = (double) (x2 - x1);
  double deltaY = (double) (y2 - y1);
  double deltaErr = deltaY/deltaX;

  if (color.compare("false")) {
    for (int ly = -4; ly <= 4; ly++) {
        for (int lx = -4; lx <= 4; lx++) {
          int xCoord2 = std::max(0, std::min(x2 + lx, width -1));
          int yCoord2 = std::max(0, std::min(y2 + ly, height -1));
          if (abs(ly) <= 3 ||abs(lx) <= 3) {
            Pixel(xCoord2, yCoord2) = R2blue_pixel;
          }
        }
      }
    for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
      for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
        double expectedY = (double) (deltaErr * (x - x1) + y1);
        if (expectedY <= y + .5 && expectedY >= y - .5) {
            Pixel(x, y) = R2green_pixel;
        }
      }
    }
  } else if (color.compare("true")) {
    for (int ly = -4; ly <= 4; ly++) {
        for (int lx = -4; lx <= 4; lx++) {
          int xCoord2 = std::max(0, std::min(x2 + lx, width -1));
          int yCoord2 = std::max(0, std::min(y2 + ly, height -1));
          if (abs(ly) <= 2 ||abs(lx) <= 2) {
            Pixel(xCoord2, yCoord2) = R2Pixel(1, 1, 0, 1);
          }
        }
      }
    for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
      for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++) {
        double expectedY = (double) (deltaErr * (x - x1) + y1);
        if (expectedY <= y + .5 && expectedY >= y - .5) {
            Pixel(x, y) = R2red_pixel;
        }
      }
    }
  }


}

void R2Image::
blendOtherImageTranslated(R2Image * otherImage)
{
	// find at least 100 features on this image, and another 100 on the "otherImage". Based on these,
	// compute the matching translation (pixel precision is OK), and blend the translated "otherImage"
	// into this image with a 50% opacity.
	fprintf(stderr, "fit other image using translation and blend imageB over imageA\n");
  R2Image temp(*this);
  std::vector<std::pair<int, int> > features = temp.Harris(2.0);

  std::cout << "OK, Harris filter is done. Move on to create matchList...\n";

  std::vector<std::pair<int, int> > matchList;
  double windowSize = 0.2;

  std::cout << "Currently there are " << matchList.size() << " matches found!\n";

  std::cout << "featureList contains " << features.size() << " elements...\n";

  //loop through all the feature points
  for (int n=0;n<features.size();n++) {
    std::pair<int, int> pair = features[n];
    int x = pair.first;
    int y = pair.second;
    std::cout << "I'm currently at feature point (" << x << ", " << y << ")\n";
    //for each feature, scan around a 20% by 20% window
    R2Pixel minSSD(100000000, 100000000, 1000000000, 1000000000);
    std::pair<int, int> matchedFeature;
    int max_X = std::min((int) (x + width*windowSize/2.0), width);
    int max_Y = std::min((int) (y + height*windowSize/2.0), height);
    std::cout << "x goes from " << x << " to: " << max_X << "and y goes from "<< y <<" to: " << max_Y << '\n';
    for (int j = (std::max((int) (y - height*windowSize/2), 0)); j < max_Y; j++) {
      for (int i = (std::max((int) (x - width*windowSize/2), 0)); i < max_X; i++) {
        //for each point in the window, define a small 12 px by 12 px panel for SSD calculation.
          R2Pixel* ssdDiff = new R2Pixel(0, 0, 0, 0);

          for (int row=-6;row<6;row++) {
            for (int col=-6;col<6;col++) {
              int otherX = std::max(0, std::min(width, i+col));
              int otherY = std::max(0, std::min(height, j+row));
              int thisX = std::max(0, std::min(width, x+col));
              int thisY = std::max(0, std::min(height, y+row));
              R2Pixel diff(Pixel(thisX, thisY) - otherImage->Pixel(otherX, otherY));
              diff = diff * diff;
              *ssdDiff += diff;
            }
          }

          //std::cout << "ssdDiff is: (" << ssdDiff->Luminance() << ")\n";
          //set minSSD if it turns out that ssdDiff is smaller.
          if (ssdDiff->Luminance() < minSSD.Luminance()) {
            minSSD = *ssdDiff;
            matchedFeature = std::make_pair(i, j);
          }
      }
    }
    matchList.push_back(matchedFeature);
    //this->drawLine(x, y, matchedFeature.first, matchedFeature.second);
    //this->drawLine(x, y, x,y);
    std::cout << "Feature coordinates: " << matchedFeature.first << ", " << matchedFeature.second << "\n";
  }

  std::cout << matchList.size();
  //a vector of indices for randomly choosing points later
  std::vector<int> v;
  for (int i=0;i<matchList.size();i++) {
    v.push_back(i);
  }

  //try 60 random vectors and see if they match many inliers.
  std::vector<int> maxInlier;
  for (int i=0;i<60;i++) {
    int random = rand() % v.size();
    int randIndex = v[random];
    std::cout << randIndex << " and the are " << v.size() << "left to test." << "\n";
    v.erase(v.begin() + random);
    int xDeltaSelected = matchList[randIndex].first - features[randIndex].first;
    int yDeltaSelected = matchList[randIndex].second - features[randIndex].second;
    std::vector<int> inlier;
    //loop through all feature points and find the error distance.
    for (int j=0;j<features.size();j++) {
      int xDelta = matchList[j].first - features[j].first;
      int yDelta = matchList[j].second - features[j].second;
      int error = (xDelta - xDeltaSelected)*(xDelta - xDeltaSelected) + (yDelta - yDeltaSelected)*(yDelta - yDeltaSelected);
      if (error <= 9) {
        //if error is lower than threshold, then count as inlier.
        inlier.push_back(j);
      }
    }
    //update the max inlier size
    if (inlier.size() > maxInlier.size()) {
      maxInlier = inlier;
    }
  }

  for (int n=0;n<matchList.size();n++) {
    std::pair<int, int> match = matchList.at(n);
    std::pair<int, int> initial = features.at(n);
    int x1 = initial.first;
    int y1 = initial.second;
    int x2 = match.first;
    int y2 = match.second;
    if (std::find(maxInlier.begin(), maxInlier.end(), n) != maxInlier.end()) {
      this->drawLine(x1, y1, x2, y2, "true");
    } else {
      this->drawLine(x1, y1, x2, y2, "false");
    }
  }

	return;
}

void R2Image::
blendOtherImageHomography(R2Image * otherImage)
{
	// find at least 100 features on this image, and another 100 on the "otherImage". Based on these,
	// compute the matching homography, and blend the transformed "otherImage" into this image with a 50% opacity.
	fprintf(stderr, "fit other image using a homography and blend imageB over imageA\n");
  R2Image temp(*this);
  std::vector<std::pair<int, int> > features = temp.Harris(2.0);

  std::cout << "OK, Harris filter is done. Move on to create matchList...\n";

  std::vector<std::pair<int, int> > matchList;
  double windowSize = 0.2;

  std::cout << "Currently there are " << matchList.size() << " matches found!\n";

  std::cout << "featureList contains " << features.size() << " elements...\n";

  //loop through all the feature points
  for (int n=0;n<features.size();n++) {
    std::pair<int, int> pair = features[n];
    int x = pair.first;
    int y = pair.second;
    std::cout << "I'm currently at feature point (" << x << ", " << y << ")\n";
    //for each feature, scan around a 20% by 20% window
    R2Pixel minSSD(0, 0, 0, 0);
    std::pair<int, int> matchedFeature;
    int max_X = std::min((int) (x + width*windowSize/2.0), width);
    int max_Y = std::min((int) (y + height*windowSize/2.0), height);
    int min_X = std::max((int) (x - width*windowSize/2.0), 0);
    int min_Y = std::max((int) (y - height*windowSize/2.0), 0);
    std::cout << "x goes from " << min_X << " to: " << max_X << "and y goes from "<< min_Y <<" to: " << max_Y << '\n';
    for (int j = min_Y; j < max_Y; j++) {
      for (int i = min_X; i < max_X; i++) {
        //for each point in the window, define a small 12 px by 12 px panel for SSD calculation.
          R2Pixel* ssdDiff = new R2Pixel(0, 0, 0, 0);

          for (int row=-6;row<6;row++) {
            for (int col=-6;col<6;col++) {
              int otherX = std::max(0, std::min(width, i+col));
              int otherY = std::max(0, std::min(height, j+row));
              int thisX = std::max(0, std::min(width, x+col));
              int thisY = std::max(0, std::min(height, y+row));
              R2Pixel diff(Pixel(thisX, thisY) - otherImage->Pixel(otherX, otherY));
              diff = diff * diff;
              *ssdDiff += diff;
            }
          }
          if (i == min_X && j == min_Y) {
            minSSD = *ssdDiff;
            matchedFeature = std::make_pair(i, j);
          } else if (ssdDiff->Luminance() < minSSD.Luminance()) {
            //set minSSD if it turns out that ssdDiff is smaller.
            minSSD = *ssdDiff;
            matchedFeature = std::make_pair(i, j);
          }
      }
    }
    matchList.push_back(matchedFeature);
    std::cout << "Feature coordinates: " << matchedFeature.first << ", " << matchedFeature.second << "\n";
  }

  std::cout << matchList.size() << "\n";

  std::vector<int> maxInlier;

  for (int i=0;i<150;i++) {
    //a vector of indices for randomly choosing points later
    std::vector<int> v;
    for (int i=0;i<matchList.size();i++) {
      v.push_back(i);
    }

    std::vector<int> inlier;
    double origin[8];
    double match[8];

    for (int k=0;k<4;k++) {
      int random = rand() % v.size();
      int randIndex = v[random];
      std::cout << randIndex << " and the are " << v.size() << "left to test." << "\n";
      v.erase(v.begin() + random);
      origin[k*2] = features[randIndex].first;
      origin[k*2+1] = features[randIndex].second;
      match[k*2] = matchList[randIndex].first;
      match[k*2+1] = matchList[randIndex].second;
    }

    //COMPUTE HOMOGRAPHY
    double** homographyMatrix = R2Image::homographyEstimate(origin, match);
    //COMPUTE THE MATCH FEATURE POINT
    //loop through all feature points and find the error distance.
    for (int j=0;j<features.size();j++) {
      double scale = homographyMatrix[3][1] * features[j].first
                + homographyMatrix[3][2] * features[j].second
                + homographyMatrix[3][3];
      int matchX = (int) ((homographyMatrix[1][1] * features[j].first
                        + homographyMatrix[1][2] * features[j].second
                        + homographyMatrix[1][3]) / scale);
      int matchY = (int) ((homographyMatrix[2][1] * features[j].first
                        + homographyMatrix[2][2] * features[j].second
                        + homographyMatrix[2][3]) / scale);
      int xDelta = matchX - matchList[j].first;
      int yDelta = matchY - matchList[j].second;
      int error = xDelta * xDelta + yDelta * yDelta;
      if (error <= 9) {
        //if error is lower than threshold, then count as inlier.
        inlier.push_back(j);
      }
    }

    //update the max inlier size
    if (inlier.size() > maxInlier.size()) {
      maxInlier = inlier;
    }
  }

  for (int n=0;n<matchList.size();n++) {
    std::pair<int, int> match = matchList.at(n);
    std::pair<int, int> initial = features.at(n);
    int x1 = initial.first;
    int y1 = initial.second;
    int x2 = match.first;
    int y2 = match.second;
    if (std::find(maxInlier.begin(), maxInlier.end(), n) != maxInlier.end()) {
      this->drawLine(x1, y1, x2, y2, "true");
    } else {
      this->drawLine(x1, y1, x2, y2, "false");
    }
  }
	return;
}

double** R2Image::
homographyEstimate(double origin[], double match[]) {
  double **a = dmatrix(1,8,1,9);
  for (int i=0;i<4;i++) {
    int first = 2*i;
    int second = 2*i + 1;
    a[first+1][1] = 0;
    a[first+1][2] = 0;
    a[first+1][3] = 0;
    a[first+1][4] = -origin[first];
    a[first+1][5] = -origin[second];
    a[first+1][6] = -1;
    a[first+1][7] = match[second]*origin[first];
    a[first+1][8] = match[second]*origin[second];
    a[first+1][9] = match[second];
    a[second+1][1] = origin[first];
    a[second+1][2] = origin[second];
    a[second+1][3] = 1;
    a[second+1][4] = 0;
    a[second+1][5] = 0;
    a[second+1][6] = 0;
    a[second+1][7] = -origin[first]*match[first];
    a[second+1][8] = -match[first]*origin[second];
    a[second+1][9] = -match[first];
  }
  // compute the SVD
	double singularValues[10]; // 1..6
	double** nullspaceMatrix = dmatrix(1,9,1,9);
	svdcmp(a, 8, 9, singularValues, nullspaceMatrix);

	// get the result
	printf("\n Singular values: %f, %f, %f, %f, %f, %f, %f, %f, %f\n",singularValues[1],singularValues[2],singularValues[3],singularValues[4],singularValues[5],singularValues[6],singularValues[7],singularValues[8],singularValues[9]);

	// find the smallest singular value:
	int smallestIndex = 1;
	for(int i=2;i<=9;i++) if(singularValues[i]<singularValues[smallestIndex]) smallestIndex=i;

  std::cout << "Hey, the smallestIndex is " << smallestIndex << '\n';

  double** homographyMat = dmatrix(1, 3, 1, 3);
  for (int i=1;i<=3;i++) {
    double x = nullspaceMatrix[3*i-2][smallestIndex];
    double y = nullspaceMatrix[3*i-1][smallestIndex];
    double z = nullspaceMatrix[3*i][smallestIndex];
    if (x < 1e-5 && x >-1e-5) x=0;
    if (y < 1e-5 && y >-1e-5) y=0;
    if (z < 1e-5 && z >-1e-5) z=0;
    homographyMat[i][1] = x;
    homographyMat[i][2] = y;
    homographyMat[i][3] = z;
    std::cout << x << ",    " << y  << ",    " << z << "\n";
  }

  return homographyMat;

}

////////////////////////////////////////////////////////////////////////
// I/O Functions
////////////////////////////////////////////////////////////////////////

int R2Image::
Read(const char *filename)
{
  // Initialize everything
  if (pixels) { delete [] pixels; pixels = NULL; }
  npixels = width = height = 0;

  // Parse input filename extension
  char *input_extension;
  if (!(input_extension = (char*)strrchr(filename, '.'))) {
    fprintf(stderr, "Input file has no extension (e.g., .jpg).\n");
    return 0;
  }

  // Read file of appropriate type
  if (!strncmp(input_extension, ".bmp", 4)) return ReadBMP(filename);
  else if (!strncmp(input_extension, ".ppm", 4)) return ReadPPM(filename);
  else if (!strncmp(input_extension, ".jpg", 4)) return ReadJPEG(filename);
  else if (!strncmp(input_extension, ".jpeg", 5)) return ReadJPEG(filename);

  // Should never get here
  fprintf(stderr, "Unrecognized image file extension");
  return 0;
}



int R2Image::
Write(const char *filename) const
{
  // Parse input filename extension
  char *input_extension;
  if (!(input_extension = (char*)strrchr(filename, '.'))) {
    fprintf(stderr, "Input file has no extension (e.g., .jpg).\n");
    return 0;
  }

  // Write file of appropriate type
  if (!strncmp(input_extension, ".bmp", 4)) return WriteBMP(filename);
  else if (!strncmp(input_extension, ".ppm", 4)) return WritePPM(filename, 1);
  else if (!strncmp(input_extension, ".jpg", 5)) return WriteJPEG(filename);
  else if (!strncmp(input_extension, ".jpeg", 5)) return WriteJPEG(filename);

  // Should never get here
  fprintf(stderr, "Unrecognized image file extension");
  return 0;
}



////////////////////////////////////////////////////////////////////////
// BMP I/O
////////////////////////////////////////////////////////////////////////

#if (RN_OS == RN_LINUX) && !WIN32

typedef struct tagBITMAPFILEHEADER {
  unsigned short int bfType;
  unsigned int bfSize;
  unsigned short int bfReserved1;
  unsigned short int bfReserved2;
  unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  unsigned int biSize;
  int biWidth;
  int biHeight;
  unsigned short int biPlanes;
  unsigned short int biBitCount;
  unsigned int biCompression;
  unsigned int biSizeImage;
  int biXPelsPerMeter;
  int biYPelsPerMeter;
  unsigned int biClrUsed;
  unsigned int biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBTRIPLE {
  unsigned char rgbtBlue;
  unsigned char rgbtGreen;
  unsigned char rgbtRed;
} RGBTRIPLE;

typedef struct tagRGBQUAD {
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
  unsigned char rgbReserved;
} RGBQUAD;

#endif

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L

#define BMP_BF_TYPE 0x4D42 /* word BM */
#define BMP_BF_OFF_BITS 54 /* 14 for file header + 40 for info header (not sizeof(), but packed size) */
#define BMP_BI_SIZE 40 /* packed size of info header */


static unsigned short int WordReadLE(FILE *fp)
{
  // Read a unsigned short int from a file in little endian format
  unsigned short int lsb, msb;
  lsb = getc(fp);
  msb = getc(fp);
  return (msb << 8) | lsb;
}



static void WordWriteLE(unsigned short int x, FILE *fp)
{
  // Write a unsigned short int to a file in little endian format
  unsigned char lsb = (unsigned char) (x & 0x00FF); putc(lsb, fp);
  unsigned char msb = (unsigned char) (x >> 8); putc(msb, fp);
}



static unsigned int DWordReadLE(FILE *fp)
{
  // Read a unsigned int word from a file in little endian format
  unsigned int b1 = getc(fp);
  unsigned int b2 = getc(fp);
  unsigned int b3 = getc(fp);
  unsigned int b4 = getc(fp);
  return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



static void DWordWriteLE(unsigned int x, FILE *fp)
{
  // Write a unsigned int to a file in little endian format
  unsigned char b1 = (x & 0x000000FF); putc(b1, fp);
  unsigned char b2 = ((x >> 8) & 0x000000FF); putc(b2, fp);
  unsigned char b3 = ((x >> 16) & 0x000000FF); putc(b3, fp);
  unsigned char b4 = ((x >> 24) & 0x000000FF); putc(b4, fp);
}



static int LongReadLE(FILE *fp)
{
  // Read a int word from a file in little endian format
  int b1 = getc(fp);
  int b2 = getc(fp);
  int b3 = getc(fp);
  int b4 = getc(fp);
  return (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
}



static void LongWriteLE(int x, FILE *fp)
{
  // Write a int to a file in little endian format
  char b1 = (x & 0x000000FF); putc(b1, fp);
  char b2 = ((x >> 8) & 0x000000FF); putc(b2, fp);
  char b3 = ((x >> 16) & 0x000000FF); putc(b3, fp);
  char b4 = ((x >> 24) & 0x000000FF); putc(b4, fp);
}



int R2Image::
ReadBMP(const char *filename)
{
  // Open file
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  /* Read file header */
  BITMAPFILEHEADER bmfh;
  bmfh.bfType = WordReadLE(fp);
  bmfh.bfSize = DWordReadLE(fp);
  bmfh.bfReserved1 = WordReadLE(fp);
  bmfh.bfReserved2 = WordReadLE(fp);
  bmfh.bfOffBits = DWordReadLE(fp);

  /* Check file header */
  assert(bmfh.bfType == BMP_BF_TYPE);
  /* ignore bmfh.bfSize */
  /* ignore bmfh.bfReserved1 */
  /* ignore bmfh.bfReserved2 */
  assert(bmfh.bfOffBits == BMP_BF_OFF_BITS);

  /* Read info header */
  BITMAPINFOHEADER bmih;
  bmih.biSize = DWordReadLE(fp);
  bmih.biWidth = LongReadLE(fp);
  bmih.biHeight = LongReadLE(fp);
  bmih.biPlanes = WordReadLE(fp);
  bmih.biBitCount = WordReadLE(fp);
  bmih.biCompression = DWordReadLE(fp);
  bmih.biSizeImage = DWordReadLE(fp);
  bmih.biXPelsPerMeter = LongReadLE(fp);
  bmih.biYPelsPerMeter = LongReadLE(fp);
  bmih.biClrUsed = DWordReadLE(fp);
  bmih.biClrImportant = DWordReadLE(fp);

  // Check info header
  assert(bmih.biSize == BMP_BI_SIZE);
  assert(bmih.biWidth > 0);
  assert(bmih.biHeight > 0);
  assert(bmih.biPlanes == 1);
  assert(bmih.biBitCount == 24);  /* RGB */
  assert(bmih.biCompression == BI_RGB);   /* RGB */
  int lineLength = bmih.biWidth * 3;  /* RGB */
  if ((lineLength % 4) != 0) lineLength = (lineLength / 4 + 1) * 4;
  assert(bmih.biSizeImage == (unsigned int) lineLength * (unsigned int) bmih.biHeight);

  // Assign width, height, and number of pixels
  width = bmih.biWidth;
  height = bmih.biHeight;
  npixels = width * height;

  // Allocate unsigned char buffer for reading pixels
  int rowsize = 3 * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
  int nbytes = bmih.biSizeImage;
  unsigned char *buffer = new unsigned char [nbytes];
  if (!buffer) {
    fprintf(stderr, "Unable to allocate temporary memory for BMP file");
    fclose(fp);
    return 0;
  }

  // Read buffer
  fseek(fp, (long) bmfh.bfOffBits, SEEK_SET);
  if (fread(buffer, 1, bmih.biSizeImage, fp) != bmih.biSizeImage) {
    fprintf(stderr, "Error while reading BMP file %s", filename);
    return 0;
  }

  // Close file
  fclose(fp);

  // Allocate pixels for image
  pixels = new R2Pixel [ width * height ];
  if (!pixels) {
    fprintf(stderr, "Unable to allocate memory for BMP file");
    fclose(fp);
    return 0;
  }

  // Assign pixels
  for (int j = 0; j < height; j++) {
    unsigned char *p = &buffer[j * rowsize];
    for (int i = 0; i < width; i++) {
      double b = (double) *(p++) / 255;
      double g = (double) *(p++) / 255;
      double r = (double) *(p++) / 255;
      R2Pixel pixel(r, g, b, 1);
      SetPixel(i, j, pixel);
    }
  }

  // Free unsigned char buffer for reading pixels
  delete [] buffer;

  // Return success
  return 1;
}



int R2Image::
WriteBMP(const char *filename) const
{
  // Open file
  FILE *fp = fopen(filename, "wb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Compute number of bytes in row
  int rowsize = 3 * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;

  // Write file header
  BITMAPFILEHEADER bmfh;
  bmfh.bfType = BMP_BF_TYPE;
  bmfh.bfSize = BMP_BF_OFF_BITS + rowsize * height;
  bmfh.bfReserved1 = 0;
  bmfh.bfReserved2 = 0;
  bmfh.bfOffBits = BMP_BF_OFF_BITS;
  WordWriteLE(bmfh.bfType, fp);
  DWordWriteLE(bmfh.bfSize, fp);
  WordWriteLE(bmfh.bfReserved1, fp);
  WordWriteLE(bmfh.bfReserved2, fp);
  DWordWriteLE(bmfh.bfOffBits, fp);

  // Write info header
  BITMAPINFOHEADER bmih;
  bmih.biSize = BMP_BI_SIZE;
  bmih.biWidth = width;
  bmih.biHeight = height;
  bmih.biPlanes = 1;
  bmih.biBitCount = 24;       /* RGB */
  bmih.biCompression = BI_RGB;    /* RGB */
  bmih.biSizeImage = rowsize * (unsigned int) bmih.biHeight;  /* RGB */
  bmih.biXPelsPerMeter = 2925;
  bmih.biYPelsPerMeter = 2925;
  bmih.biClrUsed = 0;
  bmih.biClrImportant = 0;
  DWordWriteLE(bmih.biSize, fp);
  LongWriteLE(bmih.biWidth, fp);
  LongWriteLE(bmih.biHeight, fp);
  WordWriteLE(bmih.biPlanes, fp);
  WordWriteLE(bmih.biBitCount, fp);
  DWordWriteLE(bmih.biCompression, fp);
  DWordWriteLE(bmih.biSizeImage, fp);
  LongWriteLE(bmih.biXPelsPerMeter, fp);
  LongWriteLE(bmih.biYPelsPerMeter, fp);
  DWordWriteLE(bmih.biClrUsed, fp);
  DWordWriteLE(bmih.biClrImportant, fp);

  // Write image, swapping blue and red in each pixel
  int pad = rowsize - width * 3;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      const R2Pixel& pixel = (*this)[i][j];
      double r = 255.0 * pixel.Red();
      double g = 255.0 * pixel.Green();
      double b = 255.0 * pixel.Blue();
      if (r >= 255) r = 255;
      if (g >= 255) g = 255;
      if (b >= 255) b = 255;
      fputc((unsigned char) b, fp);
      fputc((unsigned char) g, fp);
      fputc((unsigned char) r, fp);
    }

    // Pad row
    for (int i = 0; i < pad; i++) fputc(0, fp);
  }

  // Close file
  fclose(fp);

  // Return success
  return 1;
}



////////////////////////////////////////////////////////////////////////
// PPM I/O
////////////////////////////////////////////////////////////////////////

int R2Image::
ReadPPM(const char *filename)
{
  // Open file
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Read PPM file magic identifier
  char buffer[128];
  if (!fgets(buffer, 128, fp)) {
    fprintf(stderr, "Unable to read magic id in PPM file");
    fclose(fp);
    return 0;
  }

  // skip comments
  int c = getc(fp);
  while (c == '#') {
    while (c != '\n') c = getc(fp);
    c = getc(fp);
  }
  ungetc(c, fp);

  // Read width and height
  if (fscanf(fp, "%d%d", &width, &height) != 2) {
    fprintf(stderr, "Unable to read width and height in PPM file");
    fclose(fp);
    return 0;
  }

  // Read max value
  double max_value;
  if (fscanf(fp, "%lf", &max_value) != 1) {
    fprintf(stderr, "Unable to read max_value in PPM file");
    fclose(fp);
    return 0;
  }

  // Allocate image pixels
  pixels = new R2Pixel [ width * height ];
  if (!pixels) {
    fprintf(stderr, "Unable to allocate memory for PPM file");
    fclose(fp);
    return 0;
  }

  // Check if raw or ascii file
  if (!strcmp(buffer, "P6\n")) {
    // Read up to one character of whitespace (\n) after max_value
    int c = getc(fp);
    if (!isspace(c)) putc(c, fp);

    // Read raw image data
    // First ppm pixel is top-left, so read in opposite scan-line order
    for (int j = height-1; j >= 0; j--) {
      for (int i = 0; i < width; i++) {
        double r = (double) getc(fp) / max_value;
        double g = (double) getc(fp) / max_value;
        double b = (double) getc(fp) / max_value;
        R2Pixel pixel(r, g, b, 1);
        SetPixel(i, j, pixel);
      }
    }
  }
  else {
    // Read asci image data
    // First ppm pixel is top-left, so read in opposite scan-line order
    for (int j = height-1; j >= 0; j--) {
      for (int i = 0; i < width; i++) {
	// Read pixel values
	int red, green, blue;
	if (fscanf(fp, "%d%d%d", &red, &green, &blue) != 3) {
	  fprintf(stderr, "Unable to read data at (%d,%d) in PPM file", i, j);
	  fclose(fp);
	  return 0;
	}

	// Assign pixel values
	double r = (double) red / max_value;
	double g = (double) green / max_value;
	double b = (double) blue / max_value;
        R2Pixel pixel(r, g, b, 1);
        SetPixel(i, j, pixel);
      }
    }
  }

  // Close file
  fclose(fp);

  // Return success
  return 1;
}



int R2Image::
WritePPM(const char *filename, int ascii) const
{
  // Check type
  if (ascii) {
    // Open file
    FILE *fp = fopen(filename, "w");
    if (!fp) {
      fprintf(stderr, "Unable to open image file: %s", filename);
      return 0;
    }

    // Print PPM image file
    // First ppm pixel is top-left, so write in opposite scan-line order
    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");
    for (int j = height-1; j >= 0 ; j--) {
      for (int i = 0; i < width; i++) {
        const R2Pixel& p = (*this)[i][j];
        int r = (int) (255 * p.Red());
        int g = (int) (255 * p.Green());
        int b = (int) (255 * p.Blue());
        fprintf(fp, "%-3d %-3d %-3d  ", r, g, b);
        if (((i+1) % 4) == 0) fprintf(fp, "\n");
      }
      if ((width % 4) != 0) fprintf(fp, "\n");
    }
    fprintf(fp, "\n");

    // Close file
    fclose(fp);
  }
  else {
    // Open file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
      fprintf(stderr, "Unable to open image file: %s", filename);
      return 0;
    }

    // Print PPM image file
    // First ppm pixel is top-left, so write in opposite scan-line order
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");
    for (int j = height-1; j >= 0 ; j--) {
      for (int i = 0; i < width; i++) {
        const R2Pixel& p = (*this)[i][j];
        int r = (int) (255 * p.Red());
        int g = (int) (255 * p.Green());
        int b = (int) (255 * p.Blue());
        fprintf(fp, "%c%c%c", r, g, b);
      }
    }

    // Close file
    fclose(fp);
  }

  // Return success
  return 1;
}



////////////////////////////////////////////////////////////////////////
// JPEG I/O
////////////////////////////////////////////////////////////////////////


// #define USE_JPEG
#ifdef USE_JPEG
  extern "C" {
#   define XMD_H // Otherwise, a conflict with INT32
#   undef FAR // Otherwise, a conflict with windows.h
#   include "jpeg/jpeglib.h"
  };
#endif



int R2Image::
ReadJPEG(const char *filename)
{
#ifdef USE_JPEG
  // Open file
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Initialize decompression info
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, fp);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  // Remember image attributes
  width = cinfo.output_width;
  height = cinfo.output_height;
  npixels = width * height;
  int ncomponents = cinfo.output_components;

  // Allocate pixels for image
  pixels = new R2Pixel [ npixels ];
  if (!pixels) {
    fprintf(stderr, "Unable to allocate memory for BMP file");
    fclose(fp);
    return 0;
  }

  // Allocate unsigned char buffer for reading image
  int rowsize = ncomponents * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
  int nbytes = rowsize * height;
  unsigned char *buffer = new unsigned char [nbytes];
  if (!buffer) {
    fprintf(stderr, "Unable to allocate temporary memory for JPEG file");
    fclose(fp);
    return 0;
  }

  // Read scan lines
  // First jpeg pixel is top-left, so read pixels in opposite scan-line order
  while (cinfo.output_scanline < cinfo.output_height) {
    int scanline = cinfo.output_height - cinfo.output_scanline - 1;
    unsigned char *row_pointer = &buffer[scanline * rowsize];
    jpeg_read_scanlines(&cinfo, &row_pointer, 1);
  }

  // Free everything
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  // Close file
  fclose(fp);

  // Assign pixels
  for (int j = 0; j < height; j++) {
    unsigned char *p = &buffer[j * rowsize];
    for (int i = 0; i < width; i++) {
      double r, g, b, a;
      if (ncomponents == 1) {
        r = g = b = (double) *(p++) / 255;
        a = 1;
      }
      else if (ncomponents == 1) {
        r = g = b = (double) *(p++) / 255;
        a = 1;
        p++;
      }
      else if (ncomponents == 3) {
        r = (double) *(p++) / 255;
        g = (double) *(p++) / 255;
        b = (double) *(p++) / 255;
        a = 1;
      }
      else if (ncomponents == 4) {
        r = (double) *(p++) / 255;
        g = (double) *(p++) / 255;
        b = (double) *(p++) / 255;
        a = (double) *(p++) / 255;
      }
      else {
        fprintf(stderr, "Unrecognized number of components in jpeg image: %d\n", ncomponents);
        return 0;
      }
      R2Pixel pixel(r, g, b, a);
      SetPixel(i, j, pixel);
    }
  }

  // Free unsigned char buffer for reading pixels
  delete [] buffer;

  // Return success
  return 1;
#else
  fprintf(stderr, "JPEG not supported");
  return 0;
#endif
}




int R2Image::
WriteJPEG(const char *filename) const
{
#ifdef USE_JPEG
  // Open file
  FILE *fp = fopen(filename, "wb");
  if (!fp) {
    fprintf(stderr, "Unable to open image file: %s", filename);
    return 0;
  }

  // Initialize compression info
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, fp);
  cinfo.image_width = width; 	/* image width and height, in pixels */
  cinfo.image_height = height;
  cinfo.input_components = 3;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
  cinfo.dct_method = JDCT_ISLOW;
  jpeg_set_defaults(&cinfo);
  cinfo.optimize_coding = TRUE;
  jpeg_set_quality(&cinfo, 95, TRUE);
  jpeg_start_compress(&cinfo, TRUE);

  // Allocate unsigned char buffer for reading image
  int rowsize = 3 * width;
  if ((rowsize % 4) != 0) rowsize = (rowsize / 4 + 1) * 4;
  int nbytes = rowsize * height;
  unsigned char *buffer = new unsigned char [nbytes];
  if (!buffer) {
    fprintf(stderr, "Unable to allocate temporary memory for JPEG file");
    fclose(fp);
    return 0;
  }

  // Fill buffer with pixels
  for (int j = 0; j < height; j++) {
    unsigned char *p = &buffer[j * rowsize];
    for (int i = 0; i < width; i++) {
      const R2Pixel& pixel = (*this)[i][j];
      int r = (int) (255 * pixel.Red());
      int g = (int) (255 * pixel.Green());
      int b = (int) (255 * pixel.Blue());
      if (r > 255) r = 255;
      if (g > 255) g = 255;
      if (b > 255) b = 255;
      *(p++) = r;
      *(p++) = g;
      *(p++) = b;
    }
  }



  // Output scan lines
  // First jpeg pixel is top-left, so write in opposite scan-line order
  while (cinfo.next_scanline < cinfo.image_height) {
    int scanline = cinfo.image_height - cinfo.next_scanline - 1;
    unsigned char *row_pointer = &buffer[scanline * rowsize];
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }

  // Free everything
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);

  // Close file
  fclose(fp);

  // Free unsigned char buffer for reading pixels
  delete [] buffer;

  // Return number of bytes written
  return 1;
#else
  fprintf(stderr, "JPEG not supported");
  return 0;
#endif
}
