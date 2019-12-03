/* Start Header -------------------------------------------------------
Copyright (C) 2019
DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute
of Technology is prohibited.
Language:<C++>
Platform:<windows 10>
Project:<GAM 541, Void Engine>
Author:<Jin Fan>
-End Header --------------------------------------------------------*/
#include "pch.h"
#include "System/TextSystem.h"
#include "Singleton/ResourceManager.h"
#include "Resource/TextureResource.h"
#include "Component/TextComponent.h"
#include "Entity.h"
#include "Enums.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#define FONT_DIRECTION "Bin/Fonts/"
#define FONT_SUFFIX ".ttf"

C_TextSys::C_TextSys()
{
}

C_TextSys::~C_TextSys()
{
}

void C_TextSys::Register(C_World &_world, int _id)
{
	C_TextComponent &_component = _world.mTextComponents[_id];
	texRender.MakeText(_component.textureWidth, _component.textureHeight);
	_component.textureID = texRender.GetText();
	_component.rebuild = true;
	textComponents.insert(_id);
}

void C_TextSys::Unregister(C_World &_world, int _id)
{
	C_TextComponent &_component = _world.mTextComponents[_id];
	glDeleteTextures(1, &_component.textureID);
	_component.textureID = -1;
	_component.rebuild = true;
	textComponents.erase(_id);
}

void C_TextSys::Update(C_World &_world)
{
	for (auto cmp_id : textComponents)
	{
		C_TextComponent &_component = _world.mTextComponents[cmp_id];
		if (!_component.rebuild)
		{
			continue;
		}
		auto font_itr = fonts.find(_component.font);
		if (font_itr == fonts.end())
		{
			debug_func::ReportErr("Invalid font name in text component!");
			continue;
		}
		texRender.SetText(_component.textureID, _component.textureWidth, _component.textureHeight);
		//glViewport(0, 0, _component.textureWidth, _component.textureHeight);
		texRender.RenderString(font_itr->second, _component.text, _component.rowInterval, _component.scale);
		_component.rebuild = false;
	}
	

	//auto font_itr = fonts.find("arial");
	//if (font_itr == fonts.end())
	//{
	//	debug_func::ReportErr("Invalid font name in text component!");
	//	return; // later this will be continue
	//}
	//texRender.RenderString(font_itr->second, "1234567890",50); 
}

void C_TextSys::Init()
{
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		debug_func::ReportErr("Failed to initialize freetype library!");
		return;
	}
	LoadFonts();
	texRender.Init();
	//texRender.MakeText(512, 512);
}

void C_TextSys::LoadFonts()
{
	FT_Error error;
	FT_Face new_font;
	std::string font_name = "BebasNeue-Regular";
	error = FT_New_Face(library, (std::string(FONT_DIRECTION) + font_name + FONT_SUFFIX).c_str(), 0, &new_font);
	if (error)
	{
		if (error == FT_Err_Unknown_File_Format)
		{
			debug_func::ReportErr("Unsupported font format!");
		}
		else
		{
			debug_func::ReportErr("Failed to open font format!");
		}
	}
	int pen_size = 64;
	FT_Set_Pixel_Sizes(new_font, pen_size, 0);

	S_Atlas new_atlas;
	new_atlas.BuildAtlas(new_font);
	fonts.insert(std::make_pair(font_name, new_atlas));
}

C_TextRenderer::C_TextRenderer()
{
}

C_TextRenderer::~C_TextRenderer()
{
}

void C_TextRenderer::Init()
{
	vert.createShaderFromFile("Bin/BuildInShaders/text.vert", E_SHADER_TYPE_VERTEX);
	frag.createShaderFromFile("Bin/BuildInShaders/text.frag", E_SHADER_TYPE_FRAGMENT);
	program.setVertexShader(vert);
	program.setFragmentShader(frag);
	program.buildPipeline();
	texAtlas = program.findParam("atlas", E_PARAM_TYPE_UNISAMP2D);
	proj = program.findParam("projection", E_PARAM_TYPE_UNIMAT4);
	pos = program.findParam("position", E_PARAM_TYPE_ATTVEC2);
	uv = program.findParam("uv", E_PARAM_TYPE_ATTVEC2);
	glGenFramebuffers(1, &frameBufferObject);
	glGenBuffers(1, &texBuffer);
}

void C_TextRenderer::MakeText(int _width, int _height)
{
	glGenTextures(1, &text);
	glBindTexture(GL_TEXTURE_2D, text);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

struct text_point
{
	float posx;
	float posy;
	float u;
	float v;
};

void C_TextRenderer::RenderString(S_Atlas const &_atlas, std::string _str, int _row_interval, vec2 &_size)
{
	program.bindPipeline();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glViewport(0, 0, 1280, 720);
	size_t size = _str.size();
	if (size > 500)
	{
		return;
	}
	//std::vector<text_point> coords(_str.size());
	text_point coords[500];

	float xmax = 0.0f,
		ymax = 0.0f,
		ymin = 0.0f;
	int n = 0,
		x = 0,
		y = 0;
	float sx = 1.0f,
		sy = 1.0f;
	for (char const &p : _str) {

		if (p == '\n')
		{
			y -= _row_interval;
			x = 0;
			continue;
		}
		float x2 = x + _atlas.charInfos[p].bl * sx;
		float y2 = - y - _atlas.charInfos[p].bt * sy;
		float w = _atlas.charInfos[p].bw * sx;
		float h = _atlas.charInfos[p].bh * sy;

		/* Advance the cursor to the start of the next character */
		x += _atlas.charInfos[p].ax * sx;
		
		y += _atlas.charInfos[p].ay * sy;

		/* Skip glyphs that have no pixels */
		if (!w || !h)
		{
			continue;
		}

		coords[n++] = { x2, -y2, _atlas.charInfos[p].tx, 0 };
		coords[n++] = { x2 + w, -y2, _atlas.charInfos[p].tx + _atlas.charInfos[p].bw / _atlas.width, 0 };
		coords[n++] = { x2, -y2 - h, _atlas.charInfos[p].tx, _atlas.charInfos[p].bh / _atlas.height }; //remember: each glyph occupies a different amount of vertical space
		coords[n++] = { x2 + w, -y2, _atlas.charInfos[p].tx + _atlas.charInfos[p].bw / _atlas.width, 0 };
		coords[n++] = { x2, -y2 - h, _atlas.charInfos[p].tx, _atlas.charInfos[p].bh / _atlas.height };
		coords[n++] = { x2 + w, -y2 - h, _atlas.charInfos[p].tx + _atlas.charInfos[p].bw / _atlas.width, _atlas.charInfos[p].bh / _atlas.height };

		xmax = (std::max)(x2 + w, xmax);
		ymax = (std::max)(-y2, ymax);
		ymin = (std::min)(-y2 - h, ymin);
		//coords[n++] = { -10.0f, 10.0f, 0, 1 };
		//coords[n++] = { 10.0f, 10.0f, 1.0f,1.0f };
		//coords[n++] = { -10.0f, -10.0f, 0.0f,0.0f}; //remember: each glyph occupies a different amount of vertical space
		//coords[n++] = { 10.0f, 10.0f, 1.0f,1.0f };
		//coords[n++] = { -10.0f, -10.0f, 0.0f,0.0f };
		//coords[n++] = { 10.0f, -10.0f, 1.0f, 0.0f };
	}

	_size = vec2(xmax / (ymax - ymin), 1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(text_point) * n, &coords[0], GL_STATIC_DRAW);

	//C_ResourceManager *resource_mgr = C_ResourceManager::GetInstance();
	//GLuint test_text = (resource_mgr->GetTexture("level1icon.png")).texture;
	glUniform1i(texAtlas, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _atlas.text);

	mat4 projection = glm::ortho(0.0f, xmax, ymin, ymax, -100.0f, 100.0f) * glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(proj, 1, false, &projection[0][0]);


	glVertexAttribPointer(pos, 2, GL_FLOAT, false, sizeof(text_point), reinterpret_cast<GLvoid const *>(0));
	glEnableVertexAttribArray(pos);

	glVertexAttribPointer(uv, 2, GL_FLOAT, false, sizeof(text_point), reinterpret_cast<GLvoid const *>(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(uv);

	glDrawArrays(GL_TRIANGLES, 0, n);
}

GLuint C_TextRenderer::GetText()
{
	return text;
}

void C_TextRenderer::SetText(GLuint _tex, int _w, int _h)
{
	text = _tex;

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, text, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _w, _h);
}

void S_Atlas::BuildAtlas(FT_Face face)
{
	unsigned int w = 0;
	unsigned int h = 0;
	FT_GlyphSlot g = face->glyph;

	for (int i = 32; i < 128; i++) {
		FT_Error err = FT_Load_Char(face, FT_ULong(i), FT_LOAD_RENDER);
		if (err) {
			debug_func::ReportErr("Failed loading character ");
			continue;
		}

		w += g->bitmap.width;
		h = (std::max)(h, g->bitmap.rows);
	}
	width = w;
	height = h;
	GLenum err = glGetError();
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &text);
	glBindTexture(GL_TEXTURE_2D, text);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	err = glGetError();
	int r = 0;
	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			continue;
		}
		std::vector<unsigned char> test_data(g->bitmap.width * g->bitmap.rows);
		std::fill(test_data.begin(), test_data.end(), 200);
		err = glGetError();
		glTexSubImage2D(GL_TEXTURE_2D, 0, r, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		err = glGetError();


		int id = i + 1;
		charInfos[i].ax = g->advance.x >> 6;
		charInfos[i].ay = g->advance.y >> 6;

		charInfos[i].bw = g->bitmap.width;
		charInfos[i].bh = g->bitmap.rows;

		charInfos[i].bl = g->bitmap_left;
		charInfos[i].bt = g->bitmap_top;

		charInfos[i].tx = (float)r / w;

		r += g->bitmap.width;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
