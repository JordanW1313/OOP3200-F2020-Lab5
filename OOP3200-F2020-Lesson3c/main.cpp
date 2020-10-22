#include <iomanip>
#include <iostream>
#include <fstream>
#include <string.h>
#include "Vector2D.h"
#include <map>

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
		float overallDistance{};

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
		infile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		infile.open(fileName.c_str());

		if (infile.is_open())
		{
			std::string key;
			int x, y;

			while (!infile.fail())
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

		std::map<std::string, Vector2D<int>*>::iterator iter = pointsMap.begin();

		while (iter != --pointsMap.end())
		{
			float distance;
			Vector2D<int> point1 = *iter->second;

			Vector2D<int> point2 = *std::next(iter, 1)->second;

			distance = Vector2D<int>::Distance(point1, point2);

			overallDistance += distance;

			iter++;
		}
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

		while (input != "quit")
		{
			std::cout << "Enter the label of the point you wish to go to (\"quit\" to end): ";
			std::getline(std::cin, input);
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
				std::cout << "There is no point labelled \"" << input << "\" in the map.\n" << std::endl;
			}
		}
	}

	/******************************************************************************
	 *	Exception Handling:
	 *	Catch any std::exception thrown. Report to the user that a run-time error
	 *	occurred and show what exception was thrown.
	 ******************************************************************************/

	 catch(const std::ifstream::failure& e)  // an exception was thrown
	 {
	 	std::cerr << "PointsData.dat could not be opened for input. Check that the file exists." << std::endl << std::endl;
	 	std::cout << "---------------------------------" << std::endl;
	 	Pause();

	 }
	catch (...)
	{
		std::cerr << "An error occured at run-time: map::at" << std::endl << std::endl;
		std::cout << "---------------------------------" << std::endl;
		Pause();
	}


	// END-OF-PROGRAM

	return 0;
}
