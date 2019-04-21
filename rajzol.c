#include "rajzol.h"

static int szurkearnyalatos(int szin)
{
    int r, g, b;
    r=szin>>24;
    r&=0x000000FF;
    g=szin>>16;
    g&=0x000000FF;
    b=szin>>8;
    b&=0x000000FF;
    int sz=(r+g+b)/3;
    return (sz<<24)+(sz<<16)+(sz<<8)+0xFF;
}

void rajzol_menu(bool vanPalya,  bool anim, int type)
{
    int szin = vanPalya ? 0xFFFFFFFF : 0xAAAAAAFF;
    boxColor(screen, Config.ScreenWidth, 0, Config.ScreenWidth+Config.MenuWidth, Config.ScreenHeight, 0x000000FF);

    lineColor(screen, Config.ScreenWidth, 0, Config.ScreenWidth, Config.ScreenHeight, 0xFFFFFFFF);
    stringColor(screen, Config.ScreenWidth+10, 20,"Next Step", szin);
    lineColor(screen, Config.ScreenWidth, 50, Config.ScreenWidth+Config.MenuWidth, 50, 0xFFFFFFFF);
    stringColor(screen, Config.ScreenWidth+10, 70,anim?"STOP":"START", szin);
    lineColor(screen, Config.ScreenWidth, 100, Config.ScreenWidth+Config.MenuWidth, 100, 0xFFFFFFFF);

    stringColor(screen, Config.ScreenWidth+10, 120,"SAVE", szin);
    lineColor(screen, Config.ScreenWidth, 150, Config.ScreenWidth+Config.MenuWidth, 150, 0xFFFFFFFF);
    stringColor(screen, Config.ScreenWidth+10, 170,"LOAD", 0xFFFFFFFF);
    lineColor(screen, Config.ScreenWidth, 200, Config.ScreenWidth+Config.MenuWidth, 200, 0xFFFFFFFF);
    stringColor(screen, Config.ScreenWidth+10, 220,"CLEAR", szin);
    lineColor(screen, Config.ScreenWidth, 250, Config.ScreenWidth+Config.MenuWidth, 250, 0xFFFFFFFF);
    stringColor(screen, Config.ScreenWidth+10, 270,"RESET", szin);
    lineColor(screen, Config.ScreenWidth, 300, Config.ScreenWidth+Config.MenuWidth, 300, 0xFFFFFFFF);

    stringColor(screen, Config.ScreenWidth+10, 320,Config.Toroidal?"Continuous":"Discontinuous", 0xFFFFFFFF);
    lineColor(screen, Config.ScreenWidth, 350, Config.ScreenWidth+Config.MenuWidth, 350, 0xFFFFFFFF);

   // stringColor(screen, Config.ScreenWidth+10, 370,Config.VisibleBorders? "Visible Borders":"Invisible Borders", 0xFFFFFFFF);
    stringColor(screen, Config.ScreenWidth+10, 365,Config.VisibleBorders? "Visible":"Invisible", 0xFFFFFFFF);
    stringColor(screen, Config.ScreenWidth+40, 380,"Borders", 0xFFFFFFFF);
    lineColor(screen, Config.ScreenWidth, 400, Config.ScreenWidth+Config.MenuWidth, 400, 0xFFFFFFFF);

    int i;
    for (i=0; i<=7; i++)
    {
        if (type==i) boxColor(screen, Config.ScreenWidth+10+(i%2)*50, 410+(i/2)*50, Config.ScreenWidth+40+(i%2)*50, 440+(i/2)*50,Config.BgColor);
        boxColor(screen, Config.ScreenWidth+15+(i%2)*50, 415+(i/2)*50, Config.ScreenWidth+35+(i%2)*50, 435+(i/2)*50, vanPalya? szinkod[i] : szurkearnyalatos(szinkod[i]));
    }
    rectangleColor(screen, Config.ScreenWidth+15, 415, Config.ScreenWidth+35, 435, 0xDDDDDDFF);
    lineColor(screen, Config.ScreenWidth+15, 415, Config.ScreenWidth+35, 435, 0xFFFFFFFF);
    lineColor(screen, Config.ScreenWidth+15, 435, Config.ScreenWidth+35, 415, 0xFFFFFFFF);

}
void rajzol_egyet(int x, int y, int type)
{
    int szin;
    if (type>=0 && type<=7) szin=szinkod[type];
    else szin=0x000000FF;
    boxColor(screen, x*(Config.tile)+1, y*(Config.tile)+1, x*(Config.tile)+Config.tileSize, y*(Config.tile)+Config.tileSize,  szin);
}
void rajzol_egesz()
{
    int i, j;
    for (i=1; i<=Tablaa.MapX; i++)
        for (j=1; j<=Tablaa.MapY; j++)
            rajzol_egyet( i-1,j-1,Tablaa.T[i][j]);

}
void rajzol_tabla(Tabla Tb)
{
   // boxColor(screen,0,0,Config.ScreenWidth,Config.ScreenHeight,0x777777FF);
    boxColor(screen, Tb.MapX*Config.tile,0,Config.ScreenWidth-1,Config.ScreenHeight,0x77AACCFF);
    boxColor(screen, 0,Tb.MapY*Config.tile,Config.ScreenWidth-1,Config.ScreenHeight,0x77AACCFF);
    boxColor(screen,0,0,Tb.MapX*Config.tile,Tb.MapY*Config.tile,szinkod[0]|0xFF);
    if (Config.VisibleBorders)
    {
        int i=0;
        for (i=0; i<=Tb.MapX; i++)    lineColor(screen, i*Config.tile, 0, i*Config.tile, Tb.MapY*Config.tile, 0xFFFFFF99);
        for (i=0; i<=Tb.MapY; i++)    lineColor(screen, 0, i*Config.tile, Tb.MapX*Config.tile, i*Config.tile, 0xFFFFFF99);
    }
}
void rajzol_urestabla(int mx, int my)
{
    if (mx<Tablaa.MapX*Config.tile && my <Tablaa.MapY*Config.tile)
    {
        Tabla Tablaa2;
        Tablaa2.MapX=(mx/Config.tile+1);
        Tablaa2.MapY=my/Config.tile+1;
        rajzol_tabla(Tablaa2);
    }
}
