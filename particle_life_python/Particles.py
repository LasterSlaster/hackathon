from abc import ABC, abstractmethod
from typing import Tuple
import numpy as np


class Particle(ABC):
    def __init__(self, m = 1.0, x0 = 0.0, y0 = 0.0) -> None:
        self.m = m    # mass
        self.x = x0   # x pos
        self.y = y0   # y pos
        self.vx = 0.0 # x velo
        self.vy = 0.0 # y velo
        self.color = "k"

        self.ax = 0.0
        self.ay = 0.0

    @abstractmethod
    def interact(self, other) -> None:
        return NotImplementedError

    def integrate(self, xlim : Tuple, ylim : Tuple) -> None:
        self.vx += self.ax *0.5
        self.vy += self.ay *0.5
        self.x += self.vx
        self.y += self.vy
        if self.x < xlim[0]:
            self.x = xlim[0]
            self.vx *= -0.8
        elif self.x > xlim[1]:
            self.x = xlim[1]
            self.vx *= -0.8
        if self.y < ylim[0]:
            self.y = ylim[0]
            self.vy *= -0.8 
        elif self.y > ylim[1]:
            self.y = ylim[1]
            self.vy *= -0.8

        self.ax = 0.0
        self.ay = 0.0

class YellowParticle(Particle):
    def __init__(self, m = 1.0, x0 = 0.0, y0 = 0.0) -> None:
        super().__init__(m,x0,y0)
        self.color = 'yellow'

    def interact(self, other):
        g = 0.0002
        dx = other.x - self.x
        dy = other.y - self.y
        d = np.hypot(dx, dy)
        if d>0.0 and d > 1.0:
            if isinstance(other, RedParticle):
                a = -(g/d) / self.m
                self.ax += a*dx
                self.ay += a*dy
            else:
                a = (g/d) / self.m
                self.ax += a*dx
                self.ay += a*dy



class RedParticle(Particle):
    def __init__(self, m=1.0, x0=0.0, y0=0.0) -> None:
        super().__init__(m, x0, y0)
        self.color = 'red'

    def interact(self, other):
        g = -0.1
        dx = other.x - self.x
        dy = other.y - self.y
        d = np.hypot(dx, dy)
        if d>0.0 and d < 80.0:
            if isinstance(other, RedParticle):
                a = (g/d) / self.m
                self.ax += a*dx
                self.ay += a*dy
            else:
                pass
                # a = -(g/d) / self.m
                    # self.ax += a*dx
                # self.ay += a*dy
