/// Test suite for the path module.
///
/// Link all test files with the `gtest_main` library to create a command line
/// test runner.
///
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "unistd.h"  // FIXME: not portable
#else
#error "path test suite requires *nix"
#endif

#include <cassert>
#include <cstdio>
#include <memory>
#include <string>
#include <stdexcept>
#include <vector>
#include <gtest/gtest.h>
#include "pypp/path.hpp"


using namespace pypp::path;

using std::invalid_argument;
using std::make_pair;
using std::pair;
using std::string;
using std::unique_ptr;
using std::vector;
using testing::Test;
using testing::Types;


/// Test the join() function.
///
TEST(path, join)
{
    ASSERT_EQ(join({"/abc/"}), "/abc/");
    ASSERT_EQ(join({"/abc", "xyz"}), "/abc/xyz");
    ASSERT_EQ(join({"/abc", "", "xyz"}), "/abc/xyz");
    ASSERT_EQ(join({"abc/", "xyz/"}), "abc/xyz/");
    ASSERT_EQ(join({"/abc/", "/xyz/"}), "/xyz/");
    ASSERT_EQ(join({"/abc//", "xyz", ""}), "/abc//xyz/");
    ASSERT_EQ(join({""}), "");
}


/// Test the split() function.
///
TEST(path, split)
{
    ASSERT_EQ(split("//abc"), make_pair(string("//"), string("abc")));
    ASSERT_EQ(split("/abc/xyz"), make_pair(string("/abc"), string("xyz")));
    ASSERT_EQ(split("abc//xyz"), make_pair(string("abc"), string("xyz")));
    ASSERT_EQ(split("abc"), make_pair(string(""), string("abc")));
    ASSERT_EQ(split("abc/"), make_pair(string("abc"), string("")));
    ASSERT_EQ(split(""), make_pair(string(""), string("")));
}


/// Test the dirname() function.
///
TEST(path, dirname)
{
    ASSERT_EQ(dirname("//abc"), "//");
    ASSERT_EQ(dirname("/abc/xyz"), "/abc");
    ASSERT_EQ(dirname("abc//xyz"), "abc");
    ASSERT_EQ(dirname("abc"), "");
    ASSERT_EQ(dirname("abc/"), "abc");
    ASSERT_EQ(dirname(""), "");
}


/// Test the basename() function.
///
TEST(path, basename)
{
    // The explicit string casts for the expected values are needed to avoid
    // spurious failures on Linux with both clang and gcc (i.e. Travis CI).
    // Without a cast, Google Test appears to test for pointer equality instead
    // of string equality. Tests pass without explicit casts on MacOS with
    // clang.
    ASSERT_EQ(basename("//abc"), string("abc"));
    ASSERT_EQ(basename("/abc/xyz"), string("xyz"));
    ASSERT_EQ(basename("abc//xyz"), string("xyz"));
    ASSERT_EQ(basename("abc"), "abc");
    ASSERT_EQ(basename("abc/"), string(""));
    ASSERT_EQ(basename(""), "");
}


/// Test the normpath() function.
///
TEST(path, normpath)
{
    ASSERT_EQ(normpath(""), ".");
    ASSERT_EQ(normpath("./."), ".");
    ASSERT_EQ(normpath("abc"), "abc");
    ASSERT_EQ(normpath("abc/"), "abc");
    ASSERT_EQ(normpath("abc/../"), ".");
    ASSERT_EQ(normpath("abc/../../.."), "../..");
    ASSERT_EQ(normpath("/"), "/");
    ASSERT_EQ(normpath("/."), "/");
    ASSERT_EQ(normpath("/abc"), "/abc");
    ASSERT_EQ(normpath("/abc/../../"), "/");
    ASSERT_EQ(normpath("/abc/.././xyz/"), "/xyz");
}


/// Test the abspath() function.
TEST(path, abspath)
{
    unique_ptr<char> buffer(new char[FILENAME_MAX]);
    assert(getcwd(buffer.get(), FILENAME_MAX));
    const string cwd(buffer.get());
    ASSERT_EQ(abspath(""), cwd);
    ASSERT_EQ(abspath("."), cwd);
    ASSERT_EQ(abspath("/"), "/");
    ASSERT_EQ(abspath("/abc"), "/abc");
    ASSERT_EQ(abspath("abc/xyz/"), cwd + "/abc/xyz");
    ASSERT_EQ(abspath("abc/../"), cwd);
}


/// Test the isabs() function.
///
TEST(path, isabs)
{
    ASSERT_FALSE(isabs(""));
    ASSERT_FALSE(isabs("abc"));
    ASSERT_TRUE(isabs("/"));
    ASSERT_TRUE(isabs("/abc"));
}


/// Test the splitext() function.
///
TEST(path, splitext)
{
    ASSERT_EQ(splitext(""), make_pair(string(""), string("")));
    ASSERT_EQ(splitext("."), make_pair(string("."), string("")));
    ASSERT_EQ(splitext(".abc"), make_pair(string(".abc"), string("")));
    ASSERT_EQ(splitext("abc."), make_pair(string("abc"), string(".")));
    ASSERT_EQ(splitext("abc.xyz"), make_pair(string("abc"), string(".xyz")));
    ASSERT_EQ(splitext("abc..xyz"), make_pair(string("abc."), string(".xyz")));
    ASSERT_EQ(splitext("abc.def.xyz"), make_pair(string("abc.def"), string(".xyz")));
}


/// Test the exists() function.
///
TEST(path, exists)
{
    ASSERT_TRUE(exists(__FILE__));
    ASSERT_FALSE(exists(""));
}


/// Test the isfile() function.
///
TEST(path, isfile)
{
    ASSERT_TRUE(isfile(__FILE__));
    ASSERT_FALSE(isfile(""));
    ASSERT_FALSE(isfile("/"));
}


/// Test the isdir() function.
///
TEST(path, isdir)
{
    ASSERT_TRUE(isdir("/"));
    ASSERT_FALSE(isdir(""));
    ASSERT_FALSE(isdir(__FILE__));
}


/// Test the PurePath constructor.
///
TEST(PurePathTest, ctor)
{
    // TODO: Just a basic smoke test for now.
    const PurePath path;
}


/// Test the PurePath std::string operator.
///
TEST(PurePathTest, string_op)
{
    // TODO: Just a basic smoke test for now.
    const PurePath path;
    ASSERT_EQ(string(path), ".");
}


/// Test the PurePath::joinpath() method.
///
TEST(PurePathTest, joinpath)
{
    // TODO: Just a basic smoke test for now.
//    const PurePath path("abc");
    //ASSERT_EQ(string(path.joinpath(PurePath("/xyz"))), "/xyz");
}


/// Test the PurePath::joinpath() method.
///
TEST(PurePathTest, joinpath_string)
{
    // TODO: Just a basic smoke test for now.
//    const PurePath path;
    //ASSERT_EQ(typeid(path.joinpath("abc")), typeid(path));
}


/// Test the PurePath join operator.
///
TEST(PurePathTest, join_op)
{
    // TODO: Just a basic smoke test for now.
    const PurePath path;
    ASSERT_EQ(typeid(path / "abc"), typeid(path));
}


/// Test the PurePath join assignment operator.
///
TEST(PurePathTest, join_assign_op)
{
    PurePath path;
    path /= "abc";
    ASSERT_EQ(string(path), "abc");
}


/// Test the PurePath equality operator.
///
TEST(PurePathTest, eq_op)
{
    ASSERT_TRUE(PurePath() == PurePath());
    ASSERT_TRUE(PurePath("abc") == PurePath("abc"));
    ASSERT_TRUE(PurePath("abc") == PurePath("./abc"));
    ASSERT_FALSE(PurePath("abc") == PurePath("/abc/"));
    ASSERT_FALSE(PurePath("abc") == PurePath());
}


/// Test the PurePath inequality operator.
///
TEST(PurePathTest, ne_op)
{
    ASSERT_TRUE(PurePath() == PurePath());
    const PurePath path("abc");
    ASSERT_TRUE(path == path);
    ASSERT_TRUE(path == PurePath("abc"));
    ASSERT_TRUE(path == PurePath("./abc"));
    ASSERT_FALSE(path == PurePath("/abc/"));
    ASSERT_FALSE(path == PurePath());
}


/// Test the PurePath::is_absolute() method.
///
TEST(PurePathTest, is_absolute)
{
    ASSERT_FALSE(PurePath().is_absolute());
    ASSERT_FALSE(PurePath("abc").is_absolute());
    ASSERT_FALSE(PurePath("./abc").is_absolute());
    ASSERT_TRUE(PurePath("/abc").is_absolute());
}


/// Test the PurePath::name() method.
///
TEST(PurePathTest, name)
{
    ASSERT_EQ(PurePath("").name(), "");
    ASSERT_EQ(PurePath(".").name(), "");
    ASSERT_EQ(PurePath("/").name(), "");
    ASSERT_EQ(PurePath(".abc").name(), ".abc");
    ASSERT_EQ(PurePath("./abc").name(), "abc");
    ASSERT_EQ(PurePath("/abc").name(), "abc");
    ASSERT_EQ(PurePath("/abc/").name(), "abc");
    ASSERT_EQ(PurePath("abc/def").name(), "def");
}


/// Test the PurePath::parts() method.
///
TEST(PurePathTest, parts)
{
    ASSERT_EQ(PurePath().parts(), vector<string>());
    ASSERT_EQ(PurePath("").parts(), vector<string>());
    ASSERT_EQ(PurePath(".").parts(), vector<string>());
    ASSERT_EQ(PurePath("./..").parts(), vector<string>({".."}));
    ASSERT_EQ(PurePath("/").parts(), vector<string>({"/"}));
    ASSERT_EQ(PurePath("./abc").parts(), vector<string>({"abc"}));
    ASSERT_EQ(PurePath("/abc").parts(), vector<string>({"/", "abc"}));
    ASSERT_EQ(PurePath("abc/def").parts(), vector<string>({"abc", "def"}));
    ASSERT_EQ(PurePath("abc//def").parts(), vector<string>({"abc", "def"}));
    ASSERT_EQ(PurePath("../abc").parts(), vector<string>({"..", "abc"}));
}


/// Test the PurePath::parent() method.
///
TEST(PurePathTest, parent)
{
    ASSERT_EQ(PurePath().parent(), PurePath());
    ASSERT_EQ(PurePath("abc").parent(), PurePath());
    ASSERT_EQ(PurePath("/abc").parent(), PurePath("/"));
    ASSERT_EQ(PurePath("abc/def").parent(), PurePath("abc"));
    ASSERT_EQ(PurePath("/").parent(), PurePath("/"));
}


/// Test the PurePath::parents() method.
///
TEST(PurePathTest, parents)
{
    ASSERT_EQ(PurePath().parents(), vector<PurePath>());
    ASSERT_EQ(PurePath("/").parents(), vector<PurePath>());
    vector<PurePath> parents({PurePath("abc"), PurePath()});
    ASSERT_EQ(PurePath("abc/def").parents(), parents);
    parents = {PurePath("/abc"), PurePath("/")};
    ASSERT_EQ(PurePath("/abc/def").parents(), parents);
}


/// Test the PurePath::relative_to() method.
///
TEST(PurePathTest, relative_to)
{
    ASSERT_THROW(PurePath("").relative_to(string("/")), std::invalid_argument);
    ASSERT_THROW(PurePath("abc").relative_to(string("def")), std::invalid_argument);
    ASSERT_THROW(PurePath("abc").relative_to(string("abc/def")), std::invalid_argument);
    ASSERT_EQ(PurePath("abc").relative_to(string("")), PurePath("abc"));
    ASSERT_EQ(PurePath("abc").relative_to(string("abc")), PurePath());
    ASSERT_EQ(PurePath("abc/def").relative_to(string("abc")), PurePath("def"));
}


/// Test the PurePath::root() method.
///
TEST(PurePathTest, root)
{
    ASSERT_EQ(PurePath().root(), "");
    ASSERT_EQ(PurePath("abc/def").root(), "");
    ASSERT_EQ(PurePath("/").root(), "/");
    ASSERT_EQ(PurePath("/abc/def").root(), "/");
}


/// Test the PurePath::stem() method.
///
TEST(PurePathTest, stem)
{
    ASSERT_EQ(PurePath().stem(), "");
    ASSERT_EQ(PurePath(".").stem(), "");
    ASSERT_EQ(PurePath("abc.").stem(), "abc.");
    ASSERT_EQ(PurePath("/abc/def.").stem(), "def.");
    ASSERT_EQ(PurePath("/abc/def.xyz").stem(), "def");
}


/// Test the PurePath::suffix() method.
///
TEST(PurePathTest, suffix)
{
    ASSERT_EQ(PurePath().suffix(), "");
    ASSERT_EQ(PurePath("abc.").suffix(), "");
    ASSERT_EQ(PurePath(".abc").suffix(), "");
    ASSERT_EQ(PurePath("abc.xyz").suffix(), ".xyz");
    ASSERT_EQ(PurePath("abc.def.xyz").suffix(), ".xyz");
}


/// Test the PurePath::suffixes() method.
///
TEST(PurePathTest, suffixes)
{
    ASSERT_EQ(PurePath().suffixes(), vector<string>());
    ASSERT_EQ(PurePath("abc.").suffixes(), vector<string>());
    ASSERT_EQ(PurePath(".abc").suffixes(), vector<string>());
    ASSERT_EQ(PurePath("abc.xyz").suffixes(), vector<string>({".xyz"}));
    ASSERT_EQ(PurePath("abc..xyz").suffixes(), vector<string>({".", ".xyz"}));
    ASSERT_EQ(PurePath("abc.def.xyz").suffixes(), vector<string>({".def", ".xyz"}));
}


/// Test the PurePath::with_name() method.
///
TEST(PurePathTest, with_name)
{
    ASSERT_THROW(PurePath("").with_name("abc"), std::invalid_argument);
    ASSERT_THROW(PurePath(".").with_name("abc"), std::invalid_argument);
    ASSERT_THROW(PurePath("/").with_name("abc"), std::invalid_argument);
    ASSERT_THROW(PurePath("abc").with_name(""), std::invalid_argument);
    ASSERT_THROW(PurePath("abc").with_name("."), std::invalid_argument);
    ASSERT_THROW(PurePath("abc").with_name("def/"), std::invalid_argument);
    ASSERT_EQ(PurePath("abc").with_name("xyz"), PurePath("xyz"));
    ASSERT_EQ(PurePath("/abc").with_name("xyz"), PurePath("/xyz"));
    ASSERT_EQ(PurePath("abc/def").with_name("xyz"), PurePath("abc/xyz"));
}


/// Test the PurePath::with_suffx() method.
///
TEST(PurePathTest, with_suffix)
{
    ASSERT_THROW(PurePath("").with_suffix(".xyz"), std::invalid_argument);
    ASSERT_THROW(PurePath(".").with_suffix(".xyz"), std::invalid_argument);
    ASSERT_THROW(PurePath("/").with_suffix(".xyz"), std::invalid_argument);
    ASSERT_THROW(PurePath("abc").with_suffix("."), std::invalid_argument);
    ASSERT_THROW(PurePath("abc").with_suffix("./"), std::invalid_argument);
    ASSERT_EQ(PurePath("abc").with_suffix(""), PurePath("abc"));
    ASSERT_EQ(PurePath("abc").with_suffix(".xyz"), PurePath("abc.xyz"));
    ASSERT_EQ(PurePath("abc.").with_suffix(".xyz"), PurePath("abc..xyz"));
    ASSERT_EQ(PurePath("abc.def").with_suffix(".xyz"), PurePath("abc.xyz"));
}
