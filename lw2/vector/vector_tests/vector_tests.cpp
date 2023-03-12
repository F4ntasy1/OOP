#include <iostream>
#define CATCH_CONFIG_MAIN
#include "../../../catch2/catch.hpp"

#include "../vector/vector.h"

SCENARIO("Read vector from input")
{
	VectorDouble vectorDouble;

	WHEN("input stream is empty")
	{
		std::istringstream input("");
		THEN("vectorFloat is empty")
		{
			ReadVectorFromInput(input, vectorDouble);
			CHECK(vectorDouble.empty());
			CHECK(input.eof());
		}
	}

	WHEN("input stream contains one line with three numbers")
	{
		std::istringstream input("4.2 7 -1.8");
		std::vector<double> outputVector = { 4.2, 7, -1.8 };
		THEN("vectorFloat is empty")
		{
			ReadVectorFromInput(input, vectorDouble);
			CHECK(vectorDouble == outputVector);
			CHECK(input.eof());
		}
	}

	WHEN("input stream contains several lines with numbers")
	{
		std::istringstream input("4.2 7 0 \n -2 23.07");
		std::vector<double> outputVector = { 4.2, 7, 0, -2, 23.07 };
		THEN("vectorFloat is empty")
		{
			ReadVectorFromInput(input, vectorDouble);
			CHECK(vectorDouble == outputVector);
			CHECK(input.eof());
		}
	}
}

SCENARIO("Get vector average")
{
	VectorDouble vectorDouble;

	WHEN("input vector is empty")
	{
		THEN("the value of the average is zero")
		{
			CHECK(GetVectorAverage(vectorDouble) == 0);
		}
	}

	WHEN("input vector contains positive and negative values")
	{
		vectorDouble = {1.2, -2, 3.4, -44.8};
		THEN("the value of the average is 2.3")
		{
			CHECK(GetVectorAverage(vectorDouble) == 2.3);
		}

		vectorDouble = { 1, -2, 1, -44.8 };
		THEN("the value of the average is zero")
		{
			CHECK(GetVectorAverage(vectorDouble) == 1);
		}
	}

	WHEN("input vector contains only negative values")
	{
		vectorDouble = { -4.33, -12, -40.8 };
		THEN("the value of the average is zero")
		{
			CHECK(GetVectorAverage(vectorDouble) == 0);
		}
	}
}

SCENARIO("Add average to vector elements")
{
	VectorDouble vectorDouble = { 1,2,3 };

	WHEN("average is zero")
	{
		THEN("output vector will not change")
		{
			VectorDouble startVector = vectorDouble;
			AddAverageToVectorElements(vectorDouble, 0);
			CHECK(vectorDouble == startVector);
		}
	}

	WHEN("average is negative")
	{
		THEN("output vector will not change")
		{
			VectorDouble startVector = vectorDouble;
			AddAverageToVectorElements(vectorDouble, -1);
			CHECK(vectorDouble == startVector);
		}
	}

	WHEN("average is positive")
	{
		THEN("the average will be added to the elements of the vector")
		{
			VectorDouble resultVector = { 2.5, 3.5, 4.5 };
			AddAverageToVectorElements(vectorDouble, 1.5);
			CHECK(vectorDouble == resultVector);
		}
	}
}

SCENARIO("Write vector in output")
{
	std::ostringstream output;

	WHEN("input vector is empty")
	{
		VectorDouble vectorDouble;
		THEN("output string is empty")
		{
			WriteVectorToOutput(output, vectorDouble);
			CHECK(output.str() == "");
		}
	}

	WHEN("input vector is sorted")
	{
		VectorDouble vectorDouble = { -1.2, 3, 7.8 };
		THEN("output string is empty")
		{
			WriteVectorToOutput(output, vectorDouble);
			CHECK(output.str() == "-1.2 3 7.8 \n");
		}
	}

	WHEN("input vector is not sorted")
	{
		VectorDouble vectorDouble = { 5, -3, 7.8, 1.2 };
		THEN("output string is empty")
		{
			WriteVectorToOutput(output, vectorDouble);
			CHECK(output.str() == "-3 1.2 5 7.8 \n");
		}
	}
}