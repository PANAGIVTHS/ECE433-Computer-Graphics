#ifndef _BMP_H

#define _BMP_H

typedef struct BITMAPFILEHEADER_struct{
	
	unsigned short int bfType;  //specifies the file type (must always be set to 'BM' for a .bmp file)
	unsigned int bfSize;  //specifies the size in bytes of the bitmap file
	unsigned short int bfReserved1;  //must always be zero 
	unsigned short int bfReserved2;  //must always be zero
	unsigned int bOffBits;  //specifies the offset from the beginning of the file to the bitmap data

}bitmapFileHeader_t;

typedef struct BITMAPINFOHEADER_struct{
	
	unsigned int biSize;  //specifies the size of the BITMAPINFOHEADER structure, in bytes
	unsigned int biWidth;  //specifies the width of the image in pixels
	unsigned int biHeight;  //species the height of the image in pixels
	unsigned short int biPlanes; //specifies the number of color planes, must be set to zero
	unsigned short int biBitCount; //specifies the number of bit per pixel
	int biCompression;//spcifies the type of compression (usually set to zero)
	unsigned int biSizeImage;  //size of image in bytes (If there is no compression, it is valid to set this member to zero)
	unsigned int biXPelsPerMeter;  //number of pixels per meter in x axis (usually set to zero)
	unsigned int biYPelsPerMeter;  //number of pixels per meter in y axis (usually set to zero)
	unsigned int biClrUsed;  //specifies the number of colors used in the bitmap, if set to zero the number of colors is calculated using the biBitCount member
	unsigned int biClrImportant;  //specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important

}bitmapInfoHeader_t;

unsigned char *LoadBitmapFile(char *filename, bitmapFileHeader_t *bitmapFileHeader, bitmapInfoHeader_t *bitmapInfoHeader);
void reverseImage(unsigned char *imageArray, unsigned int byteWidth, unsigned int height, unsigned int padding);
#endif