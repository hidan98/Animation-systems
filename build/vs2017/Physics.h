#pragma once
#include "btBulletDynamicsCommon.h"
#include "gef_debug_drawer.h"
#include "graphics/mesh_instance.h"
#include "primitive_builder.h"
class Physics
{
public:
	Physics();
	~Physics();
	void InitPhysicsWorld(gef::Renderer3D* renderer_3d_);
	void cleanUp();
	void update(float dt);
	void init(gef::Platform* plat, gef::Renderer3D* renderer_3d_);
	void CreateRigidBodies();

	void render(gef::Renderer3D* renderer_3d_);

	
	btDiscreteDynamicsWorld* getWorld() { return dynamics_world_; }
private:

	void UpdateRigidBodies();
	void UpdatePhysicsWorld(float dt);

	btDiscreteDynamicsWorld* dynamics_world_;
	btSequentialImpulseConstraintSolver* solver_;
	btBroadphaseInterface* overlapping_pair_cache_;
	btCollisionDispatcher* dispatcher_;
	btAlignedObjectArray<btCollisionShape*> collision_shapes_;

	GEFDebugDrawer* debug_drawer_;


	gef::Mesh* floor_mesh_;
	gef::MeshInstance floor_gfx_;

	btRigidBody* sphere_rb_;
	gef::Mesh* sphere_mesh_;
	gef::MeshInstance sphere_gfx_;

	PrimitiveBuilder* builder;

};

