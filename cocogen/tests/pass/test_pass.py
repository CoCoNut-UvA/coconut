import pytest
import os

# We run the commands in a tempdir, but this dir contains the ccn files.
root_dir = os.getcwd()

def build_filepath(filename):
    return root_dir + "/" + filename

@pytest.fixture()
def exec(pytestconfig):
    return pytestconfig.getoption("exe")


def test_travdata(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("travdata.ccn"))
    assert res.ret == 0


def test_simple(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("simple.ccn"))
    assert res.ret == 0


def test_nodeset(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("nodesets.ccn"))
    assert res.ret == 0

