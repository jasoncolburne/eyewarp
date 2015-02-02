/**
 *
 */

#include "filter.h"
#include "plasma.h"

/***************** standalone engine for "plasma" ********************/

static int iparmx;      /* iparmx = parm.x * 16 */
static int shiftvalue;  /* shift based on #colors */
static int recur1=1;
static int pcolors;
static int recur_level = 0;
U16 max_plasma;

/* returns a random 16 bit value that is never 0 */
U16 rand16(void)
{
   U16 value;
   value = (U16)rand15();
   value <<= 1;
   value = (U16)(value + (rand15()&1));
   if(value < 1)
      value = 1;
   return(value);
}

void _fastcall putpot(int x, int y, U16 color)
{
   if(color < 1)
      color = 1;
   putcolor(x, y, color >> 8 ? color >> 8 : 1);  /* don't write 0 */
   /* we don't write this if dotmode==11 because the above putcolor
         was already a "writedisk" in that case */
   if (dotmode != 11)
      writedisk(x+sxoffs,y+syoffs,color >> 8);    /* upper 8 bits */
   writedisk(x+sxoffs,y+sydots+syoffs,color&255); /* lower 8 bits */
}

/* fixes border */
void _fastcall putpotborder(int x, int y, U16 color)
{
   if((x==0) || (y==0) || (x==xdots-1) || (y==ydots-1))
      color = (U16)outside;
   putpot(x,y,color);
}

/* fixes border */
void _fastcall putcolorborder(int x, int y, int color)
{
   if((x==0) || (y==0) || (x==xdots-1) || (y==ydots-1))
      color = outside;
   if(color < 1)
      color = 1;
   putcolor(x,y,color);
}

U16 _fastcall getpot(int x, int y)
{
   U16 color;

   color = (U16)readdisk(x+sxoffs,y+syoffs);
   color = (U16)((color << 8) + (U16) readdisk(x+sxoffs,y+sydots+syoffs));
   return(color);
}

static int plasma_check;                        /* to limit kbd checking */

static U16 _fastcall adjust(int xa,int ya,int x,int y,int xb,int yb)
{
   S32 pseudorandom;
   pseudorandom = ((S32)iparmx)*((rand15()-16383));
/*   pseudorandom = pseudorandom*(abs(xa-xb)+abs(ya-yb));*/
   pseudorandom = pseudorandom * recur1;
   pseudorandom = pseudorandom >> shiftvalue;
   pseudorandom = (((S32)getpix(xa,ya)+(S32)getpix(xb,yb)+1)>>1)+pseudorandom;
   if(max_plasma == 0)
   {
      if (pseudorandom >= pcolors)
         pseudorandom = pcolors-1;
   }
   else if (pseudorandom >= (S32)max_plasma)
      pseudorandom = max_plasma;
   if(pseudorandom < 1)
      pseudorandom = 1;
   plot(x,y,(U16)pseudorandom);
   return((U16)pseudorandom);
}


static int _fastcall new_subD (int x1,int y1,int x2,int y2, int recur)
{
   int x,y;
   int nx1;
   int nx;
   int ny1, ny;
   S32 i, v;

   struct sub {
      BYTE t; /* top of stack */
      int v[16]; /* subdivided value */
      BYTE r[16];  /* recursion level */
   };

   static struct sub subx, suby;

   /*
   recur1=1;
   for (i=1;i<=recur;i++)
      recur1 = recur1 * 2;
   recur1=320/recur1;
   */
   recur1 = (int)(320L >> recur);
   suby.t = 2;
   ny   = suby.v[0] = y2;
   ny1 = suby.v[2] = y1;
   suby.r[0] = suby.r[2] = 0;
   suby.r[1] = 1;
   y = suby.v[1] = (ny1 + ny) >> 1;

   while (suby.t >= 1)
   {
      if ((++plasma_check & 0x0f) == 1)
         if(keypressed())
         {
            plasma_check--;
            return(1);
         }
      while (suby.r[suby.t-1] < (BYTE)recur)
      {
         /*     1.  Create new entry at top of the stack  */
         /*     2.  Copy old top value to new top value.  */
         /*            This is largest y value.           */
         /*     3.  Smallest y is now old mid point       */
         /*     4.  Set new mid point recursion level     */
         /*     5.  New mid point value is average        */
         /*            of largest and smallest            */

         suby.t++;
         ny1  = suby.v[suby.t] = suby.v[suby.t-1];
         ny   = suby.v[suby.t-2];
         suby.r[suby.t] = suby.r[suby.t-1];
         y    = suby.v[suby.t-1]   = (ny1 + ny) >> 1;
         suby.r[suby.t-1]   = (BYTE)(max(suby.r[suby.t], suby.r[suby.t-2])+1);
      }
      subx.t = 2;
      nx  = subx.v[0] = x2;
      nx1 = subx.v[2] = x1;
      subx.r[0] = subx.r[2] = 0;
      subx.r[1] = 1;
      x = subx.v[1] = (nx1 + nx) >> 1;

      while (subx.t >= 1)
      {
         while (subx.r[subx.t-1] < (BYTE)recur)
         {
            subx.t++; /* move the top ofthe stack up 1 */
            nx1  = subx.v[subx.t] = subx.v[subx.t-1];
            nx   = subx.v[subx.t-2];
            subx.r[subx.t] = subx.r[subx.t-1];
            x    = subx.v[subx.t-1]   = (nx1 + nx) >> 1;
            subx.r[subx.t-1]   = (BYTE)(max(subx.r[subx.t],
                subx.r[subx.t-2])+1);
         }

         if ((i = getpix(nx, y)) == 0)
            i = adjust(nx,ny1,nx,y ,nx,ny);
         v = i;
         if ((i = getpix(x, ny)) == 0)
            i = adjust(nx1,ny,x ,ny,nx,ny);
         v += i;
         if(getpix(x,y) == 0)
         {
            if ((i = getpix(x, ny1)) == 0)
               i = adjust(nx1,ny1,x ,ny1,nx,ny1);
            v += i;
            if ((i = getpix(nx1, y)) == 0)
               i = adjust(nx1,ny1,nx1,y ,nx1,ny);
            v += i;
            plot(x,y,(U16)((v + 2) >> 2));
         }

         if (subx.r[subx.t-1] == (BYTE)recur) subx.t = (BYTE)(subx.t - 2);
      }

      if (suby.r[suby.t-1] == (BYTE)recur) suby.t = (BYTE)(suby.t - 2);
   }
   return(0);
}

static void _fastcall subDivide(int x1,int y1,int x2,int y2)
{
   int x,y;
   S32 v,i;
   if ((++plasma_check & 0x7f) == 1)
      if(keypressed())
      {
         plasma_check--;
         return;
      }
   if(x2-x1<2 && y2-y1<2)
      return;
   recur_level++;
   recur1 = (int)(320L >> recur_level);

   x = (x1+x2)>>1;
   y = (y1+y2)>>1;
   if((v=getpix(x,y1)) == 0)
      v=adjust(x1,y1,x ,y1,x2,y1);
   i=v;
   if((v=getpix(x2,y)) == 0)
      v=adjust(x2,y1,x2,y ,x2,y2);
   i+=v;
   if((v=getpix(x,y2)) == 0)
      v=adjust(x1,y2,x ,y2,x2,y2);
   i+=v;
   if((v=getpix(x1,y)) == 0)
      v=adjust(x1,y1,x1,y ,x1,y2);
   i+=v;

   if(getpix(x,y) == 0)
      plot(x,y,(U16)((i+2)>>2));

   subDivide(x1,y1,x ,y);
   subDivide(x ,y1,x2,y);
   subDivide(x ,y ,x2,y2);
   subDivide(x1,y ,x ,y2);
   recur_level--;
}


int plasma()
{
   int i,k, n;
   U16 rnd[4];
   int OldPotFlag, OldPot16bit;

   OldPotFlag=OldPot16bit=plasma_check = 0;

   if(colors < 4) {
      static FCODE plasmamsg[]={
         "\
Plasma Clouds can currently only be run in a 4-or-more-color video\n\
mode (and color-cycled only on VGA adapters [or EGA adapters in their\n\
640x350x16 mode])."      };
      stopmsg(0,plasmamsg);
      return(-1);
   }
   iparmx = (int)(param[0] * 8);
   if (parm.x <= 0.0) iparmx = 16;
   if (parm.x >= 100) iparmx = 800;

   if ((!rflag) && param[2] == 1)
      --rseed;
   if (param[2] != 0 && param[2] != 1)
      rseed = (int)param[2];
   max_plasma = (U16)param[3];  /* max_plasma is used as a flag for potential */

   if(max_plasma != 0)
   {
      if (pot_startdisk() >= 0)
      {
         /* max_plasma = (U16)(1L << 16) -1; */
         max_plasma = 0xFFFF;
         if(outside >= 0)
            plot    = (PLOT)putpotborder;
         else
            plot    = (PLOT)putpot;
         getpix =  getpot;
         OldPotFlag = potflag;
         OldPot16bit = pot16bit;
      }
      else
      {
         max_plasma = 0;        /* can't do potential (startdisk failed) */
         param[3]   = 0;
         if(outside >= 0)
            plot    = putcolorborder;
         else
            plot    = putcolor;
         getpix  = (U16(_fastcall *)(int,int))getcolor;
      }
   }
   else
   {
      if(outside >= 0)
        plot    = putcolorborder;
       else
        plot    = putcolor;
      getpix  = (U16(_fastcall *)(int,int))getcolor;
   }
   srand(rseed);
   if (!rflag) ++rseed;

   if (colors == 256)                   /* set the (256-color) palette */
      set_Plasma_palette();             /* skip this if < 256 colors */

   if (colors > 16)
      shiftvalue = 18;
   else
   {
      if (colors > 4)
         shiftvalue = 22;
      else
      {
         if (colors > 2)
            shiftvalue = 24;
         else
            shiftvalue = 25;
      }
   }
   if(max_plasma != 0)
      shiftvalue = 10;

   if(max_plasma == 0)
   {
      pcolors = min(colors, max_colors);
      for(n = 0; n < 4; n++)
         rnd[n] = (U16)(1+(((rand15()/pcolors)*(pcolors-1))>>(shiftvalue-11)));
   }
   else
      for(n = 0; n < 4; n++)
         rnd[n] = rand16();
   if(debugflag==3600)
      for(n = 0; n < 4; n++)
         rnd[n] = 1;

   plot(      0,      0,  rnd[0]);
   plot(xdots-1,      0,  rnd[1]);
   plot(xdots-1,ydots-1,  rnd[2]);
   plot(      0,ydots-1,  rnd[3]);

   recur_level = 0;
   if (param[1] == 0)
      subDivide(0,0,xdots-1,ydots-1);
   else
   {
      recur1 = i = k = 1;
      while(new_subD(0,0,xdots-1,ydots-1,i)==0)
      {
         k = k * 2;
         if (k  >(int)max(xdots-1,ydots-1))
            break;
         if (keypressed())
         {
            n = 1;
            goto done;
         }
         i++;
      }
   }
   if (! keypressed())
      n = 0;
   else
      n = 1;
   done:
   if(max_plasma != 0)
   {
      potflag = OldPotFlag;
      pot16bit = OldPot16bit;
   }
   plot    = putcolor;
   getpix  = (U16(_fastcall *)(int,int))getcolor;
   return(n);
}

#define dac ((Palettetype *)dacbox)
  
/* for evenly divisible palette we can use 3, 5 or 17 */
#define PLASMACOLORS 3
#define PLASMACOLORRANGE (255/PLASMACOLORS)

static void set_Plasma_palette()
{
  static Palettetype Colors[PLASMACOLORS] = {
    { 34,  4, 33 }, /* purple */
    { 25, 35,  9 }, /* green  */
    {  3, 42, 36 }, /* aqua   */
    /*
    { 38, 17, 21 },
    { 27, 31,  3 },
    */
 };

  int i, j;

  if (mapdacbox) return;               /* map= specified */

  dac[0].red   = 0;
  dac[0].green = 0;
  dac[0].blue  = 0;

  for (j = 0; j < PLASMACOLORS; j++)
    {
      int a = (PLASMACOLORS * 2 - 2 - j) % PLASMACOLORS;
      int b = (PLASMACOLORS * 2 - 1 - j) % PLASMACOLORS;

      for(i = 1; i <= PLASMACOLORRANGE; i++)
	{
	  int index = i + (j * PLASMACOLORRANGE);	  

	  int k = PLASMACOLORRANGE - i;

	  dac[index].red   = ((i * Colors[a].red   + k * Colors[b].red)   / (PLASMACOLORRANGE - 1));
	  dac[index].green = ((i * Colors[a].green + k * Colors[b].green) / (PLASMACOLORRANGE - 1));
	  dac[index].blue  = ((i * Colors[a].blue  + k * Colors[b].blue)  / (PLASMACOLORRANGE - 1));
	}
    }
  
  SetTgaColors();      /* TARGA 3 June 89  j mclain */
  spindac(0,1);
}

static void set_Plasma_palette_old()
{
   static Palettetype Red    = { 63, 0, 0 };
   static Palettetype Green  = { 0, 63, 40 };
   static Palettetype Blue   = { 0,  0,63 };
   int i;

   if (mapdacbox) return;               /* map= specified */

   

   dac[0].red  = 0 ;
   dac[0].green= 0 ;
   dac[0].blue = 0 ;

   

   for(i=1;i<=85;i++)
   {
#ifdef __SVR4
1 2
0 1
2 0

      dac[i].red       = (BYTE)((i*(int)Green.red   + (86-i)*(int)Blue.red)/85);
      dac[i].green     = (BYTE)((i*(int)Green.green + (86-i)*(int)Blue.green)/85);
      dac[i].blue      = (BYTE)((i*(int)Green.blue  + (86-i)*(int)Blue.blue)/85);

      dac[i+85].red    = (BYTE)((i*(int)Red.red   + (86-i)*(int)Green.red)/85);
      dac[i+85].green  = (BYTE)((i*(int)Red.green + (86-i)*(int)Green.green)/85);
      dac[i+85].blue   = (BYTE)((i*(int)Red.blue  + (86-i)*(int)Green.blue)/85);

      dac[i+170].red   = (BYTE)((i*(int)Blue.red   + (86-i)*(int)Red.red)/85);
      dac[i+170].green = (BYTE)((i*(int)Blue.green + (86-i)*(int)Red.green)/85);
      dac[i+170].blue  = (BYTE)((i*(int)Blue.blue  + (86-i)*(int)Red.blue)/85);
#else
      dac[i].red       = (BYTE)((i*Green.red   + (86-i)*Blue.red)/85);
      dac[i].green     = (BYTE)((i*Green.green + (86-i)*Blue.green)/85);  
      dac[i].blue      = (BYTE)((i*Green.blue  + (86-i)*Blue.blue)/85);
 
      dac[i+85].red    = (BYTE)((i*Red.red   + (86-i)*Green.red)/85);
      dac[i+85].green  = (BYTE)((i*Red.green + (86-i)*Green.green)/85);   
      dac[i+85].blue   = (BYTE)((i*Red.blue  + (86-i)*Green.blue)/85); 
      dac[i+170].red   = (BYTE)((i*Blue.red   + (86-i)*Red.red)/85);
      dac[i+170].green = (BYTE)((i*Blue.green + (86-i)*Red.green)/85);
      dac[i+170].blue  = (BYTE)((i*Blue.blue  + (86-i)*Red.blue)/85);
#endif
   }
   SetTgaColors();      /* TARGA 3 June 89  j mclain */
   spindac(0,1);
}

