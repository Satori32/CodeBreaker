#include <iostream>

#include "CodeBreakerSystem.h"


int main() {

	CodeBreakerSystem CBS;

	CodeBreakerSystem::SheardCodeBreakerAI H = std::make_shared<HumanPlayer>();
	CodeBreakerSystem::SheardCodeBreakerAI A = std::make_shared<RandomPlayer>();
	CodeBreakerSystem::SheardCodeBreakerAI B = std::make_shared<RandomPlayer>();

	std::random_device rd;
	std::mt19937 mt(rd());

	CBS.SetAI_A(H);
	//CBS.SetAI_A(A);
	CBS.SetAI_B(B);

	CBS.Initialize(3, 9, mt);

	while (!CBS.IsEnd()) {
		CBS.Game();
	}
	std::cout << "--- Game Set!---" << std::endl;
	

	std::cout << "Answer is ";
	CodeBreakerSystem::Box An = CBS.Answer();
	for (std::size_t i = 0; i<An.size(); i++) {
		std::cout << An[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "Winner is ";

	if (CBS.Winner() == 0) {
		std::cout << "Nothing." << std::endl;
	}
	if (CBS.Winner() == 1) {
		std::cout << CBS.GetA()->Name() <<"!!"<< std::endl;
	}
	if (CBS.Winner() == -1) {
		std::cout << CBS.GetB()->Name() <<"!!"<< std::endl;
	}
	return 0;
}