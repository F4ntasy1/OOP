#include <iostream>
#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"

#include "../htmlDecode/htmlDecode.h"

SCENARIO("Get decode html entity")
{
	WHEN("Html entity is exist")
	{
		THEN("Decode html symbol is exist")
		{
			REQUIRE(GetDecodeHtmlSymbol(ENCODE_DOUBLE_QUOTE) == DECODE_DOUBLE_QUOTE);
			REQUIRE(GetDecodeHtmlSymbol(ENCODE_APOSTROPHE) == DECODE_APOSTROPHE);
			REQUIRE(GetDecodeHtmlSymbol(ENCODE_SIGN_LESS) == DECODE_SIGN_LESS);
			REQUIRE(GetDecodeHtmlSymbol(ENCODE_SIGN_MORE) == DECODE_SIGN_MORE);
			REQUIRE(GetDecodeHtmlSymbol(ENCODE_AMPERSAND) == DECODE_AMPERSAND);
		}
	}

	WHEN("Html entity does not exist")
	{
		THEN("Will throw an error")
		{
			REQUIRE_THROWS(GetDecodeHtmlSymbol(""));
		}
	}
}

SCENARIO("Html decode")
{
	std::string str;

	WHEN("Input string is empty")
	{
		THEN("Output string is empty")
		{
			str = std::string();
			REQUIRE(HtmlDecode(str) == "");
		}
	}

	WHEN("Input string contains html entities")
	{
		THEN("Output string contains decode html entities")
		{
			str = "M&amp;M&apos;s";
			REQUIRE(HtmlDecode(str) == "M&M's");
			str = "M&amp;M&apos;&apos;s";
			REQUIRE(HtmlDecode(str) == "M&M''s");
			str = "M&gt;&&&lt;M";
			REQUIRE(HtmlDecode(str) == "M>&&<M");
		}
	}

	WHEN("Input string does not contains html entities")
	{
		THEN("Output string does not contains decode html entities")
		{
			str = "M&ampM's";
			REQUIRE(HtmlDecode(str) == str);
			str = "M&aposs;";
			REQUIRE(HtmlDecode(str) == str);
		}
	}
}

SCENARIO("Decode input to output")
{
	std::ostringstream output;

	WHEN("Input stream is empty")
	{
		std::istringstream input("");

		THEN("Output stream is empty")
		{
			DecodeInputToOutput(input, output);
			REQUIRE(output.str() == "");
		}
	}

	WHEN("Input stream does not contains html entities")
	{
		std::istringstream input("Hello world!\nI am here!\n");

		THEN("The output stream is the same as the input stream")
		{
			DecodeInputToOutput(input, output);
			REQUIRE(output.str() == input.str());
		}
	}

	WHEN("Input stream contains html entities")
	{
		std::istringstream input("Hello world!\nM&amp;M&apos;s is here!\n");
		const std::string inputDecode = "Hello world!\nM&M's is here!\n";

		THEN("The output stream is the same as the input stream")
		{
			DecodeInputToOutput(input, output);
			REQUIRE(output.str() == inputDecode);
		}
	}
}