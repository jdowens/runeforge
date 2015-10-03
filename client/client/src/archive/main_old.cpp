#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/System.hpp>
#include<SFGUI/SFGUI.hpp>
#include<Thor/Input.hpp>
#include<Thor/Particles.hpp>
#include<Thor/Vectors.hpp>
#include<Thor/Math.hpp>
#include<Thor/Animations.hpp>
#include<SFGUI/Widgets.hpp>
#include<string>
#include<stdio.h>
#include<functional>
#include<RF/tilemap.h>

sf::RenderWindow window;
sf::View view;
float deltaTime;
const float MOVEMENT_SPEED = 400.0f;		// pixels per second
const float WINDOW_WIDTH = 1280.0f;
const float WINDOW_HEIGHT = 720.0f;
thor::UniversalEmitter emitter;
thor::ParticleSystem particle_system;
int selected_rune = 0;

void OnQuit(thor::ActionContext<std::string> context)
{
	window.close();
}

void OnLeft(thor::ActionContext<std::string> context)
{
	//printf("LEFT, %f\n", deltaTime);
	view.move(sf::Vector2f(MOVEMENT_SPEED*-1.0f*deltaTime, 0.0f));
}

void OnRight(thor::ActionContext<std::string> context)
{
	//printf("RIGHT, %f\n", deltaTime);
	view.move(sf::Vector2f(MOVEMENT_SPEED*1.0f*deltaTime, 0.0f));
}

void OnUp(thor::ActionContext<std::string> context)
{
	//printf("UP, %f\n", deltaTime);
	view.move(sf::Vector2f(0.0f, MOVEMENT_SPEED*-1.0f*deltaTime));
}

void OnDown(thor::ActionContext<std::string> context)
{
	//printf("DOWN, %f\n", deltaTime);
	view.move(sf::Vector2f(0.0f, MOVEMENT_SPEED*1.0f*deltaTime));
}

void OnMouseDown(thor::ActionContext<std::string> context)
{

}

void OnMouseUp(thor::ActionContext<std::string> context)
{
	emitter.setParticlePosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	if (selected_rune == 1)
	{
		// Build color gradient
		thor::ColorGradient gradient;
		gradient[0.f] = sf::Color::Yellow;
		gradient[0.5f] = sf::Color::Red;
		gradient[1.f] = sf::Color::Black;

		// Create color and fade in/out animations
		thor::ColorAnimation colorizer(gradient);
		thor::FadeAnimation fader(0.1f, 0.1f);

		particle_system.clearAffectors();
		// Add particle affectors
		particle_system.addAffector(thor::AnimationAffector(colorizer));
		particle_system.addAffector(thor::AnimationAffector(fader));
		particle_system.addAffector(thor::TorqueAffector(100.f));

		emitter.setParticlePosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		emitter.setParticleVelocity(thor::Distributions::circle(sf::Vector2f(0.0f, 0.0f), 100.f));
		emitter.setEmissionRate(50.0f);
		particle_system.addEmitter(thor::refEmitter(emitter), sf::seconds(0.35f));
	}
	else if (selected_rune == 2)
	{
		// Build color gradient
		thor::ColorGradient gradient;
		gradient[0.f] = sf::Color::Blue;
		gradient[0.5f] = sf::Color::Black;
		gradient[1.f] = sf::Color::Black;

		// Create color and fade in/out animations
		thor::ColorAnimation colorizer(gradient);
		thor::FadeAnimation fader(0.1f, 0.1f);

		particle_system.clearAffectors();
		// Add particle affectors
		particle_system.addAffector(thor::AnimationAffector(colorizer));
		particle_system.addAffector(thor::AnimationAffector(fader));
		particle_system.addAffector(thor::TorqueAffector(100.f));

		emitter.setParticlePosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		emitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(0.0f, -100.0f), 90.0f));
		emitter.setEmissionRate(100.0f);
		particle_system.addEmitter(thor::refEmitter(emitter), sf::seconds(0.35f));
	}
	else if (selected_rune == 3)
	{
		// Build color gradient
		thor::ColorGradient gradient;
		gradient[0.f] = sf::Color::Green;
		gradient[0.5f] = sf::Color::Cyan;
		gradient[1.f] = sf::Color::Black;

		// Create color and fade in/out animations
		thor::ColorAnimation colorizer(gradient);
		thor::FadeAnimation fader(0.1f, 0.1f);

		particle_system.clearAffectors();
		// Add particle affectors
		particle_system.addAffector(thor::AnimationAffector(colorizer));
		particle_system.addAffector(thor::AnimationAffector(fader));
		particle_system.addAffector(thor::TorqueAffector(100.f));

		emitter.setParticlePosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		emitter.setParticleVelocity(thor::Distributions::rect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(100.0f, 100.0f)));
		emitter.setEmissionRate(150.0f);
		particle_system.addEmitter(thor::refEmitter(emitter), sf::seconds(2.0f));
	}
	else if (selected_rune == 4)
	{
		// Build color gradient
		thor::ColorGradient gradient;
		gradient[0.f] = sf::Color::Magenta;
		gradient[0.5f] = sf::Color::Green;
		gradient[1.f] = sf::Color::Black;

		// Create color and fade in/out animations
		thor::ColorAnimation colorizer(gradient);
		thor::FadeAnimation fader(0.1f, 0.1f);

		particle_system.clearAffectors();
		// Add particle affectors
		particle_system.addAffector(thor::AnimationAffector(colorizer));
		particle_system.addAffector(thor::AnimationAffector(fader));
		particle_system.addAffector(thor::TorqueAffector(100.f));
		emitter.setParticleVelocity(thor::Distributions::circle(sf::Vector2f(0.0f, 0.0f), 250.0f));
		emitter.setEmissionRate(500.0f);
		particle_system.addEmitter(thor::refEmitter(emitter), sf::seconds(1.0f));
	}
}

void OnButtonClicked()
{

}

void OnRuneOne()
{
	selected_rune = 1;
}

void OnRuneTwo()
{
	selected_rune = 2;
}

void OnRuneThree()
{
	selected_rune = 3;
}

void OnRuneFour()
{
	selected_rune = 4;
}


int main()
{
	deltaTime = 0;
	sfg::SFGUI sfgui;

	// map sprite
	sf::Texture map_texture;
	sf::Sprite map_sprite;
	map_texture.loadFromFile("resources/map.png");
	map_sprite.setTexture(map_texture);

	// window
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "Runestone");
	view.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	view.setCenter(sf::Vector2f(0, 0));
	window.setView(view);

	// timer
	sf::Clock clock;

	// thor actions
	thor::Action quit(sf::Event::Closed);
	thor::Action left(sf::Keyboard::A, thor::Action::Hold);
	thor::Action right(sf::Keyboard::D, thor::Action::Hold);
	thor::Action up(sf::Keyboard::W, thor::Action::Hold);
	thor::Action down(sf::Keyboard::S, thor::Action::Hold);
	thor::Action mouse_down(sf::Mouse::Left, thor::Action::PressOnce);
	thor::Action mouse_up(sf::Mouse::Left, thor::Action::ReleaseOnce);

	// thor action map
	thor::ActionMap<std::string> action_map;
	action_map["quit"] = quit;
	action_map["left"] = left;
	action_map["right"] = right;
	action_map["up"] = up;
	action_map["down"] = down;
	action_map["mouse_down"] = mouse_down;
	action_map["mouse_up"] = mouse_up;
	thor::ActionMap<std::string>::CallbackSystem action_callbacks;
	action_callbacks.connect("quit", &OnQuit);
	action_callbacks.connect("left", &OnLeft);
	action_callbacks.connect("right", &OnRight);
	action_callbacks.connect("up", &OnUp);
	action_callbacks.connect("down", &OnDown);
	action_callbacks.connect("mouse_down", &OnMouseDown);
	action_callbacks.connect("mouse_up", &OnMouseUp);

	// thor particles
	sf::Texture particleTexture;
	particleTexture.loadFromFile("Media/particle.png");
	emitter.setEmissionRate(100.0f);
	emitter.setParticleLifetime(sf::seconds(1.0f));

	particle_system.setTexture(particleTexture);

	// sfgui
	auto sfgui_window = sfg::Window::Create();
	sfgui_window->SetTitle("Runestones: ");
	auto rune_one = sfg::Button::Create("Rune One");
	rune_one->GetSignal(sfg::Button::OnMouseLeftPress).Connect(std::bind(&OnRuneOne));
	rune_one->GetSignal(sfg::Button::OnMouseLeftPress).Connect(std::bind(&OnButtonClicked));
	auto rune_two = sfg::Button::Create("Rune Two");
	rune_two->GetSignal(sfg::Button::OnMouseLeftPress).Connect(std::bind(&OnRuneTwo));
	rune_two->GetSignal(sfg::Button::OnMouseLeftPress).Connect(std::bind(&OnButtonClicked));
	auto rune_three = sfg::Button::Create("Rune Three");
	rune_three->GetSignal(sfg::Button::OnMouseLeftPress).Connect(std::bind(&OnRuneThree));
	rune_three->GetSignal(sfg::Button::OnMouseLeftPress).Connect(std::bind(&OnButtonClicked));
	auto rune_four = sfg::Button::Create("Rune Four");
	rune_four->GetSignal(sfg::Button::OnMouseLeftPress).Connect(std::bind(&OnRuneFour));
	rune_four->GetSignal(sfg::Button::OnMouseLeftPress).Connect(std::bind(&OnButtonClicked));
	auto hbox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 4.0f);
	hbox->Pack(rune_one);
	hbox->Pack(rune_two);
	hbox->Pack(rune_three);
	hbox->Pack(rune_four);
	sfgui_window->Add(hbox);
	sfgui_window->SetStyle(!sfg::Window::Style::RESIZE);
	sfgui_window->SetPosition(sf::Vector2f((WINDOW_WIDTH / 2.0f) -
		(sfgui_window->GetAllocation().width / 2.0f), WINDOW_HEIGHT - sfgui_window->GetAllocation().height));

	// tileset
	sf::Texture tileTexture;
	tileTexture.loadFromFile("resources/tiles.png");
	rf::TileMap tileMap(4, 8);
	tileMap.setTexture(tileTexture);
	int layout[4][8] = 
	{
		{-1, -1, -1, 1, 1, -1, -1, -1},
		{1,1,1,1,1,1,1,1},
		{-1,-1,1,1,1,1,-1,-1},
		{1,1,1,1,1,1,1,1}
	};

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			tileMap.setTile(layout[i][j], i, j);
		}
	}

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		action_map.clearEvents();

		sf::Event event;
		while (window.pollEvent(event))
		{
			sfgui_window->HandleEvent(event);
			action_map.pushEvent(event);
		}

		particle_system.update(sf::seconds(deltaTime));

		sfgui_window->Update(deltaTime);
		action_map.invokeCallbacks(action_callbacks, &window);
		window.setView(view);
		window.clear(sf::Color::Black);
		window.draw(map_sprite);
		window.draw(tileMap);
		window.draw(particle_system);
		sfgui.Display(window);
		window.display();
	}

	return 0;
}