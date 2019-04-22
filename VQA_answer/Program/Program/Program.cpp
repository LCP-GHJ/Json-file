// Program.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json/json.h"

using namespace std;

typedef struct Annotations
{
	string question_type;
	string multiple_choice_answer;
	Json::Value answers;
	int image_id;
	string answer_type;
	int question_id;
}Annotations;

int Find(vector<int> image, int image_id)
{
	for (int i = 0; i < image.size(); i++)
		if (image[i] == image_id)
			return i;
	return -1;
}

void GetInformation(vector<int> imageID)
{
	ifstream infile_json("txt/train-annotations-part3.json");
	ofstream outfile("txt/train_annotations3.json");
	//ifstream infile_json("txt/val-annotation-part2.json");
	//ofstream outfile("txt/val_annotations2.json");
	vector<Annotations> annotations;
	int last_imageid = 0;

	//Json::Value root_out;
	Json::Reader reader;
	Json::Value root;
	if(reader.parse(infile_json, root))
	{
		for(int i = 0; i < root["annotations"].size(); i++)
		{
			int image_id = root["annotations"][i]["image_id"].asInt();
			if(last_imageid == image_id || Find(imageID, image_id) != -1)
			{
				cout << image_id << " ";
				Annotations anno;
				anno.question_type			= root["annotations"][i]["question_type"].asString();
				anno.multiple_choice_answer = root["annotations"][i]["multiple_choice_answer"].asString();
				for (int j = 0; j < root["annotations"][i]["answers"].size(); j++)
					anno.answers.append(root["annotations"][i]["answers"][j]);
				anno.image_id = root["annotations"][i]["image_id"].asInt();
				anno.answer_type			= root["annotations"][i]["answer_type"].asString();
				anno.question_id			= root["annotations"][i]["question_id"].asInt();
				annotations.push_back(anno);
				//root_out["annotations"].append(root["annotations"][i]);
				last_imageid = image_id;
			}
		}
	}
	Json::Value root_out;
	for(int i = 0; i < annotations.size(); i++)
	{
		Json::Value child;
		child["question_type"]			= Json::Value(annotations[i].question_type);
		child["multiple_choice_answer"] = Json::Value(annotations[i].multiple_choice_answer);
		child["answers"]				= annotations[i].answers;
		child["image_id"]				= Json::Value(annotations[i].image_id);
		child["answer_type"]			= Json::Value(annotations[i].answer_type);
		child["question_id"]			= Json::Value(annotations[i].question_id);
		root_out["annotations"].append(child);
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
	ifstream infile("txt/human_train.txt");
	//ifstream infile("txt/human_val.txt");

	char temp_char[10];
	while (infile.getline(temp_char, 10))
	{
		target.push_back(atoi(temp_char));
		//cout << target.back() << " ";
	}
	infile.close();
	GetInformation(target);
}
