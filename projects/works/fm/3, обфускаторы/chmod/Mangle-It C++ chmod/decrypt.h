#ifndef PCSENTINEL_DECRYPT_H
#define PCSENTINEL_DECRYPT_H
#pragma once
#pragma warning(disable:4996)
#include <map>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <tchar.h>
#include <Windows.h>
using namespace std;
#define BCHAR 128
#define EKY  "143860843"
class sYYYYYYYXXYY
{
private:
  int sYYYYYYYXYXX;
  char* sYYYYYYYXYXY;
public:
	sYYYYYYYXXYY(const wchar_t* sYYYYYYYXYYX, int sYYYYYYYXYYY = 1):sYYYYYYYXYXX(sYYYYYYYXYYY)
  {
    sYYYYYYYXYXY = new char [(int)wcslen(sYYYYYYYXYYX)+1];
	wcstombs( sYYYYYYYXYXY, sYYYYYYYXYYX, (int)wcslen(sYYYYYYYXYYX)+1);
  }
  sYYYYYYYXXYY(const char* szText, int sYYYYYYYXYYY = 1):sYYYYYYYXYXX(sYYYYYYYXYYY)
  {
    sYYYYYYYXYXY = new char [(int)strlen(szText) + 1];
	strcpy(sYYYYYYYXYXY, szText);
  }
  ~sYYYYYYYXXYY(){ if (sYYYYYYYXYXX) { delete [] sYYYYYYYXYXY; }   }
  operator char*() { return (char*)sYYYYYYYXYXY;  }
  operator const char*(){ return (const char*)sYYYYYYYXYXY;  }
};
class sYYYYYYYYXXX
{
private:
  int sYYYYYYYXYXX;
  wchar_t* sYYYYYYYYXXY;
public:
  sYYYYYYYYXXX(const wchar_t* sYYYYYYYXYYX, int sYYYYYYYXYYY = 1):sYYYYYYYXYXX(sYYYYYYYXYYY)
  {
    sYYYYYYYYXXY = new wchar_t [(int)wcslen(sYYYYYYYXYYX)+1];
	wcscpy(sYYYYYYYYXXY, sYYYYYYYXYYX);
  }
  sYYYYYYYYXXX(const char* szText, int sYYYYYYYXYYY = 1):sYYYYYYYXYXX(sYYYYYYYXYYY)
  {
    sYYYYYYYYXXY = new wchar_t [(int)strlen(szText) + 1];
    mbstowcs(sYYYYYYYYXXY, szText, (int)strlen(szText) + 1);
  }
  void Delete() { if( sYYYYYYYYXXY ){ delete [] sYYYYYYYYXXY; sYYYYYYYYXXY = 0; }}
  ~sYYYYYYYYXXX(){if (sYYYYYYYXYXX) { delete [] sYYYYYYYYXXY;} }
  operator wchar_t*()  { return (wchar_t*)sYYYYYYYYXXY;}
  operator const wchar_t*() { return (const wchar_t*)sYYYYYYYYXXY;}
};
class decrypt
{
protected:
	static const char* sYYYYYYYYXYX( const char* sYYYYYYYYXYY )
	{
		int sYYYYYYYYYXX = (int)strlen(sYYYYYYYYXYY);
		if( !sYYYYYYYYYXX )
		{
			char *sYYYYYYYYYXY = new char[1];
			sYYYYYYYYYXY[0]='\0';
			return sYYYYYYYYYXY;
		}
		char* sYYYYYYYYYYX = new char[sYYYYYYYYYXX+1];
		for(int i =0; i<(sYYYYYYYYYXX+1);++i)
		{
			sYYYYYYYYYYX[i]='\0';
		}
		//SecureZeroMemory(sYYYYYYYYYYX,sYYYYYYYYYXX+1);
		sYYYYYYYYYYY sYXXXXXXXXXXX((const unsigned char *)EKY, (unsigned int)strlen((EKY)));
		if (sYYYYYYYYYXX > 2)
		{
		  int sYXXXXXXXXXXY = (int)strlen(sYYYYYYYYXYY);
		  if (sYXXXXXXXXXXY & 0x01)
		  {
			 return sYYYYYYYYXYY;
		  }
		  sYXXXXXXXXXXY/=2;
		  for (int sYXXXXXXXXXYX=0;sYXXXXXXXXXYX<sYXXXXXXXXXXY;sYXXXXXXXXXYX++)
		  {
			 int sYXXXXXXXXXYY = sYYYYYYYYXYY[sYXXXXXXXXXYX * 2] - BCHAR;
			 int sYXXXXXXXXYXX = sYYYYYYYYXYY[sYXXXXXXXXXYX * 2 + 1] - BCHAR;
			 int sYXXXXXXXXYXY = (sYXXXXXXXXXYY << 4) | sYXXXXXXXXYXX;
			 unsigned char sYXXXXXXXXYYX = sYXXXXXXXXXXX.sYXXXXXXXXYYY((unsigned char)(sYXXXXXXXXYXY));
			 if (sYXXXXXXXXXYX>0)
				strncat( sYYYYYYYYYYX, (const char*)&sYXXXXXXXXYYX,1 );
		  }
		}
		return sYYYYYYYYYYX;
	}
public:
#ifdef _UNICODE
	static const wchar_t* dec_debug( const wchar_t* sYYYYYYYYXYY ){
wstring ss(sYYYYYYYYXYY);
size_t pos = ss.find( _T("_debug_") );
if( pos !=string::npos )
	ss= ss.substr(pos+7).c_str();
GetDebugUnicodeMap();
map<wstring, wstring>::iterator sYXXXXXXXYYYX = mpw_debug->find(ss);
if(  sYXXXXXXXYYYX == mpw_debug->end() )
{
	mpw_debug->insert( map<wstring, wstring>::value_type(ss, ss ));
	map<wstring, wstring>::iterator sYXXXXXXXYYYY = mpw_debug->find(ss);
	return (*sYXXXXXXXYYYY).second.c_str();
}
else
{
	return (*sYXXXXXXXYYYX).second.c_str();
}
}
#endif
	static const wchar_t* dec( const wchar_t* sYYYYYYYYXYY )
	{
		GetUnicodeMap();
		sYYYYYYYXXYY ch(sYYYYYYYYXYY);
		const char* sYXXXXXXXYXXX = sYYYYYYYYXYX( (const char*)ch );
		sYXXXXXXXYXXX = Esc( sYXXXXXXXYXXX );
		sYYYYYYYYXXX sYXXXXXXXYXXY(sYXXXXXXXYXXX,0);
		delete [] sYXXXXXXXYXXX;
		wstring sYXXXXXXXYXYX( (wchar_t*)sYXXXXXXXYXXY );

		map<wstring, const wchar_t*>::iterator sYXXXXXXXYXYY = mpw->find(sYXXXXXXXYXYX);
		if(  sYXXXXXXXYXYY == mpw->end() )
		{
			mpw->insert( map<wstring, const wchar_t*>::value_type(sYXXXXXXXYXYX,(wchar_t*)sYXXXXXXXYXXY));
			return (wchar_t*)sYXXXXXXXYXXY;
		}
		else
		{
			sYXXXXXXXYXXY.Delete();
			return (*sYXXXXXXXYXYY).second;
		}
	}
	static const char* dec_debug( const char* sYYYYYYYYXYY ){
string ss(sYYYYYYYYXYY);
size_t pos = ss.find( "_debug_" );
if( pos !=string::npos )
	ss= ss.substr(pos+7).c_str();
GetDebugAnsiMap();
map<string, string>::iterator sYXXXXXXXYYYX = mp_debug->find(ss);
if(  sYXXXXXXXYYYX == mp_debug->end() )
{
	mp_debug->insert( map<string, string>::value_type(ss, ss ));
	map<string, string>::iterator sYXXXXXXXYYYY = mp_debug->find(ss);
	return (*sYXXXXXXXYYYY).second.c_str();
}
else
{
	return (*sYXXXXXXXYYYX).second.c_str();
}
}
	static const char* dec( const char* sYYYYYYYYXYY )
	{
		GetAnsiMap();
		const char* sYXXXXXXXYYXX = sYYYYYYYYXYX(sYYYYYYYYXYY);
		sYXXXXXXXYYXX = Esc( sYXXXXXXXYYXX );
		string sYXXXXXXXYYXY(sYXXXXXXXYYXX);
		map<string, const char*>::iterator sYXXXXXXXYYYX = mp->find(sYXXXXXXXYYXY);
		if(  sYXXXXXXXYYYX == mp->end() )
		{
			mp->insert( map<string, const char*>::value_type(sYXXXXXXXYYXY, sYXXXXXXXYYXX));
			map<string, const char*>::iterator sYXXXXXXXYYYY = mp->find(sYXXXXXXXYYXY);
			return (*sYXXXXXXXYYYY).second;
		}
		else
		{
			delete [] sYXXXXXXXYYXX;
			sYXXXXXXXYYXX=0;
			return (*sYXXXXXXXYYYX).second;
		}
	}
protected:
decrypt(void) {};
~decrypt(void){};
static map<wstring, wstring>* mpw_debug;
static map<string, string>* mp_debug;
	static map<string, const char*>* mp;
	static map<wstring, const wchar_t*>* mpw;
static const char* Esc( const char* inCh )
{
	int nLen = (int)(strlen(inCh)+2);
	char* outCh = new char[nLen];
	for( int i =0; i<nLen;++i)
	{
		outCh[i]='\0';
	}
   for (int i=0;i<nLen; i++)
   {
      int c = inCh[i];
      switch (c)
      {
      default:
		  strncat(outCh, &inCh[i],1);
         break;
      case '\\':
         if (i < nLen - 1)
         {
            c = inCh[i + 1];
            switch (c)
            {
            case ('n'):
				{
				char ch = '\n';
				strncat(outCh,&ch,1);
               break;
				}
            case ('t'):
				{char ch = '\t';
				strncat(outCh,&ch,1);
				break;}
            case ('v'):
				{
				char ch = '\v';
				strncat(outCh,&ch,1);
				break;}
            case ('b'):
				{
				char ch = '\b';
				strncat(outCh,&ch,1);
               break;
				}
            case ('r'):
				{char ch = '\r';
				strncat(outCh,&ch,1);
              break;
				}
            case ('f'):
				{char ch = '\f';
				strncat(outCh,&ch,1);
				break;}
            case ('a'):
				{char ch = '\a';
				strncat(outCh,&ch,1);
				break;}
            case ('\\'):
				{char ch = '\\';
				strncat(outCh,&ch,1);
				break;}
            case ('?'):
				{char ch = '\?';
				strncat(outCh,&ch,1);
				break;}
           case ('\''):
				{char ch = '\'';
				strncat(outCh,&ch,1);
				break;}
           case ('"'):
				{char ch = '"';
				strncat(outCh,&ch,1);
				break;}
            case ('0'):
            case ('1'):
            case ('2'):
           case ('3'):
           case ('4'):
           case ('5'):
           case ('6'):
           case ('7'):
              {
                 int iConsumed = 0;
				  char* cTemp = new char[nLen-i+2];
				  ZeroMemory(cTemp,nLen-i+2);
				  memcpy(cTemp,inCh,nLen-i);
                  if (!ExpandOctal(cTemp, outCh, iConsumed))
                  {
                     outCh[i] = inCh[i];
					 delete [] cTemp;
                    break;
                  }
                  i+=iConsumed - 1;
               }
               break;
            case ('x'):
               { 
                  int iConsumed = 0;
				  char* cTemp = new char[nLen-i+2];
				  for( int j=0;j<nLen-j+2;++j)
					  cTemp[j]='\0';
				  memcpy(cTemp,inCh,nLen-i);
                  if (!ExpandHex( cTemp, outCh, iConsumed))
                  {
                     outCh[i] = inCh[i];
                    break;
                 }
                 i+=iConsumed - 1;
              }
              break;
           }
           i++;
           continue;
        }
        else
        {
           outCh[i] = inCh[i];
           break;
        }
        break;
     }
	}
	if( inCh )
	{
		delete [] inCh;
		inCh = NULL;
	}
	return outCh;
}
static bool ExpandOctal(const char* chIn, char* chOut, int &iConsumed/*const CString &csIn, CString &csOut, int &iConsumed*/)
{
   if (strlen(chIn) < 2)
   {
      return false;
   }
   if (chIn[0] != ('\\'))
   {
     return false;
   }
   int iStart = 1;
   int iCur = iStart;
   #ifdef _UNICODE
   wstring digits;
   #else
   string digits;
   #endif
   int c = chIn[iCur];
   while ((c >= ('0')) && (c <= ('7')))
   {
      digits+= (char)c;
      if (iCur == 3)
      {
         break;
      }
     iCur++;
     c = chIn[iCur];
  }
   TCHAR *end = NULL;
   int octval = (char)_tcstol(digits.c_str(), &end, 8);
   iConsumed = (int)digits.length();
   char ch = (char)octval;
   strncat( chOut,&ch,1 );
   return true;
}
static bool ExpandHex(const char* chIn, char* chOut, int &iConsumed/*const CString &csIn, CString &csOut, int &iConsumed*/)
{
   if ( strlen(chIn) < 3)
   {
      return false;
   }
   if ((chIn[0] != ('\\')) || (chIn[1] != ('x')))
   {
      return false;
   }
   int iStart = 2;
  int iCur = iStart;
   #ifdef _UNICODE
   wstring digits;
   #else
   string digits;
   #endif
   int c = chIn[iCur];
   while (_istxdigit(c))
   {
      digits+= (char)c;
      iCur++;
      c = chIn[iCur];
   }
   TCHAR *end = NULL;
   int hex = (char)_tcstol(digits.c_str(), &end, 16);
	char ch = (char)hex;
	iConsumed = (int)digits.length();
  iConsumed++; 
   strncat(chOut,&ch, 1 );
   return true;
}
static void GetDebugAnsiMap()
{
	if( !mp_debug )
	{
		mp_debug = new map<string, string>();
	}
}
static void GetDebugUnicodeMap()
{
	if( !mpw_debug )
	{
		mpw_debug = new map<wstring, wstring>();
	}
}
	static void GetAnsiMap()
	{
		if( !mp )
			mp = new map<string, const char*>();
	}
	static void GetUnicodeMap()
	{
		if( !mpw )
			mpw = new map<wstring, const wchar_t*>();
	}
	class sYYYYYYYYYYY
	{
		public:
		   sYYYYYYYYYYY(const unsigned char *key, unsigned int sYXXXXXXYXXXX)
		   {
			  int sYXXXXXXXXXYX;unsigned char sYXXXXXXYXXXY;unsigned sYXXXXXXYXXYX;
				for(sYXXXXXXXXXYX=0;sYXXXXXXXXXYX<256;sYXXXXXXXXXYX++)
					sYXXXXXXXXYXY[sYXXXXXXXXXYX]= (unsigned char)sYXXXXXXXXXYX;
				sYXXXXXXYXXYX=0;sYXXXXXXYXXXY=0;
				for(sYXXXXXXXXXYX=255;sYXXXXXXXXXYX;sYXXXXXXXXXYX--)
					std::swap(sYXXXXXXXXYXY[sYXXXXXXXXXYX],sYXXXXXXXXYXY[sYXXXXXXYXXYY(sYXXXXXXXXXYX,key,(unsigned char)sYXXXXXXYXXXX,&sYXXXXXXYXXXY,&sYXXXXXXYXXYX)]);
				sYXXXXXXYXYXX=sYXXXXXXXXYXY[1];
				sYXXXXXXYXYXY=sYXXXXXXXXYXY[3];
				sYXXXXXXYXYYX=sYXXXXXXXXYXY[5];
				sYXXXXXXYXYYY=sYXXXXXXXXYXY[7];
				sYXXXXXXYYXXX=sYXXXXXXXXYXY[sYXXXXXXYXXXY];
				sYXXXXXXYXXXY=0;sYXXXXXXYXXYX=0;
		   }
		   unsigned char sYXXXXXXYXXYY(unsigned int sYXXXXXXYYXXY, const unsigned char *sYXXXXXXYYXYX, unsigned char sYXXXXXXYXXXX, unsigned char *sYXXXXXXYXXXY, unsigned *sYXXXXXXYXXYX)
		   {
			  unsigned sYXXXXXXYYXYY=0,sYXXXXXXYYYXX=1,sYXXXXXXYYYXY;
			  while(sYXXXXXXYYYXX<sYXXXXXXYYXXY)sYXXXXXXYYYXX=(sYXXXXXXYYYXX<<1)+1;
			  do{*sYXXXXXXYXXXY=sYXXXXXXXXYXY[*sYXXXXXXYXXXY]+sYXXXXXXYYXYX[(*sYXXXXXXYXXYX)++];
			  if(*sYXXXXXXYXXYX>=sYXXXXXXYXXXX){*sYXXXXXXYXXYX=0;*sYXXXXXXYXXXY=*sYXXXXXXYXXXY+sYXXXXXXYXXXX;}sYXXXXXXYYYXY=sYXXXXXXYYYXX&*sYXXXXXXYXXXY;
			  if(++sYXXXXXXYYXYY>11)sYXXXXXXYYYXY%=sYXXXXXXYYXXY;}while(sYXXXXXXYYYXY>sYXXXXXXYYXXY);
			  return (unsigned char)sYXXXXXXYYYXY;
		   }
			inline void sYXXXXXXYYYYX(){unsigned char sYXXXXXXYYYYY=sYXXXXXXXXYXY[sYXXXXXXYYXXX];sYXXXXXXYXYXY=sYXXXXXXYXYXY+sYXXXXXXXXYXY[sYXXXXXXYXYXX++];sYXXXXXXXXYXY[sYXXXXXXYYXXX]=sYXXXXXXXXYXY[sYXXXXXXYXYXY];sYXXXXXXXXYXY[sYXXXXXXYXYXY]=sYXXXXXXXXYXY[sYXXXXXXYXYYY];sYXXXXXXXXYXY[sYXXXXXXYXYYY]=sYXXXXXXXXYXY[sYXXXXXXYXYXX];sYXXXXXXXXYXY[sYXXXXXXYXYXX]=sYXXXXXXYYYYY;sYXXXXXXYXYYX=sYXXXXXXYXYYX+sYXXXXXXXXYXY[sYXXXXXXYYYYY];}
			inline unsigned char sYXXXXXXXXYYY(unsigned char sYXXXXXYXXXXX){sYXXXXXXYYYYX();sYXXXXXXYXYYY=sYXXXXXYXXXXX^sYXXXXXXXXYXY[(sYXXXXXXXXYXY[sYXXXXXXYXYXY]+sYXXXXXXXXYXY[sYXXXXXXYXYXX])&0xFF]^sYXXXXXXXXYXY[sYXXXXXXXXYXY[(sYXXXXXXXXYXY[sYXXXXXXYXYYY]+sYXXXXXXXXYXY[sYXXXXXXYYXXX]+sYXXXXXXXXYXY[sYXXXXXXYXYYX])&0xFF]];sYXXXXXXYYXXX=sYXXXXXYXXXXX;return sYXXXXXXYXYYY;}
		protected:
				unsigned char sYXXXXXXXXYXY[256],sYXXXXXXYXYXX,sYXXXXXXYXYXY,sYXXXXXXYXYYX,sYXXXXXXYXYYY,sYXXXXXXYYXXX;    
	};
};
#endif
