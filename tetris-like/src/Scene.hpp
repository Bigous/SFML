#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

class Updatable: public sf::Drawable
{
public:
  virtual void update() = 0;
};

class Scene : public Updatable
{
public:
	enum Planes {
		PlaneBackgroundFar = 0,
		PlaneBackgroundMid,
		PlaneBackgroundNear,
		PlaneItem,
		PlaneTouchable,
		PlaneEnemy,
		PlanePlayer,
		PlaneOverlayer,
		PlaneCount
	};

	Scene();
	virtual ~Scene() = default;

	virtual void draw( sf::RenderTarget &target, sf::RenderStates states ) const override;

  virtual void update() override {
    for( auto &plane: m_planes ) {
      for( auto &drawable: plane ) {
        auto upd = dynamic_cast<Updatable *>(drawable);
        if(upd != nullptr) {
          upd->update();
        }
      }
    }
  }

	void addDrawable( sf::Drawable *drawable, Planes plane ) { m_planes[plane].push_back( drawable ); }

private:
	std::vector< std::vector< sf::Drawable * > > m_planes;
};