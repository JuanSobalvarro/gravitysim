#include <cmath>
#include <glm/glm.hpp>

#define G 6.66704810 * pow(10, -11)

class Body
{
public:
	unsigned int ID;
	glm::dvec2 velocity;
	glm::dvec2 position;
	glm::dvec2 force;
	double mass;

	//Initialize
	Body(unsigned int id, double m, glm::dvec2 vel, glm::dvec2 pos)
	{
		ID = id;
		mass = m;
		position = pos; 
		velocity = vel;
		force = { 0,0 };
	}

	//Calculate the gravitational force between the bodies
	void gForce(double m2, glm::dvec2 r)
	{
		double calc = G * mass * m2;
		double mod = std::sqrt(pow(r[0], 2) + pow(r[1], 2));
		force = { calc * r[0] / std::pow(mod, 3), calc * r[1] / std::pow(mod, 3)};
	}

	//calcular nueva posicion usando el metodo newtoniano
	void update(double t)
	{
		//glm::dvec2 kin1 = kinEn(mass, velocity), kin2 = kinEn(mass, finalVel(0.1));
		//position
		for (auto i=0; i<2; ++i)
		{
			position[i] = position[i] + velocity[i] * t + force[i] * std::pow(t,2) / (mass * 2);
		}


		velocity = finalVel(t);
	}

	
	static glm::dvec2 distance(double x1, double x2, double y1, double y2)
	{
		return { x2 - x1, y2 - y1 };
	}


private:

	//calculo energia cinetica
	glm::dvec2 kinEn(double m, glm::dvec2 vel)
	{
		return { m * std::pow(vel[0], 2) / 2, m * std::pow(vel[1], 2) / 2 };
	}

	glm::dvec2 finalVel(double t)
	{
		return { velocity[0] + force[0] / mass * t, velocity[1] + force[1] / mass * t };
	}


};