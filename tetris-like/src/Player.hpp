#pragma once

#include "Scene.hpp"

class Player : public Updatable
{
  sf::Texture texture;
  sf::Sprite sprite;
  short x,y;
public:
  Player();
  virtual ~Player() = default;

  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  virtual void update() override {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      x -= 5;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      x += 5;
    }
    sprite.setPosition(x,y);
  }

  void move(short x, short y) { this->x += x; this->y += y; }
};