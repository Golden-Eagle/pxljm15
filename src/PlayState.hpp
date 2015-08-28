namespace pxljm {
  class PlayState : public State < std::string > {
    std::shared_ptr<Scene> m_scene;
    Game* m_game;
  public:
    PlayState(Game* game) : m_game(game) {
      m_scene = std::make_shared<Scene>(game->window());

      // Plane
      //
      entity_ptr plane = std::make_shared<Entity>(i3d::vec3d(0, -1.0, 0));
      plane->emplaceComponent<MeshDrawable>(
        assets::getMesh("plane"),
        assets::getMaterial("basic"));
      plane->emplaceComponent<RigidBody>(std::make_shared<BoxCollider>(gecom::i3d2bt(i3d::vec3d(100, 1, 100))), 0);
      m_scene->add(plane);;

      // Cube
      //
      entity_ptr cube = std::make_shared<Entity>(i3d::vec3d(2, 10, 2));
      cube->emplaceComponent<MeshDrawable>(
        assets::getMesh("cube"),
        assets::getMaterial("basic"));

      cube->emplaceComponent<BoxMove>();
      m_scene->add(cube);

      //
      // The falling cube
      //
      cube = std::make_shared<Entity>(i3d::vec3d(0, 60, 5));
      cube->emplaceComponent<MeshDrawable>(
        assets::getMesh("cube"),
        assets::getMaterial("basic"));

      cube->emplaceComponent<RigidBody>(std::make_shared<BoxCollider>(gecom::i3d2bt(i3d::vec3d::one())));
      m_scene->add(cube);

      // Tigger Cube
      //
      cube = std::make_shared<Entity>(i3d::vec3d(0, 0.5, 0));
      cube->emplaceComponent<Trigger>(std::make_shared<SphereCollider>(0.1));
      cube->emplaceComponent<TriggerTest>();
      m_scene->add(cube);

      // Another Cube
      //
      cube = std::make_shared<Entity>(i3d::vec3d(0, 10, 5));
      cube->emplaceComponent<MeshDrawable>(
        assets::getMesh("cube"),
        assets::getMaterial("basic"));

      cube->emplaceComponent<RigidBody>(std::make_shared<BoxCollider>(gecom::i3d2bt(i3d::vec3d::one())));
      m_scene->add(cube);


      // Sphere
      //
      entity_ptr sphere = std::make_shared<Entity>(i3d::vec3d(0, 50, -10));
      sphere->emplaceComponent<MeshDrawable>(
        assets::getMesh("sphere"),
        assets::getMaterial("basic"));

      sphere->emplaceComponent<RigidBody>(std::make_shared<SphereCollider>(1));

      sphere->emplaceComponent<SphereBounce>();

      sphere->emplaceComponent<CollisionCallbackTest>();

      m_scene->add(sphere);
    }

    virtual action_ptr updateForeground() override {
      // Game loop
      m_scene->update();
      return nullAction();
    }

    virtual void drawForeground() override {
      m_scene->render();
    }
  };
}