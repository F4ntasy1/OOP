#include <iostream>
#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"

#include "../vector/vector.h"

SCENARIO("Read vector from input")
{
	VectorFloat vectorFloat;

	WHEN("input stream is empty")
	{
		std::istringstream input("");
		THEN("vectorFloat is empty")
		{
			ReadVectorFromInput(input, vectorFloat);
			CHECK(vectorFloat.empty());
			CHECK(input.eof());
		}
	}

	WHEN("input stream contains one line with three numbers")
	{
		std::istringstream input("4.2 7 -1.8");
		std::vector<double> outputVector = { 4.2, 7, -1.8 };
		THEN("vectorFloat is empty")
		{
			ReadVectorFromInput(input, vectorFloat);
			CHECK(vectorFloat == outputVector);
			CHECK(input.eof());
		}
	}

	WHEN("input stream contains several lines with numbers")
	{
		std::istringstream input("4.2 7 0 \n -2 23.07");
		std::vector<double> outputVector = { 4.2, 7, 0, -2, 23.07 };
		THEN("vectorFloat is empty")
		{
			ReadVectorFromInput(input, vectorFloat);
			CHECK(vectorFloat == outputVector);
			CHECK(input.eof());
		}
	}
}

SCENARIO("Get vector average")
{
	VectorFloat vectorFloat;

	WHEN("input vector is empty")
	{
		THEN("the value of the average is zero")
		{
			CHECK(GetVectorAverage(vectorFloat) == 0);
		}
	}

	WHEN("input vector contains positive and negative values")
	{
		vectorFloat = {1.2, -2, 3.4, -44.8};
		THEN("the value of the average is 2.3")
		{
			CHECK(GetVectorAverage(vectorFloat) == 2.3);
		}

		vectorFloat = { 1, -2, 1, -44.8 };
		THEN("the value of the average is zero")
		{
			CHECK(GetVectorAverage(vectorFloat) == 1);
		}
	}

	WHEN("input vector contains only negative values")
	{
		vectorFloat = { -4.33, -12, -40.8 };
		THEN("the value of the average is zero")
		{
			CHECK(GetVectorAverage(vectorFloat) == 0);
		}
	}
}

SCENARIO("Add average to vector elements")
{
	VectorFloat vectorFloat = { 1,2,3 };

	WHEN("average is zero")
	{
		const double average = 0;
		THEN("output vector will not change")
		{
			VectorFloat startVector = vectorFloat;
			AddAverageToVectorElements(vectorFloat, average);
			CHECK(vectorFloat == startVector);
		}
	}

	WHEN("average is negative")
	{
		const double average = -1;
		THEN("output vector will not change")
		{
			VectorFloat startVector = vectorFloat;
			AddAverageToVectorElements(vectorFloat, average);
			CHECK(vectorFloat == startVector);
		}
	}

	WHEN("average is positive")
	{
		const double average = 1.5;
		THEN("the average will be added to the elements of the vector")
		{
			VectorFloat resultVector = { 2.5, 3.5, 4.5 };
			AddAverageToVectorElements(vectorFloat, average);
			CHECK(vectorFloat == resultVector);
		}
	}
}

SCENARIO("Write vector in output")
{
	std::ostringstream output;

	WHEN("input vector is empty")
	{
		VectorFloat vectorFloat;
		THEN("output string is empty")
		{
			WriteVectorInOutput(output, vectorFloat);
			CHECK(output.str() == "");
		}
	}

	WHEN("input vector is sorted")
	{
		VectorFloat vectorFloat = { -1.2, 3, 7.8 };
		THEN("output string is empty")
		{
			WriteVectorInOutput(output, vectorFloat);
			CHECK(output.str() == "-1.2 3 7.8 \n");
		}
	}

	WHEN("input vector is not sorted")
	{
		VectorFloat vectorFloat = { 5, -3, 7.8, 1.2 };
		THEN("output string is empty")
		{
			WriteVectorInOutput(output, vectorFloat);
			CHECK(output.str() == "-3 1.2 5 7.8 \n");
		}
	}
}