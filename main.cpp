#include<iostream>
#include<iomanip>
#include<math.h>
using namespace std;
#include<limits>
#define EPS std::numeric_limits<double>::epsilon()

bool feq(double a,double b){
	return fabs(a-b)<std::numeric_limits<double>::epsilon();
}
bool flt(double a,double b){
	//float less than
	if(feq(a,b)){
		return false;
	}else{
		return a<b;
	}
}
double f(double *xs,int size){
	double res=0;
	for(int i=0;i<size;i++){
		double x=xs[i];
		res+=x*x;
	}
	return res;
}
double dlines(double min,double max,int i,int total){
	//i=[0,total)
	//res=[min,max)
	return min+(max-min)*i/total;
}
int main(){
	//x range:
	double x1[2]={-10,10};
	double x2[2]={-10,10};
	//
	double xs[2],y;
#define NUMX 100
#define NUMY 100
	cout<<NUMX<<"\t"<<NUMY<<"\t"<<NUMX*NUMY<<endl;
	for(int i=0;i<NUMX;i++){
		xs[0]=dlines(x1[0],x1[1],i,NUMX);
		for(int j=0;j<NUMY;j++){
			xs[1]=dlines(x2[0],x2[1],j,NUMY);
			y=f(xs,2);
			cout<<xs[0]<<"\t"<<xs[1]<<"\t"<<y<<endl;
		}
	}
	return 0;
}

