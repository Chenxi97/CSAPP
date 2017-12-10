#include "cachelab.h"
#include "stdio.h"
#include "getopt.h"
#include "stdlib.h"
#include "unistd.h"
#include "math.h"
#include "string.h"
#define SIZE 20
typedef struct{
	int valid;
	int tag;
	int time_stamp;
}cache_line;
cache_line **cache;
int vflag=0,hit_count=0,miss_count=0,evic_count=0,time=0;
int S,s,E,b;

int getinfo(int argc,char * const argv[],char *pt);
void buildset(void);
int op(char *ps);
void get(int *ps,int *pt,char *p);
int x2d(char px);
void usage(void);
int main(int argc,char *argv[])
{
	int i,opflag;
	char trace[SIZE],ps[SIZE];
	char ins;
	FILE *fp=NULL;
	
	getinfo(argc,argv,trace);
	buildset();
	fp=fopen(trace,"r");
	if(fp==NULL){
		printf("File open fail\n");
		exit(1);
	}
	while(fscanf(fp," %c %s",&ins,ps)!=EOF){
		if(ins=='I')
			continue;
		else{
			opflag=op(ps);
			if(ins=='M') hit_count++;
			if(vflag){
				printf("%c %s %s",ins,ps,opflag?(opflag==1?
				"miss":"miss eviction"):"hit");
			if(ins=='M')
				 printf(" hit");
			printf("\n");
			}
		}		
	}
	fclose(fp);
	fp=NULL;
	for(i=0;i<S;i++)
		free(cache[i]);
	free(cache);
    printSummary(hit_count,miss_count,evic_count);
    return 0;
}

int getinfo(int argc,char * const argv[],char *pt)
{
	int opt;
	opterr=0;
	int count_arg = 0; 
	while((opt=getopt(argc,argv,"hvs:E:b:t:"))!=-1){
		switch(opt){
		case 'h':
			usage();
			break;
		case 'v':
			vflag=1;
			break;
		case 's':
			++count_arg;
			s=atoi(optarg);
			S=1<<s;
			break;
		case 'E':
			++count_arg;
			E=atoi(optarg);
			break;
		case 'b':
			++count_arg;
			b=atoi(optarg);
			break;
		case 't':
			++count_arg;
			strcpy(pt,optarg);
			break;
		default:
			usage();
			exit(-1);
			break;
		}
	}
	if (count_arg < 4) {  
		usage();  
		exit(-1);  
	}  
	return 0;
}

void buildset(void)
{
	int i,j;
	cache=(cache_line **)malloc(sizeof(cache_line *)*S);
	for(i=0;i<S;i++){
		cache[i]=(cache_line*)malloc(sizeof(cache_line)*E);
		for(j=0;j<E;j++){
		cache[i][j].valid=0;
		cache[i][j].tag=0;
		cache[i][j].time_stamp=0;
		}
	}			
}
	
int op(char *ps)
{
	int i,sets,tag,mintime,min;
	
	get(&sets,&tag,ps);
	for(i=0;i<E;i++){
		if((cache[sets][i].valid==1)&&(cache[sets][i].tag==tag)){
			cache[sets][i].time_stamp=time++;	//hit时也要更新time_stamp
			hit_count++;
			return 0;
		}	
	}
	for(i=0;i<E;i++){
		if(cache[sets][i].valid==0){
			cache[sets][i].tag=tag;
			cache[sets][i].valid=1;
			cache[sets][i].time_stamp=time++;
			miss_count++;
			return 1;
		}
	}
	mintime=time+1;
	for(i=0;i<E;i++)
		if(cache[sets][i].time_stamp<mintime){
			mintime=cache[sets][i].time_stamp;
			min=i;
		}
	cache[sets][min].tag=tag;
	cache[sets][min].time_stamp=time++;
	miss_count++;
	evic_count++;
	return 2;
}

void get(int *ps,int *pt,char *p)
{
	int i,c=1,sum=0,bias;
	
	for(i=0;p[i]!=',';i++);
	for(;i>0;i--){
		sum+=c*x2d(p[i-1]);
		c*=16;
	}
	sum>>=b;
	bias=~(-1<<s);
	*ps=(sum&bias)%S;
	*pt=sum>>s;
}

int x2d(char px)
{
	if(px>='0'&&px<='9')
		return px-'0';
	else
		return px-'a'+10;
}

void usage(void)
{
	printf("usage:./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
		"• -h: Optional help flag that prints usage info\n"
		"• -v: Optional verbose flag that displays trace info\n"
		"• -s <s>: Number of set index bits (S = 2s is the number of sets)\n"
		"• -E <E>: Associativity (number of lines per set)\n"
		"• -b <b>: Number of block bits (B = 2b is the block size)\n"
		"• -t <tracefile>: Name of the valgrind trace to replay\n");
}
	
