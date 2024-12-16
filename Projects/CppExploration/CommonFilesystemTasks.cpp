

// @brief Code from the article 22 Common Filesystem Tasks in C++20
// https://www.cppstories.com/2024/common-filesystem-cpp20/

#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <print>
#include <string>
#include <thread>
#include <vector>

#include "catch2/benchmark/catch_benchmark.hpp"
#include "catch2/catch_test_macros.hpp"

void ls()
{
  for (const auto& entry : std::filesystem::directory_iterator("."))
  {
    std::println("{}", entry.path().string());
  }

  for (const auto& entry : std::filesystem::recursive_directory_iterator("."))
  {
    std::println("{}", entry.path().string());
  }
}

TEST_CASE("CommonFilesystemTasks", "[Filesystem]")
{
  SECTION("Creating Directories (exceptions)")
  {
    std::filesystem::path dir = "example_directory";
    REQUIRE_NOTHROW(std::filesystem::create_directory(dir));
  }

  SECTION("Removing a Directory")
  {
    std::filesystem::path dir = "example_directory";
    REQUIRE_NOTHROW(std::filesystem::remove(dir));
  }

  SECTION("Creating Directories (error_code)")
  {
    std::filesystem::path dir = "example_directory_error_code";
    std::error_code ec{};
    REQUIRE(std::filesystem::create_directory(dir, ec));
    REQUIRE(!ec);
    REQUIRE_NOTHROW(std::filesystem::remove(dir));
  }

  SECTION("Creating Nested Directories")
  {
    std::filesystem::path nested = "a/b/c";
    REQUIRE_NOTHROW(std::filesystem::create_directories(nested));
    REQUIRE_NOTHROW(std::filesystem::remove(nested));
    REQUIRE_NOTHROW(std::filesystem::remove(nested.parent_path()));
    REQUIRE_NOTHROW(std::filesystem::remove(nested.parent_path().parent_path()));
  }

  SECTION("Removing All Contents of a Directory")
  {
    std::filesystem::path dir = "test";
    std::filesystem::path nested = dir / "a/b";
    std::filesystem::path more = dir / "x/y";
    REQUIRE_NOTHROW(std::filesystem::create_directories(nested) && std::filesystem::create_directories(more));

    ls();

    const auto count = std::filesystem::remove_all(dir);
    REQUIRE(count > 0);
  }

  SECTION("Creating and Using a Temporary Directory")
  {
    std::filesystem::path temp_dir = std::filesystem::temp_directory_path() / "my_temp_directory";
    REQUIRE_NOTHROW(std::filesystem::create_directory(temp_dir));
    REQUIRE_NOTHROW(std::filesystem::remove(temp_dir));
  }

  SECTION("Copying Files")
  {
    std::filesystem::path src = "source_file.txt";
    std::filesystem::path dst = "destination_file.txt";
    std::ofstream(src) << "This is file 1";
    REQUIRE_NOTHROW(std::filesystem::copy(src, dst));
    REQUIRE_NOTHROW(std::filesystem::remove(src));
    REQUIRE_NOTHROW(std::filesystem::remove(dst));
  }

  SECTION("Copying Files Recursively")
  {
    std::filesystem::path src = "source_directory";
    std::filesystem::path dst = "destination_directory";
    std::filesystem::path pathA = src / "subdir1";
    std::filesystem::path pathB = src / "subdir2";

    std::filesystem::create_directories(pathA);
    std::filesystem::create_directories(pathB);

    std::ofstream(src / "file1.txt") << "This is file 1";
    std::ofstream(pathA / "file2.txt") << "This is file 2";
    std::ofstream(pathB / "file3.txt") << "This is file 3";
    REQUIRE_NOTHROW(std::filesystem::copy(src, dst, std::filesystem::copy_options::recursive));
    REQUIRE_NOTHROW(std::filesystem::remove_all(src) > 0);
    REQUIRE_NOTHROW(std::filesystem::remove_all(dst) > 0);
  }

  SECTION("Moving and Renaming Files")
  {
    std::ofstream("old_file.txt") << "This is file 1";
    std::filesystem::path old_name = "old_file.txt";
    std::filesystem::path new_name = "new_file.txt";

    REQUIRE_NOTHROW(std::filesystem::rename(old_name, new_name));
    REQUIRE_NOTHROW(std::filesystem::remove(new_name));
  }

  // Hard links provide multiple directory entries for a single file.
  // Use the create_hard_link function to create them.
  SECTION("Creating Hard Links")
  {
    std::ofstream("target_file.txt") << "This is file 1";
    std::filesystem::path target = "target_file.txt";
    std::filesystem::path link = "hard_link_file.txt";

    REQUIRE_NOTHROW(std::filesystem::create_hard_link(target, link));
    REQUIRE_NOTHROW(std::filesystem::remove(target));
    REQUIRE_NOTHROW(std::filesystem::remove(link));
  }

  // Symbolic links (symlinks) are another way to reference files.
  // https://stackoverflow.com/questions/185899/what-is-the-difference-between-a-symbolic-link-and-a-hard-link
  SECTION("Creating Symbolic Links")
  {
    std::filesystem::path original_file = "original_file.txt";
    std::filesystem::path symlink = "symlink_to_file.txt";

    // Step 1: Create the original file
    std::ofstream(original_file) << "Hello World!";

    REQUIRE_NOTHROW(std::filesystem::create_symlink(original_file, symlink));
    REQUIRE_NOTHROW(std::filesystem::remove(original_file));
    REQUIRE_NOTHROW(std::filesystem::remove(symlink));
  }

  SECTION("Checking File or Directory Existence")
  {
    std::filesystem::path p = "example_file.txt";
    REQUIRE(std::filesystem::exists(p) == false);
  }

  SECTION("Checking if a Path is a File or Directory")
  {
    std::filesystem::path dir = "source_directory";

    REQUIRE_NOTHROW(std::filesystem::create_directory(dir));

    std::filesystem::path filepath = dir / "file1.txt";
    std::ofstream(filepath) << "This is file 1";

    REQUIRE(std::filesystem::is_regular_file(filepath) == true);
    REQUIRE(std::filesystem::is_directory(dir) == true);

    REQUIRE_NOTHROW(std::filesystem::remove_all(dir) > 0);
  }

  SECTION("Reading Symlink Status")
  {
    std::filesystem::path original_file = "original_file.txt";
    std::filesystem::path symlink = "symlink_to_file.txt";

    std::ofstream(original_file) << "Hello World!";

    REQUIRE_NOTHROW(std::filesystem::create_symlink(original_file, symlink));
    REQUIRE_NOTHROW(std::filesystem::is_symlink(symlink));
    std::filesystem::path target = std::filesystem::read_symlink(symlink);
    REQUIRE(target == original_file);
    REQUIRE_NOTHROW(std::filesystem::remove(original_file));
    REQUIRE_NOTHROW(std::filesystem::remove(symlink));
  }

  SECTION("Getting Absolute Path")
  {
    std::filesystem::path relative_path1 = "example_directory";
    std::filesystem::path relative_path2 = "../parent_directory";
    std::filesystem::path relative_path3 = "subdir/another_file.txt";

    REQUIRE_NOTHROW(std::filesystem::absolute(relative_path1));
    REQUIRE_NOTHROW(std::filesystem::absolute(relative_path2));
    REQUIRE_NOTHROW(std::filesystem::absolute(relative_path3));
  }

  SECTION("Getting Relative Path")
  {
    std::filesystem::path base_path = "/home/user";
    std::filesystem::path absolute_path = "/home/user/example_directory/file.txt";
    REQUIRE_NOTHROW(std::filesystem::relative(absolute_path, base_path));
  }

  // According to CppReference the stream operator for the path class is defined in the following way:
  // Performs stream input or output on the path p. std::quoted is used so that spaces do not cause truncation when later read by stream input operator.
  // To remove the quotes, we just need to get the raw string that represents the path.
  SECTION("Displaying paths without quotes")
  {
    std::filesystem::path base_path = "/home/user";
    std::filesystem::path absolute_path = "/home/user/example_directory/file.txt";
    // std::cout << absolute_path << "\n";
    // std::wcout << absolute_path.c_str() << "\n";
  }

  SECTION("Calculating Directory Size")
  {
    // Create a test directory with some files and subdirectories
    std::filesystem::path test_dir = "test_directory";

    std::filesystem::create_directories(test_dir / "subdir1");
    std::filesystem::create_directories(test_dir / "subdir2");

    std::ofstream(test_dir / "file1.txt") << "ABC";
    std::ofstream(test_dir / "subdir1/file2.txt") << "XYZ";
    std::ofstream(test_dir / "subdir2/file3.txt") << "123";

    // Calculate the total size of the test directory
    auto calculate_directory_size = [](const std::filesystem::path& dir) {
      std::uintmax_t size = 0;
      for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
      {
        if (std::filesystem::is_regular_file(entry.path()))
        {
          size += std::filesystem::file_size(entry.path());
        }
      }
      return size;
    };

    auto total_size = calculate_directory_size(test_dir);

    REQUIRE(total_size == 9);

    // Clean up by removing the test directory and its contents
    std::filesystem::remove_all(test_dir);
  }

  SECTION("Determining Free Space on a Filesystem")
  {
    std::filesystem::path p = "/";
    auto space_info = std::filesystem::space(p);

    REQUIRE(space_info.free > 0);
    REQUIRE(space_info.available > 0);
    REQUIRE(space_info.capacity > 0);
  }

  SECTION("Checking File Permissions")
  {
    std::filesystem::path filePath = "example_file.txt";

    std::ofstream(filePath) << "Hello World!";

    std::filesystem::perms p = std::filesystem::status(filePath).permissions();

    std::filesystem::remove(filePath);

    REQUIRE((p & std::filesystem::perms::owner_read) != std::filesystem::perms::none);
    REQUIRE((p & std::filesystem::perms::owner_write) != std::filesystem::perms::none);
    REQUIRE((p & std::filesystem::perms::owner_exec) != std::filesystem::perms::none);

    // std::cout << "Permissions for " << filePath << ":\n";
    // std::cout << ((p & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? "r" : "-")
    //           << ((p & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? "w" : "-")
    //           << ((p & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ? "x" : "-") << '\n';
  }

  SECTION("Setting File Permissions")
  {
    std::filesystem::path test_file = "test_file.txt";
    std::ofstream(test_file) << "This is a test file.";

    auto display_permissions = [](const std::filesystem::path& path) {
      auto p = std::filesystem::status(path).permissions();
      using std::filesystem::perms;

      auto show = [=](char op, perms perm) {
        std::cout << (perms::none == (perm & p) ? '-' : op);
      };
      std::print("Permissions for {}:", path.string());
      show('r', perms::owner_read);
      show('w', perms::owner_write);
      show('x', perms::owner_exec);
      show('r', perms::group_read);
      show('w', perms::group_write);
      show('x', perms::group_exec);
      show('r', perms::others_read);
      show('w', perms::others_write);
      show('x', perms::others_exec);
      std::cout << '\n';
    };

    display_permissions(test_file);

    // Set new permissions for the test file
    std::filesystem::perms new_perms = std::filesystem::perms::none;

    REQUIRE_NOTHROW(std::filesystem::permissions(test_file, new_perms));

    display_permissions(test_file);

    std::filesystem::remove(test_file);
  }

  // Sorting and listing files based on their last modification time can be useful for many applications,
  // such as finding the most recently modified files.
  // The following example demonstrates how to list files in a directory sorted by their last modification time.
  SECTION("Listing Files in Last Modification Time Order")
  {
    namespace fs = std::filesystem;
    std::map<fs::file_time_type, fs::path> files;
    std::vector<fs::path> toDelete;

    // create some files...
    for (int i = 0; i < 5; ++i)
    {
      toDelete.emplace_back(std::format("example{}.txt", i));
      std::ofstream(toDelete.back().c_str()) << "Hello, World!";
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    for (const auto& entry : fs::directory_iterator(fs::current_path()))
    {
      if (entry.is_regular_file())
      {
        files.emplace(fs::last_write_time(entry), entry.path());
      }
    }

    for (const auto& [time, path] : files)
    {
      //std::cout << std::format("{0:%X} on {0:%F}, {1}\n", time, path.string());
    }

    for (auto& f : toDelete)
    {
      fs::remove(f);
    }
  }
}
