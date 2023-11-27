from Particles import Particle, YellowParticle, RedParticle
from typing import List
import matplotlib.pyplot as plt
import random
import copy
import matplotlib
matplotlib.use('TkAgg')


class World:
    def __init__(self) -> None:
        self.xlim = (-250, 250)
        self.ylim = (-250, 250)

    def step(self, particles: List[Particle]) -> None:
        for particle in particles:
            particle.integrate(xlim=self.xlim, ylim=self.ylim)

    def render(self, particles: List[Particle]) -> None:
        plt.clf()
        for particle in particles:
            plt.plot(particle.x, particle.y, 'o', color=particle.color)
        plt.xlim(*self.xlim)
        plt.ylim(*self.ylim)
        plt.axis('off')
        plt.pause(0.10)


def main():
    p2 = RedParticle(x0=3.0, y0=1.0)
    particles = []
    particles.append(p2)

    # for _ in  range(100):
    #     randX = random.randint(-100,100)/100*4.0
    #     randY = random.randint(-100,100)/100*2.0
    #     particles.append(YellowParticle(x0=randX, y0=randY))

    for _ in range(10):
        randX = random.randint(-100, 100)/100*250.0
        randY = random.randint(-100, 100)/100*250.0
        particles.append(RedParticle(x0=randX, y0=randY))

    n_particles = len(particles)

    plt.style.use('dark_background')

    world = World()

    N = 300
    steps = []

    for k in range(N):
        # timestamp1 = time.time()
        steps.append(copy.deepcopy(particles))
        # print(f" copyying took: {time.time() - timestamp1} ms")
        # timestamp2 = time.time()
        for i in range(n_particles):
            for j in range(n_particles):
                if i == j:
                    continue
                # calculate new acceleration
                particles[i].interact(particles[j])

        world.step(particles)
        # print(f" algo took: {time.time() - timestamp2} ms")

    for step in steps:
        world.render(step)


if __name__ == '__main__':
    main()
