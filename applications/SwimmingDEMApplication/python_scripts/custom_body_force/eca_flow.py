import KratosMultiphysics
import numpy as np
import math
## Import base class file
from custom_body_force.manufactured_solution import ManufacturedSolution

def CreateManufacturedSolution(custom_settings):
    return EcaManufacturedSolution(custom_settings)

class EcaManufacturedSolution(ManufacturedSolution):

    def __init__(self, settings):

        default_settings = KratosMultiphysics.Parameters("""
            {
                "viscosity"   : 1.846e-5,
                "density"     : 1.225,
                "U1"          : 1.0,
                "L"           : 0.5,
                "sigma"       : 4.0,
                "A"           : 15,
                "B"           : 20,
                "is_periodic" : false
            }
            """
            )

        settings.ValidateAndAssignDefaults(default_settings)

        self.rho = settings["density"].GetDouble()
        self.nu = settings["viscosity"].GetDouble()

        self.U1 = settings["U1"].GetDouble()
        self.L = settings["L"].GetDouble()
        self.sigma = settings["sigma"].GetDouble()
        self.A = settings["A"].GetDouble()
        self.B = settings["B"].GetDouble()
        self.is_periodic = settings["is_periodic"].GetBool()
        self.T = self.U1 / self.L
        self.sqrt_pi = np.sqrt(np.pi)

    def BodyForce(self, x1, x2, x3, t):
        x = x1 / self.L
        y = x2 / self.L
        z = 0
        t = t / self.T

        self.CheckIsInsideDomain(x, y)
        self.InitializeAuxiliaryValues(x, y, t)

        return super(EcaManufacturedSolution, self).BodyForce(x, y, z, t)

    def Velocity(self, x1, x2, x3, t):
        x = x1 / self.L
        y = x2 / self.L
        z = 0
        t = t / self.T

        self.CheckIsInsideDomain(x, y)
        self.InitializeAuxiliaryValues(x, y, t)

        return super(EcaManufacturedSolution, self).Velocity(x, y, z, t)

    def Pressure(self, x1, x2, x3, t):
        x = x1 / self.L
        y = x2 / self.L
        z = 0
        t = t / self.T

        self.CheckIsInsideDomain(x, y)
        self.InitializePressureAuxiliaryValues(x, y, t)

        return super(EcaManufacturedSolution, self).Pressure(x, y, z, t)

    def CheckIsInsideDomain(self, x, y):
        if not (0.5 <= x and x <= 1 and 0 <= y and y <= 1):
            message = 'The coordinates must be (x, y) = (x1/L, x2/L) in [0.5, 1] X [0, 0.5]'
            message += 'The input was (x, y) = (' + str(x) + ', ' + str(y) + ')'
            message += 'with L = ' + str(self.L) + '.'
            raise ValueError(message)

    def InitializeAuxiliaryValues(self, x, y, t):
        self.eta = self.Eta(x, y)
        self.exp_minus_eta_2 = np.exp(- self.eta**2)
        self.exp_minus_B_y = np.exp(- self.B*y)
        self.exp_minus_2_dot_5_t = np.exp(- 2.5 * t)
        self.sin_1_2x = np.sin((1 - 2*x) * np.pi)
        self.cos_1_2x = np.cos((1 - 2*x) * np.pi)
        self.cos_2pit = np.cos(2*np.pi * t)
        self.sin_2pit = np.sin(2*np.pi * t)

    def Eta(self, x, y):
        return self.sigma * y / x

    def fe(self, t):
        return 1. - self.exp_minus_2_dot_5_t

    def fp(self, t):
        return 1 - self.self.cos_2pit

    def dfe(self, t):
        return 2.5 * self.exp_minus_2_dot_5_t

    def dfp(self, t):
        pi = np.pi
        return 2*pi * self.sin_2pit

    def f(self, t):
        if self.is_periodic:
            return self.fp(t)
        else:
            return self.fe(t)

    def df(self, t):
        if self.is_periodic:
            return self.dfp(t)
        else:
            return self.dfe(t)

    def uxa(self, x1, x2, t):
        return math.erf(self.eta)

    def uxb(self, x1, x2, t):
        A = self.A
        B = self.B
        pi = np.pi
        return A*x2 * self.exp_minus_B_y * self.sin_1_2x

    def uya(self, x1, x2, t):
        return 1.0 / (self.sigma * self.sqrt_pi) * (1.0 - self.exp_minus_eta_2)

    def uyb(self, x1, x2, t):
        A = self.A
        B = self.B
        pi = np.pi
        return 2*A*pi / B**2 * self.cos_1_2x * (1 - self.exp_minus_B_y * (B*x2 + 1))

    def u1(self, x1, x2, t):
        return self.uxa(x1, x2, t) + self.uxb(x1, x2, t) * self.f(t)

    def u2(self, x1, x2, t):
        return self.uya(x1, x2, t) + self.uyb(x1, x2, t) * self.f(t)

    def du1dt(self, x1, x2, t):
        return self.uxb(x1, x2, t) * self.df(t)

    def du2dt(self, x1, x2, t):
        return self.uyb(x1, x2, t) * self.df(t)

    def du11(self, x1, x2, t):
        sigma = self.sigma
        A = self.A
        B = self.B
        pi = np.pi
        return - 2/self.sqrt_pi * sigma*x2/x1**2 * self.exp_minus_eta_2 - 2*pi*A*x2 * self.exp_minus_B_y * self.cos_1_2x * self.f(t)

    def du12(self, x1, x2, t):
        A = self.A
        B = self.B
        pi = np.pi
        return 2/self.sqrt_pi * self.sigma/x1 * self.exp_minus_eta_2 + A * self.exp_minus_B_y * self.sin_1_2x * (1 - B*x2) * self.f(t)

    def du21(self, x1, x2, t):
        sigma = self.sigma
        A = self.A
        B = self.B
        pi = np.pi
        return - 2/self.sqrt_pi * sigma*x2**2/x1**3 * self.exp_minus_eta_2 + 4*A*pi**2/B**2 * self.sin_1_2x * (1 - self.exp_minus_B_y * (B*x2 + 1)) * self.f(t)

    def du22(self, x1, x2, t):
        sigma = self.sigma
        A = self.A
        B = self.B
        pi = np.pi
        return 2/self.sqrt_pi * self.sigma*x2/x1**2 * self.exp_minus_eta_2 + 2*pi*A*x2 * self.exp_minus_B_y * self.cos_1_2x * self.f(t)

    def du111(self, x1, x2, t):
        sigma = self.sigma
        A = self.A
        B = self.B
        pi = np.pi
        eta = self.eta
        return 4/self.sqrt_pi * eta/x1**2 * self.exp_minus_eta_2 * (1 - eta**2) - 4*pi**2*A*x2 * self.exp_minus_B_y * self.sin_1_2x * self.f(t)

    def du122(self, x1, x2, t):
        sigma = self.sigma
        A = self.A
        B = self.B
        pi = np.pi
        eta = self.eta
        return - 4/self.sqrt_pi * (sigma/x1)**2 * eta * self.exp_minus_eta_2 + A*B * self.exp_minus_B_y * self.sin_1_2x * (x2*B - 2) * self.f(t)

    def du211(self, x1, x2, t):
        sigma = self.sigma
        A = self.A
        B = self.B
        pi = np.pi
        eta = self.eta
        return 2/self.sqrt_pi * self.sigma*x2**2/x1**4 * self.exp_minus_eta_2 * (3 - 2*eta**2) + 8*A*pi**3/B**2 * self.cos_1_2x * (self.exp_minus_B_y * (B*x2 + 1) - 1) * self.f(t)

    def du222(self, x1, x2, t):
        sigma = self.sigma
        A = self.A
        B = self.B
        pi = np.pi
        eta = self.eta
        return 2/self.sqrt_pi * self.sigma*x2/x1**2 * self.exp_minus_eta_2 * (1 - eta**2) + 2*A*pi*self.exp_minus_B_y * self.cos_1_2x * (1 - x2 * B) * self.f(t)

    # Pressure and derivatives

    def p(self, x1, x2, t):
        return 50 * np.log(self.pxa(x1)) * np.log(self.pya(x2)) - 0.05 * np.sin(pxb(x1) * np.pi / 2) * sin(pyb(x2) * np.pi / 2) * self.f(t)

    @staticmethod
    def pxa(x):
        return x**3/3 - 3*x**2/4 + x/2 + 11/12

    @staticmethod
    def pya(y):
        return y**2/2 + 7/8

    @staticmethod
    def pxb(x):
        return (4*x - 3)**4 - 2*(4*x - 3)**2 + 1

    @staticmethod
    def pyb(y):
        return 16*y**3 - 12*y**2 + 1
