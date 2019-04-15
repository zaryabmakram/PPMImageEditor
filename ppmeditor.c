/****************************************************************
**
~~~~~~~~~~~~~~~~~~~~~~ PPM IMAGE EDITOR ~~~~~~~~~~~~~~~~~~~~~~~~~
**
Author: Zaryab Muhammad Akram
**
*****************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_DEPTH 0
#define MAX_DEPTH 255


//typedefining a structre for pixels
typedef struct {
	int R, G, B;
} pixel;

//typedefining a structure for PPMimage
typedef struct {
	int row, col, colorDepth;
	pixel *body; /*array of pixels*/
} ppmFile;

//declaring functions (prototypes) 
ppmFile* readImage(char *imageFile);
void writeImage(char *imageName, ppmFile *imageFile);
void negateRed(ppmFile *imageFile);
void negateBlue(ppmFile *imageFile);
void negateGreen(ppmFile *imageFile);
void greyScale(ppmFile *imageFile);
void flattenRed(ppmFile *imageFile);
void flattenGreen(ppmFile *imageFile);
void flattenBlue(ppmFile *imageFile);
void horizontalFlip(ppmFile *imageFile);
void extremeContrast(ppmFile* imageFile);
void horizontalBlur(ppmFile *imageFile);
void randomNoise(ppmFile *imageFile, int size);

int main(void)
{
	//declaring variables 
	char inputFile[20], outputFile[20], choice[15];
	int noiseSize;

	//welcome message
	puts("+-------------------------------------------------+");
	puts("|                                                 |");
	puts("|       Portable Pixmap (PPM) Image Editor        |");
	puts("|                                                 |");
	puts("+-------------------------------------------------+\n\n");

	//promting user to enter the file names 
	printf("Enter name of image file: ");
	scanf("%s", inputFile);

	printf("Enter name of output file: ");
	scanf("%s", outputFile);

	//reading the input file
	ppmFile *readFile = readImage(inputFile);

	//promting user to opt for different options 
	puts("\nHere are your choices:");
	puts("\t[1] convert to greyscale   [2] flip horizontally");
	puts("\t[3] negative of red        [4] negative of green");
	puts("\t[5] negative of blue       [6] just the reds");
	puts("\t[7] just the greens        [8] just the blues");
	puts("\t[9] extreme contrast	   [10] blur horizontally ");
	puts("\t[11] random noise\n");

	for (int i = 0; i < 11; ++i)
	{
		/*making sure that user enters a valid choice*/
		do {
			printf("\tDo you want [%d]? (y/n): ", i + 1);
			fflush(stdin);
			scanf(" %c", &choice[i]);
		} while ((choice[i] != 'y') && (choice[i] != 'Y') && (choice[i] != 'n') && (choice[i] != 'N'));
	}

	//performing operations according to the user choice 
	if (choice[0] == 'y' || choice[0] == 'Y')
		greyScale(readFile); //applying grey scale to the image

	if (choice[1] == 'y' || choice[1] == 'Y')
		horizontalFlip(readFile); //flipping the image horizontally 

	if (choice[2] == 'y' || choice[2] == 'Y')
		negateRed(readFile); //negating red from the image

	if (choice[3] == 'y' || choice[3] == 'Y')
		negateGreen(readFile); //negating green from the image

	if (choice[4] == 'y' || choice[4] == 'Y')
		negateBlue(readFile); //negating blue from the image

	if (choice[5] == 'y' || choice[5] == 'Y') {
		flattenGreen(readFile); //flattening green numbers 
		flattenBlue(readFile); //flattening blue numbers 
	}

	if (choice[6] == 'y' || choice[6] == 'Y') {
		flattenRed(readFile); //flattening red numbers 
		flattenBlue(readFile); //flattening blue numbers 
	}

	if (choice[7] == 'y' || choice[7] == 'Y') {
		flattenRed(readFile); //flattening red numbers 
		flattenGreen(readFile); //flattening green numbers 
	}

	if (choice[8] == 'y' || choice[8] == 'Y')
		extremeContrast(readFile); //maximizing or minimizing color depth

	if (choice[9] == 'y' || choice[9] == 'Y')
		horizontalBlur(readFile); //horizontal bluring the image

	if (choice[10] == 'y' || choice[10] == 'Y'){
	printf("\n\tEnter the size(range) of the noise: ");
	scanf("%d", &noiseSize);
	randomNoise(readFile, noiseSize);
	}
	
	printf("\nPlease wait. . .");

	//writing the output image File 
	writeImage(outputFile, readFile);

	//freeing the allocated dynamic memory
	free(readFile->body);
	free(readFile);

	//closing message
	printf("\n\n\t%s created.\n\n", outputFile);

	return EXIT_SUCCESS;
}

/********************************************************
* This function reads the input and returns the ppmFile *
*********************************************************/
ppmFile* readImage(char *imageFile)
{
	//declaring variables 
	char imageFormat[4];
	FILE *fp; //file pointer for the inputFile 

	//allocating dynamic memory for Image File
	ppmFile *imagePtr = (ppmFile*)malloc(sizeof(ppmFile));
	if (!imagePtr) {
		printf("Error: Memory allocation failed!\n");
		exit(EXIT_FAILURE);
	}

	//opening ppm file in read-binary mode
	fp = fopen(imageFile, "r");
	if (!fp) //if the file failed to opened 
	{
		printf("Error: File opening failed!\n");
		exit(EXIT_FAILURE);
	}

	//reading and checking the format of the image
	fgets(imageFormat, sizeof(imageFormat), fp);
	if (imageFormat[0] != 'P' || imageFormat[1] != '3') {
		printf("Error: Image format should be P3!\n");
		exit(EXIT_FAILURE);
	}

	//reading the size of the image and maximum color
	fscanf(fp, "   %d %d   ", &imagePtr->col, &imagePtr->row);
	fscanf(fp, "  %d  ", &imagePtr->colorDepth);

	//allocating dynamic memory for pixel data 
	imagePtr->body = (pixel*)malloc(imagePtr->row * imagePtr->col * sizeof(pixel));
	if (!imagePtr) {
		printf("Error: Memory allocation failed!\n");
		exit(EXIT_FAILURE);
	}

	//reading pixel body of the image
	for (int i = 0; i < imagePtr->row * imagePtr->col; ++i)
		fscanf(fp, "    %d %d %d    ", &(imagePtr->body[i].R), &(imagePtr->body[i].G), &(imagePtr->body[i].B));

	fclose(fp); /*closing file*/
	return imagePtr; /*returning the read file*/
}

/********************************************************************
* This function creates and writes to the file as inputted by user *
********************************************************************/
void writeImage(char *imageName, ppmFile *imageFile)
{
	FILE *fp; /*File pointer for the outputFile*/

	/*creating the output file*/
	fp = fopen(imageName, "w");
	if (!fp) //if file creation fails 
	{
		printf("Error: Could not create the file!\n");
		exit(EXIT_FAILURE);
	}

	//writing image header
	fprintf(fp, "P3"); /*Magic Number*/
	fprintf(fp, "   %d %d   ", imageFile->col, imageFile->row);  /*columns and rows*/
	fprintf(fp, "  %d  ", imageFile->colorDepth); /*Maximum color depth*/

												  //writing image body
	for (int i = 0; i < (imageFile->row * imageFile->col); ++i)
		fprintf(fp, "    %d %d %d    ", imageFile->body[i].R, imageFile->body[i].G, imageFile->body[i].B);

	fclose(fp); /*closing the created file*/
}


/*****************************************************
* This function negate the red number of each pixel *
******************************************************/
void negateRed(ppmFile *imageFile)
{
	/*if the red is low, it will become high and vice versa, being in range*/
	for (int i = 0; i < imageFile->col * imageFile->row; i++)
		imageFile->body[i].R = abs(imageFile->body[i].R - MAX_DEPTH);
}


/*****************************************************
* This function negate the Blue number of each pixel *
******************************************************/
void negateBlue(ppmFile *imageFile)
{
	/*if the blue is low, it will become high and vice versa, being in range*/
	for (int i = 0; i < imageFile->col * imageFile->row; i++)
		imageFile->body[i].B = abs(imageFile->body[i].B - MAX_DEPTH);
}


/*****************************************************
* This function negate the Green number of each pixel *
******************************************************/
void negateGreen(ppmFile *imageFile)
{
	/*if the green is low, it will become high and vice versa, being in range*/
	for (int i = 0; i < imageFile->col * imageFile->row; i++)
		imageFile->body[i].G = abs(imageFile->body[i].G - MAX_DEPTH);
}


/******************************************************************
* This function sets each pixel value to the average of the three *
*******************************************************************/
void greyScale(ppmFile *imageFile)
{
	int averageColor;
	for (int i = 0; i < imageFile->col * imageFile->row; i++)
	{
		//communting average of three colour numbers 
		averageColor = (imageFile->body[i].R + imageFile->body[i].G + imageFile->body[i].B) / 3;

		//equating all colour numbers to average
		imageFile->body[i].R = averageColor;
		imageFile->body[i].G = averageColor;
		imageFile->body[i].B = averageColor;
	}
}


/************************************************
* This function sets all the Red value to zero *
***********************************************/
void flattenRed(ppmFile *imageFile)
{
	for (int i = 0; i < imageFile->col * imageFile->row; i++)
		imageFile->body[i].R = 0;  /* equating colour number to 0 */
}


/*************************************************
* This function sets all the Green value to zero *
************************************************/
void flattenGreen(ppmFile *imageFile)
{
	for (int i = 0; i < imageFile->col * imageFile->row; i++)
		imageFile->body[i].G = 0;  /* equating colour number to 0 */
}


/************************************************
* This function sets all the Blue value to zero *
*************************************************/
void flattenBlue(ppmFile *imageFile)
{
	for (int i = 0; i < imageFile->col * imageFile->row; i++)
		imageFile->body[i].G = 0;  /* equating colour number to 0 */
}


/*********************************************
* This function flips the image horizontally *
**********************************************/
void horizontalFlip(ppmFile *imageFile)
{
	//commuting the centre of the image 
	int imageCentre = imageFile->col / 2;

	for (int i = 0; i < imageFile->row; ++i)
	{
		pixel *tmpArray = imageFile->body + i * imageFile->col;  /*Temporary array of pixels*/
		for (int j = 0; j < imageCentre; ++j)
		{
			//swapping pixels 
			pixel temp = tmpArray[j];
			tmpArray[j] = tmpArray[imageFile->col - 1 - j];
			tmpArray[imageFile->col - 1 - j] = temp;
		}
	} //end for
} //end function 


 /*********************************************************************
 * This function change each color number to either the highest color
 number possible or to 0, comparing it with half of color depth *
 *********************************************************************/
void extremeContrast(ppmFile* imageFile)
{
	/* if color number is greater than half of colour depth, it is set to 255, else 0 */
	for (int i = 0; i < imageFile->col * imageFile->row; i++)
	{
		if (imageFile->body[i].R > 128)   /* 128 = half of the color depth */
			imageFile->body[i].R = MAX_DEPTH;
		else
			imageFile->body[i].R = MIN_DEPTH;

		if (imageFile->body[i].G > 128)
			imageFile->body[i].G = MAX_DEPTH;
		else
			imageFile->body[i].G = MIN_DEPTH;

		if (imageFile->body[i].B > 128)
			imageFile->body[i].B = MAX_DEPTH;
		else
			imageFile->body[i].B = MIN_DEPTH;
	}
}

/*****************************************************
* This function negate the blue number of each pixel *
******************************************************/
void horizontalBlur(ppmFile *imageFile)
{
	int meanRed, meanBlue, meanGreen;
	for (int i = 0, j; i < (imageFile->col * imageFile->row); i += 3)
	{
		//initiallizing all mean values to 0
		meanRed = 0, meanBlue = 0, meanGreen = 0;

		j = i;
		//commuting mean value for Red
		meanRed += (imageFile->body[j++].R + imageFile->body[j++].R + imageFile->body[j].R);
		meanRed /= 3;

		j = i;
		//equating pixel colors to mean value
		imageFile->body[j++].R = meanRed;
		imageFile->body[j++].R = meanRed;
		imageFile->body[j].R = meanRed;


		j = i;
		//commuting mean value for Blue
		meanBlue += (imageFile->body[j++].B + imageFile->body[j++].B + imageFile->body[j].B);
		meanBlue /= 3;

		j = i;
		//equating pixel colors to mean value
		imageFile->body[j++].B = meanBlue;
		imageFile->body[j++].B = meanBlue;
		imageFile->body[j].B = meanBlue;

		j = i;
		//commuting mean value for Blue
		meanGreen += (imageFile->body[j++].G + imageFile->body[j++].G + imageFile->body[j].G);
		meanGreen /= 3;

		j = i;
		//equating pixel colors to mean value
		imageFile->body[j++].G = meanGreen;
		imageFile->body[j++].G = meanGreen;
		imageFile->body[j].G = meanGreen;
	}
}

/***********************************************************************
* This function adds/substracts a random number from each color number *
************************************************************************/
void randomNoise(ppmFile *imageFile, int size)
{
	srand(time(NULL)); /* generate different random numbers everytime */
	int sign;

	for (int i = 0; i < imageFile->col * imageFile->row; i++)
	{
		sign = rand() % 2; /*random number from 0-1*/
	//If sign==1 then random number is added else subtracted 
		if (sign == 1)
			imageFile->body[i].R += rand() % size;
		else
			imageFile->body[i].R -= rand() % size;

		/* If the new Red number exceeds the range, set it to the closet boundary */
		if (imageFile->body[i].R < 0)
			imageFile->body[i].R = MIN_DEPTH;
		if (imageFile->body[i].R > 255)
			imageFile->body[i].R = MAX_DEPTH;

		sign = rand() % 2;
		/* If sign==1 then random number is added else subtracted */
		if (sign == 1)
			imageFile->body[i].G += rand() % size;
		else
			imageFile->body[i].G -= rand() % size;

		/* If the new Green number exceeds the range, set it to the closet boundary */
		if (imageFile->body[i].G < 0)
			imageFile->body[i].G = MIN_DEPTH;
		if (imageFile->body[i].G > 255)
			imageFile->body[i].G = MAX_DEPTH;

		sign = rand() % 2;
		/* If sign==1 then random number is added else subtracted */
		if (sign == 1)
			imageFile->body[i].B += rand() % size;
		else
			imageFile->body[i].B -= rand() % size;

		/* If the new Blue number exceeds the range, set it to the closet boundary */
		if (imageFile->body[i].B < 0)
			imageFile->body[i].B = MIN_DEPTH;
		if (imageFile->body[i].B > 255)
			imageFile->body[i].B = MAX_DEPTH;
	} //end for
}