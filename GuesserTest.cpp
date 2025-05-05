/**
 * Unit Tests for the class
**/

#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test
{
	protected:
		GuesserTest(){} //constructor runs before each test
		virtual ~GuesserTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor)
};

// Example "smoke test" (can be deleted)
TEST(GuesserTest, smoke_test)
{
  Guesser object("Secret");
  ASSERT_EQ( 1+1, 2 );
}

TEST(GuesserTest, MatchExactSecret) {
	Guesser g("hello");
	ASSERT_TRUE(g.match("hello"));
	ASSERT_EQ(g.remaining(), 3);
}

TEST(GuesserTest, CloseGuessReducesRemaining) {
	Guesser g("code");
	ASSERT_FALSE(g.match("coda"));
	ASSERT_EQ(g.remaining(), 2);
}

TEST(GuesserTest, BruteForceLocksSecret) {
	Guesser g("code");
	ASSERT_FALSE(g.match("abcd"));
	ASSERT_FALSE(g.match("code"));
	ASSERT_EQ(g.remaining(), 2);
}

TEST(GuesserTest, FarGuessLocksSecret) {
	Guesser g("hello");
	ASSERT_FALSE(g.match("world"));
	ASSERT_FALSE(g.match("hello"));
	ASSERT_EQ(g.remaining(), 3);
}

TEST(GuesserTest, LockAfterThreeWrongCloseGuesses) {
	Guesser g("guess");
	g.match("guese");
	g.match("guese");
	g.match("guese");
	ASSERT_FALSE(g.match("guess")); 
	ASSERT_EQ(g.remaining(), 0);
}

TEST(GuesserTest, SecretTruncatesAt32Characters) {
	std::string long_secret(40, 'a');
	Guesser g(long_secret);
	std::string guess_32a = std::string(32, 'a');
	ASSERT_TRUE(g.match(guess_32a));
}

TEST(GuesserTest, ResetAfterCorrectGuess) {
	Guesser g("match");
	g.match("matcn");
	g.match("mxtch");
	ASSERT_TRUE(g.match("match"));
	ASSERT_EQ(g.remaining(), 3); 
}

TEST(GuesserTest, RepeatedBruteForceAttacksLockPermanently) {
	Guesser g("secret");

	ASSERT_FALSE(g.match("zzzzz"));
	ASSERT_FALSE(g.match("12345"));
	ASSERT_FALSE(g.match("!@#$%"));
	ASSERT_FALSE(g.match("secret"));
	ASSERT_EQ(g.remaining(), 3);
}

TEST(GuesserTest, OverflowLengthGuess) {
	Guesser g("short");
	std::string longGuess(1000, 'x');
	ASSERT_FALSE(g.match(longGuess));
	ASSERT_FALSE(g.match("short"));
}

TEST(GuesserTest, JustOverDistanceLimitLocks) {
	Guesser g("match");
	ASSERT_FALSE(g.match("mitch"));
	ASSERT_EQ(g.remaining(), 2);
	ASSERT_FALSE(g.match("botch"));
	ASSERT_FALSE(g.match("match"));
}

TEST(GuesserTest, NoResetWithoutTrueMatch) {
	Guesser g("guess");
	g.match("guest");
	ASSERT_EQ(g.remaining(), 2);
	g.match("guest");
	ASSERT_EQ(g.remaining(), 1);
	g.match("guest");
	ASSERT_FALSE(g.match("guess"));
	ASSERT_EQ(g.remaining(), 0);
}


TEST(GuesserTest, SecretTruncatedButGuessFullLength) {
	std::string longSecret(40, 'a');
	Guesser g(longSecret);
	std::string guessFull = std::string(40, 'a');
	ASSERT_FALSE(g.match(guessFull));
	ASSERT_FALSE(g.match(std::string(32, 'a')));
}