#include "Advanced.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

/* linear Normalize an image*/
Image *Normalize(Image *image, int newMinR, int newMaxR, int newMinG, int newMaxG, int newMinB, int newMaxB)
{
	assert(image);

	unsigned int Width = ImageWidth(image);
	unsigned int Height = ImageHeight(image);

	int MaxR = MIN_PIXEL, MaxG = MIN_PIXEL, MaxB = MIN_PIXEL;
	int MinR = MAX_PIXEL, MinG = MAX_PIXEL, MinB = MAX_PIXEL;

	for (int i = 0; i < Width; i++ ) {
		for(int j = 0; j < Height; j++){
			if(GetPixelR(image,i,j) > MaxR)
        			MaxR = GetPixelR(image,i,j);
			if(GetPixelG(image,i,j) > MaxG)
        			MaxG = GetPixelG(image,i,j);
      			if(GetPixelB(image,i,j) > MaxB)
        			MaxB = GetPixelB(image,i,j);

			if(GetPixelR(image,i,j) < MinR)
        			MinR = GetPixelR(image,i,j);
			if(GetPixelG(image,i,j) < MinG)
				MinG = GetPixelG(image,i,j);
			if(GetPixelB(image,i,j) < MinB)
			        MinB = GetPixelB(image,i,j);
		}
	}
	
	int A,B,C;	

	for (int i = 0; i < Width; i++ ) {
		for(int j = 0; j < Height; j++){
		
		A = (GetPixelR(image,i,j) - MinR) * (newMaxR - newMinR) / (MaxR - MinR) + newMinR;
		B = (GetPixelG(image,i,j) - MinG) * (newMaxG - newMinG) / (MaxG - MinG) + newMinG;
		C = (GetPixelB(image,i,j) - MinB) * (newMaxB - newMinB) / (MaxB - MinB) + newMinB;
		
		SetPixelR(image,i,j,A);
		SetPixelG(image,i,j,B);
		SetPixelB(image,i,j,C);
		
		}
	}
	return image;
}


/* make the image posterized */
Image *Posterize(Image *image, int rbits, int gbits, int bbits)
{	
	assert(image);

	unsigned int Width = ImageWidth(image);
	unsigned int Height = ImageHeight(image);
	
	int x, y, Tempr, Tempg, Tempb, r, g, b;

	for (x=0; x<Width; x++) {
		for (y=0; y<Height; y++) {

			r = GetPixelR(image,x,y);
			g = GetPixelG(image,x,y);
			b = GetPixelB(image,x,y);

			unsigned char One = 1;
			int i;
		
			for (i=0; i<rbits-1; i++) {
				r |= One;
				One <<= 1;
			} 
			One = ~One;
			r &= One;
			Tempr = r;
			SetPixelR(image,x,y,Tempr);
			One = 1;
		
		
			for (i=0; i<gbits-1; i++) {
				g |= One;
				One <<= 1;	
			}
			One = ~One;
			g &= One;
			Tempg = g;
			SetPixelG(image,x,y,Tempg);
			One = 1;	
					

			for (i=0; i<bbits-1; i++) {
				b |= One;
				One <<= 1;
			}
			One = ~One;
			b &= One;
			Tempb = b;
			SetPixelB(image,x,y,Tempb);
			
		}
	}
	return image;
}


/* rotate and zoom the image */
Image *Rotate(Image *image, double Angle, double ScaleFactor, int CenterX, int CenterY) 
{
	assert(image);
 
	Image *RotatedR;
	RotatedR = CreateImage(ImageWidth(image), ImageHeight(image));

	Image *RotatedG;
        RotatedG = CreateImage(ImageWidth(image), ImageHeight(image));
    
	Image *RotatedB;
        RotatedB = CreateImage(ImageWidth(image), ImageHeight(image));
    
	int x, y;
	int x2, y2;
	const double pi = 3.14159265358979323846;
        double Radian;
        double ScaleX, ScaleY;
        double M[2][2];
        int W = ImageWidth(image);
        int H = ImageHeight(image);


        Radian = -Angle/360.0 * 2*pi;
        ScaleX = ScaleFactor;
        ScaleY = ScaleFactor;
        M[0][0] = cos(Radian)/ScaleX;
        M[0][1] = -sin(Radian)/ScaleX;
        M[1][0] = sin(Radian)/ScaleY;
        M[1][1] = cos(Radian)/ScaleY;
    	
	for(y=0; y<ImageHeight(image); y++)
    	{   for(x=0; x<ImageWidth(image); x++)
        	{
            	x2 = (x-CenterX)*M[0][0] + (y-CenterY)*M[0][1] + CenterX;
            	y2 = (x-CenterX)*M[1][0] + (y-CenterY)*M[1][1] + CenterY;
            	if (  (x2<0) || (x2>W-1) || (y2<0) || (y2>H-1))
            		{
               SetPixelR(RotatedR, x, y, 0);
               SetPixelG(RotatedG, x, y, 0);
               SetPixelB(RotatedB, x, y, 0);
            		}
            else
            		{
              SetPixelR(RotatedR, x, y, GetPixelR(image, x2, y2));
              SetPixelG(RotatedG, x, y, GetPixelG(image, x2, y2));
              SetPixelB(RotatedB, x, y, GetPixelB(image, x2, y2));
            		}
       		 }
	}
    	
	for(y=0; y<H; y++) {
	     for(x=0; x<W; x++) {

		SetPixelR(image, x, y, GetPixelR(RotatedR, x, y));
            	SetPixelG(image, x, y, GetPixelG(RotatedG, x, y));
            	SetPixelB(image, x, y, GetPixelB(RotatedB, x, y));

	        }
    	}
        
	DeleteImage(RotatedR);
        RotatedR = NULL;

        DeleteImage(RotatedG);
        RotatedG = NULL;

        DeleteImage(RotatedB);
        RotatedB = NULL;

        return image;
} /* end of Rotate */

/* Add motion blur to an image */
Image *MotionBlur(Image *image, unsigned char BlurAmount)
{
	assert(image);

	unsigned int Width = ImageWidth(image);
	unsigned int Height = ImageHeight(image);

	int x,y,m;
	double temp_r , temp_g , temp_b;
	int n;

	for (x = 0; x < Width-1; x++)
	{
		for (y = 0; y < Height ; y++) 
		{
			temp_r = GetPixelR(image,x,y)/2;
			temp_b = GetPixelG(image,x,y)/2;
			temp_g = GetPixelB(image,x,y)/2;
			for (m = 1; m<=BlurAmount ; m++)
			{
				if ((x+m) < Width)
				{
					if (BlurAmount < Width-x-1)
					{
						n = BlurAmount;
					}
					else
					{
						n = Width-x-1;
					}
					temp_r = temp_r + GetPixelR(image,x+m,y) * 0.5/n;
					temp_b = temp_b + GetPixelG(image,x+m,y) * 0.5/n;
					temp_g = temp_g + GetPixelB(image,x+m,y) * 0.5/n;
				}
			}
			SetPixelR(image,x,y,temp_r);
			SetPixelG(image,x,y,temp_g);
			SetPixelB(image,x,y,temp_b); 
		}	
	}
	return image;
}


/* Adjust the brightness and contrast of an image) */	
Image *BrightnessAndContrast (Image *image, int brightness, int contrast) 
{
	assert(image);

	unsigned int Width = ImageWidth(image);
	unsigned int Height = ImageHeight(image);

	int Tempbr, Tempbg, Tempbb;
	int Tempcr, Tempcg, Tempcb;
	
	int x, y;

	if(brightness > 225) {
		brightness = 255; 
	}
	else if(brightness < -255) {
		brightness = -255; 
	}
	if(contrast > 255) {
		contrast = 255; }
	else if(contrast < -255) {
		contrast = -255;
	}
	
	Image *bnc = CreateImage(Width,Height);
	double factor = (double) (259 * (contrast + 255)) / (double) (255 * (259 - contrast));

	for (x=0;x<Width;x++) {
		for (y=0;y<Height;y++) {
			
			Tempbr = brightness + GetPixelR(image,x,y);
			
			if(Tempbr > 255) {
				Tempbr = 255;
			}
			else if(Tempbr < 0) {
				Tempbr = 0;
			}
			
			SetPixelR(bnc,x,y,Tempbr);
			Tempcr = (int) (factor * (Tempbr - 128) + 128);


			if(Tempcr > 255) {
				Tempcr = 255;
			}
			else if(Tempcr < 0) {
				Tempcr = 0;
			}
			
			SetPixelR(bnc,x,y,Tempcr);
			Tempbg = brightness + GetPixelG(image,x,y);


			if(Tempbg > 255) {
				Tempbg = 255; 
			}
			else if(Tempbg < 0) {
				Tempbg = 0;
			}
			
			SetPixelG(bnc,x,y,Tempbg);
			Tempcg = (int) (factor * (Tempbg - 128) + 128);


			if(Tempcg > 255) {
				Tempcg = 255; 
			}
			else if(Tempcg < 0) {
				Tempcg = 0;
			}
			
			SetPixelG(bnc,x,y,Tempcg);
			Tempbb = brightness + GetPixelB(image,x,y);


			if(Tempbb > 255) {
				Tempbb = 255; 
			}
			else if(Tempbb < 0) {
				Tempbb = 0;
			}

			SetPixelB(bnc,x,y,Tempbb);
			Tempcb = (int) (factor * (Tempbb - 128) + 128);

			
			if(Tempcb > 255) {
				Tempcb = 255;
			}
			else if(Tempcb < 0) {
				Tempcb = 0;
			}

			SetPixelB(bnc,x,y,Tempcb);
			
		}
	
	}
	DeleteImage(image);
	return bnc;
}


/* Crop an image */
Image *Crop(Image *image, int x, int y, int W, int H)
{
	assert(image);
 
	W = (W > image->W) ? image->W : W;
	H = (H > image->H) ? image->H : H;
	x = (x > image->W - 1) ? image->W - 1 : x;
	y = (y > image->H - 1) ? image->H - 1 : y;
    

	Image *OutPutImage;
	if ((x + W > image->W) && (y + H < image->H)) {
	        OutPutImage = CreateImage(image->W - x, H);
    	}
    

	else if ((x + W < image->W) && (y + H > image->H)) {
        	OutPutImage = CreateImage(W, image->H - y);
    	}
    
	else if ((x + W > image->W) && (y + H > image->H)) {
        	OutPutImage = CreateImage(image->W - x, image->H - y);
    	}
    
	else {
        	OutPutImage = CreateImage(W, H);
    	}
    
	if (!OutPutImage) return image;
    
	int i, j;
    
	for (i = 0; i < OutPutImage->W; i++) {
        	for (j = 0; j < OutPutImage->H; j++) {

            		SetPixelR(OutPutImage, i, j, GetPixelR(image, x + i, y + j));
            		SetPixelG(OutPutImage, i, j, GetPixelG(image, x + i, y + j));
            		SetPixelB(OutPutImage, i, j, GetPixelB(image, x + i, y + j));

		}
	}
	DeleteImage(image);
	image = NULL;
	return OutPutImage;	
}
