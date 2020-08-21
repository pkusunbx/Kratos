from __future__ import print_function, absolute_import, division

import sys
import types
import builtins

import line_profiler

import KratosMultiphysics
import KratosMultiphysics.decorator_injector as DecroatorInjector
import KratosMultiphysics.KratosUnittest as KratosUnittest

def GetFilePath(fileName):
    return os.path.join(os.path.dirname(os.path.realpath(__file__)), fileName)

def dummy_decorator_repeat(func):
    def wrapper(*args, **kwargs):
        func(*args, **kwargs)
        func(*args, **kwargs)
    return wrapper

class TestNodalDivergence(KratosUnittest.TestCase):

    @classmethod
    def setUpClass(cls):
        cls.prof = line_profiler.LineProfiler()
        if 'profile' not in builtins.__dict__ or builtins.__dict__['profile'] is None:
            builtins.__dict__['profile'] = cls.prof

    @classmethod
    def tearDownClass(cls):
        cls.prof.print_stats()

    def setUp(self):
        def add_once(input):
            input[0] += 1

        @dummy_decorator_repeat
        def add_once_decorated(input):
            input[0] += 1

        # Generate a virtual module. This should be equivalent to "import VirtualModule"
        virtual_module = types.ModuleType('VirtualModule', 'Module created to provide a context for the decorator injector')
        virtual_module.__dict__.update({
            "add_once":add_once,
            "add_once_decorated":add_once_decorated
        })

        sys.modules['VirtualModule'] = virtual_module
        self.virtual_module = virtual_module

    def test_virtual_module(self):
        a = [0]
        self.virtual_module.add_once(a)
        self.assertEqual(a[0], 1)

    def test_virtual_module_decorated(self):
        a = [0]
        self.virtual_module.add_once_decorated(a)
        self.assertEqual(a[0], 2)

    def test_virtual_module_injected(self):
        DecroatorInjector.InjectIntoModule(self.virtual_module, dummy_decorator_repeat, lambda *x: True)

        a = [0]
        self.virtual_module.add_once(a)
        self.assertEqual(a[0], 2)

    def test_virtual_module_decorated_injected(self):
        DecroatorInjector.InjectIntoModule(self.virtual_module, dummy_decorator_repeat, lambda *x: True)

        a = [0]
        self.virtual_module.add_once_decorated(a)
        self.assertEqual(a[0], 4)

    def test_virtual_module_profiler_injected(self):
        DecroatorInjector.InjectIntoModule(self.virtual_module, builtins.__dict__['profile'], lambda *x: True)

        a = [0]
        self.virtual_module.add_once(a)
        self.assertEqual(a[0], 1)

    # This tests reports a sucess, but its intention is to test if we can inject the @profile UNDER the code's
    # existing decorator, otherwhie it will profile the wrapper instead of the function ( which is essentially
    # the current behaviour)
    def test_virtual_module_profiler_decorated_injected(self):
        DecroatorInjector.InjectIntoModule(self.virtual_module, builtins.__dict__['profile'], lambda *x: True)

        a = [0]
        self.virtual_module.add_once_decorated(a)
        self.assertEqual(a[0], 2)

    def tearDown(self):
        pass

if __name__ == '__main__':
    KratosUnittest.main()
