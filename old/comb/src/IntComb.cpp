// IntComb.cpp : Defines the entry point for the console application.
//

#include "IndexCombination.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace stdcomb;

template<class BidIt>
void display(BidIt begin,BidIt end)
{
  for (BidIt it=begin;it!=end;++it)
      cout<<*it<<" ";
  cout<<endl;
}

#include "combination.h"

int main() {

	vector<int> ca, cb;

	for(unsigned i = 1; i <= 45; i++)
		ca.push_back (i);
	for(unsigned i = 1; i <= 7; i++)
		cb.push_back (i);

//  cb.push_back (33);
//  cb.push_back (35);
//  cb.push_back (37);
//  cb.push_back (41);
//  cb.push_back (42);
//  cb.push_back (43);
//  cb.push_back (44);

	do {
		display(cb.begin(),cb.end());
	} while(next_combination(ca.begin (),ca.end (),cb.begin (),cb.end()) );

	cout<<"Complete!"<<endl;

	return 0;
}


//int main(int argc, char* argv[])
//{
//	CIdxComb cb;
//
//	cb.SetSizes(5,3);
//
//
//	vector<string> vsAnimal;
//	vsAnimal.push_back( "Elephant" );
//	vsAnimal.push_back( "Deer" );
//	vsAnimal.push_back( "Cat" );
//	vsAnimal.push_back( "Bear" );
//	vsAnimal.push_back( "Ape" );
//
//	vector<unsigned int> vi(3);
//
//	vi[0] = 0;
//	vi[1] = 1;
//	vi[2] = 2;
//
//	cout<< vsAnimal[ vi[0] ] << " "
//		<< vsAnimal[ vi[1] ] << " "
//		<< vsAnimal[ vi[2] ] << "\n";
//
//	int Total = 1;
//	while ( cb.GetNextComb( vi ) )
//	{
//		// Do whatever processing you want
//
//
//		cout<< vsAnimal[ vi[0] ] << " "
//			<< vsAnimal[ vi[1] ] << " "
//			<< vsAnimal[ vi[2] ] << endl;
//
//		++Total;
//	}
//
//	cout<< "\nTotal : " << Total << endl;
//
//	return 0;
//}
