
#include "UpdateSystem.hpp"
#include "Scene.hpp"

using namespace std;
using namespace pxljm;
using namespace i3d;
using namespace std::chrono_literals;


//
// Update component
//
void Updatable::registerWith(Scene &s) { s.updateSystem().registerUpdatable(this); }


void Updatable::deregisterWith(Scene &s) { s.updateSystem().deregisterUpdatable(this); }


chrono::duration<double> Updatable::updateInterval() { return 0s; }



//
// Input Update component
//
void InputUpdatable::registerWith(Scene &s) { s.updateSystem().registerInputUpdatable(this); }


void InputUpdatable::deregisterWith(Scene &s) { s.updateSystem().deregisterInputUpdatable(this); }



// 
// UpdateComponent System
// 
UpdateSystem::UpdateSystem() { }


void UpdateSystem::registerUpdatable(Updatable *c) {
	m_updatables.insert(c);
	update_data ud;
	ud.up = c;
	ud.prev = clock_t::now();
	ud.next = ud.prev;
	m_update_queue.push(ud);
}


void UpdateSystem::deregisterUpdatable(Updatable *c) {
	m_updatables.erase(c);
}


void UpdateSystem::update(clock_t::time_point now, clock_t::duration maxlead, clock_t::duration timebudget) {
	auto time0 = clock_t::now();
	std::vector<update_data> done_updates;
	// run updates until nothing to do or time budget consumed
	while (!m_update_queue.empty() && clock_t::now() < time0 + timebudget) {
		update_data ud = m_update_queue.top();
		if (ud.next >= now + maxlead) break;
		m_update_queue.pop();
		if (m_updatables.find(ud.up) != m_updatables.end()) {
			// updateable still registered
			ud.up->update(now, ud.prev);
			ud.prev = now;
			ud.next = std::chrono::time_point_cast<clock_t::duration, clock_t>(now + ud.up->updateInterval());
			done_updates.push_back(ud);
		}
	}
	// re-queue updateables that were updated
	for (const auto &ud : done_updates) {
		m_update_queue.push(ud);
	}
}


void UpdateSystem::registerInputUpdatable(InputUpdatable *c) {
	m_inputUpdatables.insert(c);
}


void UpdateSystem::deregisterInputUpdatable(InputUpdatable *c) {
	m_inputUpdatables.erase(c);
}


void UpdateSystem::inputUpdate() {
	for (InputUpdatable *c : m_inputUpdatables)
		c->inputUpdate(*m_wep);
}