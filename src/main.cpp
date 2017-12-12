#include <iostream>
#include <vector>
#include <optional>
#include "komachi_resolver/komachi_resolver.hpp"

int main(int argc, char* argv[]){
	std::vector<int> values;
	for(int i = 1; i < argc - 1; i++ ){
		values.emplace_back( std::stoi(std::string(argv[i])) );
	}
	int answer = std::stoi( argv[argc - 1] );
	

	auto result = komachi_resolver::komachi_resolver(values, answer);
	if(result){
		for(auto&& e : *result){
			std::cout << e << " ";
		}

		std::cout << "=" << answer  << std::endl;	
	}else{
		std::cout << "no match" << std::endl;
	}

	return 0;
}
