#include <chrono>
#include <random>

#include "Shapes.hpp"

SHAPES getRandomShapeID()
{
	// static std::default_random_engine gen( std::chrono::high_resolution_clock::now().time_since_epoch().count() );
	static std::default_random_engine           gen( std::random_device{}() );
	static std::uniform_int_distribution< int > dis( FIRST_SHAPE, LAST_SHAPE );
	return static_cast< SHAPES >( dis( gen ) );
}