#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <conio.h>
// 자기참조구조체(연결리스트), 구조체
typedef struct TNodeData
{
	char m_szName[20];
	int	 m_iAge;
	char m_strAddress[50];
	struct TNodeData*  pNext;
}TNode;
char*	GetName(TNode* pNode, int iIndex ) { return pNode[iIndex].m_szName; }
int		GetAge(TNode* pNode, int iIndex) { return pNode[iIndex].m_iAge; }
char*	GetAddress(TNode* pNode, int iIndex) { return pNode[iIndex].m_strAddress; }
///////////////////////////////////////////////////////////////////
// 블록단위 입출력
///////////////////////////////////////////////////////////////////
int main ()
{
   FILE    *fp_src;
   FILE    *fp_dest;
   char     buff[4096];
   size_t   n_size;
                                          
   fp_src  = fopen( "sample.cpp", "r");
   fp_dest  = fopen( "sample.bak", "w");
                            
   while( 0 < (n_size = fread( buff, 1, 4096, fp_src)))
   {
	  fwrite( buff, 1, n_size, fp_dest);
   }                            

   fclose( fp_src);
   fclose( fp_dest);

   ///////////////////////////////////////////////////////
   // 텍스트 파일 블럭 단위 출력
   ///////////////////////////////////////////////////////      
   TNode List[3] = {
	   {"홍길동", 24, "서울"},
	   {"김정일", 44, "대전"},
	   {"나그네", 54, "부산"}   };	   
   fp_dest  = fopen( "BlockRead.dat", "w");
   fwrite( List, 1, sizeof( TNode ) * 3, fp_dest);
   fclose( fp_dest);
   ///////////////////////////////////////////////////////
   // 텍스트 파일 블록단위 입력
   ///////////////////////////////////////////////////////      
   TNode Load[3];
   fp_src  = fopen( "BlockRead.dat", "r");
   fread( Load, 1, sizeof( TNode ) *3, fp_src);
   fclose( fp_src);

   ///////////////////////////////////////////////////////
   // 텍스트 파일 라인 출력
   ///////////////////////////////////////////////////////   
   fp_dest  = fopen( "LineWrite.txt", "wt");
   for( int iCnt = 0; iCnt < 3; iCnt++ )
   {
	   fprintf( fp_dest, "\n%s %d %s",
		   GetName(List, iCnt),
		   GetAge(List, iCnt),
		   GetAddress(List, iCnt) );
   }
   fclose( fp_dest);

   ///////////////////////////////////////////////////////
   // 텍스트 파일 라인 입력
   ///////////////////////////////////////////////////////   
   TNode LineBuffer[3];
   char pBuffer[256];
   int iCount = 0;
   fp_src  = fopen( "LineWrite.txt", "rt");
   fgets( pBuffer, 256, fp_src );
   while( !feof( fp_src ) )
   {	    
		fgets( pBuffer, 256, fp_src );		
		sscanf(pBuffer, "%s %d %s", LineBuffer[iCount].m_szName, 
			&LineBuffer[iCount].m_iAge, 
			LineBuffer[iCount].m_strAddress );
		iCount++;
   }
   fclose( fp_src);
   return 0;
	
}