/// Test suite for the path module.
///
/// Link all test files with the `gtest_main` library to create a command line
/// test runner.
///
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "unistd.h"
#else
#error "path test suite requires *nix"
#endif

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <memory>
#include <set>
#include <string>
#include <stdexcept>
#include <typeinfo>
#include <vector>
#include <gtest/gtest.h>
#include "pypp/path.hpp"
#include "pypp/tempfile.hpp"


using namespace pypp::path;

using pypp::tempfile::TemporaryDirectory;
using std::invalid_argument;
using std::begin;
using std::end;
using std::fstream;
using std::getline;
using std::ios;
using std::make_pair;
using std::pair;
using std::runtime_error;
using std::set;
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
///
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


/// Test the islink() function.
///
TEST(path, islink)
{
    const TemporaryDirectory tmpdir;
    const auto link(join({tmpdir.name(), "test_islink"}));
    symlink(__FILE__, link.c_str());
    ASSERT_TRUE(islink(link));
    ASSERT_FALSE(islink("/"));  // directory
    ASSERT_FALSE(islink(__FILE__));  // file
    ASSERT_FALSE(islink(""));  // doesn't exist
}


/// Test Fixture for PurePath unit tests.
///
/// The fixture will be type-parametrized over all classes that implement the
/// PurePath interface.
///
template <typename T>
class PurePathTest: public Test {};
using PurePathTypes = Types<PurePath, Path>;
TYPED_TEST_CASE(PurePathTest, PurePathTypes);


/// Test the PurePath equality operator.
///
TYPED_TEST(PurePathTest, eq_op)
{
    ASSERT_TRUE(TypeParam() == TypeParam());
    ASSERT_TRUE(TypeParam("abc") == TypeParam("abc"));
    ASSERT_TRUE(TypeParam("abc") == TypeParam("./abc"));
    ASSERT_FALSE(TypeParam("abc") == TypeParam("/abc/"));
    ASSERT_FALSE(TypeParam("abc") == TypeParam());
    static const TypeParam path("abc");
    ASSERT_TRUE(path == path);  // identity
}


/// Test the PurePath inequality operator.
///
TYPED_TEST(PurePathTest, ne_op)
{
    ASSERT_FALSE(TypeParam() != TypeParam());
    ASSERT_FALSE(TypeParam("abc") != TypeParam("abc"));
    ASSERT_FALSE(TypeParam("abc") != TypeParam("./abc"));
    ASSERT_TRUE(TypeParam("abc") != TypeParam("/abc/"));
    ASSERT_TRUE(TypeParam("abc") != TypeParam());
    static const TypeParam path("abc");
    ASSERT_FALSE(path != path);  // identity
}


/// Test the PurePath less-than operator.
///
TYPED_TEST(PurePathTest, lt_op)
{
    static const TypeParam path("abc");
    ASSERT_FALSE(path < path);  // identity
    ASSERT_TRUE(TypeParam("abc") < TypeParam("abd"));
    ASSERT_FALSE(TypeParam("abd") < TypeParam("abc"));
    ASSERT_FALSE(TypeParam("abc") < TypeParam("./abc"));
    ASSERT_FALSE(TypeParam("./abc") < TypeParam("abc"));
}


/// Test the PurePath std::string operator.
///
TYPED_TEST(PurePathTest, string_op)
{
    ASSERT_EQ(".", string(TypeParam()));
    ASSERT_EQ(".", string(TypeParam(".")));
    ASSERT_EQ(".", string(TypeParam("./")));
    ASSERT_EQ("/", string(TypeParam("/")));
    ASSERT_EQ("/abc", string(TypeParam("/abc")));
    ASSERT_EQ("abc", string(TypeParam("abc")));
    ASSERT_EQ("abc", string(TypeParam("abc/")));
}


/// Test the PurePath::is_absolute() method.
///
TYPED_TEST(PurePathTest, is_absolute)
{
    ASSERT_FALSE(TypeParam().is_absolute());
    ASSERT_FALSE(TypeParam("abc").is_absolute());
    ASSERT_FALSE(TypeParam("./abc").is_absolute());
    ASSERT_TRUE(TypeParam("/abc").is_absolute());
}


/// Test the PurePath::name() method.
///
TYPED_TEST(PurePathTest, name)
{
    ASSERT_EQ("", TypeParam("").name());
    ASSERT_EQ("", TypeParam(".").name());
    ASSERT_EQ("", TypeParam("/").name());
    ASSERT_EQ(".abc", TypeParam(".abc").name());
    ASSERT_EQ("abc", TypeParam("./abc").name());
    ASSERT_EQ("abc", TypeParam("/abc").name());
    ASSERT_EQ("abc", TypeParam("/abc/").name());
    ASSERT_EQ("def", TypeParam("abc/def").name());
}


/// Test the PurePath::parts() method.
///
TYPED_TEST(PurePathTest, parts)
{
    ASSERT_EQ(vector<string>(), TypeParam().parts());
    ASSERT_EQ(vector<string>(), TypeParam("").parts());
    ASSERT_EQ(vector<string>(), TypeParam(".").parts());
    ASSERT_EQ(vector<string>({".."}), TypeParam("./..").parts());
    ASSERT_EQ(vector<string>({"/"}), TypeParam("/").parts());
    ASSERT_EQ(vector<string>({"abc"}), TypeParam("./abc").parts());
    ASSERT_EQ(vector<string>({"/", "abc"}), TypeParam("/abc").parts());
    ASSERT_EQ(vector<string>({"abc", "def"}), TypeParam("abc/def").parts());
    ASSERT_EQ(vector<string>({"abc", "def"}), TypeParam("abc//def").parts());
    ASSERT_EQ(vector<string>({"..", "abc"}), TypeParam("../abc").parts());
}


/// Test the PurePath::root() method.
///
TYPED_TEST(PurePathTest, root)
{
    ASSERT_EQ("", TypeParam().root());
    ASSERT_EQ("", TypeParam("abc/def").root());
    ASSERT_EQ("/", TypeParam("/").root());
    ASSERT_EQ("/", TypeParam("/abc/def").root());
}


/// Test the PurePath::stem() method.
///
TYPED_TEST(PurePathTest, stem)
{
    ASSERT_EQ("", TypeParam().stem());
    ASSERT_EQ("", TypeParam(".").stem());
    ASSERT_EQ("abc.", TypeParam("abc.").stem());
    ASSERT_EQ("def.", TypeParam("/abc/def.").stem());
    ASSERT_EQ("def", TypeParam("/abc/def.xyz").stem());
}


/// Test the PurePath::suffix() method.
///
TYPED_TEST(PurePathTest, suffix)
{
    ASSERT_EQ("", TypeParam().suffix());
    ASSERT_EQ("", TypeParam("abc.").suffix());
    ASSERT_EQ("", TypeParam(".abc").suffix());
    ASSERT_EQ(".xyz", TypeParam("abc.xyz").suffix());
    ASSERT_EQ(".xyz", TypeParam("abc.def.xyz").suffix());
}


/// Test the PurePath::suffixes() method.
///
TYPED_TEST(PurePathTest, suffixes)
{
    ASSERT_EQ(vector<string>(), TypeParam().suffixes());
    ASSERT_EQ(vector<string>(), TypeParam("abc.").suffixes());
    ASSERT_EQ(vector<string>(), TypeParam(".abc").suffixes());
    ASSERT_EQ(vector<string>({".xyz"}), TypeParam("abc.xyz").suffixes());
    ASSERT_EQ(vector<string>({".", ".xyz"}), TypeParam("abc..xyz").suffixes());
    ASSERT_EQ(vector<string>({".def", ".xyz"}), TypeParam("abc.def.xyz").suffixes());
}


/// Test the PurePath::joinpath() method.
///
TYPED_TEST(PurePathTest, joinpath)
{
    ASSERT_EQ(TypeParam(), TypeParam().joinpath(TypeParam()));
    ASSERT_EQ(TypeParam("/"), TypeParam().joinpath(TypeParam("/")));
    ASSERT_EQ(TypeParam("abc"), TypeParam().joinpath(TypeParam("abc")));
    ASSERT_EQ(TypeParam("abc/def"), TypeParam("abc").joinpath(TypeParam("def/")));
    ASSERT_EQ(TypeParam("abc/def"), TypeParam("abc").joinpath("def/"));
    //ASSERT_EQ(TypeParam("/def"), TypeParam("abc").joinpath(TypeParam("/def")));  // FIXME
}


/// Test the PurePath join operator.
///
TYPED_TEST(PurePathTest, join_op)
{
    ASSERT_EQ(TypeParam(), TypeParam() / TypeParam());
    ASSERT_EQ(TypeParam("/"), TypeParam() / TypeParam("/"));
    ASSERT_EQ(TypeParam("abc"), TypeParam() / TypeParam("abc"));
    ASSERT_EQ(TypeParam("abc/def"), TypeParam("abc") / TypeParam("def/"));
    ASSERT_EQ(TypeParam("abc/def"), TypeParam("abc") / "def");
    //ASSERT_EQ(TypeParam("/def"), TypeParam("abc") / TypeParam("/def"));  // FIXME
}


/// Test the PurePath join assignment operator.
///
TYPED_TEST(PurePathTest, join_assign_op)
{
    TypeParam path("abc");
    path /= "def";
    ASSERT_EQ(TypeParam("abc/def"), path);
}


/// Test the PurePath::parent() method.
///
TYPED_TEST(PurePathTest, parent)
{
    ASSERT_EQ(TypeParam(), TypeParam().parent());
    ASSERT_EQ(TypeParam(), TypeParam("abc").parent());
    ASSERT_EQ(TypeParam("/"), TypeParam("/abc").parent());
    ASSERT_EQ(TypeParam("abc"), TypeParam("abc/def").parent());
    ASSERT_EQ(TypeParam("abc/def"), TypeParam("abc/def/xyz").parent());
    ASSERT_EQ(TypeParam("/"), TypeParam("/").parent());
}


/// Test the PurePath::parents() method.
///
TYPED_TEST(PurePathTest, parents)
{
    ASSERT_EQ(vector<TypeParam>(), TypeParam().parents());
    ASSERT_EQ(vector<TypeParam>(), TypeParam("/").parents());
    ASSERT_EQ(vector<TypeParam>({TypeParam("abc"), TypeParam()}), TypeParam("abc/def").parents());
    ASSERT_EQ(vector<TypeParam>({TypeParam("/abc"), TypeParam("/")}), TypeParam("/abc/def").parents());
}


/// Test the PurePath::relative_to() method.
///
TYPED_TEST(PurePathTest, relative_to)
{
    ASSERT_THROW(TypeParam("").relative_to(TypeParam("/")), invalid_argument);
    ASSERT_THROW(TypeParam("abc").relative_to(TypeParam("def")), invalid_argument);
    ASSERT_THROW(TypeParam("abc").relative_to(TypeParam("abc/def")), invalid_argument);
    ASSERT_EQ(TypeParam("abc"), TypeParam("abc").relative_to(TypeParam("")));
    ASSERT_EQ(TypeParam(), TypeParam("abc").relative_to(TypeParam("abc")));
    ASSERT_EQ(TypeParam("def"), TypeParam("abc/def").relative_to(TypeParam("abc")));
}



/// Test the PurePath::with_name() method.
///
TYPED_TEST(PurePathTest, with_name)
{
    ASSERT_THROW(TypeParam("").with_name("abc"), invalid_argument);
    ASSERT_THROW(TypeParam(".").with_name("abc"), invalid_argument);
    ASSERT_THROW(TypeParam("/").with_name("abc"), invalid_argument);
    ASSERT_THROW(TypeParam("abc").with_name(""), invalid_argument);
    ASSERT_THROW(TypeParam("abc").with_name("."), invalid_argument);
    ASSERT_THROW(TypeParam("abc").with_name("def/"), invalid_argument);
    ASSERT_EQ(TypeParam("xyz"), TypeParam("abc").with_name("xyz"));
    ASSERT_EQ(TypeParam("/xyz"), TypeParam("/abc").with_name("xyz"));
    ASSERT_EQ(TypeParam("abc/xyz"), TypeParam("abc/def").with_name("xyz"));
}


/// Test the PurePath::with_suffx() method.
///
TYPED_TEST(PurePathTest, with_suffix)
{
    ASSERT_THROW(TypeParam("").with_suffix(".xyz"), invalid_argument);
    ASSERT_THROW(TypeParam(".").with_suffix(".xyz"), invalid_argument);
    ASSERT_THROW(TypeParam("/").with_suffix(".xyz"), invalid_argument);
    ASSERT_THROW(TypeParam("abc").with_suffix("."), invalid_argument);
    ASSERT_THROW(TypeParam("abc").with_suffix("./"), invalid_argument);
    ASSERT_EQ(TypeParam("abc"), TypeParam("abc").with_suffix(""));
    ASSERT_EQ(TypeParam("abc.xyz"), TypeParam("abc").with_suffix(".xyz"));
    ASSERT_EQ(TypeParam("abc..xyz"), TypeParam("abc.").with_suffix(".xyz"));
    ASSERT_EQ(TypeParam("abc.xyz"), TypeParam("abc.def").with_suffix(".xyz"));
}


/// Test fixture for Path methods that are not part of the PurePath interface.
///
/// This is used to group tests and provide common set-up and tear-down code.
/// A new test fixture is created for each test to prevent any side effects
/// between tests. Member variables and methods are injected into each test
/// that uses this fixture.
///
class PathTest: public Test
{
protected:
    const TemporaryDirectory tmpdir;
};


/// Test the Path::cwd() method.
///
TEST_F(PathTest, cwd)
{
    ASSERT_EQ(abspath("."), string(Path::cwd()));
}


/// Test the Path::pure() method.
///
TEST_F(PathTest, pure)
{
    ASSERT_EQ(PurePath("abc"), Path("abc").pure());
}


/// Test the Path::exists() method.
///
TEST_F(PathTest, exists)
{
    Path path(tmpdir.name());
    ASSERT_TRUE(path.exists());  // directory
    path /= "exists_test";
    ASSERT_FALSE(path.exists());
    path.open("wt");
    ASSERT_TRUE(path.exists());  // file
}


/// Test the Path::is_dir() method.
///
TEST_F(PathTest, is_dir)
{
    Path path(tmpdir.name());
    ASSERT_TRUE(path.is_dir());
    path /= "is_dir_test";
    ASSERT_FALSE(path.is_dir());  // does not exist
    path.open("wt");
    ASSERT_FALSE(path.is_dir());  // not a directory
}


/// Test the Path::is_file() method.
///
TEST_F(PathTest, is_file)
{
    Path path(tmpdir.name());
    ASSERT_FALSE(path.is_file());  // not a file
    path /= "is_file_test";
    ASSERT_FALSE(path.is_file());  // does not exist
    path.open("wt");  // create empty file
    ASSERT_TRUE(path.is_file());
}


/// Test the Path::is_symlink_test() method.
///
TEST_F(PathTest, is_symlink)
{
    Path path(Path(tmpdir.name()) / "is_symlink_test");
    ASSERT_FALSE(path.is_symlink());  // does not exist
    symlink(__FILE__, string(path).c_str());
    ASSERT_TRUE(path.is_symlink());
}


/// Test the Path::open() method.
///
TEST_F(PathTest, open)
{
    // The only reliable way to check the validity of the returned stream is to
    // attempt an I/O operation on it.
    const auto path(Path(tmpdir.name()) / "open_test");
    ASSERT_TRUE(path.open("xt").put('a'));
    ASSERT_EQ('a', path.open("rt").get());
    ASSERT_TRUE(path.open("at").put('b'));
    auto file(path.open("rt"));
    ASSERT_EQ('a', file.get());
    ASSERT_EQ('b', file.get());
    ASSERT_TRUE(path.open("wt").put('c'));
    ASSERT_EQ('c', path.open("rt").get());
    ASSERT_EQ(EOF, path.open("xt").get());  // error because file exists
}


/// Test the Path::mkdir() method.
///
TEST_F(PathTest, mkdir)
{
    const auto path(Path(tmpdir.name()) / "mkdir_test" / "subdir");
    ASSERT_THROW(path.mkdir(), runtime_error);  // parent doesn't exist
    path.mkdir(0777, true);
    ASSERT_TRUE(path.is_dir());
    ASSERT_THROW(path.mkdir(), runtime_error);  // already exists
    ASSERT_NO_THROW(path.mkdir(0777, false, true));  // exist_ok
}


/// Test the Path::symlink_to() method.
///
TEST_F(PathTest, symlink_to)
{
    const auto path(Path(tmpdir.name()) / "symlink_to_test");
    path.symlink_to(Path(__FILE__));
    ASSERT_TRUE(path.is_symlink());
    ASSERT_THROW(path.symlink_to(string(__FILE__)), runtime_error);  // already exists
}


/// Test the Path::unlink() method.
///
TEST_F(PathTest, unlink)
{
    const auto path(Path(tmpdir.name()) / "unlink_test");
    path.open("wt");
    path.unlink();
    ASSERT_FALSE(path.exists());
    ASSERT_THROW(path.unlink(), runtime_error);  // doesn't exist
}


/// Test the Path::rmdir() method.
///
TEST_F(PathTest, rmdir)
{
    const auto path(Path(tmpdir.name()) / "rmdir_test");
    const auto file(path / "file");
    ASSERT_THROW(path.rmdir(), runtime_error);  // doesn't exist
    path.mkdir();
    file.open("wt");
    ASSERT_THROW(path.rmdir(), runtime_error);  // not empty
    file.unlink();
    path.rmdir();
    ASSERT_FALSE(path.exists());
    ASSERT_THROW(path.rmdir(), runtime_error);  // doesn't exist
}


/// Test the Path::read_bytes() method.
///
TEST_F(PathTest, read_bytes)
{
    static const string data("Some test data");  // TODO: test non-ASCII bytes
    const auto path(Path(tmpdir.name()) / "read_bytes_test");
    path.open("wb").write(data.c_str(), data.size());
    ASSERT_EQ(data, path.read_bytes());
}


/// Test the Path::read_text() method.
///
TEST_F(PathTest, read_text)
{
    static const string data("Some test data");
    const auto path(Path(tmpdir.name()) / "read_text_test");
    path.open("wt").write(data.c_str(), data.size());
    ASSERT_EQ(data, path.read_text());
}


/// Test the Path::write_bytes() method.
///
TEST_F(PathTest, write_bytes)
{
    static const string data("Some test data");  // TODO: test non-ASCII bytes
    const auto path(Path(tmpdir.name()) / "write_bytes_test");
    path.write_bytes(data);
    string line;
    getline(path.open("rb"), line);
    ASSERT_EQ(data, line);
}


/// Test the Path::write_text() method.
///
TEST_F(PathTest, write_text)
{
    static const string data("Some test data");
    const auto path(Path(tmpdir.name()) / "write_text_test");
    path.write_text(data);
    string line;
    getline(path.open("rt"), line);
    ASSERT_EQ(data, line);
}


/// Test the Path::iterdir() method.
///
TEST_F(PathTest, iterdir)
{
    const TemporaryDirectory tmpdir;
    const Path root(tmpdir.name());
    const auto file(root / "file");
    file.open("wt");
    const auto dir(root / "dir");
    dir.mkdir();
    const auto paths(root.iterdir());
    set<Path> items(begin(paths), end(paths));  // need guaranteed ordering
    ASSERT_EQ(set<Path>({dir, file}), items);
    ASSERT_THROW(file.iterdir(), runtime_error);  // not a directory
}
