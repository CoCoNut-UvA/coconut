import pytest
import os

# We run the commands in a tempdir, but this dir contains the ccn files.
root_dir = os.getcwd()

def build_filepath(filename):
    return root_dir + "/" + filename

@pytest.fixture()
def exec(pytestconfig):
    return pytestconfig.getoption("exe")

def test_invalid_nodeset_refence(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("invalid_nodeset_reference.ccn"))
    assert res.ret != 0

def test_double_child(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("double_child.ccn"))
    assert res.ret != 0

def test_double_root(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("double_root.ccn"))
    assert res.ret != 0

def test_double_attribute(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("double_attribute.ccn"))
    assert res.ret != 0


def test_missing_action(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("missing_action.ccn"))
    res.stderr.fnmatch_lines(["error: Could not find action: missing_action"])
    assert res.ret != 0


def test_double_child(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("double_child.ccn"))
    res.stderr.fnmatch_lines(["error: Double declaration of child name: next"])
    assert res.ret != 0


def test_empty_nodeset(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("empty_nodeset.ccn"))
    res.stderr.fnmatch_lines(["error: syntax error, unexpected '}', expecting identifier"])
    assert res.ret != 0


def test_missing_uid(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("missing_uid_trav.ccn"))
    res.stderr.fnmatch_lines(["error: syntax error, unexpected nodes, expecting uid"])
    assert res.ret != 0


def test_invalid_node_lifetime(exec, testdir):
    testdir.mkdir("ccngen/")
    testdir.mkdir("ccngen/ccngen")
    res = testdir.run(exec, build_filepath("invalid_node_lifetime.ccn"))
    res.stderr.fnmatch_lines(["error: Node lifetime can not use mandatory."])
    assert res.ret != 0
