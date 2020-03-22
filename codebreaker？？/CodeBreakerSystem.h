#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <random>
#include <memory>
#include <tuple>

class CodeBreakerSystem {
public:
	typedef std::vector<std::int16_t> Box;
public:
	struct ICodeBreakerAI {
		virtual std::string Name(){ return "HogeAI"; }
		virtual bool Initialize(std::size_t L,std::size_t Radix) { return false; }
		virtual bool Think() { return false; }
		virtual CodeBreakerSystem::Box Answer() { return {}; }
		virtual bool Result(std::size_t A, std::size_t C) { return false; }
		virtual bool EnemyResult(std::size_t A, std::size_t C, CodeBreakerSystem::Box An) { return false; }
	};
	typedef std::shared_ptr<ICodeBreakerAI> SheardCodeBreakerAI;
public:
	CodeBreakerSystem() {}

	template<class r>
	bool Initialize(std::size_t L,std::size_t R, r& rnd) {
		
		std::uniform_int_distribution<> ui(1, R);

		std::cout << "Start Initialize..."<<std::endl;
		Answer_.clear();

		for (std::size_t i = 0; i < L; i++) {
			Answer_.push_back(ui(rnd));
		}
		
		Ra = R;
		W = 0;
		IsEnd_ = false;
		Turn = 1;

		std::cout << "Initialize AI A" << std::endl;
		A->Initialize(L,R);
		std::cout << "Initialize AI A ...Done" << std::endl;

		std::cout << "Initialize AI B" << std::endl;
		B->Initialize(L,R);
		std::cout << "Initialize AI B ...Done" << std::endl;

		std::cout << "End Initialize..."<<std::endl<<std::endl;
		return true;
	}
	bool Game() {
		std::cout <<"*Turn:"<< Turn++ << " is Start!*" << std::endl;
		A->Think();
		auto AA = Check(A,B);
		if (std::get<0>(AA) == Answer_.size()) {
			IsEnd_ = true;
			W++;
		}
		std::cin.clear();
		std::cout << std::endl;

		B->Think();
		auto BB = Check(B,A);
		if (std::get<0>(BB) == Answer_.size()) {
			IsEnd_ = true;
			W--;
		}
		std::cout << std::endl;
		std::cin.clear();

		std::cout << std::endl;
		return true;
	}

	bool SetAI_A(SheardCodeBreakerAI AI) {
		A = AI;
		return true;
	}
	bool SetAI_B(SheardCodeBreakerAI AI) {
		B  = AI;
		return true;
	}
	bool IsEnd() {
		return IsEnd_;
	}

	std::int8_t Winner() {
		return W;
	}

	std::size_t Radix() {
		return Ra;
	}

	Box Answer() {
		return Answer_;
	}

	SheardCodeBreakerAI GetA() {
		return A;
	}
	SheardCodeBreakerAI GetB() {
		return B;
	}

protected:
	std::tuple<std::size_t,std::size_t> Check(SheardCodeBreakerAI A,SheardCodeBreakerAI B) {
		auto AA = A->Answer();
		std::size_t C = 0;
		std::size_t H = 0;
		if (AA.size() != Answer_.size()) { return { 0,0 }; }

		std::cout << A->Name() << " is Answer:";
		for (std::size_t i = 0; i < AA.size(); i++) {
			std::cout << AA[i] << " ";
		}
		std::cout << std::endl;

		for (std::size_t i = 0; i < Answer_.size(); i++) {
			for (std::size_t j = 0; j < Answer_.size(); j++) {
				if (i == j) { continue; }
				if (Answer_[i] == AA[j]) {
					C++;
				}
			}
		}

		for (std::size_t i = 0; i < Answer_.size(); i++) {
			if (Answer_[i] == AA[i]) { H++; }
		}

		A->Result(H, C);
		B->EnemyResult(H, C, AA);
		std::cout << A->Name() << " is " << H << "Hit!" << C << "Blow!!" << std::endl;
		if (H == Answer_.size()) { std::cout << "Complete!" << std::endl; }

		return {H,C};
	}

protected:
	Box Answer_;
	bool IsEnd_ = false;
	std::size_t Turn = 0;
	std::int8_t W = 0;
	std::size_t Ra = 9;
	SheardCodeBreakerAI A = std::make_shared<ICodeBreakerAI>();
	SheardCodeBreakerAI B = std::make_shared<ICodeBreakerAI>();
};

class RandomPlayer : public CodeBreakerSystem::ICodeBreakerAI {
public:
	std::string Name() { return "RandomAIPlayer"; }
	bool Initialize(std::size_t L,std::size_t Radix) {
		this->L = L;
		this->R = Radix;
		return true;
	}
	bool Think() {
		std::random_device rd;
		std::uniform_int_distribution<> ui(1, R);

		A.clear();
		for (std::size_t i = 0; i < L; i++) {
			A.push_back(ui(rd));
		}

		return true;
	}

	CodeBreakerSystem::Box Answer() {
		return A;
	}
protected:
	std::size_t L = 0;
	std::size_t R = 0;
	CodeBreakerSystem::Box A;
};
class HumanPlayer : public CodeBreakerSystem::ICodeBreakerAI {
public:
	std::string Name() {
		return N;
	}
	bool Initialize(std::size_t L,std::size_t Radix){
		this->L = L;
		this->R = Radix;

		std::cout << "Set Your Name!" << std::endl;
		std::getline(std::cin, N);

		std::cout << "Answer Is " << L << " Digit." << std::endl;
		std::cout << "Answer Range:1-" << R << "." << std::endl;
		return true;
	}
	bool Think() {
		std::cout << "Type Answer. Numbers is separate by space." << std::endl;
		A.clear();

		CodeBreakerSystem::Box::value_type N = 0;

		while (std::cin >> N) {
			A.push_back(N);
		}

		std::cin.clear();

		return true;
	}

	CodeBreakerSystem::Box Answer() {
		return A;
	}

protected:
	std::string N = "HumanPlayer";
	std::size_t L = 0;
	std::size_t R = 0;
	CodeBreakerSystem::Box A;
};