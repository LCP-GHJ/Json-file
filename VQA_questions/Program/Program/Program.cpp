// Program.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json/json.h"

using namespace std;

typedef struct Questions
{
	int image_id;
	string question;
	int question_id;
}Questions;

int Find(vector<int> image, int image_id)
{
	for (int i = 0; i < image.size(); i++)
		if (image[i] == image_id)
			return i;
	return -1;
}

void GetInformation(vector<int> imageID)
{
	//ifstream infile_json("txt/v2_OpenEnded_mscoco_train2014_questions.json");
	//ofstream outfile("txt/train2014_questions.json");
	ifstream infile_json("txt/v2_OpenEnded_mscoco_val2014_questions.json");
	ofstream outfile("txt/val2014_questions.json");
	//ifstream infile_json("txt/v2_OpenEnded_mscoco_test2015_questions.json");
	//ofstream outfile("txt/test.json");
	vector<Questions> questions;
	int last_imageid = 0;

	Json::Reader reader;
	Json::Value root;
	if(reader.parse(infile_json, root))
	{
		for(int i = 0; i < root["questions"].size(); i++)
		{
			int image_id = root["questions"][i]["image_id"].asInt();
			if(last_imageid == image_id || Find(imageID, image_id) != -1)
			{
				cout << image_id << " ";
				Questions ques;
				ques.image_id = root["questions"][i]["image_id"].asInt();
				ques.question = root["questions"][i]["question"].asString();
				ques.question_id = root["questions"][i]["question_id"].asInt();
				questions.push_back(ques);
				last_imageid = image_id;
			}
		}
	}
	Json::Value root_out;
	for(int i = 0; i < questions.size(); i++)
	{
		Json::Value child;
		child["image_id"] = Json::Value(questions[i].image_id);
		child["question"] = Json::Value(questions[i].question);
		child["question_id"] = Json::Value(questions[i].question_id);
		root_out["questions"].append(child);
	}
	//Json::StyledWriter sw;
	Json::FastWriter fw;
	outfile << fw.write(root_out);
	infile_json.close();
	outfile.close();
}

int main()
{
	vector<int> target;
	//ifstream infile("txt/human_train.txt");
	ifstream infile("txt/human_val.txt");

	char temp_char[10];
	while (infile.getline(temp_char, 10))
	{
		target.push_back(atoi(temp_char));
		//cout << target.back() << " ";
	}
	infile.close();
	GetInformation(target);
}
