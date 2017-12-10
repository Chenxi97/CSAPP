/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1iiB direct mapped cache with a blocii size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	int i,ii,j,jj,t1,t2,t3,t4,t5,t6,t7,t8;	

	if(N==32){
		for(ii=0;ii<N;ii+=8){	
			for(jj=0;jj<M;jj+=8){
				for(i=ii;i<ii+8;i++){
					t1=A[i][jj];t2=A[i][jj+1];
					t3=A[i][jj+2];t4=A[i][jj+3];
					t5=A[i][jj+4];t6=A[i][jj+5];
					t7=A[i][jj+6];t8=A[i][jj+7];
					
					B[jj][i]=t1;B[jj+1][i]=t2;
					B[jj+2][i]=t3;B[jj+3][i]=t4;
					B[jj+4][i]=t5;B[jj+5][i]=t6;
					B[jj+6][i]=t7;B[jj+7][i]=t8;
				}			
			}
		}
	}	
	else if(N==64){
		for(i=0;i<64;i+=8){
            for(j=0;j<64;j+=8){
                for(ii=j;ii<j+4;++ii){
                    t1=A[ii][i];t2=A[ii][i+1];t3=A[ii][i+2];t4=A[ii][i+3];
                    t5=A[ii][i+4];t6=A[ii][i+5];t7=A[ii][i+6];t8=A[ii][i+7];

                    B[i][ii]=t1;B[i][ii+4]=t5;B[i+1][ii]=t2;B[i+1][ii+4]=t6;
                    B[i+2][ii]=t3;B[i+2][ii+4]=t7;B[i+3][ii]=t4;B[i+3][ii+4]=t8;                               
                }
                for(ii=i;ii<i+4;++ii){
                    t1=B[ii][j+4];t2=B[ii][j+5];t3=B[ii][j+6];t4=B[ii][j+7];
                    t5=A[j+4][ii];t6=A[j+5][ii];t7=A[j+6][ii];t8=A[j+7][ii];

                    B[ii][j+4]=t5;B[ii][j+5]=t6;B[ii][j+6]=t7;B[ii][j+7]=t8;
                    B[ii+4][j]=t1;B[ii+4][j+1]=t2;B[ii+4][j+2]=t3;B[ii+4][j+3]=t4;
                }
                for(ii=i+4;ii<i+8;++ii){
                    t1=A[j+4][ii];t2=A[j+5][ii];t3=A[j+6][ii];t4=A[j+7][ii];

                    B[ii][j+4]=t1;B[ii][j+5]=t2;B[ii][j+6]=t3;B[ii][j+7]=t4;
                }
            }
        }
	}
	else{
		for(ii=0;ii<N;ii+=16){	
			for(jj=0;jj<M;jj+=16){
				for(i=ii;i<ii+16&&i<N;i++){
					for(j=jj;j<jj+16&&j<M;j++){
						t1=A[i][j];
						B[j][i]=t1;
					}
				}
			}
		}
	}		
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    //registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checiis if B is the transpose of
 *     A. You can checii the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

