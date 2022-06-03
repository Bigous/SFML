#include <chrono>   // high_resolution_clock
#include <iostream> // cout
#include <numbers>  // pi
#include <string>   // string and stod
#include <vector>   // vector

#include <SFML/Graphics.hpp>

int main( int argc, char *argv[] )
{

	std::vector< std::string > args( argv, argv + argc );

	double fps_limit = args.size() > 1 ? std::stod( args[1] ) : 60;

	sf::Font font;
	if( !font.loadFromFile( "Resources/font.ttf" ) ) {
		std::cerr << "Failed to load font" << std::endl;
		return 1;
	}

	const auto &video = sf::VideoMode::getDesktopMode();

	// sf::RenderWindow window( sf::VideoMode( 640, 480 ), "SFML works!" );
	sf::RenderWindow window( video, "", sf::Style::Fullscreen );
	window.setPosition( { 0, 0 } );
	sf::Color darkGrey{ 40, 40, 40, 255 };

	std::vector< sf::RectangleShape > rects;
	std::vector< sf::Color >          colors{ sf::Color::Red,    sf::Color::Green,   sf::Color::Blue,
                                   sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan,
																	 sf::Color(0x3b1e08ff), sf::Color(0xfc6a03ff), sf::Color(0xaf69eeff) };
	float size       = 180.0f / colors.size() / 640.0f * video.width;

	for( auto &color: colors ) {
		sf::RectangleShape rect( { size, size } );
		rect.setFillColor( color );
		rects.push_back( std::move( rect ) );
	}

	sf::Text text;
	text.setFont( font );
	text.setCharacterSize( 24 );
	text.setFillColor( sf::Color::White );

	sf::Text input;
	input.setFont( font );
	input.setCharacterSize( 24 );
	input.setFillColor( { 127, 220, 200, 255 } );
	input.setPosition( { 0, 28 } );

	std::string typed = std::to_string( static_cast< unsigned int >( fps_limit ) );

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
				case sf::Event::Resized:
					startx     = 290.0f / 640.0f * event.size.width;
					starty     = 240.0f / 480.0f * event.size.height;
					initRadius = 100.0f / 640.0f * event.size.width;
					break;
				case sf::Event::TextEntered:
					if( event.text.unicode == '\b' ) {
						if( !typed.empty() ) {
							typed.pop_back();
						}
					} else if( event.text.unicode == '\r' ) {
						if( typed.size() > 0 ) {
							fps_limit = std::stod( typed );
							typed     = std::to_string( static_cast< unsigned int >( fps_limit ) );
						}
					} else if( event.text.unicode >= '0' && event.text.unicode <= '9' ) {
						typed += static_cast< char >( event.text.unicode );
					}
					break;
				default:
					break;
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

			text.setString( std::format( "FPS: {:.3}", fps ) );
			window.draw( text );

			input.setString( std::format( "FPS limit: {}{}", typed,
			                              ( static_cast< std::size_t >( frame / fps_limit * 2 ) % 2 == 0 ? "_" : "" ) ) );
			window.draw( input );

			window.display();
			frame++;
		}
	}

	return 0;
}
