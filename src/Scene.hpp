
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
		Scene() {}

		~Scene() {}

		void add( entity_ptr e ){
			std::cout << "add entity" << std::endl;
			for ( entity_comp_ptr c : e->getComponents() ) {
				std::cout << "add comp" << std::endl;
				c->registerToScene(*this);
			}
			entities.push_back(e);
		}


		//
		// Double dispatch for handelling components
		//

		// All components
		//
		void registerComponent( entity_comp_ptr e ) {
			updateList.push_back(e);
		}


		// Drawable / Renderable components
		//
		void registerComponent( entity_draw_ptr d ) {
			std::cout << "Added drawable" << std::endl;
			updateList.push_back(d);
			drawableList.push_back(d);
		}

		// Physical / Physics components
		//
		// void registerComponent( entity_phys_ptr ) {
		// 	//do nothing for now
		// }


		// Fix current design
		// will call update on entity components
		// and then render out the scene
		//
		std::vector< entity_draw_ptr > getDrawList( ) {

			// Update
			//
			// for (auto it = updateList.begin() ; it != updateList.end(); ) {
			// 	if (auto comp = (*it).lock()) {
			// 		comp->update(*this);
			// 		++it;
			// 	} else {
			// 		it = updateList.erase(it);
			// 	}
			// }

			// Construct render list
			//
			std::vector< entity_draw_ptr > drawList;
			for (auto it = drawableList.begin() ; it != drawableList.end(); ) {
				if (auto drawable = (*it).lock()) {
					drawList.push_back(drawable);
					++it;
				} else {
					it = drawableList.erase(it);
				}
			}
			return drawList;
		}




		//
		// Utility methods
		//

		// entity_ptr get(string name) {

		// }

	private:

		std::vector< entity_ptr > entities; //Thanks ben

		std::vector< std::weak_ptr< EntityComponent > > updateList;
		std::vector< std::weak_ptr< DrawableComponent > > drawableList;

	};

}