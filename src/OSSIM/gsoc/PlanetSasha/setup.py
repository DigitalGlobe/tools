from cx_Freeze import setup, Executable

setup(
        name = "PlanetSasha",
        version = "0.1",
        description = "PlanetSasha",
        executables = [Executable("PlanetSasha.py")])