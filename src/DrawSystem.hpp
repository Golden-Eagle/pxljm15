#pragma once

#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>

#include "ComponentSystem.hpp"
#include "Material.hpp"

namespace pxljm {

	class drawcall {
	public:
		virtual ~drawcall();
		virtual void draw() = 0;
		material_ptr material();
		bool operator< (const drawcall& rhs) const;

	protected:
		material_ptr m_mat;
	};

	class DrawableComponent : public virtual EntityComponent {
	public:
		virtual void registerWith(Scene &) override;
		virtual void deregisterWith(Scene &) override;

		virtual std::vector<drawcall *> getDrawCalls(i3d::mat4d) = 0;

	};



	class DrawableSystem : public ComponentSystem {
	public:
		DrawableSystem();

		void registerDrawableComponent(DrawableComponent *);
		void deregisterDrawableComponent(DrawableComponent *);

		std::priority_queue<drawcall *> getDrawQueue(i3d::mat4d);

	private:
		std::unordered_set<DrawableComponent *> m_drawables;
	};
}