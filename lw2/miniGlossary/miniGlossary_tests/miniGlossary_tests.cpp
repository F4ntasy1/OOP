#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"

#include "../miniGlossary/miniGlossary.h"

SCENARIO("Open file for reading")
{
	std::ifstream input;

	WHEN("Input file name is valid")
	{
		THEN("The file will open for reading")
		{
			REQUIRE_NOTHROW(WorkWithFile::OpenFileForReading(input, "test"));
			REQUIRE(input.is_open());
		}
	}

	WHEN("Input file name does not valid")
	{
		THEN("The file will not open for reading")
		{
			REQUIRE_THROWS(WorkWithFile::OpenFileForReading(input, ""));
			REQUIRE(!input.is_open());
		}
	}
}

SCENARIO("Open file for writing")
{
	std::ofstream output;

	WHEN("Output file name is valid")
	{
		THEN("The file will open for writing")
		{
			REQUIRE_NOTHROW(WorkWithFile::OpenFileForWriting(output, "test"));
			REQUIRE(output.is_open());
		}
	}

	WHEN("Output file name does not valid")
	{
		THEN("The file will not open for writing")
		{
			REQUIRE_THROWS(WorkWithFile::OpenFileForWriting(output, ""));
			REQUIRE(!output.is_open());
		}
	}
}

SCENARIO("Check string for Russian characters")
{
	WHEN("String is empty")
	{
		THEN("Result is false")
		{
			REQUIRE(!Util::CheckStringForRussianCharacters(""));
		}
	}

	WHEN("String contains Russian characters")
	{
		THEN("Result is true")
		{
			REQUIRE(Util::CheckStringForRussianCharacters("Привет!"));
		}
	}

	WHEN("String does not contains Russian characters")
	{
		THEN("Result is false")
		{
			REQUIRE(!Util::CheckStringForRussianCharacters("Hello!"));
		}
	}
}

SCENARIO("Check string for English characters")
{
	WHEN("String is empty")
	{
		THEN("Result is false")
		{
			REQUIRE(!Util::CheckStringForEnglishCharacters(""));
		}
	}

	WHEN("String contains English characters")
	{
		THEN("Result is true")
		{
			REQUIRE(Util::CheckStringForEnglishCharacters("Hello!"));
		}
	}

	WHEN("String does not contains English characters")
	{
		THEN("Result is false")
		{
			REQUIRE(!Util::CheckStringForEnglishCharacters("Привет!"));
		}
	}
}

SCENARIO("Copy glossary from input to vector")
{
	Glossary glossary;

	WHEN("Input stream is empty")
	{
		THEN("Glossary is empty")
		{
			std::istringstream input("");
			REQUIRE_NOTHROW(
				WorkWithGlossary::CopyGlossaryFromInputToVector(input, glossary));
			REQUIRE(glossary.size() == 0);
		}
	}

	WHEN("Input stream has error format")
	{
		THEN("Throw error about glossary format")
		{
			std::istringstream input("line 1");
			REQUIRE_THROWS(
				WorkWithGlossary::CopyGlossaryFromInputToVector(input, glossary));
		}
	}

	WHEN("Input stream has a valid format")
	{
		THEN("Glossary copyed success")
		{
			std::istringstream input("line 1\nлиния 1");
			REQUIRE_NOTHROW(
				WorkWithGlossary::CopyGlossaryFromInputToVector(input, glossary));
			REQUIRE(glossary["line 1"] == "линия 1");
		}
	}
}