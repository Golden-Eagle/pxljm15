
#include <iostream>
#include <vector>

#include "GECom.hpp"
#include "Entity.hpp"
#include "Camera.hpp"
#include "Initial3D.hpp"


namespace gecom {

	class Scene;
	using scene_ptr = std::shared_ptr<Scene>;


	//Colection of entities
	class Scene : public std::enable_shared_from_this<Scene> {
	public:
		Scene() {
			m_drawableSystem = new DrawableSystem();
			m_systems.push_back(m_drawableSystem);
		}

		~Scene() {
			for (ComponentSystem  *s : m_systems) {
				delete s;
			}
		}

		void add( entity_ptr e ){
			std::cout << "add entity" << std::endl;
			for (ComponentSystem *s : m_systems) {
				for ( entity_comp_ptr c : e->getComponents() ) {
					std::cout << "Try adding component" << std::endl;
					s->addComponent(c);
				}
			}
			m_entities.push_back(e);
		}

		std::vector< entity_draw_ptr > getDrawList( ) {
			return m_drawableSystem->getDrawList();
		}

	private:
		std::vector<entity_ptr> m_entities;
		std::vector<ComponentSystem  *> m_systems;

		DrawableSystem *m_drawableSystem;
	};

}