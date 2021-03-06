/*
	Author: Anh-Dzung Doan
*/

#include "CreateSparseTrajectory.h"

std::vector<Vertex> _vertex_list;

bool readVertexList(std::string vertex_file_text)
{
	std::ifstream ifile(vertex_file_text);
	if (!ifile.is_open())
	{
		std::cout << "Unable to load vertex list file" << std::endl;
		return false;
	}

	while (!ifile.eof())
	{
		Vertex v;
		ifile >> v.x;
		ifile >> v.y;
		ifile >> v.z;
		_vertex_list.push_back(v);
	}

	_vertex_list.pop_back();
	ifile.close();
	return true;
}

void parseOrderLine(std::vector<uint32_t> &v, std::string str_line)
{
	v.clear();
	if (str_line.empty()) return;

	int s_idx, e_idx;
	s_idx = 0;
	for (int i = 0; i < str_line.size(); i++)
	{
		if (str_line.at(i) == ',')
		{
			std::string s = str_line.substr(s_idx, i - s_idx);
			uint32_t order = std::stoul(s);
			v.push_back(order);
			s_idx = i + 1;
		}
		else
			if (i == str_line.size() - 1)
			{
				std::string s = str_line.substr(s_idx, i - s_idx + 1);
				uint32_t order = std::stoul(s);
				v.push_back(order);
				break;
			}
	}
}

bool readVertexOrder(std::string vertex_order_file_text)
{
	std::ifstream ifile(vertex_order_file_text);
	if (!ifile.is_open())
	{
		std::cout << "Unable to load vertex order file" << std::endl;
		return false;
	}

	int ii = 0;

	while (!ifile.eof())
	{
		std::string str;
		std::getline(ifile, str);
		parseOrderLine(_vertex_list[ii].order, str);
		ii++;
		if (ii == _vertex_list.size()) break;
	}
	ifile.close();
	return true;
}

void displayVertexInfo()
{
	for (int i = 0; i < _vertex_list.size(); i++)
	{
		std::cout << "vertex " << i << std::endl;
		std::cout << "x = " << _vertex_list[i].x << ", y = " << _vertex_list[i].y << ", z = " << _vertex_list[i].z << std::endl;
		std::cout << "Orders (Size = " << _vertex_list[i].order.size() << ") = ";
		for (int k = 0; k < _vertex_list[i].order.size(); k++)
			std::cout << _vertex_list[i].order[k] << " ";

		std::cout << std::endl;
		std::cout << "================================" << std::endl << std::endl;
	}

}

bool createTrajectory(std::string trajectory_file_text)
{
	std::ofstream ofile;
	ofile.open(trajectory_file_text, std::ios_base::out);

	if (!ofile.is_open())
	{
		std::cout << "Unable to create trajectory file" << std::endl;
		return false;
	}
	int trajectory_index = 1;
	int vertex_index;
	std::cout << "CREATING TRAJECTORY....";
	do
	{
		vertex_index = -1;
		for (int ii = 0; ii < _vertex_list.size(); ii++)
		{
			for (int k = 0; k < _vertex_list[ii].order.size(); k++)
				if (trajectory_index == _vertex_list[ii].order[k])
				{
					vertex_index = ii;
					
					ofile << _vertex_list[ii].x << " " << _vertex_list[ii].y << " " << _vertex_list[ii].z << std::endl;
					trajectory_index++;
					break;
				}

		}

	} while (vertex_index != -1);
	std::cout << "FINISHED" << std::endl;
	return true;

}

int main(int argc, char* argv[])
{
	_vertex_list.clear();
	std::string vertex_file_text, vertex_order_file_text, trajectory_file_text;
	if (argc == 1)
	{
		std::cout << "This program creates sparse trajectory from vertex and vertex order" << std::endl;
		std::cout << "How to use: " << std::endl;
		std::cout << "CreateSparseTrajectory [vertex file] [vertex order file] [sparse trajectory file]" << std::endl;
		return 0;
	}
	if (argc < 4)
	{
		std::cout << "Not enought parameters" << std::endl;
		return 0;
	}

	vertex_file_text = argv[1];
	vertex_order_file_text = argv[2];
	trajectory_file_text = argv[3];

	if (readVertexList(vertex_file_text) == false)
		return 0;

	if (readVertexOrder(vertex_order_file_text) == false)
		return 0;

	// Use for debug, to see whether we can read the input files
	//display_vertex_info();

	if (createTrajectory(trajectory_file_text) == false)
		return 0;
	return 0;
}

