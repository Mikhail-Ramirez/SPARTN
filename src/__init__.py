# src/__init__.py

# Import the subpackages so they are available when the src package is imported
from . import sensors
from . import processing
from . import communications
from . import utils

# Optionally, you can expose a main function from the main driver script
from .main import main

