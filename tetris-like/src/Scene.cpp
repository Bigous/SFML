#include "Scene.hpp"

Scene::Scene() { m_planes.resize( PlaneCount ); }

void Scene::draw( sf::RenderTarget &target, sf::RenderStates states ) const
{
	for( auto &plane: m_planes ) {
		for( auto &drawable: plane ) {
			target.draw( *drawable, states );
		}
	}
}