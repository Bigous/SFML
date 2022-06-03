#include <chrono>   // high_resolution_clock
#include <iostream> // cout
#include <numbers>  // pi
#include <string>   // string and stod
#include <vector>   // vector

#include <SFML/Graphics.hpp>

#include "TextBox.hpp"

int main( int argc, char *argv[] )
{

	std::vector< std::string > args( argv, argv + argc );

	double fps_limit = args.size() > 1 ? std::stod( args[1] ) : 60;

	sf::Font font;
	if( !font.loadFromFile( "Resources/font.ttf" ) ) {
		std::cerr << "Failed to load font" << std::endl;
		return 1;
	}

	// const auto &video = sf::VideoMode::getDesktopMode();
	const auto video = sf::VideoMode( 640, 480 );

	sf::RenderWindow window( video, "SFML works!" );
	sf::Color        darkGrey{ 40, 40, 40, 255 };

	std::vector< sf::RectangleShape > rects;
	std::vector< sf::Color >          colors{ sf::Color::Red,          sf::Color::Green,        sf::Color::Blue,
                                   sf::Color::Yellow,       sf::Color::Magenta,      sf::Color::Cyan,
                                   sf::Color( 0x3b1e08ff ), sf::Color( 0xfc6a03ff ), sf::Color( 0xaf69eeff ) };
	float                             size = 180.0f / colors.size() / 640.0f * video.width;

	for( auto &color: colors ) {
		sf::RectangleShape rect( { size, size } );
		rect.setFillColor( color );
		rects.push_back( std::move( rect ) );
	}

	sf::Text text;
	text.setFont( font );
	text.setCharacterSize( 24 );
	text.setFillColor( sf::Color::White );

	sf::Text tFpsLimit;
	tFpsLimit.setFont( font );
	tFpsLimit.setCharacterSize( 24 );
	tFpsLimit.setFillColor( { 127, 220, 200, 255 } );
	tFpsLimit.setPosition( { 0, 30 } );
	tFpsLimit.setString( "FPS Limit: " );

	sf::Text tSquaresN;
	tSquaresN.setFont( font );
	tSquaresN.setCharacterSize( 24 );
	tSquaresN.setFillColor( { 127, 220, 200, 255 } );
	tSquaresN.setPosition( { 0, 64 } );
	tSquaresN.setString( "Squares Number: " );

	// std::string txtFpsLimit = std::to_string( static_cast< unsigned int >( fps_limit ) );

	TextBox tbFpsLimit( font, 24, { 127, 220, 200, 255 }, { 20, 20, 20, 255 } );
	tbFpsLimit.setPosition( { 150, 28 } );
	tbFpsLimit.setSize( 100, 34 );
	tbFpsLimit.setText( std::to_string( static_cast< unsigned int >( fps_limit ) ) );
	tbFpsLimit.onTextChanged = [&fps_limit]( const std::string &value ) {
		if( value.empty() ) {
			fps_limit = 60;
		} else {
			fps_limit = std::stod( value );
			if( fps_limit < 6 ) {
				fps_limit = 6;
			}
		}
	};
	tbFpsLimit.onInputFilter = [&tbFpsLimit]( sf::Event &event ) {
		// just numbers
		if( event.type == sf::Event::TextEntered ) {
			if( event.text.unicode >= '0' && event.text.unicode <= '9' && tbFpsLimit.getText().getSize() < 4 ) {
				return true;
			}
		}
		return false;
	};
	tbFpsLimit.setFocused( true );

	TextBox tbSquareN( font, 24, { 127, 220, 200, 255 }, { 20, 20, 20, 255 } );
	tbSquareN.setPosition( { 220, 62 } );
	tbSquareN.setSize( 30, 34 );
	tbSquareN.setText( std::to_string( rects.size() ) );
	tbSquareN.onTextChanged = [&fps_limit, &rects, &colors]( const std::string &value ) {
		int nrects = 3;
		if( !value.empty() ) {
			nrects = std::stoi( value );
		}
		if( nrects > colors.size() ) {
			nrects = colors.size();
		}
		auto sz = rects[0].getSize();
		rects.resize( nrects );
		for(int i = 0; i < nrects; ++i) {
			sf::RectangleShape rect( sz );
			rect.setFillColor( colors[i] );
			rects[i] = std::move( rect );
		}
	};
	tbSquareN.onInputFilter = [&tbSquareN]( sf::Event &event ) {
		// just numbers
		if( event.type == sf::Event::TextEntered ) {
			if( event.text.unicode >= '0' && event.text.unicode <= '9' && tbSquareN.getText().getSize() < 1 ) {
				return true;
			}
		}
		return false;
	};

	tbFpsLimit.onFocusLost = [&tbSquareN]() { tbSquareN.setFocused( true ); };
	tbSquareN.onFocusLost  = [&tbFpsLimit]() { tbFpsLimit.setFocused( true ); };

	auto        t0    = std::chrono::high_resolution_clock::now();
	double      fps   = 0.0;
	std::size_t frame = 0;

	float startx     = 290.0f / 640.0f * video.width;
	float starty     = 240.0f / 480.0f * video.height;
	float initRadius = 100.0f / 640.0f * video.width;

	while( window.isOpen() ) {
		sf::Event event;
		while( window.pollEvent( event ) ) {
			switch( event.type ) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					if( event.key.code == sf::Keyboard::Escape ) {
						window.close();
					}
					break;
				default:
					break;
			}
			if(!tbFpsLimit.processEvent( event )) {
				tbSquareN.processEvent( event );
			}
		}

		auto t1 = std::chrono::high_resolution_clock::now();
		auto dt = std::chrono::duration_cast< std::chrono::nanoseconds >( t1 - t0 ).count();
		fps     = 1e9 / dt;

		if( fps <= fps_limit ) {
			t0 = t1;

			window.clear( darkGrey );

			float a = 0.06f * frame;
			float t = 2.0f * std::numbers::pi_v< float > / rects.size();
			float r = initRadius * std::cos( 0.1f * a );

			int i = 0;
			for( auto &rect: rects ) {
				rect.setPosition( { startx + r * std::cos( a + t * i ), starty + r * std::sin( a + t * i ) } );
				window.draw( rect );
				i++;
			}

			text.setString( std::format( "FPS: {:.3} - Squares: {}/{}", fps, rects.size(), colors.size() ) );
			window.draw( text );

			window.draw( tbFpsLimit );
			window.draw( tFpsLimit );

			window.draw( tSquaresN );
			window.draw( tbSquareN );

			window.display();
			frame++;
		}
	}

	return 0;
}
