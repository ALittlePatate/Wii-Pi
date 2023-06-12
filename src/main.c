#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

static u32 *xfb;
static GXRModeObj *rmode;


void Initialise() {
  
	VIDEO_Init();
	WPAD_Init();
 
	rmode = VIDEO_GetPreferredMode(NULL);

	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
 
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
}

void WaitForInput() {
    while (1) {
        WPAD_ScanPads();

        u16 buttonsDown = WPAD_ButtonsDown(0);

        if (buttonsDown & WPAD_BUTTON_A) {
            break;
        }
    }
}

void spigot() { //https://craftofcoding.wordpress.com/tag/spigot-algorithm/
   int i, j, k, q, x;
   int len, nines=0, predigit=0;
   int N=20000; //you can actually go way up but it makes the calculation slow
 
   len = (10*N/3)+1;
   int* a = malloc(len * sizeof(int));
   if (a == NULL) {
       printf("Error allocating memory.\n");
       return;
   }
 
   // Initialize A to (2,2,2,2,2,...,2)
   for (i=0; i<len; i=i+1)
      a[i] = 2;
 
   // Repeat n times
   for (j=1; j<=N; j=j+1) {
      q = 0;
      for (i=len; i>0; i=i-1) {
         x = 10 * a[i-1] + q*i;
         a[i-1] = x % (2*i-1);
         q = x / (2*i-1);
      }
      a[0] = q % 10;
      q = q / 10;
      if (q == 9)
         nines = nines + 1;
      else if (q == 10) {
         printf("%d", predigit+1);
         for (k=0; k<nines; k=k+1)
            printf("%d",0);
         predigit = 0;
         nines = 0;
      }
      else {
         printf("%d", predigit);
         predigit = q;
         if (nines != 0) {
            for (k=0; k<nines; k=k+1)
               printf("%d",9);
            nines = 0;
         }
     }
  }
  printf("%d\n", predigit);

  free(a);
}

int main() {
 
	Initialise();
 
    spigot();
    WaitForInput();
	return 0;
}
