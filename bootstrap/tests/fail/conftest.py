import pytest

pytest_plugins = "pytester"
def pytest_addoption(parser):
    parser.addoption("--exe", action="store")
