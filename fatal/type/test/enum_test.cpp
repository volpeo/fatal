/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#include <fatal/type/enum.h>

#include <fatal/test/driver.h>

#include <cstring>

namespace fatal {

struct names {
  FATAL_S(state0, "state0");
  FATAL_S(state1, "state1");
  FATAL_S(state2, "state2");
  FATAL_S(state3, "state3");
  FATAL_S(field, "field");
  FATAL_S(field10, "field10");
  FATAL_S(field2, "field2");
};

FATAL_RICH_ENUM(
  test_enum,
  state0,
  (state1, 97),
  (state2, 4),
  state3
);
FATAL_S(test_enum_name, "test_enum");

enum class custom_enum { field, field10 = 37, field2 = 22 };
FATAL_S(custom_enum_name, "custom_enum");

struct custom_enum_traits {
  using type = custom_enum;

  using name = custom_enum_name;

  struct member {
    struct field {
      using name = names::field;
      using value = std::integral_constant<type, type::field>;
    };
    struct field10 {
      using name = names::field10;
      using value = std::integral_constant<type, type::field10>;
    };
    struct field2 {
      using name = names::field2;
      using value = std::integral_constant<type, type::field2>;
    };
  };

  using fields = list<member::field, member::field10, member::field2>;
};

struct custom_metadata {};

FATAL_REGISTER_ENUM_TRAITS(custom_enum_traits, custom_metadata);

/* TODO: FIX SUPPORT FOR EMPTY ENUMS
FATAL_RICH_ENUM_CLASS(empty_enum);
/*/
enum class empty_enum {};
FATAL_S(empty_enum_name, "empty_enum");

struct empty_enum_traits {
  using type = empty_enum;

  using name = empty_enum_name;

  struct member {};

  using fields = list<>;
};

FATAL_REGISTER_ENUM_TRAITS(empty_enum_traits);
//*/

FATAL_TEST(enums, has_enum_traits) {
  FATAL_EXPECT_SAME<std::true_type, has_enum_traits<test_enum>>();
  FATAL_EXPECT_SAME<std::true_type, has_enum_traits<custom_enum>>();
  FATAL_EXPECT_SAME<std::false_type, has_enum_traits<void>>();
}

FATAL_TEST(enums, types) {
  FATAL_EXPECT_SAME<test_enum, enum_traits<test_enum>::type>();
  FATAL_EXPECT_SAME<custom_enum, enum_traits<custom_enum>::type>();

  FATAL_EXPECT_SAME<test_enum_name, enum_traits<test_enum>::name>();
  FATAL_EXPECT_SAME<custom_enum_name, enum_traits<custom_enum>::name>();

  FATAL_EXPECT_SAME<void, enum_traits<test_enum>::metadata>();
  FATAL_EXPECT_SAME<custom_metadata, enum_traits<custom_enum>::metadata>();
}

FATAL_TEST(enums, declare_enum) {
  FATAL_EXPECT_EQ(test_enum::state0, static_cast<test_enum>(0));
  FATAL_EXPECT_EQ(test_enum::state1, static_cast<test_enum>(97));
  FATAL_EXPECT_EQ(test_enum::state2, static_cast<test_enum>(4));
  FATAL_EXPECT_EQ(test_enum::state3, static_cast<test_enum>(5));

  FATAL_EXPECT_EQ(custom_enum::field, static_cast<custom_enum>(0));
  FATAL_EXPECT_EQ(custom_enum::field10, static_cast<custom_enum>(37));
  FATAL_EXPECT_EQ(custom_enum::field2, static_cast<custom_enum>(22));
}

FATAL_TEST(enums, names) {
  FATAL_EXPECT_SAME<
    fatal::list<names::state0, names::state1, names::state2, names::state3>,
    transform<enum_traits<test_enum>::fields, get_type::name::apply>
  >();

  FATAL_EXPECT_SAME<
    fatal::list<names::field, names::field10, names::field2>,
    transform<enum_traits<custom_enum>::fields, get_type::name::apply>
  >();
}

FATAL_TEST(enums, values) {
  FATAL_EXPECT_SAME<
    fatal::value_list<
      test_enum,
      test_enum::state0, test_enum::state1, test_enum::state2, test_enum::state3
    >,
    transform<enum_traits<test_enum>::fields, get_type::value::apply>
  >();

  FATAL_EXPECT_SAME<
    fatal::value_list<
      custom_enum,
      custom_enum::field, custom_enum::field10, custom_enum::field2
    >,
    transform<enum_traits<custom_enum>::fields, get_type::value::apply>
  >();
}

FATAL_TEST(enums, name_of) {
  {
    using traits = enum_traits<test_enum>;
    FATAL_EXPECT_SAME<names::state0, traits::name_of<test_enum::state0>>();
    FATAL_EXPECT_SAME<names::state1, traits::name_of<test_enum::state1>>();
    FATAL_EXPECT_SAME<names::state2, traits::name_of<test_enum::state2>>();
    FATAL_EXPECT_SAME<names::state3, traits::name_of<test_enum::state3>>();
  }

  {
    using traits = enum_traits<custom_enum>;
    FATAL_EXPECT_SAME<names::field, traits::name_of<custom_enum::field>>();
    FATAL_EXPECT_SAME<names::field10, traits::name_of<custom_enum::field10>>();
    FATAL_EXPECT_SAME<names::field2, traits::name_of<custom_enum::field2>>();
  }
}

FATAL_TEST(enums, value_of) {
  {
    using traits = enum_traits<test_enum>;
    FATAL_EXPECT_SAME<
      std::integral_constant<test_enum, test_enum::state0>,
     traits::value_of<names::state0>
    >();
    FATAL_EXPECT_SAME<
      std::integral_constant<test_enum, test_enum::state1>,
     traits::value_of<names::state1>
    >();
    FATAL_EXPECT_SAME<
      std::integral_constant<test_enum, test_enum::state2>,
     traits::value_of<names::state2>
    >();
    FATAL_EXPECT_SAME<
      std::integral_constant<test_enum, test_enum::state3>,
     traits::value_of<names::state3>
    >();
  }

  {
    using traits = enum_traits<custom_enum>;
    FATAL_EXPECT_SAME<
      std::integral_constant<custom_enum, custom_enum::field>,
      traits::value_of<names::field>
    >();
    FATAL_EXPECT_SAME<
      std::integral_constant<custom_enum, custom_enum::field10>,
      traits::value_of<names::field10>
    >();
    FATAL_EXPECT_SAME<
      std::integral_constant<custom_enum, custom_enum::field2>,
      traits::value_of<names::field2>
    >();
  }
}

FATAL_TEST(enums, array::names) {
  {
    using actual = enum_traits<test_enum>::array::names;
    std::array<string_view, 4> expected{{
      string_view("state0", 6),
      string_view("state1", 6),
      string_view("state2", 6),
      string_view("state3", 6)
    }};
    FATAL_ASSERT_EQ(expected.size(), actual::size::value);
    FATAL_EXPECT_TRUE(
      std::equal(expected.begin(), expected.end(), actual::data)
    );

    std::size_t i = 0;
    for (auto const e: actual::data) {
      FATAL_ASSERT_LT(i, expected.size());
      FATAL_EXPECT_EQ(expected[i].size(), e.size());
      FATAL_EXPECT_EQ(expected[i], e);
      ++i;
    }
    FATAL_EXPECT_EQ(i, expected.size());

    static_assert(actual::data[0].size() == 6, "ensure constexpr-ness");
    static_assert(actual::data[1].size() == 6, "ensure constexpr-ness");
    static_assert(actual::data[2].size() == 6, "ensure constexpr-ness");
    static_assert(actual::data[3].size() == 6, "ensure constexpr-ness");

    static_assert(actual::data[0][0] == 's', "ensure constexpr-ness");
    static_assert(actual::data[1][0] == 's', "ensure constexpr-ness");
    static_assert(actual::data[2][0] == 's', "ensure constexpr-ness");
    static_assert(actual::data[3][0] == 's', "ensure constexpr-ness");

    static_assert(actual::data[0][5] == '0', "ensure constexpr-ness");
    static_assert(actual::data[1][5] == '1', "ensure constexpr-ness");
    static_assert(actual::data[2][5] == '2', "ensure constexpr-ness");
    static_assert(actual::data[3][5] == '3', "ensure constexpr-ness");
  }

  {
    using actual = enum_traits<custom_enum>::array::names;
    std::array<string_view, 3> expected{{
      string_view("field", 5),
      string_view("field10", 7),
      string_view("field2", 6)
    }};
    FATAL_ASSERT_EQ(expected.size(), actual::size::value);
    FATAL_EXPECT_TRUE(
      std::equal(expected.begin(), expected.end(), actual::data)
    );

    std::size_t i = 0;
    for (auto const e: actual::data) {
      FATAL_ASSERT_LT(i, expected.size());
      FATAL_EXPECT_EQ(expected[i].size(), e.size());
      FATAL_EXPECT_EQ(expected[i], e);
      ++i;
    }
    FATAL_EXPECT_EQ(i, expected.size());

    static_assert(actual::data[0].size() == 5, "ensure constexpr-ness");
    static_assert(actual::data[1].size() == 7, "ensure constexpr-ness");
    static_assert(actual::data[2].size() == 6, "ensure constexpr-ness");

    static_assert(actual::data[0][0] == 'f', "ensure constexpr-ness");
    static_assert(actual::data[1][0] == 'f', "ensure constexpr-ness");
    static_assert(actual::data[2][0] == 'f', "ensure constexpr-ness");

    static_assert(actual::data[0][4] == 'd', "ensure constexpr-ness");
    static_assert(actual::data[1][6] == '0', "ensure constexpr-ness");
    static_assert(actual::data[2][5] == '2', "ensure constexpr-ness");
  }
}

FATAL_TEST(enums, array::values) {
  {
    using actual = enum_traits<test_enum>::array::values;
    std::array<test_enum, 4> expected{{
      test_enum::state0, test_enum::state1, test_enum::state2, test_enum::state3
    }};
    FATAL_ASSERT_EQ(expected.size(), actual::size::value);
    FATAL_EXPECT_TRUE(
      std::equal(expected.begin(), expected.end(), actual::data)
    );

    std::size_t i = 0;
    for (auto const e: actual::data) {
      FATAL_ASSERT_LT(i, expected.size());
      FATAL_EXPECT_EQ(expected[i], e);
      ++i;
    }
    FATAL_EXPECT_EQ(i, expected.size());
  }

  {
    using actual = enum_traits<custom_enum>::array::values;
    std::array<custom_enum, 3> expected{{
      custom_enum::field, custom_enum::field10, custom_enum::field2
    }};
    FATAL_ASSERT_EQ(expected.size(), actual::size::value);
    FATAL_EXPECT_TRUE(
      std::equal(expected.begin(), expected.end(), actual::data)
    );

    std::size_t i = 0;
    for (auto const e: actual::data) {
      FATAL_ASSERT_LT(i, expected.size());
      FATAL_EXPECT_EQ(expected[i], e);
      ++i;
    }
    FATAL_EXPECT_EQ(i, expected.size());
  }
}

FATAL_TEST(enums, is_valid) {
  {
    using traits = enum_traits<empty_enum>;

    FATAL_EXPECT_FALSE(traits::is_valid(static_cast<empty_enum>(-1)));
  }

  {
    using traits = enum_traits<test_enum>;

    FATAL_EXPECT_FALSE(traits::is_valid(static_cast<test_enum>(-1)));
    FATAL_EXPECT_TRUE(traits::is_valid(test_enum::state0));
    FATAL_EXPECT_TRUE(traits::is_valid(test_enum::state1));
    FATAL_EXPECT_TRUE(traits::is_valid(test_enum::state2));
    FATAL_EXPECT_TRUE(traits::is_valid(test_enum::state3));
  }

  {
    using traits = enum_traits<custom_enum>;

    FATAL_EXPECT_FALSE(traits::is_valid(static_cast<custom_enum>(-1)));
    FATAL_EXPECT_TRUE(traits::is_valid(custom_enum::field));
    FATAL_EXPECT_TRUE(traits::is_valid(custom_enum::field10));
    FATAL_EXPECT_TRUE(traits::is_valid(custom_enum::field2));
  }
}

FATAL_TEST(enums, is_valid_enum) {
  FATAL_EXPECT_FALSE(is_valid_enum(static_cast<empty_enum>(-1)));

  FATAL_EXPECT_FALSE(is_valid_enum(static_cast<test_enum>(-1)));
  FATAL_EXPECT_TRUE(is_valid_enum(test_enum::state0));
  FATAL_EXPECT_TRUE(is_valid_enum(test_enum::state1));
  FATAL_EXPECT_TRUE(is_valid_enum(test_enum::state2));
  FATAL_EXPECT_TRUE(is_valid_enum(test_enum::state3));

  FATAL_EXPECT_FALSE(is_valid_enum(static_cast<custom_enum>(-1)));
  FATAL_EXPECT_TRUE(is_valid_enum(custom_enum::field));
  FATAL_EXPECT_TRUE(is_valid_enum(custom_enum::field10));
  FATAL_EXPECT_TRUE(is_valid_enum(custom_enum::field2));
}

FATAL_TEST(enums, to_string) {
  {
    using traits = enum_traits<test_enum>;

    FATAL_EXPECT_EQ(nullptr, traits::to_string(static_cast<test_enum>(-1)));
    FATAL_EXPECT_EQ("state0", traits::to_string(test_enum::state0));
    FATAL_EXPECT_EQ("state1", traits::to_string(test_enum::state1));
    FATAL_EXPECT_EQ("state2", traits::to_string(test_enum::state2));
    FATAL_EXPECT_EQ("state3", traits::to_string(test_enum::state3));

    FATAL_EXPECT_EQ("", traits::to_string(static_cast<test_enum>(-1), ""));
    FATAL_EXPECT_EQ("state0", traits::to_string(test_enum::state0, ""));
    FATAL_EXPECT_EQ("state1", traits::to_string(test_enum::state1, ""));
    FATAL_EXPECT_EQ("state2", traits::to_string(test_enum::state2, ""));
    FATAL_EXPECT_EQ("state3", traits::to_string(test_enum::state3, ""));
  }

  {
    using traits = enum_traits<custom_enum>;

    FATAL_EXPECT_EQ(nullptr, traits::to_string(static_cast<custom_enum>(-1)));
    FATAL_EXPECT_EQ("field", traits::to_string(custom_enum::field));
    FATAL_EXPECT_EQ("field10", traits::to_string(custom_enum::field10));
    FATAL_EXPECT_EQ("field2", traits::to_string(custom_enum::field2));

    FATAL_EXPECT_EQ("", traits::to_string(static_cast<custom_enum>(-1), ""));
    FATAL_EXPECT_EQ("field", traits::to_string(custom_enum::field, ""));
    FATAL_EXPECT_EQ("field10", traits::to_string(custom_enum::field10, ""));
    FATAL_EXPECT_EQ("field2", traits::to_string(custom_enum::field2, ""));
  }
}

FATAL_TEST(enums, enum_to_string) {
  FATAL_EXPECT_EQ(nullptr, enum_to_string(static_cast<test_enum>(-1)));
  FATAL_EXPECT_EQ("state0", enum_to_string(test_enum::state0));
  FATAL_EXPECT_EQ("state1", enum_to_string(test_enum::state1));
  FATAL_EXPECT_EQ("state2", enum_to_string(test_enum::state2));
  FATAL_EXPECT_EQ("state3", enum_to_string(test_enum::state3));

  FATAL_EXPECT_EQ("", enum_to_string(static_cast<test_enum>(-1), ""));
  FATAL_EXPECT_EQ("state0", enum_to_string(test_enum::state0, ""));
  FATAL_EXPECT_EQ("state1", enum_to_string(test_enum::state1, ""));
  FATAL_EXPECT_EQ("state2", enum_to_string(test_enum::state2, ""));
  FATAL_EXPECT_EQ("state3", enum_to_string(test_enum::state3, ""));

  FATAL_EXPECT_EQ(nullptr, enum_to_string(static_cast<custom_enum>(-1)));
  FATAL_EXPECT_EQ("field", enum_to_string(custom_enum::field));
  FATAL_EXPECT_EQ("field10", enum_to_string(custom_enum::field10));
  FATAL_EXPECT_EQ("field2", enum_to_string(custom_enum::field2));

  FATAL_EXPECT_EQ("", enum_to_string(static_cast<custom_enum>(-1), ""));
  FATAL_EXPECT_EQ("field", enum_to_string(custom_enum::field, ""));
  FATAL_EXPECT_EQ("field10", enum_to_string(custom_enum::field10, ""));
  FATAL_EXPECT_EQ("field2", enum_to_string(custom_enum::field2, ""));
}

FATAL_TEST(enums, try_parse) {
# define CREATE_TEST(e, x) \
  do { \
    std::string const s(FATAL_TO_STR(x)); \
    e out = static_cast<e>(-1); \
    FATAL_EXPECT_TRUE(enum_traits<e>::try_parse(out, s)); \
    FATAL_EXPECT_EQ(e::x, out); \
    \
    out = static_cast<e>(-1); \
    FATAL_EXPECT_TRUE(enum_traits<e>::try_parse(out, s.begin(), s.end())); \
    FATAL_EXPECT_EQ(e::x, out); \
    \
    out = static_cast<e>(-1); \
    FATAL_EXPECT_FALSE(enum_traits<e>::try_parse(out, s.begin(), s.begin())); \
    FATAL_EXPECT_EQ(static_cast<e>(-1), out); \
    \
    FATAL_EXPECT_FALSE( \
      enum_traits<e>::try_parse(out, std::next(s.begin()), s.end()) \
    ); \
    FATAL_EXPECT_EQ(static_cast<e>(-1), out); \
    \
    auto const i = s + "invalid"; \
    FATAL_EXPECT_FALSE((enum_traits<e>::try_parse(out, i))); \
    FATAL_EXPECT_FALSE((enum_traits<e>::try_parse(out, i.begin(), i.end()))); \
  } while (false)

  CREATE_TEST(test_enum, state0);
  CREATE_TEST(test_enum, state1);
  CREATE_TEST(test_enum, state2);
  CREATE_TEST(test_enum, state3);

  CREATE_TEST(custom_enum, field);
  CREATE_TEST(custom_enum, field10);
  CREATE_TEST(custom_enum, field2);

# undef CREATE_TEST

  {
    using traits = enum_traits<test_enum>;
    traits::type out;

    FATAL_EXPECT_FALSE(traits::try_parse(out, std::string()));
    FATAL_EXPECT_FALSE(traits::try_parse(out, std::string("invalid")));
  }

  {
    using traits = enum_traits<custom_enum>;
    traits::type out;

    FATAL_EXPECT_FALSE(traits::try_parse(out, std::string()));
    FATAL_EXPECT_FALSE(traits::try_parse(out, std::string("invalid")));
  }
}

FATAL_TEST(enums, parse) {
# define CREATE_TEST(e, x) \
  do { \
    std::string const s(FATAL_TO_STR(x)); \
    FATAL_EXPECT_EQ(e::x, enum_traits<e>::parse(s)); \
    FATAL_EXPECT_EQ(e::x, enum_traits<e>::parse(s.begin(), s.end())); \
    FATAL_EXPECT_THROW(std::invalid_argument) { \
      enum_traits<e>::parse(s.begin(), s.begin()); \
    }; \
  } while (false)

  CREATE_TEST(test_enum, state0);
  CREATE_TEST(test_enum, state1);
  CREATE_TEST(test_enum, state2);
  CREATE_TEST(test_enum, state3);

  CREATE_TEST(custom_enum, field);
  CREATE_TEST(custom_enum, field10);
  CREATE_TEST(custom_enum, field2);

# undef CREATE_TEST

# define CREATE_TEST(e, x) \
  do { \
    std::string const s(FATAL_TO_STR(x)); \
    FATAL_EXPECT_THROW(std::invalid_argument) { \
      enum_traits<e>::parse(std::next(s.begin()), s.end()); \
    }; \
  } while (false)

  CREATE_TEST(test_enum, state0);
  CREATE_TEST(test_enum, state1);
  CREATE_TEST(test_enum, state2);
  CREATE_TEST(test_enum, state3);

  CREATE_TEST(custom_enum, field);
  CREATE_TEST(custom_enum, field10);
  CREATE_TEST(custom_enum, field2);

# undef CREATE_TEST

# define CREATE_TEST(e, x) \
  do { \
    std::string const s(FATAL_TO_STR(x) "invalid"); \
    FATAL_EXPECT_THROW(std::invalid_argument) { \
      enum_traits<e>::parse(s); \
    }; \
  } while (false)

  CREATE_TEST(test_enum, state0);
  CREATE_TEST(test_enum, state1);
  CREATE_TEST(test_enum, state2);
  CREATE_TEST(test_enum, state3);

  CREATE_TEST(custom_enum, field);
  CREATE_TEST(custom_enum, field10);
  CREATE_TEST(custom_enum, field2);

# undef CREATE_TEST

# define CREATE_TEST(e, x) \
  do { \
    std::string const s(FATAL_TO_STR(x) "invalid"); \
    FATAL_EXPECT_THROW(std::invalid_argument) { \
      enum_traits<e>::parse(s.begin(), s.end()); \
    }; \
  } while (false)

  CREATE_TEST(test_enum, state0);
  CREATE_TEST(test_enum, state1);
  CREATE_TEST(test_enum, state2);
  CREATE_TEST(test_enum, state3);

  CREATE_TEST(custom_enum, field);
  CREATE_TEST(custom_enum, field10);
  CREATE_TEST(custom_enum, field2);

# undef CREATE_TEST

  {
    using traits = enum_traits<test_enum>;

    FATAL_EXPECT_THROW(std::invalid_argument) {
      traits::parse(std::string());
    };
    FATAL_EXPECT_THROW(std::invalid_argument) {
      traits::parse(std::string("invalid"));
    };
  }

  {
    using traits = enum_traits<custom_enum>;

    FATAL_EXPECT_THROW(std::invalid_argument) {
      traits::parse(std::string());
    };
    FATAL_EXPECT_THROW(std::invalid_argument) {
      traits::parse(std::string("invalid"));
    };
  }
}

} // namespace fatal {
