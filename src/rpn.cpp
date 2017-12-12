#include <iostream>
#include <stack>
#include <vector>
#include <optional>
#include <string>
#include <cstddef>
#include "symbol.hpp"

template<typename T>
std::optional<T> stack_result(const std::vector<momoka::Symbol<T>>& expression){
	// 基本的な逆ポーランド記法の演算の実装
	// wikiマン
	using namespace momoka;
	std::stack<T> stack;
	T accumulator = 0;

	for(auto&& e : expression){
		if( e.has_data() ){
			stack.push(e.get_data());
		}else{
			accumulator = stack.top();
			stack.pop();
			switch(e.get_operator_symbol()){
				case operator_symbol::plus :
					accumulator += stack.top();
					break;
				case operator_symbol::minus :
					accumulator = stack.top() - accumulator;
					break;
				case operator_symbol::times :
					accumulator *= stack.top();
					break;
				case operator_symbol::divide : 		
					// 0除算が発生した場合は計算失敗
					if( accumulator == 0 ){
						return std::nullopt;
					}

					accumulator = stack.top() / accumulator;
					break;
			}
			stack.pop();
			stack.push(accumulator);
		}
	}

	return std::make_optional( stack.top() );
}

template<typename T>
std::vector<momoka::Symbol<T>> expression_to_revers_polish_notation(const std::vector<momoka::Symbol<T>>& expression){
	// 通常の式を逆ポーランド記法にする
	using namespace momoka;
	std::stack<operator_symbol> operator_stack;
	std::vector<Symbol<T>> rpn;

	for(auto&& e : expression){
		if( e.has_data() ){
			rpn.emplace_back( e.get_data());
		}else{
			if( !operator_stack.empty() && operator_stack.top() < e.get_operator_symbol() ){
				while( !operator_stack.empty() ){
					rpn.emplace_back( operator_stack.top() );
					operator_stack.pop();
				}
			}

			operator_stack.push( e.get_operator_symbol() );
		}
	}

	return rpn;
}

template<typename T>
std::optional<std::vector<momoka::Symbol<T>>> komachi_resolver(const std::vector<T>& values, const T& answer){
	using namespace momoka;
	std::vector<operator_symbol> operators{
		operator_symbol::plus,
		operator_symbol::minus,
		operator_symbol::times,
		operator_symbol::divide
	};

	// 与えられた数-1分の演算子を用意
	std::vector<std::size_t> operator_index( values.size() - 1, 0 );

	while(true){
		// 連結
		std::vector<Symbol<T>> expression;
		for(std::size_t i = 0u; i < values.size(); i++){
			expression.emplace_back( values[i] );
			// 最後じゃなければ式を格納
			if( i != values.size() - 1 ){
				expression.emplace_back( operators[ operator_index[i] ] );
			}
		}

		// 答えがあうかね？
		auto result = stack_result( expression_to_revers_polish_notation( expression ));
		if( result && *result == answer ){
			// 合えばvectorを返す
			return expression;
		}

		// あってない場合は1ずつ繰り上げ
		bool is_break = false;
		for(std::size_t i = 0; i < operator_index.size(); i++){
			operator_index[i] += 1;
			if( operator_index[i] >= operators.size() ){
				// 最後のタイミングならやめ
				if( i == operator_index.size() - 1 ){
					is_break = true;
					break;
				}
				operator_index[i] = 0;
			}else{
				break;
			}
		}

		if(is_break){
			break;
		}
	}

	return std::nullopt;
}

int main(int argc, char* argv[]){
	using namespace momoka;
	std::vector<int> values;
	for(int i = 1; i < argc - 1; i++ ){
		values.emplace_back( std::stoi(std::string(argv[i])) );
	}
	int answer = std::stoi( argv[argc - 1] );
	
	auto result = komachi_resolver(values, answer);
	if(result){
		for(auto&& e : *result){
			if( e.has_data() ){
				std::cout << e.get_data() << " ";		
			}else{
				std::cout << e.get_operator_symbol()  << " ";
			}
		}

		std::cout << "=" << answer  << std::endl;	
	}else{
		std::cout << "no match" << std::endl;
	}

	return 0;
}
