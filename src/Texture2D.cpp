#include "Texture2D.hpp"

#include <iostream>

Texture2D::Texture2D() {
	Width = 0.f;
	Height = 0.f;
	u = 0.f;
	v = 0.f;
	angle = 0.f;
	scaleX = 1.f;
	scaleY = 1.f;
	flippedOnX = false;
	flippedOnY = false;
	tint = Color::White;
}

Texture2D::Texture2D(const std::string& imageFile) {
	Width = 0.f;
	Height = 0.f;
	u = 0.f;
	v = 0.f;
	angle = 0.f;
	scaleX = 1.f;
	scaleY = 1.f;
	flippedOnX = false;
	flippedOnY = false;
	tint = Color::White;

	Load(imageFile);
}

Texture2D::~Texture2D() {
    Release();
}

bool Texture2D::Load(const std::string& imageFile) {
	SDL_Surface* image = NULL;
	GLint colors;
	GLenum format = 0;

	if((image = IMG_Load(imageFile.c_str())) == NULL) {
		image = NULL;
		return false;
	}

	if((image->w & (image->w - 1)) != 0) {
		std::cerr << "Warning   : Image's Widthidth is not a poWidther of 2" << std::endl
				  << "Image file: " << imageFile << std::endl;
	}

	if((image->h & (image->h - 1)) != 0) {
		std::cerr << "Warning   : Image's height is not a poWidther of 2" << std::endl
				  << "Image file: " << imageFile << std::endl;
	}

	Width = (float)image->w;
	Height = (float)image->h;
	u = 1.f;
	v = 1.f;

	colors = image->format->BytesPerPixel;

	if(colors == 4) {
		format = GL_RGBA;

		if(image->format->Rmask == 0x000000ff)
			format = GL_RGBA;
        else
            format = GL_BGRA;
    } else if(colors == 3) {
		format = GL_RGB;

        if(image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
    } else {
		std::cerr << "Warning: Image is not truecolor. This might break" << std::endl
				  << "Image file: " << imageFile << std::endl;
    }

	if (textureID != 0)
		glDeleteTextures(1, &textureID);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, colors, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);

	if(image)
		SDL_FreeSurface(image);

	image = NULL;

	return true;
}

bool Texture2D::Load(const std::string& imageFile, const Color& colorKey) {
	SDL_Surface* image = NULL;
	GLint colors;
	GLenum format = 0;

	if((image = IMG_Load(imageFile.c_str())) == NULL) {
		image = NULL;
		return false;
	}

	if((image->w & (image->w - 1)) != 0) {
		std::cerr << "Warning   : Image's Widthidth is not a poWidther of 2" << std::endl
				  << "Image file: " << imageFile << std::endl;
	}

	if((image->h & (image->h - 1)) != 0) {
		std::cerr << "Warning   : Image's height is not a poWidther of 2" << std::endl
				  << "Image file: " << imageFile << std::endl;
	}

	Width = (float)image->w;
	Height = (float)image->h;
	u = 1.f;
	v = 1.f;

	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, colorKey.R, colorKey.G, colorKey.B));
	SDL_Surface* newImage = SDL_DisplayFormatAlpha(image);

	colors = newImage->format->BytesPerPixel;

	if(colors == 4) {
		format = GL_RGBA;

		if(newImage->format->Rmask == 0x000000ff)
			format = GL_RGBA;
        else
            format = GL_BGRA;
    } else if(colors == 3) {
		format = GL_RGB;

        if(newImage->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
    } else {
		std::cerr << "Warning: Image is not truecolor. This might break" << std::endl
				  << "Image file: " << imageFile << std::endl;
    }

	if (textureID != 0)
		glDeleteTextures(1, &textureID);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, colors, newImage->w, newImage->h, 0, format, GL_UNSIGNED_BYTE, newImage->pixels);

	if(image)
		SDL_FreeSurface(image);
	if(newImage)
		SDL_FreeSurface(newImage);

	image = NULL;
	newImage = NULL;

	return true;
}

bool Texture2D::LoadPOT(const std::string& imageFile) {
	SDL_Surface* image = NULL;
	GLint colors;
	GLenum format = 0;

	if((image = IMG_Load(imageFile.c_str())) == NULL) {
		image = NULL;
		return false;
	}

	Width = (float)GetNearestPowerOfTwo(image->w);
	Height = (float)GetNearestPowerOfTwo(image->h);
	u = 1.f;
	v = 1.f;

	colors = image->format->BytesPerPixel;

	if(colors == 4) {
		format = GL_RGBA;

		if(image->format->Rmask == 0x000000ff)
			format = GL_RGBA;
        else
            format = GL_BGRA;
    } else if(colors == 3) {
		format = GL_RGB;

		if(image->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
    } else {
		std::cerr << "Warning: Image is not truecolor. This might break" << std::endl
				  << "Image file: " << imageFile << std::endl;
    }

	if (textureID != 0)
		glDeleteTextures(1, &textureID);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, colors, Width, Height, 0, format, GL_UNSIGNED_BYTE, image->pixels);

	if(image)
		SDL_FreeSurface(image);

	image = NULL;

	return true;
}

bool Texture2D::LoadPOT(const std::string& imageFile, const Color& colorKey) {
	SDL_Surface* image = NULL;
	GLint colors;
	GLenum format = 0;

	if((image = IMG_Load(imageFile.c_str())) == NULL) {
		image = NULL;
		return false;
	}

	Width = (float)GetNearestPowerOfTwo(image->w);
	Height = (float)GetNearestPowerOfTwo(image->h);
	u = 1.f;
	v = 1.f;

	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, colorKey.R, colorKey.G, colorKey.B));
	SDL_Surface* newImage = SDL_DisplayFormatAlpha(image);

	colors = newImage->format->BytesPerPixel;

	if(colors == 4) {
		format = GL_RGBA;

		if(newImage->format->Rmask == 0x000000ff)
			format = GL_RGBA;
        else
            format = GL_BGRA;
	} else if(colors == 3) {
		format = GL_RGB;

        if(newImage->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
    } else {
		std::cerr << "Warning: Image is not truecolor. This might break" << std::endl
				  << "Image file: " << imageFile << std::endl;
    }

	if (textureID != 0)
		glDeleteTextures(1, &textureID);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, colors, Width, Height, 0, format, GL_UNSIGNED_BYTE, newImage->pixels);

	if(image)
		SDL_FreeSurface(image);
	if(newImage)
		SDL_FreeSurface(newImage);

	image = NULL;
	newImage = NULL;

	return true;
}

void Texture2D::Release() {
    if (textureID != 0)
        glDeleteTextures(1, &textureID);
}

void Texture2D::Draw(float x, float y) {
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTranslatef(x + Width / 2.f, y + Height / 2.f, 0.f);
	glRotatef(-angle, 0.f, 0.f, 1.f);

	if(scaleX != 1.f && scaleY != 1.f)
		glScalef(scaleX, scaleY, 1.f);

	if(flippedOnX)
		glScalef(-1.f, 1.f, 1.f);
	if(flippedOnY)
		glScalef(1.f, -1.f, 1.f);

	glTranslatef(-(x + (Width / scaleX) / 2.f), -(y + (Height / scaleY) / 2.f), 0.f);

	Graphics::BeginQuads();
		glColor4f(tint.R, tint.G, tint.B, tint.A);
			glTexCoord2f(0, 0); glVertex2f(x, y);
			glTexCoord2f(u, 0); glVertex2f(x + Width, y);
			glTexCoord2f(u, v); glVertex2f(x + Width, y + Height);
			glTexCoord2f(0, v); glVertex2f(x, y + Height);
	Graphics::End();
}

void Texture2D::DrawClipped(float x, float y, int sx, int sy, int sw, int sh) {
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTranslatef(x + sx + sw/2, y + sy + sh/2, 0.f);
	glRotatef(-angle, 0.f, 0.f, 1.f);

	if(flippedOnX)
		glScalef(-1.f, 1.f, 1.f);
	if(flippedOnY)
		glScalef(1.f, -1.f, 1.f);

	if(scaleX != 1.f && scaleY != 1.f)
		glScalef(scaleX, scaleY, 1.f);

	glTranslatef(-(x + sx + sw/2), -(y + sy + sh/2), 0.f);

	float rx = sx / Width;
	float ry = sy / Height;
	float rw = sw / Width;
	float rh = sh / Height;

	Graphics::BeginQuads();
		glColor4f(tint.R, tint.G, tint.B, tint.A);
			glTexCoord2f(rx, ry);			glVertex2f(x, y);
			glTexCoord2f(rx + rw, ry);		glVertex2f(x + sw, y);
			glTexCoord2f(rx + rw, ry + rh); glVertex2f(x + sw, y + sh);
			glTexCoord2f(rx, ry + rh);		glVertex2f(x, y + sh);
	Graphics::End();
}

void Texture2D::Rotate(float angle) {
	if(angle > 360.f)
		angle -= 360.f;
	else if(angle < 0.f)
		angle += 360.f;

	this->angle = angle;
}

void Texture2D::Scale(float factor) {
	if(factor > 0.f) {
		scaleX = factor;
		scaleY = factor;
	}
}

void Texture2D::ScaleTo(float width, float height) {
	scaleX = width / Width;
	scaleY = height / Height;
}

void Texture2D::SetAlpha(float alpha) {
	if(alpha >= 0.f && alpha <= 1.f)
		tint.A = 1.f - alpha;
}

void Texture2D::SetTint(const Color& tint) {
	this->tint = tint;
}

void Texture2D::FlipOnX() {
	flippedOnX = true;
}

void Texture2D::FlipOnY() {
	flippedOnY = true;
}

void Texture2D::AdjustUVCoords(float u, float v) {
	AdjustUCoord(u);
	AdjustVCoord(v);
}

void Texture2D::AdjustUCoord(float u) {
	if(u >= 0.f && u <= 1.f)
		this->u = u;
}

void Texture2D::AdjustVCoord(float v) {
	if(v >= 0.f && v <= 1.f)
		this->v;
}

void Texture2D::ResetFlip() {
	flippedOnX = false;
	flippedOnY = false;
}

void Texture2D::Reset() {
	Rotate(0.f);
	Scale(1.f);
	ResetFlip();
}

int Texture2D::GetNearestPowerOfTwo(int i) {
	int value = 1;

	while (value < i)
		value <<= 1;

	return value;
}

GLuint Texture2D::ID() const {
	return textureID;
}

float Texture2D::GetAlpha() const {
	return tint.A;
}

float Texture2D::GetAngle() const {
	return angle;
}

float Texture2D::GetScaleX() const {
	return scaleX;
}

float Texture2D::GetScaleY() const {
	return scaleY;
}

float Texture2D::GetUCoord() const {
	return u;
}

float Texture2D::GetVCoord() const {
	return v;
}
