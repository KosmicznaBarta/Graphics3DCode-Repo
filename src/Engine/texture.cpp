#include "stdio.h"
#include "glm/glm.hpp"
#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "texture.h"
#include "stb/stb_image.h"
#include "Application/utils.h"

namespace xe {
      GLuint create_texture(const std::string &name, bool is_sRGB) {
          stbi_set_flip_vertically_on_load(true);
          GLint width, height, channels;
          auto img = stbi_load(name.c_str(), &width, &height, &channels, 0);
          if (!img) {
              std::cerr<<"Could not read image from file `"<<name<<"'\n";
          } else {
              std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
          }

          GLuint texID;
          // glGenTexture(GLsizei n, *GLuint textures)
          OGL_CALL(glGenTextures(1, &texID));
          // glBindTexture(GLenum target, GLuint texture)
          OGL_CALL(glBindTexture(GL_TEXTURE_2D, texID));

          GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
          GLenum internal_format;
          if (is_sRGB) {
              internal_format = (channels == 4) ? GL_SRGB_ALPHA : GL_SRGB;
          } else {
              internal_format = (channels == 4) ? GL_RGBA : GL_RGB;
          }

          // glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, 
          //              GLint border, GLenum format, GLenum type, const void *data)
          OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, img));
          // glTexParameteri(GLenum target, GLenum pname, GLint param);
          OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
          OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

          stbi_image_free(img);

          return texID;
      };
}