#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// N represents the amount of simulation steps
#define N 1000
// !!ADD NEW COLOR: Define a constant for the amount of particles of your new color and add them to the total amount of particles in MAX_PARTICLES
#define MAX_PARTICLES 30
#define COUNT_RED_PARTICLES 10
#define COUNT_YELLOW_PARTICLES 10
#define COUNT_GREEN_PARTICLES 10

typedef struct {
  int r, g, b, a;
} Color;

typedef struct {
  int x0, y0, x1, y1;
  double vx, vy;
  Color color;
  int id;
} Particle;

typedef struct {
  int xlim, ylim;
} World;

void rule(Particle** particles1, int count1, Particle** particles2, int count2,
          double g, int sizeX, int sizeY, double radius) {
  for (int i = 0; i < count1; ++i) {
    double fx = 0, fy = 0;

    for (int j = 0; j < count2; ++j) {
      double dx = particles1[i]->x1 - particles2[j]->x1;
      double dy = particles1[i]->y1 - particles2[j]->y1;
      double d = hypot(dx, dy);

      if (d > 0 && d < radius) {
        double F = g * 1 / d;
        fx += F * dx;
        fy += F * dy;
      }
    }

    particles1[i]->vx = (particles1[i]->vx + fx) * 0.891;
    particles1[i]->vy = (particles1[i]->vy + fy) * 0.891;
    particles1[i]->x0 = particles1[i]->x1;
    particles1[i]->y0 = particles1[i]->y1;
    particles1[i]->x1 += particles1[i]->vx;
    particles1[i]->y1 += particles1[i]->vy;

    if (particles1[i]->x1 <= 0 || particles1[i]->x1 >= sizeX) {
      particles1[i]->vx *= -1;
      particles1[i]->x1 = particles1[i]->x0 + particles1[i]->vx;
    }

    if (particles1[i]->y1 <= 0 || particles1[i]->y1 >= sizeY) {
      particles1[i]->vy *= -1;
      particles1[i]->y1 = particles1[i]->y0 + particles1[i]->vy;
    }
  }
}

void createParticles(Particle** particles, int count, int sizeX, int sizeY,
                     Color color) {
  for (int i = 0; i < count; ++i) {
    particles[i] = (Particle*)malloc(sizeof(Particle));
    if (particles[i] == NULL) {
      // Handle memory allocation failure
      fprintf(stderr, "Memory allocation failed.\n");
      exit(EXIT_FAILURE);
    }
    particles[i]->x0 = particles[i]->x1 = rand() % sizeX;
    particles[i]->y0 = particles[i]->y1 = rand() % sizeY;
    particles[i]->vx = 0.101;
    particles[i]->vy = 0.101;
    particles[i]->id = rand();
    particles[i]->color = color;
  }
}

void printParticles(Particle* particles, int count) {
  for (int i = 0; i < count; ++i) {
    printf("Particle %d: x=%d, y=%d, vx=%.4f, vy=%.4f, color=%d,%d,%d,%d",
           particles[i].id, particles[i].x1, particles[i].y1, particles[i].vx,
           particles[i].vy, particles[i].color.r, particles[i].color.g, particles[i].color.b, particles[i].color.a);
  }
}

void drawParticles(SDL_Renderer* renderer, Particle*** steps, size_t stepsCount,
                   size_t particleCount) {
  for (int i = 0; i < stepsCount; ++i) {
    SDL_SetRenderDrawColor(renderer, 0,0,0,0); // Set particle color (white)
    SDL_RenderClear(renderer);
    for (int j = 0; j < particleCount; ++j) {
      SDL_SetRenderDrawColor(renderer, steps[i][j]->color.r, steps[i][j]->color.g, steps[i][j]->color.b, steps[i][j]->color.a); // Set particle color (white)
      SDL_Rect rect = {steps[i][j]->x1, steps[i][j]->y1, 3, 3};
      SDL_RenderFillRect(renderer, &rect);
      /* SDL_RenderDrawPoint(renderer, steps[i][j]->x1, steps[i][j]->y1); */
    }
    SDL_RenderPresent(renderer);
    struct timespec sleepTime = {0, 10000000};
    nanosleep(&sleepTime, NULL);
  }
}

Particle** copyParticles(Particle** source, int count) {
  Particle** destination = (Particle**)malloc(count * sizeof(Particle*));

  if (destination == NULL) {
    // Handle memory allocation failure
    fprintf(stderr, "Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < count; ++i) {
    destination[i] = (Particle*)malloc(sizeof(Particle));
    if (destination[i] == NULL) {
      // Handle memory allocation failure
      fprintf(stderr, "Memory allocation failed.\n");
      exit(EXIT_FAILURE);
    }
    memcpy(destination[i], source[i], sizeof(Particle));
  }

  return destination;
}

int main() {
  // required to seed the random number generator (rand())
  srand(time(NULL));

  // Initialize world and particles
  World world = {500, 500};
  Particle** yellow = (Particle**)malloc(COUNT_YELLOW_PARTICLES * sizeof(Particle*));
  Particle** red = (Particle**)malloc(COUNT_RED_PARTICLES * sizeof(Particle*));
  Particle** green = (Particle**)malloc(COUNT_GREEN_PARTICLES * sizeof(Particle*));

  Color colorYellow;
  Color colorRed;
  Color colorGreen;
  colorYellow.r = 255;
  colorYellow.g = 255;
  colorYellow.b = 0;
  colorYellow.a = 0;
  colorRed.r = 255;
  colorRed.g = 0;
  colorRed.b = 0;
  colorRed.a = 0;
  colorGreen.r = 0;
  colorGreen.g = 200;
  colorGreen.b = 0;
  colorGreen.a = 0;

  createParticles(yellow, COUNT_YELLOW_PARTICLES, world.xlim, world.ylim, colorYellow);
  createParticles(red, COUNT_RED_PARTICLES, world.xlim, world.ylim, colorRed);
  createParticles(green, COUNT_GREEN_PARTICLES, world.xlim, world.ylim, colorGreen);
  // !!ADD NEW COLOR: Define your new color here an create a particles array for it
  if (yellow_particles == NULL || red_particles == NULL || green_particles == NULL) {
    // Handle memory allocation failure
    fprintf(stderr, "Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  // !!ADD NEW COLOR: Populate your new color with particles here
  // populate particle arrays

  // reference the color particle arrays in an aggregated particles array
  Particle* particles[MAX_PARTICLES];

  // !!ADD NEW COLOR: Add your new color to the particles array
  for (int i = 0; i < COUNT_YELLOW_PARTICLES; ++i) {
    particles[i] = yellow[i];
  }

  for (int i = 0; i < COUNT_RED_PARTICLES; ++i) {
    particles[i + COUNT_YELLOW_PARTICLES] = red[i];
  }

  for (int i = 0; i < COUNT_GREEN_PARTICLES; ++i) {
    particles[i + COUNT_YELLOW_PARTICLES + COUNT_RED_PARTICLES] = green[i];
  }

  // pointer to an pointer pointing to a pointer pointing to
  Particle*** steps = (Particle***)malloc(N * sizeof(Particle**));

  if (steps == NULL) {
    // Handle memory allocation failure
    fprintf(stderr, "Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }

  /* Particle* steps[N]; */
  for (int k = 0; k < N; ++k) {
    rule(yellow, COUNT_YELLOW_PARTICLES, yellow, COUNT_YELLOW_PARTICLES, -0.001, world.xlim, world.ylim, 80);
    rule(yellow, COUNT_YELLOW_PARTICLES, red, COUNT_RED_PARTICLES, -0.001, world.xlim, world.ylim, 100);
    rule(red, COUNT_RED_PARTICLES, yellow, COUNT_YELLOW_PARTICLES, 0.01, world.xlim, world.ylim, 50);
    rule(red, COUNT_RED_PARTICLES, red, COUNT_RED_PARTICLES, 0.002, world.xlim, world.ylim, 100);
    rule(green, COUNT_GREEN_PARTICLES, red, COUNT_RED_PARTICLES, -0.002, world.xlim, world.ylim, 100);
    // !!ADD NEW COLOR: Add your rules for new colors here

    if (k % 100 == 0) {
      printf("N = %d/%d\n", k, N);
    }
    steps[k] = copyParticles(particles, MAX_PARTICLES);
  }

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  // Create a window and renderer
  SDL_Window* window =
      SDL_CreateWindow("Particle Visualization", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

  if (window == NULL) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  // Draw particles
  drawParticles(renderer, steps, N, MAX_PARTICLES);
  printf("done");

  // Main loop
  while (1) {

    // Handle events (e.g., window close)
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 0;
      }
    }
  }

  // Uncomment the following line to print the final state of particles
  // printParticles(particles, 140);

  return 0;
}
