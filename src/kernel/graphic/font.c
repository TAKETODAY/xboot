/*
 * kernel/graphic/font.c
 *
 * Copyright(c) 2007-2019 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <ft2build.h>
#include FT_FREETYPE_H
#include <shash.h>
#include <string.h>
#include <graphic/font.h>

struct font_context_t * font_context_alloc(void)
{
	struct font_context_t * ctx;

	ctx = malloc(sizeof(struct font_context_t));
	if(!ctx)
		return NULL;
	FT_Init_FreeType((FT_Library *)&ctx->library);
	ctx->map = hmap_alloc(0);
	return ctx;
}

static void font_callback(const char * key, void * value)
{
	if(value)
		FT_Done_Face((FT_Face)value);
}

void font_context_free(struct font_context_t * ctx)
{
	if(!ctx)
	{
		hmap_walk(ctx->map, font_callback);
		hmap_free(ctx->map);
		FT_Done_FreeType((FT_Library)ctx->library);
	}
}

void * font_search(struct font_context_t * ctx, const char * family)
{
	const char * path;
	FT_Face font;

	if(ctx)
	{
		font = hmap_search(ctx->map, family);
		if(font)
			return font;
		switch(shash(family))
		{
		case 0x3e518f77: /* "regular" */
			path = "/framework/assets/fonts/Roboto-Regular.ttf";
			break;
		case 0x0536d35b: /* "italic" */
			path = "/framework/assets/fonts/Roboto-Italic.ttf";
			break;
		case 0x7c94b326: /* "bold" */
			path = "/framework/assets/fonts/Roboto-Bold.ttf";
			break;
		case 0xad0768e9: /* "bold-italic" */
			path = "/framework/assets/fonts/Roboto-BoldItalic.ttf";
			break;
		default:
			path = "/framework/assets/fonts/Roboto-Regular.ttf";
			break;
		}
		if(FT_New_Face((FT_Library)ctx->library, path, 0, &font) == 0)
		{
			FT_Select_Charmap(font, FT_ENCODING_UNICODE);
			hmap_add(ctx->map, family, font);
			return font;
		}
	}
	return NULL;
}