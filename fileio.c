#include "fileio.h"
 #define min(a,b) a < b ? a : b;

//#define TEXT_SAVE
//#define TEXT_LOAD

void save()
{

#ifdef TEXT_SAVE
    FILE *fp; // fájl mutató (file pointer/handle)

    fp = fopen("save.txt", "wt"); // megnyitás
    if (fp == NULL) {
       perror("save.txt megnyitása");
       return;    // nem folytathatjuk!
    }

    fprintf(fp, "%d %d\n",Tablaa.MapX,Tablaa.MapY);


    int i, j;
    for (i=1; i<=Tablaa.MapX; i++)
    {
        for (j=1; j<=Tablaa.MapY; j++)
        {
            fprintf(fp, "%d ", Tablaa.T[i][j]);
        }
        fprintf(fp, "\n");
    }
     fclose(fp);

#else

    FILE *fp; // fájl mutató (file pointer/handle)

    fp = fopen("save.hex", "wb"); // megnyitás
    if (fp == NULL) {
       perror("save.txt megnyitása");
       return;    // nem folytathatjuk!
    }
    fwrite(&Tablaa.MapX,sizeof(int),1,fp);
    fwrite(&Tablaa.MapY,sizeof(int),1,fp);
    int i, j;
    for (i=1; i<=Tablaa.MapX; i++)
    {
        for (j=1; j<=Tablaa.MapY; )
        {
            char adat[8];
            int k;
            for (k=0; k<8; k++)
            {
                adat[k]= j<=Tablaa.MapY ? Tablaa.T[i][j] : 0;
                j++;
            }
            char c[3];
            c[0]=(((adat[0]<<5)|(adat[1]<<2)|(adat[2]>>1))&0xFF);
            c[1]=((adat[2]<<7)|(adat[3]<<4)|(adat[4]<<1)|(adat[5]>>2))&0xFF;
            c[2]=((adat[5]<<6)|(adat[6]<<3)|(adat[7]))&0xFF;

            fwrite(&c[0],sizeof(char),1,fp);
            fwrite(&c[1],sizeof(char),1,fp);
            fwrite(&c[2],sizeof(char),1,fp);
        }
       // fwrite(&0,sizeof(char),1,fp);
    }

    fclose(fp);
#endif

}



void load()
{

    #ifdef TEXT_LOAD
    FILE *fp; // fájl mutató (file pointer/handle)

    fp = fopen("save.txt", "rt"); // megnyitás
    if (fp == NULL) {
       perror("save.txt megnyitása");
       return;    // nem folytathatjuk!
    }

    fscanf(fp, "%d %d\n",&(Tablaa.MapX),&(Tablaa.MapY));

    if (Config.ScreenWidth<Tablaa.MapX*(Config.tile) || (Config.ScreenHeight<Tablaa.MapY)*(Config.tile))
    {
        Config.tile=min(Config.ScreenWidth/Tablaa.MapX,Config.ScreenHeight/Tablaa.MapY);
        Config.tileSize=Config.tile-1;
    }


    Tablaa.MapX+=2;
    Tablaa.MapY+=2;
    if (Tablaa.T!=NULL) free(Tablaa.T[0]);
    free(Tablaa.T);
    Tablaa.T=NULL;
    Tablaa.T = (int**) malloc(Tablaa.MapX * sizeof(int*));
    Tablaa.T[0] = (int*) calloc(Tablaa.MapY*Tablaa.MapX , sizeof(int));
    int y;
    for (y = 1; y < Tablaa.MapX; ++y)
       Tablaa.T[y] = Tablaa.T[0] + y*Tablaa.MapY;
    Tablaa.MapX-=2;
    Tablaa.MapY-=2;



    int i, j;
    for (i=1; i<=Tablaa.MapX; i++)
    {
        for (j=1; j<=Tablaa.MapY; j++)
        {

            fscanf(fp, "%d", &(Tablaa.T[i][j]));
        }

    }

    fclose(fp);

    #else
    FILE *fp; // fájl mutató (file pointer/handle)

    fp = fopen("save.hex", "rb"); // megnyitás
    if (fp == NULL) {
       perror("save.txt megnyitása");
       return;    // nem folytathatjuk!
    }


    fread(&(Tablaa.MapX),sizeof(int),1,fp);
    fread(&(Tablaa.MapY),sizeof(int),1,fp);

    if (Config.ScreenWidth<Tablaa.MapX*(Config.tile) || (Config.ScreenHeight<Tablaa.MapY)*(Config.tile))
    {
        Config.tile=min(Config.ScreenWidth/Tablaa.MapX,Config.ScreenHeight/Tablaa.MapY);
        Config.tileSize=Config.tile-1;
    }


    Tablaa.MapX+=2;
    Tablaa.MapY+=2;
    if (Tablaa.T!=NULL) free(Tablaa.T[0]);
    free(Tablaa.T);
    Tablaa.T=NULL;
    Tablaa.T = (char**) malloc(Tablaa.MapX * sizeof(char*));
    Tablaa.T[0] = (char*) calloc(Tablaa.MapY*Tablaa.MapX , sizeof(char));
    int y;
    for (y = 1; y < Tablaa.MapX; ++y)
       Tablaa.T[y] = Tablaa.T[0] + y*Tablaa.MapY;
    Tablaa.MapX-=2;
    Tablaa.MapY-=2;



    int i, j, l;
    for (i=1; i<=Tablaa.MapX; i++)
    {
        j=1;
        for (l=1; l<=(Tablaa.MapY-1)/8+1; l++)
        {
            char betu1, betu2, betu3;
            fread(&betu1,sizeof(char),1,fp);
            fread(&betu2,sizeof(char),1,fp);
            fread(&betu3,sizeof(char),1,fp);

            if (j<=Tablaa.MapY)
            {
                Tablaa.T[i][j++]=(betu1>>5) & 0x7;
            }
            if (j<=Tablaa.MapY)
            {
                Tablaa.T[i][j++]=(betu1>>2) & 0x7;
            }
            if (j<=Tablaa.MapY)
            {
                Tablaa.T[i][j++]=((betu1<<1)| ((betu2>>7)& 0x01)) & 0x07;
            }
            if (j<=Tablaa.MapY)
            {
                Tablaa.T[i][j++]=(betu2>>4) & 0x7;
            }
            if (j<=Tablaa.MapY)
            {
                Tablaa.T[i][j++]=(betu2>>1) & 0x7;
            }
            if (j<=Tablaa.MapY)
            {
                char adat=((betu2<<2) | ((betu3>>6) & 0x03)) & 0x7;
                Tablaa.T[i][j++]=adat;
            }
            if (j<=Tablaa.MapY)
            {
                Tablaa.T[i][j++]=(betu3>>3) & 0x7;
            }
            if (j<=Tablaa.MapY)
            {
                Tablaa.T[i][j++]=(betu3) & 0x7;
            }

        }
    }

    fclose(fp);
#endif

}

void load_config()
{
        //alapértékek arra az esetre, ha nem volna konfigurációs fájl.
        Config.ScreenWidth=903;
        Config.ScreenHeight=647;
        Config.tile=20;
        Config.tileSize=19;
        Config.tileBorder=1;
        Config.MenuWidth=120;
        Config.VisibleBorders=true;
        Config.Toroidal=true;
        Config.BgColor=0x77AACCFF;

        szinkod[0]=0x777777FF;
        szinkod[1]=0x00AA22FF;
        szinkod[2]=0xAA22AAFF;
        szinkod[3]=0xfa56b5FF;
        szinkod[4]=0x9cca27FF;
        szinkod[5]=0xf08605FF;
        szinkod[6]=0xf2ca77FF;
        szinkod[7]=0x4f298fFF;


        int i;
        for (i=1; i<=7; i++)
        {
            cell[i].bornmin=3;
            cell[i].bornmax=3;
            cell[i].survivemin=3;
            cell[i].survivemax=4;
            cell[i].occupymin=5;
            cell[i].occupymax=6;
        }

        FILE *fp = fopen ("Config.txt", "r");
        if (fp==NULL)
        {

        }
        else
        {
                char line[100];
                int attetszoseg;
                i=0;
                while(fgets(line, sizeof(line), fp) != NULL)
                {
                        char *ertek;
                        ertek = strstr(line,"=");
                        ertek = ertek + strlen("=");
                        int r, g, b;

                        if (i == 0){
                                sscanf(ertek,"%d",&Config.ScreenWidth);
                        } else if (i == 1){
                                sscanf(ertek,"%d",&Config.ScreenHeight);
                        } else if (i == 2){
                                sscanf(ertek,"%d",&Config.tile);
                                Config.tileSize=Config.tile-1;
                                Config.tileBorder=1;
                        } else if (i == 3){
                                sscanf(ertek,"%d",&Config.MenuWidth);
                        } else if (i == 4){
                                sscanf(ertek,"%d",&Config.VisibleBorders);
                        } else if (i == 5){
                                sscanf(ertek,"%d",&Config.Toroidal);
                        } else if (i == 6){
                                sscanf(ertek,"%d, %d, %d",&r,&g,&b);
                                Config.BgColor=r<<24 | g<<16 | b<<8 | 0xFF;
                        } else if (i == 7){
                                sscanf(ertek,"%d",&attetszoseg);
                        } else if (i >= 8 && i<16){
                                sscanf(ertek,"%d, %d, %d",&r,&g,&b);
                                szinkod[i-8]=r<<24 | g<<16 | b<<8 | 0xFF;
                        } else if (i >= 16 && i<16+7){
                                sscanf(ertek,"%d, %d, %d, %d, %d, %d",&cell[i-15].bornmin,&cell[i-15].bornmax,&cell[i-15].survivemin,&cell[i-15].survivemax,&cell[i-15].occupymin,&cell[i-15].occupymax);

                        }

                        i++;
                }
                szinkod[0]&=0xFFFFFF00;
                szinkod[0]|=attetszoseg;
                fclose(fp);
        }
}
