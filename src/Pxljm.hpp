#ifndef PXLJM_HPP
#define PXLJM_HPP

#include <gecom/Initial3D.hpp>
#include <btBulletDynamicsCommon.h>

namespace pxljm {

	inline i3d::vec3d bt2i3d(btVector3 v) { return i3d::vec3d(v.getX(), v.getY(), v.getZ()); }
	inline i3d::quatd bt2i3d(btQuaternion q) { return i3d::quatd(q.getW(), q.getX(), q.getY(), q.getZ()); }

	inline btVector3 i3d2bt(i3d::vec3d v) { return btVector3(v.x(), v.y(), v.z()); }
	inline btQuaternion i3d2bt(i3d::quatd q) { return btQuaternion(q.x(), q.y(), q.z(), q.w()); }

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

#endif
