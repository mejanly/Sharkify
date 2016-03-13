#ifndef __ParticleSystem__
#define __ParticleSystem__

#include "Headers.h"
#include "Particle.h"
#include "Program.h"
#include "MatrixStack.h"

#define NUM_FOOD 1
#define NUM_FOOD_PARTICLES 30
#define FOOD_TTL 35.0f 

using namespace std;

class ParticleSystem {
   public:
      ParticleSystem(GLuint _ShadeProg, GLuint _partTex);
      virtual ~ParticleSystem();

      void init(Program* prog);
      void step(Program* prog);
      
   private:
      vector<Particle*> foodParticles;
      GLuint ShadeProg;
		GLuint partTex;

		float randFloat(float l, float h);
		void applyProjectionMatrix(MatrixStack* P);
		void applyViewMatrix(MatrixStack* MV);

 		// time info for particles
      float t, t0_disp, t_disp;
      float h;
      glm::vec3 g;
};

#endif
