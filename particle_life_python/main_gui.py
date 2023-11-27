import threading
import random
from tkinter import *
import time
import copy
import numpy as np
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
from typing import List

class thread(threading.Thread):
    def __init__(self, thread_name, thread_ID):
        threading.Thread.__init__(self)
        self.thread_name = thread_name
        self.thread_ID = thread_ID
        threading.Thread.run

    def run(self, function):
        function() 

class Particle:
    counter = 1
    def __init__(self, x, y, c):
        self.x0 = x
        self.y0 = y
        self.x1 = x
        self.y1 = y
        self.vx = 0.001
        self.vy = 0.001
        self.color = c
        self.id = Particle.counter
        Particle.counter = Particle.counter + 1

def rule(particles1: List[Particle], particles2: List[Particle], g, sizeX, sizeY, radius):
    for a in particles1:
        fx = 0
        fy = 0
        for b in particles2:
            dx = a.x1 - b.x1
            dy = a.y1 - b.y1
            d = np.hypot(dx,dy)
            if d > 0 and d < radius:
                F = g * 1/d
                fx +=  F * dx
                fy += F * dy
        a.vx = (a.vx + fx) * 0.891
        a.vy = (a.vy + fy) * 0.891
        a.x0 = a.x1
        a.y0 = a.y1
        a.x1 += a.vx
        a.y1 += a.vy
        if a.x1 <= 0 or a.x1 >= sizeX:
            a.vx *= -1
            a.x1 = a.x0 + a.vx
        if a.y1 <= 0 or a.y1 >= sizeY:
            a.vy *= -1
            a.y1 = a.y0 + a.vy

class World:
    def __init__(self, x, y) -> None:
        self.ylim = y
        self.xlim = x


    # def render(self, particles : List[Particle]) -> None:
    #     plt.clf()
    #     plt.plot([-100,600], [-100,-100], color = "white")
    #     plt.plot([600,600], [-100,600], color = "white")
    #     plt.plot([600,-100], [600,500], color = "white")
    #     plt.plot([-100,-100], [600,-100], color = "white")
    #     for particle in particles:
    #         plt.plot(particle.x1,particle.y1,'o', color=particle.color)
    #     plt.xlim(*self.xlim)
    #     plt.ylim(*self.ylim)
    #     plt.axis('off')
    #     plt.pause(0.01)

    # @staticmethod
    # def draw(particles: List[Particle]):

    def create(self, count, color) -> List[Particle]:
        particles = []
        for _ in  range(count):
            randX = random.randint(0,100)/100*self.xlim
            randY = random.randint(0,100)/100*self.ylim
            particles.append(Particle(randX, randY, color))
        return particles

def main():
    world = World(500, 500)
    yellow = world.create(40, "Yellow")
    red = world.create(100, "Red")
    green = world.create(0, "Green")
    particles = yellow+red+green


    # Create canvas
    master = Tk()
    canvas_width = world.xlim
    canvas_height = world.ylim
    w = Canvas(master, width=canvas_width, height=canvas_height)
    w.pack(fill="both", expand=True)

    # Initialize ovals
    for particle in particles:
        particle_oval = w.create_oval(particle.x0,particle.y0, particle.x0+1, particle.y0+1, fill=particle.color, outline=particle.color, width=10)

    N = 1200
    steps = []
    for k in range(N):
        steps.append(copy.deepcopy(particles))
        rule(yellow, yellow, -0.01, world.xlim, world.ylim, 200)
        rule(yellow, red, - 0.01, world.xlim, world.ylim, 200)
        rule(red, yellow, 0.1, world.xlim, world.ylim, 100)
        rule(red, red, 0.02, world.xlim, world.ylim, 100)
        if k % 100 == 0:
            print(f"N = {k}/{N}")

    # Update canvas
    while True:
        for step in steps:
            w.delete("all")
            for particle in step:
                w.create_oval(particle.x1,particle.y1, particle.x1+1, particle.y1+1, fill=particle.color, outline=particle.color, width=10)
            w.update()
            time.sleep(0.01)
        time.sleep(2)

if __name__ == '__main__':
    main()
