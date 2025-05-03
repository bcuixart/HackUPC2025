#include "Test.h"

using namespace std;

int currentCorrectIndex = -1;

bool GetQuestionIsCorrect(int questionIndex, int buttonIndex) 
{
	return buttonIndex == currentCorrectIndex;
}

TestQuestion TestQuestionProcedure(int index) 
{
	TestQuestion question = questions[index];

	vector<string> answers(3);
	answers[0] = question.correctAnswer;
	answers[1] = question.incorrectAnswer1;
	answers[2] = question.incorrectAnswer2;

	std::random_device rd;
    std::mt19937 gen(rd());
	std::shuffle(answers.begin(), answers.end(), gen);

	cerr << answers[0] << ' ' << answers[1] << ' ' << answers[2] << endl;

	for (int i = 0; i < 3; ++i) {
		if (answers[i] == question.correctAnswer) {
			currentCorrectIndex = i;
		}
	}

	cout << currentCorrectIndex << endl;

	question.button0 = answers[0];
	question.button1 = answers[1];
	question.button2 = answers[2];

	return question;
}

void StartTest() 
{
	int questionSize = questions.size();

	vector<int> indices(questionSize);
	for (int i = 0; i < questionSize; ++i) 
		indices[i] = i;

	std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(indices.begin(), indices.end(), gen);
}