#include "gae_file.h"
#include "gae_memory.h"

#include "SDL2/SDL.h"

typedef struct gae_file_sdl2_s {
	SDL_RWops* file;
	
	Sint64 pos;
	Sint64 length;
} gae_file_sdl2_t;

gae_file_t* gae_file_init(gae_file_t* file, enum gae_file_mode_e mode, enum gae_file_type_e type)
{
	gae_file_sdl2_t* platform = gae_alloc(sizeof(gae_file_sdl2_t));
	file->mode = mode;
	file->type = type;
	file->status = GAE_FILE_NO_FILE;
	
	file->platform = platform;
	platform->file = 0;
	platform->pos = 0;
	platform->length = 0;
	
	return file;
}

gae_file_t* gae_file_open(gae_file_t* file, const char* path)
{
	char options[3];
	gae_file_sdl2_t* platform = file->platform;
	
	if ((GAE_FILE_NO_FILE != file->status)
	&& (GAE_FILE_CLOSED != file->status))
		return file;
	
	switch (file->mode) {
		case GAE_FILE_MODE_READ: options[0] = 'r'; break;
		case GAE_FILE_MODE_WRITE: options[0] = 'w'; break;
		case GAE_FILE_MODE_APPEND: options[0] = 'a'; break;
		default: break;
	}
	
	switch (file->type) {
		case GAE_FILE_TYPE_ASCII: options[1] = '\0'; break;
		case GAE_FILE_TYPE_BINARY: options[1] = '+'; options[2] = 'b'; break;
		default: break;
	}

	platform->file = SDL_RWFromFile(path, options);

	if (0 == platform->file) {
		file->status = GAE_FILE_NOT_FOUND;
		return file;
	} else {
		file->status = GAE_FILE_NO_ERROR;
		SDL_RWseek(platform->file, 0, RW_SEEK_END);
		platform->length = SDL_RWtell(platform->file);
		SDL_RWseek(platform->file, 0, RW_SEEK_SET);
	}
	
	return file;
}

gae_file_t* gae_file_read(gae_file_t* file, unsigned long size, void* data)
{
	gae_file_sdl2_t* platform = file->platform;
	/* TODO: control how many bytes are read.. speed up I/O a bit */
	Sint64 readAmount = SDL_RWread(platform->file, data, 1, size);
	if (0 <= readAmount) { /* could be eof or error */
		file->status = GAE_FILE_READ_ERROR;
	} else
		file->status = GAE_FILE_NO_ERROR;
	
	platform->pos += readAmount;
	return file;
}

gae_file_t* gae_file_write(gae_file_t* file, unsigned long size, void* data)
{
	gae_file_sdl2_t* platform = file->platform;
	/* TODO: control how many bytes are written.. speed up I/O a bit */
	const size_t writeAmount = SDL_RWwrite(platform->file, data, 1, size);
	if (writeAmount < size)
		file->status = GAE_FILE_WRITE_ERROR;
	else
		file->status = GAE_FILE_NO_ERROR;
	
	platform->pos += writeAmount;
	return file;
}

gae_file_t* gae_file_seek(gae_file_t* file, unsigned long position, enum gae_file_seek_e seek)
{
	gae_file_sdl2_t* platform = file->platform;
	if (position == (unsigned int)GAE_FILE_SEEK_END) {
		SDL_RWseek(platform->file, 0, RW_SEEK_END);
		platform->pos = platform->length;
		return file;
	}
	
	switch (seek) {
		case GAE_FILE_SEEK_ABSOLUTE: {
			if (0 != SDL_RWseek(platform->file, position, RW_SEEK_SET))
				file->status = GAE_FILE_SEEK_ERROR;
			else
				platform->pos = position;
		};
		break;
		case GAE_FILE_SEEK_RELATIVE: {
			if (0 != SDL_RWseek(platform->file, position, RW_SEEK_CUR))
				file->status = GAE_FILE_SEEK_ERROR;
			else
				platform->pos += position;
		};
	};
	
	return file;
}

unsigned long gae_file_position(gae_file_t* file)
{
	gae_file_sdl2_t* platform = file->platform;
	return (unsigned long)platform->pos;
}

unsigned long gae_file_length(gae_file_t* file)
{
	gae_file_sdl2_t* platform = file->platform;
	return (unsigned long)platform->length;
}

gae_file_t* gae_file_close(gae_file_t* file)
{
	gae_file_sdl2_t* platform = file->platform;
	if (GAE_FILE_NO_ERROR != file->status) return file;
	
	SDL_RWclose(platform->file);
	file->status = GAE_FILE_CLOSED;
	
	return file;
}

gae_file_t* gae_file_destroy(gae_file_t* file)
{
	gae_file_close(file);
	gae_free(file->platform);
	return file;
}
