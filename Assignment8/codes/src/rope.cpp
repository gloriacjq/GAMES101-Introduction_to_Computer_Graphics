#include <iostream>
#include <vector>

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.

        if (num_nodes <= 1) {
            return;
        }

        //masses and springs
        Vector2D unit = (end - start) / (num_nodes - 1);
        masses.push_back(new Mass(start, node_mass, false));
        for (int i = 1; i < num_nodes; i++) {
            Vector2D pos = start + i*unit;
            masses.push_back(new Mass(pos, node_mass, false));
            springs.push_back(new Spring(masses[i-1], masses[i], k));
        }

        //Comment-in this part when you implement the constructor
        for (auto &i : pinned_nodes) {
            masses[i]->pinned = true;
        }

    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            Vector2D dir = s->m2->position - s->m1->position;
            double dis = dir.norm();
            Vector2D force =  (s->k) * (dir / dis) * (dis - s->rest_length);
            s->m1->forces += force;
            s->m2->forces -= force;
        }
        // change the time step while using explicit method
        // delta_t /= 5;
        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
                m->forces += gravity * m->mass;

                // TODO (Part 2): Add global damping
                float damping = 0.1;
                m->forces += - damping * m->velocity;//.norm() * m->velocity;

                Vector2D a = m->forces / m->mass;

                // explicit method
                // m->position += m->velocity * delta_t;
                // m->velocity += a * delta_t;

                // semi-implicit method
                m->velocity += a * delta_t;
                m->position += m->velocity * delta_t;
            }

            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }

    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            Vector2D dir = s->m2->position - s->m1->position;
            float dis = dir.norm();
            Vector2D unitDir = dir / dis;
            
            if (!s->m1->pinned && !s->m2->pinned) {
                s->m1->position += unitDir * (dis - s->rest_length) / 2.0f;
                s->m2->position -= unitDir * (dis - s->rest_length) / 2.0f;
            } else if (!s->m1->pinned && s->m2->pinned) {
                s->m1->position += unitDir * (dis - s->rest_length);
            } else if (s->m1->pinned && !s->m2->pinned) {
                s->m2->position -= unitDir * (dis - s->rest_length);
            }
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                Vector2D temp_position = m->position;
                m->forces += gravity * m->mass;
                Vector2D a = m->forces / m->mass;

                // TODO (Part 3.1): Set the new position of the rope mass
                // m->position += m->position - m->last_position + a * delta_t *delta_t;

                // TODO (Part 4): Add global Verlet damping
                float damping_factor = 0.00005;
                m->position += (1 - damping_factor) * (m->position - m->last_position) + a * delta_t * delta_t;
                
                m->last_position = temp_position;
            }
            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);
        }
    }
}
