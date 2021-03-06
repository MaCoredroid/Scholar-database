

#include "pch.h"
#include <iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include<fstream>
#include <cstdlib>
#include <sstream>
struct author
{
	std::string name = "";
	std::string org = "";

};
struct article
{
	std::string id = "";
	std::string title = "";
	std::vector<author> authors;
	std::string venue;
	int year = 0;
	std::vector<std::string> keywords;
	std::vector<std::string> foses;
	int n_citation = 0;
	std::vector<std::string> references;
	std::string page_start = "";
	std::string page_end = "";
	std::string lang = "";
	std::string publisher = "";
	std::string volume = "";
	std::string issue = "";
	std::string issn = "";
	std::string isbn = "";
	std::string doi = "";
	std::string pdf = "";
	std::vector<std::string> urls;
	std::string abstract = "";


};

void readarticle(std::vector<std::string> &tokens, article &current, std::unordered_map<std::string, std::vector<std::string> > &authornametoids)
{
	current.id = tokens[3];
	for (int i = 5; i < tokens.size(); i++)
	{
		if (tokens[i] == "title")
		{
			current.title = tokens[i + 2];
			i = i + 3;
			continue;
		}
		if (tokens[i] == "authors")
		{
			while (tokens[i + 2] == "name")
			{
				author temp;
				temp.name = tokens[i + 4];
				if (tokens[i + 6] == "org")
				{
					temp.org = tokens[i + 8];
					i = i + 8;
				}
				else
				{
					i = i + 4;
				}

				current.authors.push_back(temp);
				if (i + 2 >= tokens.size())
				{
					break;
				}
			}
			i++;
			continue;
		}
		if (tokens[i] == "year")
		{
			current.year = std::stoi(tokens[i + 1].substr(2, 5));
			i++;
			continue;
		}
		if (tokens[i] == "keywords")
		{
			while (tokens[i + 1] != "], " &&tokens[i + 1] != "]}")
			{
				current.keywords.push_back(tokens[i + 2]);
				i = i + 2;
				if (i + 1 >= tokens.size())
				{
					break;
				}
			}
			i++;
			continue;
		}
		if (tokens[i] == "fos")
		{
			while (tokens[i + 1] != "], " &&tokens[i + 1] != "]}")
			{
				current.foses.push_back(tokens[i + 2]);
				i = i + 2;
				if (i + 1 >= tokens.size())
				{
					break;
				}
			}
			i++;
			continue;
		}
		if (tokens[i] == "n_citation")
		{
			current.n_citation = std::stoi(tokens[i + 1].substr(2, 5));
			i++;
			continue;
		}
		if (tokens[i] == "references")
		{
			while (tokens[i + 1] != "], "&&tokens[i + 1] != "]}")
			{
				current.references.push_back(tokens[i + 2]);
				i = i + 2;
				if (i + 1 >= tokens.size())
				{
					break;
				}
			}
			i++;
			continue;
		}
		if (tokens[i] == "lang")
		{
			current.lang = tokens[i + 2];
			i++;
			continue;
		}
		if (tokens[i] == "publisher")
		{
			current.publisher = tokens[i + 2];
			i++;
			continue;
		}
		if (tokens[i] == "issue")
		{
			current.issue = tokens[i + 2];
			i++;
			continue;
		}
		if (tokens[i] == "volume")
		{
			current.volume = tokens[i + 2];
			i++;
			continue;
		}
		if (tokens[i] == "issn")
		{
			current.issn = tokens[i + 2];
			i++;
			continue;
		}
		if (tokens[i] == "isbn")
		{
			current.isbn = tokens[i + 2];
			i++;
			continue;
		}
		if (tokens[i] == "doi")
		{
			current.doi = tokens[i + 2];
			i++;
			continue;
		}
		if (tokens[i] == "pdf")
		{
			current.pdf = tokens[i + 2];
			i++;
			continue;
		}
		if (tokens[i] == "url")
		{
			while (tokens[i + 1] != "], " &&tokens[i + 1] != "]}")
			{
				current.urls.push_back(tokens[i + 2]);
				i = i + 2;
				if (i + 1 >= tokens.size())
				{
					break;
				}
			}
			i++;
			continue;
		}
		if (tokens[i] == "abstract")
		{
			current.abstract = tokens[i + 2];
			i++;
			continue;
		}



	}
}
void readauthors(std::vector<std::string>&tokens, std::vector<std::string>& authors)
{
	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "authors")
		{
			while (tokens[i + 2] == "name")
			{
				authors.push_back(tokens[i + 4]);
				if (tokens[i + 6] == "org")
				{
					i = i + 8;
				}
				else
				{
					i = i + 4;
				}
				if (i + 2 >= tokens.size())
				{
					break;
				}
			}
			break;
		}
	}
}
void readdata(std::ifstream &ifs, std::unordered_map<std::string, std::streampos> &idtoarticle, std::unordered_map<std::string, std::vector<std::string> > &authornametoids)
{
	if (ifs.is_open())
	{
		while (!ifs.eof())
		{
			std::string temp = "";
			std::streampos pos = ifs.tellg();
			std::getline(ifs, temp);
			std::vector<std::string> tokens;
			std::vector<std::string> authors;
			std::string token = "";
			std::istringstream tokenStream(temp);
			while (std::getline(tokenStream, token, '\"'))
			{
				tokens.push_back(token);
			}
			std::string id;
			if (tokens.size() > 4)
			{
				id = tokens[3];
			}
			else
			{
				continue;
			}
			readauthors(tokens, authors);
			for (int i = 0; i < authors.size(); i++)
			{
				authornametoids[authors[i]].push_back(id);
			}
			idtoarticle[id] = pos;
		}



	}
	else
	{
		return;
	}
}

void showtimeline(std::string inputauthor, std::unordered_map<std::string, std::streampos > &idtoarticle, std::unordered_map<std::string, std::vector<std::string>> &authornametoids)
{
	std::cout << "Timeline:" << std::endl;
	std::vector<article> articles;
	for (int i = 0; i < authornametoids[inputauthor].size(); i++)
	{
		std::streampos temppos = idtoarticle[authornametoids[inputauthor][i]];
		std::ifstream ifs("aminer_papers_0.txt", std::ios::in | std::ios::binary);
		ifs.seekg(temppos);
		std::string temp = "";
		std::getline(ifs, temp);
		std::vector<std::string> tokens;
		std::string token = "";
		std::istringstream tokenStream(temp);
		while (std::getline(tokenStream, token, '\"'))
		{
			tokens.push_back(token);
		}
		article current;
		readarticle(tokens, current, authornametoids);
		articles.push_back(current);
	}
	std::map<int, std::vector<std::string>> yeartoarticles;
	for (int i = 0; i < articles.size(); i++)
	{
		yeartoarticles[articles[i].year].push_back(articles[i].title);
	}
	std::map<int, std::vector<std::string>>::iterator it;
	for (it = yeartoarticles.begin(); it != yeartoarticles.end(); it++)
	{
		for (int i = 0; i < it->second.size(); i++)
		{
			std::cout << it->second[i] << " " << it->first << std::endl;
		}
	}




}

void findcitations(int&num, std::vector<std::string> ids)
{
	std::ifstream ifs("aminer_papers_0.txt", std::ios::in | std::ios::binary);
	if (ifs.is_open())
	{
		while (!ifs.eof())
		{
			std::string temp = "";
			std::streampos pos = ifs.tellg();
			std::getline(ifs, temp);
			std::vector<std::string> tokens;
			std::vector<std::string> authors;
			std::string token = "";
			std::istringstream tokenStream(temp);
			while (std::getline(tokenStream, token, '\"'))
			{
				tokens.push_back(token);
			}
			for (int i = 0; i < tokens.size(); i++)
			{
				if (tokens[i] == "references")
				{
					while (tokens[i + 1] != "], "&&tokens[i + 1] != "]}")
					{
						for (int j = 0; j < ids.size(); j++)
						{
							if (tokens[i + 2] == ids[j])
							{
								num++;
								break;
							}
						}
						i = i + 2;
						if (i + 1 >= tokens.size())
						{
							break;
						}
					}
					break;
				}
			}

		}



	}
	else
	{
		return;
	}
}

void showcitations(std::string inputauthor, std::unordered_map<std::string, std::streampos> &idtoarticle, std::unordered_map<std::string, std::vector<std::string> > &authornametoids)
{
	std::cout << std::endl;
	int num = 0;
	findcitations(num, authornametoids[inputauthor]);
	std::cout << "Citations:" << " " << num << std::endl;
}

void showtransitions(std::string inputauthor, std::unordered_map<std::string, std::streampos> &idtoarticle, std::unordered_map<std::string, std::vector<std::string> > &authornametoids)
{
	std::cout << std::endl;
	std::cout << "Transitions:" << std::endl;
	std::vector<article> articles;
	for (int i = 0; i < authornametoids[inputauthor].size(); i++)
	{
		std::streampos temppos = idtoarticle[authornametoids[inputauthor][i]];
		std::ifstream ifs("aminer_papers_0.txt", std::ios::in | std::ios::binary);
		ifs.seekg(temppos);
		std::string temp = "";
		std::getline(ifs, temp);
		std::vector<std::string> tokens;
		std::string token = "";
		std::istringstream tokenStream(temp);
		while (std::getline(tokenStream, token, '\"'))
		{
			tokens.push_back(token);
		}
		article current;
		readarticle(tokens, current, authornametoids);
		articles.push_back(current);
	}
	std::map<int, std::vector<std::string>> yeartoorgs;
	for (int i = 0; i < articles.size(); i++)
	{
		for (int j = 0; j < articles[i].authors.size(); j++)
		{
			if (articles[i].authors[j].name == inputauthor && articles[i].authors[j].org != "")
			{
				yeartoorgs[articles[i].year].push_back(articles[i].authors[j].org);
				break;
			}
		}
	}
	std::map<int, std::vector<std::string>>::iterator it;
	for (it = yeartoorgs.begin(); it != yeartoorgs.end(); it++)
	{
		for (int i = 0; i < it->second.size(); i++)
		{
			std::cout << it->second[i] << " " << it->first << std::endl;
		}
	}
}
int main()
{
	std::unordered_map<std::string, std::streampos> idtoarticle;
	std::unordered_map < std::string, std::vector<std::string>> authornametoids;
	std::ifstream ifs("aminer_papers_0.txt", std::ios::in | std::ios::binary);
	readdata(ifs, idtoarticle, authornametoids);
	std::string inputauthor = "";
	std::cout << "Please input author name:" << std::endl;
	std::getline(std::cin, inputauthor);
	std::vector<article> articles;
	for (int i = 0; i < authornametoids[inputauthor].size(); i++)
	{
		std::streampos temppos = idtoarticle[authornametoids[inputauthor][i]];
		std::ifstream ifs("aminer_papers_0.txt", std::ios::in | std::ios::binary);
		ifs.seekg(temppos);
		std::string temp = "";
		std::getline(ifs, temp);
		std::vector<std::string> tokens;
		std::string token = "";
		std::istringstream tokenStream(temp);
		while (std::getline(tokenStream, token, '\"'))
		{
			tokens.push_back(token);
		}
		article current;
		readarticle(tokens, current, authornametoids);
		articles.push_back(current);

	}
	showtimeline(inputauthor, idtoarticle, authornametoids);
	showcitations(inputauthor, idtoarticle, authornametoids);
	showtransitions(inputauthor, idtoarticle, authornametoids);

}

