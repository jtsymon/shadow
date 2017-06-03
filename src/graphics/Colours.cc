#include "Colours.h"

RGB HSV::toRGB() {

  unsigned char region, remainder, p, q, t;

  if (this->s == 0) {
    return RGB(this->v, this->v, this->v);
  }

  region = this->h / 43;
  remainder = (this->h - (region * 43)) * 6;

  p = (this->v * (255 - this->s)) >> 8;
  q = (this->v * (255 - ((this->s * remainder) >> 8))) >> 8;
  t = (this->v * (255 - ((this->s * (255 - remainder)) >> 8))) >> 8;

  switch (region) {
  case 0:
    return RGB(this->v, t, p);
    break;
  case 1:
    return RGB(q, this->v, p);
    break;
  case 2:
    return RGB(p, this->v, t);
    break;
  case 3:
    return RGB(p, q, this->v);
    break;
  case 4:
    return RGB(t, p, this->v);
    break;
  default:
    return RGB(this->v, p, q);
    break;
  }
}

HSV RGB::toHSV() {
  unsigned char rgbMin, rgbMax;

  rgbMin = this->r < this->g ? (this->r < this->b ? this->r : this->b) : (this->g < this->b ? this->g : this->b);
  rgbMax = this->r > this->g ? (this->r > this->b ? this->r : this->b) : (this->g > this->b ? this->g : this->b);

  if (rgbMax == 0) {
    return HSV(0, 0, 0);
  }

  unsigned char sat = 255 * (long)(rgbMax - rgbMin) / rgbMax;
  if (sat == 0) {
    return HSV(0, 0, rgbMax);
  }

  if (rgbMax == this->r)
    return HSV(0 + 43 * (this->g - this->b) / (rgbMax - rgbMin), sat, rgbMax);
  else if (rgbMax == this->g)
    return HSV(85 + 43 * (this->b - this->r) / (rgbMax - rgbMin), sat, rgbMax);
  else
    return HSV(171 + 43 * (this->r - this->g) / (rgbMax - rgbMin), sat, rgbMax);
}
