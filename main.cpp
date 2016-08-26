/*
 * Author:	Ericlin
 * Email:	463222898@qq.com
 * Date:	Fri Aug 26 21:42:28 CST 2016
 */
#include<iostream>
#include<iomanip>
#include<fstream>
#include<math.h>
#include<assert.h>
#include<chrono>
#include<random>
#include<vector>
using namespace std;
#define Trace(m)
#include<limits>
#define EPS std::numeric_limits<double>::epsilon()

bool feq(double a,double b){
	return fabs(a-b)<std::numeric_limits<double>::epsilon();
}
double dlines(double min, double max,int i,int size){
	return min+(max-min)*i/size;
}
void printArr(vector<double>arr){
	for(int i=0;i<arr.size();i++){
		cout<<arr[i]<<"\t";
	}
}
void printArr(ofstream&out, vector<double>arr){
	for(int i=0;i<arr.size();i++){
		out<<arr[i]<<"\t";
	}
}
void printArr(double *arr,int size){
	for(int i=0;i<size;i++){
		cout<<arr[i]<<"\t";
	}
}
void printArr(ofstream &out,double *arr,int size){
	for(int i=0;i<size;i++){
		out<<arr[i]<<"\t";
	}
}
void addArr(double *a,double *b,int size){
	//a+=b;
	for(int i=0;i<size;i++){
		a[i]+=b[i];
	}
}
void multiplyArr(double *a,int size,double scale){
	//a*=scale.
	for(int i=0;i<size;i++){
		a[i]*=scale;
	}
}
int getBestIndex(double *a,int size){
	int index=0;
	for(int i=1;i<size;i++){
		if(a[i]>a[index]){
			index=i;
		}
	}
	return index;
}
class NormalRand{
	public:
		unsigned lastSeed;
		default_random_engine generator;
		normal_distribution<double>distribution;
		NormalRand(double mean=0,double std=1.0):distribution(mean,std){
			seed();
		}
		void seed(unsigned s=0){
			if(s==0){
				s=chrono::system_clock::now().time_since_epoch().count();
			}
			generator.seed(s);
			lastSeed=s;
		}
		void reset(){
			//reset to last seed.
			generator.seed(lastSeed);
			distribution.reset();
		}
		double next(){
			return distribution(generator);
		}
};
class UniRand{
	public:
		unsigned lastSeed;
		default_random_engine generator;
		uniform_real_distribution<double>distribution;
		UniRand(double min=0.0,double max=1.0):distribution(min,max){
			//~[min,max)
			seed();
		}
		void seed(unsigned s=0){
			if(s==0){
				s=chrono::system_clock::now().time_since_epoch().count();
			}
			generator.seed(s);
			lastSeed=s;
		}
		void reset(){
			//reset to last seed.
			generator.seed(lastSeed);
			distribution.reset();
		}
		double next(){
			return distribution(generator);
		}
};

class UniDirectionRand{
	int numD;
	vector<NormalRand>r;
	static double getLength(vector<double>&a){
		double res=0;
		for(int i=0;i<a.size();i++){
			res+=a[i]*a[i];
		}
		return sqrt(res);
	}
	static void normalize(vector<double>&a){
		double l=getLength(a);
		if(feq(l,0.0))return ;
		for(int i=0;i<a.size();i++){
			a[i]/=l;
		}
	}
	public:
		void setD(int d){
			numD=d;
			r.resize(numD);
			for(int i=0;i<numD;i++){
				r[i].seed();
			}
		}
		UniDirectionRand(int numDimension=1):numD(numDimension),r(numD){
			for(int i=0;i<numD;i++){
				r[i].seed();
			}
		}
		vector<double>next(){
			vector<double>a(numD);
			for(int i=0;i<numD;i++){
				a[i]=r[i].next();
			}
			normalize(a);
			return a;
		}
};

class MPB{
	//Moving Peaks Problem
	//refer to paper:Memory Enhanced Evolutionary Algorithms for Changing Optimization Problems, Jurgen Branke
#define NumPeak 5
#define NumDim 5
	public:
		double W[NumPeak],H[NumPeak];
		double X[NumPeak][NumDim];
		int numP,numD;
		NormalRand Wtheta[NumPeak],Htheta[NumPeak];
		UniDirectionRand Vtheta[NumPeak];
		MPB(int numd=5,int nump=5){
			numP=nump;
			numD=numd;
			init();
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
			//seed the rands:
			for(int i=0;i<NumPeak;i++){
				Vtheta[i].setD(numD);
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
			for(int i=0;i<numP;i++){
				H[i]+=7.0*Htheta[i].next();
				W[i]+=0.01*Wtheta[i].next();
				vector<double>v=Vtheta[i].next();
				const double s=0.9;
				multiplyArr(&v[0],v.size(),s);
				addArr(X[i],&v[0],numD);
			}
		}
		double *getPeak(int i){
			return X[i];
		}
		double *getOptima(){
			int bi=getBestIndex(H,numP);
			return X[bi];
		}
		double *getH(){
			return H;
		}
		double *getW(){
			return W;
		}
};
const char * geneRandDirection(const char *file){
	UniDirectionRand n;
	n.setD(2);
	ofstream out(file);
	for(int i=0;i<10000;i++){
		printArr(out,n.next());out<<endl;
	}
	out.close();
	return file;
}
const char * geneNormalDistribution(const char *file){
	NormalRand n;
	ofstream out(file);
	for(int i=0;i<10000;i++){
		out<<n.next()<<endl;
	}
	out.close();
	return file;
}
const char * geneMPB(MPB&p,const char *file){
	//x range:
	const int NUMX=100;
	const int NUMY=100;
	double x1[2]={0,100};
	double x2[2]={0,100};
	//
	double xs[2],y;
	ofstream out(file);
#define cout out
	cout<<NUMX<<"\t"<<NUMY<<"\t"<<NUMX*NUMY<<endl;
	for(int i=0;i<NUMX;i++){
		xs[0]=dlines(x1[0],x1[1],i,NUMX);
		for(int j=0;j<NUMY;j++){
			xs[1]=dlines(x2[0],x2[1],j,NUMY);
			y=p.evaluate(xs,2);
			cout<<xs[0]<<"\t"<<xs[1]<<"\t"<<y<<endl;
		}
	}
#undef cout
	out.close();
	cout<<"Optima:";printArr(p.getOptima(),2);cout<<endl;
	return file;
}

const char * geneMovementOfOptima(MPB&p,const char *file){
	ofstream out(file);
	for(int i=0;i<5;i++){
		printArr(out,p.getPeak(i),2);out<<endl;
	}

	for(int i=0;i<300;i++){
		printArr(out,p.getOptima(),2);printArr(out,p.getH(),5);out<<endl;
		p.change();
	}
	out.close();
	return file;
}
void draw(const char *file,const char *type){
	char buff[1000];
	sprintf(buff,"python3 drawAll.py %s %s",file,type);
	system(buff);
}
void geneAndDrawAll(){
	MPB p1(2,5),p2(2,5),p3(2,5);
	draw(geneMovementOfOptima(p1,"movementOfOptima.txt"),"path");
	draw(geneMPB(p2,"MPB-function.txt"),"surface");
	draw(geneNormalDistribution("normalDistribution.txt"),"hist");
	draw(geneRandDirection("randDirection.txt"),"directionHist");
}

int main(){
	geneAndDrawAll();
	return 0;
}

