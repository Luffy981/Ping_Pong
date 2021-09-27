#include "pinpon.h"
SDL_Window *window= NULL;
SDL_Renderer *renderer= NULL;
const int WIDTH = 1020;
const int HEIGHT = 720;
const int BALL_SIZE = 10;
ball_t ball;

void main (void)
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	Uint32 curtick;
	Uint32 diff;

	srand((unsigned int)time(NULL));
	atexit(shut_down);
	if (initialize() == false)
	{
		SDL_LogError(2, "Cant initialize SDL: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	bool quit = false;
	SDL_Event event;
	Uint32 lastTick = SDL_GetTicks();
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		curtick = SDL_GetTicks();
		diff = curtick - lastTick;
		float elapsed = diff / 1000.0f;
		update(elapsed);
		lastTick = curtick;
	}
	exit(EXIT_SUCCESS);
}
bool initialize(void)
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) !=0)
	{
		SDL_LogError(2, "Unable to initialize SDL: %s\n", SDL_GetError());
		return(false);			
	}
	window = SDL_CreateWindow(
			"Ping Pong Game",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WIDTH,
			HEIGHT,
			SDL_WINDOW_OPENGL
			);
	if (window == NULL)
	{
		SDL_LogError(2, "Can't open window SDL: %s\n", SDL_GetError());
		return (false);	
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_LogError(2, "Can't renderer window SDL: %s\n", SDL_GetError());
		return (false);
	}
	ball = make_ball(BALL_SIZE);
	return (true);

}
ball_t make_ball(int size)
{
	const float SPEED = 120;
	ball_t ball = {
		.x = WIDTH / .2 - size / 2,
		.y = HEIGHT / .2 - size / 2,
		.size = size,
		.xSpeed = SPEED * (CoinFlip() ? 1 : -1),
		.ySpeed = SPEED * (CoinFlip() ? 1 : -1),
	};
	return (ball);
}
void update(float elapsed)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	update_ball(&ball, elapsed);
	render_ball(&ball);
	SDL_RenderPresent(renderer);
}
void update_ball(ball_t *ball, float elapsed)
{
	ball->x += ball->xSpeed * elapsed;
	ball->y += ball->ySpeed * elapsed;
	if (ball->x < BALL_SIZE / 2)
	{
		ball->xSpeed = fabs(ball->xSpeed);
	}
	if (ball->x > WIDTH - BALL_SIZE / 2)
	{
		ball->xSpeed = -fabs(ball->xSpeed);
	}
	if (ball->y < BALL_SIZE / 2)
	{
		ball->ySpeed = fabs(ball->ySpeed);
	}
	if (ball->y > HEIGHT - BALL_SIZE / 2)
	{
		ball->ySpeed = -fabs(ball->ySpeed);
	}
}
void render_ball(const ball_t *ball)
{
	int size = ball->size;
	int halfsize = size / 2;
	SDL_Rect rect = {
		.x = ball->x - halfsize,
		.y = ball->y - halfsize,
		.w = size,
		.h = size,
	};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);
}
bool CoinFlip(void)
{
	return (rand() % 2 == 1 ? true : false);
}
void shut_down(void)
{
	if (renderer)
	{
		SDL_DestroyRenderer(renderer);
	}
	if (window)
	{
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}
