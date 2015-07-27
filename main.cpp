#include "quasiOneD.h"
#include "grid.h"
#include <iostream>
#include <vector>

int main()
{
	std::vector <double> x, S, V;
	std::vector <double> dx;
	int nx=10;


	InitializeGrid(nx, x, dx, S);

	std::cout<<"X\n";
	for(int i=0;i<x.size();i++)
		std::cout<<i<<" "<<x[i]<<std::endl;
	std::cout<<"S\n";
	for(int i=0;i<S.size();i++)
		std::cout<<i<<" "<<S[i]<<std::endl;
	std::cout<<"dx";
	for(int i=0;i<dx.size();i++)
		std::cout<<i<<" "<<dx[i]<<std::endl;
	
	
	//std::cout<<quasiOneD()<<std::endl;
	//std::cout<<"Test2222\n";
}
