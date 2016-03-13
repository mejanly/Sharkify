#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(GLuint _ShadeProg, GLuint _partTex) {
	ShadeProg = _ShadeProg;	
	partTex = _partTex;

	t = 0.0f;
   t0_disp = 0.0f;
   t_disp = 0.0f;
	h = 1.0f;
	g = glm::vec3(0.0f, -0.01f, 0.0f);
}

ParticleSystem::~ParticleSystem() {
   for (int i=0; i<foodParticles.size(); ++i) {
      delete foodParticles[i];
   }
}

float ParticleSystem::randFloat(float l, float h)
{
   float r = rand() / (float)RAND_MAX;
   return (1.0f - r) * l + r * h;
}

void ParticleSystem::init(Program* prog) {
   // load foodParticles
   foodParticles.clear();
	for(int i = 0; i < NUM_FOOD_PARTICLES; ++i) {
		Particle* particle = new Particle();
		particle->load();
		particle->setTexture(partTex);
		/*if (i%2 == 0) {
		   particle->setStartPos(glm::vec3(0.0f, 0.0f, 0.0f));
		}
		else {*/
		   particle->setStartPos(glm::vec3(0.0f, 0.0f, 0.0f));
		//}
		particle->setStartCol(glm::vec3(1.0f, 0.0f, 0.0f));
		particle->setStartTTL(FOOD_TTL);
		particle->startTime = randFloat(0.0f, FOOD_TTL);
		particle->setStartOpacity(0.6f);
		particle->setOpacityTaper(false);
		foodParticles.push_back(particle);
	
      foodParticles[i]->init(prog);
   }
}

// TODO IMPLEMENT THESE TWO FUNCS
void ParticleSystem::applyProjectionMatrix(MatrixStack* P) {
   //P->multMatrix(Projection);
}

void ParticleSystem::applyViewMatrix(MatrixStack* MV) {
   //MV->multMatrix(View);
}

void ParticleSystem::step(Program* prog) {
   // Display every 60 Hz
	t += h;
	
	//t_disp = window->getElapsedTime();
	t_disp = 0.5;
	
	// sort the fountainParticles from back to front
   /*MatrixStack temp;
   camera->applyViewMatrix(&temp);
   glm::mat4 V = temp.topMatrix();
   ParticleSorter sorter;
   sorter.C = glm::transpose(glm::inverse(V)); // glm is transposed!
   std::sort(fountainParticles.begin(), fountainParticles.end(), sorter);
   std::sort(fireflyParticles.begin(), fireflyParticles.end(), sorter);*/
   
   // Create matrix stacks
	// TODO
	MatrixStack P, MV;
	// Apply camera transforms
	P.pushMatrix();
	//applyProjectionMatrix(&P);
	MV.pushMatrix();
	//applyViewMatrix(&MV);
	// Bind the program
	prog->bind();
	
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P.topMatrix()));
	for (int i=0; i<foodParticles.size(); ++i) {
	   foodParticles[i]->update(t, h, g);
		foodParticles[i]->draw(&MV);
	}
	
	// Unbind the program
	prog->unbind();
}
