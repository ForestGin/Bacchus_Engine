#include "ModuleTextures.h"
#include "Application.h"
#include "OpenGL.h"
#include "ModuleResources.h"
#include "FileSystem.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")


#include "mmgr/mmgr.h"

ModuleTextures::ModuleTextures(bool start_enabled) : Module(start_enabled)
{
	name = "Textures";
}

ModuleTextures::~ModuleTextures() {}

bool ModuleTextures::Init(json file)
{
	bool ret = true;

	// Check versions
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
		iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
		ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		LOG("|[error]: DevIL version is different. Exiting...");
		ret = false;
	}

	
	// Initializing DevIL
	LOG("Initializing DevIL Version: %i", IL_VERSION);

	// Initialize IL
	ilInit();

	// Initialize ILU
	iluInit();

	// Initialize ILUT with OpenGL support.
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}

bool ModuleTextures::Start()
{
	// Loading Checkers Texture
	CheckerTexID = LoadCheckImage();

	return true;
}

bool ModuleTextures::CleanUp()
{
	
	return true;
}

uint ModuleTextures::LoadCheckImage() const
{
	// Creating pixel data for checkers texture
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	// Create the texture
	return CreateTextureFromPixels(GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, GL_RGBA, checkImage, true);
}

uint ModuleTextures::GetCheckerTextureID() const
{
	return CheckerTexID;
}

inline void ModuleTextures::SetTextureParameters(bool CheckersTexture) const
{
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	
	if (CheckersTexture)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else
	{

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		
		if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
		{
			GLfloat largest_supported_anisotropy;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);
		}
	}
}

uint ModuleTextures::CreateTextureFromPixels(int internalFormat, uint width, uint height, uint format, const void* pixels, bool CheckersTexture) const
{
	uint TextureID = 0;

	
	// Generate the texture ID 
	glGenTextures(1, (GLuint*)&TextureID);
	// Bind the texture so we can work with it
	glBindTexture(GL_TEXTURE_2D, TextureID);
	
	SetTextureParameters(CheckersTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);


	if (!CheckersTexture)
	{
		// Generate Mipmap and enable 2D Textures
		glEnable(GL_TEXTURE_2D);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	LOG("Loaded Texture: ID: %i , Width: %i , Height: %i ", TextureID, width, height);

	//Returning id so a mesh can use it
	return TextureID;
}

inline void ModuleTextures::CreateTextureFromImage(uint& TextureID, uint& width, uint& height, const char* path, bool load_existing) const
{
	ILinfo imageInfo;
	iluGetImageInfo(&imageInfo);
	width = imageInfo.Width;
	height = imageInfo.Height;

	if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT && ! load_existing)
		iluFlipImage();

	if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
	{
		TextureID = CreateTextureFromPixels(ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), ilGetData());

		ILuint size;
		ILubyte* data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer

		if (size > 0) 
		{
			data = new ILubyte[size]; // allocate data buffer

			if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
				App->fs->Save(path, data, size);

			delete[] data;
		}
	}
	else
		LOG("|[error]: Image conversion failed. ERROR: %s", iluErrorString(ilGetError()));
}

uint ModuleTextures::CreateTextureFromFile(const char* path, uint& width, uint& height, bool load_existing) const
{
	uint TextureID = 0;

	if (path == nullptr)
	{
		LOG("|[error]: Error at loading texture from path. ERROR: Path %s was nullptr", path);
		return TextureID;
	}


	uint ImageName = 0;
	ilGenImages(1, (ILuint*)&ImageName);

	ilBindImage(ImageName);

	if (!load_existing)
	{
		std::string file_path = path;
		std::string name = TEXTURES_FOLDER;
		uint count = file_path.find_last_of("/");
		file_path = file_path.substr(count + 1, file_path.size());

		uint countdot = file_path.find_last_of(".");
		file_path = file_path.substr(0, countdot);

		name.append(file_path);
		name.append(".dds");
	}
	
	if (ilLoadImage(path))
		CreateTextureFromImage(TextureID, width, height, name.data(), load_existing);
	else
		LOG("|[error]: DevIL could not load the image. ERROR: %s", iluErrorString(ilGetError()));

	ilDeleteImages(1, (const ILuint*)&ImageName);


	return TextureID;
}