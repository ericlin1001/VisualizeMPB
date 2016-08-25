#include<iostream>
#include<iomanip>
#include<math.h>
#include<assert.h>
using namespace std;
#define Trace(m)
#include<limits>
#define EPS std::numeric_limits<double>::epsilon()

bool feq(double a,double b){
	return fabs(a-b)<std::numeric_limits<double>::epsilon();
}
/*
bool flt(double a,double b){
	//float less than
	if(feq(a,b)){
		return false;
	}else{
		return a<b;
	}
}
*/
double f(double *xs,int size){
	double res=0;
	for(int i=0;i<size;i++){
		double x=xs[i];
		res+=x*x;
	}
	return res;
}
class MPB{
#define NumPeak 5
#define NumDim 5
	public:
		double W[NumPeak],H[NumPeak];
		double X[NumPeak][NumDim];
		int numP,numD;
		MPB(int numd=5,int nump=5){
			init();
			numP=nump;
			numD=numd;
		}
		void init(){
			//NumPeak*(NumDim+2)
			double tmp[NumPeak][NumDim+2]={{8.0,64.0,67.0,55.0,4.0,0.1,50.0},
							{50.0,13.0,76.0,15.0,7.0,0.1,50.0},
							{9.0,19.0,27.0,67.0,24.0,0.1,50.0},
							{66.0,87.0,65.0,19.0,43.0,0.1,50.0},
							{76.0,32.0,43.0,54.0,65.0,0.1,50.0},
			};
			for(int i=0;i<NumPeak;i++){
				W[i]=tmp[i][NumDim];
				H[i]=tmp[i][NumDim+1];
				for(int j=0;j<NumDim;j++){
					X[i][j]=tmp[i][j];
				}
			}
		}
		double evaluate(double *xs,int size){
			/* Let peak[i]=X[i][];
			 * Fmax=max(H[i]), with X*=peak[arg_max_i{H[i]}];
			 * i.e. F gets its optima H[i], when x is the X[i][:], with H[i] is the max of H[:].
			 *
			 * */
			Trace(size);
			Trace(numD);
			
			assert(size==numD);
			double res=-std::numeric_limits<double>::infinity();
			double tmp=0;
			for(int i=0;i<numP;i++){
				double tmp1=0;
				for(int j=0;j<numD;j++){
					tmp1+=pow(xs[j]-X[i][j],2);
				}
				tmp=(H[i]/(1.0+W[i]*tmp1));
				if(tmp>res){
					res=tmp;
				}
			}
			return res;

		}
		void change(){
		}
};
double dlines(double min,double max,int i,int total){
	//i=[0,total), res=[min,max)
	return min+(max-min)*i/total;
}
void geneSimpleF(){
	//x range:
	const int NUMX=100;
	const int NUMY=100;
	double x1[2]={-10,10};
	double x2[2]={-10,10};
	//
	double xs[2],y;
	cout<<NUMX<<"\t"<<NUMY<<"\t"<<NUMX*NUMY<<endl;
	for(int i=0;i<NUMX;i++){
		xs[0]=dlines(x1[0],x1[1],i,NUMX);
		for(int j=0;j<NUMY;j++){
			xs[1]=dlines(x2[0],x2[1],j,NUMY);
			y=f(xs,2);
			cout<<xs[0]<<"\t"<<xs[1]<<"\t"<<y<<endl;
		}
	}
}
void geneMPB(){
	MPB p(2,5);
	//x range:
	const int NUMX=100;
	const int NUMY=100;
	double x1[2]={0,100};
	double x2[2]={0,100};
	//
	double xs[2],y;
	cout<<NUMX<<"\t"<<NUMY<<"\t"<<NUMX*NUMY<<endl;
	for(int i=0;i<NUMX;i++){
		xs[0]=dlines(x1[0],x1[1],i,NUMX);
		for(int j=0;j<NUMY;j++){
			xs[1]=dlines(x2[0],x2[1],j,NUMY);
			y=p.evaluate(xs,2);
			cout<<xs[0]<<"\t"<<xs[1]<<"\t"<<y<<endl;
		}
	}
}
int main(){
	//geneSimpleF();
	geneMPB();
	return 0;
}

