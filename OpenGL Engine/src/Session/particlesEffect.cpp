#include "particlesEffect.h"

ParticlesEffect_Demo::ParticlesEffect_Demo(Window *const wnd, Keyboard *const kbd, Mouse *const mse, Camera *const camera) : wnd(wnd), kbd(kbd), mse(mse), camera(camera) {

	Utilities::Random::seedRandom();

	OpenGL::initTexture(screenTexture, 1, wnd->getWidth(), wnd->getHeight());
	OpenGL::initDepthBufferTexture(depthBuffer, wnd->getWidth(), wnd->getHeight());

	camera->setPosition(glm::vec3(MAX_PARTICLE_SYSTEMS * 1.5f, 0.0f, 30.0f));

	particleEmitter = new ParticleSystem * [MAX_PARTICLE_SYSTEMS];

	for (int i = 0; i < MAX_PARTICLE_SYSTEMS; ++i) {

		Sprite *particleSprite = new Sprite(wnd, camera, "../Media/particle.bmp", &screenTexture);
		ParticleEntity *particle = new ParticleEntity(particleSprite, PARTICLE_LIFETIME);

		particle->sprite->setBlendType((Sprite::BlendType)Utilities::Random::randBetweenInt(0, 1), false);

		particleEmitter[i] = new ParticleSystem(particle, MAX_PARTICLE_EMIT, EMITTION_DELAY);

		particleEmitter[i]->transform.setPosition(glm::vec3(i * 5.0f, 0.0f, 0.0f));

		CustomParticleEffect_Type1 *cpe1 = new CustomParticleEffect_Type1(true, true, false, true, true);

		cpe1->setEffectTime(PARTICLE_LIFETIME);

		cpe1->setRotateTarget(glm::vec3(0.0f, 0.0f, Utilities::Random::randBetweenFloat(0.0f, 50.0f)));

		cpe1->setRotateType(CustomParticleEffect_Type1::EffectType::FLAT);
		/*
		{ //Temp scope start

			float random = Utilities::Random::randBetweenFloat(1.0f, 3.0f);

			cpe1->setSizeTarget(glm::vec2(random, random));

			cpe1->setSizeType((CustomParticleEffect_Type1::EffectType)Utilities::Random::randBetweenInt(0, 3));

		} //Temp scope end
		*/
		cpe1->setAccelerationTarget(
			glm::vec3(
				Utilities::Random::randBetweenFloat(0.0f, 0.1f),
				Utilities::Random::randBetweenFloat(0.0f, 0.1f),
				Utilities::Random::randBetweenFloat(0.0f, 0.1f)
			)
		);

		cpe1->setAccelerationType(CustomParticleEffect_Type1::EffectType::FLAT);
		
		cpe1->setColourTarget(
			glm::vec4(
				Utilities::Random::randBetweenFloat(0.0f, 1.0f),
				Utilities::Random::randBetweenFloat(0.0f, 1.0f),
				Utilities::Random::randBetweenFloat(0.0f, 1.0f),
				Utilities::Random::randBetweenFloat(0.0f, 1.0f)
			)
		);

		cpe1->setColourType((CustomParticleEffect_Type1::EffectType)Utilities::Random::randBetweenInt(0, 3));

		cpe1->setSpeedRange(glm::vec2(10.0f, 20.0f));

		cpe1->setDirectionShape((CustomParticleEffect_Type1::DirectionShape)i);

		cpe1->setSpawnShape((CustomParticleEffect_Type1::SpawnShape)Utilities::Random::randBetweenInt(0, 3));

		particleEmitter[i]->setParticleAffector(cpe1);

	}

	emitterController = 0;

	std::cout << "\nControls:\nPress 1 - " <<MAX_PARTICLE_SYSTEMS << " to switch particle emitter control\nPress WASD to move controlled particle emitter\nHold Z and use mouse to move camera freely" << "\n";

}

ParticlesEffect_Demo::~ParticlesEffect_Demo() {

	for (int i = 0; i != MAX_PARTICLE_SYSTEMS; ++i) {

		delete particleEmitter[i];
		particleEmitter[i] = nullptr;

	}

	delete[] particleEmitter;
	particleEmitter = nullptr;

	glUseProgram(0);

}

void ParticlesEffect_Demo::controls() {

	for (int i = 0; i < MAX_PARTICLE_SYSTEMS; ++i) {

		if (kbd->isPressedOnce(GLFW_KEY_1 + i)) {

			emitterController = i;

			std::cout << "\nControlling particle system " << emitterController + 1 << "\n";

		}

	}

	if (kbd->isPressed('W')) { particleEmitter[emitterController]->transform.translate(glm::vec3(0.0f, MOVEMENT_SPEED, 0.0f) * (float)Window::getDeltaTime_Seconds()); }
	if (kbd->isPressed('A')) { particleEmitter[emitterController]->transform.translate(glm::vec3(-MOVEMENT_SPEED, 0.0f, 0.0f) * (float)Window::getDeltaTime_Seconds()); }
	if (kbd->isPressed('S')) { particleEmitter[emitterController]->transform.translate(glm::vec3(0.0f, -MOVEMENT_SPEED, 0.0f) * (float)Window::getDeltaTime_Seconds()); }
	if (kbd->isPressed('D')) { particleEmitter[emitterController]->transform.translate(glm::vec3(MOVEMENT_SPEED, 0.0f, 0.0f) * (float)Window::getDeltaTime_Seconds()); }

}

void ParticlesEffect_Demo::update() {

	for (int i = 0; i < MAX_PARTICLE_SYSTEMS; ++i) { particleEmitter[i]->update(); }

	controls();

}

void ParticlesEffect_Demo::render() {

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	for (int i = 0; i < MAX_PARTICLE_SYSTEMS; ++i) { particleEmitter[i]->render(); }

	renderToScreen(screenTexture, wnd->getWidth(), wnd->getHeight());

}