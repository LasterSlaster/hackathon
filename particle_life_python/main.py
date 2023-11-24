import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import numpy as np

class Particle:
    def __init__(self, m = None, x0 = None, y0 = None, color='k') -> None:
        self.m = m or 1    # mass
        self.x = x0 or 0   # x pos
        self.y = y0 or 0   # y pos
        self.vx = 0 # x velo
        self.vy = 0 # y velo
        self.color = color

    def integrate(self, other, g=0.01):

        dx = other.x - self.x
        dy = other.y-self.y
        d = np.hypot(dx, dy)
        if d>0:
            a = (g/d) / self.m
        else:
            a = 0

        self.vx += a*dx
        self.vy += a*dy

        self.x += self.vx
        self.y += self.vy


def main():
    p1 = Particle()
    p2 = Particle(x0=1,y0=1,color='r')

    particles = [p1,p2]

    n_particles = len(particles)

    N = 100
    for k in range(N):
        for i in range(n_particles):
            for j in range(n_particles):
                if i==j: continue
                particles[i].integrate(particles[j])
        plt.clf()
        for particle in particles:
            plt.plot(particle.x,particle.y,'o', color=particle.color)
        plt.xlim(-1.5,1.5)
        plt.ylim(-1.5,1.5)
        plt.pause(0.01)

    plt.show()


if __name__ == '__main__':
    main()

