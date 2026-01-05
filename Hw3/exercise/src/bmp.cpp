#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"

unsigned char *LoadTextureFile(char *filename, bitmapFileHeader_t *bitmapFileHeader, bitmapInfoHeader_t *bitmapInfoHeader) 
{

	FILE *filePtr; //file pointer
	
	unsigned short int bfType;  //specifies the file type (must always be set to 'BM' for a .bmp file)
	unsigned int bfSize;  //specifies the size in bytes of the bitmap file
	unsigned short int bfReserved1;  //must always be zero 
	unsigned short int bfReserved2;  //must always be zero
	unsigned int bOffBits;  //specifies the offset from the beginning of the file to the bitmap data
	unsigned int biSize;  //specifies the size of the BITMAPINFOHEADER structure, in bytes
	int biWidth;  //specifies the width of the image in pixels
	int biHeight;  //species the height of the image in pixels
	unsigned short int biPlanes; //specifies the number of color planes, must be set to zero
	unsigned short int biBitCount; //specifies the number of bit per pixel
	int biCompression;//spcifies the type of compression (usually set to zero)
	unsigned int biSizeImage;  //size of image in bytes (If there is no compression, it is valid to set this member to zero)
	int biXPelsPerMeter;  //number of pixels per meter in x axis (usually set to zero)
	int biYPelsPerMeter;  //number of pixels per meter in y axis (usually set to zero)
	unsigned int biClrUsed;  //specifies the number of colors used in the bitmap, if set to zero the number of colors is calculated using the biBitCount member
	unsigned int biClrImportant;  //specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important
	
	unsigned char *bitmapImage;
	unsigned int size;
	
    //open filename in read binary mode
	filePtr = fopen(filename,"rb");
	if (filePtr == NULL) {
		
		printf( "Can't find file with filename %s in current directory\n", filename );
		exit(0);

	}
	
	// Read the bitmap info
	
	fread(&bfType, sizeof(unsigned short int),1,filePtr);
	fread(&bfSize, sizeof(unsigned int),1,filePtr);
	fread(&bfReserved1, sizeof(unsigned short int),1,filePtr);
	fread(&bfReserved2, sizeof(unsigned short int),1,filePtr);
	fread(&bOffBits, sizeof(unsigned int),1,filePtr);
	fread(&biSize, sizeof(unsigned int),1,filePtr);
	fread(&biWidth, sizeof(int),1,filePtr);
	fread(&biHeight, sizeof(int),1,filePtr);
	fread(&biPlanes, sizeof(unsigned short int),1,filePtr);
	fread(&biBitCount, sizeof(unsigned short int),1,filePtr);
	fread(&biCompression, sizeof(int),1,filePtr);
	fread(&biSizeImage, sizeof(unsigned int),1,filePtr);
	fread(&biXPelsPerMeter, sizeof(int),1,filePtr);
	fread(&biYPelsPerMeter, sizeof(int),1,filePtr);
	fread(&biClrUsed, sizeof(unsigned int),1,filePtr);
	fread(&biClrImportant, sizeof(unsigned int),1,filePtr);
	
	//verify that this is a bmp file by check bitmap id
	if (bfType !=0x4D42)
	{
		printf("Can handle only bitmap files\n");
		fclose(filePtr);
		exit(0);
	}
	
	//only 24-bit images
	if(biBitCount != 24)
	{
		
		printf("Can only handle 24bit images\n");
		fclose(filePtr);
		exit(0);
	
	}

	/* We assume that the bytes of each column is divisible by 4 so as
	 * no padding is needed
	 */

	if(3*biWidth % 4) {

		printf("The number of columns must be divisible by 4\n");
		fclose(filePtr);
		exit(0);

	}
	
	//initialize the structs
	
	bitmapFileHeader->bfType = bfType;
	bitmapFileHeader->bfSize = bfSize;
	bitmapFileHeader->bfReserved1 = bfReserved1;
	bitmapFileHeader->bfReserved2 = bfReserved2;
	bitmapFileHeader->bOffBits = bOffBits;
	
	bitmapInfoHeader->biSize = biSize;
	bitmapInfoHeader->biWidth = biWidth;
	bitmapInfoHeader->biHeight = biHeight;
	bitmapInfoHeader->biPlanes = biPlanes;
	bitmapInfoHeader->biBitCount = biBitCount;
	bitmapInfoHeader->biCompression = biCompression;
	bitmapInfoHeader->biSizeImage = biSizeImage;
	bitmapInfoHeader->biXPelsPerMeter = biXPelsPerMeter;
	bitmapInfoHeader->biYPelsPerMeter = biYPelsPerMeter;
	bitmapInfoHeader->biClrUsed = biClrUsed;
	bitmapInfoHeader->biClrImportant = biClrImportant;
	
	//calculate the size of the image
	size = bfSize - bOffBits;
	
	//allocate enough memory for the bitmap image data
	bitmapImage = (unsigned char *)malloc(size*sizeof(unsigned char));
	
	//verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		perror("Sustem has run out of memory\n");
		exit(0);
	}
	

	//read the bitmap image
	fread(bitmapImage, sizeof(unsigned char), size, filePtr);
	fclose(filePtr);
		
	return bitmapImage;

}

unsigned char *LoadTextureFile(char *filename, int width, int height) 
{
    FILE *filePtr;
    unsigned char *bitmapImage;
    long size;

    // Open file in read binary mode
    filePtr = fopen(filename, "rb");
    if (filePtr == NULL) {
        printf("Can't find file with filename %s in current directory\n", filename);
        return NULL;
    }

    // Calculate size: Width * Height * 3 bytes (RGB)
    size = width * height * 3;

    // Allocate memory for the image data
    bitmapImage = (unsigned char *)malloc(size * sizeof(unsigned char));

    // Verify memory allocation
    if (!bitmapImage) {
        fclose(filePtr);
        perror("System has run out of memory\n");
        exit(0);
    }

    size_t result = fread(bitmapImage, 1, size, filePtr);

    if (result != size) {
        printf("Error reading raw file. Expected %ld bytes, read %zu bytes.\n", size, result);
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    fclose(filePtr);

    return bitmapImage;
}


/* reverseImage
 *
 * Purpose: Each row of a bitmap file is stored in an upside down fashion. The purpose of this function is to 
 * reverse the pixel values for the image.
 *
 * Parameters: The array with the pixel values for rgb, the image width in bytes, the image height and the padding (if present)
 *
 * Preconditions: The pixel values for the rgb are stored in the following format:
 * R1, G1, B1, R2, G2, B2, ...
 *
 * Postconditions: The imageArray contains the information for each pixel in the correct order
 *
 */


void reverseImage(unsigned char *imageArray, unsigned int byteWidth, unsigned int height, unsigned int padding)
{
	
	char *tmp;
	int iterations;
	int i;
	
	// Allocate temporary space
	tmp = (char *)malloc((byteWidth + padding)*sizeof(char));
	
	if(!tmp)
	{
		
		perror("System has run out of memory\n");
		return;
	
	}
	
	// Calculate the number of the iterations
	iterations = (height % 2) == 0 ? height/2 : (height/2) - 1;
	
	// Reverse the image
	for(i = 0; i < iterations; i++)
	{
		
		memcpy(tmp, (imageArray + i*(byteWidth + padding)), (byteWidth + padding)*sizeof(char));
		memcpy((imageArray + i*(byteWidth + padding)), (imageArray + ((height - 1) - i)*(byteWidth + padding)), (byteWidth + padding)*sizeof(char));
		memcpy((imageArray + ((height - 1) - i)*(byteWidth + padding)), tmp, (byteWidth + padding)*sizeof(char));
		
	
	}
	

}
