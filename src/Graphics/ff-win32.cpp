#include "stdafx.h"
#ifndef kame
#ifdef _WIN32
#include <string>
using namespace std;
#include <windows.h>

#ifdef _WIN64
#pragma pack( push )
#pragma pack( 16 )
#include "Commdlg.h"
#pragma pack( pop )
#else
#include "Commdlg.h"
#endif   // _WIN64
//#include <commdlg.h>
#include <io.h>      //*OT  use for the console window
#endif
bool ShowOpenDialogBox1(char *fileName)
{
  OPENFILENAME *pofn= new OPENFILENAME[2], &ofn= *pofn;
    
  char szDirName[256];   
  const char *strFilter="freefem++ Files (*.edp)\0*.edp\0All Files (*.*)\0*.*\0\0"; 
  
  memset(&ofn, 0, 2*sizeof(OPENFILENAME));
  getcwd(szDirName,sizeof(szDirName));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = NULL;
  ofn.lpstrFilter = strFilter;
  ofn.lpstrFileTitle = fileName;
  ofn.nMaxFileTitle = 1024;
  ofn.lpstrInitialDir=szDirName;
  ofn.lpstrTitle ="Choose you freefem '*.edp' File";
  ofn.Flags=OFN_SHOWHELP|OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
  
  BOOL ret= GetOpenFileNameA(pofn);
    cout << " ret "<< ret << " "<< fileName << " "<< szDirName << endl;
  delete[] pofn;
  return ret;
} 
#ifndef kame


string ChangeExt(const string & ff,const char * suff)
{
  int dot = ff.rfind(".edp");
  assert(dot>0);
  return ff.substr(0,dot)+suff;
}

bool GetConsoleBuff(const string &edpname)
{
  CONSOLE_SCREEN_BUFFER_INFO csbi; //* to get buffer info
  HANDLE hConOut= GetStdHandle(STD_OUTPUT_HANDLE);
  //cout << " handle " << hConOut << endl; 
  if( hConOut == 0) return false ;  
  if ( INVALID_HANDLE_VALUE == hConOut) return false; 
  GetConsoleScreenBufferInfo(hConOut, &csbi);
  
  COORD coordLine = {0,0};
  CHAR *szLine=0;  //* buffer to read from the console (a line)
  DWORD dwCharsRead;
  FILE *fp;
  string  fname=ChangeExt(edpname,".log");
  if ((fp = fopen(fname.c_str(),"w"))==NULL) {
    perror(fname.c_str());
    cout<< " err fopen logfile: "<< fname <<  endl;	  	
    return false;
  }
  szLine = new CHAR [csbi.dwSize.X+1];
  for (int i=0; i<csbi.dwCursorPosition.Y; i++) 
	{
       	 if (ReadConsoleOutputCharacter(hConOut, szLine,
				   csbi.dwSize.X, coordLine,
				   &dwCharsRead)== FALSE)
      {
	perror("ReadConsoleOutputCharacter");
	cout << " err ReadConsoleOutputCharacter " <<i << " " << csbi.dwCursorPosition.Y <<  endl;
	return false;
      }
    int j=csbi.dwSize.X-1;
    while ((szLine[j] == ' ') && (j > 0)) szLine[j--] =0;
    if (j < csbi.dwSize.X-1) szLine[j+1] = '\n'; 
    fprintf(fp,"%s",szLine);
    coordLine.Y++;
  }
  fclose(fp);
  delete [] szLine;
  cout << " save log in :  '"<< fname << "'\n"  ;
  return true;
}
#endif
#endif