#include "TextBox.hpp"

#include <algorithm>

TextBox::TextBox( sf::Font &font, unsigned int characterSize, sf::Color color, sf::Color backgroundColor )
{
	m_text.setFont( font );
	m_text.setCharacterSize( characterSize );
	m_text.setPosition( { 2, 2 } );
	m_backgroundColor = backgroundColor;
	m_renderTexture.create( 200, characterSize + 8 + 2 );
	m_sprite.setTexture( m_renderTexture.getTexture() );
	m_isFocused = false;
}

void TextBox::setText( const sf::String &value )
{
	m_text.setString( value );
	onTextChanged( value );

	refreshTexture();
}

void TextBox::refreshTexture()
{
	auto clearColor = m_backgroundColor;
	if( m_isFocused ) {
		clearColor.r += 40;
		clearColor.g += 40;
		clearColor.b += 40;
	}
	m_renderTexture.clear( clearColor );
	m_renderTexture.draw( m_text );
	m_renderTexture.display();
}

void TextBox::setSize( const unsigned int &width, const unsigned int &height )
{
	if( height > 6 ) {
		//m_text.setCharacterSize( height - 4 );
		// TODO: improve this...
		const auto sz = m_renderTexture.getSize();
		m_sprite.setTextureRect( { 0, 0, static_cast<int>(std::min( width, sz.x )), static_cast<int>(std::min( height, sz.y )) } );
		refreshTexture();
	}
}

void TextBox::setPosition( sf::Vector2f position ) { m_sprite.setPosition( position ); }

void TextBox::setPosition( float x, float y ) { m_sprite.setPosition( x, y ); }

void TextBox::setColor( sf::Color color )
{
	m_text.setFillColor( color );
	refreshTexture();
}

void TextBox::setBackgroundColor( sf::Color color )
{
	m_backgroundColor = color;
	refreshTexture();
}

void TextBox::setFont( sf::Font &font )
{
	m_text.setFont( font );
	refreshTexture();
}

void TextBox::setCharacterSize( unsigned int characterSize )
{
	m_text.setCharacterSize( characterSize );
	refreshTexture();
}

void TextBox::setFocused( bool focused )
{
	m_isFocused = focused;
	if( !m_isFocused ) {
		onFocusLost();
	}
	refreshTexture();
}

bool TextBox::isFocused() const { return m_isFocused; }

void TextBox::draw( sf::RenderTarget &target, sf::RenderStates states ) const { target.draw( m_sprite, states ); }

bool TextBox::processEvent( sf::Event &event )
{
	if( m_isFocused ) {
		if( event.type == sf::Event::TextEntered ) {
			if( event.text.unicode == '\b' ) {
				if( m_text.getString().getSize() > 0 ) {
					setText( m_text.getString().substring( 0, m_text.getString().getSize() - 1 ) );
				}
			} else if( event.text.unicode == '\t' ) {
				setFocused( false );
			} else if( onInputFilter( event ) ) {
				setText( m_text.getString() + static_cast< char >( event.text.unicode ) );
			}
			return true; // Event was consumed
		}
	}
	return false;
}