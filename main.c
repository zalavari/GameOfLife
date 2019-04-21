#include "lepteto.h"
#include "fileio.h"
#include "rajzol.h"
#include "types.h"

int type;
bool anim;
bool vanPalya;
bool hold;

// ez a fuggveny hivodik meg az idozito altal, a folyamatos megjelenítéshez kell
Uint32 idozit(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   // ujabb varakozas
}

void tablatorol()
{
    Tablaa.MapX=Config.ScreenWidth/Config.tile;
    Tablaa.MapY=Config.ScreenHeight/Config.tile;
    if (Tablaa.T!=NULL) free(Tablaa.T[0]);
    free(Tablaa.T);
    Tablaa.T=NULL;
}

void tablaletrehoz()
{

    Tablaa.MapX+=2;
    Tablaa.MapY+=2;
    Tablaa.T = (char**) malloc(Tablaa.MapX * sizeof(char*));
    Tablaa.T[0] = (char*) calloc(Tablaa.MapY*Tablaa.MapX , sizeof(char));
    int y;
    for (y = 1; y < Tablaa.MapX; ++y)
       Tablaa.T[y] = Tablaa.T[0] + y*Tablaa.MapY;
    Tablaa.MapX-=2;
    Tablaa.MapY-=2;

    rajzol_tabla(Tablaa);
}

void reset()
{
    type=1;
    anim=false;
    vanPalya = false;
    hold=false;

    tablatorol();
    rajzol_menu(vanPalya, anim, type);
}


int main(int argc, char *argv[]) {
    SDL_Event event;
    SDL_TimerID id;

    load_config();


    /* SDL inicializálása és ablak megnyitása */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    screen = SDL_SetVideoMode(Config.ScreenWidth+Config.MenuWidth, Config.ScreenHeight, 0, SDL_ANYFORMAT);
    if (!screen) {
        fprintf(stderr, "Nem sikerult megnyitni az ablakot!\n");
        exit(1);
    }
    SDL_WM_SetCaption("Életjáték v1.0", "Eletjatek v1.0");

    reset();

    SDL_Flip(screen);

    /* varunk a kilepesre */
    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT) {
        switch (event.type)
        {
        case SDL_USEREVENT:
        {
            if (anim)   //feleslegesnek tűnik, de lehetséges, hogy van még
            {           //SDL_USEREVENT a feldolgozando események között, pedig már a táblát is felszabadítottuk
                leptet(Config.Toroidal);
                rajzol_egesz();
            }

            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            int mx, my;
            mx=event.button.x;
            my=event.button.y;
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                hold=true;
                //ha nincs még pálya
                if (!vanPalya)
                {
                    //ha mezőre kattintottunk, létrehozzuk a pályát
                    if (mx<Tablaa.MapX*Config.tile && my <Tablaa.MapY*Config.tile)
                    {
                        Tablaa.MapX=(mx/Config.tile+1);
                        Tablaa.MapY=my/Config.tile+1;

                        tablaletrehoz(&Tablaa);
                        vanPalya=true;
                        rajzol_menu(vanPalya, anim, type);
                    }
                    else if (mx>Config.ScreenWidth)
                    {

                        if (my <50)
                        ;//Next Step
                        else if (my <100)
                        ;//START

                        else if (my <150)
                        ;//SAVE
                        else if (my <200)
                        {//LOAD
                            vanPalya=true;
                            tablatorol();
                            load();
                            rajzol_tabla(Tablaa);
                            rajzol_menu(vanPalya, anim, type);
                            rajzol_egesz();
                        }
                        else if (my<250)
                        ;//CLEAR
                        else if (my<300)
                        ;//RESET
                        else if (my <350)
                        {//Continuous
                            Config.Toroidal=!Config.Toroidal;
                            rajzol_menu(vanPalya, anim, type);
                        }
                        else if (my <400)
                        {//Visible Borders
                            Config.VisibleBorders=!Config.VisibleBorders;
                            rajzol_menu(vanPalya, anim, type);
                        }

                    }
                }
                else if (vanPalya)
                {
                    //ha mezőre kattintottunk, rajzolunk egyet
                    if (mx<Tablaa.MapX*Config.tile && my <Tablaa.MapY*Config.tile)
                    {
                        Tablaa.T[(mx/Config.tile+1)][my/Config.tile+1]=type;
                        rajzol_egyet(mx/Config.tile,my/Config.tile,type);
                    }
                    else if (mx>Config.ScreenWidth)
                    {
                        if (my <50)
                        {//Next step
                            leptet(Config.Toroidal);
                            rajzol_egesz(Tablaa);
                        }
                        else if (my <100)
                        {//START-STOP
                            if (!anim)
                            {// idozito hozzaadasa: 40 ms; 1000 ms / 40 ms -> 25 fps
                                id = SDL_AddTimer(40, idozit, NULL);
                            }
                            else
                            {
                                SDL_RemoveTimer(id);
                            }
                            anim=!anim;
                            rajzol_menu(vanPalya, anim, type);
                        }
                        else if (my <150)
                        {//SAVE
                            save(&Tablaa);
                        }
                        else if (my <200)
                        {//LOAD
                            load();
                            rajzol_tabla(Tablaa);
                            rajzol_menu(vanPalya, anim, type);
                            rajzol_egesz();
                        }
                        else if (my <250)
                        {//CLEAR
                            clear();
                            rajzol_egesz();
                        }
                        else if (my <300)
                        {//RESET
                            rajzol_tabla(Tablaa);
                            reset();
                        }
                        else if (my <350)
                        {//Continuous
                            Config.Toroidal=!Config.Toroidal;
                            rajzol_menu(vanPalya, anim, type);
                        }
                        else if (my <400)
                        {//Visible Borders
                            Config.VisibleBorders=!Config.VisibleBorders;
                            rajzol_menu(vanPalya, anim, type);
                            rajzol_tabla(Tablaa);
                            rajzol_egesz();
                        }
                        else if (my <700)
                        {//Color select

                            type=((my-400)/50*2+1);
                            if (mx<Config.ScreenWidth+50) type-=1;

                            rajzol_menu(vanPalya, anim, type);
                        }
                    }
                }
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) {

            }
            else if (event.button.button==SDL_BUTTON_WHEELUP) {
                if (!vanPalya && Config.tile<100)
                {
                    Config.tile++;
                    Config.tileSize++;
                    tablatorol();
                    rajzol_urestabla(event.button.x,event.button.y);
                }
                else
                {
                    if ((Config.tile+1)*Tablaa.MapX<=Config.ScreenWidth && (Config.tile+1)*Tablaa.MapY<=Config.ScreenHeight)
                    {
                        Config.tile++;
                        Config.tileSize++;
                        rajzol_tabla(Tablaa);
                        rajzol_egesz();
                    }
                }
            }

            else if (event.button.button==SDL_BUTTON_WHEELDOWN) {
                if (!vanPalya && Config.tile>3)
                {
                    Config.tile--;
                    Config.tileSize--;
                    tablatorol();
                    rajzol_urestabla(event.button.x,event.button.y);
                }
                 else
                {
                    if (Config.tile>3)
                    {
                        Config.tile--;
                        Config.tileSize--;
                        rajzol_tabla(Tablaa);
                        rajzol_egesz();
                    }
                }
            }
            break;

            }
        case SDL_MOUSEBUTTONUP:
            hold=false;
            if (event.button.button == SDL_BUTTON_LEFT)
            {

            }
            break;

        case SDL_MOUSEMOTION:
            if (!vanPalya)
            {
                rajzol_urestabla(event.button.x,event.button.y);
            }
            else if (hold && vanPalya)
            {
                int mx, my;
                mx=event.button.x;
                my=event.button.y;
                if (mx<Tablaa.MapX*Config.tile && my <Tablaa.MapY*Config.tile)
                    {
                        Tablaa.T[(mx/Config.tile+1)][my/Config.tile+1]=type;
                        rajzol_egyet(mx/Config.tile,my/Config.tile,type);
                    }
            }
            break;

        case SDL_QUIT:

            break;
        }

        SDL_Flip(screen);
    }
    SDL_Quit();

    return 0;
}
