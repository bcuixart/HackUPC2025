#include "Test.h"

using namespace std;

void TestQuestionProcedure(int index) 
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

	cerr << "dorga" << endl;
}