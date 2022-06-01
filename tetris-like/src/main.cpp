#include <SFML/Graphics.hpp>
#include <chrono>
#include <format>
#include <iostream>
#include <locale>

#include "Player.hpp"
#include "Scene.hpp"
#include "Shapes.hpp"
#include "consts.hpp"

int main()
{
	try {
		auto locale = std::locale( "pt-BR" );
		std::locale::global( locale );

		sf::RenderWindow window( sf::VideoMode( 800, 600 ), "SFML works!" );
		window.setVerticalSyncEnabled( true );

		{
			sf::Image icon;
			if( !icon.loadFromFile( "Resources\\icon.png" ) ) {
				throw std::runtime_error( "Could not load icon.png" );
			}

			window.setIcon( icon.getSize().x, icon.getSize().y, icon.getPixelsPtr() );
		}

		sf::Font CaskaydiaCove;
		if( !CaskaydiaCove.loadFromFile( "Resources/Caskaydia Cove Nerd Font Complete Mono Windows "
		                                 "Compatible.ttf" ) ) {
			throw std::runtime_error( "Could not load font" );
		}

		Scene  scene;
		Player player;

		scene.addDrawable( &player, Scene::PlanePlayer );

		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

		sf::Text fps;
		fps.setFont( CaskaydiaCove );
		fps.setCharacterSize( 24 );
		fps.setFillColor( sf::Color::White );

		scene.addDrawable( &fps, Scene::PlaneBackgroundFar );

		// While the program is running
		while( window.isOpen() ) {
			sf::Event event;

			// Process events
			while( window.pollEvent( event ) ) {
				if( event.type == sf::Event::Closed ) {
					window.close();
				}
			}

			scene.update();

			// Calculate the framerate
			auto   t2                   = std::chrono::high_resolution_clock::now();
			auto   time_span            = std::chrono::duration_cast< std::chrono::nanoseconds >( t2 - t1 );
			double time_span_in_seconds = time_span.count() / 1'000'000'000.0;
			double framerate            = 1.0 / time_span_in_seconds;

			// Restart the timer
			t1 = t2;

			// Draw the fps
			fps.setString( std::format( "FPS: {:.0Lf} - TimeSpan: {:L}ns", framerate, time_span.count() ) );

			// Update
			window.clear();
			// Draw the contents of the window here...

			window.draw( scene );

			window.display();
		}
	} catch( const std::exception &e ) {
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}