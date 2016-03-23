//// For benchmarking CIdxComb and next_combination()
//// This version uses vectors
//#include <iostream>
//#include <vector>
////#include <windows.h>
//#include "combination.h"
//#include "IndexCombination.h"
//#include "FindTotalComb.h"
//#include "FindCombByIdx.h"
//
//#include "BigIntegerLibrary.h"
//
//using namespace std;
//using namespace stdcomb;
//
////#define XINT64 __int64
//#define XINT64 BigInteger
//
//// Function Prototypes
//int CalPercent( double Curr, double Total );
//double ConvLargeInt( LARGE_INTEGER Li );
//
//const int SET = 20;
//const int COMB = 6;
//const int TOTALCOMB = 38760;
//
//LARGE_INTEGER Freq;
//LARGE_INTEGER Beg;
//LARGE_INTEGER End;
//
//int main()
//{
//
//	if( !QueryPerformanceFrequency( &Freq ) )
//		return 1;
//
////  Benchmark for template function, next_combination()
//	vector<unsigned int> ca;
//	int a=0;
//	for( a=0; a<SET; ++a )
//		ca.push_back (a);
//
//	vector<unsigned int> cb;
//	for( int b=0; b<COMB; ++b )
//		cb.push_back (b);
//
//	bool bEnd = false;
//
//	int CurrTotal = 1;
//	int CurrPercent = 0;
//
//	QueryPerformanceCounter( &Beg );
//
//	const int ITERATION = 10;
//	for( a = 0; a<ITERATION; ++a )
//	{
//		while( !bEnd )
//		{
//
//
//			if( !next_combination( ca.begin(), ca.end(), cb.begin(),cb.end() ) )
//			{
//				bEnd  = true;
//			}
//	/*
//			++CurrTotal;
//
//			int Percent = CalPercent( CurrTotal, TOTALCOMB );
//			if( CurrPercent != Percent )
//			{
//				CurrPercent = Percent;
//				cout<< "Percentage completed : " << CurrPercent << endl;
//			}
//	*/
//		}
//	}
//	QueryPerformanceCounter( &End );
//
//	double dInteval = ConvLargeInt(End) - ConvLargeInt(Beg);
//	double dFreq = ConvLargeInt(Freq);
//	double dNextComb = dInteval / dFreq * 1000.0;
//	cout<< "Inteval for next_combination : " << dNextComb << endl;
//
////  Benchmark for CIdxComb class
//	CIdxComb cic(SET,COMB);
//
//	std::vector<unsigned int> vi(COMB);
//
//	int c=0;
//	for( c=0; c<COMB; ++c )
//		vi[c] = c;
//
//	bEnd = false;
//
//	CurrTotal = 1;
//	CurrPercent = 0;
//
//	QueryPerformanceCounter( &Beg );
//	for( a=0; a<ITERATION; ++a )
//	{
//		while( !bEnd )
//		{
//
//			if( !cic.GetNextComb( vi ) )
//			{
//				bEnd  = true;
//			}
//	/*
//			++CurrTotal;
//
//			int Percent = CalPercent( CurrTotal, TOTALCOMB );
//			if( CurrPercent != Percent )
//			{
//				CurrPercent = Percent;
//				cout<< "Percentage completed : " << CurrPercent << endl;
//			}
//	*/
//		}
//	}
//	QueryPerformanceCounter( &End );
//
//	dInteval = ConvLargeInt(End) - ConvLargeInt(Beg);
//	dFreq = ConvLargeInt(Freq);
//	double dIdxComb = dInteval / dFreq * 1000.0;
//	cout<< "Inteval for CIdxComb : " << dIdxComb << endl;
//
//
////  Benchmark for FindCombByIdx function
//////////////////////////////////////////////
//	std::vector<unsigned int> vi2(COMB);
//
//	for( c=0; c<COMB; ++c )
//		vi2[c] = c;
//
//	bEnd = false;
//
//	CurrTotal = 1;
//	CurrPercent = 0;
//
//	unsigned int CIndex = 0;
//
//	CFindCombByIdx< CFindTotalComb<XINT64>, XINT64 > fcbi;
//
//	QueryPerformanceCounter( &Beg );
//	for( a=0; a<ITERATION; ++a )
//	{
//		while( !bEnd )
//		{
//
//			fcbi.FindCombByIdx( SET, COMB, CIndex, vi2 );
//
//			++CIndex;
//
//			if( CIndex == TOTALCOMB )
//			{
//				bEnd  = true;
//			}
//
//			//++CurrTotal;
//
//			//int Percent = CalPercent( CurrTotal, TOTALCOMB );
//			//if( CurrPercent != Percent )
//			//{
//			//	CurrPercent = Percent;
//			//	cout<< "Percentage completed : " << CurrPercent << endl;
//			//}
//		}
//	}
//	QueryPerformanceCounter( &End );
//
//	dInteval = ConvLargeInt(End) - ConvLargeInt(Beg);
//	dFreq = ConvLargeInt(Freq);
//	double dFindComb = dInteval / dFreq * 1000.0;
//	cout<< "Inteval for CFindComb : " << dFindComb << endl;
//
//	system( "pause" );
//
//	return 0;
//}
//
//
//int CalPercent( double Curr, double Total )
//{
//	if( Total == 0 )
//		throw string("Division by Zero in CalPercent()");
//
//	int Ans = (int) (( Curr/Total ) * 100.0 + 0.51);
//
//	return Ans;
//}
//
//
//double ConvLargeInt( LARGE_INTEGER Li )
//{
//	double Ans = Li.LowPart + Li.HighPart * 4294967296.0;
//
//	return Ans;
//}
