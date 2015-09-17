/*Parallel Hierarchical One-Dimensional Search for motion estimation*/
/*Initial algorithm - Used for simulation and profiling             */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 144     /*Frame dimension for QCIF format*/
#define M 176     /*Frame dimension for QCIF format*/

#define p 7       /*Search space. Restricted in a [-p,p] region around the
                    original location of the block.*/

int Bx,By;      /*Block size*/

void read_sequence(int current[N][M], int previous[N][M])
{ 
	FILE *picture0, *picture1;
	int i, j;

	if((picture0=fopen("akiyo0.y","rb")) == NULL)
	{
		printf("Previous frame doesn't exist.\n");
		exit(-1);
	}

	if((picture1=fopen("akiyo1.y","rb")) == NULL) 
	{
		printf("Current frame doesn't exist.\n");
		exit(-1);
	}

  /*Input for the previous frame*/
  for(i=0; i<N; i++)
  {
    for(j=0; j<M; j++)
    {
      previous[i][j] = fgetc(picture0);
      current[i][j] = fgetc(picture1);  // 1st optimization: do current and previous in the same loop

    }
  }
/*
	//Input for the current frame
	for(i=0; i<N; i++)
  {
		for(j=0; j<M; j++)
    {
			current[i][j] = fgetc(picture1);
    }
  }*/

	fclose(picture0);
	fclose(picture1);
}


void phods_motion_estimation(int current[N][M], int previous[N][M],
    int vectors_x[N/Bx][M/By],int vectors_y[N/Bx][M/By])
{
  int x, y, i, k, l, p1, p2, q2, distx, disty, S, min1, min2, bestx, besty;
  int Byun;

  distx = 0;
  disty = 0;

  /*Initialize the vector motion matrices*/
  /*for(i=0; i<N/B; i++)
  {
    for(j=0; j<M/B; j++)
    {
      vectors_x[i][j] = 0;
      vectors_y[i][j] = 0;
    }
  }*/

  /*For all blocks in the current frame*/
  for(x=0; x<N/Bx; x++)
  {
    for(y=0; y<M/By; y++)
    {
      S = 4;

      //this is the 3rd optimization to do, but it is not so sure! Check it dr.Josef 
      vectors_x[x][y] = 0;
      vectors_y[x][y] = 0;

      while(S > 0)
      {
        min1 = 255*Bx*Bx;
        min2 = 255*By*By;

        /*For all candidate blocks in X&Y dimension*/  // 2nd optimization is to have both x&y dimension processing
        for(i=-S; i<S+1; i+=S)     
        {
          distx = 0;
          //loop unrolling
	  Byun = By - 1;
          /*For all pixels in the block*/
          for(k=0; k<Bx; k++)     
          {
            for(l=0; l<Byun/2; l++)
            {
              // x-dimension
              p1 = current[Bx*x + k][By*y + 2*l];
              int help1=vectors_x[x][y];
              int aux1 = Bx * x + help1 + i + k; 
              int aux2 = By * y + help1 + 2*l; 
              if ( aux1 < 0 || aux1 > (N-1) ||
                   aux2 < 0 || aux2 > (M-1))       
   /*           if((Bx*x + help1 + i + k) < 0 ||
                  (Bx*x + help1 + i + k) > (N-1) ||
                  (Bx*y + help1 + l) < 0 ||
                  (Bx*y + help1 + l) > (M-1))
     */ 
              {
                p2 = 0;
              } else {
//                p2 = previous[Bx*x+help1+i+k][By*y+help1+l];
                p2 = previous[aux1][aux2];
              }

              distx += abs(p1-p2);

              // y-dimension
              p1 = current[Bx*x + k][By*y + 2*l];  // this can be removed, but keep it temporarily for better understanding of the code
              int aux3 = Bx * x + help1 + k; 
              int aux4 = By * y + help1 + i + 2*l; 
              if ( aux3 < 0 || aux3 > (N-1) ||
                   aux4 < 0 || aux4 > (M-1)) 
/*              if((By*x + help1 + k) <0 ||
                  (By*x + help1 + k) > (N-1) ||
                  (By*y + help1 + i + l) < 0 ||
                  (By*y + help1 + i + l) > (M-1))
*/
              {
                q2 = 0;
              } else {
              //  q2 = previous[Bx*x+help1+k][By*y+help1+i+l];
                q2 = previous[aux3][aux4];
              }

              disty += abs(p1-q2);
               // x-dimension
              p1 = current[Bx*x + k][By*y + 2*l + 1];
              help1=vectors_x[x][y];
              aux1 = Bx * x + help1 + i + k; 
              aux2 = By * y + help1 + 2*l + 1; 
              if ( aux1 < 0 || aux1 > (N-1) ||
                   aux2 < 0 || aux2 > (M-1))       
   /*           if((Bx*x + help1 + i + k) < 0 ||
                  (Bx*x + help1 + i + k) > (N-1) ||
                  (Bx*y + help1 + l) < 0 ||
                  (Bx*y + help1 + l) > (M-1))
     */ 
              {
                p2 = 0;
              } else {
//                p2 = previous[Bx*x+help1+i+k][By*y+help1+l];
                p2 = previous[aux1][aux2];
              }

              distx += abs(p1-p2);

              // y-dimension
              p1 = current[Bx*x + k][By*y + 2*l + 1];  // this can be removed, but keep it temporarily for better understanding of the code
              aux3 = Bx * x + help1 + k; 
              aux4 = By * y + help1 + i + 2*l + 1; 
              if ( aux3 < 0 || aux3 > (N-1) ||
                   aux4 < 0 || aux4 > (M-1)) 
/*              if((By*x + help1 + k) <0 ||
                  (By*x + help1 + k) > (N-1) ||
                  (By*y + help1 + i + l) < 0 ||
                  (By*y + help1 + i + l) > (M-1))
*/
              {
                q2 = 0;
              } else {
              //  q2 = previous[Bx*x+help1+k][By*y+help1+i+l];
                q2 = previous[aux3][aux4];
              }

              disty += abs(p1-q2);
            }

            if (By % 2 == 1) {
              l = By - 1;
              // x-dimension
              p1 = current[Bx*x + k][By*y + l];
              int help1=vectors_x[x][y];
              int aux1 = Bx * x + help1 + i + k; 
              int aux2 = By * y + help1 + l; 
              if ( aux1 < 0 || aux1 > (N-1) ||
                   aux2 < 0 || aux2 > (M-1))       
   /*           if((Bx*x + help1 + i + k) < 0 ||
                  (Bx*x + help1 + i + k) > (N-1) ||
                  (Bx*y + help1 + l) < 0 ||
                  (Bx*y + help1 + l) > (M-1))
     */ 
              {
                p2 = 0;
              } else {
//                p2 = previous[Bx*x+help1+i+k][By*y+help1+l];
                p2 = previous[aux1][aux2];
              }

              distx += abs(p1-p2);

              // y-dimension
              p1 = current[Bx*x + k][By*y + l];  // this can be removed, but keep it temporarily for better understanding of the code
              int aux3 = Bx * x + help1 + k; 
              int aux4 = By * y + help1 + i + l; 
              if ( aux3 < 0 || aux3 > (N-1) ||
                   aux4 < 0 || aux4 > (M-1)) 
/*              if((By*x + help1 + k) <0 ||
                  (By*x + help1 + k) > (N-1) ||
                  (By*y + help1 + i + l) < 0 ||
                  (By*y + help1 + i + l) > (M-1))
*/
              {
                q2 = 0;
              } else {
              //  q2 = previous[Bx*x+help1+k][By*y+help1+i+l];
                q2 = previous[aux3][aux4];
              }

              disty += abs(p1-q2);
            }
          }

          // x-dimension
          if(distx < min1)
          {
            min1 = distx;
            bestx = i;
          }
          // y-dimension
          if(disty < min2)
          {
            min2 = disty;
            besty = i;
          }
        }

       /* //For all candidate blocks in Y dimension
        for(i=-S; i<S+1; i+=S)     
        {
          disty = 0;

          //For all pixels in the block
          for(k=0; k<B; k++)     
          {
            for(l=0; l<B; l++)
            {
              p1 = current[B*x+k][B*y+l];

              if((B*x + vectors_x[x][y] + k) <0 ||
                  (B*x + vectors_x[x][y] + k) > (N-1) ||
                  (B*y + vectors_y[x][y] + i + l) < 0 ||
                  (B*y + vectors_y[x][y] + i + l) > (M-1))
              {
                q2 = 0;
              } else {
                q2 = previous[B*x+vectors_x[x][y]+k][B*y+vectors_y[x][y]+i+l];
              }

              disty += abs(p1-q2);
            }
          }

          if(disty < min2)
          {
            min2 = disty;
            besty = i;
          }
        }*/

        S = S/2;
        vectors_x[x][y] += bestx;
        vectors_y[x][y] += besty;
      }
    }
  }
} 

int main(int argc, char ** argv) {
  
  //read 2D-domain dimensions
  if (argc<2) {
    fprintf(stderr,"Usage: ./exec Bx (By-only for question-4)");
    exit(-1);
  }   
  else if (argc==2) {
    Bx=atoi(argv[1]);
    By=Bx;
  }
  else {
    Bx=atoi(argv[1]);
    By=atoi(argv[2]);
  }

  int current[N][M], previous[N][M], motion_vectors_x[N/Bx][M/By],
      motion_vectors_y[N/Bx][M/By];

  struct timeval tts,ttf;
	double timeTot,time1,time2;

 
	timeTot=time1=time2=0;

	gettimeofday(&tts,NULL);
	read_sequence(current,previous);
	gettimeofday(&ttf,NULL);
	time1+=(ttf.tv_sec-tts.tv_sec)+(ttf.tv_usec-tts.tv_usec)*0.000001;
	
	//printf("Time  to read is:%lf\n",time1);

	gettimeofday(&tts,NULL);
	phods_motion_estimation(current,previous,motion_vectors_x,motion_vectors_y);
	gettimeofday(&ttf,NULL);
	time2+=(ttf.tv_sec-tts.tv_sec)+(ttf.tv_usec-tts.tv_usec)*0.000001;
	//printf("Time  to proc-estimate is:%lf\n",time2);

	timeTot = time1 + time2 ;
	//printf("Total time is:%lf\n",timeTot);
  printf("%lf\n",timeTot);

  return 0;
}
