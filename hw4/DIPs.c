#include "DIPs.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>


/* Reverse image color */
Image *Negative(Image *image)
{
	assert(image);
	
	int x, y;
	int a, b, c;

	for (y = 0; y < image->H; y++) {
		for (x = 0; x < image->W; x++) {

			a = MAX_PIXEL - GetPixelR(image,x,y);
			b = MAX_PIXEL - GetPixelG(image,x,y);
			c = MAX_PIXEL - GetPixelB(image,x,y);

			SetPixelR(image,x,y,a);
			SetPixelG(image,x,y,b);
			SetPixelB(image,x,y,c);
		}
	}
	return image;
}


/* color filter */
Image *ColorFilter(Image *image, int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b) {

	assert(image);

	int x, y;

	replace_r = (replace_r > MAX_PIXEL) ? MAX_PIXEL : (replace_r < 0) ? 0 : replace_r;
	replace_g = (replace_g > MAX_PIXEL) ? MAX_PIXEL : (replace_g < 0) ? 0 : replace_g;
	replace_b = (replace_b > MAX_PIXEL) ? MAX_PIXEL : (replace_b < 0) ? 0 : replace_b;

		
	for (y=0; y<image->H; y++) {
		for (x=0; x<image->W; x++) {
			if (abs(GetPixelR(image,x,y) - target_r) <= threshold
					&& abs(GetPixelG(image,x,y - target_g) <= threshold
					&& abs(GetPixelB(image,x,y - target_b) <= threshold))) {
				SetPixelR(image,x,y,replace_r);
				SetPixelG(image,x,y,replace_g);
				SetPixelB(image,x,y,replace_b);
			}
		}
	}
   return image;

}


/* edge detection */
Image *Edge(Image *image)
{
	assert(image);
	
	unsigned int Width = ImageWidth(image);
	unsigned int Height = ImageHeight(image);

	int x, y, m, n, a, b;
	
	unsigned char   TempR[Width*Height];
	unsigned char   TempG[Width*Height];
	unsigned char   TempB[Width*Height];
	
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			TempR[x+y*Width] = GetPixelR(image,x,y);
			TempG[x+y*Width] = GetPixelG(image,x,y);
			TempB[x+y*Width] = GetPixelB(image,x,y);
		}
	}
	
	int SUMR = 0;   
	int SUMG = 0;
	int SUMB = 0;
	
	for (y = 1; y < Height - 1; y++) {
		for (x = 1; x < Width - 1; x++) {
			for (n = -1; n <= 1; n++){
				for (m = -1; m <= 1; m++) {
					a = (x + m >= Width) ? Width - 1 : (x + m < 0) ? 0 : x + m;
					b = (y + n >= Height) ? Height - 1 : (y + n < 0) ? 0 : y + n;
	
					SUMR += (TempR[x+y*Width] - TempR[a+b*Width]);
					SUMR += (TempG[x+y*Width] - TempG[a+b*Width]);	
					SUMR += (TempB[x+y*Width] - TempB[a+b*Width]);
				}
			}
			SetPixelR(image,x,y,SUMR);
			SetPixelG(image,x,y,SUMG);
			SetPixelB(image,x,y,SUMB);
			
			SUMR = (SUMR > MAX_PIXEL) ? MAX_PIXEL: (SUMR < MIN_PIXEL) ? MIN_PIXEL: SUMR;
			SUMG = (SUMG > MAX_PIXEL) ? MAX_PIXEL: (SUMG < MIN_PIXEL) ? MIN_PIXEL: SUMG;
			SUMB = (SUMB > MAX_PIXEL) ? MAX_PIXEL: (SUMB < MIN_PIXEL) ? MIN_PIXEL: SUMB;

			SetPixelR(image,x,y,SUMR);
			SetPixelG(image,x,y,SUMG);
			SetPixelB(image,x,y,SUMB);
		
			SUMR = SUMG = SUMB = 0;
			}
		}
		


	/* set all four borders to 0 */

	for (y=0;y<Height;y++) {
		
		SetPixelR(image,0,y,0);
		SetPixelG(image,0,y,0);
		SetPixelB(image,0,y,0);
		
		SetPixelR(image,Width-1,y,0);
		SetPixelG(image,Width-1,y,0);
		SetPixelB(image,Width-1,y,0);
	
	}
	for (x=0;x<Width;x++) {
		
		SetPixelR(image,x,0,0);
		SetPixelG(image,x,0,0);
		SetPixelB(image,x,0,0);
		
		SetPixelR(image,x,Height-1,0);
		SetPixelG(image,x,Height-1,0);
		SetPixelB(image,x,Height-1,0);
	}
	return image;
}

	
/* mirror image horizontally */
Image *HMirror(Image *image) {
	assert(image);
	int x, y;

	unsigned int Width = ImageWidth(image);
	unsigned int Height = ImageHeight(image);
	
	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width / 2; x++) {
			SetPixelR(image,Width - 1 - x,y,GetPixelR(image,x,y));
			SetPixelG(image,Width - 1 - x,y,GetPixelG(image,x,y));
			SetPixelB(image,Width - 1 - x,y,GetPixelB(image,x,y));
		}
	}
	return image;
}


/* add border to the image */
Image *AddBorder(Image *image, char *color, int border_width) {

	unsigned int Width = ImageWidth(image);
	unsigned int Height = ImageHeight(image);	

	int x, y;
	int border_r = 255;
	int border_g = 255;
	int border_b = 255;

	if (!strcmp(color, "black")) {
		border_r = 0;
		border_g = 0;
		border_b = 0;
	} else if (!strcmp(color, "white")) {
		border_r = 255;
		border_g = 255;
		border_b = 255;
	} else if (!strcmp(color, "red")) {
		border_r = 255;
		border_g = 0;
		border_b = 0;
	} else if (!strcmp(color, "green")) {
		border_r = 0;
		border_g = 255;
		border_b = 0;
	} else if (!strcmp(color, "blue")) {
		border_r = 0;
		border_g = 0;
		border_b = 255;
	} else if (!strcmp(color, "yellow")) {
		border_r = 255;
		border_g = 255;
		border_b = 0;
	} else if (!strcmp(color, "cyan")) {
		border_r = 0;
		border_g = 255;
		border_b = 255;
	} else if (!strcmp(color, "pink")) {
		border_r = 255;
		border_g = 192;
		border_b = 203;
	} else if (!strcmp(color, "orange")) {
		border_r = 255;
		border_g = 165;
		border_b = 0;
	} else {
		printf("Unsurported color.\n");
		return image;
	}

	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width; x++) {
			if ((y < border_width) || (y > Height - 1 - border_width) ||
					(x < border_width) || (x > Width - 1 - border_width)) {
				SetPixelR(image,x,y,border_r);
				SetPixelG(image,x,y,border_g);
				SetPixelB(image,x,y,border_b);
			}
		}
	}
	return image;
}


/* Black and White */
Image *BlackNWhite(Image *image){
	
	assert(image);

	unsigned int Width = ImageWidth(image);
	unsigned int Height = ImageHeight(image);	
	
	int x, y;
	int L;

	for (y=0; y < Height; y++) {
		for (x=0; x < Width; x++) {
			
			L = (GetPixelR(image,x,y) + GetPixelG(image,x,y) + GetPixelB(image,x,y)) / 3;

			SetPixelR(image,x,y,L); 
			SetPixelG(image,x,y,L);
			SetPixelB(image,x,y,L);
		
		}
	}
	return image;
}


/* flip image horizontally */
Image *HFlip(Image *image)
{
	assert(image);
	
	unsigned int Width = ImageWidth(image);
	unsigned int Height = ImageHeight(image);

	int x, y;
	unsigned char r, g, b;

	for (y = 0; y < Height; y++) {
		for (x = 0; x < Width / 2; x++) {
		

			r = GetPixelR(image,Width-1-x,y);
			g = GetPixelG(image,Width-1-x,y);
			b = GetPixelB(image,Width-1-x,y);	

			SetPixelR(image,Width-1-x,y,GetPixelR(image,x,y));
			SetPixelG(image,Width-1-x,y,GetPixelG(image,x,y));
			SetPixelB(image,Width-1-x,y,GetPixelB(image,x,y));

			SetPixelR(image,x,y,r);
			SetPixelG(image,x,y,g);
			SetPixelB(image,x,y,b);
		}
	}
	return image;
}


/* Shuffle the image */
Image *Shuffle(Image *image)
{
	unsigned int Width = ImageWidth(image);
	unsigned int Height = ImageHeight(image);

	assert(image);
	
	int block_cnt = SHUFF_HEIGHT_DIV * SHUFF_WIDTH_DIV;
	int block_width = Width/SHUFF_WIDTH_DIV;
	int block_height = Height/SHUFF_HEIGHT_DIV;
	int block[SHUFF_WIDTH_DIV][SHUFF_HEIGHT_DIV];
	int i, j;

	srand(time(NULL));
	/* Initialize block markers to not done (-1) */
	for (i = 0; i < SHUFF_WIDTH_DIV; i++) {
		for (j = 0; j< SHUFF_HEIGHT_DIV; j++) {
			block[i][j] = -1;
		}
	}

	while (block_cnt > 0) {
		/* Generate a random pair of blocks */
		int dest_height = rand() % SHUFF_HEIGHT_DIV;
		int dest_width = rand() % SHUFF_WIDTH_DIV;
		int src_height = rand() % SHUFF_HEIGHT_DIV;
		int src_width = rand() % SHUFF_WIDTH_DIV;

		/* Check if these blocks are already swaped, if not swap blocks */
		if ((block[dest_width][dest_height] == -1) && (block[src_width][src_height] == -1) && (dest_height!=src_height || dest_width!=src_width)) {
			/* Swap blocks */
			for (i = 0; i < block_height; i++) {
				/* Init src and dest height offset */
				int h_dest = ((dest_height * block_height) + i) % Height;
				int h_src  = ((src_height * block_height) + i) % Height;
				for (j = 0; j < block_width; j++) {
					int TEMP;
					/* Init src and dest width offset */
					int w_src  = ((src_width * block_width) + j) % Width;
					int w_dest = ((dest_width * block_width) + j) % Width;

					TEMP = GetPixelR(image,w_dest,h_dest);
					SetPixelR(image,w_dest,h_dest,GetPixelR(image,w_src,h_src));
					SetPixelR(image,w_src,h_src,TEMP);

					TEMP = GetPixelG(image,w_dest,h_dest);
					SetPixelG(image,w_dest,h_dest,GetPixelG(image,w_src,h_src));
					SetPixelG(image,w_src,h_src,TEMP);

					TEMP = GetPixelB(image,w_dest,h_dest);
					SetPixelB(image,w_dest,h_dest,GetPixelB(image,w_src,h_src));
					SetPixelB(image,w_src,h_src,TEMP);
				}
			}
			
			/* Set marker as done */
			block[dest_width][dest_height] = 1;
			block[src_width][src_height] = 1;
			/* Decrease block count */
			block_cnt -= 2; /* Two blocks are swapped */
		}
	}
	return image;
}
