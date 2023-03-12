#include <iostream>
#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"

#include "../htmlDecode/htmlDecode.h"
#include "../htmlDecode/consts.h"

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
			REQUIRE(HtmlDecode(str) == "");
		}
	}

	WHEN("Input string contains html entities")
	{
		THEN("Output string contains decode html entities")
		{
			REQUIRE(HtmlDecode("M&amp;M&apos;s") == "M&M's");
			REQUIRE(HtmlDecode("M&amp;M&apos;&apos;s") == "M&M''s");
			REQUIRE(HtmlDecode("M&gt;&&&lt;M") == "M>&&<M");
		}
	}

	WHEN("Input string does not contains html entities")
	{
		THEN("Output string does not contains decode html entities")
		{
			REQUIRE(HtmlDecode("M&ampM's") == "M&ampM's");
			REQUIRE(HtmlDecode("M&aposs;") == "M&aposs;");
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