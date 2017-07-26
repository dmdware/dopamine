

#include "utils.h"
#include "../algo/bool.h"

FILE *g_applog = NULL;


void openlog(const char* file, int ver)
{
	char full[DMD_MAX_PATH+1];
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);

	if(g_applog)
		return;

	fullwrite(file, full);
	g_applog = fopen(full, "w");

	if(!g_applog)
		return;

	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	fprintf(g_applog, "%s Version %d \r\n\r\n", buf, ver);
}

/* get a power of 2 number that is big enough to hold 'lowerbound' but does not exceed 2048 */
int max2pow(int lowerbound)
{
	int twopow = 2;

	while( twopow < lowerbound
#if 0
		&& twopow < 2048
#endif
		)
		twopow *= 2;

	return twopow;
}

int maxpow32(int lowerbound)
{
	int twopow = 32;

	while( twopow < lowerbound
#if 0
		&& twopow < 2048
#endif
		)
		twopow *= 2;

	return twopow;
}

unsigned __int64 getticks()
{
	//return time(0);
#ifdef PLAT_WIN
	
	SYSTEMTIME st;
	FILETIME ft;

	//return GetTickCount64();
	GetSystemTime (&st);
	SystemTimeToFileTime(&st, &ft);
	//LARGE_INTEGER lint;
	//lint.HighPart = ft.dwHighDateTime;
    //lint.LowPart = ft.dwLowDateTime;
	//convert from 100-nanosecond intervals to milliseconds
	return (*(unsigned __int64*)&ft)/(10*1000);
#else
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return
    (unsigned __int64)(tv.tv_sec) * 1000 +
    (unsigned __int64)(tv.tv_usec) / 1000;
#endif
}

void corslash(char *s)
{
	while(*s)
	{
		if(*s=='\\')
			*s='/';
		++s;
	}
}

void stripfile(char* in, char* out)
{
	char temp[DMD_MAX_PATH+1], *lastof;
	strcpy(temp, in);
	corslash(temp);

	lastof = strrchr(temp, '/');
	if(!lastof)
	{
		strcpy(out, in);
		return;
	}

	++lastof;
	memcpy(out, in, (lastof-temp)+1);
}

void strippath(char* in, char* out)
{
	char temp[DMD_MAX_PATH+1], *lastof;
	strcpy(temp, in);
	corslash(temp);

	lastof = strrchr(temp, '/');
	if(!lastof)
	{
		strcpy(out, in);
		return;
	}

	++lastof;
	memcpy(out, in+(lastof-temp), strlen(lastof)+1);
}

void stripext(char* in, char* out)
{
	char *lastof;

	lastof = strrchr(in, '.');

	if(!lastof)
	{
		strcpy(out, in);
		return;
	}

	if(lastof == in)
	{
		strcpy(out, "");
		return;
	}

	--lastof;
	memcpy(out, in, (lastof-in));
	out[(lastof-in)+1] = 0;
}

void makerel(const char* full, char* rel)
{
	char temp[DMD_MAX_PATH+1];
	char exe[DMD_MAX_PATH+1];
	char *pos;
	strcpy(temp, full);
	corslash(temp);

	exepath(exe);
	corslash(exe);

	pos = strstr(temp, exe);

	if(!pos)
	{
		strcpy(rel, temp);
		return;
	}

	memcpy(rel, pos+strlen(exe), strlen(pos)-strlen(exe)+1);
}

void fullwrite(const char* filename, char* full)
{
#ifdef PLAT_IOS
	/*
	 char exepath[SPE_MAX_PATH+1];
	 GetModuleFileName(NULL, exepath, SPE_MAX_PATH);
	 string path = stripfile(exepath);
	 
	 //char full[SPE_MAX_PATH+1];
	 sprintf(full, "%s", path.c_str());
	 
	 char c = full[ strlen(full)-1 ];
	 if(c != '\\' && c != '/')
	 strcat(full, "\\");
	 
	 strcat(full, filename);*/
	
	//NSString *path = [NSHomeDirectory() stringByAppendingPathComponent:@"Documents/"];
	//NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
	//NSString *path = [paths objectAtIndex:0];
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	NSString *path = [paths objectAtIndex:0];
	sprintf(full, "%s/%s", [path cStringUsingEncoding:NSUTF8StringEncoding], filename);
	
	//NSLog(@"full write %s", full);
#else
	
	fullpath(filename, full);
#endif
}

void fullpath(const char* file, char* full)
{
	char exe[DMD_MAX_PATH+1];
	char path[DMD_MAX_PATH+1];
	char c;
	exepath(exe);
	stripfile(exe, path);

	//char full[DMD_MAX_PATH+1];
	strcpy(full, path);

	c = full[ strlen(full)-1 ];
	if(c != '\\' && c != '/')
		strcat(full, "\\");
	//strcat(full, "/");

	strcat(full, file);
	corslash(full);
}

void exepath(char* exe)
{
#ifndef PLAT_IOS
#if 0
#ifdef PLAT_WIN
	//char buffer[WF_MAX_PATH+1];
	GetModuleFileName(NULL, exepath, WF_MAX_PATH+1);
	//std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
	//std::string strexepath = std::string( buffer ).substr( 0, pos);
	//strcpy(exepath, strexepath.c_str());
#else
	char szTmp[32];
	//char buffer[WF_MAX_PATH+1];
	sprintf(szTmp, "/proc/%d/exe", getpid());
	int bytes = std::min((int)readlink(szTmp, exepath, WF_MAX_PATH+1), WF_MAX_PATH);
	if(bytes >= 0)
		exepath[bytes] = '\0';
	//std::string strexepath = stripfile(std::string(buffer));
	//strcpy(exepath, strexepath.c_str());
#endif
#else
	char *base = SDL_GetBasePath();
#if 0
	if (base_path) {
        data_path = SDL_strdup(base_path);
        SDL_free(base_path);
    } else {
        data_path = SDL_strdup("./");
    }
#endif
	if(base)
	{
		strcpy(exe, base);
		SDL_free(base);
	}
#endif
#endif
	
#ifdef PLAT_IOS
	char *base = SDL_GetBasePath();
	if(base)
	{
		strcpy(exe, base);
		SDL_free(base);
	}
	strcat(exe, "testfolder/");
#endif
}

void makedir(const char* full)
{
#ifdef PLAT_WIN
	CreateDirectory(full, NULL);
#else
	mkdir(full, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

void errm(const char* title, const char* m)
{
	fprintf(g_applog, "%s: %s\r\n", title, m);
	fflush(g_applog);
	//SDL_ShowCursor(dtrue);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, m, NULL);
	//SDL_ShowCursor(dfalse);
}

void infm(const char* title, const char* m)
{
	//SDL_ShowCursor(dtrue);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, m, NULL);
	//SDL_ShowCursor(dfalse);
}

void warm(const char* title, const char* m)
{
	//SDL_ShowCursor(dtrue);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, title, m, NULL);
	//SDL_ShowCursor(dfalse);
}

float maxf(const float a, const float b)
{
	return (((a)>(b))?(a):(b));
}

float minf(const float a, const float b)
{
	return (((a)<(b))?(a):(b));
}

int maxi(const int x, const int y)
{
	// https://graphics.stanford.edu/~seander/bithacks.html#IntegerMinOrMax
	return x ^ ((x ^ y) & -(x < y)); // max(x, y)
}

int mini(const int x, const int y)
{
	// https://graphics.stanford.edu/~seander/bithacks.html#IntegerMinOrMax
	return y ^ ((x ^ y) & -(x < y)); // min(x, y)
}

int iabs(int v)
{
	// https://graphics.stanford.edu/~seander/bithacks.html
	int const mask = v >> ( (sizeof(int) * CHAR_BIT) - 1 );
	return (v + mask) ^ mask;
}

int isign(int x)
{
	//https://graphics.stanford.edu/~seander/bithacks.html#CopyIntegerSign
	return +1 | (x >> (sizeof(int) * CHAR_BIT - 1));
}

double fsign(double x)
{
	return x / fabs(x);
}

//deterministic ceil
int iceil(const int num, const int denom)
{
	int div, mul, rem;

	if(denom  == 0)
		return 0;

	div = num / denom;
	mul = div * denom;
	rem = num - mul;

	if(rem > 0)
		div += 1;

	return div;
}