#include "texture.hpp"

Texture::Texture(
  const char *filename,
  const GLenum target,
  const GLint format,
  const GLint wrap
) : target(target), format(format), wrap(wrap) {
  this->filename = new char[strlen(filename) + 1];
  strcpy(this->filename, filename);
  glGenTextures(1, &buffer);
  glBindTexture(target, buffer);
  if (target == GL_TEXTURE_2D) {
    char *path = new char[strlen(filename) + 6];
    sprintf(path, "%s.webp", filename);
    SDL_Surface *surface = IMG_Load(path);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexImage2D(GL_TEXTURE_2D, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    // GLfloat anisotropy;
    // glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
    SDL_FreeSurface(surface);
    delete [] path;
    path = NULL;
  }
  if (target == GL_TEXTURE_CUBE_MAP) {
    for (unsigned int i = 0; i < 6; i += 1) {
      char *path = new char[strlen(filename) + 7];
      sprintf(path, "%s%d.webp", filename, i);
      SDL_Surface *surface = IMG_Load(path);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
      SDL_FreeSurface(surface);
      delete [] path;
      path = NULL;
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
  glBindTexture(target, 0);
}

std::vector<Texture> Texture::cache;
const GLuint Texture::loadFromCache(const char *filename, const GLenum target, const GLint format, const GLint wrap) {
  for (std::vector<Texture>::iterator texture = cache.begin(); texture != cache.end(); ++texture) {
    if (
      strcmp((*texture).filename, filename) == 0 &&
      (*texture).target == target &&
      (*texture).format == format &&
      (*texture).wrap == wrap
    ) {
      return (*texture).buffer;
    }
  }
  Texture texture(filename, target, format, wrap);
  cache.push_back(texture);
  return texture.buffer;
}
