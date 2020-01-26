/*#include <nds.h>

#include <stdio.h>

// git adds a nice header we can include to access the data
// this has the same name as the image
#include "drunkenlogo.h"

int main(void)
{
    // set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	consoleDemoInit();

	iprintf("\n\n\tHello DS devers\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\t256 color bitmap demo");

	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

	dmaCopy(drunkenlogoBitmap, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(drunkenlogoPal, BG_PALETTE, 256*2);

	while(1) {
		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;
	}

	return 0;
}*/

/*---------------------------------------------------------------------------------

-- Snek Tu
-- Joshua

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//---------------------------------------------------------------------------------

// git adds a nice header we can include to access the data
// this has the same name as the image
#include "drunkenlogo.h"
void SinglePlayer(void);
void DoublePlayer(void);

int main(void)
{
    // set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);

	consoleDemoInit();

	iprintf("\n\n\tSNEK 2\n");
	iprintf("\tPress A for Single Player\n");
	iprintf("\tPress B for 2 Player");

	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

	dmaCopy(drunkenlogoBitmap, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(drunkenlogoPal, BG_PALETTE, 256*2);

	while(1) {
		swiWaitForVBlank();
		scanKeys();
        if (keysDown()&KEY_A) SinglePlayer();
        if (keysDown()&KEY_B) DoublePlayer();
		//if (keysDown()&KEY_START) break;
	}

	return 0;
}


void SinglePlayer(){
    //---------------------------------------------------------------------------------
	int i = 0;
	//touchPosition touch;

	videoSetMode(MODE_0_2D);
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
	vramSetBankA(VRAM_A_MAIN_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_32, true);

	u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	u16* gfx2 = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);

	for(i = 0; i < 16 * 16 / 2; i++)
	{
		gfx[i] = 1 | (1 << 8);
		gfx2[i] = 1 | (1 << 8);
	}

	vramSetBankF(VRAM_F_LCD);

	VRAM_F_EXT_SPR_PALETTE[0][1] = RGB15(31,0,0);
	VRAM_F_EXT_SPR_PALETTE[1][1] = RGB15(0,31,0);

	// set vram to ex palette
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);

    srand(time(NULL));

    int movement = 16;
	int size = 16;
    int xPos = 0;
    int yPos = 0;
	int direction = 4;// up 0, down 1, left 2, right 3, stop 4
    int count = 10;
    int snake_pos[50][2];
    float appleY = 128, appleX = 48, size_snake = 1;


	while(1) {

		scanKeys();

		int keys = keysHeld();
        if (keysDown()&KEY_START) break;
		
		if(keys & KEY_START) break;

		if(keys)
		{
			if(keys & KEY_UP)
			{
                if(direction==1){
                    direction=1;
                }
                else{direction = 0;}
				
			}
			if(keys & KEY_LEFT)
			{
				if(direction==3){
                    direction=3;
                }
                else{direction = 2;}
			}
			if(keys & KEY_RIGHT)
			{
				if(direction==2){
                    direction=2;
                }
                else{direction = 3;}
			}
			if(keys & KEY_DOWN)
			{
               if(direction==0){
                    direction=0;
                }
                else{direction = 1;}
			}
        }

        switch(direction){
            case 0:
                yPos -= movement;
                break;
            case 1:
                yPos += movement;
                break;
            case 2:
                xPos -= movement;
                break;
            case 3:
                xPos += movement;
                break;
            default:
                NULL;
        }

        if(xPos>256-size){
            xPos=0;
        }
        else if(xPos<0){
            xPos=256-size;
        }
        if(yPos>192-size){
            yPos=0;
        }
        else if(yPos<0){
            yPos=192-size;
        }
        // SNAKE
        snake_pos[0][0] = xPos;
        snake_pos[0][1] = yPos;
        if(count==10){
            movement = 16;
            count = 0;
            for(int i=0; i<2;i++){
                for(int j=size_snake; j>0;j--){
                    snake_pos[j][i]=snake_pos[j-1][i];
                }
            }
            for(int j=4; j<size_snake;j++){
                if(snake_pos[j][0]==snake_pos[0][0]&&snake_pos[j][1]==snake_pos[0][1]){
                    for(int i=0; i<2;i++){
                        for(int j=size_snake; j>0;j--){
                            snake_pos[j][i]=0;
                        }
                    }
                    for(int i=0;i<size_snake;i++){
                        oamSet(&oamMain, //main graphics engine context
                            i,           //oam index (0 to 127)  
                            snake_pos[i][0], snake_pos[i][1],   //x and y pixle location of the sprite
                            0,                    //priority, lower renders last (on top)
                            0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
                            SpriteSize_16x16,     
                            SpriteColorFormat_256Color, 
                            gfx,                  //pointer to the loaded graphics
                            -1,                  //sprite rotation data  
                            false,               //double the size when rotating?
                            true,			//hide the sprite?
                            false, false, //vflip, hflip
                            false	//apply mosaic
                            );  
                    }
                    size_snake = 1;
                    direction = 4;
                }
            }
        }
        else{movement=0;}

        for(int i=0;i<size_snake;i++){
            oamSet(&oamMain, //main graphics engine context
                i,           //oam index (0 to 127)  
                snake_pos[i][0], snake_pos[i][1],   //x and y pixle location of the sprite
                0,                    //priority, lower renders last (on top)
                0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
                SpriteSize_16x16,     
                SpriteColorFormat_256Color, 
                gfx,                  //pointer to the loaded graphics
                -1,                  //sprite rotation data  
                false,               //double the size when rotating?
                false,			//hide the sprite?
                false, false, //vflip, hflip
                false	//apply mosaic
                );  
        }

        // APPLE
        if(xPos==appleX && yPos==appleY){
            size_snake++;
            appleX = (rand() % 16+1)*16;
            appleY = (rand() % 12+1)*16;
            if(appleX==256){appleX-=16;}
            if(appleY==192){appleY-=16;}
        }
        for(int j=0; j<size_snake;j++){
            if(snake_pos[j][0]==appleX&&snake_pos[j][1]==appleY){
                appleX -=16;
                appleY -=16;
            }
        }

        oamSet(&oamMain, //main graphics engine context
            127,           //oam index (0 to 127)  
            appleX, appleY,   //x and y pixle location of the sprite
            0,                    //priority, lower renders last (on top)
            1,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
            SpriteSize_16x16,     
            SpriteColorFormat_256Color, 
            gfx2,                  //pointer to the loaded graphics
            -1,                  //sprite rotation data  
            false,               //double the size when rotating?
            false,			//hide the sprite?
            false, false, //vflip, hflip
            false	//apply mosaic
            );   		  
	
		count++;
        swiWaitForVBlank();

		
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}
	videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);

	consoleDemoInit();

	iprintf("\n\n\tSNEK 2\n");
	iprintf("\tPress A for Single Player\n");
	iprintf("\tPress B for 2 Player");

	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

	dmaCopy(drunkenlogoBitmap, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(drunkenlogoPal, BG_PALETTE, 256*2);
}

void DoublePlayer(){
    //---------------------------------------------------------------------------------
	int i = 0;
	//touchPosition touch;

	videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
	
	vramSetBankA(VRAM_A_MAIN_SPRITE);
    vramSetBankD(VRAM_D_SUB_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_32, true);
    oamInit(&oamSub, SpriteMapping_1D_32, false);

	u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	u16* gfx2 = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
    u16* gfxsub = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	u16* gfxsub2 = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);

	for(i = 0; i < 16 * 16 / 2; i++)
	{
		gfx[i] = 1 | (1 << 8);
		gfx2[i] = 1 | (1 << 8);
        gfxsub[i] = 1 | (1 << 8);
		gfxsub2[i] = 1 | (1 << 8);
	}

	vramSetBankF(VRAM_F_LCD);
    //vramSetBankG(VRAM_G_LCD);

	VRAM_F_EXT_SPR_PALETTE[0][1] = RGB15(31,0,0);
    VRAM_F_EXT_SPR_PALETTE[1][1] = RGB15(0,31,0);
    VRAM_F_EXT_SPR_PALETTE[0][0] = RGB15(31,31,0);
    VRAM_F_EXT_SPR_PALETTE[1][0] = RGB15(31,31,0);
    //VRAM_G_EXT_SPR_PALETTE[0][1] = RGB15(31,0,31);
	//VRAM_G_EXT_SPR_PALETTE[1][1] = RGB15(0,31,31);

    //set vram to ex palette
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);
    //vramSetBankG(VRAM_G_SPRITE_EXT_PALETTE);

    srand(time(NULL));

    int movement = 16;
	int size = 16;
    int xPos = 0;
    int yPos = 0;
	int direction = 4;// up 0, down 1, left 2, right 3, stop 4
    int count = 10;
    int snake_pos[50][2];
    float appleY = 128, appleX = 48, size_snake = 1;
    int xPos2 = 192;
    int yPos2 = 256;
	int direction2 = 4;// up 0, down 1, left 2, right 3, stop 4
    int snake_pos2[50][2];
    float appleY2 = 128, appleX2 = 48, size_snake2 = 1;

//-----------2 player version --------//
	while(1) {
        oamEnable(&oamSub);

		scanKeys();

		int keys = keysHeld();
        if (keysDown()&KEY_START) break;
		
		//if(keys & KEY_START) break;
//----------snek 1-------------------//
		if(keys)
		{
			if(keys & KEY_UP)
			{
                if(direction==1){
                    direction=1;
                }
                else{direction = 0;}
				
			}
			if(keys & KEY_LEFT)
			{
				if(direction==3){
                    direction=3;
                }
                else{direction = 2;}
			}
			if(keys & KEY_RIGHT)
			{
				if(direction==2){
                    direction=2;
                }
                else{direction = 3;}
			}
			if(keys & KEY_DOWN)
			{
               if(direction==0){
                    direction=0;
                }
                else{direction = 1;}
			}
        }

        switch(direction){
            case 0:
                yPos -= movement;
                break;
            case 1:
                yPos += movement;
                break;
            case 2:
                xPos -= movement;
                break;
            case 3:
                xPos += movement;
                break;
            default:
                NULL;
        }

        if(xPos>256-size){
            xPos=0;
        }
        else if(xPos<0){
            xPos=256-size;
        }
        if(yPos>192-size){
            yPos=0;
        }
        else if(yPos<0){
            yPos=192-size;
        }

//-----------snek 2-----------//
        if(keys)
		{
			if(keys & KEY_X)
			{
                if(direction2==1){
                    direction2=1;
                }
                else{direction2 = 0;}
				
			}
			if(keys & KEY_Y)
			{
				if(direction2==3){
                    direction2=3;
                }
                else{direction2 = 2;}
			}
			if(keys & KEY_A)
			{
				if(direction2==2){
                    direction2=2;
                }
                else{direction2 = 3;}
			}
			if(keys & KEY_B)
			{
               if(direction2==0){
                    direction2=0;
                }
                else{direction2 = 1;}
			}
        }

        switch(direction2){
            case 0:
                yPos2 -= movement;
                break;
            case 1:
                yPos2 += movement;
                break;
            case 2:
                xPos2 -= movement;
                break;
            case 3:
                xPos2 += movement;
                break;
            default:
                NULL;
        }

        if(xPos2>256-size){
            xPos2=0;
        }
        else if(xPos2<0){
            xPos2=256-size;
        }
        if(yPos2>192-size){
            yPos2=0;
        }
        else if(yPos2<0){
            yPos2=192-size;
        }
        // SNAKE
        snake_pos[0][0] = xPos;
        snake_pos[0][1] = yPos;
        snake_pos2[0][0] = xPos2;
        snake_pos2[0][1] = yPos2;
        if(count==10){
            movement = 16;
            count = 0;
            for(int i=0; i<2;i++){
                for(int j=size_snake2; j>0;j--){
                    snake_pos2[j][i]=snake_pos2[j-1][i];
                }
            }
            for(int j=4; j<size_snake2;j++){
                if(snake_pos2[j][0]==snake_pos2[0][0]&&snake_pos2[j][1]==snake_pos2[0][1]){
                    for(int i=0; i<2;i++){
                        for(int j=size_snake2; j>0;j--){
                            snake_pos2[j][i]=0;
                        }
                    }
                    for(int i=0;i<size_snake2;i++){
                        oamSet(&oamMain, //main graphics engine context
                            60+i,           //oam index (0 to 127)  
                            snake_pos2[i][0], snake_pos2[i][1],   //x and y pixle location of the sprite
                            0,                    //priority, lower renders last (on top)
                            0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
                            SpriteSize_16x16,     
                            SpriteColorFormat_256Color, 
                            gfxsub,                  //pointer to the loaded graphics
                            -1,                  //sprite rotation data  
                            false,               //double the size when rotating?
                            true,			//hide the sprite?
                            false, false, //vflip, hflip
                            false	//apply mosaic
                            );  
                    }
                    size_snake2 = 1;
                    direction2 = 4;
                }
            }
            for(int i=0; i<2;i++){
                for(int j=size_snake; j>0;j--){
                    snake_pos[j][i]=snake_pos[j-1][i];
                }
            }
            for(int j=4; j<size_snake;j++){
                if(snake_pos[j][0]==snake_pos[0][0]&&snake_pos[j][1]==snake_pos[0][1]){
                    for(int i=0; i<2;i++){
                        for(int j=size_snake; j>0;j--){
                            snake_pos[j][i]=0;
                        }
                    }
                    for(int i=0;i<size_snake;i++){
                        oamSet(&oamMain, //main graphics engine context
                            i,           //oam index (0 to 127)  
                            snake_pos[i][0], snake_pos[i][1],   //x and y pixle location of the sprite
                            0,                    //priority, lower renders last (on top)
                            0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
                            SpriteSize_16x16,     
                            SpriteColorFormat_256Color, 
                            gfx,                  //pointer to the loaded graphics
                            -1,                  //sprite rotation data  
                            false,               //double the size when rotating?
                            true,			//hide the sprite?
                            false, false, //vflip, hflip
                            false	//apply mosaic
                            );  
                    }
                    size_snake = 1;
                    direction = 4;
                }
            }
            for(int j=2; j<size_snake;j++){
                if(snake_pos[j][0]==snake_pos2[0][0]&&snake_pos[j][1]==snake_pos2[0][1]){
                    for(int i=0; i<2;i++){
                        for(int j=size_snake2; j>0;j--){
                            snake_pos2[j][i]=0;
                        }
                    }
                    for(int i=0;i<size_snake2;i++){
                        oamSet(&oamMain, //main graphics engine context
                            60+i,           //oam index (0 to 127)  
                            snake_pos2[i][0], snake_pos2[i][1],   //x and y pixle location of the sprite
                            0,                    //priority, lower renders last (on top)
                            0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
                            SpriteSize_16x16,     
                            SpriteColorFormat_256Color, 
                            gfxsub,                  //pointer to the loaded graphics
                            -1,                  //sprite rotation data  
                            false,               //double the size when rotating?
                            true,			//hide the sprite?
                            false, false, //vflip, hflip
                            false	//apply mosaic
                            );  
                    }
                    size_snake2 = 1;
                    direction2 = 4;
                }
            }
            for(int j=2; j<size_snake;j++){
                if(snake_pos2[j][0]==snake_pos[0][0]&&snake_pos2[j][1]==snake_pos[0][1]){
                    for(int i=0; i<2;i++){
                        for(int j=size_snake; j>0;j--){
                            snake_pos[j][i]=0;
                        }
                    }
                    for(int i=0;i<size_snake;i++){
                        oamSet(&oamMain, //main graphics engine context
                            i,           //oam index (0 to 127)  
                            snake_pos[i][0], snake_pos[i][1],   //x and y pixle location of the sprite
                            0,                    //priority, lower renders last (on top)
                            0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
                            SpriteSize_16x16,     
                            SpriteColorFormat_256Color, 
                            gfx,                  //pointer to the loaded graphics
                            -1,                  //sprite rotation data  
                            false,               //double the size when rotating?
                            true,			//hide the sprite?
                            false, false, //vflip, hflip
                            false	//apply mosaic
                            );  
                    }
                    size_snake = 1;
                    direction = 4;
                }
            }
            if(snake_pos2[0][0]==snake_pos[0][0]&&snake_pos2[0][1]==snake_pos[0][1]){
                for(int i=0; i<2;i++){
                    for(int j=size_snake; j>0;j--){
                        snake_pos[j][i]=0;
                    }
                }
                for(int i=0; i<2;i++){
                    for(int j=size_snake2; j>0;j--){
                        snake_pos2[j][i]=0;
                    }
                }
                for(int i=0;i<size_snake;i++){
                    oamSet(&oamMain, //main graphics engine context
                        i,           //oam index (0 to 127)  
                        snake_pos[i][0], snake_pos[i][1],   //x and y pixle location of the sprite
                        0,                    //priority, lower renders last (on top)
                        0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
                        SpriteSize_16x16,     
                        SpriteColorFormat_256Color, 
                        gfx,                  //pointer to the loaded graphics
                        -1,                  //sprite rotation data  
                        false,               //double the size when rotating?
                        true,			//hide the sprite?
                        false, false, //vflip, hflip
                        false	//apply mosaic
                        );  
                }
                for(int i=0;i<size_snake2;i++){
                    oamSet(&oamMain, //main graphics engine context
                        60+i,           //oam index (0 to 127)  
                        snake_pos2[i][0], snake_pos2[i][1],   //x and y pixle location of the sprite
                        0,                    //priority, lower renders last (on top)
                        0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
                        SpriteSize_16x16,     
                        SpriteColorFormat_256Color, 
                        gfx,                  //pointer to the loaded graphics
                        -1,                  //sprite rotation data  
                        false,               //double the size when rotating?
                        true,			//hide the sprite?
                        false, false, //vflip, hflip
                        false	//apply mosaic
                        );  
                }
                size_snake = 1;
                direction = 4;
                size_snake2 = 1;
                direction2 = 4;
            }
        }
        else{movement=0;}

        for(int i=0;i<size_snake;i++){
            oamSet(&oamMain, //main graphics engine context
                i,           //oam index (0 to 127)  
                snake_pos[i][0], snake_pos[i][1],   //x and y pixle location of the sprite
                0,                    //priority, lower renders last (on top)
                0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
                SpriteSize_16x16,     
                SpriteColorFormat_256Color, 
                gfx,                  //pointer to the loaded graphics
                -1,                  //sprite rotation data  
                false,               //double the size when rotating?
                false,			//hide the sprite?
                false, false, //vflip, hflip
                false	//apply mosaic
                );  
        }

        // APPLE
        if(xPos==appleX && yPos==appleY){
            size_snake++;
            appleX = (rand() % 16+1)*16;
            appleY = (rand() % 12+1)*16;
            if(appleX==256){appleX-=16;}
            if(appleY==192){appleY-=16;}
        }
        if(xPos2==appleX && yPos2==appleY){
            size_snake2++;
            appleX = (rand() % 16+1)*16;
            appleY = (rand() % 12+1)*16;
            if(appleX==256){appleX-=16;}
            if(appleY==192){appleY-=16;}
        }
        for(int j=0; j<size_snake;j++){
            if(snake_pos[j][0]==appleX&&snake_pos[j][1]==appleY){
                appleX -=16;
                appleY -=16;
            }
        }
        for(int j=0; j<size_snake2;j++){
            if(snake_pos2[j][0]==appleX&&snake_pos2[j][1]==appleY){
                appleX -=16;
                appleY -=16;
            }
        }	

        oamSet(&oamMain, //main graphics engine context
            127,           //oam index (0 to 127)  
            appleX, appleY,   //x and y pixle location of the sprite
            0,                    //priority, lower renders last (on top)
            1,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
            SpriteSize_16x16,     
            SpriteColorFormat_256Color, 
            gfx2,                  //pointer to the loaded graphics
            -1,                  //sprite rotation data  
            false,               //double the size when rotating?
            false,			//hide the sprite?
            false, false, //vflip, hflip
            false	//apply mosaic
            );   		

        for(int i=0;i<size_snake2;i++){
            oamSet(&oamMain, //main graphics engine context
                60+i,           //oam index (0 to 127)  
                snake_pos2[i][0], snake_pos2[i][1],   //x and y pixle location of the sprite
                0,                    //priority, lower renders last (on top)
                0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
                SpriteSize_16x16,     
                SpriteColorFormat_256Color, 
                gfxsub,                  //pointer to the loaded graphics
                -1,                  //sprite rotation data  
                false,               //double the size when rotating?
                false,			//hide the sprite?
                false, false, //vflip, hflip
                false	//apply mosaic
                );  
        }	    
	
		count++;
        swiWaitForVBlank();

		
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}
	videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);

	consoleDemoInit();

	iprintf("\n\n\tSNEK 2\n");
	iprintf("\tPress A for Single Player\n");
	iprintf("\tPress B for 2 Player");

	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

	dmaCopy(drunkenlogoBitmap, bgGetGfxPtr(bg3), 256*256);
	dmaCopy(drunkenlogoPal, BG_PALETTE, 256*2);
}
