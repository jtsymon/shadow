/* 
 * File:   Buffer.cc
 * Author: jts
 * 
 * Created on 26 June 2014, 4:33 PM
 */

#include "Buffer.h"
#include "../main.h"

Buffer::Buffer(int width, int height) : Texture::Texture(width, height) {
    glGenFramebuffers(1, &this->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->texture, 0);

    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw Exception("Failed to create framebuffer!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Buffer::Buffer() : Buffer(Graphics::width, Graphics::height) {
}

void Buffer::begin() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    this->prev_width = Graphics::width;
    this->prev_height = Graphics::height;
    Graphics::update_dimensions(this->width, this->height);
}

void Buffer::end() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Graphics::update_dimensions(this->prev_width, this->prev_height);
}
