#include "lepteto.h"

void clear()
{
        int i, j;
        for (i=0; i<=(Tablaa.MapX)+1; i++)
            for (j=0; j<=(Tablaa.MapY)+1; j++)
                Tablaa.T[i][j]=0;
}

void leptet(bool periodical)
{
    int MapX=Tablaa.MapX; //rövidítésképpen
    int MapY=Tablaa.MapY;
    char **Tloc=Tablaa.T;

    char Tc[Tablaa.MapX+2][Tablaa.MapY+2]; //A másolat tábla

    int i, j;
    if (periodical)
    {
        for (i=1; i<=MapY; i++) {
            Tloc[0][i]=Tloc[(MapX)][i];
            Tloc[(MapX+1)][i]=Tloc[1][i];
        }
        for (i=1; i<=MapX; i++) {
            Tloc[i][0]=Tloc[i][MapY];
            Tloc[i][MapY+1]=Tloc[i][1];
        }
        Tloc[0][0]=Tloc[(MapX)][MapY];
        Tloc[(MapX+1)][MapY+1]=Tloc[1][1];

        Tloc[(MapX+1)][0]=Tloc[1][MapY];
        Tloc[0][(MapY+1)]=Tloc[MapX][1];
    }
    else
    {
        for (i=1; i<=MapY; i++) {
            Tloc[0][i]=0;
            Tloc[(MapX+1)][i]=0;
        }
        for (i=1; i<=MapX; i++) {
            Tloc[i][0]=0;
            Tloc[i][MapY+1]=0;
        }
    }


    for (i=1; i<=MapX; i++)
    {
        int isz;
        int sum[10]={0,0,0,0,0,0,0,0,0,0};
        for (isz=i-1; isz<=i+1; isz++)
        {
                sum[Tloc[isz][0]]++;
                sum[Tloc[isz][1]]++;
        }


        for (j=1; j<=MapY; j++)
        {

            if (j>1)
                for (isz=i-1; isz<=i+1; isz++)
                    sum[Tloc[isz][j-2]]--;

            for (isz=i-1; isz<=i+1; isz++)
                sum[Tloc[isz][j+1]]++;


          /*  switch (Tloc[i][j])
            {
            case 0:
                if (2*sum[1]+sum[0]==12 || 2*sum[1]+sum[0]==12) Tc[i][j]=1;
                else if (2*sum[2]+sum[0]==12 || 2*sum[2]+sum[0]==12) Tc[i][j]=2;
                else Tc[i][j]=0;
                break;

            case 1 :
                if (2*sum[1]+sum[0]==12 || 2*sum[1]+sum[0]==13) Tc[i][j]=1;
                else if (2*sum[2]+sum[0]==14 || 2*sum[2]+sum[0]==15) Tc[i][j]=2;
                else Tc[i][j]=0;
                break;

            case 2 :
                if (2*sum[1]+sum[0]==14 || 2*sum[1]+sum[0]==15) Tc[i][j]=1;
                else if (2*sum[2]+sum[0]==12 || 2*sum[2]+sum[0]==13) Tc[i][j]=2;
                else Tc[i][j]=0;
                break;

            default: Tc[i][j]=0;
            }*/

            int mezo=Tloc[i][j];
            Tc[i][j]=0;
            if(mezo==0)
            {
                int k=1;
                for (k=1; k<=7; k++)
                {
                    int relevant=2*sum[k]+sum[0]-9;
                    if (cell[k].bornmin<=relevant && relevant<=cell[k].bornmax) Tc[i][j]=k;
                }
            }
            else{
                int k=1;
                for (k=1; k<=7; k++)
                {
                    int relevant=2*sum[k]+sum[0]-9;
                    if (mezo==k)
                    {
                         if (cell[k].survivemin<=relevant && relevant<=cell[k].survivemax) Tc[i][j]=k;
                    }
                    else
                    {
                        if (cell[k].occupymin<=relevant && relevant<=cell[k].occupymax) Tc[i][j]=k;
                    }
                }
            }

        }
    }

    //a régi tábla felülírása az újjal
    for (i=1; i<=MapX; i++)
    {
        for (j=1; j<=MapY; j++)
        {
            Tloc[i][j]=Tc[i][j];
        }
    }

}
