"""
Attempt to setup c module.
"""
from distutils.core import setup, Extension

module1 = Extension("CPrimes", sources=["primemodule.c"])

setup(name="CPrimes", description="My first C package.", ext_modules=[module1])
