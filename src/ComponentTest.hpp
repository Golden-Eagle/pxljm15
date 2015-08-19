#pragma once

#include "Entity.hpp"
#include "GECom.hpp"

namespace gecom {


	class BoxMove : public virtual UpdateComponent {
	public:
		BoxMove() { }

		virtual void update() {
			entity()->root()->position += i3d::vec3d(0.005, 0, 0);
		}
		
	};
}