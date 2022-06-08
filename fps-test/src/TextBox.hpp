#pragma once

#include <functional>
#include <string>

#include <SFML/Graphics.hpp>

class TextBox : public sf::Drawable, public sf::Transformable
{
	sf::Text          m_text;
	sf::RenderTexture m_renderTexture;
	sf::Color         m_backgroundColor;
	sf::Sprite        m_sprite;
	bool              m_isFocused;

protected:
	void refreshTexture();

public:
	TextBox( sf::Font &font, unsigned int characterSize = 24, sf::Color color = sf::Color::Black,
	         sf::Color backgroundColor = sf::Color::White );
	void              setText( const sf::String &value );
	const sf::String &getText() const { return m_text.getString(); }
	void              setSize( const unsigned int &width, const unsigned int &height );
	void              setPosition( sf::Vector2f position );
	void              setPosition( float x, float y );
	void              setColor( sf::Color color );
	void              setBackgroundColor( sf::Color color );
	void              setFont( sf::Font &font );
	void              setCharacterSize( unsigned int characterSize );
	void              setFocused( bool focused );
	bool              isFocused() const;
	void              draw( sf::RenderTarget &target, sf::RenderStates states ) const;
	bool              processEvent( sf::Event &event );

	// Events
	std::function< void( const sf::String & ) > onTextChanged = []( const sf::String & ) {};
	std::function< void( void ) >               onFocusLost   = []( void ) {};

	// Input Filter callback
	std::function< bool( sf::Event & ) > onInputFilter = []( sf::Event &event ) { return true; };
};
