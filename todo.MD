// constructs a naked entity with no components and returns its identifier
auto entity = registry.create();

// destroys an entity and all its components
registry.destroy(entity);

// destroys all the entities in a range
auto view = registry.view<a_component, another_component>();
registry.destroy(view.begin(), view.end());

// releases an orphaned identifier
registry.release(entity);

// returns true if the entity is still valid, false otherwise
bool b = registry.valid(entity);

// gets the actual version for the given entity
auto curr = registry.current(entity);

registry.emplace<position>(entity, 0., 0.);

// ...

auto &vel = registry.emplace<velocity>(entity);
vel.dx = 0.;
vel.dy = 0.;