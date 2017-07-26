











#include "inc.h"
#include "texture.h"
#include "../gui/gui.h"
#include "utils.h"
#include "debug.h"
#include "../app/appmain.h"

gltex g_tex[TEXTURES];
vector g_texload; /* textoload */

int g_lastloadtex = -1;

dbool g_hidetexerr = dfalse;
dbool g_usepalette = dfalse;
int g_savebitdepth = 8;


void ltexinit(texdata *lt)
{
	lt->data = NULL;
}

void ltexfree(texdata *lt)
{
	free(lt->data);
	lt->data = NULL;
}

void texinit(gltex *tex)
{
	tex->full = NULL;
	tex->loaded = dfalse;
}

void texfree(gltex *tex)
{
	free(tex->full);
	tex->full = NULL;
	tex->loaded = dfalse;
}

texdata* loadjpg(const char *file)
{
	texdata *ltex = NULL;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE* fp;
	int len;
	unsigned char* raw;
	int rowspan;
	int i;
	unsigned char** rowptr;
	int rowsread;

	ltex = (texdata*)malloc(sizeof(texdata));

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	fp = fopen(file, "rb");

	if (!fp)
	{
		free(ltex);
		fprintf(g_applog, "Error opening jpeg %s \r\n", file);
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	raw = (unsigned char*)malloc(len);
	fseek(fp, 0, SEEK_SET);
	fread(raw, 1, len, fp);
	fclose(fp);

	// the jpeg_stdio_src alternative func, which is also included in IJG's lib.
	jpeg_mem_src(&cinfo, raw, len);

	jpeg_read_header(&cinfo, TRUE);

	jpeg_start_decompress(&cinfo);

	ltex->channels = cinfo.num_components;
	ltex->sizex    = cinfo.image_width;
	ltex->sizey    = cinfo.image_height;

	rowspan = cinfo.image_width * cinfo.num_components;

	ltex->data = ((unsigned char*)malloc(sizeof(unsigned char)*rowspan*ltex->sizey));

	rowptr = (unsigned char**)malloc(sizeof(unsigned char*)*ltex->sizey);

	for (i = 0; i < ltex->sizey; i++)
		rowptr[i] = &(ltex->data[i * rowspan]);

	rowsread = 0;

	while (cinfo.output_scanline < cinfo.output_height)
		rowsread += jpeg_read_scanlines(&cinfo, &rowptr[rowsread], cinfo.output_height - rowsread);

	free(rowptr);

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	free(raw);

	return ltex;
}

static FILE* loadpngfp = NULL;

void png_file_read(png_structp png_ptr, png_bytep data, png_size_t length) 
{
	fread(data, 1, length, loadpngfp);
}

texdata *loadpng(const char *file)
{
	texdata *ltex = NULL;
	//header for testing if it is a png
	png_byte header[8];
	int is_png;
	png_structp png_ptr;
	png_infop info_ptr;
	png_infop end_info;
	int i;
	
	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;
	png_bytep trans_alpha = NULL;
	int num_trans = 0;
	png_color_16p trans_color = NULL;
	int row_bytes;
	png_bytep *row_pointers;

	loadpngfp = fopen(file, "rb");

	if(!loadpngfp)
	{
		return NULL;
	}

	fread(header, 8, 1, loadpngfp);
	//g_src.read((void*)header, 8);

	//test if png
	is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) 
	{
		fprintf(g_applog,
			"Not a png file : %s %d,%d,%d,%d,%d,%d,%d,%d \r\n", file, 
			(int)header[0], (int)header[1], (int)header[2], (int)header[3], 
			(int)header[4], (int)header[5], (int)header[6], (int)header[7]);
		fclose(loadpngfp);
		return NULL;
	}

	//create png struct
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) 
	{
		fprintf(g_applog, "Unable to create png struct : %s \r\n", file);
		fclose(loadpngfp);
		return NULL;
	}

	//create png info struct
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) 
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fprintf(g_applog, "Unable to create png info : %s \r\n", file);
		fclose(loadpngfp);
		return NULL;
	}

	//create png info struct
	end_info = png_create_info_struct(png_ptr);
	if (!end_info) 
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		fprintf(g_applog, "Unable to create png end info : %s \r\n", file);
		fclose(loadpngfp);
		return NULL;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr))) 
	{
		fprintf(g_applog, "Error during setjmp : %s \r\n", file);
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(loadpngfp);
		return NULL;
	}

	png_set_read_fn(png_ptr, NULL, png_file_read);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);

	//	dbool alphaFlag;

	ltex = (texdata*)malloc(sizeof(texdata));
	ltexinit(ltex);

	ltex->sizex = twidth;
	ltex->sizey = theight;

	switch( color_type )
	{
	case PNG_COLOR_TYPE_RGBA:
		ltex->channels = 4;
		break;
	case PNG_COLOR_TYPE_RGB:
		ltex->channels = 3;
		break;
	case PNG_COLOR_TYPE_PALETTE:
		{
			png_set_palette_to_rgb(png_ptr);
			ltex->channels = 3;
#if 0
			png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);
			if (trans_alpha != NULL)
				alphaFlag = dtrue;
			else
				alphaFlag = dfalse;
			if(alphaFlag)
				ltex->channels = 4;
#endif

			//if(alphaFlag)
			if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
			{
				ltex->channels = 4;
				png_set_tRNS_to_alpha(png_ptr);
			}
		}
		break;
	default:
		fprintf(g_applog,
			"%s color type %d not supported \r\n", file, (int)png_get_color_type(png_ptr, info_ptr));

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		ltexfree(ltex);
		free(ltex);
		fclose(loadpngfp);
		return NULL;
	}

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	row_bytes = png_get_rowbytes(png_ptr, info_ptr);

	ltex->data = (unsigned char*) malloc(row_bytes * ltex->sizey);

	// Allocate the image_data as a big block, to be given to opengl
	if(!ltex->data)
	{
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fprintf(g_applog, "Unable to allocate image_data while loading %s \r\n", file);
		ltexfree(ltex);
		free(ltex);
		fclose(loadpngfp);
		return NULL;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	row_pointers = (png_bytep*)malloc( sizeof(png_bytep)*ltex->sizey );
	if (!row_pointers) 
	{
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		ltexfree(ltex);
		free(ltex->data);
		fprintf(g_applog, "Unable to allocate row_pointer while loading %s \r\n", file);
		fclose(loadpngfp);
		return NULL;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (i = 0; i < ltex->sizey; ++i)
		row_pointers[i] = ltex->data + i * row_bytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	free(row_pointers);
	fclose(loadpngfp);

	return ltex;
}

dbool findtex(unsigned int *ti, const char* relative)
{
	char c[DMD_MAX_PATH+1];
	gltex* t;
	int i;

	strcpy(c, relative);
	corslash(c);

	for(i=0; i<TEXTURES; ++i)
	{
		t = g_tex+i;

		if(t->loaded && strcmp(t->full, c) == 0)
		{
			*ti = i;
			return dtrue;
		}
	}

	return dfalse;
}

void freetex(int i)
{
	gltex* t;

	if(i < 0)
		return;

	t = g_tex+i;

	if(t->loaded)
	{
		t->loaded = dfalse;
		glDeleteTextures(1, &t->texname);
	}
}

int newtex()
{
	gltex *t;
	for(t=g_tex; t<g_tex+TEXTURES; ++t)
	{
		if(!t->loaded)
			return (t-g_tex);
	}
	return -1;
}

dbool regtex(unsigned int texture, 
				 const char* relative, 
				 dbool transp, 
				 dbool clamp, 
				 dbool mipmaps, 
				 unsigned int* texin,
				 dbool reload,
				 int sizex, int sizey)
{
	char c[DMD_MAX_PATH+1];
	gltex* t;

	strcpy(c, relative);
	corslash(c);

	if(!reload)
	{
		*texin = newtex();

		if((int)*texin < 0)
		{
			texin = 0;	// Give a harmless texture index
			return dfalse;
		}
	}

	t = g_tex+*texin;
	t->loaded = dtrue;
	t->full = c;
	t->texname = texture;
	t->width = sizex;
	t->height = sizey;
	t->transp = transp;
	t->clamp = clamp;
	t->mipmaps = mipmaps;

	return dtrue;
}

void freetexs()
{
	int i;

	for(i=0; i<TEXTURES; i++)
	{
		if(!g_tex[i].loaded)
			continue;

		glDeleteTextures(1, &g_tex[i].texname);
		g_tex[i].loaded = dfalse;	// Needed to reload textures. EDIT: not.
	}
}

void findtexext(char *relative)
{
	char jp[DMD_MAX_PATH] = {0};
	char pp[DMD_MAX_PATH] = {0};
	FILE *fp = NULL;

	fullpath("", jp);

	strcat(jp, relative);
	strcpy(pp, jp);

	strcat(jp, ".jpg");
	strcat(pp, ".png");

	if((fp = fopen(jp, "rb")) != NULL)
	{
		fclose(fp);
		strcat(relative, ".jpg");
		return;
	}

	if((fp = fopen(pp, "rb")) != NULL)
	{
		fclose(fp);
		strcat(relative, ".png");
		return;
	}
}

dbool loadqtex()
{
	textoload* t;

	if(g_lastloadtex+1 < (int)g_texload.total)
	{
		t = (textoload*)vget(&g_texload, g_lastloadtex+1);
		setstat(t->relative);
	}

	CHECKGL();

	if(g_lastloadtex >= 0)
	{
		t = (textoload*)vget(&g_texload, g_lastloadtex);
		if(t->reload)
			createtex(&t->texin, t->relative, t->clamp, t->mipmaps, t->reload);
		else
			createtex(t->ptexin, t->relative, t->clamp, t->mipmaps, t->reload);
	}

	g_lastloadtex ++;

	if(g_lastloadtex >= (int)g_texload.total)
	{
		return dfalse;	// Done loading all textures
	}

	return dtrue;	// Not finished loading textures
}

void queuetex(unsigned int* texin, const char* relative, dbool clamp, dbool mipmaps)
{
	textoload toload;
	toload.ptexin = texin;
	strcpy(toload.relative, relative);
	toload.clamp = clamp;
	toload.reload = dfalse;
	toload.mipmaps = mipmaps;

	vpub(&g_texload, &toload);
}

//"requeue texi"
void reloadtex(unsigned int texin, const char* relative, dbool clamp, dbool mipmaps)
{
	textoload toload;
	toload.texin = texin;
	strcpy(toload.relative, relative);
	toload.clamp = clamp;
	toload.reload = dtrue;
	toload.mipmaps = mipmaps;

	vpub(&g_texload, &toload);
}

//"reload texs"
void reloadtexs()
{
	gltex* t;
	char rel[DMD_MAX_PATH+1];
	for(t=g_tex; t<g_tex+TEXTURES; ++t)
	{
		if(!t->loaded)
			continue;

		makerel(t->full, rel);
		reloadtex((t-g_tex), rel, t->clamp, t->mipmaps);
	}
}

texdata* loadtex(const char* full)
{
	char low[DMD_MAX_PATH+1];
	int i;

	for(i=0; full[i]; ++i)
		low[i] = (char)tolower((int)full[i]);

	low[i] = 0;

	if( (i>=3+1 && strstr(low+i-3-1, ".jpg")) ||
		(i>=4+1 && strstr(low+i-4-1, ".jpeg")) ||
		(i>=3+1 && strstr(low+i-3-1, ".jpe")) )
	{
		return loadjpg(full);
	}
	else if(i>=3+1 && strstr(low+i-3-1, ".png"))
	{
		return loadpng(full);
	}

	fprintf(g_applog, "Unrecognized texture file extension: %s. Only .jpg .png are accepted. \r\n", low);

	return NULL;
}

dbool createtex2(texdata* pim, unsigned int* texname, dbool clamp, dbool mipmaps)
{
	int infm;
	int textype;
	dbool transp;

	if(!pim)
		return dfalse;
	if(!pim->data)
		return dfalse;

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, texname);

	CHECKGL();
	// This sets the alignment requirements for the start of each pixel row in memory.
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	CHECKGL();

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, *texname);

	CHECKGL();
	// Assume that the texture is a 24 bit RGB texture (We convert 16-bit ones to 24-bit)
	infm = GL_RGB8;
	textype = GL_RGB;
	transp = dfalse;

	// If the image is 32-bit (4 channels), then we need to specify GL_RGBA for an alpha
	if(pim->channels == 4)
	{
		infm = GL_RGBA8;
		textype = GL_RGBA;
		transp = dtrue;
	}
	//grayscale
	else if(pim->channels == 1)
	{
		infm = GL_ALPHA8;
		textype = GL_ALPHA;
	}

	CHECKGL();

#if 0	//old

	if(mipmaps)
	{
#if 0
		glEnable(GL_TEXTURE_2D);	// ATI fix
		// Option 1: with mipmaps

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if(clamp)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
		//glTexImage2D(GL_TEXTURE_2D, 0, textype, pim->sizex, pim->sizey, 0, textype, GL_UNSIGNED_BYTE, pim->data);
		//glGenerateMipmap(GL_TEXTURE_2D);
		gluBuild2DMipmaps(GL_TEXTURE_2D, textype, pim->sizex, pim->sizey, textype, GL_UNSIGNED_BYTE, pim->data);

		glerr(__FILE__, __LINE__);
#else
		//if(transp)
		if(dtrue)
		{
			//linear filter, better for thin details with transparency

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			if(clamp)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}

			glTexImage2D(GL_TEXTURE_2D, 0, textype, pim->sizex, pim->sizey, 0, textype, GL_UNSIGNED_BYTE, pim->data);

			glerr(__FILE__, __LINE__);
		}
		else
		{
			glEnable(GL_TEXTURE_2D);	// ATI fix
			// Option 1: with mipmaps

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if(clamp)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}

			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
			//glTexImage2D(GL_TEXTURE_2D, 0, textype, pim->sizex, pim->sizey, 0, textype, GL_UNSIGNED_BYTE, pim->data);
			//glGenerateMipmap(GL_TEXTURE_2D);
			gluBuild2DMipmaps(GL_TEXTURE_2D, textype, pim->sizex, pim->sizey, textype, GL_UNSIGNED_BYTE, pim->data);

			glerr(__FILE__, __LINE__);
		}
#endif
	}
	else
	{
		// Option 2: without mipmaps
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		if(clamp)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, textype, pim->sizex, pim->sizey, 0, textype, GL_UNSIGNED_BYTE, pim->data);

		glerr(__FILE__, __LINE__);
	}

	glerr(__FILE__, __LINE__);
#endif


#if 1

	//Log("mipmaps:"<<(int)mipmaps<<" :"<<relative);

	if(mipmaps)
	{
		glEnable(GL_TEXTURE_2D);	// ATI fix
		// Option 1: with mipmaps

		CHECKGL();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		CHECKGL();
#ifdef PLAT_GL14
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
#endif
		CHECKGL();

		if(clamp)
		{
#ifdef PLAT_GL14
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif

#ifdef PLAT_GLES20
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, infm, pim->sizex, pim->sizey, 0, textype, GL_UNSIGNED_BYTE, pim->data);

		glGenerateMipmap(GL_TEXTURE_2D);  //undeclared identifier in xcode!?

		//gluBuild2DMipmaps(GL_TEXTURE_2D, infm, pim->sizex, pim->sizey, textype, GL_UNSIGNED_BYTE, pim->data);  //undeclared identifier in xcode!?

		CHECKGL();
	}
	else
	{
		// Option 2: without mipmaps
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		CHECKGL();

		if(clamp)
		{
#ifdef PLAT_GL14
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif

#ifdef PLAT_GLES20
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, infm, pim->sizex, pim->sizey, 0, textype, GL_UNSIGNED_BYTE, pim->data);

		CHECKGL();
	}

	CHECKGL();
#else
	// Option 3: without mipmaps linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, textype, pim->sizex, pim->sizey, 0, textype, GL_UNSIGNED_BYTE, pim->data);
#endif

	glBindTexture(GL_TEXTURE_2D, -1);
	return dtrue;
}

dbool createtex(unsigned int *texin, const char* relative, dbool clamp, dbool mipmaps, dbool reload)
{
	// Define a pointer to a texdata
	texdata *pim = NULL;
	char full[DMD_MAX_PATH+1];
	unsigned int texname;
	dbool transp;

	CHECKGL();

	if(!relative)
		return dfalse;

	if(!reload)
		if(findtex(texin, relative))
			return dtrue;

	fullpath(relative, full);
	pim = loadtex(full);

	// Make sure valid image data was given to pim, otherwise return dfalse
	if(pim == NULL)
	{
		fprintf(g_applog, "Failed to load %s \r\n", relative);

		if(!reload)
			*texin = 0;	// Give a harmless texture index

		return dfalse;
	}

	if(!createtex2(pim, &texname, clamp, mipmaps))
	{
		ltexfree(pim);
		free(pim);								// Free the image structure
		return dfalse;
	}

	transp = dfalse;

	if(pim->channels == 4)
	{
		transp = dtrue;
	}

	regtex(texname, relative, transp, clamp, mipmaps, texin, reload, pim->sizex, pim->sizey);

	if(pim)
	{
		ltexfree(pim);
		free(pim);								// Free the image structure
		fprintf(g_applog, "%s \r\n", relative);
	}

	// Return a success
	return dtrue;
}

void reqxs()
{
	gltex* t;

	freetexs();

	for(t=g_tex; t<g_tex+TEXTURES; ++t)
	{
		if(t->loaded)
			reloadtex((t-g_tex), t->full, t->clamp, t->mipmaps);
	}

	//Loadpart3fs();
	//LoadProjectiles();
	//LoadTerraintexs();
	//LoadUnitSprites();
	//BSprites();
}

void diffpathj(const char* basepath, char* diffpath)
{
	strcpy(diffpath, basepath);
	//stripext(diffpath);
	strcat(diffpath, ".jpg");
}

void diffpathp(const char* basepath, char* diffpath)
{
	strcpy(diffpath, basepath);
	//stripext(diffpath);
	strcat(diffpath, ".png");
}

void specpathj(const char* basepath, char* specpath)
{
	strcpy(specpath, basepath);
	//stripext(specpath);
	strcat(specpath, ".spec.jpg");
}

void normpathj(const char* basepath, char* normpath)
{
	strcpy(normpath, basepath);
	//stripext(normpath);
	strcat(normpath, ".norm.jpg");
}

void ownpathp(const char* basepath, char* ownpath)
{
	strcpy(ownpath, basepath);
	//stripext(ownpath);
	strcat(ownpath, ".team.png");
}

void alloctex(texdata* empty, int width, int height, int channels)
{
	empty->data = (unsigned char*)malloc(width * height * channels * sizeof(unsigned char));
	empty->sizex = width;
	empty->sizey = height;
	empty->channels = channels;
	NOMEM(empty->data);
}

void blittex(texdata* src, texdata* dest, v2i pos)
{
	int x, y;
	int srcpx, destpx;

	if(src == NULL || src->data == NULL)
		return;

	for(x=0; x<src->sizex; ++x)
	{
		if(x+pos.x < 0)
			continue;

		if(x+pos.x >= dest->sizex)
			continue;

		for(y=0; y<src->sizey; ++y)
		{
			if(y+pos.y < 0)
				continue;

			if(y+pos.y >= dest->sizey)
				continue;

			srcpx = x*src->channels + y*src->channels*src->sizex;
			destpx = (x+pos.x)*dest->channels + (y+pos.y)*dest->channels*dest->sizex;

			dest->data[destpx + 0] = src->data[srcpx + 0];
			dest->data[destpx + 1] = src->data[srcpx + 1];
			dest->data[destpx + 2] = src->data[srcpx + 2];

			if(dest->channels > 3)
			{
				if(src->channels <= 3)
					dest->data[destpx + 3] = 255;
				else
					dest->data[destpx + 3] = src->data[srcpx + 3];
			}
		}
	}
}

static vector my_buffer; /* JOCTET */
#define BLOCK_SIZE 16384

void my_init_destination(j_compress_ptr cinfo)
{
	vinit(&my_buffer, sizeof(JOCTET));
	vresize(&my_buffer, BLOCK_SIZE);
	cinfo->dest->next_output_byte = &my_buffer.items[0];
	cinfo->dest->free_in_buffer = my_buffer.total;
}

boolean my_empty_output_buffer(j_compress_ptr cinfo)
{
	size_t oldsize = my_buffer.total;
	vresize(&my_buffer, oldsize + BLOCK_SIZE);
	cinfo->dest->next_output_byte = &my_buffer.items[oldsize];
	cinfo->dest->free_in_buffer = my_buffer.total - oldsize;
	return dtrue;
}

void my_term_destination(j_compress_ptr cinfo)
{
	vresize(&my_buffer, my_buffer.total - cinfo->dest->free_in_buffer);
}


dbool savejpg(const char* full, texdata* image, float quality)
{
	FILE *ouf;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr       jerr;
	JSAMPROW row_pointer;
	int row_stride = image->sizex * 3;

	if ((ouf = fopen(full, "wb")) == NULL)
	{
		return dfalse;
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, NULL);
	cinfo.dest->init_destination = &my_init_destination;
	cinfo.dest->empty_output_buffer = &my_empty_output_buffer;
	cinfo.dest->term_destination = &my_term_destination;

	cinfo.image_width      = image->sizex;
	cinfo.image_height     = image->sizey;
	cinfo.input_components = 3;
	cinfo.in_color_space   = JCS_RGB;

	jpeg_set_defaults(&cinfo);

	cinfo.dest->init_destination = &my_init_destination;
	cinfo.dest->empty_output_buffer = &my_empty_output_buffer;
	cinfo.dest->term_destination = &my_term_destination;

	/*set the quality [0..100]  */
	//have to use TRUE instead of dtrue or else doesn't work
	//in xcode which requires conversion to custom type boolean
	//jpeg_set_quality (&cinfo, 100*quality, TRUE);
	//jpeg_start_compress(&cinfo, TRUE);
	jpeg_set_quality (&cinfo, (int)(100*quality), (boolean)TRUE);
	jpeg_start_compress(&cinfo, (boolean)TRUE);

	row_stride = image->sizex * 3;

	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer = (JSAMPROW) &image->data[cinfo.next_scanline*row_stride];
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);

	fwrite(&my_buffer.items[0], my_buffer.total, 1, ouf);
	fclose(ouf);

	jpeg_destroy_compress(&cinfo);

	vfree(&my_buffer);

	return dtrue;
}

static FILE* savepngfp = NULL;

void png_file_write(png_structp png_ptr, png_bytep data, png_size_t length) 
{
	fwrite(data, 1, length, savepngfp);
}

void png_file_flush(png_structp png_ptr)
{
	fflush(savepngfp);
}

//"palettize"
//only for palette+alpha
void paz(png_color_16 Colors[PNG_MAX_PALETTE_LENGTH],
			   png_byte Trans[PNG_MAX_PALETTE_LENGTH],
			   int* npal,
			   texdata* lt,
			   png_colorp palette)
{
	int *closest;
	int mentions[PNG_MAX_PALETTE_LENGTH];
	int x, y;
	int nearpi;
	int neard ;
	int pi=0;
	int d; 
	int avga[PNG_MAX_PALETTE_LENGTH];

	*npal = 0;

	//1	
	closest = (int*)malloc( sizeof(int) * lt->sizex * lt->sizey );
	memset(closest, 0, lt->sizex * lt->sizey * sizeof(int));
	memset(mentions, 0, PNG_MAX_PALETTE_LENGTH * sizeof(int));

	for(x=0; x<lt->sizex; ++x)
	{
		//tryout:
		for(y=0; y<lt->sizey; ++y)
		{
			nearpi = -1;
			neard = -1;

			for(pi=0; pi<PNG_MAX_PALETTE_LENGTH; ++pi)
			{
				d = iabs((int)(palette[pi].red-lt->data[lt->channels*(x+y*lt->sizex)+0])) * 
					iabs((int)(palette[pi].green-lt->data[lt->channels*(x+y*lt->sizex)+1])) * 
					iabs((int)(palette[pi].blue-lt->data[lt->channels*(x+y*lt->sizex)+2]));

				if(nearpi < 0)
					nearpi = pi;
				if(neard < 0)
					neard = d;

				//if(mentions[pi] <= mentions[nearpi] &&
				//	d >= neard)
				if(d < neard)
				{
					nearpi = pi;
					neard = d;
				}
			}

			for(pi=0; pi<PNG_MAX_PALETTE_LENGTH; ++pi)
			{
				d = iabs((int)(palette[pi].red-lt->data[lt->channels*(x+y*lt->sizex)+0])) * 
					iabs((int)(palette[pi].green-lt->data[lt->channels*(x+y*lt->sizex)+1])) * 
					iabs((int)(palette[pi].blue-lt->data[lt->channels*(x+y*lt->sizex)+2]));

				if(nearpi < 0)
					nearpi = pi;
				if(neard < 0)
					neard = d;

				if(mentions[pi] <= mentions[nearpi] &&
					d >= neard)
				{
					nearpi = pi;
					neard = d;
					if(d > 0)
						mentions[pi] = 0;
					//x=0;
					//y=0;
					//goto tryout;
				}
			}

			closest[(x+y*lt->sizex)] = nearpi;
			mentions[nearpi]++;
			palette[nearpi].red = lt->data[lt->channels*(x+y*lt->sizex)+0];
			palette[nearpi].green = lt->data[lt->channels*(x+y*lt->sizex)+1];
			palette[nearpi].blue = lt->data[lt->channels*(x+y*lt->sizex)+2];
		}
	}

	//2
	//texdata closest;
	//alloctex(&closest, lt->sizex, lt->sizey, lt->channels);
	//memset(closest.data, 0, lt->sizex * lt->sizey * lt->channels);
	memset(closest, 0, lt->sizex * lt->sizey * sizeof(int));
	memset(mentions, 0, PNG_MAX_PALETTE_LENGTH * sizeof(int));

	for(x=0; x<lt->sizex; ++x)
	{
		for(y=0; y<lt->sizey; ++y)
		{
			nearpi = -1;
			neard = -1;
			for(pi=0; pi<PNG_MAX_PALETTE_LENGTH; ++pi)
			{
				d = iabs((int)(palette[pi].red-lt->data[lt->channels*(x+y*lt->sizex)+0])) * 
					iabs((int)(palette[pi].green-lt->data[lt->channels*(x+y*lt->sizex)+1])) * 
					iabs((int)(palette[pi].blue-lt->data[lt->channels*(x+y*lt->sizex)+2]));

				if(nearpi < 0 || neard > d)
				{
					nearpi = pi;
					neard = d;
				}
			}

			closest[(x+y*lt->sizex)] = nearpi;
			mentions[nearpi]++;
			//lt->data[lt->channels*(x+y*lt->sizex)+0] = palette[pi].red;
			//lt->data[lt->channels*(x+y*lt->sizex)+1] = palette[pi].green;
			//lt->data[lt->channels*(x+y*lt->sizex)+2] = palette[pi].blue;
		}
	}

	memset(avga, 0, sizeof(int)*PNG_MAX_PALETTE_LENGTH);

	for(x=0; x<lt->sizex; ++x)
	{
		for(y=0; y<lt->sizey; ++y)
		{
			nearpi = closest[(x+y*lt->sizex)];
			avga[nearpi] += lt->data[(x+y*lt->sizex)*lt->channels] *2000/255;
		}
	}

	for(pi=0; pi<PNG_MAX_PALETTE_LENGTH; ++pi)
		avga[pi] /= maxi(1, 2000 * mentions[pi] / 255);

	for(pi=0; pi<PNG_MAX_PALETTE_LENGTH; ++pi)
		Trans[pi] = maxi(0, mini(255, avga[pi]));

	free(closest);

	*npal = PNG_MAX_PALETTE_LENGTH;
}

dbool savepng(const char* full, texdata* image)
{
	//FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	//png_colorp palette;
	int color_type;
	int bit_depth;
	png_colorp palette;	
	png_color_16 Colors[PNG_MAX_PALETTE_LENGTH];
	png_byte Trans[PNG_MAX_PALETTE_LENGTH];
	int npal;
	png_bytep* row_pointers;
	int y;
	png_text text_ptr[1];
	char srcstr[123];

	/* Open the file */
	savepngfp = fopen(full, "wb");
	if (savepngfp == NULL)
		return dfalse;

	/* Create and initialize the png_struct with the desired error handler
	* functions.  If you want to use the default stderr and longjump method,
	* you can supply NULL for the last three parameters.  We also check that
	* the library version is compatible with the one used at compile time,
	* in case we are using dynamically linked libraries.  REQUIRED.
	*/
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
		(png_voidp) NULL, NULL, NULL);

	if (png_ptr == NULL)
	{
		fclose(savepngfp);
		return dfalse;
	}

	/* Allocate/initialize the image information data.  REQUIRED */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(savepngfp);
		png_destroy_write_struct(&png_ptr,  NULL);
		return dfalse;
	}

	color_type = PNG_COLOR_TYPE_RGB;

	if(image->channels == 4)
		color_type = PNG_COLOR_TYPE_RGBA;
	if(image->channels == 1)
		color_type = PNG_COLOR_TYPE_GRAY;

	if(g_usepalette)
	{
		color_type = PNG_COLOR_TYPE_PALETTE;
	}

	bit_depth = 8;

	if(g_savebitdepth == 2 ||
		g_savebitdepth == 4 ||
		g_savebitdepth == 8 ||
		g_savebitdepth == 16)
	{
		bit_depth = g_savebitdepth;
	}
	else if(!g_hidetexerr)
	{
		errm("PNG write_bit_depth", "write_bit_depth must be either 2, 4, 8, or 16. Defaulting to 8.");
	}

	png_set_IHDR(png_ptr, info_ptr, image->sizex, image->sizey, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	/* Set error handling.  REQUIRED if you aren't supplying your own
	* error handling functions in the png_create_write_struct() call.
	*/
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		/* If we get here, we had a problem writing the file */
		fclose(savepngfp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return dfalse;
	}

	if(g_usepalette)
	{
		palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));//4
		if (!palette) {
			errm("PNG error", "Error allocating palette");
			OUTMEM();
			fclose(savepngfp);
			png_destroy_write_struct(&png_ptr, &info_ptr);//
			return dfalse;
		}

		npal = 0;

		paz(Colors, Trans, &npal, image, palette);
		//BGColor[0].index = 0;
		//TrnsColor[0] = 100; //0 = transparent , 255 = opaque
		png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);//12
		//png_set_tRNS(png_ptr, info_ptr, Trans, npal, NULL);//Colors);
	}

	/* One of the following I/O initialization functions is REQUIRED */

	/* Set up the output control if you are using standard C streams */
	//png_init_io(png_ptr, savepngfp);
	png_set_write_fn(png_ptr, NULL, png_file_write, png_file_flush);

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * image->sizey);

	if(!row_pointers)
	{
		OUTMEM();
		return dfalse;
	}

	for (y=0; y<image->sizey; y++)
		row_pointers[y] = (png_byte*)&image->data[y*image->sizex*image->channels];

	sprintf(srcstr, "Rendered using %s Version %d", APPTIT, APPVER);
#ifdef USESTEAM
	strcat(srcstr, " Authorized Steam Build");
#endif
	text_ptr[0].key = "Source";
	text_ptr[0].text = srcstr;
	text_ptr[0].compression = PNG_TEXT_COMPRESSION_NONE;
	// text_ptr[2].compression = PNG_TEXT_COMPRESSION_zTXt;
#ifdef PNG_iTXt_SUPPORTED
	text_ptr[0].lang = NULL;
#endif
	text_ptr[0].lang_key = NULL;

	png_set_text(png_ptr, info_ptr, text_ptr, 1);

	png_write_info(png_ptr, info_ptr);

	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, NULL);

	//for (y=0; y<image->sizey; y++)
	//   free(row_pointers[y]);
	free(row_pointers);


	/* This is the easy way.  Use it if you already have all the
	* image info living in the structure.  You could "|" many
	* PNG_TRANSFORM flags into the png_transforms integer here.
	*/
	//png_write_png(png_ptr, info_ptr, NULL, NULL);

	/* If you png_malloced a palette, free it here (don't free info_ptr->palette,
	* as recommended in versions 1.0.5m and earlier of this example; if
	* libpng mallocs info_ptr->palette, libpng will free it).  If you
	* allocated it with malloc() instead of png_malloc(), use free() instead
	* of png_free().
	*/
	//png_free(png_ptr, palette);
	//palette = NULL;

	/* Similarly, if you png_malloced any data that you passed in with
	* png_set_something(), such as a hist or trans array, free it here,
	* when you can be sure that libpng is through with it.
	*/
	//png_free(png_ptr, trans);
	//trans = NULL;
	/* Whenever you use png_free() it is a good idea to set the pointer to
	* NULL in case your application inadvertently tries to png_free() it
	* again.  When png_free() sees a NULL it returns without action, thus
	* avoiding the double-free security problem.
	*/

	/* Clean up after the write, and free any memory allocated */
	png_destroy_write_struct(&png_ptr, &info_ptr);

	/* Close the file */
	fclose(savepngfp);

	/* That's it */
	return dtrue;
}

//"flip"
void fliptex(texdata* image)
{
	int x;
	int y2;
	int y;
	byte temp[4];
	int stride = image->sizex * image->channels;
	unsigned char *pLine;
	unsigned char *pLine2;

	for(y=0; y<image->sizey/2; ++y)
	{
		y2 = image->sizey - y - 1;

		pLine = &(image->data[stride * y]);
		pLine2 = &(image->data[stride * y2]);

		for(x = 0; x < image->sizex * image->channels; x += image->channels)
		{
			temp[0] = pLine[x + 0];
			temp[1] = pLine[x + 1];
			temp[2] = pLine[x + 2];
			if(image->channels == 4)
				temp[3] = pLine[x + 3];

			pLine[x + 0] = pLine2[x + 0];
			pLine[x + 1] = pLine2[x + 1];
			pLine[x + 2] = pLine2[x + 2];
			if(image->channels == 4)
				pLine[x + 3] = pLine2[x + 3];

			pLine2[x + 0] = temp[0];
			pLine2[x + 1] = temp[1];
			pLine2[x + 2] = temp[2];
			if(image->channels == 4)
				pLine2[x + 3] = temp[3];
		}
	}
}

//"save RAW"
dbool saveraw(const char* full, texdata* image)
{
	FILE* fp = fopen(full, "wb");

	fwrite(image->data, image->sizex*image->sizey*image->channels, 1, fp);

	fclose(fp);

	return dtrue;
}

//"stream RAW"
void straw(FILE* fp, unsigned int* texname, v2i fullsz, v2i srcpos, v2i srcsz, v2i destsz)
{
	int stridex;
	int stridey;
	int i, x, y;
	texdata newtex;

	if(srcpos.x < 0)
		srcpos.x = 0;
	if(srcpos.y < 0)
		srcpos.y = 0;
	if(srcpos.x + srcsz.x > fullsz.x)
		srcsz.x = fullsz.x - srcpos.x;
	if(srcpos.y + srcsz.y > fullsz.y)
		srcsz.y = fullsz.y - srcpos.y;

	stridex = srcsz.x / destsz.x * 3;
	stridey = srcsz.y / destsz.y * 3 * fullsz.x;

	alloctex(&newtex, destsz.x, destsz.y, 3);
	i = 0;
	for(y=srcpos.y; srcpos.y < srcpos.y+srcsz.y; y+=stridey)
	{
		for(x=srcpos.x; srcpos.x < srcpos.x+srcsz.x; x+=stridex)
		{
			fseek(fp, y * 3 * fullsz.x + x * 3, SEEK_SET);
			fread(&newtex.data[i], 3, 1, fp);
			i+=3;
		}
	}

	ltexfree(&newtex);
}

//"extract"
void extract(texdata* original, texdata* empty, int x1, int y1, int x2, int y2)
{
	int x, y;
	int i1, i2;
	int c;

	alloctex(empty, x2-x1, y2-y1, original->channels);

	for(x=x1; x<x2; ++x)
	{
		for(y=y1; y<y2; ++y)
		{
			i1 = (x + y * original->sizex) * original->channels;
			i2 = ((x-x1) + (y-y1) * empty->sizex) * empty->channels;

			for(c=0; c<original->channels; ++c)
				empty->data[i2] = original->data[i1];
		}
	}
}

//TODO deterministic
//"resample"
void resample(texdata* original, texdata* empty, v2i newdim)
{
	double scaleW;
	double scaleH;
	int cy, cx;
	int pixel, nearest;

	if(original == NULL || original->data == NULL || original->sizex <= 0 || original->sizey <= 0)
	{
		empty->data = NULL;

		empty->sizex = 0;
		empty->sizey = 0;

		if(original != NULL)
			empty->channels = original->channels;

		return;
	}

	alloctex(empty, newdim.x, newdim.y, original->channels);

	scaleW =  (double)newdim.x / (double)original->sizex;
	scaleH = (double)newdim.y / (double)original->sizey;

	for(cy = 0; cy < newdim.y; cy++)
	{
		for(cx = 0; cx < newdim.x; cx++)
		{
			pixel = cy * (newdim.x * original->channels) + cx*original->channels;
			nearest =  (int)(cy / scaleH) * original->sizex * original->channels + (int)(cx / scaleW) * original->channels;

			empty->data[pixel    ] =  original->data[nearest    ];
			empty->data[pixel + 1] =  original->data[nearest + 1];
			empty->data[pixel + 2] =  original->data[nearest + 2];

			if(original->channels > 3)
				empty->data[pixel + 3] =  original->data[nearest + 3];
		}
	}
}
