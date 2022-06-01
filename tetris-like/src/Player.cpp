#include "Player.hpp"

Player::Player()
{
  texture.loadFromFile("Resources/Player.png");
  sprite.setTexture(texture);
  //sprite.setOrigin( sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2 );
  x = 0;
  y = 0;
  sprite.setPosition( x, y );
  sprite.setTextureRect( sf::IntRect( 0, 0, 16, 16 ) );

}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw( sprite, states );
}