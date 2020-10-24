/****************************************************************
* Name: Jordan Wriker & Ahmed Rizvi
* Date: 25th October, 2020
* Course: OOP-3200
* File: OOP3200-F2020-Lab5
* Purpose: The purpose of lab is to demonstrate the use of the STL.
*		   In this case, we use the <map> template library to make
*		   use of "mapping" objects with an identifier/label. The
*		   label is then used to access the objects in practical
*		   use case scenarios.
*
******************************************************************/

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string.h>
#include "Vector2D.h"
#include <map>
#include <type_traits>

// Method to pause the program until the user decides to move forward.
void Pause()
{
	std::cout << "\nPress \'ENTER\' to continue..." << std::endl;
	std::cin.ignore();	fflush(stdin);
}

int main()
{
	try
	{
		/************************
		 *	DECLARATIONS
		 ************************/

		std::map<std::string, Vector2D<int>*> pointsMap;
		std::string input{};
		float overallDistance{}; // Holds sum of distance between each point.

		/******************************************************************************
		 *	Reading Labels/Points into the Map:
		 *	Attempt to open the data file.  If the data file opens, read a label,
		 *	followed by a point and insert them into the map using the label as the key.
		 *	Repeat until all the data is read and close the file. If the data file did
		 *	not open, tell the user and remind them to check that the file exists. If
		 *	the file opened but the map is empty after the input loop, report that to
		 *	the user and remind them to check that the file contains valid data in
		 *	the correct format. Only continue processing if the file was opened and the
		 *	map is not empty.
		 ******************************************************************************/

		std::ifstream infile;
		std::string fileName = "PointData.dat";
		std::string emptyCheck{};

		infile.open(fileName.c_str());

		// Check if file is open and empty
		if (infile.peek() == std::ifstream::traits_type::eof() && infile.is_open())
		{
			// Throw an empty file error.
			throw std::runtime_error("The map is empty. Check that the file contains valid data in the correct format.\n\n");
		}
		
		if (infile.is_open()) // The file successfully opened for reading. 
		{
			std::string key;
			int x, y;

			while (!infile.fail()) // Keep reading until failing to read occurs.
			{
				infile >> key;
				infile.ignore(1, ' ');
				infile.ignore(1, '(');
				infile >> x;
				infile.ignore(1, ',');
				infile.ignore(1, ' ');
				infile >> y;
				infile.ignore(1, ')');

				auto* temp_object = new Vector2D<int>(x, y);

				pointsMap[key] = temp_object;
			}
			infile.close();
		}
		else // The file name was not found within the project, throw an error.
		{		
				throw std::runtime_error("The file could not be found, check if it exists.");
		}
		

		/******************************************************************************
		 *	Determine the Total Distance Between All Points in Order:
		 *	Use an iterator and a loop to traverse each label/point in the map. For
		 *	each label/point, determine the distance from that point to the previous
		 *	point (or next point depending on how you implement this) and add that
		 *	distance to a total.  Note that the Vector2D class includes a static
		 *	distance function to determine the distance between two Vector2D
		 *	objects, so you should not need to use any complicated math here.  Report
		 *	to the user how many points the map contains and what the total distance is.
		 ******************************************************************************/

		std::map<std::string, Vector2D<int>*>::iterator iter = pointsMap.begin(); // Iterator begins with the first point on the map.

		while (iter != --pointsMap.end()) // Loop until the there are no more points on the map.
		{
			float distance;

			Vector2D<int> point1 = *iter->second;

			Vector2D<int> point2 = *std::next(iter, 1)->second;

			distance = Vector2D<int>::Distance(point1, point2); // Calculate distance between the current two points.

			overallDistance += distance; // Add the calculated distance to overall sum.

			iter++; // increase the iterator to move onto the next set of two points.
		}
		// Display the overall distance summed to the user.
		std::cout << "The map contains " << pointsMap.size() << " points for a total distance of " << overallDistance << ".\n" << std::endl;

		/******************************************************************************
		 *	Determine the Distance Between the Start Point and a User Selected Point:
		 *	Prompt the user to enter a label or to enter "quit" to end.  If the user
		 *	entered anything other than "quit", attempt to find the label they entered
		 *	in the map. If it was found, report the distance between the point for the
		 *	label they entered and the start point (i.e. the point labeled "AA").
		 *	Otherwise, tell the user that the label they entered is not in the map.
		 *	Repeat these steps until the user enters "quit".
		 ******************************************************************************/

		while (input != "quit") // Keep the loop going until the user decides to stop. 
		{
			std::cout << "Enter the label of the point you wish to go to (\"quit\" to end): ";
			std::getline(std::cin, input); // Get the label to search for.
			std::cout << std::endl;

			iter = pointsMap.find(input);

			if (iter != pointsMap.end()) // Has found or is still looking for element with matching key.
			{
				float distance = Vector2D<int>::Distance(*pointsMap.begin()->second, *pointsMap[input]);

				std::cout << "The distance between " << pointsMap.begin()->first << " " << pointsMap.begin()->second->ToString()
					<< " and " << input << pointsMap[input]->ToString() << " is " << distance << std::endl << std::endl;
			}
			else if (input != "quit") // Could not find element with matching key.
			{
				std::cout << "There is no point labeled \"" << input << "\" in the map.\n" << std::endl;
			}
		}
	}

	/******************************************************************************
	 *	Exception Handling:
	 *	Catch any std::exception thrown. Report to the user that a run-time error
	 *	occurred and show what exception was thrown.
	 ******************************************************************************/

	 catch(std::exception e)  // an exception was thrown. Message is displayed with runtime_error()
	 {
	 	std::cerr << "An exception was thrown: " << e.what() <<  std::endl << std::endl;
	 	std::cout << "---------------------------------" << std::endl;
	 	Pause();
	 }
	 catch (...) // An unexpected exception was thrown. Display error number here.
	 {
		 std::cerr << "Am error occured at run-time: " << strerror(errno) << std::endl << std::endl;
		 std::cout << "---------------------------------" << std::endl;;
		 Pause();
	 }

	// END-OF-PROGRAM

	return 0;
}
