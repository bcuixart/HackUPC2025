#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <QTimer>
#include <random>
#include <algorithm>

using namespace std;

struct TestQuestion {
	string question;
	string correctAnswer;
	string incorrectAnswer1;
	string incorrectAnswer2;

	string button0;
	string button1;
	string button2;

	glm::vec3 camPos;
	glm::vec3 camRot;
	float camDist;
};

void StartTest();

const vector<TestQuestion> questions = {
	{"Question1", "Correct", "Incorrect", "Incorrect", "", "", "", glm::vec3(0,0,0), glm::vec3(0,0,0), 5 },
	{"Question2", "Correct", "Incorrect", "Incorrect", "", "", "", glm::vec3(0,0,0), glm::vec3(0,0,0), 5 },
	{"Question3", "Correct", "Incorrect", "Incorrect", "", "", "", glm::vec3(0,0,0), glm::vec3(0,0,0), 5 },
	{"Question4", "Correct", "Incorrect", "Incorrect", "", "", "", glm::vec3(0,0,0), glm::vec3(0,0,0), 5 },
	{"Question5", "Correct", "Incorrect", "Incorrect", "", "", "", glm::vec3(0,0,0), glm::vec3(0,0,0), 5 },
	{"Question6", "Correct", "Incorrect", "Incorrect", "", "", "", glm::vec3(0,0,0), glm::vec3(0,0,0), 5 },
	{"Question7", "Correct", "Incorrect", "Incorrect", "", "", "", glm::vec3(0,0,0), glm::vec3(0,0,0), 5 },
	{"Question8", "Correct", "Incorrect", "Incorrect", "", "", "", glm::vec3(0,0,0), glm::vec3(0,0,0), 5 },
	{"Question9", "Correct", "Incorrect", "Incorrect", "", "", "", glm::vec3(0,0,0), glm::vec3(0,0,0), 5 },
};

TestQuestion TestQuestionProcedure(int index);

bool GetQuestionIsCorrect(int questionIndex, int buttonIndex);