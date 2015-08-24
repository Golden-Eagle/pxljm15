/*
 * GECom Main Header
 *
 * aka shit that needs to go somewhere
 */

#pragma once

#include <cctype>
#include <string>
#include <algorithm>
#include <memory>
#include <utility>

#include <btBulletDynamicsCommon.h>

#include "Initial3D.hpp"

// this alias will be available by default in new i3d
namespace i3d = initial3d;

namespace gecom {

	inline i3d::vec3d bt2i3d(btVector3 v) { return i3d::vec3d(v.getX(), v.getY(), v.getZ()); }
	inline i3d::quatd bt2i3d(btQuaternion q) { return i3d::quatd(q.getW(), q.getX(), q.getY(), q.getZ()); }

	inline btVector3 i3d2bt(i3d::vec3d v) { return btVector3(v.x(), v.y(), v.z()); }
	inline btQuaternion i3d2bt(i3d::quatd q) { return btQuaternion(q.x(), q.y(), q.z(), q.w()); }

	class Uncopyable {
	private:
		Uncopyable(const Uncopyable &rhs) = delete;
		Uncopyable & operator=(const Uncopyable &rhs) = delete;
	protected:
		Uncopyable() { }
	};

	// trim leading and trailing whitespace
	inline std::string trim(const std::string &s) {
		auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) { return std::isspace(c); });
		auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) { return std::isspace(c); }).base();
		return wsback <= wsfront ? std::string() : std::string(wsfront, wsback);
	}

	// function to declare things as unused
	template <typename T1, typename... TR>
	inline void unused(const T1 &t1, const TR &...tr) {
		(void) t1;
		unused(tr...);
	}

	// unused() base case
	inline void unused() { }
}

template <class T>
inline void hash_combine(std::size_t & seed, const T & v) {
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
	template<typename S, typename T>
	struct hash<pair<S, T>> {
		inline size_t operator()(const pair<S, T> & v) const {
			size_t seed = 0;
			::hash_combine(seed, v.first);
			::hash_combine(seed, v.second);
			return seed;
		}
	};
}