#include "projectile_trap.h"

#define MAX_PROJECTILES 5

ProjectileTrap::ProjectileTrap(int x, int z, int id) : Trap(x, z, id) {
}

// Setup to shoot every interval
void ProjectileTrap::setup_timer(long long time_interval_ms) {
    // cancels any previous timer, if exists.
    if (cancel_timer)
        cancel_timer();

    cancel_timer = Timer::get()->do_every(
        std::chrono::milliseconds(time_interval_ms),
        [&]() {
        shoot();
    });
}

// Makes copies of the given projectile to fill the projectile_pool with
void ProjectileTrap::fill_projectile_pool(Projectile projectile) {
    // Removes all projectiles from pool
    for each(Projectile proj in projectile_pool) {
        proj.disable();
    }
    projectile_pool.clear();

    // Fills projetile_pool
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectile_pool.push_back(projectile);
        projectile_pool[i].set_parent(id);
        projectile_pool[i].disable();
    }
}

// Looks for an available projectile from pool, then shoots it
void ProjectileTrap::shoot() {
    // Only shoots projectile if it is not already being shot

    Projectile *to_shoot;

    for (int i = 0; i < projectile_pool.size(); i++) {
        if (!projectile_pool[i].is_enabled) {
            to_shoot = &projectile_pool[i];
            break;
        }
    }

    if (to_shoot)
        to_shoot->fire(transform);
}