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

	sf::RenderWindow window( sf::VideoMode( 640, 480 ), "SFML works!" );
	sf::Color darkGrey{ 40, 40, 40, 255};

	sf::RectangleShape rect1( { 60.f, 60.f } );
	rect1.setFillColor( sf::Color::Red );

	sf::RectangleShape rect2( { 60.f, 60.f } );
	rect2.setFillColor( sf::Color::Green );

	sf::RectangleShape rect3( { 60.f, 60.f } );
	rect3.setFillColor( sf::Color::Blue );

	sf::Text text;
	text.setFont( font );
	text.setCharacterSize( 24 );
	text.setFillColor( sf::Color::White );

	auto        t0    = std::chrono::high_resolution_clock::now();
	double      fps   = 0.0;
	std::size_t frame = 0;

	while( window.isOpen() ) {
		sf::Event event;
		while( window.pollEvent( event ) ) {
			if( event.type == sf::Event::Closed )
				window.close();
		}

		auto t1 = std::chrono::high_resolution_clock::now();
		auto dt = std::chrono::duration_cast< std::chrono::nanoseconds >( t1 - t0 ).count();
		fps     = 1e9 / dt;

		if( fps <= fps_limit ) {
			t0 = t1;

			window.clear(darkGrey);

			float a = 0.06f * frame;
			float t = 2.0f * std::numbers::pi_v< float > / 3.0f;
			float r = 100.0f * std::cos( 0.1f * a );

			rect1.setPosition( { 290 + r * std::cos( a ), 240 + r * std::sin( a ) } );
			window.draw( rect1 );

			rect2.setPosition( { 290 + r * std::cos( a + t ), 240 + r * std::sin( a + t ) } );
			window.draw( rect2 );

			rect3.setPosition( { 290 + r * std::cos( a + 2 * t ), 240 + r * std::sin( a + 2 * t ) } );
			window.draw( rect3 );

			text.setString( std::format( "FPS: {:.3}", fps ) );
			window.draw( text );

			window.display();
			frame++;
		}
	}

	return 0;
}
